#ifndef __SL_LIST_H__
#define __SL_LIST_H__

#include <stddef.h> /*size_t*/

typedef struct sl_list sl_list_t;
typedef struct sll_node sll_node_t;
typedef sll_node_t* iterator_t;

/* 
*   @desc: Allocates single linked list. Must be freed using @SLListDestroy.
*   @params: None
*   @return value: Returns a pointer to the allocated list.
*   @error: Returns null if allocation failed
*/
sl_list_t* SLListCreate(void);

/* 
*   @desc: frees single linked list. Must have been created using @SLListCreate.
*   @params: @sl_list: pointer to allocated list using @SLListCreate
*   @return value: None
*   @error: None
*/
void SLListDestroy(sl_list_t* sl_list);

/* 
*   @desc: Inserts a new node with @element into the linked list before the current element indicated by @iter.
*	   Invalidates @iter.
*   @params: @iter: Position to insert element before.
*	     @element: Pointer to an element to be added into the linked list
*   @return value: Returns iterator pointing to the newly added element
*   @error: In the event insertion fails(due to allocation) will return End iterator
*	    Undefined Behavior if iter is begin of the list or iter is not valid
*/
iterator_t SLListInsertBefore(iterator_t iter, const void* element);

/* 
*   @desc: 		   Removes node from the linked list at the position pointed by @iter.
*				   Invalidates @iter.
*   @params: 	   @iter: Position to insert element before.
*   @return value: Returns iterator pointing to the next element after the removed element
*   @error: 	   Undefined Behavior if iter is end of the list or iter is not valid
*/
iterator_t SLListRemoveElement(iterator_t iter);

/* 
*   @desc: 		   Counts the amount of elements in @sl_list.
*   @params: 	   @sl_list: List to count elements.
*   @return value: Returns count of elements
*   @error: 	   Undefined Behavior if @sl_list is not valid
*/
size_t SLListCount(const sl_list_t* sl_list);

/* 
*   @desc: 		   Checks if @sl_list is empty.
*   @params: 	   @sl_list: List to check on.
*   @return value: Returns 1 if sl_list is empty otherwise 0
*   @error: 	   Undefined Behavior if @sl_list is not valid
*/
int SLListIsEmpty(const sl_list_t* sl_list);

/* 
*   @desc: 		   Returns iterator to the beginning of the linked list.
*   @params: 	   @sl_list: List to create iterator on.
*   @return value: Returns valid iterator to the beginning of the linked list
*   @error: 	   Undefined Behavior if @sl_list is not valid
*/
iterator_t SLListBegin(const sl_list_t* sl_list);

/* 
*   @desc: 		   Returns iterator to the end of the linked list.
*   @params: 	   @sl_list: List to create iterator on.
*   @return value: Returns valid iterator to the end of the linked list
*   @error: 	   Undefined Behavior if @sl_list is not valid
*/
iterator_t SLListEnd(const sl_list_t* sl_list);

/* 
*   @desc: 		   Advances @iter to the next element and returns the new iterator.
*   @params: 	   @iter: Valid Iterator.
*   @return value: Returns valid iterator advanced by one element
*   @error: 	   Undefined Behavior if @iter is not valid or @iter is end iterator.
*/
iterator_t SLListNext(iterator_t iter);

/* 
*   @desc: 		   Checks if one and other point to the same element.
*   @params: 	   @one: Valid Iterator.
*				   @other: Valid Iterator.
*   @return value: Returns 1 if the iterators are the same and 0 otherwise
*   @error: 	   Undefined Behavior if @one or @other is not valid.
*/
int SLListIsSameIter(iterator_t one, iterator_t other);

/* 
*   @desc: 		   Returns the value the current iterator points to.
*   @params: 	   @iter: Valid Iterator.
*   @return value: Returns a pointer to the data the current node is referencing
*   @error: 	   Undefined Behavior if @iter is invalid or @iter is end iterator
*/
void* SLListGetValue(iterator_t iter);

/* 
*   @desc: 		   Sets the value of the current iterator points to into @value.
*   @params: 	   @iter: Valid Iterator.
*   @return value: None
*   @error: 	   Undefined Behavior if @iter is invalid or @iter is end iterator
*/
void SLListSetValue(iterator_t iter, const void* value);

/* 
*   @desc: 		   Finds an element matching @element in range @from(inclusive) to @to(execlusive) where @match_func will return 1 on match and 0 on mismatch
*   @params: 	   @from: Valid iterator to start searching from
*				   @to: Valid iterator to end searching at (make sure @to is after @from)
*				   @is_match: A compare function that returns 1 upon matching the two elements and 0 otherwise
*				   @element: An element to compare to
*   @return value: Iterator to the found element or end if not found
*   @error: 	   Undefined Behavior if @from or @to is invalid or @to is before @from or if @is_match is not valid function
*/
iterator_t SLListFind(iterator_t from, iterator_t to, int (*is_match)(const void* data, const void* element) ,const void* element);

/* 
*   @desc: 		   Activates @action_func with @param on each of the elements in range @from(inclusive) to @to(execlusive)
*   @params: 	   @from: Valid iterator to start action from 
*				   @to: Valid iterator to end action at (make sure @to is after @from)
*				   @action_func: A function that does an action on elements, and returns 0 upon success (otherwise it has failed)
*				   @param: Additional paramater to pass into the action function (user defined)
*   @return value: Iterator to @to or returns the iterator to the element on which the action failed on 
*   @error: 	   Undefined Behavior if @from or @to is invalid or @to is before @from or if @action_func is not valid function
*/
iterator_t SLListForEach(iterator_t from, iterator_t to, int (*action_func)(void* data, void* param), void* param);

void SLListAppend(sl_list_t* sl_src, sl_list_t* sl_dst);

#endif  /*End of header guard*/ 
