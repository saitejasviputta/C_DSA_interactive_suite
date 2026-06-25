#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_programming.h"
#include "mock_printf.h"

// Define overrides to suppress history logging and DP table prints
#define printf mock_printf
#include "../../src/dynamic_programming/knapsack.c"
#undef printf

void test_knapsack_base_cases()
{
    reset_printf_buf();
    int wt[] = {2, 3, 4};
    int val[] = {3, 4, 5};

    // Zero capacity W = 0
    assert(knapsack(0, wt, val, 3) == 0);
    // Verify table printed empty capacity values
    assert(strstr(g_printf_buf, "--- 0/1 Knapsack DP Table ---") != NULL);

    reset_printf_buf();
    // Zero items n = 0
    assert(knapsack(5, wt, val, 0) == 0);
    assert(strstr(g_printf_buf, "--- 0/1 Knapsack DP Table ---") != NULL);
}

void test_knapsack_all_fit()
{
    reset_printf_buf();
    int wt[] = {1, 2, 3};
    int val[] = {10, 15, 40};
    int n = 3;
    int W = 6; // All fit: total weight = 6, total value = 65

    assert(knapsack(W, wt, val, n) == 65);
}

void test_knapsack_subset_fit()
{
    reset_printf_buf();
    int wt[] = {1, 2, 3, 5};
    int val[] = {1, 6, 18, 22};
    int n = 4;
    int W = 5; // Best: item 2 (wt 2, val 6) and item 3 (wt 3, val 18) total wt 5, val 24. (Item 4 is wt 5 val 22)

    assert(knapsack(W, wt, val, n) == 24);
}

void test_knapsack_none_fit()
{
    reset_printf_buf();
    int wt[] = {5, 6, 7};
    int val[] = {10, 20, 30};
    int n = 3;
    int W = 3; // None fit

    assert(knapsack(W, wt, val, n) == 0);
}

int main()
{
    test_knapsack_base_cases();
    test_knapsack_all_fit();
    test_knapsack_subset_fit();
    test_knapsack_none_fit();

    fprintf(stdout, "All Knapsack tests passed successfully!\n");
    return 0;
}
