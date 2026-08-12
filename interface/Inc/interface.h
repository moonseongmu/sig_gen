#pragma once

#include <stdint.h>

void system_init (void);

// setup interrupt/trigger rate timer, setup dma(if available)
void block_transfer_init (void);

// output to dac at timer rate, call block_transfer_complete_ISR
// when transfer of block complete
void block_transfer_start (void);
void block_transfer_complete_ISR (void); // swap buffers
void block_transfer_end (void);          // stop transfer of data

void dac_init (void);             // setup dac
void dac_update (uint16_t value); // send new value to dac