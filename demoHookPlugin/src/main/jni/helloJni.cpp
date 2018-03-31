#include "jni.h"
#include <android/log.h>
#include <string.h>
#include <dlfcn.h>
#include <string>
#include <algorithm>
#include "sodef.h"

#define LOG_TAG "VA++"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define HOOK_DEF(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)

#ifdef __arm__
#define GETLR(store_lr)	__asm__ __volatile__("mov %0, lr\n\t" : "=r"(store_lr))
#else
#define GETLR(store_lr) ((void)0)
#endif

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    LOGE("hello world from jni");

    return JNI_VERSION_1_6;
}


HOOK_DEF(char*, hookme, char* buff)
{
    long lr;
    GETLR(lr);
    LOGE("call at %X", lr);
    return strcpy(buff, "hook me success");
}

typedef void (*fn_MSHookFunction)(void *symbol, void *replace, void **result);
extern "C" void on_dlopen(const char *name, void *handle, fn_MSHookFunction hookfun)
{
    soinfo* p_si = (soinfo*)handle;
    std::string sopath = name;
    std::transform(sopath.begin(), sopath.end(), sopath.begin(), ::tolower);
    if (sopath.find("libcommlib.so") != std::string::npos)
    {
        void * symbol = dlsym(handle, "hookme");
        hookfun(symbol, (void *)new_hookme, (void **)&orig_hookme);
        LOGD("find and hook -- %X", symbol);
    }

    LOGD("hook-load %s", name);
}