/*
 * Copyright (c) 2025, Joe Inman
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at:
 *     https://opensource.org/licenses/MIT
 *
 * This file is part of the RPixel Library.
 */

#pragma once

#include <hardware/pio.h>
#include <pico/stdlib.h>

#include <cstdint>
#include <functional>
#include <vector>

namespace jsi
{

class RPixel
{
public:
    RPixel(uint8_t data_pin, uint64_t num_pixels, bool is_rgbw);
    ~RPixel() = default;

    void setPixel(size_t pixel_index, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 255) noexcept;
    void show() const noexcept;

private:
    std::vector<uint32_t> pixels_;
    bool                  is_rgbw_;
    uint64_t              num_pixels_;
    uint8_t               data_pin_;

    PIO  pio_;
    uint sm_;
    uint offset_;

    void     writePixel(PIO pio, uint sm, uint32_t pixel_grb) const noexcept;
    uint32_t getUINT32FromRGB(uint8_t r, uint8_t g, uint8_t b);
    uint32_t getUINT32FromRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
};

}  // namespace jsi
