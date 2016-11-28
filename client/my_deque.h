//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <stdlib.h>

typedef char *Pointer;

typedef struct tDeque {
    Pointer *buffer;
    Pointer *buffer_end;
    Pointer *data_start;
    Pointer *data_end;
    size_t size;
    size_t initial_size;
    size_t increment;
    int _isTuned;
} Deque;

typedef struct tDequeIterator {
    Pointer *data;
} Iterator;

/* Создать пустую очередь */
void deque_create(Deque *pqueue);

/* Уничтожить очередь, освободив выделенную память */
void deque_destroy(Deque *pqueue);

/* Поместить значение value в конец очереди */
void deque_add_first(Deque *pqueue, Pointer value);

void deque_add_last(Deque *pqueue, Pointer value);

/* Возвращает количество элементов в очереди (0, если очередь пуста) */
size_t deque_size(Deque *pqueue);

/* Исключить и вернуть значение первого элемента очереди.
   Если очередь пуста, возвращает 0 */
Pointer deque_remove_first(Deque *pqueue);

Pointer deque_remove_last(Deque *pqueue);

/*
 * Возвращает значение первого элемента, не удаляя его из очереди.
 * Если очередь пуста, возвращает 0
 */
Pointer deque_peek_first(Deque *pqueue);

Pointer deque_peek_last(Deque *pqueue);

/*
 * Настраивает параметры очереди.
 * initial_size: начальный размер очереди, при первом выделении памяти
 *               (по умолч.: 50)
 * increment: на сколько элементов расширять очередь при последующих
 *            выделениях памяти
 */
void deque_tune(Deque *pqueue, size_t initial_size, size_t increment);

Pointer deque_iterator_next(Deque *pqueue, Iterator *it);

Pointer deque_iterator_get_data(Iterator *it);

Iterator *deque_iterator_create(Deque *pqueue);

void deque_iterator_destroy(Iterator *it);