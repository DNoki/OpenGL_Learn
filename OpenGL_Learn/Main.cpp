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

void Trace(const char* format, ...)
{
    char buffer[256];
    va_list ap;
    va_start(ap, format);
    vsprintf_s(buffer, 256, format, ap);
    va_end(ap);
    cout << buffer << endl;
}
void OutputMatrix4x4(const Matrix4x4& m)
{
    //cout << m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << m[0][3] << "\t" << endl;
    //cout << m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << m[1][3] << "\t" << endl;
    //cout << m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\t" << m[2][3] << "\t" << endl;
    //cout << m[3][0] << "\t" << m[3][1] << "\t" << m[3][2] << "\t" << m[3][3] << "\t" << endl;
    Trace("%.2f\t%.2f\t%.2f\t%.2f", m[0][0], m[0][1], m[0][2], m[0][3]);
    Trace("%.2f\t%.2f\t%.2f\t%.2f", m[1][0], m[1][1], m[1][2], m[1][3]);
    Trace("%.2f\t%.2f\t%.2f\t%.2f", m[2][0], m[2][1], m[2][2], m[2][3]);
    Trace("%.2f\t%.2f\t%.2f\t%.2f", m[3][0], m[3][1], m[3][2], m[3][3]);
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