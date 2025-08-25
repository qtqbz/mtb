#include "mtb_list.h"
#include "mtb_type.h"


public void
mtb_list_init(MtbListNode *head)
{
    head->next = head->prev = head;
}

public void
mtb_list_reset(MtbListNode *head)
{
    mtb_list_init(head);
}

public bool
mtb_list_is_empty(MtbListNode *head)
{
    return head->next == head;
}

public MtbListNode *
mtb_list_remove(MtbListNode *item)
{
    item->prev->next = item->next;
    item->next->prev = item->prev;
    return item;
}

public MtbListNode *
mtb_list_insert_before(MtbListNode *pivot, MtbListNode *item)
{
    item->prev = pivot->prev;
    item->next = pivot;
    pivot->prev->next = item;
    pivot->prev = item;
    return item;
}

public MtbListNode *
mtb_list_insert_after(MtbListNode *pivot, MtbListNode *item)
{
    return mtb_list_insert_before(pivot->next, item);
}

public MtbListNode *
mtb_list_add_last(MtbListNode *head, MtbListNode *item)
{
    return mtb_list_insert_before(head, item);
}

public MtbListNode *
mtb_list_remove_last(MtbListNode *head)
{
    return mtb_list_is_empty(head) ? nil : mtb_list_remove(head->prev);
}

public MtbListNode *
mtb_list_get_last(MtbListNode *head)
{
    return mtb_list_is_empty(head) ? nil : head->prev;
}

public MtbListNode *
mtb_list_add_first(MtbListNode *head, MtbListNode *item)
{
    return mtb_list_insert_after(head, item);
}

public MtbListNode *
mtb_list_remove_first(MtbListNode *head)
{
    return mtb_list_is_empty(head) ? nil : mtb_list_remove(head->next);
}

public MtbListNode *
mtb_list_get_first(MtbListNode *head)
{
    return mtb_list_is_empty(head) ? nil : head->next;
}

public MtbListNode *
mtb_list_push(MtbListNode *head, MtbListNode *item)
{
    return mtb_list_add_last(head, item);
}

public MtbListNode *
mtb_list_pop(MtbListNode *head)
{
    return mtb_list_remove_last(head);
}

public MtbListNode *
mtb_list_top(MtbListNode *head)
{
    return mtb_list_get_last(head);
}

public MtbListNode *
mtb_list_enq(MtbListNode *head, MtbListNode *item)
{
    return mtb_list_add_last(head, item);
}

public MtbListNode *
mtb_list_deq(MtbListNode *head)
{
    return mtb_list_remove_first(head);
}

public MtbListNode *
mtb_list_front(MtbListNode *head)
{
    return mtb_list_get_first(head);
}


#ifdef MTB_LIST_TESTS

#include <assert.h>


intern void
test_mtb_list_list(void)
{
    MtbListNode list = {0};
    MtbListNode first = {0};
    MtbListNode last = {0};
    MtbListNode *expOrder[] = {&first, &last};

    mtb_list_init(&list);
    assert(mtb_list_is_empty(&list));

    mtb_list_add_first(&list, &first);
    mtb_list_add_last(&list, &last);
    assert(!mtb_list_is_empty(&list));

    u32 i = 0;
    mtb_list_foreach(&list, item) {
        MtbListNode *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_get_first(&list) == &first);
    assert(mtb_list_get_last(&list) == &last);
    assert(mtb_list_remove_first(&list) == &first);
    assert(mtb_list_remove_last(&list) == &last);
    assert(mtb_list_is_empty(&list));
}

intern void
test_mtb_list_stack(void)
{
    MtbListNode stack = {0};
    MtbListNode itemA = {0};
    MtbListNode itemB = {0};
    MtbListNode itemC = {0};
    MtbListNode *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&stack);
    assert(mtb_list_is_empty(&stack));

    mtb_list_push(&stack, &itemA);
    mtb_list_push(&stack, &itemB);
    mtb_list_push(&stack, &itemC);
    assert(!mtb_list_is_empty(&stack));

    u32 i = 0;
    mtb_list_foreach(&stack, item) {
        MtbListNode *expItem = expOrder[i++];
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

intern void
test_mtb_list_queue(void)
{
    MtbListNode queue = {0};
    MtbListNode itemA = {0};
    MtbListNode itemB = {0};
    MtbListNode itemC = {0};
    MtbListNode *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&queue);
    assert(mtb_list_is_empty(&queue));

    mtb_list_enq(&queue, &itemA);
    mtb_list_enq(&queue, &itemB);
    mtb_list_enq(&queue, &itemC);
    assert(!mtb_list_is_empty(&queue));

    u32 i = 0;
    mtb_list_foreach(&queue, item) {
        MtbListNode *expItem = expOrder[i++];
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

intern void
test_mtb_list(void)
{
    test_mtb_list_list();
    test_mtb_list_stack();
    test_mtb_list_queue();
}

#endif // MTB_LIST_TESTS
