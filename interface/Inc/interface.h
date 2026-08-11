#pragma once

#include <stdint.h>

void system_init(void);


void block_transfer_init(void); //setup 1MHz interrupt/trigger rate timer, setup dma(if available)
void block_transfer_start(void); //every 1MHz ouptut new tuning fork dac value and read in new tuning fork adc value, calculate new dds tuning word using pid loop
void block_transfer_complete_ISR(void); // swap buffers
void block_transfer_end(void); //stop transfer of data and calculation of pid loop


void dac_init(void); //setup dac
void dac_update(uint16_t value); //send new value to dac

void adc_init(void); //setup adc
void adc_read(void); //read new value from adc