#pragma once

#include "dds.h"
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

typedef enum active_buffer_t
{
    buffer_1,
    buffer_2
} active_buffer_t; //buffer used for calculations, not currently being filled by adc/being output by dac

typedef struct oscillator_t
{

    uint32_t init_counter;
    uint16_t sin_buffer_1[BLOCK_SIZE];
    uint16_t sin_buffer_2[BLOCK_SIZE];
    uint16_t cos_buffer_1[BLOCK_SIZE];
    uint16_t cos_buffer_2[BLOCK_SIZE];
    uint16_t signal_buffer_1[BLOCK_SIZE];
    uint16_t signal_buffer_2[BLOCK_SIZE];
    float amplitude_buffer_1[BLOCK_SIZE];
    float amplitude_buffer_2[BLOCK_SIZE];
    float phase_buffer_1[BLOCK_SIZE];
    float phase_buffer_2[BLOCK_SIZE];

    dds_t sin_dds;
    dds_t cos_dds;

    active_buffer_t active_buffer;

    bool buffers_swapped;
} oscillator_t;