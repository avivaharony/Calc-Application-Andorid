//
// Created by aviv on 17/11/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "calc.h"
#include "../jni/calc-jni.h"

typedef struct exp_test_case
{
    char*           expression;
    double          result;
    enum return_status  status;
}exp_test_case_t;


calc_t *calc = NULL;

JNIEXPORT void JNICALL
Java_com_example_calcapplication_MainActivity_createCalculatorJNI(JNIEnv *env, jobject thiz) {
    // TODO: implement createCalculatorJNI()

    calc = CalcCreate();
}

JNIEXPORT void JNICALL
Java_com_example_calcapplication_MainActivity_destroyCalculatorJNI(JNIEnv *env, jobject thiz) {
    // TODO: implement destroyCalculatorJNI()
    CalcDestroy(calc);
    calc = NULL;
}


JNIEXPORT jobject JNICALL
Java_com_example_calcapplication_MainActivity_calcEquationWithObjectJNI(JNIEnv *env, jobject thiz,
                                                                        jstring equation) {
    // TODO: implement calcEquationWithObjectJNI()

    jmethodID constructor;
    jvalue args[2];
    jobject object;

    jclass cls = (*env)->FindClass(env, "com/example/calcapplication/CalculatorObject");

    //convert jstring to char*
    jboolean isCopy;
    const char *nativeEquationString = (*env)->GetStringUTFChars(env, equation, &isCopy);

    double result = 0;

    enum return_status status = SUCCESS;
    double res;

    if (calc != NULL) {

        status = CalcCalc(calc, &res, nativeEquationString);
        if (SUCCESS == status) {
            result = res;
        }

    }
    //release conver jstring to char*
    (*env)->ReleaseStringUTFChars(env, equation, nativeEquationString);

    constructor = (*env)->GetMethodID(env, cls, "<init>", "(DLjava/lang/String;)V");
    args[0].d = result;
    const char *errors[] = {"SUCCESS", "END", "MEMORY ALLOCATION", "SYNTAX ERROR", "MATH ERROR"};
    args[1].l = (*env)->NewStringUTF(env, errors[status]);

    object = (*env)->NewObjectA(env, cls, constructor, args);

    return object;

}
