from . import CppBindingGenerator as cbg
import ctypes
import sys

from .common import *

StaticFile = cbg.Class('altseed', 'StaticFile')
with StaticFile as class_:
    with class_.add_func('GetBuffer') as func:
        func.return_type = Int8Array
    with class_.add_func('GetPath') as func:
        func.return_type = ctypes.c_wchar_p
    with class_.add_func('GetSize') as func:
        func.return_type = int
    with class_.add_func('GetIsInPackage') as func:
        func.return_type = bool
    with class_.add_func('Reload') as func:
        func.return_type = bool

StreamFile = cbg.Class('altseed', 'StreamFile')
with StreamFile as class_:
    with class_.add_func('GetSize') as func:
        func.return_type = int
    with class_.add_func('GetCurrentPosition') as func:
        func.return_type = int
    with class_.add_func('Read') as func:
        func.add_arg(int, 'size')
        func.return_type = int
    with class_.add_func('GetTempBuffer') as func:
        func.return_type = Int8Array
    with class_.add_func('GetTempBufferSize') as func:
        func.return_type = int
    with class_.add_func('GetIsInPackage') as func:
        func.return_type = bool
    with class_.add_func('Reload') as func:
        func.return_type = bool

File = cbg.Class('altseed', 'File')
with File as class_:
    with class_.add_func('GetInstance') as func:
        func.return_type = File
    with class_.add_func('CreateStaticFile') as func:
        func.add_arg(ctypes.c_wchar_p, 'path')
        func.return_type = StaticFile
    with class_.add_func('CreateStreamFile') as func:
        func.add_arg(ctypes.c_wchar_p, 'path')
        func.return_type = StreamFile
    with class_.add_func('AddRootDirectory') as func:
        func.add_arg(ctypes.c_wchar_p, 'path')
        func.return_type = bool
    with class_.add_func('AddRootPackageWithPassword') as func:
        func.add_arg(ctypes.c_wchar_p, 'path')
        func.add_arg(ctypes.c_wchar_p, 'password')
        func.return_type = bool
    with class_.add_func('AddRootPackage') as func:
        func.add_arg(ctypes.c_wchar_p, 'path')
        func.return_type = bool
    class_.add_func('ClearRootDirectories')
    with class_.add_func('Exists') as func:
        func.add_arg(ctypes.c_wchar_p, 'path')
        func.return_type = bool
    with class_.add_func('Pack') as func:
        func.add_arg(ctypes.c_wchar_p, 'srcPath')
        func.add_arg(ctypes.c_wchar_p, 'dstPath')
        func.return_type = bool
    with class_.add_func('Pack') as func:
        func.add_arg(ctypes.c_wchar_p, 'srcPath')
        func.add_arg(ctypes.c_wchar_p, 'dstPath')
        func.add_arg(ctypes.c_wchar_p, 'password')
        func.return_type = bool
        