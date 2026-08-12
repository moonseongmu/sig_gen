#pragma once

#include "dds.h"
#include "interface.h"
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

typedef enum active_buffer_t
{
    buffer_1,
    buffer_2
} active_buffer_t; //buffer used for calculations, not currently being output by dac

typedef struct oscillator_t
{
    uint16_t buffer_1[BLOCK_SIZE];
    uint16_t buffer_2[BLOCK_SIZE];

    dds_t dds;

    active_buffer_t active_buffer;

    bool buffers_swapped;
} oscillator_t;

void fill_buffer(oscillator_t *oscillator, uint32_t block_size);
void start_oscillations(float out_freq, float clock_freq, oscillator_t *oscillator, uint32_t block_size);
