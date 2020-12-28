#ifndef CALC_H_
#define CALC_H_

enum return_status
{
    SUCCESS,
    END,
    ALLOCATE_FAILURE,
    SYNTAX_ERROR,
    MATH_ERROR,
    MAX_RETURN_STATUS
};

typedef struct calc_st calc_t;

calc_t *CalcCreate(void);

enum return_status CalcCalc(calc_t *calc, double *result, const char *equation);
/*result is out parameter*/

void CalcDestroy(calc_t *calc);


#endif /*CALC_H_*/


