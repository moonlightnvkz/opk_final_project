//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdlib.h>

typedef void *Pointer;

typedef struct tQueue {
    Pointer *buffer;
    Pointer *buffer_end;
    Pointer *data_start;
    Pointer *data_end;
    size_t allocated;
    size_t size;
    size_t initial_size;
    size_t increment;
    int _isTuned;
} Queue;

/* Создать пустую очередь */
void queue_create(Queue *pqueue);

/* Уничтожить очередь, освободив выделенную память */
void queue_destroy(Queue *pqueue);

/* Поместить значение value в конец очереди */
void queue_enqueue(Queue *pqueue, Pointer value);

/* Возвращает количество элементов в очереди (0, если очередь пуста) */
size_t queue_size(Queue *pqueue);

/* Исключить и вернуть значение первого элемента очереди.
   Если очередь пуста, возвращает 0 */
Pointer queue_dequeue(Queue *pqueue);

/*
 * Возвращает значение первого элемента, не удаляя его из очереди.
 * Если очередь пуста, возвращает 0
 */
Pointer queue_peek(Queue *pqueue);

/*
 * Настраивает параметры очереди.
 * initial_size: начальный размер очереди, при первом выделении памяти
 *               (по умолч.: 50)
 * increment: на сколько элементов расширять очередь при последующих
 *            выделениях памяти
 */
void queue_tune(Queue *pqueue, size_t initial_size, size_t increment);

int queue_full(Queue *pqueue);