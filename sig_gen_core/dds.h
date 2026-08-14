#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t phase_accumulator;
    uint32_t tuning_word;
    uint16_t value;
} dds_t;

void dds_init(dds_t *dds, uint32_t start_phase);
void dds_calculate(dds_t *dds);
void dds_update_tuning_word(dds_t *dds, uint32_t tuning_word);
uint32_t freq_to_tuning_word(float frequency, float clock_frequency);
