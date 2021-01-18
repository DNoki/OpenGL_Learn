#include <Windows.h>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "../OpenGL_Core/GameSystem.h"
#include "ProjectSetting.h"

using namespace std;
using namespace OpenGL_Core;
using namespace OpenGL_Learn;

void TestFunction(bool (*func_ptr)(const int&))
{
    cout << (*func_ptr)(20) << endl;
}

extern void TestBullet();

int main()
{
    SetConsoleOutputCP(65001);

    //TestFunction([](const int& a) ->bool { return true; });
    //TestBullet();
    //system("pause");
    //return 0;

    ProjectSetting::LoadSetting();
    GameSystem::LifeCycle();

    //system("pause");

    return 0;
}