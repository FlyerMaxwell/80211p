//
// Created by cyx02 on 2022/6/27.
//

#include "common.h"
#include <stddef.h>
#include <stdlib.h>
#include <cstdio>


/*
 *  Dual list
 */

struct Duallist* duallist_copy_by_reference(struct Duallist *destDuallist, struct Duallist *aDuallist)
{
    struct Item *aItem;

    if(destDuallist == NULL) {
        destDuallist = (struct Duallist*)malloc(sizeof(struct Duallist));
    }
    duallist_init(destDuallist);

    if(aDuallist!=NULL) {
        aItem = aDuallist->head;
        while(aItem!=NULL) {
            duallist_add_to_tail(destDuallist, aItem->datap);
            aItem=aItem->next;
        }
    }
    return destDuallist;
}


struct Duallist* duallist_copy(struct Duallist *destDuallist, struct Duallist *aDuallist, void*(*copy_func)(void*))
{
    struct Item *aItem;

    if(destDuallist == NULL) {
        destDuallist = (struct Duallist*)malloc(sizeof(struct Duallist));
    }
    duallist_init(destDuallist);
    if(aDuallist!=NULL) {
        aItem = aDuallist->head;
        while(aItem!=NULL) {
            duallist_add_to_tail(destDuallist, copy_func(aItem->datap));
            aItem=aItem->next;
        }
    }
    return destDuallist;
}


struct Duallist* duallist_reverse_copy(struct Duallist *destDuallist, struct Duallist *aDuallist, void*(*copy_func)(void*))
{
    struct Item *aItem;

    if(destDuallist == NULL) {
        destDuallist = (struct Duallist*)malloc(sizeof(struct Duallist));
    }
    duallist_init(destDuallist);
    if(aDuallist!=NULL) {
        aItem = aDuallist->head;
        while(aItem!=NULL) {
            duallist_add_to_head(destDuallist, copy_func(aItem->datap));
            aItem=aItem->next;
        }
    }
    return destDuallist;
}


void duallist_init(struct Duallist *duallist)
{
    //Function：to initialize a duallist
    //Input: Address of a duallist
    //Output: Set the head of Duallist to 0, set the nItems=0.
    if(duallist == NULL) {
        return;
    }
    duallist->head = NULL;
    duallist->nItems = 0;
}

struct Item* duallist_add_to_head(struct Duallist* duallist, void *data)
{
    struct Item *newp;

    if(duallist == NULL) {
        return NULL;
    }
    newp = (struct Item*)malloc(sizeof(struct Item));
    newp->datap = data;
    if(duallist->head != NULL) {
        newp->next = duallist->head;
        newp->prev = duallist->head->prev;
        duallist->head->prev = newp;
        duallist->head = newp;
    } else {
        newp->next = NULL;
        newp->prev = newp;
        duallist->head = newp;
    }
    duallist->nItems ++;
    return newp;
}

struct Item* duallist_add_to_tail(struct Duallist *duallist, void *data)
{
    //Function: Add a data to the duallist at rhe tail.
    //Input: Address of a duallist, address of a data.
    //Output: Address of an Item, which is not necessary.
    struct Item *newp;

    if(duallist == NULL) {
        return NULL;
    }
    newp = (struct Item*)malloc(sizeof(struct Item));
    newp->datap = data;
    if(duallist->head != NULL) {
        newp->next = NULL;
        newp->prev = duallist->head->prev;
        duallist->head->prev->next = newp;
        duallist->head->prev = newp;
    } else {
        newp->next = NULL;
        newp->prev = newp;
        duallist->head = newp;
    }
    duallist->nItems ++;
    return newp;
}



struct Item* duallist_add_in_sequence_from_head(struct Duallist *duallist, void* data, int(*sort_func)(void*, void*))
{
    struct Item *newp, *aItem;

    if(duallist == NULL) {
        return NULL;
    }
    newp = (struct Item*)malloc(sizeof(struct Item));
    newp->datap = data;
    if(duallist->head != NULL) {
        aItem = duallist->head;
        while(aItem->next != NULL && sort_func(data, aItem->datap)){
            aItem = aItem->next;
        }
        if(!sort_func(data, aItem->datap)) {
            if (aItem == duallist->head) {
                newp->next = aItem;
                newp->prev = aItem->prev;
                aItem->prev = newp;
                duallist->head = newp;
            } else {
                newp->next = aItem;
                newp->prev = aItem->prev;
                aItem->prev = newp;
                newp->prev->next = newp;
            }
        } else {
            newp->next = NULL;
            aItem->next = newp;
            newp->prev = aItem;
            duallist->head->prev = newp;
        }
    } else {
        newp->next = NULL;
        newp->prev = newp;
        duallist->head = newp;
    }
    duallist->nItems ++;
    return newp;
}

struct Item* duallist_add_in_sequence_from_tail(struct Duallist *duallist, void* data, int(*sort_func)(void*, void*))
{
    //Function：
    //Input: (1)address of a Duallist,(2)address of a data (3)address of a sort function
    //Output:
    struct Item *newp, *aItem;

    if(duallist == NULL) {
        return NULL;
    }
    newp = (struct Item*)malloc(sizeof(struct Item));
    newp->datap = data;
    if(duallist->head != NULL) {
        aItem = duallist->head->prev;
        while(aItem != duallist->head && sort_func(data, aItem->datap)){
            aItem = aItem->prev;
        }
        if(!sort_func(data, aItem->datap)) {
            if (aItem->next == NULL) {
                newp->next = NULL;
                newp->prev = aItem;
                aItem->next = newp;
                duallist->head->prev = newp;
            } else {
                newp->next = aItem->next;
                newp->prev = aItem;
                aItem->next->prev = newp;
                aItem->next = newp;
            }
        } else {
            newp->next = aItem;
            newp->prev = aItem->prev;
            aItem->prev = newp;
            duallist->head = newp;
        }
    } else {
        newp->next = NULL;
        newp->prev = newp;
        duallist->head = newp;
    }
    duallist->nItems ++;
    return newp;
}

struct Item* duallist_add_before_item(struct Duallist *duallist, struct Item *fItem, struct Item *lItem, void *data)
{
    struct Item *theItem;

    if (fItem->next == NULL) {
        theItem = duallist_add_to_head(duallist, data);
        return theItem;
    }

    theItem = (struct Item*)malloc(sizeof(struct Item));
    theItem->datap = data;

    fItem->next = theItem;
    theItem->next = lItem;
    theItem->prev = fItem;
    lItem->prev = theItem;

    duallist->nItems ++;
    return theItem;
}

/*
 * Find the location of an item in a duallist
 */
struct Item* duallist_find(struct Duallist *duallist, void *key, int(*equal_func)(void*, void*))
{
    struct Item *aItem;

    if(duallist == NULL) {
        return NULL;
    }
    aItem = duallist->head;
    while(aItem != NULL) {
        if(equal_func(key, aItem->datap))
            return aItem;
        aItem = aItem->next;
    }
    return NULL;
}



//add by cscs
struct Item* duallist_add_unique(struct Duallist *duallist, void *data, int(*judge_func)(void*, void*))
{
    struct Item* newp;
    if(duallist_find(duallist, data, judge_func)==NULL) {
        newp = duallist_add_to_head(duallist, data);
        return newp;
    } else {
        return NULL;
    }
}//add by cscs

/*
 * Pick up an item from the duallist and return the
 * wrapped data.
 */
void* duallist_pick(struct Duallist *duallist, void *key, int(*judge_func)(void*, void*))
{
    struct Item *aItem;
    void *rt = NULL;

    if(duallist == NULL) {
        return NULL;
    }
    aItem = duallist->head;
    while(aItem != NULL) {
        if(judge_func(key, aItem->datap)) {
            if(aItem == duallist->head) {
                duallist->head = aItem->next;
                if(aItem->next != NULL)
                    duallist->head->prev = aItem->prev;
            } else {
                aItem->prev->next = aItem->next;
                if(aItem->next != NULL)
                    aItem->next->prev = aItem->prev;
                else
                    duallist->head->prev = aItem->prev;
            }
            rt = aItem->datap;
            free(aItem);
            duallist->nItems --;
            return rt;
        }
        aItem = aItem->next;
    }
    return NULL;
}

void* duallist_pick_item(struct Duallist *duallist, struct Item *theItem)
{
    void *rt;

    if(duallist == NULL) {
        return NULL;
    }
    if(theItem == duallist->head) {
        duallist->head = theItem->next;
        if(theItem->next != NULL)
            duallist->head->prev = theItem->prev;
    } else {
        theItem->prev->next = theItem->next;
        if(theItem->next != NULL)
            theItem->next->prev = theItem->prev;
        else
            duallist->head->prev = theItem->prev;
    }
    rt = theItem->datap;
    free(theItem);
    duallist->nItems --;
    return rt;
}

/*
 * Pick up the head item from the duallist and return the
 * wrapped data.
 */
void * duallist_pick_head(struct Duallist *duallist)
{
    struct Item *aItem;
    void *rt;

    if(duallist == NULL) {
        return NULL;
    }
    if(duallist->head != NULL) {
        aItem = duallist->head;
        duallist->head = aItem->next;
        if(aItem->next != NULL)
            duallist->head->prev = aItem->prev;
        rt = aItem->datap;
        free(aItem);
        duallist->nItems --;
        return rt;
    }
    return NULL;
}

/*
 * Pick up the tail item from the duallist and return the
 * wrapped data.
 */
void * duallist_pick_tail(struct Duallist *duallist)
{
    struct Item *aItem;
    void *rt;

    if(duallist == NULL) {
        return NULL;
    }
    if(duallist->head != NULL) {
        aItem = duallist->head->prev;
        if(aItem->prev == aItem)
            duallist->head = NULL;
        else {
            aItem->prev->next = NULL;
            duallist->head->prev = aItem->prev;
        }
        rt = aItem->datap;
        free(aItem);
        duallist->nItems --;
        return rt;
    }
    return NULL;
}

int is_duallist_empty(struct Duallist *duallist)
{
    if(duallist == NULL) {
        return -1;
    }
    if(duallist->head != NULL)
        return 0;
    return 1;
}

void duallist_destroy(struct Duallist *duallist, void (*free_func)(void*))
{
    //Function: To destroy a duallist
    //Input: (1)address of a duallist (2)address of free function
    //Output:
    void *datap;

    if(duallist == NULL) {
        return ;
    }
    while (!is_duallist_empty(duallist)) {
        datap = duallist_pick_head(duallist);
        if(free_func)
            free_func(datap);
    }
}

void duallist_dump(FILE *fOutput, struct Duallist *duallist, void(*dump_func)(FILE*, void*))
{
    unsigned long i;
    struct Item *aItem;

    if(duallist == NULL) {
        return ;
    }
    fwrite(&duallist->nItems, sizeof(unsigned long), 1, fOutput);
    aItem = duallist->head;
    for(i=0;i<duallist->nItems;i++) {
        dump_func(fOutput, aItem->datap);
        aItem = aItem->next;
    }
}


void duallist_load(FILE *fInput, struct Duallist *duallist, void*(*load_func)(FILE*))
{
    unsigned long i, nItems;

    if(duallist == NULL) {
        duallist = (struct Duallist*)malloc(sizeof(struct Duallist)) ;
    }
    duallist_init(duallist);
    fread(&nItems, sizeof(unsigned long), 1, fInput);
    for (i = 0; i<nItems; i++)
        duallist_add_to_tail(duallist, load_func(fInput));
}





unsigned long distance_to_tail(struct Item *aItem)
{
    unsigned long count = 0;

    while(aItem) {
        count ++;
        aItem = aItem->next;
    }
    return count;
}


unsigned long distance_to_head(struct Duallist *aDuallist, struct Item *aItem)
{
    unsigned long count = 0;

    while(aItem && aItem!=aDuallist->head) {
        count ++;
        aItem = aItem->prev;
    }
    return count;
}

int is_sublist(struct Duallist *aList, struct Duallist *bList, int(*equal_func)(void*, void*))
{
    struct Item *aItem, *bItem;
    int foundItem;

    if(aList && bList) {
        aItem = aList->head;
        while(aItem) {
            bItem = bList->head;
            foundItem = 0;
            while(bItem) {
                if(equal_func(aItem->datap, bItem->datap))
                    foundItem = 1;
                bItem = bItem->next;
            }
            if(!foundItem)
                return 0;
            aItem = aItem->next;
        }
        return 1;
    }
    return 0;
}

void duallist_remove_loops(struct Duallist *aList, int(*equal_func)(void*,void*), void(*free_func)(void*))
{
    struct Item *aItem, *bItem, *temp;
    void *datap;

    if(aList) {
        aItem = aList->head;
        while(aItem) {
            bItem = aItem->next;
            while(bItem) {
                if(equal_func && equal_func(aItem->datap, bItem->datap)) {
                    temp = aItem->next;
                    while(temp!=bItem->next) {
                        datap = duallist_pick_item(aList, aItem);
                        if(free_func)
                            free_func(datap);
                        aItem = temp;
                        temp = aItem->next;
                    }
                }
                bItem = bItem->next;
            }
            aItem = aItem->next;
        }
    }
}
