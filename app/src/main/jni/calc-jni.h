//
// Created by aviv on 17/11/20.
//

#ifndef CALC_APPLICATION_CALC_JNI_H
#define CALC_APPLICATION_CALC_JNI_H

#include "../../../../../../Android/Sdk/ndk/22.0.6917172/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/jni.h"

//extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_calcapplication_MainActivity_calcEquationJNI(JNIEnv *env, jobject thiz,
                                                              jstring equation);

#endif //CALC_APPLICATION_HELLO_JNI_H
