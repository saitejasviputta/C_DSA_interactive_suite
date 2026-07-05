#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

// hamming code receiver (decoder) demo.
void hamming_receiver_demo(void)
{
    while (1)
    {
        char frame[CHECKSUM_MAX_BITS + 1];

        int frame_status =
            safe_input_binary_string(frame, sizeof(frame),
                                     "\n\nHamming Code Receiver Demo\n"
                                     "enter received Hamming codeword or 'X' to exit:- ");

        if (frame_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Hamming Code Receiver demo....");
            return;
        }

        if (frame_status == 0)
        {
            continue;
        }

        int n = (int)strlen(frame);
        int r = 0;
        while ((1 << r) <= n)
        {
            r++;
        }

        char corrected_frame[CHECKSUM_MAX_BITS + 16];
        char data_out[CHECKSUM_MAX_BITS + 16];

        int syndrome = hamming_verify(frame, corrected_frame, data_out);

        printf("\nReceived codeword    : %s", frame);
        printf("\nCodeword length (n)  : %d", n);
        printf("\nParity bits (r)      : %d", r);
        printf("\nSyndrome value       : %d", syndrome);

        if (syndrome == 0)
        {
            printf("\nResult               : no error detected -- data accepted");
        }
        else if (syndrome > n)
        {
            // syndrome points outside frame: multi-bit error, cannot correct
            printf("\nResult               : syndrome %d is out of range -- "
                   "multiple-bit error, cannot correct",
                   syndrome);
        }
        else
        {
            printf("\nResult               : error detected at position %d -- correcting",
                   syndrome);
            printf("\nCorrected codeword   : %s", corrected_frame);
        }

        printf("\nRecovered data bits  : %s\n", data_out);
    }
}
