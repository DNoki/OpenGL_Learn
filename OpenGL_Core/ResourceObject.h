#pragma once

#include <string>

#include "List.hpp"
#include "Tuple.hpp"

namespace OpenGL_Core
{
    using namespace std;

    enum class HideFlagType
    {
        NORMAL = 0,
        STATIC = 1,
    };

    /// <summary>
    /// 资源对象的基类
    /// </summary>
    class ResourceObject
    {
    public:
        /// <summary>
        /// 对象的名称
        /// </summary>
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