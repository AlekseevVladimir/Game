#pragma once
#define GLFW_INCLUDE_NONE


class GLFWwindow;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double x, double y);

class WindowCtrl {
public:
	static WindowCtrl& getInstance() {
		static WindowCtrl inst;
		return inst;
	}

	GLFWwindow* getWindow() {
		return m_window;
	}

	void init();

	bool isValid() const {
		return m_isValid;
	}

	void restoreWindow()
	{
		setWindowDimensions(m_width, m_height);
	}

	void setWindowDimensions(int width, int height);

	void draw();

	void onKeyPressed(int key, int scancode) {
		//m_transformer.lock()->processInput(key, scancode);
	}

	void onMousePosChanged(double x, double y);

private:
	GLFWwindow* m_window;

	float m_alpha = 0.2f;

	int m_width = 0;

	int m_height = 0;

	mutable bool m_isValid = false;

	void processInput();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
