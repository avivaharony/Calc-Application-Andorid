#include <stdio.h>
//#include <math.h>

#include "calc.h"

typedef struct exp_test_case
{
    char*           expression;
    double          result;
    enum return_status  status;
}exp_test_case_t;


int main()
{
    int i, num_of_errors = 0;
    enum return_status status = SUCCESS;
    double res;
    calc_t *calc = CalcCreate();
    const char *errors[] = {"SUCCESS", "END", "MEMORY ALLOCATION", "SYNTAX", "MATH ERROR"};

    exp_test_case_t  test_cases[] = {
            {"1", 1, SUCCESS},
            {"2345", 2345, SUCCESS},
            {"(123)", 123, SUCCESS},
            {"1+1", 2, SUCCESS},
            {"33-12", 21, SUCCESS},
            {"333*3", 999, SUCCESS},
            {"1/2", 0.5, SUCCESS},
            {"(1+1)", 2, SUCCESS},
            {"(5-3)-2", 0, SUCCESS},
            {"5-(3-2)", 4, SUCCESS},
            {"5-3-2", 0, SUCCESS},
            {"(320/8)/4", 10, SUCCESS},
            {"320    /  (   8   /   4   )", 160, SUCCESS},
            {"320/8/4", 10, SUCCESS},
            {"1+2+3+4", 10, SUCCESS},
            {"1-2-3-4", -8, SUCCESS},
            {"1*2*3*4", 24, SUCCESS},
            {"12-3+4",  13, SUCCESS},
            {"12+3-4",  11, SUCCESS},
            {"8/4*2",  4, SUCCESS},
            {"8*4/2",  16, SUCCESS},
            {"12+3*5",  27, SUCCESS},
            {"12*3+5",  41, SUCCESS},
            {"2*(4+1)-3", 7, SUCCESS},
            {"((2+1)*3)", 9, SUCCESS},
            {"(2*(2-3))", -2, SUCCESS},
            {"(3*(3+3)/3)", 6, SUCCESS},
            {"8/(16*2-30)", 4, SUCCESS},
            {"(1+2+3+4+5)*2", 30, SUCCESS},
            {"1/3*6", 2, SUCCESS},
            {"1/5/2", 0.10, SUCCESS},
            {"1/(0+-2)", -0.5, SUCCESS},
            {"-1", -1, SUCCESS},
            {"-1 -2 +3 + +3 + -4", -1, SUCCESS},
            {"-1.5 - +4.5 - -0.5", -5.5, SUCCESS},
            {"-1.5-+4.5--0.5", -5.5, SUCCESS},
            {"-1.5--4.5", 3, SUCCESS},
            {"-1.5 - -4.5", 3, SUCCESS},
            {"- 6", 0, SYNTAX_ERROR},
            {"-", 0, SYNTAX_ERROR},
            {"7**6", 0, SYNTAX_ERROR},
            {"2 3", 0, SYNTAX_ERROR},
            {"+9 *", 0, SYNTAX_ERROR},
            {"-6+-2+-5*-3", 7, SUCCESS},
            {"-6+-2+-5.5*-3.5", 11.25, SUCCESS},
            {"+43 - 1", 42, SUCCESS},
            {"-0.5+0.5", 0, SUCCESS},
            {"0.5-0.5", 0, SUCCESS},
            {"-42 * -1", 42, SUCCESS},
            {"0-42.5", -42.5, SUCCESS},
            {"0.5+41.5", 42, SUCCESS},
            {")6", 0, SYNTAX_ERROR},
            {"(4))", 0, SYNTAX_ERROR},
            {"((8", 0, SYNTAX_ERROR},
            {"2^(2)", 4, SUCCESS},
            {"(-2)^(3)", -8, SUCCESS},
            {"(-2)^(-3)", -1.0/8, SUCCESS},
            {"2^(-2)", 1.0/4, SUCCESS},
            {"((2-4)^3/((2*2)^2*2-30))^2", 16, SUCCESS},
            {"6/(3-3)", 0, MATH_ERROR},
            {NULL, 0, SUCCESS}
    };

    for (i = 0; test_cases[i].expression; ++i)
    {
        status = CalcCalc(calc, &res, test_cases[i].expression);
        if (status == test_cases[i].status)
        {
            if (SUCCESS == status)
            {
                /*printf("Expression# %d: %s = %.2f\n",i + 1, test_case[i].expression, res);*/
                if (res != test_cases[i].result)
                {
                    ++num_of_errors;
                    printf("-----> Error. Bad result: Expression: \"%s\".\nExpected: %f. Result: %f.\n\n", test_cases[i].expression, test_cases[i].result, res);
                }
            }
        }
        else
        {
            printf("-----> Error. Bad status: Expression: \"%s\".\nExpected: %s. Status: %s.\n"
                   "\nStopping test cases.\nPerformed %d test cases out of %lu, having %d errors before this one.\n\n",
                   test_cases[i].expression, errors[test_cases[i].status], errors[status], i, sizeof(test_cases) / sizeof(test_cases[0]) - 1, num_of_errors);
            break;
        }
    }

    if (i+1 == (sizeof(test_cases) / sizeof(test_cases[0])))
    {
        printf("Performed %d test cases out of %lu, with %d errors.\n", i, sizeof(test_cases) / sizeof(test_cases[0]) - 1, num_of_errors);
    }

    CalcDestroy(calc); calc = NULL;

    return (0);
}




