﻿#include "pch.h"

#include "Skybox.h"

#include "SceneManager.h"
#include "Graphics.h"

#include "Mesh.h"
#include "Material.h"

namespace OpenGL_Core
{
    unique_ptr<Mesh> Skybox::skyboxMesh;


    void Skybox::DrawSkyBox()
    {
        for (unsigned int i = 0; i < _material->GetPassCount(); i++)
            Graphics::DrawMesh(*skyboxMesh, *_material, i);
    }

    Skybox::Skybox(const string& name, Material* material) :ResourceObject(name), _material(material)
    {
        // 初始化天空盒模型
        if (!skyboxMesh)
        {
            skyboxMesh = make_unique<Mesh>("SkyboxMesh");
            skyboxMesh->HideFlag = HideFlagType::STATIC;
            float skyboxVertices[] = {
                // positions          
                -1.0f,  1.0f, -1.0f,	-1.0f, -1.0f, -1.0f,	 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,	 1.0f,  1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,	-1.0f, -1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,	-1.0f,  1.0f,  1.0f,	-1.0f, -1.0f,  1.0f,

                 1.0f, -1.0f, -1.0f,	 1.0f, -1.0f,  1.0f,	 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,	 1.0f,  1.0f, -1.0f,	 1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,	 1.0f, -1.0f,  1.0f,	-1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,	 1.0f,  1.0f, -1.0f,	 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	-1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,	-1.0f, -1.0f,  1.0f,	 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,	-1.0f, -1.0f,  1.0f,	 1.0f, -1.0f,  1.0f
            };

            for (unsigned int i = 0; i < 36; i++)
            {
                auto v = Vector3(skyboxVertices[i * 3], skyboxVertices[i * 3 + 2], skyboxVertices[i * 3 + 1]);
                skyboxMesh->vertices.push_back(v);
            }
            skyboxMesh->Complete();
        }
    }

}