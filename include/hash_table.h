#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stddef.h>     /* size_t */

typedef size_t (*hash_func_t)(const void* data);
typedef int (*action_func_t)(void* data, void* param);
typedef int (*is_match_t)(const void* data, const void* param);

typedef struct hash_table hash_table_t;

/* 
*   @details:       Allocates new hash table with @n_buckets and @hash_func 
*					as it's hash function and will use @match_func for matching 
*					elements
*	@param:			@n_buckets: Num of buckets to allocate
*					@hash_func: Hash function for hashing objects
*					@match_func: Match function for matching objects
*   @return value: 	Allocated Hash Table
*   @error: 	    Return NULL if allocation failed
*					Undefined behavior if @hash_func is invalid or @match_func 
*					is invalid
*   @time complex: 	O(n * malloc) for both AC/WC
*   @space complex: O(n * malloc) for both AC/WC
*/
hash_table_t* HTCreate(size_t n_buckets, hash_func_t hash_func, 
                                                        is_match_t match_func);

/* 
*   @desc:          Frees @hash_table which was allocated using @HTCreate
*	@param:			@hash_table: preallocated hash table
*   @return value:  None
*   @error: 		Undefined behvaior if @hash_table is not valid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(n) for both AC/WC
*/
void HTDestroy(hash_table_t* hash_table);

/* 
*   @desc:          Removes @data from @hash_table using @match_func
*	@param:			@hash_table: preallocated hash table
*					@data: data to remove from @hash_table
*   @return value:  None
*   @error: 		Undefined behavior if @hash_table is invalid
*   @time complex: 	O(1) for AC, O(n) for WC
*   @space complex: O(1) for both AC/WC
*/
void HTRemove(hash_table_t* hash_table, const void* data);

/* 
*   @desc:          Inserts @data to @hash_table
*	@param:			@hash_table: preallocated hash table
*					@data: data to insert to @hash_table
*   @return value:  Returns zero if inserts success otherwise will return 
*					nonzero
*   @error: 		returns nonzero value if allocation failed
*   @time complex: 	AC O(1)  WC O(n)
*   @space complex: O(1) for both AC/WC
*/
int HTInsert(hash_table_t* hash_table, const void* data);

/* 
*   @desc:          Returns the count of elements in @hash_table
*	@param:			@hash_table: preallocated hash table
*   @return value:  Returns the count of elements in @hash_table
*   @error: 		Undefined behavior if @hash_table is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
size_t HTGetSize(const hash_table_t* hash_table);

/* 
*   @desc:          Checks if @hash_table is empty
*   @params:        @hash_table: preallocated hash table
*   @return value:  Returns one if @hash_table is empty or zero returns 
*   @error: 		Undefined behavior if @hash_table is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int HTIsEmpty(const hash_table_t* hash_table);

/* 
*   @desc:          Searches @hash_table for @data
*   @params:        @hash_table: preallocated hash table
*                   @data: data to search for
*   @return value:  Returns the found data if found otherwise returns NULL
*   @error: 		Undefined behavior if @hash_table is invalid
*   @time complex: 	O(1) for AC, O(n) for WC
*   @space complex: O(1) for both AC/WC
*/
void* HTFind(const hash_table_t* hash_table, const void* data);

/*  
*   @desc:          Iterates over each element in @hash_table and performs 
*                   @action_func
*                   with @param and said element.
*   @params:        @hash_table: preallocated hash table
*                   @action_func: a function that does user defined actions
*                   on elements data and @param and returns zero upon success
*					@param: a user defined additional parameter to pass into 
*					@action_func
*   @return value:  Returns zero if the function performed successfully
*                   otherwise returns @action_func return value.
*   @error: 		Undefined behavior if @hash_table is invalid or 
*                   @action_func is invalid or
*                   user changes an element hash value.
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
int HTForeach(const hash_table_t* hash_table, action_func_t action_func, 
                void* param);

/*  
*   @desc:          Returns @hash_table load factor
*   @params:        @hash_table preallocated hash table
*   @return value:  Returns @hash_table: load factor
*   @error: 		Undefined behavior if @hash_table is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
double HTLoadFactor(const hash_table_t* hash_table);


/*  
*   @desc:          Returns @hash_table standard deviation
*   @params:        @hash_table: preallocated hash table
*   @return value:  Returns @hash_table load factor
*   @error: 		Undefined behavior if @hash_table is invalid
*   @time complex: 	O(n) for both AC/WC
*   @space complex: O(1) for both AC/WC
*/
double HTGetSD(const hash_table_t* hash_table);

#endif /* __HASH_TABLE_H__ */