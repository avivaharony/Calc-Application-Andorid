#include <stdlib.h> /*malloc */
#include <string.h> /*strlen, strtod */
#include <assert.h>

#include "stack.h"
#include "calc.h"

#define ASCII_CHARS 256

/* --------------------------------------------- */

enum states
{
    EXPECT_OPERAND,
    EXPECT_OPERATOR,
    MAX_STATES
};

enum events
{
	ERROR_EV = 0,
	NUM_EV,
	SPACE_EV,
	PLUS_EV,
	MINUS_EV,
	MUL_EV,
	DIV_EV,
	POWER_EV,
	LEFT_BRACKET_EV,
	RIGHT_BRACKET_EV,
	NUL_EV,
	MAX_EVENTS
};

enum operations
{
	ADD_OPER,
	SUB_OPER,
	MUL_OPER,
	DIV_OPER,
	POWER_OPER,
	LEFT_BRACKET_OPER,
	MAX_OPERATIONS
};

enum precdences
{
	NULL_PRCD = 0,
	LEFT_BRACKET_PRCD = 1,
	ADD_PRCD = 2,
	SUB_PRCD = 2,
	MUL_PRCD = 3,
	DIV_PRCD = 3,
	POWER_PRCD = 4,
	MAX_PRECEDENCES
};

/* --------------------------------------------- */

typedef struct opertion_st
{
	enum return_status (*func) (double num1, double num2, double *result);
	enum precdences precdence;
	
}opertion_t;

struct calc_st
{
	stack_t *operands_stack;
	stack_t *operators_stack;
	enum states curr_state;
	char *equation;
	char *curr_equ;
	enum events events_map[ASCII_CHARS];
	enum events operations_map[MAX_EVENTS];
	enum return_status (*state_per_event[MAX_STATES][MAX_EVENTS]) (calc_t *calc);
	
	opertion_t operations[MAX_OPERATIONS];
};

/* --------------------------------------------- */

static enum return_status OperandHandler (calc_t *calc);
static enum return_status SpaceHandler (calc_t *calc);
static enum return_status ErrorHandler (calc_t *calc);
static enum return_status LeftBracketHandler (calc_t *calc);
static enum return_status RightBracketHandler (calc_t *calc);
static enum return_status OperatorHandler (calc_t *calc);
static enum return_status NullHandler (calc_t *calc);

static enum return_status AddNumbers (double num1, double num2, double *result);
static enum return_status SubNumbers (double num1, double num2, double *result);
static enum return_status MultNumbers (double num1, double num2, double *result);
static enum return_status DivNumbers (double num1, double num2, double *result);
static enum return_status PowerNumbers (double num1, double num2, double *result);

static enum return_status InitState(calc_t *calc, const char *equation);

static enum events GetEvent(char **curr_equ, enum events *events_map);
static int ComparePrecedence(calc_t *calc, unsigned char next_operator, unsigned char stack_op);

static enum operations GetOperation(calc_t *calc, unsigned char oper);
static enum return_status IvokeFunction(calc_t *calc, unsigned char stack_op, double *result);

static void SetMapTables(calc_t *calc);

/* --------------------------------------------- */

calc_t *CalcCreate(void)
{
	calc_t *calc = NULL;
	
	calc = (calc_t*)malloc(sizeof(calc_t));
	if (NULL == calc)
	{
		return (NULL); 
	}
	
	SetMapTables(calc);
	
	
	return (calc);
}

/* --------------------------------------------- */

static void SetMapTables(calc_t *calc)
{

	int num_ascii = '0';
	
	memset(calc->events_map, ERROR_EV, ASCII_CHARS * sizeof(int));

	for (; num_ascii <= '9' ; ++num_ascii)
	{
		calc->events_map[num_ascii] = NUM_EV;
	}
		
	calc->events_map[' '] = SPACE_EV;
	calc->events_map['\t'] = SPACE_EV;
	calc->events_map['+'] = PLUS_EV;
	calc->events_map['-'] = MINUS_EV;
	calc->events_map['*'] = MUL_EV;
	calc->events_map['/'] = DIV_EV;
	calc->events_map['^'] = POWER_EV;
	calc->events_map['('] = LEFT_BRACKET_EV;
	calc->events_map[')'] = RIGHT_BRACKET_EV;
	calc->events_map['\0'] = NUL_EV;
	
	calc->operations_map[PLUS_EV] = ADD_OPER;
	calc->operations_map[MINUS_EV] = SUB_OPER;
	calc->operations_map[MUL_EV] = MUL_OPER;
	calc->operations_map[DIV_EV] = DIV_OPER;
	calc->operations_map[POWER_EV] = POWER_OPER;
	calc->operations_map[LEFT_BRACKET_EV] = LEFT_BRACKET_OPER;
	
	calc->state_per_event[EXPECT_OPERAND][NUM_EV] = OperandHandler;
	calc->state_per_event[EXPECT_OPERAND][SPACE_EV] = SpaceHandler;
	calc->state_per_event[EXPECT_OPERAND][PLUS_EV] = OperandHandler;
	calc->state_per_event[EXPECT_OPERAND][MINUS_EV] = OperandHandler;
	calc->state_per_event[EXPECT_OPERAND][MUL_EV] = ErrorHandler;
	calc->state_per_event[EXPECT_OPERAND][DIV_EV] = ErrorHandler;
	calc->state_per_event[EXPECT_OPERAND][POWER_EV] = ErrorHandler;
	calc->state_per_event[EXPECT_OPERAND][LEFT_BRACKET_EV] = LeftBracketHandler;
	calc->state_per_event[EXPECT_OPERAND][RIGHT_BRACKET_EV] = ErrorHandler;
	calc->state_per_event[EXPECT_OPERAND][NUL_EV] = ErrorHandler;
	
	calc->state_per_event[EXPECT_OPERATOR][NUM_EV] = ErrorHandler;
	calc->state_per_event[EXPECT_OPERATOR][SPACE_EV] = SpaceHandler;
	calc->state_per_event[EXPECT_OPERATOR][PLUS_EV] = OperatorHandler;
	calc->state_per_event[EXPECT_OPERATOR][MINUS_EV] = OperatorHandler;
	calc->state_per_event[EXPECT_OPERATOR][MUL_EV] = OperatorHandler;
	calc->state_per_event[EXPECT_OPERATOR][DIV_EV] = OperatorHandler;
	calc->state_per_event[EXPECT_OPERATOR][POWER_EV] = OperatorHandler;
	calc->state_per_event[EXPECT_OPERATOR][LEFT_BRACKET_EV] = ErrorHandler;
	calc->state_per_event[EXPECT_OPERATOR][RIGHT_BRACKET_EV] = RightBracketHandler;
	calc->state_per_event[EXPECT_OPERATOR][NUL_EV] = NullHandler;
	
	calc->operations[ADD_OPER].func = AddNumbers;
	calc->operations[ADD_OPER].precdence = ADD_PRCD;

	calc->operations[SUB_OPER].func = SubNumbers;
	calc->operations[SUB_OPER].precdence = SUB_PRCD;
	
	calc->operations[MUL_OPER].func = MultNumbers;
	calc->operations[MUL_OPER].precdence = MUL_PRCD;
	
	calc->operations[DIV_OPER].func = DivNumbers;
	calc->operations[DIV_OPER].precdence = DIV_PRCD;

	calc->operations[POWER_OPER].func = PowerNumbers;
	calc->operations[POWER_OPER].precdence = POWER_PRCD;

	calc->operations[LEFT_BRACKET_OPER].func = AddNumbers;
	calc->operations[LEFT_BRACKET_OPER].precdence = LEFT_BRACKET_PRCD;
}
/* --------------------------------------------- */

void CalcDestroy(calc_t *calc)
{
	assert (calc != NULL);

	free(calc);
	calc = NULL;
}

/* --------------------------------------------- */

enum return_status CalcCalc(calc_t *calc, double *result, const char *equation)
{
	enum return_status ret_stat = SUCCESS;
	enum events event = 0;
	
	assert(calc && result && equation);

	ret_stat = InitState(calc, equation);
	if (ret_stat != SUCCESS)
	{
		return (ret_stat);
	}

	while (SUCCESS == ret_stat)
	{
		event = GetEvent(&calc->curr_equ, calc->events_map);
	
		ret_stat = calc->state_per_event[calc->curr_state][event](calc);
	}

	if (END == ret_stat)
	{
		*result = *(double*)StackPeek(calc->operands_stack);
		ret_stat = SUCCESS;
	}

	StackDestroy(calc->operands_stack);
	StackDestroy(calc->operators_stack);
	free(calc->equation);

	return (ret_stat);
}

/* --------------------------------------------- */

static enum return_status InitState(calc_t *calc, const char *equation)
{
	size_t equ_size = 0;
	
	assert(calc);

	calc->curr_state = EXPECT_OPERAND;

	equ_size = strlen(equation);

	calc->equation = (char *)malloc((equ_size+1) * sizeof(char));
	if (calc->equation == NULL) 
	{
		return (ALLOCATE_FAILURE);
	}
	calc->equation = strcpy(calc->equation, equation);

	calc->curr_equ = calc->equation;
	
	calc->operands_stack = (stack_t*)StackCreate(equ_size, sizeof(double));
	if (NULL == calc->operands_stack)
	{
		free(calc->equation);
		return (ALLOCATE_FAILURE);
	}		
	calc->operators_stack = (stack_t*)StackCreate(equ_size, sizeof(char));
	if (NULL == calc->operators_stack)
	{
		free(calc->equation);
		StackDestroy(calc->operands_stack);
		return (ALLOCATE_FAILURE);
	}
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum events GetEvent(char **curr_equ, enum events *events_map)
{
	unsigned char event_idx = '0';
	
	assert(curr_equ != NULL);
	assert(events_map != NULL);

	event_idx = **curr_equ;

	return (events_map[event_idx]);
}

/* --------------------------------------------- */

static enum return_status OperandHandler (calc_t *calc)
{
	char *dup_equ = NULL;
	double num = 0.0;
	
	assert(calc != NULL);

	dup_equ = calc->curr_equ;
		
	num = strtod(dup_equ, &calc->curr_equ);
	
	if (dup_equ == calc->curr_equ)
	{
		return (SYNTAX_ERROR);
	}
	
	StackPush(calc->operands_stack, &num);
	
	calc->curr_state = EXPECT_OPERATOR;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status SpaceHandler (calc_t *calc)
{
	
	assert(calc != NULL);
	
	++calc->curr_equ;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status ErrorHandler (calc_t *calc)
{
	(void)calc;
	
	return (SYNTAX_ERROR);
}

/* --------------------------------------------- */

static enum return_status LeftBracketHandler (calc_t *calc)
{
	enum return_status ret_stat = SUCCESS;
	char bracket = '(';
	
	assert(calc != NULL);

	++calc->curr_equ;
	
	StackPush(calc->operators_stack, &bracket);
	
	return (ret_stat);
}

/* --------------------------------------------- */

static enum return_status RightBracketHandler (calc_t *calc)
{

	enum return_status ret_stat = SUCCESS;
	double result = 0.0;
	unsigned char open_bracket = '(';
	unsigned char stack_op = open_bracket;
	
	if (StackSize(calc->operators_stack) == 0)
	{
		return (SYNTAX_ERROR);
	}
	stack_op = *(char*)StackPeek(calc->operators_stack);
	
	assert(calc != NULL);
	
	while (stack_op != open_bracket )
	{
		ret_stat = IvokeFunction(calc ,stack_op, &result);
		if (ret_stat != SUCCESS)
		{
			return (ret_stat);
		}

		if (StackSize(calc->operators_stack) == 0)
		{
			return (SYNTAX_ERROR);
		}
		stack_op = *(char*)StackPeek(calc->operators_stack);
	}
	
	StackPop(calc->operators_stack);
	
	++calc->curr_equ;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status OperatorHandler (calc_t *calc)
{
	unsigned char next_operator = 0;
	char *stack_op_ptr = NULL;
	unsigned char stack_op = 0;
	double result = 0.0;

	enum return_status ret_stat = SUCCESS;
	
	assert(calc != NULL);

	next_operator = *calc->curr_equ;
	
	stack_op_ptr = StackPeek(calc->operators_stack);

	if (stack_op_ptr != NULL)
	{
		stack_op = *stack_op_ptr;
		/* to calculate when precdence equal or lower */
		if (ComparePrecedence(calc, next_operator, stack_op))
		{
			ret_stat = IvokeFunction(calc ,stack_op, &result);
			if (ret_stat != SUCCESS)
			{
				return (ret_stat);
			}
		}
	}
	
	StackPush(calc->operators_stack, &next_operator);
	calc->curr_state = EXPECT_OPERAND;
	
	++calc->curr_equ;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status IvokeFunction(calc_t *calc, unsigned char stack_op, double *result)
{
	double num1 = 0.0, num2 = 0.0;
	enum return_status ret_stat = SUCCESS;
	enum operations oper_to_run = 0;
	
	assert(calc != NULL);
	
	num1 = *(double*)StackPeek(calc->operands_stack);
	StackPop(calc->operands_stack);
	num2 = *(double*)StackPeek(calc->operands_stack);
	StackPop(calc->operands_stack);
	
	oper_to_run = GetOperation(calc, stack_op);
	ret_stat = calc->operations[oper_to_run].func(num1, num2, result);

	StackPop(calc->operators_stack);

	StackPush(calc->operands_stack, result);
		
	return (ret_stat);

}

/* --------------------------------------------- */
/* returns operation according to given char using Look Up Tables */
static enum operations GetOperation(calc_t *calc, unsigned char oper)
{
	assert(calc != NULL);
	
	return (calc->operations_map[calc->events_map[oper]]);
}

/* --------------------------------------------- */

static int ComparePrecedence(calc_t *calc, unsigned char next_operator, unsigned char stack_op)
{
	enum precdences next_op_prced;
	enum precdences stack_op_prced;
	
	enum operations next_operation = 0;
	enum operations stack_operation = 0;
	
	assert(calc != NULL);
	
	next_operation = GetOperation(calc, next_operator);
	next_op_prced = calc->operations[next_operation].precdence;
	
	stack_operation = GetOperation(calc, stack_op);
	stack_op_prced = calc->operations[stack_operation].precdence;
	
	return (next_op_prced <= stack_op_prced);
}

/* --------------------------------------------- */

static enum return_status NullHandler (calc_t *calc)
{
	enum return_status ret_stat = SUCCESS;
	double result = 0.0;
	unsigned char stack_op = 0;
	
	assert(calc != NULL);
	
	/* loop while operators_stack stack is not empty */
	while ((ret_stat == SUCCESS) && StackSize(calc->operators_stack) != 0 )
	{
		stack_op = *(char*)StackPeek(calc->operators_stack);
		if ('(' == stack_op)
		{
			ret_stat = SYNTAX_ERROR;
			break;
		}
		ret_stat = IvokeFunction(calc ,stack_op, &result);
		/*if (ret_stat != SUCCESS)
		{
			ret_stat = SYNTAX_ERROR;
		}*/
	}
	
	if (ret_stat == SUCCESS)
	{
		ret_stat = END;
	}
	return (ret_stat);
}

/* --------------------------------------------- */

static enum return_status AddNumbers (double num1, double num2, double *result)
{
	assert (result != NULL);
	
	*result = num1 + num2;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status SubNumbers (double num1, double num2, double *result)
{
	assert (result != NULL);
	
	*result = num2 - num1;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status MultNumbers (double num1, double num2, double *result)
{
	assert (result != NULL);
	
	*result = num1 * num2;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status DivNumbers (double num1, double num2, double *result)
{
	assert (result != NULL);

	if (0 == num1)
	{
		return (MATH_ERROR);
	}
	
	*result = num2 / num1;
	
	return (SUCCESS);
}

/* --------------------------------------------- */

static enum return_status PowerNumbers (double num1, double num2, double *result)
{
	int i = 0;
	int sign = 1;
	assert (result != NULL);

	*result = 1;
	
	if (num1 < 0) {
		sign = -1;
		num1 *= -1;
	}

	for (i = 0; i < num1; ++i){
		*result *= num2;
	}

	if (-1 == sign) {
		*result = 1.0 / (*result);
	}

	return (SUCCESS);
}



