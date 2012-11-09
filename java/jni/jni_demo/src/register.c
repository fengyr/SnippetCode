/*
 * =====================================================================================
 *
 *       Filename:  register.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十一月 08, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "org_example_jni_TestJni.h"
#include <assert.h>

static const JNINativeMethod gMethods[] = {
    {"hello", "()V", (void*)Java_org_example_jni_TestJni_hello},
    {"hello2", "([Ljava/lang/String;)I", (void*)Java_org_example_jni_TestJni_hello2},
    {"hello3", "([Ljava/lang/String;[I)Z", (void*)Java_org_example_jni_TestJni_hello3},
    {"hello4", "([Ljava/lang/String;[II)Ljava/lang/String;", (void*)Java_org_example_jni_TestJni_hello4},
};

int registerMethods(JNIEnv* env) {
    static const char* const kClassName = "org/example/jni/TestJni";
    jclass clazz;
 
    clazz = (*env)->FindClass(env, kClassName);
    if (clazz == NULL) {
        return -1;
    }
 
    if ((*env)->RegisterNatives(env, clazz, gMethods,
            sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK) {
        return -1;
    }
 
    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;
 
    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }
    assert(env != NULL);
 
    registerMethods(env);
 
    result = JNI_VERSION_1_4;
 
bail:
    return result;
}
