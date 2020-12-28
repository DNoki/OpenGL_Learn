#pragma once

#include <Windows.h>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "Math.h"

using namespace std;

#include "GameSystem.h"

using namespace OpenGL_Learn;

class Father
{
public:
    virtual void DoFun()
    {
        cout << "Father" << endl;
    }

private:

};

class Child : public Father
{
public:
    void DoFun() override
    {
        cout << "Child" << endl;
    }

    ~Child()
    {
        cout << "Child Destruct" << endl;
    }

private:

};

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