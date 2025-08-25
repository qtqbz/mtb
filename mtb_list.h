#ifndef MTB_LIST_H
#define MTB_LIST_H

#include "mtb_macro.h"


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
public void mtb_list_reset(MtbListNode *head);
public bool mtb_list_is_empty(MtbListNode *head);

public MtbListNode *mtb_list_remove(MtbListNode *item);
public MtbListNode *mtb_list_insert_before(MtbListNode *pivot, MtbListNode *item);
public MtbListNode *mtb_list_insert_after(MtbListNode *pivot, MtbListNode *item);

public MtbListNode *mtb_list_add_last(MtbListNode *head, MtbListNode *item);
public MtbListNode *mtb_list_remove_last(MtbListNode *head);
public MtbListNode *mtb_list_get_last(MtbListNode *head);

public MtbListNode *mtb_list_add_first(MtbListNode *head, MtbListNode *item);
public MtbListNode *mtb_list_remove_first(MtbListNode *head);
public MtbListNode *mtb_list_get_first(MtbListNode *head);


/* Stack API */

public MtbListNode *mtb_list_push(MtbListNode *head, MtbListNode *item);
public MtbListNode *mtb_list_pop(MtbListNode *head);
public MtbListNode *mtb_list_top(MtbListNode *head);


/* Queue API */

public MtbListNode *mtb_list_enq(MtbListNode *head, MtbListNode *item);
public MtbListNode *mtb_list_deq(MtbListNode *head);
public MtbListNode *mtb_list_front(MtbListNode *head);

#endif //MTB_LIST_H
