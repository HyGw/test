/******************************************************************************
 *	LEGAL DISCLAIMER
 *
 *	(HEADER OF MEDIATEK SOFTWARE/FIRMWARE RELEASE OR DOCUMENTATION)
 *
 *	(c) 2008 MediaTek Inc.  ALL RIGHTS RESERVED.
 *
 *	BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND 
 *	AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS 
 *	(MEDIATEK SOFTWARE) RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE 
 *	PROVIDED TO BUYER ON AN AS IS BASIS ONLY.  MEDIATEK EXPRESSLY DISCLAIMS 
 *	ANY AND ALL WARRANTIES, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 *	LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 *	PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  NOR DOES MEDIATEK PROVIDE ANY 
 *	WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PART(IES) 
 *	WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
 *	SOFTWARE.  BUYER AGREES TO LOOK ONLY TO SUCH THIRD PART(IES) FOR ANY AND ALL 
 *	WARRANTY CLAIMS RELATING THERETO.  NEITHER SHALL MEDIATEK BE RESPONSIBLE FOR 
 *	ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR CONFORMING 
 *	TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *	BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE 
 *	LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER SHALL BE, 
 *	AT MEDIATEK'S SOLE OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT 
 *	ISSUE OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGES PAID BY BUYER 
 *	TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE. 
 *
 *	THE MEDIATEK SOFTWARE IS PROVIDED FOR AND ONLY FOR USE WITH MEDIATEK CHIPS 
 *	OR PRODUCTS.  EXCEPT AS EXPRESSLY PROVIDED, NO LICENSE IS GRANTED BY 
 *	IMPLICATION OR OTHERWISE UNDER ANY INTELLECTUAL PROPERTY RIGHTS, INCLUDING 
 *	PATENT OR COPYRIGHTS, OF MEDIATEK.  UNAUTHORIZED USE, REPRODUCTION, OR 
 *	DISCLOSURE OF THE MEDIATEK SOFTWARE IN WHOLE OR IN PART IS STRICTLY 
 *	PROHIBITED.
 *
 *	THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH 
 *	THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF 
 *	LAWS PRINCIPLES.  
 */
/**
 *	@file  llist.h
 *	@brief high efficient doubly "embedded" linked-list implementation
 *
 *	@author Howard Chen, Moja Hsu ,HR Hsu
 *
 *	@date   2006 , 7
 *
 **/
#ifndef LLIST0630
#define LLIST0630

#ifdef __RVCT__
#define __inline__  __inline
#endif

/**
 *	@brief the embedded linked list
 */
typedef struct llist {
	struct llist *next, *prev;
} llist_t;

#define LIST_Empty(ptr)		((ptr)->next == (ptr))

/**
 * @brief the llist_t initializator
 */
#define LIST_VALUE(name) { &(name), &(name) }

/**
 * @brief the llist_t constructor
 */
#define LIST_Init(ptr) do { \
	 (ptr)->next = (ptr); (ptr)->prev = (ptr); \
}while(0)

/**
 * @brief Insert a _new entry between two known consecutive entries.
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
void __LIST_Add(struct llist *_new, struct llist *prev, struct llist *next);

/**
 * @brief Insert a _new entry after the specified head.
 * @param _new  _new entry to be added
 * @param head list head to add it after
 *
 */
void LIST_Add(struct llist *_new, struct llist *head);

/**
 * @brief Insert a _new entry before the specified head.
 * @param _new: _new entry to be added
 * @param head: list head to add it before
 *
 */
void LIST_AddTail(struct llist *_new, struct llist *head);

/**
 * @brief Delete a list entry by making the prev/next entries
 * point to each other.This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
void __LIST_Del(struct llist *prev, struct llist *next);
/**
* @brief deletes entry from list.
* @param entry: the element to delete from the list.
* @note: list_empty on entry does not return true after this, the entry is in an undefined state.
*/
llist_t* LIST_Del(struct llist *entry);

/**
* @brief deletes entry from list and reinitialize it.
* @param entry: the element to delete from the list.
*/
void LIST_DelInit(struct llist *entry);

/**
 * @brief list_splice - join two lists
 * @param list: the _new list to add.
 * @param head: the place to add it in the first list.
 */
void LIST_Splice(struct llist *list, struct llist *head);

/**
 * @brief get the struct for this entry
 * @param ptr		the &struct list_head pointer.
 * @param type 		the type of the struct this is embedded in.
 * @param member	the name of the list_struct within the struct.
 */
#define LIST_Entry(ptr, type, member) \
        ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * @brief iterate over a list
 * @param pos	the &struct list_head to use as a loop counter.
 * @param head  the head for your list.
 */
#define LIST_ForEach(pos, head) \
        for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * @brief iterate over a list safe against removal of list entry
 * @param pos    	the &struct list_head to use as a loop counter.
 * @param n         another &struct list_head to use as temporary storage
 * @param head      the head for your list.
 */
#define LIST_ForEachSafe(pos, n, head) \
        for (pos = (head)->next, n = pos->next; pos != (head); \
                pos = n, n = pos->next)

/**
 * @brief LIST_for_each_prev   -       iterate over a list in reverse order
 * @param pos	the &struct list_head to use as a loop counter.
 * @param head	the head for your list.
 */
#define LIST_ForEachPrev(pos, head) \
        for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 *  @brief Get the first of the linked list
 */
#define LIST_GetFirst(head, type, member)\
  (((head)->next != (head)) ? \
   (LIST_Entry((head)->next, type, member)) : (NULL))


#endif
