#include "AssimpImporter.h"

#include <iostream>

#include "Math.h"
#include "GameObject.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"

namespace OpenGL_Learn
{
    unique_ptr<AssimpImporter> AssimpImporter::AssimpLoad(const string& path, unsigned int postProcessSetps)
    {
        //Assimp::Importer i;
        Assimp::Importer importer;
        // 更多枚举类型 http://sir-kimmi.de/assimp/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410a8857a0e30688127a82c7b8939958c6dc
        const aiScene* scene = importer.ReadFile(path.c_str(), postProcessSetps
            //aiProcess_Triangulate | // 对所有网格的所有面进行三角剖分。
            //aiProcess_FlipUVs | // 沿y轴翻转所有UV坐标，并相应地调整材质设置和双切线。
            //aiProcess_CalcTangentSpace | // 计算导入的网格的切线和切线。
            //aiProcess_MakeLeftHanded | // 导入为左手坐标系
            //aiProcess_FlipWindingOrder // 翻转绕线顺序（翻转法线）
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            throw;
        }

        auto assimp = unique_ptr<AssimpImporter>(new AssimpImporter());
        assimp->ProcessScene(scene);
        return assimp;
    }

    void AssimpImporter::AssingToScene(Scene& scene, AssimpImporter& assimp)
    {
        for (auto& obj : assimp.gameObjects)
        {
            scene.AddGameObject(obj);
        }
        for (auto& mesh : assimp.meshs)
        {
            scene.AddResourceObject(mesh);
        }
        for (auto& material : assimp.materials)
        {
            scene.AddResourceObject(material);
        }
    }

    void AssimpImporter::ProcessScene(const aiScene* scene)
    {
        //for (unsigned int i = 0; i < scene->mNumTextures; i++)
        //{
        //    auto tex = scene->mTextures[i];
        //}

        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            materials.push_back(move(ProcessMaterial(scene->mMaterials[i])));
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            meshs.push_back(move(ProcessMesh(scene->mMeshes[i])));
        }

        ProcessNode(scene->mRootNode, scene, nullptr);
    }

    void AssimpImporter::ProcessNode(const aiNode* node, const aiScene* scene, Transform* parent)
    {
        auto objPtr = make_unique<GameObject>(node->mName.C_Str());
        auto obj = objPtr.get();
        gameObjects.push_back(move(objPtr));

        obj->GetTransform().SetParent(parent);

        auto matrix = Matrix4x4();
        for (unsigned int x = 0; x < 4; x++)
        {
            for (unsigned int y = 0; y < 4; y++)
            {
                matrix[x][y] = node->mTransformation[x][y];
            }
        }
        obj->GetTransform().SetTransformMatrix(matrix);

        // 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            auto meshRenderer = &obj->AddComponent<MeshRenderer>();
            meshRenderer->SetData(*meshs[node->mMeshes[i]],
                *materials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]);
        }

        //MeshRenderer* meshRenderer;
        //if (node->mNumMeshes > 0)
        //{
        //    meshRenderer = &obj->AddComponent<MeshRenderer>();

        //    // 处理节点所有的网格（如果有的话）
        //    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        //    {
        //        meshRenderer->SetData(
        //            *meshs[node->mMeshes[i]].get(),
        //            *materials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]);
        //    }
        //}

        // 接下来对它的子节点重复这一过程
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene, &obj->GetTransform());
        }
    }

    unique_ptr<Mesh> AssimpImporter::ProcessMesh(const aiMesh* mesh)
    {
        auto meshResult = make_unique<Mesh>(mesh->mName.C_Str());

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            meshResult->vertices.push_back(Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

            if (mesh->HasNormals())
                meshResult->normals.push_back(Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
            if (mesh->HasTangentsAndBitangents())
                meshResult->tangents.push_back(Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
            if (mesh->HasVertexColors(0)) // 是否有第0套VertexColor集
                meshResult->colors.push_back(Vector4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a));
            if (mesh->HasTextureCoords(0)) // 是否有第0套UV集
                meshResult->uv.push_back(Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            auto face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                meshResult->indices.push_back(face.mIndices[j]);
            }
        }
        meshResult->Complete();
        return meshResult;
    }

    unique_ptr<Material> AssimpImporter::ProcessMaterial(const aiMaterial* material)
    {
        aiString aiName;
        material->Get(AI_MATKEY_NAME, aiName);

        auto materialResult = unique_ptr<Material>(new Material(aiName.C_Str(), initializer_list<Shader*>()));

        return materialResult;
    }

    //Tuple_Mesh_Material AssimpImporter::processMesh(aiMesh* mesh, const aiScene* scene)
    //{
    //    auto findMesh = ResourceManager::FindMesh(mesh->mName.C_Str(), mesh->mNumVertices);
    //    if (findMesh)
    //    {
    //        cout << "找到重复网格" << endl;
    //        return Tuple_Mesh_Material(findMesh, Material(const_cast<Shader&>(shader)));
    //    }

    //    auto m = make_unique<Mesh>(mesh->mName.C_Str());

    //    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    //    {
    //        m->vertices->push_back(Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
    //        m->normals->push_back(Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
    //        if (mesh->HasTangentsAndBitangents())
    //        {
    //            m->tangents->push_back(Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
    //        }
    //        if (mesh->mColors[0])
    //        {
    //            m->colors->push_back(vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a));
    //        }
    //        if (mesh->mTextureCoords[0])
    //        {
    //            m->uv->push_back(vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
    //        }
    //    }

    //    // 索引
    //    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    //    {
    //        auto face = mesh->mFaces[i];
    //        for (unsigned int j = 0; j < face.mNumIndices; j++)
    //        {
    //            m->indices->push_back(face.mIndices[j]);
    //        }
    //        //if (!mesh->HasTangentsAndBitangents() && face.mNumIndices == 3)
    //        //{
    //        //    CalculateTangent(vertices[face.mIndices[0]], vertices[face.mIndices[1]], vertices[face.mIndices[2]]);
    //        //}

    //    }
    //    // 材质
    //    if (mesh->mMaterialIndex >= 0)
    //    {
    //        auto material = scene->mMaterials[mesh->mMaterialIndex];
    //    }

    //    m->Complete();
    //    auto rMesh = m.get();
    //    //Scene::Meshes.push_back(move(m));
    //    ResourceManager::AddMesh(move(m));

    //    return Tuple_Mesh_Material(rMesh, Material(const_cast<Shader&>(shader)));
    //}
}