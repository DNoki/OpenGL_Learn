#pragma once

#include <map>
#include <glad/glad.h>

#include "Time.h"

struct GLFWwindow;

struct Key
{
public:
	bool IsPress;
	bool IsPressed;
	bool IsRelease;

	Key();
};

class Input
{
public:
	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);

	static void Reset();
	static void Initialize();

private:
	static std::map<int, Key> Keys;
	friend void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

enum KeyCode
{
	SPACE = 32,
	KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	ESCAPE = 256,
};