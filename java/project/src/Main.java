import com.krisvers.kgfx.*;
import com.krisvers.kgfxgh.*;
import org.lwjgl.glfw.*;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.ByteBuffer;

public class Main {
	public static byte[] floatArrayToByteArray(float[] input) {
		ByteBuffer buffer = ByteBuffer.allocate(input.length * Float.BYTES);
		for (int x = 0; x < input.length; x++) {
			buffer.putFloat(input[x]);
		}
		return buffer.array();
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
		KGFXshader vshader = context.createShader("vmain", source, 0, 2);
		KGFXshader fshader = context.createShader("pmain", source, 1, 2);

		KGFXpipelinelayout layout = new KGFXpipelinelayout(new KGFXpipelinebinding[1], new KGFXdescriptorsetdesc[0]);
		layout.bindings[0] = new KGFXpipelinebinding(0, new KGFXpipelineattribute[1], 0, 0);
		layout.bindings[0].attributes[0] = new KGFXpipelineattribute("POSITION", 2, 0);

		KGFXpipeline pipeline = context.createPipeline(new KGFXshader[] {vshader, fshader}, layout, 0, 0, 0, 2);
		vshader.destroy();
		fshader.destroy();

		float[] vertices = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.0f, 0.5f
		};

		KGFXbuffer buffer = context.createBuffer(1, 1, vertices, vertices.length * Float.BYTES);
		KGFXcommandlist commandList = context.createCommandList();

		while (!GLFW.glfwWindowShouldClose(window)) {
			commandList.reset();
			commandList.bindVertexBuffer(buffer, 0);
			commandList.bindPipeline(pipeline);
			commandList.draw(3, 1, 0, 0);
			commandList.present();
			commandList.submit();
			GLFW.glfwPollEvents();
		}

		commandList.destroy();
		buffer.destroy();
		pipeline.destroy();
		context.destroy();
		GLFW.glfwDestroyWindow(window);
		GLFW.glfwTerminate();
	}
}