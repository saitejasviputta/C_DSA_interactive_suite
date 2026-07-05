#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

/*
 * LRC (Longitudinal Redundancy Check)
 * ------------------------------------
 * Each bit of the LRC is the XOR (even parity) of the corresponding
 * bit column across all data words.  The result is appended to the
 * transmitted block so the receiver can verify integrity.
 */

#define LRC_MAX_ROWS 20
#define LRC_MAX_COLS 64

// lrc_calculate: computes the LRC by XOR-ing each column across all `words`.
// Writes a null-terminated binary string of length `word_len` to `lrc_out`.
void lrc_calculate(const char* const* words, int num_words, int word_len, char* lrc_out)
{
    for (int j = 0; j < word_len; j++)
    {
        int ones = 0;
        for (int i = 0; i < num_words; i++)
        {
            if (words[i][j] == '1')
            {
                ones++;
            }
        }
        lrc_out[j] = (ones % 2 == 0) ? '0' : '1';
    }
    lrc_out[word_len] = '\0';
}

// lrc_verify: returns 1 if the computed LRC matches `received_lrc`, 0 otherwise.
int lrc_verify(const char* const* words, int num_words, int word_len, const char* received_lrc)
{
    char computed_lrc[LRC_MAX_COLS + 1];
    lrc_calculate(words, num_words, word_len, computed_lrc);
    return (strcmp(computed_lrc, received_lrc) == 0) ? 1 : 0;
}

void lrc_demo(void)
{
    int rows;
    printf("\n=== LRC (Longitudinal Redundancy Check) ===\n");

    int result = safe_input_int(&rows, "Enter number of data words (1-20): ", 1, LRC_MAX_ROWS);
    if (result == -111)
    {
        printf("Exiting LRC demo...\n");
        return;
    }
    if (result == 0)
    {
        printf("Invalid input. Returning to main menu.\n");
        return;
    }

    char data[LRC_MAX_ROWS][LRC_MAX_COLS + 1];
    int cols = -1;

    printf("Enter binary data words (same length, e.g. 10110011):\n");
    for (int i = 0; i < rows; i++)
    {
        printf("  Word %d: ", i + 1);
        if (fgets(data[i], sizeof(data[i]), stdin) == NULL)
        {
            printf("Error reading input.\n");
            return;
        }

        int len = strlen(data[i]);
        if (len > 0 && data[i][len - 1] == '\n')
        {
            data[i][len - 1] = '\0';
            len--;
        }
        else
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }

        if (len == 0)
        {
            printf("Error: word cannot be empty.\n");
            return;
        }

        for (int j = 0; j < len; j++)
        {
            if (data[i][j] != '0' && data[i][j] != '1')
            {
                printf("Error: word must contain only 0s and 1s.\n");
                return;
            }
        }

        if (cols == -1)
        {
            cols = len;
        }
        else if (len != cols)
        {
            printf("Error: all data words must have the same length.\n");
            return;
        }
    }

    const char* words[LRC_MAX_ROWS];
    for (int i = 0; i < rows; i++)
    {
        words[i] = data[i];
    }

    char lrc[LRC_MAX_COLS + 1];
    lrc_calculate(words, rows, cols, lrc);

    printf("\nTransmitted Block:\n");
    printf("------------------\n");
    for (int i = 0; i < rows; i++)
        printf("  Data word %2d : %s\n", i + 1, data[i]);
    printf("  LRC          : %s\n", lrc);
    printf("------------------\n");
    printf("LRC = %s\n", lrc);

    printf("\nColumn-by-column parity breakdown:\n");
    for (int j = 0; j < cols; j++)
    {
        int ones = 0;
        for (int i = 0; i < rows; i++)
            if (data[i][j] == '1')
                ones++;
        printf("  Col %2d : %d one(s) -> parity bit = %c\n", j + 1, ones, lrc[j]);
    }
}