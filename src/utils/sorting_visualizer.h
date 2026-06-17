#ifndef SORTING_VISUALIZER_H
#define SORTING_VISUALIZER_H

/**
 * Renders the state of an array as colored horizontal block bars.
 * 
 * @param arr The array to visualize
 * @param n Total number of elements
 * @param active_idx1 First index involved in comparison/swap (-1 if none)
 * @param active_idx2 Second index involved in comparison/swap (-1 if none)
 * @param pivot_idx Index of current pivot (-1 if none)
 * @param status_message Title/status text to display above the bars
 */
void visualize_sort(const int arr[], int n, int active_idx1, int active_idx2, int pivot_idx, const char* status_message);

#endif // SORTING_VISUALIZER_H
