//
// Created by cyx02 on 2022/6/27.
//

#ifndef BUBBLEMAC_INFOCOM_COMMON_H
#define BUBBLEMAC_INFOCOM_COMMON_H
#include <cstdio>
/*
 *  A general item, with two-direction pointers
 */
struct Item
{
    void *datap;
    struct Item *prev;
    struct Item *next;
};

/* dual list, the next pointer is NULL when
 * no more items at the tail of the list; the
 * prev pointer of the first item points to
 * the last item in the list.
 */
struct Duallist
{
    //Struct Duallist
    struct Item *head;
    unsigned long nItems;
};

void duallist_init(struct Duallist *duallist);
void duallist_destroy(struct Duallist *duallist, void (*free_func)(void*));
void duallist_dump(FILE *fOutput, struct Duallist *duallist, void(*dump_func)(FILE*, void*));
void duallist_load(FILE *fInput, struct Duallist *duallist, void*(*load_func)(FILE*));
struct Item* duallist_add_to_head(struct Duallist *duallist, void *data);
struct Item* duallist_add_to_tail(struct Duallist *duallist, void *data);
struct Item* duallist_add_in_sequence_from_head(struct Duallist *duallist, void* data, int(*sort_func)(void*, void*));
struct Item* duallist_add_in_sequence_from_tail(struct Duallist *duallist, void* data, int(*sort_func)(void*, void*));
struct Item* duallist_add_before_item(struct Duallist *duallist, struct Item *fItem, struct Item *lItem, void *data);
struct Item* duallist_add_unique(struct Duallist *duallist, void *data, int(*judge_func)(void*, void*));
struct Item* duallist_find(struct Duallist *duallist, void *key, int(*judge_func)(void*, void*));
void* duallist_pick(struct Duallist *duallist, void *key, int(*judge_func)(void*, void*));
void* duallist_pick_item(struct Duallist *duallist, struct Item *theItem);
void* duallist_pick_head(struct Duallist *duallist);
void* duallist_pick_tail(struct Duallist *duallist);
int is_duallist_empty(struct Duallist *duallist);
struct Duallist* duallist_copy_by_reference(struct Duallist *destDuallist, struct Duallist *aDaullist);
struct Duallist* duallist_copy(struct Duallist *destDuallist, struct Duallist *aDuallist, void*(*copy_func)(void*));
struct Duallist* duallist_reverse_copy(struct Duallist *destDuallist, struct Duallist *aDuallist, void*(*copy_func)(void*));
unsigned long distance_to_tail(struct Item *aItem);
unsigned long distance_to_head(struct Duallist *aDuallist, struct Item *aItem);
int is_sublist(struct Duallist *aList, struct Duallist *bList, int(*equal_func)(void*, void*));
void duallist_remove_loops(struct Duallist *aList, int(*equal_func)(void*,void*), void(*free_func)(void*));



#endif //BUBBLEMAC_INFOCOM_COMMON_H
