#include "sorting_visualizer.h"
#include "cross_platform_timer.h"
#include <stdio.h>

void visualize_sort(const int arr[], int n, int active_idx1, int active_idx2, int pivot_idx, const char* status_message)
{
    if (n <= 0 || !is_terminal_interactive())
    {
        return;
    }

    // Find min and max values in the array for dynamic scaling
    int min_val = arr[0];
    int max_val = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < min_val)
        {
            min_val = arr[i];
        }
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }

    // Clear terminal screen using ANSI code
    printf("\033[H\033[J");

    if (status_message)
    {
        printf("\n=== %s ===\n\n", status_message);
    }

    for (int i = 0; i < n; i++)
    {
        // Highlight logic:
        // Yellow for pivot
        // Red for active comparison/swap
        // Cyan for neutral elements
        if (i == pivot_idx)
        {
            printf("\033[1;33m"); // Yellow
        }
        else if (i == active_idx1 || i == active_idx2)
        {
            printf("\033[1;31m"); // Red
        }
        else
        {
            printf("\033[1;36m"); // Cyan
        }

        printf("[%2d]: %4d | ", i, arr[i]);

        int width = 0;
        if (max_val == min_val)
        {
            width = 10;
        }
        else
        {
            width = 1 + (arr[i] - min_val) * 39 / (max_val - min_val);
        }

        for (int w = 0; w < width; w++)
        {
            printf("█");
        }

        // Reset color
        printf("\033[0m\n");
    }
    printf("\n");
    fflush(stdout);

    // Sleep for 0.1 seconds to make the animation watchable
    sleep_seconds(0.1f);
}
