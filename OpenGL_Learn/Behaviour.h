#pragma once

#include "Component.h"

#include "SceneManager.h"
#include "GameSystem.h"
#include "GameObject.h"

#include "Time.h"


namespace OpenGL_Learn
{
    using namespace std;

    // ��Ϊ�ǿ������û���õ����
    class Behaviour : public Component
    {
    public:
        // �Ƿ񼤻�
        bool Enabled;
        // ����Ϊ�Ƿ��Ѽ�������ã�
        inline bool GetIsActiveAndEnabled()
        {
            return Enabled && GetGameObject().GetActive();
        }

        // �����ҵ��ĵ�һ�����
        template <typename T = Component>
        T * GetComponent() const
        {
            return GetGameObject().GetComponent<T>();
        }
        // �����ҵ������
        template <typename T = Component>
        List<T*> GetComponents() const
        {
            return GetGameObject().GetComponents<T>();
        }

        Behaviour(GameObject& obj) :Component(obj), Enabled(true) {}
    };
}

