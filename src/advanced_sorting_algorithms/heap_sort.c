#include "advanced_sorting.h"
#include "array.h"
#include "priority_queue.h"
#include "safe_input.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void heap_sort_with_telemetry(int arr[], int n, SortingTelemetry* telemetry)
{
    if (n <= 1)
        return;

    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Heap Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("heap_sort");

    priority_queue* pq = pq_init(MIN_HEAP);
    if (pq == NULL)
    {
        telemetry_close();
        if (telemetry)
            sorting_telemetry_stop(telemetry);
        return;
    }

    int inserted = 0;
    for (int i = 0; i < n; i++)
    {
        visualize_sort(arr, n, i, -1, -1, "Heap Sort: Inserting elements into Min-Heap");
        sorting_telemetry_add_comparison(telemetry, 1);
        sorting_telemetry_add_copy(telemetry, 1);
        if (insert(pq, arr[i]) == 0) // heap full
            break;
        inserted++;
    }

    for (int i = 0; i < inserted; i++)
    {
        sorting_telemetry_add_comparison(telemetry, 1);
        sorting_telemetry_add_swap(telemetry, 1);
        extractTop(pq, &arr[i]); // min first -> ascending order
        visualize_sort(arr, n, i, -1, -1, "Heap Sort: Extracting min elements back to array");
    }

    destroy_pq(pq);
    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void heap_sort(int arr[], int n)
{
    heap_sort_with_telemetry(arr, n, NULL);
}
