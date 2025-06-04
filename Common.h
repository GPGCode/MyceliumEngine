#pragma once

#include <cstdint>
#include <bitset>
//When making component storage, EntityID wasn't defined, so defining it here instead in case
//we need to use it anywhere else.

using EntityID = std::uint32_t;

constexpr std::size_t MAX_COMPONENTS = 64;
constexpr std::size_t MAX_ENTITIES = 5000;

using Signature = std::bitset<MAX_COMPONENTS>;
using ComponentType = std::uint8_t; // Allows up to 256 component types