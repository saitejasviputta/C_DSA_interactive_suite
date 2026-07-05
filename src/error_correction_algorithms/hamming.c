#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

// hamming_generate: builds the (n,k) Hamming codeword for `data` by inserting
// r even-parity bits at the power-of-2 positions (1, 2, 4, 8, …).
// The codeword is written to `codeword_out` (caller must supply a buffer of at
// least k + ceil(log2(k+r+1)) + 1 bytes; CHECKSUM_MAX_BITS + 16 is safe).
void hamming_generate(const char* data, char* codeword_out)
{
    int k = (int)strlen(data);

    // Smallest r such that 2^r >= k + r + 1
    int r = 0;
    while ((1 << r) < (k + r + 1))
    {
        r++;
    }
    int n = k + r;

    // ham[1..n]: power-of-2 positions are parity placeholders, all others hold data
    int ham[CHECKSUM_MAX_BITS + 16];
    int data_index = 0;
    for (int pos = 1; pos <= n; pos++)
    {
        if ((pos & (pos - 1)) == 0) // power of two -> parity position
        {
            ham[pos] = 0;
        }
        else
        {
            ham[pos] = data[data_index] - '0';
            data_index++;
        }
    }

    // Compute each parity bit (even parity) over every position it covers
    for (int i = 0; i < r; i++)
    {
        int parity_pos = (1 << i);
        int parity = 0;
        for (int pos = 1; pos <= n; pos++)
        {
            if ((pos & parity_pos) && pos != parity_pos)
            {
                parity ^= ham[pos];
            }
        }
        ham[parity_pos] = parity;
    }

    for (int pos = 1; pos <= n; pos++)
    {
        codeword_out[pos - 1] = ham[pos] + '0';
    }
    codeword_out[n] = '\0';
}

// hamming_verify: recomputes the syndrome from a received codeword and returns
// the 1-based position of a single-bit error (0 = no error, >n = uncorrectable
// multi-bit error).  If the error position is within range it is corrected
// in-place.  The corrected codeword and recovered data bits are written to
// `corrected_codeword_out` and `data_out` respectively (both may be NULL).
int hamming_verify(const char* received_codeword, char* corrected_codeword_out, char* data_out)
{
    int n = (int)strlen(received_codeword);

    int ham[CHECKSUM_MAX_BITS + 16];
    for (int pos = 1; pos <= n; pos++)
    {
        ham[pos] = received_codeword[pos - 1] - '0';
    }

    int r = 0;
    while ((1 << r) <= n)
    {
        r++;
    }

    // Build syndrome
    int syndrome = 0;
    for (int i = 0; i < r; i++)
    {
        int parity_pos = (1 << i);
        int parity = 0;
        for (int pos = 1; pos <= n; pos++)
        {
            if (pos & parity_pos)
            {
                parity ^= ham[pos];
            }
        }
        if (parity)
        {
            syndrome += parity_pos;
        }
    }

    // Correct single-bit error if within range
    if (syndrome > 0 && syndrome <= n)
    {
        ham[syndrome] ^= 1;
    }

    if (corrected_codeword_out != NULL)
    {
        for (int pos = 1; pos <= n; pos++)
        {
            corrected_codeword_out[pos - 1] = ham[pos] + '0';
        }
        corrected_codeword_out[n] = '\0';
    }

    if (data_out != NULL)
    {
        int data_idx = 0;
        for (int pos = 1; pos <= n; pos++)
        {
            if ((pos & (pos - 1)) != 0) // not a power of two -> data bit
            {
                data_out[data_idx] = ham[pos] + '0';
                data_idx++;
            }
        }
        data_out[data_idx] = '\0';
    }

    return syndrome;
}

// hamming code generation demo (sender side).
void hamming_demo(void)
{
    while (1)
    {
        char data[CHECKSUM_MAX_BITS + 1];

        int data_status = safe_input_binary_string(data, sizeof(data),
                                                   "\n\nHamming Code Demo\n"
                                                   "enter binary data bits or 'X' to exit:- ");

        if (data_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Hamming Code demo....");
            return;
        }

        if (data_status == 0)
        {
            continue;
        }

        char codeword[CHECKSUM_MAX_BITS + 16];
        hamming_generate(data, codeword);

        int k = (int)strlen(data);
        int n = (int)strlen(codeword);
        int r = n - k;

        printf("\nData bits (k)        : %d", k);
        printf("\nParity bits (r)      : %d", r);
        printf("\nCodeword length (n)  : %d", n);

        printf("\nHamming Code (P=parity, transmitted left to right):\n");
        for (int pos = 1; pos <= n; pos++)
        {
            if ((pos & (pos - 1)) == 0)
            {
                printf("P%d=%c ", pos, codeword[pos - 1]);
            }
            else
            {
                printf("D%d=%c ", pos, codeword[pos - 1]);
            }
        }

        printf("\nGenerated Hamming Code (%d,%d): %s\n", n, k, codeword);
    }
}
