#ifndef MTB_LIST_H
#define MTB_LIST_H

#include "mtb_macro.h"
#include "mtb_type.h"


typedef struct mtb_list_node MtbListNode;
struct mtb_list_node
{
    MtbListNode *next;
    MtbListNode *prev;
};


/* Doubly Linked List API */

#define _mtb_list_foreach(h, i, _h) \
    for (MtbListNode *_h = (h), *i = _h->next; i != _h; i = i->next)
#define mtb_list_foreach(h, i) _mtb_list_foreach(h, i, mtb_id(_h))

public void mtb_list_init(MtbListNode *head);
public bool mtb_list_is_empty(MtbListNode *head);
public MtbListNode *mtb_list_remove(MtbListNode *item);
public MtbListNode *mtb_list_insert_before(MtbListNode *pivot, MtbListNode *item);
#define mtb_list_insert_after(p, i) mtb_list_insert_before((p)->next, i)

#define mtb_list_add_last(h, i) mtb_list_insert_before(h, i)
public MtbListNode *mtb_list_remove_last(MtbListNode *head);
public MtbListNode *mtb_list_get_last(MtbListNode *head);

#define mtb_list_add_first(h, i) mtb_list_insert_after(h, i)
public MtbListNode *mtb_list_remove_first(MtbListNode *head);
public MtbListNode *mtb_list_get_first(MtbListNode *head);


/* Stack API */

#define mtb_list_push(s, i) mtb_list_add_last(s, i)
#define mtb_list_pop(s) mtb_list_remove_last(s)
#define mtb_list_top(s) mtb_list_get_last(s)


/* Queue API */

#define mtb_list_enq(q, i) mtb_list_add_last(q, i)
#define mtb_list_deq(q) mtb_list_remove_first(q)
#define mtb_list_front(q) mtb_list_get_first(q)

#endif //MTB_LIST_H
