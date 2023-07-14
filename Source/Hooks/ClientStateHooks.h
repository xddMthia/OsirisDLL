#pragma once

#include <HookType.h>
#include <Platform/Macros/CallingConventions.h>
#include <Platform/Macros/PlatformSpecific.h>
#include <RetSpoof/FunctionInvoker.h>
#include <Vmt/VmtLengthCalculator.h>

namespace csgo { struct ClientState; }

class ClientStateHooks {
public:
    explicit ClientStateHooks(VmtLengthCalculator vmtLengthCalculator)
        : hookImpl{ vmtLengthCalculator }
    {
    }

    void install(csgo::ClientState* clientState)
    {
        hookImpl.init(clientState);
        originalPacketEnd = reinterpret_cast<decltype(originalPacketEnd)>(hookImpl.hookAt(WIN32_LINUX(6, 7), &packetEnd));
    }

    void uninstall()
    {
        hookImpl.restore();
    }

    static void FASTCALL_CONV packetEnd(FASTCALL_THIS(csgo::ClientState* thisptr)) noexcept;

private:
    HookType hookImpl;

    void (THISCALL_CONV* originalPacketEnd)(csgo::ClientState* thisptr);
};
