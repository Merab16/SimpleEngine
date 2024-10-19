#include "Renderer_OpenGL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "SimpleEngineCore/Log.h"

namespace SimpleEngine {

	bool Renderer_OpenGL::Init(GLFWwindow* window) {
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			LOG_CRITICAL("Failed to initialize GLAD");
			return false;
		}

		LOG_INFO("OpenGL context initialized: ");
		LOG_INFO("\tVendor: {0}", GetVendorStr());
		LOG_INFO("\tRenderer: {0}", GetRendererStr());
		LOG_INFO("\tVersion: {0}", GetVersionStr());
		return true;
	}

	void Renderer_OpenGL::Draw(const VertexArray& vertex_array) {
		vertex_array.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.GetIndicesCount()),
			GL_UNSIGNED_INT, nullptr);
	}

	void Renderer_OpenGL::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void Renderer_OpenGL::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer_OpenGL::SetViewport(unsigned int width, unsigned int height,
		unsigned int left_offset, unsigned int bottom_offset) {
		glViewport(left_offset, bottom_offset, width, height);
	}

	const char* Renderer_OpenGL::GetVendorStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	}

	const char* Renderer_OpenGL::GetRendererStr() {
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}

	const char* Renderer_OpenGL::GetVersionStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}


}