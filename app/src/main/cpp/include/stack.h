#ifndef STACK_
#define STACK_
#include <stddef.h>

typedef struct stack_st stack_t;

/* Returns pointer to a new Created stack with given number of elements , and given element size  ,  Returns NULL pointer if malloc failed   */ 
/* element_size and num_elements CAN'T be 0 */ 
/* StackDestroy must be used before programs ends */ 
stack_t *StackCreate(size_t num_elements, size_t element_size);
/* Destroy stack and all it's elements */
void StackDestroy(stack_t *stack);
/* Removes last element from stack */
int StackPop(stack_t *stack);
/* Adds a new element to stack , returns 0 for success and 1 for failure */
int StackPush(stack_t *stack, const void * new_element);
/* returns the last element in the stack */
void *StackPeek(const stack_t *stack);
/* returns stack number of elements */
size_t StackSize(const stack_t *stack);


#endif /*   STACK_    */
