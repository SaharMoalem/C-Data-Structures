#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stddef.h>         /* size_t */
#include <sys/types.h>      /* ssize_t */

typedef struct circular_buffer circular_buffer_t;

/* 
*   @desc: 	        Allocates a circular buffer. Must be freed using @CBDestroy.
*   @params: 	    @size_t capacity of the circular buffer in bytes
*   @return value:  Returns a pointer to the allocated circular buffer.
*   @error: 	    Returns null if the allocation failed
*   @time complex:  O(1)
*   @space complex: O(n)
*/
circular_buffer_t* CBCreate(size_t capacity);

/* 
*   @desc: 	        Frees a circular buffer. Must have been created using @CBCreate.
*   @params: 	    @c_buffer: the buffer to free
*   @return value:  None
*   @error: 	    None
*   @time complex:  O(1)
*   @space complex: O(1)
*/
void CBDestroy(circular_buffer_t* c_buffer);

/* 
*   @desc: 	        Writes @bytes bytes into @c_buffer from @src
*   @params: 	    @c_buffer: circular buffer to write into
*				    @src: Pointer to the location of bytes to copy from
*                   @bytes: Count of bytes to write into
*   @return value:  Returns the amount of bytes written into the buffer(will be 0 if it is full)
*   @error: 	    Undefined behavior if c_buffer is not valid or src is invalid or don't have enough bytes
*   @time complex:  O(n)
*   @space complex: O(1)
*/
ssize_t CBWrite(circular_buffer_t* c_buffer, const void* src, size_t bytes);

/* 
*   @desc:		    Reads @bytes from @c_buffer into @dst
*   @params: 	    @c_buffer: circular buffer to read from
*                   @dst: destination buffer to write the read bytes into
*                   @bytes: count of bytes to read from buffer and copy into dst
*   @return value:  Count of bytes read from buffer (will return 0 if buffer is empty)
*   @error: 	    Undefined Behavior if queue is invalid or dst is invalid or don't have enough space for bytes read.
*   @time complex:  O(n)
*   @space complex: O(1)
*/
ssize_t CBRead(circular_buffer_t* c_buffer, void* dst, size_t bytes);

/* 
*   @desc: 		    Checks if @c_buffer is empty
*   @params: 	    @c_buffer: the buffer to check
*   @return value:  1 if the buffer is empty 0 otherwise
*   @error: 	    Undefined behavior if @c_buffer is invalid
*   @time complex:  O(1)
*   @space complex: O(1)
*/
int CBIsEmpty(const circular_buffer_t* c_buffer);

/* 
*   @desc: 		    Returns the amount of bytes of free in @c_buffer
*   @params: 	    @c_buffer: the buffer to check
*   @return value:  Returns the amount of bytes free in the buffer
*   @error: 	    Undefined behavior if @c_buffer is invalid
*   @time complex:  O(1)
*   @space complex: O(1)
*/
size_t CBFreeSpace(const circular_buffer_t* c_buffer);

/* 
*   @desc: 		    Returns the amount of bytes of occupied in @c_buffer
*   @params: 	    @c_buffer: the buffer to check
*   @return value:  Returns the amount of bytes occupied in the buffer
*   @error: 	    Undefined behavior if @c_buffer is invalid
*   @time complex:  O(1)
*   @space complex: O(1)
*/
size_t CBGetSize(const circular_buffer_t* c_buffer);

/* 
*   @desc: 		   Returns the amount of bytes @c_buffer has allocated for data(max size of buffer)
*   @params: 	   @c_buffer: the buffer to check
*   @return value: Returns the amount of bytes allocated in the buffer
*   @error: 	   Undefined behavior if @c_buffer is invalid
*   @time complex:  O(1)
*   @space complex: O(1)
*/
size_t CBGetCapacity(const circular_buffer_t* c_buffer);

#endif  /* End of header guard */ 
