#pragma once

#include <Platform/DynamicLibrary.h>
#include <Platform/Macros/IsPlatform.h>

#include <RetSpoof/RetSpoofGadgets.h>
#include <CSGO/Constants/DllNames.h>
#include <CSGO/Constants/InterfaceNames.h>

#include "Interfaces/InterfaceFinder.h"
#include "Interfaces/InterfaceFinderWithLog.h"

namespace csgo
{
    struct BaseFileSystemPOD;
    struct CvarPOD;
    struct InputSystemPOD;
    struct LocalizePOD;
    struct MaterialSystemPOD;
    struct PanoramaUIEnginePOD;
    struct PhysicsSurfacePropsPOD;
    struct SoundEmitterPOD;
    struct StudioRenderPOD;
    struct SurfacePOD;
}

struct OtherInterfacesPODs {
    template <typename PlatformApi>
    OtherInterfacesPODs(PlatformApi)
        : baseFileSystem{ static_cast<csgo::BaseFileSystemPOD*>(find<PlatformApi>(csgo::FILESYSTEM_DLL, csgo::i::VBaseFileSystem)) },
          cvar{ static_cast<csgo::CvarPOD*>(find<PlatformApi>(csgo::VSTDLIB_DLL, csgo::i::VEngineCvar)) },
          inputSystem{ static_cast<csgo::InputSystemPOD*>(find<PlatformApi>(csgo::INPUTSYSTEM_DLL, csgo::i::InputSystem)) },
          localize{ static_cast<csgo::LocalizePOD*>(find<PlatformApi>(csgo::LOCALIZE_DLL, csgo::i::Localize)) },
          materialSystem{ static_cast<csgo::MaterialSystemPOD*>(find<PlatformApi>(csgo::MATERIALSYSTEM_DLL, csgo::i::VMaterialSystem)) },
          panoramaUIEngine{ static_cast<csgo::PanoramaUIEnginePOD*>(find<PlatformApi>(csgo::PANORAMA_DLL, csgo::i::PanoramaUIEngine)) },
          physicsSurfaceProps{ static_cast<csgo::PhysicsSurfacePropsPOD*>(find<PlatformApi>(csgo::VPHYSICS_DLL, csgo::i::VPhysicsSurfaceProps)) },
          soundEmitter{ static_cast<csgo::SoundEmitterPOD*>(find<PlatformApi>(csgo::SOUNDEMITTERSYSTEM_DLL, csgo::i::VSoundEmitter)) },
          studioRender{ static_cast<csgo::StudioRenderPOD*>(find<PlatformApi>(csgo::STUDIORENDER_DLL, csgo::i::VStudioRender)) },
          surface{ static_cast<csgo::SurfacePOD*>(find<PlatformApi>(csgo::VGUIMATSURFACE_DLL, csgo::i::VGUI_Surface)) }
    {
    }

    csgo::BaseFileSystemPOD* baseFileSystem;
    csgo::CvarPOD* cvar;
    csgo::InputSystemPOD* inputSystem;
    csgo::LocalizePOD* localize;
    csgo::MaterialSystemPOD* materialSystem;
    csgo::PanoramaUIEnginePOD* panoramaUIEngine;
    csgo::PhysicsSurfacePropsPOD* physicsSurfaceProps;
    csgo::SoundEmitterPOD* soundEmitter;
    csgo::StudioRenderPOD* studioRender;
    csgo::SurfacePOD* surface;

private:
    template <typename PlatformApi>
    static void* find(const char* moduleName, const char* name) noexcept
    {
        const DynamicLibrary<PlatformApi> dll{ moduleName };
        const InterfaceFinderWithLog finder{ InterfaceFinder{ dll, retSpoofGadgets->client } };
        return finder(name);
    }
};
