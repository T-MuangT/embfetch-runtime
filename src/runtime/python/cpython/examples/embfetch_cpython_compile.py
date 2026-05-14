#!/usr/bin/env python3
# compile_embfetch.py
# Compiles embfetch_compiled.py into embfetch_compiled.pyc.
# The compiled variant reports uptime as "Compiled" since
# precompiled bytecode has no meaningful runtime uptime.
#
# Usage: python compile_embfetch.py

import py_compile
import os
import shutil

SOURCE = "embfetch_compiled.py"
OUTPUT = "embfetch_compiled.pyc"

# Generate the source file with "Compiled" uptime marker
source_code = '''\
#!/usr/bin/env python3
# embfetch_compiled.py
# Precompiled variant of embfetch sample.
# Uptime is reported as "Compiled" since this is a bytecode artifact.

import embfetch

def main():
    embfetch.print_info_compiled()

if __name__ == "__main__":
    main()
'''

# Write the temporary source file
with open(SOURCE, "w") as f:
    f.write(source_code)

# Compile to .pyc
py_compile.compile(SOURCE, cfile=OUTPUT, optimize=0)
print(f"Compiled: {OUTPUT}")

# Clean up source
os.remove(SOURCE)
print(f"Removed temporary: {SOURCE}")