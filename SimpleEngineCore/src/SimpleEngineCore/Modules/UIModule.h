#pragma once


struct GLFWwindow;

namespace SimpleEngine {

	class UIModule {
	private:


	private:


	public:
		static void OnWindowCreate(GLFWwindow* window);
		static void OnWindowClose();
		static void OnUIDrawBegin();
		static void OnUIDrawEnd();
		static void ShowExampleAppDockSpace(bool* p_open);
	};




}