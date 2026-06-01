#define LOGO_DEFINE_OPENJDK
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <jni.h>
#include "runtime_sysinfo.h"
#include "logo.h"

// VM start time — captured when JNI_OnLoad is called
static double vm_start_time = 0.0;

// Compiled mode flag — set by printInfoCompiled()
static int compiled_mode = 0;

// Global state holders to bridge JNI arguments to the shared sysinfo_print engine
static char g_version[64]     = "Unknown";
static char g_impl[64]        = "OpenJDK";
static char g_environment[64] = "Unknown";
static char g_memory[64]      = "Unknown";
static char g_gc_objects[64]  = "Unknown";
static char g_threads[32]     = "Unknown";

int embfetch_is_compiled_mode(void) {
    return compiled_mode;
}

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024 * 1024) {
        snprintf(dst, len, "%.1fGB", (double)bytes / (1024.0 * 1024.0 * 1024.0));
    } else if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%.1fMB", (double)bytes / (1024.0 * 1024.0));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%.1fKB", (double)bytes / 1024.0);
    } else {
        snprintf(dst, len, "%zuB", bytes);
    }
}

// Static info — JVM implementation and version via System properties
void sysinfo_fetch_static(sysinfo_runtime_static_t *dst) {
    // These are set by the Java side before calling printInfo()
    // via System.getProperty() — stored in thread-local or global state
    // For simplicity, defaults are set here and overridden by Java side
    dst->implementation = g_impl;
    dst->version        = g_version;
    dst->environment    = g_environment;
    dst->build_date     = __DATE__ " " __TIME__;
}

// Dynamic info — heap, GC, threads via JNI ManagementFactory
void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst) {
    // Uptime
    double now      = (double)clock() / CLOCKS_PER_SEC;
    double uptime_s = now - vm_start_time;
    dst->uptime_m = (uint32_t)(uptime_s / 60.0);
    dst->uptime_s = (uint32_t)((uint64_t)uptime_s % 60);

    // Heap, GC, threads — populated by Java side via JNI callbacks
    // Copy values passed up from the Java Management API
    snprintf(dst->threads,    sizeof(dst->threads),    "%s", g_threads);
    snprintf(dst->gc_objects, sizeof(dst->gc_objects), "%s", g_gc_objects);
    snprintf(dst->memory,     sizeof(dst->memory),     "%s", g_memory);
}

// JNI output wrapper
static void jni_putline(void *ctx, const char *line) {
    JNIEnv *env = (JNIEnv *)ctx;
    if (env != NULL) {
        // Find System.out
        jclass sys_class = (*env)->FindClass(env, "java/lang/System");
        jfieldID out_id = (*env)->GetStaticFieldID(env, sys_class, "out", "Ljava/io/PrintStream;");
        jobject out_obj = (*env)->GetStaticObjectField(env, sys_class, out_id);
        
        // Find PrintStream.println(String)
        jclass ps_class = (*env)->FindClass(env, "java/io/PrintStream");
        jmethodID println_id = (*env)->GetMethodID(env, ps_class, "println", "(Ljava/lang/String;)V");
        
        // Print it using Java's stream so JShell can capture it perfectly!
        jstring jmsg = (*env)->NewStringUTF(env, line);
        (*env)->CallVoidMethod(env, out_obj, println_id, jmsg);
        (*env)->DeleteLocalRef(env, jmsg);
    } else {
        // Fallback for regular compiled Java terminal execution
        printf("%s\n", line);
        fflush(stdout);
    }
}

// JNI method — Embfetch.printInfo()
JNIEXPORT void JNICALL Java_Embfetch_printInfo
    (JNIEnv *env, jclass cls,
     jstring jversion, jstring jimpl, jstring jenvironment,
     jlong heap_used, jlong heap_max,
     jlong gc_count, jint thread_count) {

    compiled_mode = 0;

    // 1. Extract Java strings safely
    const char *version     = (*env)->GetStringUTFChars(env, jversion, NULL);
    const char *impl        = (*env)->GetStringUTFChars(env, jimpl, NULL);
    const char *environment = (*env)->GetStringUTFChars(env, jenvironment, NULL);

    // 2. Use strncpy to make a concrete, local buffer copy that won't get lost
    if (version)     strncpy(g_version,     version,     sizeof(g_version) - 1);
    if (impl)        strncpy(g_impl,        impl,        sizeof(g_impl) - 1);
    if (environment) strncpy(g_environment, environment, sizeof(g_environment) - 1);

    // Clean up JNI memory hooks immediately
    if (version)     (*env)->ReleaseStringUTFChars(env, jversion,     version);
    if (impl)        (*env)->ReleaseStringUTFChars(env, jimpl,        impl);
    if (environment) (*env)->ReleaseStringUTFChars(env, jenvironment, environment);

    // 3. Format dynamic counters
    char heap_used_str[16], heap_max_str[16];
    format_size(heap_used_str, sizeof(heap_used_str), (size_t)heap_used);
    format_size(heap_max_str,  sizeof(heap_max_str),  (size_t)heap_max);
    
    snprintf(g_memory, sizeof(g_memory), "%s / %s", heap_used_str, heap_max_str);
    snprintf(g_gc_objects, sizeof(g_gc_objects), "%" PRId64, (int64_t)gc_count);
    snprintf(g_threads,    sizeof(g_threads),    "%d", (int)thread_count);

    // 4. Render the fetch UI
    sysinfo_print(jni_putline, env);
}

// JNI method — Embfetch.printInfoCompiled()
JNIEXPORT void JNICALL Java_Embfetch_printInfoCompiled
    (JNIEnv *env, jclass cls,
     jstring jversion, jstring jimpl, jstring jenvironment,
     jlong heap_used, jlong heap_max,
     jlong gc_count, jint thread_count) {

    compiled_mode = 1;
    Java_Embfetch_printInfo(env, cls, jversion, jimpl, jenvironment,
                            heap_used, heap_max, gc_count, thread_count);
    compiled_mode = 0;
}

// JNI_OnLoad — capture VM start time
// Dynamic method registration table
static JNINativeMethod methods[] = {
    {"printInfo",         "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JJJI)V", (void *)&Java_Embfetch_printInfo},
    {"printInfoCompiled", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JJJI)V", (void *)&Java_Embfetch_printInfoCompiled}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    (void)reserved;
    vm_start_time = (double)clock() / CLOCKS_PER_SEC;

    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_8) != JNI_OK) {
        return JNI_ERR;
    }

    // 1. Look for the target class under standard compiled environments
    jclass cls = (*env)->FindClass(env, "Embfetch");
    
    // 2. FALLBACK FOR JSHELL: If Embfetch is not found, look for JShell's wrapped classes
    if (cls == NULL) {
        (*env)->ExceptionClear(env); // Clear the ClassNotFoundException
        
        // We look for any wrapped REPL variations generated by JShell
        // JShell classes usually follow names like REPL.$JShell$1$Embfetch, REPL.$JShell$2$Embfetch, etc.
        // We can scan through a few likely iterations to register our native hooks dynamically!
        char jshell_cls_name[64];
        for (int i = 0; i < 20; i++) {
            snprintf(jshell_cls_name, sizeof(jshell_cls_name), "REPL/$JShell$%d$Embfetch", i);
            cls = (*env)->FindClass(env, jshell_cls_name);
            if (cls != NULL) break;
            (*env)->ExceptionClear(env);
        }
    }

    // Register our native functions safely to whatever classloader container is active
    if (cls != NULL) {
        if ((*env)->RegisterNatives(env, cls, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
            return JNI_ERR;
        }
    }

    return JNI_VERSION_1_8;
}