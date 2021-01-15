#pragma once

#include <string>
#include <memory>

#include "List.hpp"

namespace OpenGL_Core
{
    using namespace std;

    class MeshRenderer;
    class Mesh;
    class Material;
    class Transform;
    class GameObject;
    class Scene;

    class ModelImporter
    {
    public:
        /// <summary>
        /// 加载模型
        /// </summary>
        /// <param name="path">路径</param>
        /// <param name="postProcessSetps">后处理步骤</param>
        /// <returns></returns>
        static unique_ptr<ModelImporter> ModelLoad(const string& path,
            unsigned int postProcessSetps = (0
                | 8 // aiProcess_Triangulate  // 对所有网格的所有面进行三角剖分。
                | 1 // aiProcess_CalcTangentSpace // 沿y轴翻转所有UV坐标，并相应地调整材质设置和双切线。
                //| 4 // aiProcess_MakeLeftHanded // 导入为左手坐标系
                //| 16777216 // | aiProcess_FlipWindingOrder // 翻转绕线顺序（翻转法线）
                ));
        /// <summary>
        /// 添加模型到场景
        /// </summary>
        /// <param name="scene"></param>
        /// <param name="assimp"></param>
        static void AssingToScene(Scene& scene, ModelImporter& assimp);

        List<unique_ptr<GameObject>> gameObjects;
        List<unique_ptr<Mesh>> meshs;
        List<unique_ptr<Material>> materials;

    private:
        //void ProcessScene(const aiScene* scene);
        //void ProcessNode(const aiNode* node, const aiScene* scene, Transform* parent);
        //unique_ptr<Mesh> ProcessMesh(const aiMesh* mesh);
        //unique_ptr<Material> ProcessMaterial(const aiMaterial* material);

        ModelImporter() :gameObjects(), meshs(), materials() { }
    };
}