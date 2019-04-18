#include "Input.h"

#include <GLFW/glfw3.h>

using namespace std;
map<int, Key> Input::Keys;

Key::Key() :IsPress(false), IsPressed(false), IsRelease(false) {}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (Input::Keys.count(key))
	{
		if (action == GLFW_PRESS)
		{
			Input::Keys[key].IsPress = true;
			Input::Keys[key].IsPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			//Input::Keys[key].IsPressed = false;
			Input::Keys[key].IsRelease = true;
		}
	}
}

bool Input::GetKey(int key)
{
	return Keys[key].IsPressed;
}

bool Input::GetKeyDown(int key)
{
	return Keys[key].IsPress;
}

bool Input::GetKeyUp(int key)
{
	return Keys[key].IsRelease;
}

void Input::Reset()
{
	for (auto& value : Keys)
	{
		value.second.IsPress = false;
		if (value.second.IsRelease)
		{
			value.second.IsPressed = false;
			value.second.IsRelease = false;
		}
	}
}

void Input::Initialize()
{
	Input::Keys.insert({ GLFW_KEY_SPACE, Key() });

	Input::Keys.insert({ GLFW_KEY_0, Key() });	
	Input::Keys.insert({ GLFW_KEY_1, Key() });
	Input::Keys.insert({ GLFW_KEY_2, Key() });
	Input::Keys.insert({ GLFW_KEY_3, Key() });
	Input::Keys.insert({ GLFW_KEY_4, Key() });
	Input::Keys.insert({ GLFW_KEY_5, Key() });
	Input::Keys.insert({ GLFW_KEY_6, Key() });
	Input::Keys.insert({ GLFW_KEY_7, Key() });
	Input::Keys.insert({ GLFW_KEY_8, Key() });
	Input::Keys.insert({ GLFW_KEY_9, Key() });

	Input::Keys.insert({ GLFW_KEY_A, Key() });
	Input::Keys.insert({ GLFW_KEY_B, Key() });
	Input::Keys.insert({ GLFW_KEY_C, Key() });
	Input::Keys.insert({ GLFW_KEY_D, Key() });
	Input::Keys.insert({ GLFW_KEY_E, Key() });
	Input::Keys.insert({ GLFW_KEY_F, Key() });
	Input::Keys.insert({ GLFW_KEY_G, Key() });
	Input::Keys.insert({ GLFW_KEY_H, Key() });
	Input::Keys.insert({ GLFW_KEY_I, Key() });
	Input::Keys.insert({ GLFW_KEY_J, Key() });
	Input::Keys.insert({ GLFW_KEY_K, Key() });
	Input::Keys.insert({ GLFW_KEY_L, Key() });
	Input::Keys.insert({ GLFW_KEY_M, Key() });
	Input::Keys.insert({ GLFW_KEY_N, Key() });
	Input::Keys.insert({ GLFW_KEY_O, Key() });
	Input::Keys.insert({ GLFW_KEY_P, Key() });
	Input::Keys.insert({ GLFW_KEY_Q, Key() });
	Input::Keys.insert({ GLFW_KEY_R, Key() });
	Input::Keys.insert({ GLFW_KEY_S, Key() });
	Input::Keys.insert({ GLFW_KEY_T, Key() });
	Input::Keys.insert({ GLFW_KEY_U, Key() });
	Input::Keys.insert({ GLFW_KEY_V, Key() });
	Input::Keys.insert({ GLFW_KEY_W, Key() });
	Input::Keys.insert({ GLFW_KEY_X, Key() });
	Input::Keys.insert({ GLFW_KEY_Y, Key() });
	Input::Keys.insert({ GLFW_KEY_Z, Key() });

	Input::Keys.insert({ GLFW_KEY_ESCAPE, Key() });
}

