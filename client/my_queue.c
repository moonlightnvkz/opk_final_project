//
// Created by moonlightnvkz on 15.11.16.
//

#include "my_queue.h"

#include <malloc.h>
#include <assert.h>
//#include <stdio.h>

int queue_full(Queue *pqueue);

void queue_resize(Queue *pqueue, size_t new_size);

void queue_create(Queue *pqueue)
{
    assert(pqueue != NULL);
    if (pqueue->_isTuned != 1)
        queue_tune(pqueue, 50, 1);
    pqueue->buffer = malloc(sizeof(Pointer) * pqueue->initial_size);
    if (pqueue->buffer == NULL)
    {
        fprintf(stderr, "Can't allocate memory for circular buffer (queue_create)");
        exit(1);
    }
    pqueue->allocated = pqueue->initial_size;
    pqueue->buffer_end = pqueue->buffer + pqueue->allocated;
    pqueue->data_start = pqueue->data_end = pqueue->buffer;
    pqueue->size = 0;
}

void queue_destroy(Queue *pqueue)
{
    assert(pqueue != NULL);
    free(pqueue->buffer);
    pqueue->buffer = NULL;
    pqueue->allocated = 0;
    pqueue->buffer_end = pqueue->data_start = pqueue->data_end = 0;
    pqueue->increment = pqueue->initial_size = pqueue->size = pqueue->_isTuned = 0;
}

void queue_enqueue(Queue *pqueue, Pointer value)
{
    assert(pqueue != NULL && pqueue->buffer != NULL);
    if (queue_full(pqueue) && pqueue->increment != 0)
        queue_resize(pqueue, pqueue->size + pqueue->increment);
    *pqueue->data_end = value;
    pqueue->data_end++;
    if (pqueue->data_end == pqueue->buffer_end)
        pqueue->data_end = pqueue->buffer;
    pqueue->size++;
}

size_t queue_size(Queue *pqueue)
{
    assert(pqueue != NULL);
    return pqueue->size;
}

Pointer queue_dequeue(Queue *pqueue)
{
    assert(pqueue != NULL && pqueue->buffer != NULL);
    if (pqueue->size == 0)
        return 0;
    if (pqueue->increment != 0 && pqueue->allocated - pqueue->size >= pqueue->increment)
        queue_resize(pqueue, pqueue->allocated - pqueue->increment);
    Pointer poped = *pqueue->data_start;
    pqueue->data_start++;
    if (pqueue->data_start == pqueue->buffer_end)
        pqueue->data_start = pqueue->buffer;
    pqueue->size--;
    return poped;
}

int queue_full(Queue *pqueue)
{
    assert(pqueue != NULL);
    return pqueue->allocated == pqueue->size;
}

void queue_resize(Queue *pqueue, size_t new_size)
{
    Pointer *temp = malloc(sizeof(Pointer) * new_size);
    if (!temp)
    {
        fprintf(stderr, "Can't allocate memory for new circular buffer (queue_resize)");
        exit(1);
    }
    for (struct { size_t i; Pointer *p; } loopy = { 0, pqueue->data_start };
         loopy.i < pqueue->size;
         ++loopy.p, ++loopy.i)
    {
        if (loopy.p == pqueue->buffer_end)
            loopy.p = pqueue->buffer;
        temp[loopy.i] = *loopy.p;

    }
    free(pqueue->buffer);
    pqueue->buffer = temp;
    pqueue->buffer_end = pqueue->buffer + new_size;
    pqueue->data_start = pqueue->buffer;
    pqueue->data_end = pqueue->buffer + pqueue->size;
    if (pqueue->data_end == pqueue->buffer_end)
        pqueue->data_end = pqueue->buffer;
    pqueue->allocated = new_size;
    pqueue->buffer_end = pqueue->buffer + pqueue->allocated;
}

Pointer queue_peek(Queue *pqueue)
{
    if (pqueue->size == 0)
        return NULL;
    return *pqueue->data_start;
}

void queue_tune(Queue *pqueue, size_t initial_size, size_t increment)
{
    assert(pqueue != NULL);
    pqueue->initial_size = initial_size;
    pqueue->increment = increment;
    pqueue->_isTuned = 1;
}