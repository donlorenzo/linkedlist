#include "linkedlist.h"

#include <stdlib.h>


typedef struct list_node_type {
    struct list_node_type *next, *previous;
    void *data;
} ll_node_type;

typedef struct {
    ll_node_type *head, *tail;
    unsigned long size;
} ll_list_type;

typedef struct {
    ll_list_type *l;
    ll_node_type *current;
    int reverse;
} ll_iter_type;


static ll_node_type* merge_sort(ll_node_type *head,
                                int (*cmp)(void *a, void *b));
static ll_node_type* merge_sort_merge(ll_node_type *a, ll_node_type *b,
                                      int (*cmp)(void *a, void *b));
static ll_node_type* merge_sort_get_middle(ll_node_type *head);


static ll_node_type* ll_new_node(void *data) {
    ll_node_type *node = (ll_node_type*)malloc(sizeof(ll_node_type));
    node->previous = node->next = NULL;
    node->data = data;
    return node;
}

static void ll_node_free(ll_node_type *node) {
    free(node);
}

ll_list_t ll_new_list() {
    ll_list_type *list = (ll_list_type*)malloc(sizeof(ll_list_type));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return (ll_list_t)list;
}

void ll_free(ll_list_t list) {
    free((ll_list_t*)list);
}

unsigned long ll_size(ll_list_t list) {
    ll_list_type *l = (ll_list_type*)list;
    return l->size;
}

int ll_empty(ll_list_t list) {
    ll_list_type *l = (ll_list_type*)list;
    return l->size == 0;
}

void ll_clear(ll_list_t list) {
    ll_iter_t it = ll_iter(list);
    while(!ll_empty(list)) {
        ll_iter_remove(it);
    }
}

void ll_push_front(ll_list_t list, void *item) {
    ll_list_type *l = (ll_list_type*)list;
    ll_node_type *n = ll_new_node(item);
    n->next = l->head;
    /* set head */
    if (l->head) {
        l->head->previous = n;
    }
    l->head = n;
    /* set tail */
    if (l->tail == NULL) {
        l->tail = n;
    }
    l->size++;
}

void ll_push_back(ll_list_t list, void *item) {
    ll_list_type *l = (ll_list_type*)list;
    ll_node_type *n = ll_new_node(item);
    n->previous = l->tail;
    /* set tail */
    if (l->tail) {
        l->tail->next = n;
    }
    l->tail = n;
    /* set head */
    if (l->head == NULL) {
        l->head = n;
    }
    l->size++;
}


void* ll_pop_front(ll_list_t list) {
    ll_node_type *n;
    void *item;
    ll_list_type *l = (ll_list_type*)list;
    if (l->size == 0) {
        return NULL;
    }
    n = l->head;
    l->head = l->head->next;
    if (l->head) {
        l->head->previous = NULL;
    }
    l->size--;
    if (l->size == 0) {
        l->tail = NULL;
    }
    item = n->data;
    ll_node_free(n);
    return item;
}

void* ll_pop_back(ll_list_t list) {
    ll_node_type *n;
    void *item;
    ll_list_type *l = (ll_list_type*)list;
    if (l->size == 0) {
        return NULL;
    }
    n = l->tail;
    l->tail = l->tail->previous;
    if (l->tail) {
        l->tail->next = NULL;
    }
    l->size--;
    if (l->size == 0) {
        l->head = NULL;
    }
    item = n->data;
    ll_node_free(n);
    return item;
}

ll_iter_t ll_find(ll_list_t list, void *item) {
    ll_iter_t it = ll_iter(list);
    void *current_item;
    while ((current_item = ll_iter_get(it)) != NULL &&
           (current_item != item)) {
        ll_iter_advance(it);
    }
    return it;
}

void ll_copy(ll_list_t src, ll_list_t dst) {
    void *item;
    ll_iter_t it = ll_iter(src);
    ll_clear(dst);
    while ((item = ll_iter_get(it)) != NULL) {
        ll_push_back(dst, item);
        ll_iter_advance(it);
    }
    ll_iter_free(it);
}

void ll_sort(ll_list_t list, int (*cmp)(void *a, void *b)) {
    ll_node_type *n;
    ll_list_type *l = (ll_list_type*)list;
    l->head = merge_sort(l->head, cmp);
    /* merge_sort breaks double linkage so we need to repair it */
    n = l->head;
    if (n != NULL) {
        n->previous = NULL;
        while (n->next) {
            n->next->previous = n;
            n = n->next;
        }
    }
    l->tail = n;
}

/* adapted from https://stackoverflow.com/a/8238253 */
/* sorts a singelly-linked list recursively with the merge_sort algorithm */
static ll_node_type* merge_sort(ll_node_type *head, int (*cmp)(void *a, void *b)) {
    ll_node_type *middle, *second_half;
    /* break recursion if zero or one element */
    if (head == NULL || head->next == NULL) {
        return head;
    }
    /* find middle and break list into two */
    middle = merge_sort_get_middle(head);
    second_half = middle->next;
    middle->next = NULL;
    /* recurse and merge */
    return merge_sort_merge(merge_sort(head, cmp),
                            merge_sort(second_half, cmp), cmp);
}

/* merge two lists and return the new head */
static ll_node_type* merge_sort_merge(ll_node_type *a, ll_node_type *b,
                                      int (*cmp)(void *a, void *b)) {
    ll_node_type *dummy_head, *current;
    if (a == NULL && b == NULL) {
        return NULL;
    }
    dummy_head = ll_new_node(NULL);
    current = dummy_head;
    /* merge as long as neither list is exhausted */
    while (a != NULL && b != NULL) {
        /* pick smallest element from lists and append to merge list */
        if (cmp(a->data, b->data) <= 0) {
            current->next = a;
            a = a->next;
        } else {
            current->next = b;
            b = b->next;
        }
        current = current->next;
    }
    /* append the list that hasn't been exhausted, yet */
    current->next = (a == NULL) ? b : a;
    /* get rid of dummy head */
    current = dummy_head->next;
    ll_node_free(dummy_head);
    return current;
}

/* return the middle node of a linked list */
static ll_node_type* merge_sort_get_middle(ll_node_type *head) {
    ll_node_type *fast_ptr, *slow_ptr;
    if (head == NULL) {
        return head;
    }
    fast_ptr = slow_ptr = head;
    /* step through list while fast_ptr moves twice as fast as slow_ptr */
    while (fast_ptr->next != NULL && fast_ptr->next->next != NULL) {
        slow_ptr = slow_ptr->next;
        fast_ptr = fast_ptr->next->next;
    }
    return slow_ptr;
}



/**************
 * Iterator *
 *************/

ll_iter_t ll_iter(ll_list_t list) {
    ll_iter_type *iter = (ll_iter_type*)malloc(sizeof(ll_iter_type));
    iter->l = (ll_list_type*)list;
    iter->current = iter->l->head;
    iter->reverse = 0;
    return (ll_iter_t)iter;
}

ll_iter_t ll_iter_reverse(ll_list_t list) {
    ll_iter_type *iter = (ll_iter_type*)malloc(sizeof(ll_iter_type));
    iter->l = (ll_list_type*)list;
    iter->current = iter->l->tail;
    iter->reverse = 1;
    return (ll_iter_t)iter;
}

void ll_iter_free(ll_iter_t iter) {
    free((ll_iter_type*)iter);
}

void* ll_iter_get(ll_iter_t iter) {
    ll_iter_type *it = (ll_iter_type*)iter;
    return (it->current ? it->current->data : NULL);
}

void ll_iter_advance(ll_iter_t iter) {
    ll_iter_type *it = (ll_iter_type*)iter;
    if (it->current) {
        if (it->reverse) {
            it->current = it->current->previous;
        } else {
            it->current = it->current->next;
        }
    }
}

void ll_iter_remove(ll_iter_t iter) {
    ll_iter_type *it = (ll_iter_type*)iter;
    ll_node_type *n;
    if (it->current) {
        n = it->current;
        ll_iter_advance(iter);
        ll_node_free(n);
        it->l->size--;
    }
}

void ll_iter_insert_before(ll_iter_t iter, void *item) {
    ll_iter_type *it = (ll_iter_type*)iter;
    ll_node_type *n = ll_new_node(item);
    if (it->reverse) {
        n->next = it->current->next;
        n->previous = it->current;
        n->next->previous = n;
        it->current->next = n;
    } else {
        n->previous = it->current->previous;
        n->next = it->current;
        n->previous->next = n;
        it->current->previous = n;
    }
}

