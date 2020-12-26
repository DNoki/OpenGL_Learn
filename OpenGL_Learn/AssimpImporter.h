#pragma once

#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "List.h"

namespace OpenGL_Learn
{
    using namespace std;

    class MeshRenderer;
    class Mesh;
    class Material;
    class Transform;
    class GameObject;
    class Scene;

    class AssimpImporter
    {
    public:
        static unique_ptr<AssimpImporter> AssimpLoad(const string& path,
            unsigned int postProcessSetps = (aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded));
        static void AssingToScene(Scene& scene, AssimpImporter& assimp);

        List<unique_ptr<GameObject>> gameObjects;
        List<unique_ptr<Mesh>> meshs;
        List<unique_ptr<Material>> materials;

    private:
        void ProcessScene(const aiScene* scene);
        void ProcessNode(const aiNode* node, const aiScene* scene, Transform* parent);
        unique_ptr<Mesh> ProcessMesh(const aiMesh* mesh);
        unique_ptr<Material> ProcessMaterial(const aiMaterial* material);

        AssimpImporter() :gameObjects(), meshs(), materials() { }
    };
}