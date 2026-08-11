all source files and headers in folder tufo_core whould onyl refernceinterface.h
interface.h defines functions that should be implemented as platofrm specific code
to port to different microcontroller, or different reviison of board, just implement the functions defined in interface.h

```
allocate double buffers for sine wave,cosine wave(dac output) and adc input for tuning fork of size BLOCK_SIZE
main loop waits for command to begin tuning fork oscillations 
if in pll mode 
-> fill sine and cos buffer 1 
-> start timer to clock in adc data and clock out dac data from buffer 1
    ->simultaneously, inphasebuffer = adcbuffer2 x cosbuffer2 and quadraturebuffer =  adcbuffer2 x sinbuffer2 
    ->low pass filter both inphase and quadrature buffers
    ->calculate amplitude and phase
    ->pid loop to find new tuning word 
    ->fill new sin and cos buffer 2 
-> once data finished transfering out, swap buffers and repeat

if frequency sweep mode
-> fill sine and cos buffer 1 
-> start timer to clock in adc data and clock out dac data from buffer 1
    ->simultaneously, inphasebuffer = adcbuffer2 x cosbuffer2 and quadraturebuffer =  adcbuffer2 x sinbuffer2 
    ->low pass filter both inphase and quadrature buffers
    ->calculate amplitude and phase
    ->check if amplitude stable
        -> if yes, update new tuning word according to frequency step 
    ->fill new sin and cos buffer 2 
-> once data finished transfering out, swap buffers and repeat


main while(1)
{
    state = state_idle
    blocksize = 512
    cosbuf1 
    cosbuf2
    sinbuf1
    sinbuf2
    inbuf1
    inbuf2
    ampbuf1
    ampbuf2
    phasebuf1
    phasebuf2

    switch(state)
    {
        case(state_idle):
            wait_for_command()
            if command == start sweep
            {
                state = state_sweep
            } else if command == start acquire
            {
                state = state_acquire
            }
            break
        
        case(state_sweep):
            start_oscillations(start_freq_tuning_word)
            bool sweep_complete = flase
            prevampl = 0
            while(sweep_complete == false && buffer_swapped == true)
            {
                lockin()
                if abs(prevampl-ampl) < tolerance
                {
                    dds->tuningword += tuning_word_step
                    prevampl = ampl
                }

                fillsincosbuffers()
                buffer_swapped = false

                if dds->tuningword >= freq_end_tuning_word
                {
                    sweep_complete = true
                }
            }

            end_oscillations()
            state= state_idle
            break

        case(state_acquire):
            break


    }
}

fill_sincos_buffers(dds_t *sindds, dds_t *cosdds, blocksize)
{
    for int i=0; i<blocksize; i++
    {
        sindds_val = dds_calculate(sindds)
        cosdds_val = dds_calculate(cosdds)
        switch (active_buffer)
        {
            case buffer1:
                cosbuf1[i] = cosdds_val
                sinbuf1[i] = sindds_val
            case buffer2:
                cosbuf2[i] = cosdds_val
                sinbuf2[i] = sindds_val
        }
    }
}

start_oscillations()
{
    start timer and dma to adc and dac, interrupt calls when buffer filled/finished outputing
    init sindds with sweep starting frequency + starting phase 90
    init cosdds with sweep starting frequency + starting phase 0
    for(int i = 0; i < 6; i++ )
    {
        fillsincosbuffers()
    }
}


block_transfer_finished_isr
{
    if active buffer == buf1{
        active_buffer = buf2
    } else {
        active_buffer = buf1
    }

    buffer_swapped = true
}

```