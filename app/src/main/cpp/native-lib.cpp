#include <jni.h>
#include <string>
#include "jvmti.h"
#include <android/log.h>

jvmtiEnv *selfJvmtiEnv;

extern "C" JNIEXPORT jstring JNICALL
Java_com_zhengdianfang_jvmtitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void ObjectAllocCallback(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jobject object,
                         jclass object_klass, jlong size) {

    char *classSign;
    selfJvmtiEnv->GetClassSignature(object_klass, &classSign, nullptr);
    __android_log_print(ANDROID_LOG_DEBUG, "JVMTITest", "the object alloc name is %s , size is %d",  classSign, size);
    selfJvmtiEnv->Deallocate(reinterpret_cast<unsigned char *>(classSign));
}

void ObjectFreeCallback(jvmtiEnv *jvmti_env, jlong tag) {
    __android_log_print(ANDROID_LOG_DEBUG, "JVMTITest", "the object free, size is %d", tag);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zhengdianfang_jvmtitest_JVMTIManager_native_1start(JNIEnv *env,  jobject) {

    jvmtiEventCallbacks callbacks;
    callbacks.VMObjectAlloc = ObjectAllocCallback;
    callbacks.ObjectFree = ObjectFreeCallback;
    selfJvmtiEnv->SetEventCallbacks(&callbacks, sizeof(callbacks));

    selfJvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, NULL);
    selfJvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_OBJECT_FREE, NULL);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_zhengdianfang_jvmtitest_JVMTIManager_native_1stop(JNIEnv *env,  jobject) {
    selfJvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, NULL);
    selfJvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_OBJECT_FREE, NULL);
}

JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM *vm, char *options, void *reserved) {
    vm->GetEnv(reinterpret_cast<void **>(&selfJvmtiEnv), JVMTI_VERSION_1_2);
    jvmtiCapabilities capabilities;
    selfJvmtiEnv->GetPotentialCapabilities(&capabilities);
    selfJvmtiEnv->AddCapabilities(&capabilities);
    return JNI_OK;
}
