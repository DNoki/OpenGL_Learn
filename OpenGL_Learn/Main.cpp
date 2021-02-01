#include "pch.h"

#include <Windows.h>

#include "ProjectSetting.h"

using namespace std;
using namespace OpenGL_Core;
using namespace OpenGL_Learn;

void TestFunction(bool (*func_ptr)(const int&))
{
    cout << (*func_ptr)(20) << endl;

}

int main()
{
    SetConsoleOutputCP(65001);

    //TestFunction([](const int& a) ->bool { return true; });
    //system("pause");
    //return 0;

    ProjectSetting::LoadSetting();
    GameSystem::LifeCycle();

    //system("pause");

    return 0;
}