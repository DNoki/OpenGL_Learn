#pragma once

#include <iostream>

#define DebugP(text) (std::cout << text << std::endl)
// ��Դ���·��
#define ASSET_PATH(path) ((string("../Asset/") + string(path)).c_str())