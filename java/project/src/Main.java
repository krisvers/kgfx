import com.krisvers.kgfx.*;
import com.krisvers.kgfxgh.*;
import org.lwjgl.glfw.*;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.ByteBuffer;

public class Main {
	public static byte[] floatArrayToByteArray(float[] input) {
		byte[] ret = new byte[input.length*4];
		for (int x = 0; x < input.length; x++) {
			ByteBuffer.wrap(ret, x*4, 4).putFloat(input[x]);
		}
		return ret;
	}

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

		KGFXwindow kgfxWindow = KGFXGHjni.kgfxWindowFromGLFW(window);
		KGFXcontext context = new KGFXcontext(0, 0, 0, kgfxWindow);

		String source = "struct vinput_t { float2 position : POSITION; }; struct pinput_t { float4 position : SV_POSITION; }; pinput_t vmain(vinput_t input) { pinput_t output; output.position = float4(input.position, 0.0f, 1.0f); return output; } float4 pmain(pinput_t input) : SV_TARGET { return float4(1, 0, 0, 1); }";
		long vshader = KGFXjni.createShader(context.getHandle(), "vmain", source.getBytes(StandardCharsets.UTF_8), 0, 2);
		if (vshader == 0) {
			System.out.println("Failed to create shader");
			context.destroy();
			GLFW.glfwDestroyWindow(window);
			GLFW.glfwTerminate();
			return;
		}

		long fshader = KGFXjni.createShader(context.getHandle(), "pmain", source.getBytes(StandardCharsets.UTF_8), 1, 2);
		if (fshader == 0) {
			System.out.println("Failed to create shader");
			context.destroy();
			GLFW.glfwDestroyWindow(window);
			GLFW.glfwTerminate();
			return;
		}

		KGFXpipelinelayout layout = new KGFXpipelinelayout(new KGFXpipelinebinding[1], new KGFXdescriptorsetdesc[0]);
		layout.bindings[0] = new KGFXpipelinebinding(0, new KGFXpipelineattribute[1], 0, 0);
		layout.bindings[0].attributes[0] = new KGFXpipelineattribute("POSITION", 2, 0);

		long pipeline = KGFXjni.createPipeline(context.getHandle(), new long[] { vshader, fshader }, layout, 0, 0, 0, 0);
		KGFXjni.destroyShader(context.getHandle(), vshader);
		KGFXjni.destroyShader(context.getHandle(), fshader);

		if (pipeline == 0) {
			System.out.println("Failed to create pipeline");
			context.destroy();
			GLFW.glfwDestroyWindow(window);
			GLFW.glfwTerminate();
			return;
		}

		float[] vertices = {
			1, 1,
			1, 0,
			0, 1,
		};

		byte[] bytes = floatArrayToByteArray(vertices);
		long buffer = KGFXjni.createBuffer(context.getHandle(), 0, 1, bytes, bytes.length);
		if (buffer == 0) {
			System.out.println("Failed to create buffer");
			KGFXjni.destroyPipeline(context.getHandle(), pipeline);
			context.destroy();
			GLFW.glfwDestroyWindow(window);
			GLFW.glfwTerminate();
			return;
		}

		long commandList = KGFXjni.createCommandList(context.getHandle());
		if (commandList == 0) {
			System.out.println("Failed to create command list");
			KGFXjni.destroyBuffer(context.getHandle(), buffer);
			KGFXjni.destroyPipeline(context.getHandle(), pipeline);
			context.destroy();
			GLFW.glfwDestroyWindow(window);
			GLFW.glfwTerminate();
			return;
		}

		while (!GLFW.glfwWindowShouldClose(window)) {
			KGFXjni.commandReset(context.getHandle(), commandList);
			KGFXjni.commandBindVertexBuffer(context.getHandle(), commandList, buffer, 0);
			KGFXjni.commandBindPipeline(context.getHandle(), commandList, pipeline);
			KGFXjni.commandDraw(context.getHandle(), commandList, 3, 1, 0, 0);
			KGFXjni.commandPresent(context.getHandle(), commandList);
			KGFXjni.commandListSubmit(context.getHandle(), commandList);
			GLFW.glfwPollEvents();
		}

		KGFXjni.destroyCommandList(context.getHandle(), commandList);
		KGFXjni.destroyBuffer(context.getHandle(), buffer);
		KGFXjni.destroyPipeline(context.getHandle(), pipeline);
		context.destroy();
		GLFW.glfwDestroyWindow(window);
		GLFW.glfwTerminate();
	}
}