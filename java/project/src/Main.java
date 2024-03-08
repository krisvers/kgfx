import com.krisvers.kgfx.*;
import com.krisvers.kgfxgh.*;
import org.lwjgl.glfw.*;

import math.*;

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

		KGFXwindow kgfxWindow = KGFXGHjni.kgfxWindowFromGLFW(window);
		KGFXcontext context = new KGFXcontext(0, 0, 0, kgfxWindow);

		String source =
			"struct vinput_t { float2 position : POSITION; };\n" +
			"struct pinput_t { float4 position : SV_POSITION; };\n" +
			"cbuffer ubo_t : register(b0) { float4x4 mvp; };\n" +
			"pinput_t vmain(vinput_t input) {\n" +
				"pinput_t output;\n" +
				"output.position = mul(float4(input.position, 0.0f, 1.0f), mvp);\n" +
				"return output;\n" +
			"}\n" +
			"float4 pmain(pinput_t input) : SV_TARGET { return float4(1, 0, 0, 1); }";

		KGFXshader vshader = context.createShader(
			"vmain", source,
			KGFXjni.KGFX_SHADERTYPE_VERTEX, KGFXjni.KGFX_MEDIUM_HLSL
		);

		KGFXshader fshader = context.createShader(
			"pmain", source,
			KGFXjni.KGFX_SHADERTYPE_FRAGMENT, KGFXjni.KGFX_MEDIUM_HLSL
		);

		KGFXpipelinelayout layout = new KGFXpipelinelayout(
			new KGFXpipelinebinding[1], new KGFXdescriptorsetdesc[1]
		);

		layout.bindings[0] = new KGFXpipelinebinding(
			0, new KGFXpipelineattribute[1],
			KGFXjni.KGFX_BINDPOINT_VERTEX, 0
		);

		layout.bindings[0].attributes[0] = new KGFXpipelineattribute(
			"POSITION", KGFXjni.KGFX_DATATYPE_FLOAT2, 0
		);

		layout.descriptorSets[0] = new KGFXdescriptorsetdesc(
			KGFXjni.KGFX_BINDPOINT_VERTEX, 0,
			KGFXjni.KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER, Matrix.bytesize(4, 4)
		);

		KGFXpipeline pipeline = context.createPipeline(
			new KGFXshader[] {vshader, fshader}, layout,
			KGFXjni.KGFX_CULLMODE_NONE, KGFXjni.KGFX_FRONTFACE_CCW,
			KGFXjni.KGFX_FILLMODE_SOLID, KGFXjni.KGFX_TOPOLOGY_TRIANGLES
		);

		vshader.destroy();
		fshader.destroy();

		float[] vertices = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.0f, 0.5f
		};

		KGFXbuffer buffer = context.createBuffer(
			KGFXjni.KGFX_BUFFER_LOCATION_GPU, KGFXjni.KGFX_BUFFER_USAGE_VERTEX_BUFFER,
			vertices, vertices.length * Float.BYTES
		);
		KGFXcommandlist commandList = context.createCommandList();

		Matrix mvp = new Matrix(4, 4);

		KGFXbuffer uniformBuffer = context.createBuffer(
			KGFXjni.KGFX_BUFFER_LOCATION_CPU, KGFXjni.KGFX_BUFFER_USAGE_UNIFORM_BUFFER,
			mvp.toFloatArray(), mvp.bytesize()
		);

		while (!GLFW.glfwWindowShouldClose(window)) {
			mvp.identity();
			mvp = mvp.rotate(0, 0, (float) GLFW.glfwGetTime());

			KGFXjni.writeBufferFloats(
				context.getHandle(), uniformBuffer.getHandle(),
				mvp.toFloatArray()
			);

			commandList.reset();
			commandList.bindPipeline(pipeline);
			commandList.bindVertexBuffer(buffer, 0);
			commandList.bindDescriptorSetBuffer(uniformBuffer, 0);
			commandList.draw(
				3, 1,
				0, 0);
			commandList.present();
			commandList.submit();

			GLFW.glfwPollEvents();
		}

		commandList.destroy();
		uniformBuffer.destroy();
		buffer.destroy();
		pipeline.destroy();
		context.destroy();
		GLFW.glfwDestroyWindow(window);
		GLFW.glfwTerminate();
	}
}