#pragma once

#include <memory>
#include <optional>
#include <type_traits>

#include <Platform/Macros/IsPlatform.h>

#if IS_WIN32() || IS_WIN64()
#include <d3d9.h>
#include <Windows.h>
#elif IS_LINUX()
#include "SdlFunctions.h"
struct SDL_Window;
union SDL_Event;
#endif

#include "Hooks/MinHook.h"
#include "Hooks/VmtSwap.h"
#include "Memory.h"
#include "InventoryChanger/InventoryChanger.h"
#include "Hooks/ClientHooks.h"
#include "Hooks/ClientStateHooks.h"
#include "Hooks/ClientModeHooks.h"
#include "Hooks/EngineHooks.h"
#include "Hooks/PanoramaMarshallHelperHooks.h"
#include "Hooks/ViewRenderHooks.h"
#include "Hooks/CSPlayerInventoryHooks.h"
#include "Hooks/InventoryManagerHooks.h"
#include "Hooks/BspQueryHooks.h"
#include "Hooks/EngineSoundHooks.h"
#include "Hooks/SvCheatsHooks.h"
#include "Hooks/ModelRenderHooks.h"
#include "Hooks/SurfaceHooks.h"

#if IS_WIN32() || IS_WIN64()
#include "Hooks/KeyValuesSystemHooks.h"
#include "Hooks/WindowProcedureHook.h"
#endif

#include "HookType.h"

namespace csgo
{

struct matrix3x4;
struct ModelRenderInfo;
struct SoundInfo;
struct ClientPOD;

}

class ClientInterfaces;
class OtherInterfaces;
class Glow;
class Visuals;
class Misc;

#if IS_LINUX()

int pollEvent(SDL_Event* event) noexcept;

#endif

class Hooks {
public:
#if IS_WIN32() || IS_WIN64()
    template <typename PlatformApi>
    Hooks(HMODULE moduleHandle, DynamicLibrary<PlatformApi> clientDll, DynamicLibrary<PlatformApi> engineDll, DynamicLibrary<PlatformApi> vstdlibDll, DynamicLibrary<PlatformApi> vguiMatSurfaceDll) noexcept
        : windowProcedureHook{ FindWindowW(L"Valve001", nullptr) }
        , keyValuesSystemHooks{ vstdlibDll.getCodeSection() }
        , engineHooks{ engineDll.getCodeSection() }
        , clientHooks{ clientDll.getCodeSection() }
        , clientModeHooks{ clientDll.getCodeSection() }
        , clientStateHooks{ engineDll.getCodeSection() }
        , playerInventoryHooks{ clientDll.getCodeSection() }
        , panoramaMarshallHelperHooks{ clientDll.getCodeSection() }
        , viewRenderHooks{ clientDll.getCodeSection() }
        , inventoryManagerHooks{ clientDll.getCodeSection() }
        , bspQueryHooks{ engineDll.getCodeSection() }
        , engineSoundHooks{ engineDll.getCodeSection() }
        , svCheatsHooks{ engineDll.getCodeSection() }
        , modelRenderHooks{ engineDll.getCodeSection() }
        , surfaceHooks{ vguiMatSurfaceDll.getCodeSection() }
        , moduleHandle{ moduleHandle }
    {
    }

    WindowProcedureHook windowProcedureHook;
    std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> originalPresent;
    std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> originalReset;
#elif IS_LINUX()
    template <typename PlatformApi>
    Hooks(DynamicLibrary<PlatformApi> clientDll, DynamicLibrary<PlatformApi> engineDll, DynamicLibrary<PlatformApi> vstdlibDll, DynamicLibrary<PlatformApi> vguiMatSurfaceDll) noexcept
        : sdlFunctions{ DynamicLibrary<PlatformApi>{ "libSDL2-2.0.so.0" } }
        , engineHooks{ engineDll.getCodeSection() }
        , clientHooks{ clientDll.getCodeSection() }
        , clientModeHooks{ clientDll.getCodeSection() }
        , clientStateHooks{ engineDll.getCodeSection() }
        , playerInventoryHooks{ clientDll.getCodeSection() }
        , panoramaMarshallHelperHooks{ clientDll.getCodeSection() }
        , viewRenderHooks{ clientDll.getCodeSection() }
        , inventoryManagerHooks{ clientDll.getCodeSection() }
        , bspQueryHooks{ engineDll.getCodeSection() }
        , engineSoundHooks{ engineDll.getCodeSection() }
        , svCheatsHooks{ engineDll.getCodeSection() }
        , modelRenderHooks{ engineDll.getCodeSection() }
        , surfaceHooks{ vguiMatSurfaceDll.getCodeSection() }
    {
        pollEvent = *reinterpret_cast<decltype(pollEvent)*>(sdlFunctions.pollEvent);
        *reinterpret_cast<decltype(::pollEvent)**>(sdlFunctions.pollEvent) = ::pollEvent;
    }

    SdlFunctions sdlFunctions;

    std::add_pointer_t<int(SDL_Event*)> pollEvent;
    std::add_pointer_t<void(SDL_Window*)> swapWindow;
#endif

    void install(csgo::ClientPOD* clientInterface, const EngineInterfaces& engineInterfaces, const OtherInterfaces& interfaces, const Memory& memory) noexcept;
    void uninstall(Misc& misc, Glow& glow, const Memory& memory, Visuals& visuals, inventory_changer::InventoryChanger& inventoryChanger) noexcept;

#if IS_WIN32()
    KeyValuesSystemHooks keyValuesSystemHooks;
#endif

    EngineHooks engineHooks;
    ClientHooks clientHooks;
    ClientModeHooks clientModeHooks;
    ClientStateHooks clientStateHooks;
    CSPlayerInventoryHooks playerInventoryHooks;
    PanoramaMarshallHelperHooks panoramaMarshallHelperHooks;
    ViewRenderHooks viewRenderHooks;
    InventoryManagerHooks inventoryManagerHooks;
    BspQueryHooks bspQueryHooks;
    EngineSoundHooks engineSoundHooks;
    SvCheatsHooks svCheatsHooks;
    ModelRenderHooks modelRenderHooks;
    SurfaceHooks surfaceHooks;

private:
#if IS_WIN32() || IS_WIN64()
    HMODULE moduleHandle;
#endif
};

inline std::optional<Hooks> hooks;
