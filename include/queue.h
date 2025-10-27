#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h>     /* size_t */

typedef struct queue queue_t;

/* 
*   @desc: 		   Allocates a queue. Must be freed using @QDestroy.
*   @params: 	   None
*   @return value: Returns a pointer to the allocated queue.
*   @error: 	   Returns null if the allocation failed
*/
queue_t* QCreate(void);

/* 
*   @desc: 		   Frees a queue. Must have been created using @QCreate.
*   @params: 	   @queue_t: pointer to allocated queue using @QCreate
*   @return value: None
*   @error: 	   Undefined behavior if queue was not created by @QCreate
*/
void QDestroy(queue_t* queue);

/* 
*   @desc: 		   Inserts an item to the end of @queue with @data.
*   @params: 	   @queue: Queue to insert the element into.
*				   @data: Pointer to an element to be added into the queue
*   @return value: Returns 0 on success
*   @error: 	   In the event insertion fails(due to allocation) will return non zero value
*				   Undefined Behavior if queue is not valid
*/
int QEnqueue(queue_t* queue, const void* data);

/* 
*   @desc:		   Removes the first element from @queue
*   @params: 	   @queue: Pointer to a valid queue
*   @return value: None 
*   @error: 	   Undefined Behavior if queue is invalid or empty
*/
void QDequeue(queue_t* queue);

/* 
*   @desc: 		   Returns the first element of @queue
*   @params: 	   @queue: Pointer to a valid queue
*   @return value: The data of the first element
*   @error: 	   Undefined behavior if queue is invalid or empty
*/
void* QPeek(const queue_t* queue);

/* 
*   @desc: 		   Checks if @queue is empty
*   @params: 	   @queue: Pointer to a valid queue
*   @return value: 1 if the queue is empty 0 otherwise
*   @error: 	   Undefined behavior if queue is invalid
*/
int QIsEmpty(const queue_t* queue);

/* 
*   @desc: 		   Returns the current size of the queue
*   @params: 	   @queue: Pointer to a valid queue
*   @return value: Current size of the queue
*   @error: 	   Undefined behavior if queue is invalid
*/
size_t QGetSize(const queue_t* queue);

/* 
*   @desc: 		   Appends @src queue into @dst, empties the @src queue
*   @params: 	   @src: Src queue to remove elements from
*				   @dst: Destenation queue to append the elements into
*   @return value: None
*   @error: 	   Undefined behavior if src or dst are not valid
*/
void QAppend(queue_t* src, queue_t* dst);

#endif  /* End of header guard */ 
