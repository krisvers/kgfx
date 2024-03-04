# Python bindings for kgfx
# - krisvers

import ctypes
import sys

KGFX_SUCCESS = 0
KGFX_GENERIC_ERROR = 1
KGFX_VERSION_NOT_SUPPORTED = 2
KGFX_VERSION_MAJOR_MISMATCH = 3
KGFX_NULL_OUTPUT_ARGUMENT = 4
KGFX_INVALID_CONTEXT = 5
KGFX_NOT_IMPLEMENTED = 6
KGFX_INVALID_ARGUMENT = 7
KGFX_SIZE_TOO_LARGE = 8

KGFX_SHADERTYPE_VERTEX = 0
KGFX_SHADERTYPE_FRAGMENT = 1
KGFX_SHADERTYPE_GEOMETRY = 2
KGFX_SHADERTYPE_COMPUTE = 3

KGFX_MEDIUM_SPIRV = 0
KGFX_MEDIUM_GLSL = 1
KGFX_MEDIUM_HLSL = 2
KGFX_MEDIUM_MSL = 3

if sys.platform == 'win32':
    _kgfx = ctypes.cdll.kgfx
elif sys.platform == 'darwin':
    _kgfx = ctypes.cdll.LoadLibrary('libkgfx.dylib')
else:
    _kgfx = ctypes.cdll.LoadLibrary('libkgfx.so')

class KGFXwindowWIN(ctypes.Structure):
	_fields_ = [
		("hwnd", ctypes.c_void_p)
	]

class KGFXwindowXlib(ctypes.Structure):
	_fields_ = [
		("display", ctypes.c_void_p),
		("window", ctypes.c_long),
	]

class KGFXwindowCocoa(ctypes.Structure):
	_fields_ = [
		("window", ctypes.c_void_p),
		("contentView", ctypes.c_void_p),
		("layer", ctypes.c_void_p),
	]

KGFXshader = ctypes.c_void_p

class KGFXshaderdesc(ctypes.Structure):
	_fields_ = [
		("entryName", ctypes.c_char_p),
		("pData", ctypes.c_void_p),
		("size", ctypes.c_uint),
		("type", ctypes.c_uint),
		("medium", ctypes.c_uint),
	]

class KGFXcontext(object):
	handle = ctypes.c_void_p(0)

	def __init__(self, window, major = 0xFF, minor = 0xFFF, patch = 0xFFF):
		result = _kgfx.kgfxCreateContext((major << 24) | (minor << 12) | (patch), window, ctypes.pointer(self.handle))
		if result != KGFX_SUCCESS:
			raise Exception("KGFXcontext __init__ failure")

	def __del__(self):
		_kgfx.kgfxDestroyContext(self.handle)

	def create_shader(self, entry_name: str, data: bytearray, type, medium) -> KGFXshader:
		bentry = str.encode("utf-8")
		pentry = ctypes.c_char_p(0)
		if (entry_name != ""):
			pentry = ctypes.c_char_p(bentry)
		pdata = ctypes.c_char * len(bentry)
		pdata.from_buffer(data)

		desc = KGFXshaderdesc()
		desc.entryName = pentry
		desc.pData = ctypes.c_void_p()
		desc.size = len(bentry)
		desc.type = ctypes.c_uint(type)
		desc.medium = ctypes.c_uint(medium)
		_kgfx.kgfxCreateShader(desc)

