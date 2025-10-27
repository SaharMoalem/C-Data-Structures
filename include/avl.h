#ifndef __AVL_H__
#define __AVL_H__

#include <sys/types.h> /* size_t, ssize_t */

#include "dl_list.h" /* dl_list */

typedef struct avl avl_t;
typedef int (*compare_func_t)(const void* data, const void* param);
typedef int (*action_func_t)(void* ,void* );
typedef int (*is_match_t)(const void* data, const void* param);

typedef enum 
{
    PRE = 0,
    POST = 1,
    IN = 2
} traversal_order_t;

/* 
*   @desc:          Allocates a new AVL which is sorted by @compare_func. 
*                   Must be destroyed with @AVLDestroy
*   @params: 	    @compare_func: compare function that the AVL is sorted by
*   @return value:  Pointer to the allocation
*   @error: 		NULL if allocation fails
*                   Undefined behavior if @compare_func is invalid
*   @time complex: 	O(malloc) for both AC/WC
*   @space complex: O(malloc) for both AC/WC
*/
avl_t* AVLCreate(compare_func_t compare_func);


/* 
*   @desc:          Frees AVL. Must have been created using @AVLCreate    
*   @params: 	    @avl: AVL to free
*   @return value:  None
*   @error: 		Undefined behavior if @avl is not valid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(n) for both AC/WC
*/
void AVLDestroy(avl_t* avl);


/*
*   @desc:          Inserts @data at the appropiate location in @avl
*                   based on the avl's compare function.
*   @params: 	    @avl: pre-allocated avl
*                   @data: data to insert
*   @return value:  Returns zero if insert was successful and non zero otherwise
*   @error: 		Returns non zero value if allocation failed
*                   Undefined behaviour if @avl is invalid                   
*   @time complex: 	O(log(n)) for both AC/WC
*   @space complex: O(log(n)) for both AC/WC
*/
int AVLInsert(avl_t* avl, const void* data);


/* 
*   @desc:          Removes @param from @avl
*   @params: 	    @avl: pre-allocated avl
*					@param: param to remove from avl
*   @return value:  None
*   @error: 		Undefined behavior if @avl is invalid.
*   @time complex: 	O(log(n)) for both AC/WC
*   @space complex: O(log(n)) for both AC/WC 
*/
void AVLRemove(avl_t* avl, const void* param);


/* 
*   @desc:          finds @param in @avl
*   @params:	    @avl: pre-allocated AVL Tree 
*                   @data: data to find in @avl
*   @return value:  returns pointer to the found ocuurance of @param or
*					NULL if not found.
*   @error:		    undefined behavior if @avl is invalid
*   @time complex:  O(log(n)) for both AC/WC
*   @space complex: O(log(n)) for both AC/WC 
*/
void* AVLFind(const avl_t* avl, const void* param);


/* 
*   @desc:          Counts the number of nodes in @avl     
*   @params:        @avl: valid AVL
*   @return value:  Number of nodes in @avl.
*   @error:         Undefined beahvior if @avl is invalid.	
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(n) for both AC/WC
*/
size_t AVLGetSize(const avl_t* avl);


/* 
*   @desc:          Returns the height of @avl     
*   @params:        @avl: valid AVL
*   @return value:  Height of @avl.
*   @error:         Undefined beahvior if @avl is invalid.	
*   @time complex: 	O(1) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
size_t AVLGetHeight(const avl_t* avl);


/* 
*   @desc:          Iterates over each element in @avl and performs @action_func
*                   with @param and said element.
*   @params: 		@avl: valid AVL
*					@action_func: a function that does user defined actions
*                   on elements data and returns zero upon success
*					@param: a user defined additional parameter to pass into 
*					@action_func
*                   @order: In which order to preform the iteration.
*   @return value: 	Returns zero if the function performed successfully
*                   otherwise returns @action_func return value
*   @error: 		Undefined behavior if @avl is invalid or if @is_match is not
*                   valid.
*   @time complex: 	O(n) * O(@action_func)
*   @space complex: O(n) * O(@action_func) 
*/
int AVLForEach(const avl_t* avl, action_func_t action_func, void* param,
                                                    traversal_order_t order);


/* 
*   @desc:          Checks if @avl is empty.         
*   @params: 	    @avl: Valid AVL
*   @return value:  Return one if @avl is empty and zero otherwise.
*   @error: 		Undefined behavior if @avl is not valid.
*   @time complex: 	O(1)
*   @space complex: O(1)
*/
int AVLIsEmpty(const avl_t* avl);


/* 
*   @desc:          Finds all instances matching @match_func with @param and
*                   puts them in @out_list         
*   @params: 	    @avl: Valid AVL
*                   @match_func: Function to match with @param should return one
*                   if matches and zero otherwise
*                   @param: Additional user parameter to match with the elements
*                   @out_list: doubly linked list to append elements into
*   @return value:  Return the amount of elements found or negative one if
*                   allocation failed
*   @error: 		Undefined behavior if @avl is not valid or @match_func is
*                   invalid or @dl_list is invalid
*                   Returns negative one if allocation failed
*   @time complex: 	O(n)
*   @space complex: O(n)
*/
ssize_t AVLMultipleFind(const avl_t* avl, is_match_t match_func, void* param,
                                                        dl_list_t* out_list);


/* 
*   @desc:          Removes all instances matching @match_func with @param and
*                   puts them in @out_list         
*   @params: 	    @avl: Valid AVL
*                   @match_func: Function to match with @param should return one
*                   if matches and zero otherwise
*                   @param: Additional user parameter to match with the elements
*                   @out_list: doubly linked list to append elements into
*   @return value:  Return the amount of elements removed or negative one if
*                   allocation failed
*   @error: 		Undefined behavior if @avl is not valid or @match_func is
*                   invalid or @dl_list is invalid
*                   Returns negative one if allocation failed
*   @time complex: 	O(n)
*   @space complex: O(n)
*/
ssize_t AVLMultipleRemove(const avl_t* avl, is_match_t match_func, void* param,
                                                        dl_list_t* out_list);

#endif      /* __AVL_H__ */ 