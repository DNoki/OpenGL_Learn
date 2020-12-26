#include "ResourceObject.h"

#include "SceneManager.h"
#include "RenderTexture.h"

namespace OpenGL_Learn
{
    void ResourceObject::Destroy(ResourceObject& _object)
    {
        auto& destroyList = SceneManager::GetActiveScene().GetDestroyedObjects();

        auto gameObject = dynamic_cast<GameObject*>(&_object);
        if (gameObject)
        {
            auto& Objects = SceneManager::GetActiveScene().GetAllGameObjects();
            auto uniqueptr = Objects.Find([&](const unique_ptr<GameObject>& o) { return o.get() == &_object; });
            if (!uniqueptr) return; // ������û�ҵ��������
            destroyList.push_back(move(*uniqueptr)); // ��ӵ����ͷ��б�
            Objects.Remove(*uniqueptr);
            return;
        }

        {
            auto& Objects = SceneManager::GetActiveScene().GetAllResourceObjects();
            for (auto& pairObjs : Objects)
            {
                auto uniqueptr = pairObjs.second.Find([&](const unique_ptr<ResourceObject>& o) { return o.get() == &_object; });
                if (uniqueptr) 
                {
                    destroyList.push_back(move(*uniqueptr)); // ��ӵ����ͷ��б�
                    pairObjs.second.Remove(*uniqueptr);
                    return;
                }
            }
        }
    }
    void ResourceObject::DestroyImmediate(ResourceObject& _object)
    {
        auto gameObject = dynamic_cast<GameObject*>(&_object);
        if (gameObject)
        {
            auto& Objects = SceneManager::GetActiveScene().GetAllGameObjects();
            auto uniqueptr = Objects.Find([&](const unique_ptr<GameObject>& o) { return o.get() == &_object; });
            if (!uniqueptr) return; // ������û�ҵ��������
            Objects.Remove(*uniqueptr);
            return;
        }

        {
            auto& Objects = SceneManager::GetActiveScene().GetAllResourceObjects();
            for (auto& pairObjs : Objects)
            {
                auto uniqueptr = pairObjs.second.Find([&](const unique_ptr<ResourceObject>& o) { return o.get() == &_object; });
                if (uniqueptr)
                {
                    pairObjs.second.Remove(*uniqueptr);
                    return;
                }
            }
        }
    }
    void ResourceObject::DontDestroyOnLoad()
    {
    }
    //void ResourceObject::Instantiate(ResourceObject& obj)
    //{

    //}
}