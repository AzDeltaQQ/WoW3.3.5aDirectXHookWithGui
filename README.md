# WoW 3.3.5a DirectX 9 Hook

This project creates a DLL that can be injected into World of Warcraft 3.3.5a to display a custom ImGui interface.  It uses MinHook for hooking DirectX 9 functions.

## Prerequisites

-   Visual Studio 2019 or later
-   CMake 3.10 or later
-   DirectX 9 SDK (maybe idk) 
-   World of Warcraft 3.3.5a client (32-bit)

compile as 32 bit - noob. 

## Dependencies

The project requires the following external libraries (included as submodules):

-   MinHook
-   Dear ImGui

included : https://github.com/TsudaKageyu/minhook

included : https://github.com/ocornut/imgui
## Building

1.  Clone the repository with submodules:

    ```bash
    git clone --recursive https://github.com/AzDeltaQQ/WoW3.3.5aDirectXHookWithGui.git
    cd WoW3.3.5aDirectXHookWithGui
    ```

2.  Create a build directory and navigate to it:

    ```bash
    mkdir build
    cd build
    ```

3.  Generate the Visual Studio solution:

    ```bash
    cmake ..
    ```

4.  Open the generated solution in Visual Studio and build it
    -   Or build from command line:

    ```bash
    cmake --build . --config Release
    ```

## Picture
![Screenshot 2025-02-14 025529](https://github.com/user-attachments/assets/2933c926-00bf-4f3f-9588-8e1eab6d66d7)



## Injection Methods

Recommended injectors for development:

-   Cheat Engine ( make sure to rename https://www.unknowncheats.me/forum/anti-cheat-bypass/504191-undetected-cheat-engine-driver-2023-bypass-anticheats-eac.html )
-   Extreme Injector - User-friendly alternative (good alternative)
-   Xenos - Feature-rich injector (havent tried. heard good things)

## Usage

1.  Start World of Warcraft 3.3.5a.
2.  Use your preferred DLL injector to inject the compiled DLL into the WoW process.
3.  The GUI should appear in-game.
4. Win Key + Insert for Show / Hide gui . 

## Notes

-   This hook targets the DirectX 9 EndScene function.
-   The GUI can be toggled on/off (implementation pending).
-   Make sure to use a 32-bit DLL injector as WoW 3.3.5a is a 32-bit application.

## License

wtf is a license ?
