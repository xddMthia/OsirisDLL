#pragma once

#include <algorithm>

#include "PlayedSound.h"
#include <Utils/DynamicArray.h>

class WatchedSounds {
public:
    bool addSound(const PlayedSound& sound) noexcept
    {
        return sounds.pushBack(sound);
    }

    [[nodiscard]] bool hasSound(int guid) const noexcept
    {
        return std::ranges::find(sounds, guid, &PlayedSound::guid) != sounds.end();
    }

    template <typename Predicate>
    void removeExpiredSounds(Predicate&& predicate) noexcept
    {
        for (std::size_t i = 0; i < sounds.getSize();) {
            auto& sound = sounds[i];
            if (predicate(std::as_const(sound)))
                removeSound(sound);
            else
                ++i;
        }
    }

    template <typename F>
    void forEach(F&& f) const noexcept
    {
        std::ranges::for_each(sounds, std::forward<F>(f));
    }

private:
    void removeSound(PlayedSound& sound) noexcept
    {
        sound = sounds.back();
        sounds.popBack();
    }

    DynamicArray<PlayedSound> sounds;
};
