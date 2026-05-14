# Embfetch Runtime

Embfetch Runtime is a C library for displaying a system information in summary along with an ASCII logo, akin to Neofetch, but for runtime environments of high-level languages such as Python, Java, and .NET.

## Installation

Depending on which system you may have, the library may be installed in various real-time operating systems.
Currently, the supported platforms are:

### Python
Python is one of the supported languages, with the supported runtime being CPython.
#### CPython
1. Setup a Python virtual environment and activate the virtual environment.
2. `cd src/runtime/python/cpython`.
3. To build with `setup.py`, use `python setup.py build_ext --inplace`. To build with `pyproject.toml`, use `pip install .`
4. `python -c "import embfetch"`.
5. After import, the C extension is available under `embfetch.print_info()` within Python REPL.
6. To compile into Python Bytecode, use `python embfetch_cpython_compile.py`.
7. After compiling into Bytecode, the `embfetch_compiled.pyc` will be available.