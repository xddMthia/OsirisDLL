#pragma once

#include <cstdint>
#include <cstring>

#include "RttiCompleteObjectLocator.h"
#include "ToRvaConverter.h"
#include <MemorySearch/BinaryBytePattern.h>
#include <Utils/MemorySection.h>

struct RttiTypeDescriptor;

class RttiCompleteObjectLocatorFinder {
public:
    RttiCompleteObjectLocatorFinder(MemorySection rdataSection, ToRvaConverter toRvaConverter) noexcept
        : rdataSection{rdataSection}
        , toRvaConverter{toRvaConverter}
    {
    }
    
    [[nodiscard]] const RttiCompleteObjectLocator* findCompleteObjectLocator(const RttiTypeDescriptor* typeDescriptor) const noexcept
    {
        const BinaryBytePattern typeDescriptorRvaPattern{toRvaConverter.toRva(reinterpret_cast<std::uintptr_t>(typeDescriptor))};
        HybridPatternFinder typeDescriptorCrossReferenceFinder{rdataSection.raw(), typeDescriptorRvaPattern};

        auto typeDescriptorReference{typeDescriptorCrossReferenceFinder.findNextOccurrence()};
        while (typeDescriptorReference && (!isCompleteObjectLocator(reinterpret_cast<std::uintptr_t>(typeDescriptorReference)) || !isCompleteObjectLocatorOfCompleteClass(typeDescriptorReference)))
            typeDescriptorReference = typeDescriptorCrossReferenceFinder.findNextOccurrence();

        if (typeDescriptorReference)
            return reinterpret_cast<const RttiCompleteObjectLocator*>(typeDescriptorReference - RttiCompleteObjectLocator::kOffsetOfTypeDescriptorRva);

        return nullptr;
    }

private:
    [[nodiscard]] bool isCompleteObjectLocatorOfCompleteClass(const std::byte* typeDescriptorReference) const noexcept
    {
        std::uint32_t offsetInCompleteClass;
        std::memcpy(&offsetInCompleteClass, typeDescriptorReference - RttiCompleteObjectLocator::kOffsetOfTypeDescriptorRva + RttiCompleteObjectLocator::kOffsetOfOffsetInCompleteClass, sizeof(std::uint32_t));
        return offsetInCompleteClass == 0;
    }

    [[nodiscard]] bool isCompleteObjectLocator(std::uintptr_t typeDescriptorReference) const noexcept
    {
        return hasNoCrossReferences(BinaryBytePattern{toRvaConverter.toRva(typeDescriptorReference)}) && rdataSection.offsetOf(typeDescriptorReference) >= RttiCompleteObjectLocator::kOffsetOfTypeDescriptorRva;
    }

    [[nodiscard]] bool hasNoCrossReferences(BytePattern pattern) const noexcept
    {
        return HybridPatternFinder{rdataSection.raw(), pattern}.findNextOccurrence() == nullptr;
    }

    MemorySection rdataSection;
    ToRvaConverter toRvaConverter;
};
