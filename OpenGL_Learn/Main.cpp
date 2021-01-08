#include <Windows.h>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "Math.h"

using namespace std;

#include "../OpenGL_Core/GameSystem.h"

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

    GameSystem::LifeCycle();

    //system("pause");

    return 0;
}