import kgfx
from pyglfw import pyglfw

import kgfx_gh

def main():
	if pyglfw.init() != True:
		print("glfw init error")
		return
	
	pyglfw.Window.hint(client_api = 0)
	glfw_window = pyglfw.Window(800, 600, "pytriangle")

	window = kgfx_gh.kgfx_window_from_glfw(glfw_window.handle)
	ctx = kgfx.KGFXcontext(window)

	shader = ctx.create_shader("main", bytearray(b"#version 450\nvoid main() {}"), kgfx.KGFX_SHADERTYPE_VERTEX, kgfx.KGFX_MEDIUM_GLSL)

	while (not glfw_window.should_close):
		
		pyglfw.poll_events()

	glfw_window.close()
	pyglfw.terminate()

if __name__ == "__main__":
	main()
