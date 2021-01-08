#include "SceneManager.h"

#include "Texture.h"
#include "SampleScene.h"
#include "PointLightScene.h"



namespace OpenGL_Learn
{
    Scene* SceneManager::_activeScene = nullptr;
    List<unique_ptr<Scene>> SceneManager::_scenes = List<unique_ptr<Scene>>();

    void SceneManager::Initialize()
    {
        _scenes.push_back(unique_ptr<Scene>(new SampleScene("Test Scene")));
        //_scenes.push_back(unique_ptr<Scene>(new PointLightScene("Test Scene")));

        _activeScene = _scenes[0].get(); // 第一个要加载的场景

        {
            auto pixels = vector<Tuple4<char, char, char, char>>(16 * 16, Tuple4<char, char, char, char>((char)0xFF, (char)0xFF, (char)0xFF, (char)0xFF));
            _activeScene->AddResourceObject(Texture2D::CreateTexture2D("Default White Texture", 16, 16, GL_RGBA, FormatType::RGBA, TextureType::UNSIGNED_BYTE, &pixels[0]));
            pixels = vector<Tuple4<char, char, char, char>>(16 * 16, Tuple4<char, char, char, char>((char)0x7F, (char)0x7F, (char)0x7F, (char)0xFF));
            _activeScene->AddResourceObject(Texture2D::CreateTexture2D("Default Gray Texture", 16, 16, GL_RGBA, FormatType::RGBA, TextureType::UNSIGNED_BYTE, &pixels[0]));
            pixels = vector<Tuple4<char, char, char, char>>(16 * 16, Tuple4<char, char, char, char>((char)0x00, (char)0x00, (char)0x00, (char)0xFF));
            _activeScene->AddResourceObject(Texture2D::CreateTexture2D("Default Black Texture", 16, 16, GL_RGBA, FormatType::RGBA, TextureType::UNSIGNED_BYTE, &pixels[0]));
            pixels = vector<Tuple4<char, char, char, char>>(16 * 16, Tuple4<char, char, char, char>((char)0x7F, (char)0x7F, (char)0xFF, (char)0xFF));
            _activeScene->AddResourceObject(Texture2D::CreateTexture2D("Default Normal Texture", 16, 16, GL_RGBA, FormatType::RGBA, TextureType::UNSIGNED_BYTE, &pixels[0]));
            
            pixels= vector<Tuple4<char, char, char, char>>(16 * 16, Tuple4<char, char, char, char>((char)0xFF, (char)0xFF, (char)0xFF, (char)0xFF));
            _activeScene->AddResourceObject(TextureCube::CreateTextureCube("Default White TextureCube", 16, 16, GL_RGBA, FormatType::RGBA, TextureType::UNSIGNED_BYTE, &pixels[0]));
        }

        _activeScene->Initialize();
    }

    void SceneManager::LoadNextScene(unsigned int index)
    {
        if (index >= _scenes.size()) return;

    }

    void SceneManager::Destruct()
    {
        for (auto& scene : _scenes)
        {
            scene = nullptr;
        }
    }
}