/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"

/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int (*comparer)(const void *, const void *))
{
  q->head = NULL;
  q->tail = q->head;
  q->index = q->head;
  q->comparer = comparer;
  q->size = 0;
}

/**
  Insert the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
  int index = 0;
  node *new_elem = (node *)malloc(1 * sizeof(node));
  new_elem->element = ptr;
  new_elem->next = NULL;
  node *cursor = q->head, *prev_cursor = NULL;

  while (cursor != NULL)
  {
    if (q->comparer(cursor->element, new_elem->element) <= 0)
    {
      index++;
      if (cursor->next == NULL)
      {
        cursor->next = new_elem;
        break;
      }
      else
      {
        prev_cursor = cursor;
        cursor = cursor->next;
      }
    }
    else
    {
      new_elem->next = cursor;
      if (prev_cursor != NULL)
      {
        prev_cursor->next = new_elem;
      }
      break;
    }
  }

  if (index == 0)
  {
    q->head = new_elem;
  }
  q->size++;
  return index;
}

/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
  if (q->head != NULL)
  {
    return q->head->element;
  }
  else
  {
    return NULL;
  }
}

/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
  if (q->head == NULL)
  {
    return NULL;
  }
  else
  {
    node *temp = q->head->next;
    void *elem = q->head->element;
    free(q->head);
    q->head = temp;
    q->size--;
    return elem;
  }
}

/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  if (index >= q->size)
  {
    return NULL;
  }
  else
  {
    node *temp = q->head;
    for (int i = 0; i < index; i++)
    {
      temp = temp->next;
    }
    return temp->element;
  }
}

/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
  if (q->head == NULL) {
    return 0;
  } 
  else 
  {
    int sum = 0;
    node* temp = q->head;
    node* jump = temp;

    while (jump != NULL) 
    {
      Node* temp2 = temp->next;
      if (temp == ptr) 
      { 
        sum++;
        free(temp);
      }
      jump = temp2;
      jump = jump->next;
    }
    return sum;
  }
}

/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
  if (index >= q->size)
  {
    return NULL;
  }
  else
  {
    node *temp = q->head;
    void *element;
    for (int i = 0; i < index - 1; i++)
    {
      temp = temp->next;
    }

    temp->next = temp->next->next;
    element = temp->next->element;
    free(temp->next);
    q->size--;

    return element;
  }
}

/**
  Return the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
  return q->size;
}

/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  node *temp = q->head;
  node* prev_temp;
  while (temp != NULL)
  {
    prev_temp = temp;
    temp = temp->next;
    free(prev_temp);
  }

  q->head = NULL;
  q->tail = NULL;
  q->index = NULL;
  q->size = 0;
}
