#include "freq_sweep.h"

float mean(float *array, uint32_t size)
{
    float sum = 0;
    for (uint32_t i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum/size;
}

void set_sweep_settings(sweep_settings_t *settings, float start_freq, float end_freq, float step, float max_epsilon)
{
    settings->prev_amplitude = 0;
    settings->start_freq = start_freq;
    settings->end_freq = end_freq;
    settings->step_size = step;
    settings->max_epsilon = max_epsilon;
}

void start_oscillations(sweep_settings_t *settings, oscillator_t *oscillator, uint32_t block_size)
{
    //start timer and dma to adc and dac, interrupt calls when buffer filled/finished outputing
    block_transfer_init();
    //init cosdds with sweep starting frequency + starting phase 0
    oscillator->cos_dds.tuning_word = freq_to_tuning_word(settings->start_freq, settings->clock_freq);
    oscillator->cos_dds.phase_accumulator = 0;

    //init sindds with sweep starting frequency + starting phase 90
    oscillator->sin_dds.tuning_word = freq_to_tuning_word(settings->start_freq, settings->clock_freq);
    oscillator->sin_dds.phase_accumulator = 1073741824; //(2^32-1)/4 for 90* initial starting offset phase
    settings->current_freq = settings->start_freq;

    block_transfer_start();
    float amplitude_avg = 0.0f;
    
    while(oscillator->buffers_swapped == true && oscillator->init_counter < 6)
    {
        switch(oscillator->active_buffer)
        {
            case buffer_1:
                lock_in(oscillator->sin_buffer_1, oscillator->cos_buffer_1, oscillator->signal_buffer_1, oscillator->amplitude_buffer_1, oscillator->phase_buffer_1, block_size);
                amplitude_avg = mean(oscillator->amplitude_buffer_1, block_size);
                fill_sin_cos_buffers(&(oscillator->sin_dds), &(oscillator->cos_dds), block_size, oscillator->sin_buffer_1, oscillator->cos_buffer_1);
                break;
            case buffer_2:
                lock_in(oscillator->sin_buffer_2, oscillator->cos_buffer_2, oscillator->signal_buffer_2, oscillator->amplitude_buffer_2, oscillator->phase_buffer_2, block_size);
                amplitude_avg = mean(oscillator->amplitude_buffer_2, block_size);
                fill_sin_cos_buffers(&(oscillator->sin_dds), &(oscillator->cos_dds), block_size, oscillator->sin_buffer_2, oscillator->cos_buffer_2);
                break;
        }
        settings->prev_amplitude = amplitude_avg;
        oscillator->init_counter += 1;
        oscillator->buffers_swapped = false;
    }
}


void continue_oscillations(sweep_settings_t *settings, oscillator_t *oscillator, uint32_t block_size)
{
    float amplitude_avg = 0.0f;
    switch(oscillator->active_buffer)
    {
        case buffer_1:
            lock_in(oscillator->sin_buffer_1, oscillator->cos_buffer_1, oscillator->signal_buffer_1, oscillator->amplitude_buffer_1, oscillator->phase_buffer_1, block_size);
            amplitude_avg = mean(oscillator->amplitude_buffer_1, block_size);
            fill_sin_cos_buffers(&(oscillator->sin_dds), &(oscillator->cos_dds), block_size, oscillator->sin_buffer_1, oscillator->cos_buffer_1);
            break;
        case buffer_2:
            lock_in(oscillator->sin_buffer_2, oscillator->cos_buffer_2, oscillator->signal_buffer_2, oscillator->amplitude_buffer_2, oscillator->phase_buffer_2, block_size);
            amplitude_avg = mean(oscillator->amplitude_buffer_2, block_size);
            fill_sin_cos_buffers(&(oscillator->sin_dds), &(oscillator->cos_dds), block_size, oscillator->sin_buffer_2, oscillator->cos_buffer_2);
            break;
    }
    
    if(fabs(amplitude_avg - settings->prev_amplitude) < settings->max_epsilon)
    {
        //amplitude has stabilised, move tuning word to next step
        settings->current_freq += settings->step_size;
        oscillator->sin_dds.tuning_word = freq_to_tuning_word(settings->current_freq, settings->clock_freq);
        oscillator->cos_dds.tuning_word = freq_to_tuning_word(settings->current_freq, settings->clock_freq);
    }
    settings->prev_amplitude = amplitude_avg;
    oscillator->buffers_swapped = false;
}

void fill_sin_cos_buffers(dds_t *sin_dds, dds_t *cos_dds, uint32_t block_size, uint16_t *sin_buf, uint16_t *cos_buf)
{
    for (uint32_t i=0; i<block_size; i++)
    {
        dds_calculate(sin_dds);
        dds_calculate(cos_dds);
        cos_buf[i] = cos_dds->value;
        sin_buf[i] = sin_dds->value;
    }
}


