#include <stdio.h>		
#include <stdlib.h>		/* malloc */
#include <string.h> 	/* memmove */
#include <assert.h>

#include "stack.h"		

struct stack_st
{
	size_t element_size;	/* blocks number of a single element */
	char *end; 				/* Pointer to end of stack available area 	*/
	char *top;				/* Pointer to element on top of stack		*/
		
	char data[1];
};

/* Returns pointer to a new Created stack with given number of elements , and given element size .  Returns NULL pointer if malloc failed  */
/* element_size CAN'T be 0 */
/* num_elements CAN'T be 0 */ 
stack_t *StackCreate(size_t num_elements, size_t element_size)
{
	stack_t *stack = NULL;

	assert(element_size != 0);
	assert(num_elements != 0);
		
	/* Alloacate memory for stack, with size for the struct and the elements  */
	stack = (stack_t *)malloc(sizeof(stack_t) + (num_elements * element_size));	
	if (NULL == stack)
	{
		return NULL;
	}
				
	/* Set element_size field ,and pointer to end of available stack   */
	stack->element_size = element_size ;
	stack->end = stack->data + (num_elements * element_size) ;
	/* set top stack pointer to data */
	stack->top = stack->data ;
	/* return new stack_t pointer */
	
	return (stack);
}

/* Destroy stack and all it's elements */
void StackDestroy(stack_t *stack)
{
	assert(stack != NULL);
	
	free(stack);
	stack = NULL ; 
	
	return;
}

/* Removes last element from stack */
int StackPop(stack_t *stack)
{
	assert(stack != NULL);
	/* decrease top of stack pointer by stack element size */
	if (stack->top > stack->data)
	{
		stack->top -= stack->element_size;
	}	
	else
	{
		return (1);	/* can't pop - stack is empty */
	}
	
	return (0);
}


/* Adds a new element to stack , returns 0 for success and 1 for failure (Full Stack) */
int StackPush(stack_t *stack, const void * new_element)
{
	assert(stack != NULL);
	assert(new_element != 0);
	/* if stack top pointer didn't reach end pointer,				*/
	/* update stack top pointer and then copy new_element to stack . */
	if (stack->top != stack->end)
	{
		stack->top += stack->element_size;
		memcpy(stack->top, new_element, stack->element_size);
	}
	else
	{
		return (1);
	}
	
	return (0);
}

/* returns the last element in the stack  */
void *StackPeek(const stack_t *stack)
{
	assert(stack != NULL);
	if (stack->top > stack->data)
	{
		return (stack->top);
	}
	else	
	{
		return (NULL);
	}
	
}
	

/* returns stack number of elements */
size_t StackSize(const stack_t *stack)
{
	assert(stack != NULL);
	
	return ((stack->top - stack->data) / stack->element_size);
}


