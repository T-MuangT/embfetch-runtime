#include "embfetch_config.h"

#ifndef EMBFETCH_PORT_H
#define EMBFETCH_PORT_H

#if defined(EMBFETCH_CPYTHON)
    void embfetch_print_info(void);
    void embfetch_print_info_compiled(void);
#elif defined(EMBFETCH_MICROPYTHON)
    // Exposed as MicroPython native module — no C function declaration needed
    // Import via: import embfetch; embfetch.print_info()
#elif defined(EMBFETCH_CIRCUITPYTHON)
    // Exposed as CircuitPython built-in — no C function declaration needed
    // Import via: import embfetch; embfetch.print_info()
#elif defined(EMBFETCH_OPENJDK) || defined(EMBFETCH_ORACLE_JDK)
    // Exposed via JNI — declarations in Embfetch.java
    // No C header needed for Java side
#elif defined(EMBFETCH_DOTNET_CORE) || defined(EMBFETCH_DOTNET_FRAMEWORK) || defined(EMBFETCH_MONO)
    // Exposed via P/Invoke — declarations in Embfetch.cs
    // No C header needed for .NET side
#elif defined(EMBFETCH_NODEJS) || defined(EMBFETCH_V8)
    // Placeholder — not yet implemented
    #warning "Node.js/V8 port is not yet implemented"
#else
    #error "No platform defined."
#endif

#endif /* EMBFETCH_PORT_H */