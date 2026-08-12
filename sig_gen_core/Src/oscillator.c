#include "oscillator.h"

void start_oscillations(float out_freq,
                        float clock_freq,
                        oscillator_t *oscillator,
                        uint32_t block_size)
{
    // start timer and dma dac, interrupt calls when buffer filled/finished
    // outputing
    block_transfer_init();
    // init cosdds with sweep starting frequency + starting phase 0
    oscillator->dds.tuning_word = freq_to_tuning_word(out_freq, clock_freq);
    oscillator->dds.phase_accumulator = 0;

    block_transfer_start();

    while (oscillator->buffers_swapped == true)
    {
        fill_buffer(oscillator, block_size);
        oscillator->buffers_swapped = false;
    }
}

void fill_buffer(oscillator_t *oscillator, uint32_t block_size)
{
    for (uint32_t i = 0; i < block_size; i++)
    {
        dds_calculate(&(oscillator->dds));
        switch (oscillator->active_buffer)
        {
            case buffer_1:
                oscillator->buffer_1[i] = oscillator->dds.value;
            case buffer_2:
                oscillator->buffer_2[i] = oscillator->dds.value;
        }
    }
}
