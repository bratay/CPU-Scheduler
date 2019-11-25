/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_

typedef int (*compare)(const void *, const void *);
typedef void(*helperHelp)(void *);

typedef struct node
{
  void *element;
  struct node *next;
} node;

/**
  Priqueue Data Structure
*/
typedef struct _priqueue_t
{
  node *head;
  node *tail;
  node *index;
  compare comparer;
  int size;
} priqueue_t;

void priqueue_init(priqueue_t *q, int (*comparer)(const void *, const void *));

int priqueue_offer(priqueue_t *q, void *ptr);
void *priqueue_peek(priqueue_t *q);
void *priqueue_poll(priqueue_t *q);
void *priqueue_at(priqueue_t *q, int index);
int priqueue_remove(priqueue_t *q, void *ptr);
void *priqueue_remove_at(priqueue_t *q, int index);
int priqueue_size(priqueue_t *q);

void priqueue_destroy(priqueue_t *q);

#endif /* LIBPQUEUE_H_ */
