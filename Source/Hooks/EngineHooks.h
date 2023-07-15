#pragma once

#include <HookType.h>
#include <Platform/Macros/CallingConventions.h>
#include <Platform/Macros/PlatformSpecific.h>
#include <Vmt/VmtLengthCalculator.h>

namespace csgo { struct DemoPlaybackParameters; }
namespace csgo { struct EnginePOD; }

class EngineHooks {
public:
    explicit EngineHooks(const VmtLengthCalculator& vmtLengthCalculator)
        : hookImpl{ vmtLengthCalculator }
    {
    }
    
    void install(csgo::EnginePOD* engine)
    {
        hookImpl.install(*reinterpret_cast<std::uintptr_t**>(engine));

        originalIsPlayingDemo = reinterpret_cast<decltype(originalIsPlayingDemo)>(hookImpl.hook(82, std::uintptr_t(&isPlayingDemo)));
        originalGetScreenAspectRatio = reinterpret_cast<decltype(originalGetScreenAspectRatio)>(hookImpl.hook(101, std::uintptr_t(&getScreenAspectRatio)));
        originalGetDemoPlaybackParameters = reinterpret_cast<decltype(originalGetDemoPlaybackParameters)>(hookImpl.hook(WIN32_LINUX(218, 219), std::uintptr_t(&getDemoPlaybackParameters)));
    }

    void uninstall(csgo::EnginePOD* engine)
    {
        hookImpl.uninstall(*reinterpret_cast<std::uintptr_t**>(engine));
    }

    [[nodiscard]] auto getOriginalIsPlayingDemo() const
    {
        return FunctionInvoker{ retSpoofGadgets->client, originalIsPlayingDemo };
    }

    [[nodiscard]] auto getOriginalGetScreenAspectRatio() const
    {
        return FunctionInvoker{ retSpoofGadgets->client, originalGetScreenAspectRatio };
    }

    [[nodiscard]] auto getOriginalGetDemoPlaybackParameters() const
    {
        return FunctionInvoker{ retSpoofGadgets->client, originalGetDemoPlaybackParameters };
    }

    static bool FASTCALL_CONV isPlayingDemo(FASTCALL_THIS(csgo::EnginePOD* thisptr)) noexcept;
    static float FASTCALL_CONV getScreenAspectRatio(FASTCALL_THIS(csgo::EnginePOD* thisptr), int width, int height) noexcept;
    static const csgo::DemoPlaybackParameters* FASTCALL_CONV getDemoPlaybackParameters(FASTCALL_THIS(csgo::EnginePOD* thisptr)) noexcept;

private:
    HookType hookImpl;

    bool (THISCALL_CONV* originalIsPlayingDemo)(csgo::EnginePOD* thisptr);
    float (THISCALL_CONV* originalGetScreenAspectRatio)(csgo::EnginePOD* thisptr, int width, int height);
    const csgo::DemoPlaybackParameters* (THISCALL_CONV* originalGetDemoPlaybackParameters)(csgo::EnginePOD* thisptr);
};
