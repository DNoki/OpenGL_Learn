#pragma once

#include <iostream>

#define DebugP(text) (std::cout << text << std::endl)
// 资源相对路径
#define ASSET_PATH(path) ((string("../Asset/") + string(path)).c_str())