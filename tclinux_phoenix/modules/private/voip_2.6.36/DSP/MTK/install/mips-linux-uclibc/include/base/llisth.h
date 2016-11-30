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
 *	@file	llisth.h
 *	@brief
 *
 *	@author mtk01732
 *	@date	06/14/07
 *
 **/

#ifndef	LLISTH_06_14_07
#define LLISTH_06_14_07

#include "cerr.h"
#include "link_base.h"
#include "llist.h"

/**
 *	@brief the llisth object
 */
typedef struct _llisth{
  llist_t list;
  int nnodes;
}llisth_t;

/**
 *	@brief Initializes a linked list header
 */
#define LISTH_Init(head)\
{\
	/* Adjust head and tail */ \
  LIST_Init(&(head)->list);\
	(head)->nnodes = 0;\
}

/**
 *	@brief destroy the linked list, but not release memory space for
 *	each element
 */
#define LISTH_Dest(head)\
{\
  llist_t *p, *tmp;\
  LIST_ForEachSafe(p, tmp, &(head)->list) {\
    p->prev = p->next = NULL;\
  }\
  LIST_Init(&(head)->list);\
  (head)->nnodes = 0;\
}

/**
 *  @brief Insert an item before node in a linked list
 */
#define LISTH_AddBefore(head, newnode, item)\
{\
  __LIST_Add(newnode, (item)->prev, item);\
  (head)->nnodes++;\
}

/**
 *  @brief Insert an item after node in a linked list
 */
#define LISTH_AddAfter(head, newnode, item)\
{\
  __LIST_Add(newnode, item, (item)->next);\
  (head)->nnodes++;\
}

/**
 *  @brief Insert an item at the front of the linked list
 */
#define LISTH_AddHead(head, newnode)\
{\
  LIST_Add(newnode, &(head)->list);\
  (head)->nnodes++;\
}

/**
 *  @brief Insert an item at the end of the linked list
 */
#define LISTH_AddTail(head, newnode)\
{\
  LIST_AddTail(newnode, &(head)->list);\
  (head)->nnodes++;\
}

/**
 *  @brief Get the head of the linked list
 */
#define LISTH_GetFirst(head, type, member)\
  (((head)->list.next != &(head)->list) ? \
   (LIST_Entry((head)->list.next, type, member)) : (NULL))

/**
 *  @brief Get the tail of the linked list
 */
#define LISTH_GetLast(head, type, member)\
  (((head)->list.prev != &(head)->list) ? \
   (LIST_Entry((head)->list.prev, type, member)) : (NULL))

/**
 *  @brief Get the next node to a certain node in a linked list
 */
#define LISTH_GetNext(head, node, type, member)\
  (((node)->next != &(head)->list) ? \
   (LIST_Entry((node)->next, type, member)) : (NULL))

/**
 *  @brief Get the next node to a certain node in a linked list,
 *  if the current node is the last one, the function returns the first node
 */
#define LISTH_GetCycNext(head, node, type, member)\
  (((node)->next != &(head)->list) ? \
   (LIST_Entry((node)->next, type, member)) : (LIST_Entry((head)->list.next, type, member)))

/**
 *  @brief Get the previous node to a certain node in a linked list
 */
#define LISTH_GetPrev(head, node, type, member)\
  (((node)->prev != &(head)->list) ? \
   (LIST_Entry((node)->prev, type, member)) : (NULL))

/**
 *  @brief Get the previous node to a certain node in a linked list,
 *  if the current node is the first one, the function returns the last node
 */
#define LISTH_GetCycPrev(head, node, type, member)\
  (((node)->prev != &(head)->list) ? \
   (LIST_Entry((node)->prev, type, member)) : (LIST_Entry((head)->list.prev, type, member)))

/**
 *  @brief Remove a certain node from linked list
 */
#define LISTH_Remove(head, node)\
{\
   LIST_Del(node);\
  (head)->nnodes--;\
}

/**
 * @brief Remove the first node from linked list
 */
#define LISTH_RemoveHead(head)\
{\
  LIST_Del((head)->list.next);\
  (head)->nnodes--;\
}

/**
 *  @brief Remove the last node from linked list
 */
#define LISTH_RemoveTail(head)\
{\
  LIST_Del((head)->list.prev);\
  (head)->nnodes--;\
}

/**
 *  @brief Check if the linked list is empty or not
 */
#define LISTH_IsEmpty(head)\
  ((head)->nnodes == 0)

/**
 *  @brief Return the number of inserted nodes in the linked list
 */
#define LISTH_GetNNodes(head)\
  ((head)->nnodes)

/**
 *  @brief search key from a linked list
 */
#define LISTH_Find(head, keyvalue, type, keymember, linkmember, retNode)\
{\
  type *n;\
  llist_t *p;\
  retNode = NULL;\
  LIST_ForEach(p, &(head)->list) {\
    n = LIST_Entry(p, type, linkmember);\
    if(n->keymember == keyvalue){\
      retNode = n;\
      break;\
    }\
  }\
}

/**
 *  @brief search key after a node from a linked list
 */
#define LISTH_FindN(head, node, keyvalue, type, keymember, linkmember, retNode)\
{\
  type *n;\
  llist_t *p;\
  retNode = NULL;\
  for (p = (node)->next; p != &(head)->list; p = p->next){\
    n = LIST_Entry(p, type, linkmember);\
    if(n->keymember == keyvalue){\
      retNode = n;\
      break;\
    }\
  }\
}

/**
 *  @brief Insert a increasing sequential linked list by a specified field
 */
#define LISTH_AddSort(head, newnode, type, keymember, linkmember)\
{\
  type *n, *ori;\
  llist_t *p, *tmp;\
  ori = LIST_Entry(newnode, type, linkmember);\
  LIST_ForEachSafe(p, tmp, &(head)->list){\
    n = LIST_Entry(p, type, linkmember);\
    if(ori->keymember < n->keymember){\
      break;\
    }\
  }\
  LISTH_AddBefore(head, newnode, p);\
}
#endif
