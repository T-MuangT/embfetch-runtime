#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__wasi__)
// WebAssembly WASI — no rusage support
#elif defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#else
// Unknown platform — memory will report Unknown
#endif

#include "runtime_sysinfo.h"

// VM start time — captured at module init
static double vm_start_time = 0.0;

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%.1fMB", (double)bytes / (1024 * 1024));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%.1fKB", (double)bytes / 1024);
    } else {
        snprintf(dst, len, "%zuB", bytes);
    }
}

// Static info — implementation, version, environment
void sysinfo_fetch_static(sysinfo_runtime_static_t *dst) {
    // Implementation name via sys.implementation.name
    PyObject *impl = PySys_GetObject("implementation");
    if (impl) {
        PyObject *name = PyObject_GetAttrString(impl, "name");
        if (name && PyUnicode_Check(name)) {
            dst->implementation = PyUnicode_AsUTF8(name);
        } else {
            dst->implementation = "CPython";
        }
        Py_XDECREF(name);
    } else {
        dst->implementation = "CPython";
    }

    // Version string via sys.version
    PyObject *ver = PySys_GetObject("version");
    if (ver && PyUnicode_Check(ver)) {
        // Trim to first line only — sys.version can be multiline
        const char *full = PyUnicode_AsUTF8(ver);
        static char ver_buf[64];
        strncpy(ver_buf, full, sizeof(ver_buf) - 1);
        char *nl = strchr(ver_buf, '\n');
        if (nl) *nl = '\0';
        dst->version = ver_buf;
    } else {
        dst->version = "Unknown";
    }

    // Environment — venv vs global vs conda
    PyObject *prefix     = PySys_GetObject("prefix");
    PyObject *base_prefix = PySys_GetObject("base_prefix");
    if (prefix && base_prefix) {
        PyObject *eq = PyObject_RichCompare(prefix, base_prefix, Py_EQ);
        int is_global = PyObject_IsTrue(eq);
        Py_XDECREF(eq);

        // Check for conda
        PyObject *conda = PySys_GetObject("conda_prefix");
        if (conda && conda != Py_None) {
            dst->environment = "conda";
        } else {
            dst->environment = is_global ? "Global" : "venv";
        }
    } else {
        dst->environment = "Unknown";
    }

    dst->build_date = __DATE__ " " __TIME__;
}

// Dynamic info — uptime, threads, GC, memory
void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst) {
    // Uptime from vm_start_time captured at module init
    double now = (double)clock() / CLOCKS_PER_SEC;
    double uptime_s = now - vm_start_time;
    dst->uptime_m = (uint32_t)(uptime_s / 60);
    dst->uptime_s = (uint32_t)((uint64_t)uptime_s % 60);

    // Active thread count via threading.active_count()
    PyObject *threading = PyImport_ImportModule("threading");
    if (threading) {
        PyObject *count = PyObject_CallMethod(threading, "active_count", NULL);
        if (count && PyLong_Check(count)) {
            snprintf(dst->threads, sizeof(dst->threads), "%ld", PyLong_AsLong(count));
        } else {
            snprintf(dst->threads, sizeof(dst->threads), "Unknown");
        }
        Py_XDECREF(count);
        Py_DECREF(threading);
    } else {
        snprintf(dst->threads, sizeof(dst->threads), "Unknown");
    }

    // GC object count via gc.get_objects()
    PyObject *gc = PyImport_ImportModule("gc");
    if (gc) {
        PyObject *objects = PyObject_CallMethod(gc, "get_objects", NULL);
        if (objects && PyList_Check(objects)) {
            snprintf(dst->gc_objects, sizeof(dst->gc_objects),
                     "%zd", PyList_Size(objects));
        } else {
            snprintf(dst->gc_objects, sizeof(dst->gc_objects), "Unknown");
        }
        Py_XDECREF(objects);
        Py_DECREF(gc);
    } else {
        snprintf(dst->gc_objects, sizeof(dst->gc_objects), "Unknown");
    }

    // Memory — RSS via platform-specific API
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        format_size(dst->memory, sizeof(dst->memory), pmc.WorkingSetSize);
    } else {
        snprintf(dst->memory, sizeof(dst->memory), "Unknown");
    }
#elif defined(__wasi__)
    // WASI — no memory usage API available
    snprintf(dst->memory, sizeof(dst->memory), "Unknown");
#elif defined(__unix__) || defined(__APPLE__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        // ru_maxrss is in bytes on macOS, KB on Linux and other POSIX
#if defined(__APPLE__)
        format_size(dst->memory, sizeof(dst->memory), (size_t)usage.ru_maxrss);
#else
        format_size(dst->memory, sizeof(dst->memory), (size_t)usage.ru_maxrss * 1024);
#endif
    } else {
        snprintf(dst->memory, sizeof(dst->memory), "Unknown");
    }
#else
    // Unknown platform
    snprintf(dst->memory, sizeof(dst->memory), "Unknown");
#endif
}

// Global compiled mode flag — overrides uptime with "Compiled"
static int embfetch_compiled_mode = 0;

// Getter used by sysinfo_python_common.c to check compiled mode
int embfetch_is_compiled_mode(void) {
    return embfetch_compiled_mode;
}

// Python-callable print_info() — normal mode
static PyObject *embfetch_print_info(PyObject *self, PyObject *args) {
    (void)self;
    (void)args;
    embfetch_compiled_mode = 0;
    sysinfo_print_file(stdout);
    Py_RETURN_NONE;
}

// Python-callable print_info_compiled() — compiled mode, uptime shows "Compiled"
static PyObject *embfetch_print_info_compiled(PyObject *self, PyObject *args) {
    (void)self;
    (void)args;
    embfetch_compiled_mode = 1;
    sysinfo_print_file(stdout);
    embfetch_compiled_mode = 0;
    Py_RETURN_NONE;
}

// Method table
static PyMethodDef embfetch_methods[] = {
    {"print_info", embfetch_print_info, METH_NOARGS,
     "Print system information in embfetch style."},
    {"print_info_compiled", embfetch_print_info_compiled, METH_NOARGS,
     "Print system information with uptime shown as 'Compiled'."},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef embfetch_module = {
    PyModuleDef_HEAD_INIT,
    "embfetch",
    "Embfetch runtime sysinfo for CPython.",
    -1,
    embfetch_methods
};

// Module init — captures VM start time
PyMODINIT_FUNC PyInit_embfetch(void) {
    vm_start_time = (double)clock() / CLOCKS_PER_SEC;
    return PyModule_Create(&embfetch_module);
} // sysinfo_cpython.c