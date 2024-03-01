from pyglfw import pyglfw
import sys
import ctypes

if sys.platform == 'win32':
    _kgfx_gh = ctypes.cdll.kgfx_gh
elif sys.platform == 'darwin':
    _kgfx_gh = ctypes.cdll.LoadLibrary('libkgfx_gh.dylib')
else:
    _kgfx_gh = ctypes.cdll.LoadLibrary('libkgfx_gh.so')

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


if sys.platform == 'win32':
    def kgfx_window_from_glfw(window) -> KGFXwindowWIN:
        _kgfx_window = _kgfx_gh.kgfxWindowFromGLFW(window)
        kgfx_window = KGFXwindowWIN()
        kgfx_window.hwnd = _kgfx_window.hwnd
        return kgfx_window
elif sys.platform == 'darwin':
    def kgfx_window_from_glfw(window) -> KGFXwindowCocoa:
        _kgfx_window = _kgfx_gh.kgfxWindowFromGLFW(window)
        kgfx_window = KGFXwindowCocoa()
        kgfx_window.window = _kgfx_window.window
        kgfx_window.contentView = _kgfx_window.contentView
        kgfx_window.layer = _kgfx_window.layer
        return kgfx_window
else:
    def kgfx_window_from_glfw(window) -> KGFXwindowXlib:
        _kgfx_window = _kgfx_gh.kgfxWindowFromGLFW(window)
        kgfx_window = KGFXwindowXlib()
        kgfx_window.window = _kgfx_window.window
        kgfx_window.display = _kgfx_window.display
        return kgfx_window
