from setuptools import setup, Extension
import os

# Source files relative to this setup.py
# Assumes setup.py lives in src/runtime/python/cpython/
SRC_ROOT = os.path.join(os.path.dirname(__file__), '..', '..', '..', '..')

embfetch_ext = Extension(
    name='embfetch',
    sources=[
        'sysinfo_cpython.c',
        os.path.join('..', 'sysinfo_python_common.c'),
        os.path.join(SRC_ROOT, 'sysinfo_runtime_common.c'),
    ],
    include_dirs=[
        '.',                                          # logo.h
        os.path.join('..'),                           # sysinfo_python_common.c
        os.path.join(SRC_ROOT, 'include'),            # runtime_sysinfo.h
    ],
    extra_compile_args=['-Wall'],
)

setup(
    name='embfetch',
    version='0.1.0',
    description='Neofetch-style system info display for Python runtimes',
    ext_modules=[embfetch_ext],
    py_modules=['embfetch_sample'],
)