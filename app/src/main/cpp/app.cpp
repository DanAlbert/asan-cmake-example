#include <jni.h>

extern "C" JNIEXPORT jstring JNICALL foo(JNIEnv *env, jobject) {
    return env->NewStringUTF("Hello, world!");
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/android/developer/asantest/MainActivity");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {"stringFromJNI", "()Ljava/lang/String;", reinterpret_cast<void*>(foo)},
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}
