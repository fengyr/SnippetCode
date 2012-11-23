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

/* InnerTestClass releated */
const char *inner_class_name = "org/example/jni/TestJni$InnerTestClass";

static struct testJni_innerClass {
    jclass      innerClass;
    jmethodID   callInnerId;   
};
static struct testJni_innerClass g_inner_class;

/**
 * @Synopsis 在调用Java对象之前，事先获取对应Java类的对象和方法ID,
 *           并将其保存到数据结构中。
 *
 * @Param env
 */
void get_inner_class_info(JNIEnv *env)
{
    g_inner_class.innerClass = (*env)->FindClass(env, inner_class_name);
    g_inner_class.callInnerId = (*env)->GetMethodID(env, g_inner_class.innerClass, "callInnerClass", "(Ljava/lang/String;)V");
}

void JNICALL Java_org_example_jni_TestJni_hello
(JNIEnv *e, jclass c) {
    printf("hello jni!\n");
}
/* InnerTestClass releated */

/*
 * Class:     org_example_jni_TestJni
 * Method:    hello2
 * Signature: ([Ljava/lang/String;)I
 */
jint JNICALL Java_org_example_jni_TestJni_hello2
(JNIEnv *env, jclass c, jobjectArray a) {
    jsize size = (*env)->GetArrayLength(env, a);
    int i = 0;
    for (i = 0; i < size; i++) {
        jobject obj = (*env)->GetObjectArrayElement(env, a, i);
        const char *langArray = (obj != 0) ? (*env)->GetStringUTFChars(env, obj, 0) : 0;
        printf("%s, ", langArray);

        if (obj != 0) {
            (*env)->ReleaseStringUTFChars(env, obj, langArray);
        }
    }

    printf("\n");

    return 0;
}

/*
 * Class:     org_example_jni_TestJni
 * Method:    hello3
 * Signature: ([II)V
 */
jstring JNICALL Java_org_example_jni_TestJni_hello3
(JNIEnv *env, jclass c, jintArray ia, jint len) {
    jboolean isCopy = JNI_TRUE;
    jint *i_array = (*env)->GetIntArrayElements(env, ia, &isCopy);
    int i = 0;
    for (i = 0; i < len; i++) {
        printf("hello3 num=%d\n", i_array[i]);
    }

    if (i_array != NULL) {
        (*env)->ReleaseIntArrayElements(env, ia, i_array, 1);
    }

    return NULL; 
}

/*
 * Class:     org_example_jni_TestJni
 * Method:    callInner
 * Signature: (Lorg/example/jni/TestJni/InnerTestClass;)V
 */
JNIEXPORT void JNICALL Java_org_example_jni_TestJni_callInner
(JNIEnv *env, jclass class, jobject obj) {
    jstring jstr;
    jstr = (*env)->NewStringUTF(env, "Now InnerClass called!");
    (*env)->CallVoidMethod(env, obj, g_inner_class.callInnerId, jstr);

    jboolean hasExcept = (*env)->ExceptionCheck(env);
    if (hasExcept) {
        printf("Has innerClass Exception!!!\n");
    }

    /* clear exception */
    (*env)->ExceptionClear(env);
}
