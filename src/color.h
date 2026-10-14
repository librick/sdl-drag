#pragma once
#include <cstdint>
#include <SDL2/SDL_pixels.h>

constexpr SDL_Color color_from_hex(std::uint32_t rgb) {
    return SDL_Color{
        static_cast<Uint8>((rgb >> 16) & 0xFF),
        static_cast<Uint8>((rgb >> 8) & 0xFF),
        static_cast<Uint8>(rgb & 0xFF),
        255,
    };
}

