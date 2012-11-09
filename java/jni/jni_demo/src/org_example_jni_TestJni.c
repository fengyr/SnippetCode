/**
 *       File       org_example_jni_TestJni.c
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.00.00
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2011年03月15日 14时48分37秒
 *
 * =====================================================================================
 */
#include "org_example_jni_TestJni.h"
#include <stdio.h>

void JNICALL Java_org_example_jni_TestJni_hello
(JNIEnv *e, jclass c) 
{
    printf("hello jni!\n");
}

/*
 * Class:     org_example_jni_TestJni
 * Method:    hello2
 * Signature: ([Ljava/lang/String;)I
 */
jint JNICALL Java_org_example_jni_TestJni_hello2
(JNIEnv *e, jclass c, jobjectArray a) 
{
    return 0;
}

/*
 * Class:     org_example_jni_TestJni
 * Method:    hello3
 * Signature: ([Ljava/lang/String;[I)V
 */
jboolean JNICALL Java_org_example_jni_TestJni_hello3
(JNIEnv *e, jclass c, jobjectArray o, jintArray a) 
{
    return 0;
}

/*
 * Class:     org_example_jni_TestJni
 * Method:    hello4
 * Signature: ([Ljava/lang/String;[II)V
 */
jstring JNICALL Java_org_example_jni_TestJni_hello4
(JNIEnv *e, jclass c, jobjectArray o, jintArray ia, jint i) 
{
    return NULL; 
}
