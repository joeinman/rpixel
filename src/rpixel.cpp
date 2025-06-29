/*
 * Copyright (c) 2025, Joe Inman
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at:
 *     https://opensource.org/licenses/MIT
 *
 * This file is part of the RPixel Library.
 */

#include "rpixel/rpixel.hpp"

#include "ws2812.pio.h"

namespace jsi
{

RPixel::RPixel(uint8_t data_pin, uint64_t num_pixels, bool is_rgbw)
{
    num_pixels_ = num_pixels;
    pixels_.resize(static_cast<std::vector<long unsigned int>::size_type>(num_pixels));
    is_rgbw_  = is_rgbw;
    data_pin_ = data_pin;

    // Clear all pixels
    for (size_t i = 0; i < num_pixels_; i++)
    {
        pixels_[i] = 0;
    }

    // This will find a free pio and state machine for our program and load it for us
    // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
    // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
    bool success =
        pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio_, &sm_, &offset_, data_pin_, 1, true);
    hard_assert(success);

    constexpr uint PIO_FREQUENCY = 800000;
    ws2812_program_init(pio_, sm_, offset_, data_pin_, PIO_FREQUENCY, is_rgbw);
}

void RPixel::writePixel(PIO pio, uint state_machine, uint32_t pixel_grb) const noexcept
{
    pio_sm_put_blocking(pio, state_machine, pixel_grb << 8u);
}

uint32_t RPixel::getUINT32FromRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | (uint32_t) (b);
}

uint32_t RPixel::getUINT32FromRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    return ((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | ((uint32_t) (w) << 24) | (uint32_t) (b);
}

void RPixel::setPixel(size_t pixel_index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) noexcept
{
    if (pixel_index >= num_pixels_)
    {
        return;
    }

    if (is_rgbw_)
    {
        pixels_[pixel_index] = getUINT32FromRGBW(r, g, b, w);
    }
    else
    {
        pixels_[pixel_index] = getUINT32FromRGB(r, g, b);
    }
}

void RPixel::show() const noexcept
{
    for (size_t i = 0; i < num_pixels_; i++)
    {
        writePixel(pio_, sm_, pixels_[i]);
    }
    sleep_us(500);
}

}  // namespace jsi
