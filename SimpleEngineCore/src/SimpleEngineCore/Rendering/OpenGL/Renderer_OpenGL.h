#pragma once

struct GLFWwindow;

namespace SimpleEngine {

	class VertexArray;

	class Renderer_OpenGL {
	private:



	private:



	public:
		static bool Init(GLFWwindow* window);

		static void Draw(const VertexArray& vertex_array);
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetViewport(unsigned int width, unsigned int height,
			unsigned int left_offset = 0, unsigned int bottom_offset = 0);

		static const char* GetVendorStr();
		static const char* GetRendererStr();
		static const char* GetVersionStr();

	};



}