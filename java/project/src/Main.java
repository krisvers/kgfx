import com.krisvers.kgfx.*;
import org.lwjgl.glfw.*;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

public class Main {
	public static void main(String[] args) {
		System.out.println("Hello world!");

		if (!GLFW.glfwInit()) {
			System.out.println("Failed to initialize GLFW");
			return;
		}

		GLFW.glfwWindowHint(GLFW.GLFW_CLIENT_API, GLFW.GLFW_NO_API);
		long window = GLFW.glfwCreateWindow(800, 600, "Hello World", 0, 0);
		if (window == 0) {
			System.out.println("Failed to create window");
			return;
		}

		KGFXcontext context = new KGFXcontext(0, 0, 0, new KGFXwindow(window));

		String source = "struct vinput_t { float3 position : POSITION; }; struct pinput_t { float4 position : SV_POSITION; }; pinput_t vmain(vinput_t input) { pinput_t output; output.position = float4(input.position, 1.0f); return output; } float4 pmain(pinput_t input) : SV_TARGET { return float4(1, 0, 0, 1); }";
		long vshader = KGFXjni.createShader(context.getHandle(), "vmain", source.getBytes(StandardCharsets.UTF_8), 0, 2);
		if (vshader == 0) {
			System.out.println("Failed to create shader");
			return;
		}

		long fshader = KGFXjni.createShader(context.getHandle(), "pmain", source.getBytes(StandardCharsets.UTF_8), 1, 2);
		if (fshader == 0) {
			System.out.println("Failed to create shader");
			return;
		}



		while (!GLFW.glfwWindowShouldClose(window)) {
			GLFW.glfwPollEvents();
		}

		KGFXjni.destroyShader(context.getHandle(), vshader);
		context.destroy();
		GLFW.glfwDestroyWindow(window);
		GLFW.glfwTerminate();
	}
}