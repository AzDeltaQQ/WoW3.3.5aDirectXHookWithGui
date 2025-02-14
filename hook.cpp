#include "hook.h"
#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <MinHook.h>

#pragma comment(lib, "d3d9.lib")

typedef HRESULT(APIENTRY* EndScene)(LPDIRECT3DDEVICE9 pDevice);
EndScene oEndScene = nullptr;

typedef HRESULT(APIENTRY* Reset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
Reset oReset = nullptr;

bool initialized = false;
bool show_demo_window = true;
bool show_gui = true;
bool is_cursor_visible = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WNDPROC oWndProc;
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (show_gui) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
            return true;
    }

    return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT APIENTRY HookedEndScene(LPDIRECT3DDEVICE9 pDevice) {
    if (!initialized) {
        D3DDEVICE_CREATION_PARAMETERS params;
        pDevice->GetCreationParameters(&params);

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(params.hFocusWindow);
        ImGui_ImplDX9_Init(pDevice);

        // Configure ImGui
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDrawCursor = false;  // Start with game cursor

        oWndProc = (WNDPROC)SetWindowLongPtr(params.hFocusWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

        initialized = true;
    }

    // Handle hotkey toggle (Insert key)
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        show_gui = !show_gui;
        // Toggle cursor visibility
        ImGui::GetIO().MouseDrawCursor = show_gui;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Only draw GUI if show_gui is true
    if (show_gui) {
        ImGui::Begin("WoW Hook", &show_demo_window);
        ImGui::Text("Hello from DirectX Hook!");
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return oEndScene(pDevice);
}

HRESULT APIENTRY HookedReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT result = oReset(pDevice, pPresentationParameters);
    if (SUCCEEDED(result)) {
        ImGui_ImplDX9_CreateDeviceObjects();
    }
    return result;
}

void InitializeHook() {
    // Initialize MinHook
    if (MH_Initialize() != MH_OK) {
        return;
    }

    // Get Direct3D device
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) return;

    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = GetForegroundWindow();
    d3dpp.Windowed = TRUE;

    IDirect3DDevice9* pDevice = nullptr;
    HRESULT result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice);

    if (FAILED(result) || !pDevice) {
        pD3D->Release();
        return;
    }

    void** vTable = *reinterpret_cast<void***>(pDevice);
    
    // Hook EndScene (index 42 in the vtable)
    if (MH_CreateHook(vTable[42], &HookedEndScene, reinterpret_cast<LPVOID*>(&oEndScene)) != MH_OK) {
        pDevice->Release();
        pD3D->Release();
        return;
    }

    // Hook Reset (index 16)
    if (MH_CreateHook(vTable[16], &HookedReset, reinterpret_cast<LPVOID*>(&oReset)) != MH_OK) {
        pDevice->Release();
        pD3D->Release();
        return;
    }

    // Enable both hooks
    if (MH_EnableHook(vTable[42]) != MH_OK || MH_EnableHook(vTable[16]) != MH_OK) {
        pDevice->Release();
        pD3D->Release();
        return;
    }

    pDevice->Release();
    pD3D->Release();
}

void CleanupHook() {
    if (initialized) {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
} 