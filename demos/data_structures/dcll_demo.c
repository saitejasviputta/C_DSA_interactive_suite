#include "dcll.h"
#include "safe_input.h"
#include <stdio.h>

void dcll_demo(void)
{
    dcll list;
    dcll_init(&list);

    int dcll_element_count;
    int dcll_length_status;

start_dcll:
    dcll_length_status = safe_input_int(
        &dcll_element_count,
        "enter how many elements you want to insert, (between 1 and 100), enter '-1' to exit :- ",
        1, 100);

    if (dcll_length_status == INPUT_EXIT_SIGNAL)
    {
        printf("\nExiting dcll demo\n");
        dcll_destroy(&list);
        return;
    }

    if (dcll_length_status == 0)
    {
        goto start_dcll;
    }

    while (dcll_element_count > 0)
    {
        int dcll_position_choice;
        int dcll_position_status;

    dcll_position_selection:
        dcll_position_status = safe_input_int(&dcll_position_choice,
                                              "\nenter '0' for inserting at beginning"
                                              "\nenter '1' for inserting at end"
                                              "\nenter '2' for inserting at specific position"
                                              "\nenter '-1' to exit :- ",
                                              0, 2);

        if (dcll_position_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dcll demo\n");
            dcll_destroy(&list);
            return;
        }

        if (dcll_position_status == 0)
        {
            goto dcll_position_selection;
        }

        if (dcll_position_choice == 1)
        {
            int dcll_end_status;
            int dcll_end_value;
        dcll_enter_end_value:
            dcll_end_status = safe_input_int(&dcll_end_value,
                                             "enter the no. you want to insert at end, (between 1 "
                                             "and 100), enter '-1' to exit :- ",
                                             1, 100);

            if (dcll_end_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_end_status == 0)
            {
                goto dcll_enter_end_value;
            }

            int status = dcll_insertAtEnd(&list, dcll_end_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dcll_enter_end_value;
            }
            printf("\n");
            dcll_printlist(&list);
        }
        else if (dcll_position_choice == 0)
        {
            int dcll_start_status;
            int dcll_start_value;

        dcll_enter_start_value:
            dcll_start_status = safe_input_int(&dcll_start_value,
                                               "enter the no. you want to insert at beginning, "
                                               "(between 1 and 100), enter '-1' to exit :- ",
                                               1, 100);

            if (dcll_start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_start_status == 0)
            {
                goto dcll_enter_start_value;
            }
            int status = dcll_insertAtBeginning(&list, dcll_start_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dcll_enter_start_value;
            }
            printf("\n");
            dcll_printlist(&list);
        }
        else if (dcll_position_choice == 2)
        {
            int dcll_pos_status;
            int dcll_pos_value;
            int dcll_pos_index;
            char dcll_pos_prompt[128];

        dcll_enter_pos_value:
            dcll_pos_status = safe_input_int(&dcll_pos_value,
                                             "enter the no. you want to insert, (between 1 "
                                             "and 100), enter '-1' to exit :- ",
                                             1, 100);

            if (dcll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_pos_status == 0)
            {
                goto dcll_enter_pos_value;
            }

        dcll_enter_pos_index:
            snprintf(dcll_pos_prompt, sizeof(dcll_pos_prompt),
                     "enter the position (0 to %d), enter '-1' to exit :- ", dcll_getLength(&list));
            dcll_pos_status =
                safe_input_int(&dcll_pos_index, dcll_pos_prompt, 0, dcll_getLength(&list));

            if (dcll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_pos_status == 0)
            {
                goto dcll_enter_pos_index;
            }

            int status = dcll_insertAtPosition(&list, dcll_pos_value, dcll_pos_index);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dcll_enter_pos_value;
            }
            else if (status == -2)
            {
                printf("\ninvalid position. try again\n");
                goto dcll_enter_pos_index;
            }
            printf("\n");
            dcll_printlist(&list);
        }

        dcll_element_count--;
    }

    // searching elements in dcll
    while (1)
    {
        int dcll_search_status;
        int dcll_search_value;
        dcll_search_status = safe_input_int(
            &dcll_search_value,
            "\nenter the element to be searched, (between 1 and 100), enter '-1' to exit :- ", 1,
            100);
        if (dcll_search_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (dcll_search_status == 0)
        {
            continue;
        }

        int index = dcll_search(&list, dcll_search_value);
        printf("\nelement found at index :- %d", index);
    }

    // deleting elements in dcll
    while (1)
    {
        int dcll_delete_choice;
        int dcll_delete_status;

    dcll_delete_selection:
        dcll_delete_status = safe_input_int(&dcll_delete_choice,
                                            "\nenter '0' to delete at beginning"
                                            "\nenter '1' to delete at end"
                                            "\nenter '2' to delete by value"
                                            "\nenter '3' to delete at position"
                                            "\nenter '-1' to exit :- ",
                                            0, 3);

        if (dcll_delete_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dcll demo\n");
            dcll_destroy(&list);
            return;
        }
        if (dcll_delete_status == 0)
        {
            goto dcll_delete_selection;
        }

        if (dcll_delete_choice == 0)
        {
            int status = dcll_deleteAtBeginning(&list);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
        else if (dcll_delete_choice == 1)
        {
            int status = dcll_deleteAtEnd(&list);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
        else if (dcll_delete_choice == 2)
        {
            int dcll_delete_value;
            dcll_delete_status = safe_input_int(
                &dcll_delete_value,
                "\nenter the element to be deleted, (between 1 and 100), enter '-1' to exit :- ", 1,
                100);

            if (dcll_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }
            if (dcll_delete_status == 0)
            {
                continue;
            }

            int status = dcll_deleteByValue(&list, dcll_delete_value);
            if (status == -2)
            {
                printf("\nList is empty\n");
            }
            else if (status == -1)
            {
                printf("\nvalue not found\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
        else if (dcll_delete_choice == 3)
        {
            int dcll_pos_delete_status;
            int dcll_pos_delete_index;
            char dcll_pos_delete_prompt[128];

            if (dcll_getLength(&list) == 0)
            {
                printf("\nList is empty\n");
                continue;
            }

        dcll_delete_pos_input:
            snprintf(dcll_pos_delete_prompt, sizeof(dcll_pos_delete_prompt),
                     "enter the position to delete (0 to %d), enter '-1' to exit :- ",
                     dcll_getLength(&list) - 1);
            dcll_pos_delete_status = safe_input_int(&dcll_pos_delete_index, dcll_pos_delete_prompt,
                                                    0, dcll_getLength(&list) - 1);

            if (dcll_pos_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_pos_delete_status == 0)
            {
                goto dcll_delete_pos_input;
            }

            int status = dcll_deleteAtPosition(&list, dcll_pos_delete_index);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else if (status == -2)
            {
                printf("\nInvalid position\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
    }

    dcll_destroy(&list);
}
