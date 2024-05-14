#pragma once

#include <cstdint>

#include <CS2/Classes/Entities/C_CSPlayerPawn.h>
#include <Utils/FieldOffset.h>

template <typename FieldType, typename OffsetType>
using PlayerPawnOffset = FieldOffset<cs2::C_CSPlayerPawn, FieldType, OffsetType>;

using OffsetToPlayerPawnImmunity = PlayerPawnOffset<cs2::C_CSPlayerPawn::m_bGunGameImmunity, std::int32_t>;
using OffsetToWeaponServices = PlayerPawnOffset<cs2::C_CSPlayerPawn::m_pWeaponServices, std::int32_t>;
using OffsetToPlayerController = PlayerPawnOffset<cs2::C_CSPlayerPawn::m_hController, std::int32_t>;
using OffsetToIsDefusing = PlayerPawnOffset<cs2::C_CSPlayerPawn::m_bIsDefusing, std::int32_t>;
using OffsetToIsPickingUpHostage = PlayerPawnOffset<cs2::C_CSPlayerPawn::m_bIsGrabbingHostage, std::int32_t>;
using OffsetToHostageServices = PlayerPawnOffset<cs2::C_CSPlayerPawn::m_pHostageServices, std::int32_t>;
