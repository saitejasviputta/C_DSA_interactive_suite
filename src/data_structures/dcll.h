#ifndef DCLL_H
#define DCLL_H

// For Doubly cirular linked list

typedef struct dcll_Node
{
    void* data;
    struct dcll_Node* next;
    struct dcll_Node* prev;
} dcll_Node;

typedef struct dcll
{
    dcll_Node* head;
    dcll_Node* tail;
    int length;
} dcll;

// Initialize a doubly circular linked list.
void dcll_init(dcll* list);

// Insert a value at the beginning of a doubly circular linked list. Returns 1 on success, -1 on
// failure.
int dcll_insertAtBeginning(dcll* list, void* value);

// Insert a value at the end of a doubly circular linked list. Returns 1 on success, -1 on failure.
int dcll_insertAtEnd(dcll* list, void* value);

// Insert a value at a specific position in a doubly circular linked list. Returns 1 on success, -1
// on failure, -2 on invalid position.
int dcll_insertAtPosition(dcll* list, void* value, int position);

// Delete the first element from a doubly circular linked list. Returns 1 on success, -1 if the list
// is empty.
int dcll_deleteAtBeginning(dcll* list, void (*free_data)(void*));

// Delete the last element from a doubly circular linked list. Returns 1 on success, -1 if the list
// is empty.
int dcll_deleteAtEnd(dcll* list, void (*free_data)(void*));

// Delete the first occurrence of a value from a doubly circular linked list. Returns 1 on success,
// -1 if not found, -2 if empty list.
int dcll_deleteByValue(dcll* list, const void* value, int (*compare)(const void*, const void*),
                       void (*free_data)(void*));

// Delete a node at a specific position in a doubly circular linked list. Returns 1 on success, -1
// if the list is empty, -2 on invalid position.
int dcll_deleteAtPosition(dcll* list, int position, void (*free_data)(void*));

// Search for a value in a doubly circular linked list. Returns The index of the value or -1 if not
// found.
int dcll_search(const dcll* list, const void* key, int (*compare)(const void*, const void*));

// Get the number of nodes in a doubly circular linked list. Returns The number of nodes in the
// list.
int dcll_getLength(const dcll* list);

// Print the contents of a doubly circular linked list.
void dcll_printlist(const dcll* list, void (*print_element)(const void*));

// Free resources used by a doubly circular linked list.
void dcll_destroy(dcll* list, void (*free_data)(void*));

// Run the doubly circular linked list demonstration module.
void dcll_demo(void);

#endif
