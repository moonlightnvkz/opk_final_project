//
// Created by moonlightnvkz on 19.11.16.
//

#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include "my_deque.h"

static void deque_resize(Deque *pdeque, size_t new_size);
static int deque_full(Deque *pqueue);

void deque_create(Deque *pdeque)
{
    assert(pdeque != NULL);
    if (pdeque->_isTuned != 1)
        deque_tune(pdeque, 100, 100);
    pdeque->buffer = malloc(sizeof(Pointer) * pdeque->initial_size);
    if (pdeque->buffer == NULL)
    {
        fprintf(stderr, "Can't allocate memory for circular buffer (queue_create)");
        exit(1);
    }
    pdeque->buffer_end = pdeque->buffer + pdeque->initial_size;
    pdeque->data_start = pdeque->data_end = pdeque->buffer;
    pdeque->size = 0;
}

void deque_destroy(Deque *pdeque)
{
    assert(pdeque != NULL);
    free(pdeque->buffer);
    pdeque->buffer = NULL;
    pdeque->buffer_end = pdeque->data_start = pdeque->data_end = 0;
    pdeque->increment = pdeque->initial_size = pdeque->size = pdeque->_isTuned = 0;
}

void deque_add_first(Deque *pdeque, Pointer value)
{
    assert(pdeque != NULL && pdeque->buffer != NULL);
    if (deque_full(pdeque) && pdeque->increment != 0)
        deque_resize(pdeque, pdeque->size + pdeque->increment);
    pdeque->data_start--;
    if (pdeque->data_start == pdeque->buffer - 1)
        pdeque->data_start = pdeque->buffer_end - 1;
    *pdeque->data_start = value;
    pdeque->size++;
}

void deque_add_last(Deque *pdeque, Pointer value)
{
    assert(pdeque != NULL && pdeque->buffer != NULL);
    if (deque_full(pdeque) && pdeque->increment != 0)
        deque_resize(pdeque, pdeque->size + pdeque->increment);
    *pdeque->data_end = value;
    pdeque->data_end++;
    if (pdeque->data_end == pdeque->buffer_end)
        pdeque->data_end = pdeque->buffer;
    pdeque->size++;
}

size_t deque_size(Deque *pdeque)
{
    assert(pdeque != NULL);
    return pdeque->size;
}

Pointer deque_remove_first(Deque *pdeque)
{
    assert(pdeque != NULL && pdeque->buffer != NULL);
    if (pdeque->size == 0)
        return 0;
    if (pdeque->increment != 0 && pdeque->buffer_end - pdeque->buffer - pdeque->size >= pdeque->increment)
        deque_resize(pdeque, pdeque->buffer_end - pdeque->buffer - pdeque->increment);
    Pointer poped = *pdeque->data_start;
    pdeque->data_start++;
    if (pdeque->data_start == pdeque->buffer_end)
        pdeque->data_start = pdeque->buffer;
    pdeque->size--;
    return poped;
}

Pointer deque_remove_last(Deque *pdeque)
{
    assert(pdeque != NULL && pdeque->buffer != NULL);
    if (pdeque->size == 0)
        return 0;
    Pointer poped = pdeque->data_end[-1];
    pdeque->data_end--;
    if (pdeque->data_end == pdeque->buffer - 1)
        pdeque->data_end = pdeque->buffer_end - sizeof(Pointer);
    pdeque->size--;
    if (pdeque->increment != 0 && pdeque->buffer_end - pdeque->buffer - pdeque->size >= pdeque->increment)
        deque_resize(pdeque, pdeque->buffer_end - pdeque->buffer - pdeque->increment);
    return poped;
}

static int deque_full(Deque *pdeque)
{
    assert(pdeque != NULL);
    return (size_t)(pdeque->buffer_end - pdeque->buffer) == pdeque->size;
}

// FIXED: REALLOC

static void deque_resize(Deque *pdeque, size_t new_size)
{
    if (pdeque->data_start != pdeque->buffer)
    {
        int shift = pdeque->data_start - pdeque->buffer;
        for (int i = shift; i < pdeque->buffer_end - pdeque->buffer; ++i)
        {
            Pointer t = pdeque->buffer[i - shift];
            pdeque->buffer[i - shift] = pdeque->buffer[i];
            pdeque->buffer[i] = t;
        }
    }

    Pointer *temp = realloc(pdeque->buffer, sizeof(Pointer) * new_size);
    if (!temp)
    {
        fprintf(stderr, "Can't reallocate memory (deque_resize)");
        exit(1);
    }
    pdeque->buffer = temp;
    pdeque->buffer_end = pdeque->buffer + new_size;
    pdeque->data_start = pdeque->buffer;
    pdeque->data_end = pdeque->buffer + pdeque->size;
}

Pointer deque_peek_first(Deque *pdeque)
{
    if (pdeque->size == 0)
        return NULL;
    return *pdeque->data_start;
}

Pointer deque_peek_last(Deque *pdeque)
{
    if (pdeque->size == 0)
        return NULL;
    if (pdeque->data_end == pdeque->buffer)
        return pdeque->buffer_end[-1];
    else
        return pdeque->data_end[-1];
}

void deque_tune(Deque *pdeque, size_t initial_size, size_t increment)
{
    assert(pdeque != NULL);
    pdeque->initial_size = initial_size;
    pdeque->increment = increment;
    pdeque->_isTuned = 1;
}