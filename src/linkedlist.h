#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define ll_iter_get_as(it, type) ((type)ll_iter_get(it))

typedef struct ll_list_type *ll_list_t;
typedef struct ll_iter_type *ll_iter_t;

ll_list_t ll_new_list();
void ll_free(ll_list_t list);

void ll_push_front(ll_list_t list, void *item);
void ll_push_back(ll_list_t list, void *item);
void* ll_pop_front(ll_list_t list);
void* ll_pop_back(ll_list_t list);
unsigned long ll_size(ll_list_t list);
int ll_empty(ll_list_t list);
void ll_clear(ll_list_t list);
ll_iter_t ll_find(ll_list_t list, void *item);
/**
 * perform a stable inplace sort of list. smallest items first.
 * 
 */
void ll_sort(ll_list_t list, int (*cmp)(void *a, void *b));

ll_iter_t ll_iter(ll_list_t list);
ll_iter_t ll_iter_reverse(ll_list_t list);
void ll_iter_free(ll_iter_t iter);
void* ll_iter_get(ll_iter_t iter);
void ll_iter_advance(ll_iter_t iter);
void ll_iter_remove(ll_iter_t iter);
void ll_iter_insert_before(ll_iter_t iter, void *item);


#endif /* LINKEDLIST_H */
