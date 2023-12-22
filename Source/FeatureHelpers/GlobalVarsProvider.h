#pragma once

#include <cassert>

#include <CS2/Classes/GlobalVars.h>
#include <MemoryPatterns/ClientPatterns.h>

struct GlobalVarsProvider {
    explicit GlobalVarsProvider(const ClientPatterns& clientPatterns) noexcept
        : globalVars{clientPatterns.globalVars()}
    {
    }

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return globalVars != nullptr;
    }

    [[nodiscard]] cs2::GlobalVars* getGlobalVars() const noexcept
    {
        assert(static_cast<bool>(*this));
        return *globalVars;
    }

private:
    cs2::GlobalVars** globalVars;
};
