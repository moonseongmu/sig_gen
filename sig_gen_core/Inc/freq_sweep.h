#pragma once

#include "dds.h"
#include "interface.h"
#include "oscillator.h"
#include <stdbool.h>
#include <stdint.h>
#include <tgmath.h>

typedef struct sweep_settings_t
{
    float start_freq;
    float end_freq;
    float step_size;
    float current_freq;
    float prev_amplitude;
    float max_epsilon; //used to compare current amplitude to previous amplitude to determine if amplitude has stabilised
    uint32_t clock_freq;
    bool sweep_complete;
} sweep_settings_t;

void fill_sin_cos_buffers(dds_t *sin_dds, dds_t *cos_dds, uint32_t block_size, uint16_t *sin_buf, uint16_t *cos_buf);
void start_oscillations(sweep_settings_t *settings, oscillator_t *oscillator, uint32_t block_size);
void continue_oscillations(sweep_settings_t *settings, oscillator_t *oscillator, uint32_t block_size);
void set_sweep_settings(sweep_settings_t *settings, float start_freq, float end_freq, float step, float max_epsilon);