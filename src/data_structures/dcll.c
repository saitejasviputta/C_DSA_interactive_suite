#include "dcll.h"
#include <stdio.h>
#include <stdlib.h>

// Doubly circular linked list. See data_structures.h for the head/tail/length invariant.
//
// A note on traversal: because the list is circular there is no NULL terminator to stop on.
// Every traversal here therefore walks at most `length` nodes, using the idiom
//     cur = head; do { ...; cur = cur->next; } while (cur != head);
// which visits each node exactly once and is guaranteed to terminate (it stops the moment it
// arrives back at head). This is what prevents the infinite loops a circular list invites.
//
// Unlike scll, each node also carries a prev pointer so deletions at the end are O(1) —
// no predecessor walk is needed.

// Allocates a node holding `value`. Returns NULL on malloc failure. The caller is responsible
// for linking it into the ring (next and prev are left as NULL on purpose).

static dcll_Node* dcll_create_node(int value)
{
    dcll_Node* node = malloc(sizeof(dcll_Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void dcll_init(dcll* list)
{
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

int dcll_insertAtBeginning(dcll* list, int value)
{
    dcll_Node* node = dcll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    if (list->head == NULL)
    {
        // First node: it must point to itself so that tail->next == head and head->prev == tail.
        node->next = node;
        node->prev = node;
        list->head = node;
        list->tail = node;
    }
    else
    {
        // New node becomes the head; tail keeps closing the ring onto the new head.
        node->next = list->head;
        node->prev = list->tail;
        list->head->prev = node;
        list->tail->next = node;
        list->head = node;
    }

    list->length++;
    return 1;
}

int dcll_insertAtEnd(dcll* list, int value)
{
    dcll_Node* node = dcll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    if (list->head == NULL)
    {
        // First node: point to itself in both directions.
        node->next = node;
        node->prev = node;
        list->head = node;
        list->tail = node;
    }
    else
    {
        // Link after the current tail and close the ring back onto head. O(1) thanks to tail.
        node->next = list->head;
        node->prev = list->tail;
        list->tail->next = node;
        list->head->prev = node;
        list->tail = node;
    }

    list->length++;
    return 1;
}

int dcll_insertAtPosition(dcll* list, int value, int position)
{
    // Valid insert positions are 0..length (length == append at the end).
    if (position < 0 || position > list->length)
    {
        return -2;
    }

    if (position == 0)
    {
        return dcll_insertAtBeginning(list, value);
    }
    if (position == list->length)
    {
        return dcll_insertAtEnd(list, value);
    }

    // Interior insert: walk to the node just before the target position.
    dcll_Node* node = dcll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    dcll_Node* prev = list->head;
    for (int i = 0; i < position - 1; i++)
    {
        prev = prev->next;
    }

    node->next = prev->next;
    node->prev = prev;
    prev->next->prev = node;
    prev->next = node;
    // head and tail are untouched here (position is strictly interior), invariant preserved.

    list->length++;
    return 1;
}

int dcll_deleteAtBeginning(dcll* list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    dcll_Node* old_head = list->head;

    if (list->head == list->tail)
    {
        // Single node deletes down to the empty list.
        free(old_head);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        list->head = old_head->next;
        list->head->prev = list->tail; // new head's prev closes onto tail
        list->tail->next = list->head; // tail closes ring onto new head
        free(old_head);
    }

    list->length--;
    return 1;
}

int dcll_deleteAtEnd(dcll* list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    dcll_Node* old_tail = list->tail;

    if (list->head == list->tail)
    {
        // Single node deletes down to the empty list.
        free(old_tail);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        // O(1) because prev pointer gives us the predecessor directly.
        list->tail = old_tail->prev;
        list->tail->next = list->head; // re-close the ring forward
        list->head->prev = list->tail; // re-close the ring backward
        free(old_tail);
    }

    list->length--;
    return 1;
}

int dcll_deleteByValue(dcll* list, int value)
{
    if (list->head == NULL)
    {
        return -2;
    }

    // Deleting the head value is delegated so the head/tail bookkeeping stays in one place.
    if (list->head->data == value)
    {
        return dcll_deleteAtBeginning(list);
    }

    // Search for the node holding `value`, bounded by one full lap.
    dcll_Node* cur = list->head->next;
    while (cur != list->head)
    {
        if (cur->data == value)
        {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            if (cur == list->tail)
            {
                list->tail = cur->prev; // removed the tail: prev becomes the new tail
            }
            free(cur);
            list->length--;
            return 1;
        }
        cur = cur->next;
    }

    return -1; // value not present
}

int dcll_deleteAtPosition(dcll* list, int position)
{
    if (list->head == NULL)
    {
        return -1;
    }
    // Valid delete positions are 0..length-1.
    if (position < 0 || position >= list->length)
    {
        return -2;
    }

    if (position == 0)
    {
        return dcll_deleteAtBeginning(list);
    }

    if (position == list->length - 1)
    {
        return dcll_deleteAtEnd(list);
    }

    // Walk to the target node.
    dcll_Node* target = list->head;
    for (int i = 0; i < position; i++)
    {
        target = target->next;
    }

    target->prev->next = target->next;
    target->next->prev = target->prev;
    free(target);

    list->length--;
    return 1;
}

int dcll_search(const dcll* list, int key)
{
    if (list->head == NULL)
    {
        return -1;
    }

    int index = 0;
    const dcll_Node* cur = list->head;
    do
    {
        if (cur->data == key)
        {
            return index;
        }
        cur = cur->next;
        index++;
    } while (cur != list->head);

    return -1;
}

int dcll_getLength(const dcll* list)
{
    return list->length;
}

void dcll_printlist(const dcll* list)
{
    printf("HEAD->");
    if (list->head == NULL)
    {
        printf("(empty)");
        return;
    }

    const dcll_Node* cur = list->head;
    do
    {
        printf("%d->", cur->data);
        cur = cur->next;
    } while (cur != list->head);

    printf("(back to HEAD)");
}

void dcll_destroy(dcll* list)
{
    if (list->head == NULL)
    {
        return;
    }

    // Break the ring first so the walk has a NULL terminator and we never revisit a freed node.
    list->tail->next = NULL;
    dcll_Node* cur = list->head;
    while (cur != NULL)
    {
        dcll_Node* upcoming = cur->next;
        free(cur);
        cur = upcoming;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}
