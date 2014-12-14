/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 *
 * @(#)$Id: my402list.h,v 1.1 2014/05/20 17:40:03 william Exp $
 */

#ifndef _MY402LIST_C_
#define _MY402LIST_C_

#include "my402list.h"
#include "stdlib.h"
#include "stdio.h"
#include "cs402.h"


//Returns the number of elements in the list.
 
int  My402ListLength(My402List *list){
    
    return list->num_members;
    
}

//Returns TRUE if the list is empty. Returns FALSE otherwise.
int  My402ListEmpty(My402List *elem){
    
    if(elem->num_members == 0)
        return 1;
    
    
    return 0;
    
}

//If list is empty, just add obj to the list. Otherwise, add obj after Last(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise

int  My402ListAppend(My402List *list, void *obj){
    My402List *temp = NULL ;
    My402ListElem *r = NULL;
    
    temp = list;
    
    if( My402ListEmpty(list)){
        
        r = malloc(sizeof( My402ListElem));
        if( r == 0){
            fprintf(stderr,"ERROR: Out of Memory");
            return 0;
        }
        temp->num_members = 1;
        r->obj = obj;
        temp->anchor.next = r;
        r->next = &(temp->anchor);
        temp->anchor.prev = r;
        r->prev = &(temp->anchor);
        
        
    }
    else{
        r = malloc(sizeof(My402ListElem));
        
        if( r == 0){
            fprintf(stderr,"ERROR: Out of Memory");
            return FALSE;
        }
        
        temp->num_members++;
        
        r->obj = obj;
        
        r->next = &(temp->anchor);
        r->prev = temp->anchor.prev;
        temp->anchor.prev->next = r;
        temp->anchor.prev = r;
        

        

    }
    
    return TRUE;
    
}
//If list is empty, just add obj to the list. Otherwise, add obj before First(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.

int  My402ListPrepend(My402List *list, void *obj){
    
    My402List *temp = NULL ;
    My402ListElem *r = NULL;
    
    temp = list;
    
    if( My402ListEmpty(list)){
        
        r = malloc(sizeof( My402ListElem));
        if( r == 0){
            fprintf(stderr,"ERROR: Out of Memory");
            return FALSE;
        }
        temp->num_members = 1;
        r->obj = obj;
        temp->anchor.next = r;
        r->next = &(temp->anchor);
        temp->anchor.prev = r;
        r->prev = &(temp->anchor);
        
        
    }
    
    else{
        
        r = malloc(sizeof(My402ListElem));
        
        if( r == 0){
            fprintf(stderr,"ERROR: Out of Memory");
            return FALSE;
        }
        
        temp->num_members++;
        
        r->obj = obj;
        
        r->next = temp->anchor.next;
        r->prev = &(temp->anchor);
        
        temp->anchor.next->prev = r;
        temp->anchor.next = r;
        
        
    }
    
    return TRUE;
}

//Unlink and delete elem from the list. Please do not delete the object pointed to by elem and do not check if elem is on the list.
void My402ListUnlink(My402List *list, My402ListElem *elem){
    
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
    
    list->num_members--;
    
    free(elem);
    
}

//Unlink and delete all elements from the list and make the list empty. Please do not delete the objects pointed to be the list elements.


void My402ListUnlinkAll(My402List *list){
    My402ListElem *elem=NULL;
    
    for (elem=My402ListFirst(list);
         elem != NULL;
         elem=My402ListNext(list, elem)) {
        
        My402ListUnlink(list,elem);
        
    }
    
}

//Insert obj between elem and elem->next. If elem is NULL, then this is the same as Append(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.

int  My402ListInsertAfter(My402List *list, void *obj, My402ListElem *elem){
    if(elem == NULL){
        My402ListAppend(list,obj);
        
    }
    else{
        My402ListElem *newElem = NULL;
        newElem = malloc(sizeof(My402ListElem));
        
        if(newElem == NULL){
            return FALSE;
        }
        
        newElem->obj = obj;
        
        newElem->next = elem->next;
        newElem->prev = elem;
        elem->next->prev = newElem;
        elem->next = newElem;
        
        list->num_members++;
        
        
    }
    
    return TRUE;
}


//Insert obj between elem and elem->prev. If elem is NULL, then this is the same as Prepend(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.

int  My402ListInsertBefore(My402List *list, void *obj, My402ListElem *elem){
    if(elem == NULL){
        My402ListPrepend(list,obj);
        
    }
    else{
        My402ListElem *newElem = NULL;
        newElem = malloc(sizeof(My402ListElem));
        
        if(newElem == NULL){
            return FALSE;
        }
        
        newElem->obj = obj;
        
        newElem->next = elem;
        newElem->prev = elem->prev;
        elem->prev->next = newElem;
        elem->prev = newElem;
        
        list->num_members++;
        
        
    }
    
    return TRUE;
}

//Returns the first list element or NULL if the list is empty.
My402ListElem *My402ListFirst(My402List *list){
    
    if(My402ListLength(list) == 0 )
        return NULL;
    
    return list->anchor.next ;
    
    
}

//Returns the last list element or NULL if the list is empty

My402ListElem *My402ListLast(My402List *list){
    
    if(My402ListLength(list) == 0 )
        return NULL;
    
    return list->anchor.prev ;
    
}

//Returns elem->next or NULL if elem is the last item on the list. Please do not check if elem is on the list.
My402ListElem *My402ListNext(My402List *list, My402ListElem *elem){
    
    
    if(elem->next == &(list->anchor))
        return NULL;
    
    return elem->next;
    
}

//Returns elem->prev or NULL if elem is the first item on the list. Please do not check if elem is on the list.
My402ListElem *My402ListPrev(My402List *list, My402ListElem *elem){
    
    if(elem->prev == &(list->anchor))
        return NULL;
    return elem->prev;
    
}

//Returns the list element elem such that elem->obj == obj. Returns NULL if no such element can be found.
My402ListElem *My402ListFind(My402List *list, void *obj){
    My402ListElem *elem=NULL;
    
    for (elem=My402ListFirst(list);
         elem != NULL;
         elem=My402ListNext(list, elem)) {
        
        if(elem->obj == obj){
            return elem;
        }
        
    }
    
    return NULL;
    
}

//Initialize the list into an empty list. Returns TRUE if all is well and returns FALSE if there is an error initializing the list
int My402ListInit(My402List *list){
    
    list->anchor.obj = NULL;
    list->anchor.next = NULL;
    list->anchor.prev = NULL;
    list->num_members = 0;
    
    
    return TRUE;
    
}


#endif /*_MY402LIST_C_*/
