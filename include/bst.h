#ifndef __BST_H__
#define __BST_H__

#include <stddef.h> /* size_t */

typedef struct bst bst_t;
typedef struct bst_iter bst_iter_t;
typedef struct bst_node bst_node_t;
typedef int (*compare_func_t)(const void*, const void*);
typedef int (*action_func_t)(void* data,void* param);

struct bst_iter
{
    bst_node_t* node;
};

/* 
*   @desc:          Allocates a new BST which is sorted by @compare_func. 
*                   Must be destroyed with @BSTDestroy
*   @params: 	    @compare_func: compare function that the BST is sorted by
*   @return value:  Pointer to the allocation
*   @error: 		NULL if allocation fails
*                   Undefined behavior if @CompareFunc is invalid
*   @time complex: 	O(malloc) for both AC/WC
*   @space complex: O(malloc) for both AC/WC
*/
bst_t* BSTCreate(compare_func_t compare_func);

/* 
*   @desc:          Frees BST. Must have been created using @BSTCreate    
*   @params: 	    @bst: BST to free
*   @return value:  None
*   @error: 		Undefined behavior if @bst is not valid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
void BSTDestroy(bst_t* bst);

/* 
*   @desc:          Inserts @data at the appropiate location in the @bst
*                   based on the bst's compare function.
*   @params: 	    @bst: pre-allocated bst.
*                   @data: data to insert.
*   @return value:  Returns the newly added iterator
*   @error: 		Returns invalid iterator if allocation failed.
*                   Undefined behaviour if @bst is invalid.                   
*   @time complex: 	AC:O(log(n)) ,WC:O(n)
*   @space complex: O(1) for both AC/WC 
*/
bst_iter_t BSTInsert(bst_t* bst, const void* data);

/* 
*   @desc:          Removes @iter from the BST.
*   @params: 	    @iter: iter to remove.
*   @return value:  returns the data @iter holds.
*   @error: 		Undefined behavior if @iter is invalid.
*   @time complex: 	AC:O(log(n)) ,WC:O(n)
*   @space complex: O(1) for both AC/WC 
*/
void* BSTRemove(bst_iter_t iter);

/* 
*   @desc:          finds first occurunce of @data in @bst.
*   @params:	    @bst: pre-allocated Binary Search Tree 
*                   @data: data to find in @bst
*   @return value:  returns iter to the found ocuurance of @data or
*					invalid	iter if not found.
*   @error:		    undefined behavior if @bst is invalid
*   @time complex:  AC:O(log(n)) , WC:O(n)
*   @space complex: O(1) for both AC/WC 
*/
bst_iter_t BSTFind(const bst_t* bst, const void* data);

/* 
*   @desc:           Returns an iterator to the first element in the @bst.
*   @params: 	     @bst: Pointer to the BST.
*   @return value:   Iterator to the first element. 
*   @error:          Undefined behavior if @bst is invalid.	
*   @time complex: 	 AC:O(log(n)) , WC:O(n)
*   @space complex:  O(1)
*/
bst_iter_t BSTBegin(const bst_t* bst);

/* 
*   @desc:          Returns invalid iter after the last element of @bst. 
*   @params: 	    @bst: Valid bst.    
*   @return value:  Invalid iter after the last element of @bst.
*   @error: 		Undefined behavior if @bst is invalid.
*   @time complex: 	AC:O(log(n)) , WC:O(n)
*   @space complex: O(1)
*/
bst_iter_t BSTEnd(const bst_t* bst);

/* 
*   @desc:          Returns the next iterator
*   @params:        @iter: valid iterator	    
*   @return value:  Returns the next iterator
*   @error:         Undefined behavior if @iter is invalid		
*   @time complex: 	AC:O(log(n)) , WC:O(n)
*   @space complex: O(1)
*/
bst_iter_t BSTNext(bst_iter_t iter);

/* 
*   @desc:          Returns the previous iterator for @iter in the bst.     
*   @params:        @iter iterator in BST to iterate from. 
*   @return value:  Return the previous iterator.
*   @error:         Undefined behavior if @iter is the beginning of the tree.
*   @time complex: 	AC:O(log(n)) , WC:O(n)
*   @space complex: O(1)
*/
bst_iter_t BSTPrev(bst_iter_t iter);

/* 
*   @desc:          Counts the number of nodes in @bst     
*   @params:        @bst: valid BST
*   @return value:  Number of nodes in @bst.
*   @error:         Undefined beahvior if @bst is invalid.	
*   @time complex: 	O(n)
*   @space complex: O(1)
*/
size_t BSTGetSize(const bst_t* bst);

/* 
*   @desc:          Returns the value of the node iter is pointing to.      
*   @params:        @iter: valid iterator    
*   @return         Returns the value iter is pointing to.
*   @error:         Undefined behavior if @iter is invalid	
*   @time complex: 	O(1)
*   @space complex: O(1)
*/
void* BSTGetValue(bst_iter_t iter);

/* 
*   @desc:          Iterates over each element in range from @from(inclusive)
*					to @to(exclusive) and performs @action_func with @param and
*					said element.
*   @params: 		@from: valid iterator to start action on
*					@to: iterator to end of range.
*					@action_func: a function that does user defined actions
*                   on elements data and returns 0 upon success
*					@param: a user defined additional parameter to pass into 
*					@action_func
*   @return value: 	Returns 0 if the function performed successfully otherwise 
*					returns @action_func return value
*   @error: 		Undefined behavior if @from is invalid or @to is before
*					@from or if @is_match is not valid.
*   @time complex: 	O(n) * O(@action_func)
*   @space complex: O(@action_func) 
*/
int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func,
                void* param);

/* 
*   @desc:          Checks if @one and @other refer to the same element.
*   @params: 	    @one: iterator
*					@other: iterator
*   @return value:  Returns 1 if the iterators are the same and 0 otherwise
*   @error: 		None
*   @time complex: 	O(1)
*   @space complex: O(1)
*/
int BSTIsSameIter(bst_iter_t one, bst_iter_t other);

/* 
*   @desc:          Checks if @bst is empty.         
*   @params: 	    @bst: Valid bst.
*   @return value:  Return 1 if the @bst is empty , 0 otherwise.
*   @error: 		Undefined behavior if @bst is not valid.
*   @time complex: 	O(1)
*   @space complex: O(1)
*/
int BSTIsEmpty(const bst_t* bst);

#endif      /* __BST_H__ */ 