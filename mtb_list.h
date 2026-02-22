#ifndef MTB_LIST_H
#define MTB_LIST_H

#ifdef MTB_IMPLEMENTATION
#define MTB_LIST_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_LIST_TESTS
#endif


typedef struct mtb_list_node MtbList;
struct mtb_list_node
{
    MtbList *next;
    MtbList *prev;
};


/* Doubly Linked List API */

#define _mtb_list_foreach(head, var, _head) \
    for (MtbList *_head = (head), *var = _head->next; var != _head; var = var->next)
#define mtb_list_foreach(head, var) _mtb_list_foreach(head, var, mtb_id(_head))

func void mtb_list_init(MtbList *head);
func void mtb_list_reset(MtbList *head);
func bool mtb_list_is_empty(MtbList *head);

func MtbList *mtb_list_remove(MtbList *item);
func MtbList *mtb_list_insert_before(MtbList *pivot, MtbList *item);
func MtbList *mtb_list_insert_after(MtbList *pivot, MtbList *item);

func MtbList *mtb_list_add_last(MtbList *head, MtbList *item);
func MtbList *mtb_list_remove_last(MtbList *head);
func MtbList *mtb_list_get_last(MtbList *head);

func MtbList *mtb_list_add_first(MtbList *head, MtbList *item);
func MtbList *mtb_list_remove_first(MtbList *head);
func MtbList *mtb_list_get_first(MtbList *head);


/* Stack API */

func MtbList *mtb_list_push(MtbList *head, MtbList *item);
func MtbList *mtb_list_pop(MtbList *head);
func MtbList *mtb_list_top(MtbList *head);


/* Queue API */

func MtbList *mtb_list_enq(MtbList *head, MtbList *item);
func MtbList *mtb_list_deq(MtbList *head);
func MtbList *mtb_list_front(MtbList *head);


/* Iterator API */

typedef struct mtb_list_iter MtbListIter;
struct mtb_list_iter
{
    MtbList *head;
    MtbList *prev;
    MtbList *next;
};


func void mtb_list_iter_init(MtbListIter *it, MtbList *head);
func void mtb_list_iter_reset(MtbListIter *it);
func bool mtb_list_iter_has_next(MtbListIter *it);
func void *mtb_list_iter_next(MtbListIter *it);
func void *mtb_list_iter_remove(MtbListIter *it);

#endif //MTB_LIST_H


#ifdef MTB_LIST_IMPLEMENTATION

func void
mtb_list_init(MtbList *head)
{
    head->next = head->prev = head;
}

func void
mtb_list_reset(MtbList *head)
{
    mtb_list_init(head);
}

func bool
mtb_list_is_empty(MtbList *head)
{
    return head->next == head;
}

func MtbList *
mtb_list_remove(MtbList *item)
{
    item->prev->next = item->next;
    item->next->prev = item->prev;
    return item;
}

func MtbList *
mtb_list_insert_before(MtbList *pivot, MtbList *item)
{
    item->prev = pivot->prev;
    item->next = pivot;
    pivot->prev->next = item;
    pivot->prev = item;
    return item;
}

func MtbList *
mtb_list_insert_after(MtbList *pivot, MtbList *item)
{
    return mtb_list_insert_before(pivot->next, item);
}

func MtbList *
mtb_list_add_last(MtbList *head, MtbList *item)
{
    return mtb_list_insert_before(head, item);
}

func MtbList *
mtb_list_remove_last(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : mtb_list_remove(head->prev);
}

func MtbList *
mtb_list_get_last(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : head->prev;
}

func MtbList *
mtb_list_add_first(MtbList *head, MtbList *item)
{
    return mtb_list_insert_after(head, item);
}

func MtbList *
mtb_list_remove_first(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : mtb_list_remove(head->next);
}

func MtbList *
mtb_list_get_first(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : head->next;
}

func MtbList *
mtb_list_push(MtbList *head, MtbList *item)
{
    return mtb_list_add_last(head, item);
}

func MtbList *
mtb_list_pop(MtbList *head)
{
    return mtb_list_remove_last(head);
}

func MtbList *
mtb_list_top(MtbList *head)
{
    return mtb_list_get_last(head);
}

func MtbList *
mtb_list_enq(MtbList *head, MtbList *item)
{
    return mtb_list_add_last(head, item);
}

func MtbList *
mtb_list_deq(MtbList *head)
{
    return mtb_list_remove_first(head);
}

func MtbList *
mtb_list_front(MtbList *head)
{
    return mtb_list_get_first(head);
}

func void
mtb_list_iter_init(MtbListIter *it, MtbList *head)
{
    it->head = head;
    mtb_list_iter_reset(it);
}

func void
mtb_list_iter_reset(MtbListIter *it)
{
    it->prev = it->head;
    it->next = nil;
}

func bool
mtb_list_iter_has_next(MtbListIter *it)
{
    it->next = it->prev->next;
    return it->next != it->head;
}

func void *
mtb_list_iter_next(MtbListIter *it)
{
    mtb_assert_always(it->next != nil);
    it->prev = it->next;
    it->next = nil;
    return it->prev;
}

func void *
mtb_list_iter_remove(MtbListIter *it)
{
    mtb_assert_always(it->prev != it->head);
    return mtb_list_remove(it->prev);
}

#endif // MTB_LIST_IMPLEMENTATION


#ifdef MTB_LIST_TESTS

#include <assert.h>


func void
_test_mtb_list_list(void)
{
    MtbList list = {0};
    MtbList first = {0};
    MtbList last = {0};
    MtbList *expOrder[] = {&first, &last};

    mtb_list_init(&list);
    assert(mtb_list_is_empty(&list));

    mtb_list_add_first(&list, &first);
    mtb_list_add_last(&list, &last);
    assert(!mtb_list_is_empty(&list));

    u32 i = 0;
    mtb_list_foreach(&list, item) {
        MtbList *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_get_first(&list) == &first);
    assert(mtb_list_get_last(&list) == &last);
    assert(mtb_list_remove_first(&list) == &first);
    assert(mtb_list_remove_last(&list) == &last);
    assert(mtb_list_is_empty(&list));
}

func void
_test_mtb_list_stack(void)
{
    MtbList stack = {0};
    MtbList itemA = {0};
    MtbList itemB = {0};
    MtbList itemC = {0};
    MtbList *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&stack);
    assert(mtb_list_is_empty(&stack));

    mtb_list_push(&stack, &itemA);
    mtb_list_push(&stack, &itemB);
    mtb_list_push(&stack, &itemC);
    assert(!mtb_list_is_empty(&stack));

    u32 i = 0;
    mtb_list_foreach(&stack, item) {
        MtbList *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_top(&stack) == &itemC);
    assert(mtb_list_pop(&stack) == &itemC);
    assert(mtb_list_top(&stack) == &itemB);
    assert(mtb_list_pop(&stack) == &itemB);
    assert(mtb_list_top(&stack) == &itemA);
    assert(mtb_list_pop(&stack) == &itemA);
    assert(mtb_list_is_empty(&stack));
}

func void
_test_mtb_list_queue(void)
{
    MtbList queue = {0};
    MtbList itemA = {0};
    MtbList itemB = {0};
    MtbList itemC = {0};
    MtbList *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&queue);
    assert(mtb_list_is_empty(&queue));

    mtb_list_enq(&queue, &itemA);
    mtb_list_enq(&queue, &itemB);
    mtb_list_enq(&queue, &itemC);
    assert(!mtb_list_is_empty(&queue));

    u32 i = 0;
    mtb_list_foreach(&queue, item) {
        MtbList *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_front(&queue) == &itemA);
    assert(mtb_list_deq(&queue) == &itemA);
    assert(mtb_list_front(&queue) == &itemB);
    assert(mtb_list_deq(&queue) == &itemB);
    assert(mtb_list_front(&queue) == &itemC);
    assert(mtb_list_deq(&queue) == &itemC);
    assert(mtb_list_is_empty(&queue));
}

func void
_test_mtb_list_iter(void)
{
    MtbList list = {0};
    MtbList itemA = {0};
    MtbList itemB = {0};
    MtbList itemC = {0};
    MtbList *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&list);
    assert(mtb_list_is_empty(&list));

    MtbListIter it = {0};
    mtb_list_iter_init(&it, &list);
    assert(!mtb_list_iter_has_next(&it));

    mtb_list_add_last(&list, &itemA);
    mtb_list_add_last(&list, &itemB);
    mtb_list_add_last(&list, &itemC);
    assert(!mtb_list_is_empty(&list));

    mtb_list_iter_reset(&it);
    assert(mtb_list_iter_has_next(&it));

    u32 i = 0;
    while (mtb_list_iter_has_next(&it)) {
        MtbList *expItem = expOrder[i++];
        assert(mtb_list_iter_next(&it) == expItem);
        assert(mtb_list_iter_remove(&it) == expItem);
    }

    mtb_list_iter_reset(&it);
    assert(!mtb_list_iter_has_next(&it));
    assert(mtb_list_is_empty(&list));
}

func void
_test_mtb_list(void)
{
    _test_mtb_list_list();
    _test_mtb_list_stack();
    _test_mtb_list_queue();
    _test_mtb_list_iter();
}

#endif // MTB_LIST_TESTS
