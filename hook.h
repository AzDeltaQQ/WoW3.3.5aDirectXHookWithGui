#pragma once
#include <Windows.h>
#include <d3d9.h>

void InitializeHook();
HRESULT APIENTRY HookedEndScene(LPDIRECT3DDEVICE9 pDevice);
void CleanupHook(); 