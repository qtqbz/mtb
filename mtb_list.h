#ifndef MTB_LIST_H
#define MTB_LIST_H

#include "mtb_macro.h"
#include "mtb_type.h"


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

public void mtb_list_init(MtbList *head);
public void mtb_list_reset(MtbList *head);
public bool mtb_list_is_empty(MtbList *head);

public MtbList *mtb_list_remove(MtbList *item);
public MtbList *mtb_list_insert_before(MtbList *pivot, MtbList *item);
public MtbList *mtb_list_insert_after(MtbList *pivot, MtbList *item);

public MtbList *mtb_list_add_last(MtbList *head, MtbList *item);
public MtbList *mtb_list_remove_last(MtbList *head);
public MtbList *mtb_list_get_last(MtbList *head);

public MtbList *mtb_list_add_first(MtbList *head, MtbList *item);
public MtbList *mtb_list_remove_first(MtbList *head);
public MtbList *mtb_list_get_first(MtbList *head);


/* Stack API */

public MtbList *mtb_list_push(MtbList *head, MtbList *item);
public MtbList *mtb_list_pop(MtbList *head);
public MtbList *mtb_list_top(MtbList *head);


/* Queue API */

public MtbList *mtb_list_enq(MtbList *head, MtbList *item);
public MtbList *mtb_list_deq(MtbList *head);
public MtbList *mtb_list_front(MtbList *head);


/* Iterator API */

typedef struct mtb_list_iter MtbListIter;
struct mtb_list_iter
{
    MtbList *head;
    MtbList *prev;
    MtbList *next;
};


public void mtb_list_iter_init(MtbListIter *it, MtbList *head);
public void mtb_list_iter_reset(MtbListIter *it);
public bool mtb_list_iter_has_next(MtbListIter *it);
public void *mtb_list_iter_next(MtbListIter *it);
public void *mtb_list_iter_remove(MtbListIter *it);

#endif //MTB_LIST_H
