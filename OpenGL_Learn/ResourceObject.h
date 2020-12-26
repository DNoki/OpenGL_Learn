#pragma once

#include <string>

#include "List.h"
#include "Tuple.h"

namespace OpenGL_Learn
{
    using namespace std;

    enum class HideFlagType
    {
        NORMAL = 0,
        STATIC = 1,
    };

    // 资源对象的基类
    class ResourceObject
    {
    public:
        // 对象的名称。
        string Name;
        HideFlagType HideFlag;

        static void Destroy(ResourceObject& obj);
        static void DestroyImmediate(ResourceObject& _object);
        static void DontDestroyOnLoad();
        //template <typename T = ResourceObject>
        //static T Instantiate(const T & obj)
        //{

        //}

        virtual ~ResourceObject() {}

    protected:
        ResourceObject(const string& name) :Name(name), HideFlag(HideFlagType::NORMAL) {}

    private:

    };
}