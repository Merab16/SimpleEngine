#pragma once


struct GLFWwindow;

namespace SimpleEngine {

	class UIModule {
	private:


	private:


	public:
		static void OnWindowCreate(GLFWwindow* window);
		static void OnWindowClose();
		static void OnWindowUpdateBegin();
		static void OnWindowUpdateDraw();
		static void ShowExampleAppDockSpace(bool* p_open);
	};




}