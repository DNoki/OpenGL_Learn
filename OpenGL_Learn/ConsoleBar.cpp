#include "pch.h"

#include "ConsoleBar.h"

#include "IConsoleBarScriptBehaviour.hpp"

namespace OpenGL_Learn
{
    void ConsoleBar::CheckGameObject(int index, stringstream& fmt, GameObject& obj)
    {
        fmt.str(""); fmt << index;
        auto enable = obj.GetActive(false);
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        obj.SetActive(enable);
        fmt.str(""); fmt << obj.Name;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn(); ImGui::NextColumn();
            auto& transform = obj.GetTransform();
            if (ImGui::TreeNode("Transform"))
            {
                ImGui::NextColumn();
                ImGui::DragFloat3("P", const_cast<float*>(transform.LocalPosition.GetPtr()), 0.01f, 0.f, 0.f, "%.3f"); ImGui::SameLine();
                if (ImGui::Button("Reset Pos")) transform.LocalPosition = Vector3::Zero;
                ImGui::DragFloat3("R", const_cast<float*>(transform.LocalEulerAngles.GetPtr()), 1.0f, 0, 0, "%.3f"); ImGui::SameLine();
                if (ImGui::Button("Reset Rot")) transform.LocalEulerAngles = Vector3::Zero;
                ImGui::DragFloat3("S", const_cast<float*>(transform.LocalScale.GetPtr()), 0.01f, 0, 0, "%.3f"); ImGui::SameLine();
                if (ImGui::Button("Reset Scl")) transform.LocalScale = Vector3::One;
                ImGui::NextColumn();
                ImGui::TreePop();
            }

            auto& components = obj.GetAllComponents();
            for (size_t j = 0; j < components.Count(); j++)
            {
                ImGui::NextColumn(); ImGui::NextColumn();
                if (CheckCamera(j, fmt, components[j].get()) ||
                    //CheckPostProcess(j, fmt, components[j].get()) ||
                    CheckDirLight(j, fmt, components[j].get()) ||
                    CheckPointLight(j, fmt, components[j].get()) ||
                    CheckRenderer(j, fmt, components[j].get())
                    ) {
                }
                else
                {
                    auto script = dynamic_cast<IConsoleBarScriptBehaviour*>(components[j].get());
                    if (script)
                    {
                        script->OnConsoleBar(j, fmt);
                    }
                    else
                    {
                        fmt.str(""); fmt << j << " " << typeid(*components[j]).name();
                        if (ImGui::TreeNode(fmt.str().c_str()))
                            // 其他未设定组件
                            ImGui::TreePop();
                    }
                }
            }
            auto& childs = transform.GetChilds();
            for (size_t ci = 0; ci < obj.GetChildsCount(); ci++)
                CheckGameObject(ci, fmt, childs[ci]->GetGameObject());
            ImGui::TreePop();
        }

    }
    bool ConsoleBar::CheckCamera(int index, stringstream& fmt, Component* component)
    {
        auto camera = dynamic_cast<Camera*>(component);
        if (!camera) return false;
        fmt.str(""); fmt << index;
        auto enable = camera->GetEnable();
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        camera->SetEnable(enable);
        fmt.str(""); fmt << "Camera";
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            ImGui::ColorEdit4("Background Color", &camera->State.BackgroundColor.r, ImGuiColorEditFlags_HDR);
            ImGui::DragFloat("Clear Depte", &(camera->State.ClearDepth), 0.01f, 0.0f, 1.0f);
            ImGui::DragInt("Clear Stencil", &(camera->State.ClearStencil), 1, 0, 1);

            ImGui::Text("Projection Mode");
            ImGui::RadioButton("PERSPECTIVE", (int*)&camera->ProjectionMode, static_cast<int>(ProjectionMode::PERSPECTIVE));
            ImGui::SameLine();
            ImGui::RadioButton("ORTHOGRAPHIC", (int*)&camera->ProjectionMode, static_cast<int>(ProjectionMode::ORTHOGRAPHIC));

            ImGui::DragFloat("Fov", &camera->FieldOfView, 1.0f, 1.0f, 179.0f);
            ImGui::DragFloat("Orthographic Size", &camera->OrthographicSize);
            ImGui::Checkbox("IsMSAA", &camera->IsMSAA);
            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }
    //bool ConsoleBar::CheckPostProcess(int index, stringstream& fmt, Component* component)
    //{
    //    auto postProcess = dynamic_cast<PostProcess*>(component);
    //    if (!postProcess) return false;
    //    fmt.str(""); fmt << index;
    //    ImGui::Checkbox(fmt.str().c_str(), &postProcess->_enabled); ImGui::SameLine();
    //    fmt.str(""); fmt << "PostProcess" ;
    //    if (ImGui::TreeNode(fmt.str().c_str()))
    //    {
    //        for (size_t i = 0; i < postProcess->GetEffectCount(); i++)
    //        {
    //            ImGui::NextColumn(); ImGui::NextColumn();
    //            auto effect = postProcess->GetEffectMaterial(i);
    //            ImGui::Checkbox(to_string(i).c_str(), &effect->Item1); ImGui::SameLine();
    //            CheckMaterial(i, fmt, effect->Item2);
    //            //ImGui::Text(effect->Item2->Name.c_str());
    //        }
    //        ImGui::TreePop();
    //    }
    //    return true;
    //}
    bool ConsoleBar::CheckDirLight(int index, stringstream& fmt, Component* component)
    {
        auto dirLight = dynamic_cast<DirectionalLight*>(component);
        if (!dirLight) return false;
        fmt.str(""); fmt << index;
        auto enable = dirLight->GetEnable();
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        dirLight->SetEnable(enable);
        fmt.str(""); fmt << "Directional Light";
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            ImGui::SliderFloat("Intensity", &dirLight->Intensity, 0.f, 1.f, "Intensity:%.3f");
            ImGui::ColorEdit3("Light Color", &dirLight->LightColor.r, ImGuiColorEditFlags_HDR);

            ImGui::SliderFloat("Strength", &dirLight->ShadowStrength, 0.f, 1.f, "Strength:%.3f");
            ImGui::SliderFloat("Bias", &dirLight->Bias, 0.f, 0.001f, "Bias:%.6f");
            ImGui::SliderFloat("NormalBias", &dirLight->NormalBias, 0.f, 3.f, "NormalBias:%.4f");

            ImGui::DragFloat("Shadow Distance", &dirLight->ShadowDistance);
            ImGui::DragFloat("Shadow Near Plane Offset", &dirLight->ShadowNearPlaneOffset);
            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }
    bool ConsoleBar::CheckPointLight(int index, stringstream& fmt, Component* component)
    {
        auto pointLight = dynamic_cast<PointLight*>(component);
        if (!pointLight) return false;
        fmt.str(""); fmt << index;
        auto enable = pointLight->GetEnable();
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        pointLight->SetEnable(enable);
        fmt.str(""); fmt << "Point Light";
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            ImGui::SliderFloat("Intensity", &pointLight->Intensity, 0.f, 1.f, "Intensity:%.3f");
            ImGui::ColorEdit3("Light Color", &pointLight->LightColor.r, ImGuiColorEditFlags_HDR);

            ImGui::SliderFloat("Strength", &pointLight->ShadowStrength, 0.f, 1.f, "Strength:%.3f");
            ImGui::SliderFloat("Bias", &pointLight->Bias, 0.f, 0.1f, "Bias:%.6f");
            ImGui::SliderFloat("NormalBias", &pointLight->NormalBias, 0.f, 3.f, "NormalBias:%.4f");
            ImGui::DragFloat2("Attenuation", const_cast<float*>(pointLight->Attenuation.GetPtr()), 0.001f, 0.0f, 10.0f);

            ImGui::DragFloat("Shadow Distance", &pointLight->ShadowDistance);
            ImGui::DragFloat("Shadow Near Plane Offset", &pointLight->ShadowNearPlaneOffset);
            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }
    bool ConsoleBar::CheckRenderer(int index, stringstream& fmt, Component* component)
    {
        auto renderer = dynamic_cast<Renderer*>(component);
        if (!renderer) return false;
        fmt.str(""); fmt << index;
        auto enable = renderer->GetEnable();
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        renderer->SetEnable(enable);
        fmt.str(""); fmt << "Renderer";
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::Text("Material:");
            ImGui::NextColumn(); ImGui::NextColumn();
            CheckMaterial(0, fmt, renderer->GetMaterial());

            //auto items = renderer->GetRenderItems();
            //for (size_t i = 0; i < items->size(); i++)
            //{
            //    //(*items)[i]->index;
            //}

            if (typeid(*renderer) == typeid(MeshRenderer))
            {
                auto meshRenderer = dynamic_cast<MeshRenderer*>(renderer);
                ImGui::Text("Mesh:");
                ImGui::NextColumn(); ImGui::NextColumn();
                CheckMesh(0, fmt, meshRenderer->GetMesh());
            }
            ImGui::TreePop();
        }
        return true;
    }
    void ConsoleBar::CheckResources(map<string, List<unique_ptr<ResourceObject>>>& objs)
    {
        stringstream fmt;

        for (auto& obj : objs)
        {
            ImGui::NextColumn(); ImGui::NextColumn();
            if (obj.first == typeid(Shader).name())
                if (ImGui::TreeNode("Shaders"))
                {
                    for (size_t i = 0; i < obj.second.size(); i++)
                    {
                        ImGui::NextColumn(); ImGui::NextColumn();
                        CheckShader(i, fmt, obj.second[i].get());
                    }
                    ImGui::TreePop();
                }
            if (obj.first == typeid(Material).name())
                if (ImGui::TreeNode("Materials"))
                {
                    for (size_t i = 0; i < obj.second.size(); i++)
                    {
                        ImGui::NextColumn(); ImGui::NextColumn();
                        CheckMaterial(i, fmt, obj.second[i].get());
                    }
                    ImGui::TreePop();
                }
            if (obj.first == typeid(Mesh).name())
                if (ImGui::TreeNode("Meshs"))
                {
                    for (size_t i = 0; i < obj.second.size(); i++)
                    {
                        ImGui::NextColumn(); ImGui::NextColumn();
                        CheckMesh(i, fmt, obj.second[i].get());
                    }
                    ImGui::TreePop();
                }
        }
        ImGui::NextColumn(); ImGui::NextColumn();
        if (ImGui::TreeNode("Textures"))
        {
            for (auto& obj : objs)
            {
                ImGui::NextColumn(); ImGui::NextColumn();
                if (obj.first == typeid(Texture2D).name() ||
                    obj.first == typeid(TextureCube).name())
                    for (size_t i = 0; i < obj.second.size(); i++)
                    {
                        ImGui::NextColumn(); ImGui::NextColumn();
                        CheckTexture(i, fmt, obj.second[i].get());
                    }
            }
            ImGui::TreePop();
        }
        ImGui::NextColumn(); ImGui::NextColumn();
        if (ImGui::TreeNode("RenderTextures"))
        {
            for (auto& obj : objs)
            {
                ImGui::NextColumn(); ImGui::NextColumn();
                if (obj.first == typeid(RenderTexture).name())// ||
                    //obj.first == typeid(MultisampleRenderTexture).name() ||
                    //obj.first == typeid(CubeRenderTexture).name())
                    for (size_t i = 0; i < obj.second.size(); i++)
                    {
                        ImGui::NextColumn(); ImGui::NextColumn();
                        CheckRenderTexture(i, fmt, obj.second[i].get());
                    }
            }
            ImGui::TreePop();
        }
    }
    bool ConsoleBar::CheckShader(int index, stringstream& fmt, ResourceObject* obj)
    {
        auto shader = dynamic_cast<Shader*>(obj);
        if (!shader) return false;
        fmt.str(""); fmt << index << " " << shader->Name;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            ImGui::Text("Shader ID: %d", shader->ID);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
            ImGui::SliderInt("Render Sequence", (int*)&shader->RenderSequence, 0, 5000);
            ImGui::Combo("Hide Flag", (int*)&shader->HideFlag, "Normal\0Static\0");

            if (ImGui::TreeNode("Uniforms"))
            {
                auto uniforms = shader->GetAllUniforms();
                for (auto& uniform : uniforms)
                {
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
                    if (uniform.Item1 == UniformType::INT)
                    {
                        ImGui::DragInt(uniform.Item2.c_str(), shader->GetIntPtr(uniform.Item2));
                    }
                    else if (uniform.Item1 == UniformType::FLOAT)
                    {
                        ImGui::DragFloat(uniform.Item2.c_str(), shader->GetFloatPtr(uniform.Item2), 0.01f);
                    }
                    else if (uniform.Item1 == UniformType::FLOAT_VEC4)
                    {
                        ImGui::DragFloat4(uniform.Item2.c_str(), (float*)shader->GetVector4Ptr(uniform.Item2), 0.01f);
                    }
                    else if (uniform.Item1 == UniformType::SAMPLER_2D)
                    {
                        fmt.str(""); fmt << uniform.Item2.c_str() << ": " << shader->GetInt(uniform.Item2);
                        ImGui::Text(fmt.str().c_str());
                    }
                }
                ImGui::TreePop();
            }

            { // 深度测试
                ImGui::Checkbox("Depth Test", &shader->State.DepthTest);
                if (shader->State.DepthTest)
                {
                    ImGui::Checkbox("Depth Mask", &shader->State.DepthMask);
                    auto mode = static_cast<int>(shader->State.DepthTestMode) - static_cast<int>(TestModeType::NEVER);
                    ImGui::Combo("Depth Test Mode", &mode, "NEVER\0LESS\0EQUAL\0LEQUAL\0GREATER\0NOTEQUAL\0GEQUAL\0ALWAYS\0");
                    shader->State.DepthTestMode = static_cast<TestModeType>(mode + static_cast<int>(TestModeType::NEVER));
                }
            }
            { // 模板测试
                ImGui::Checkbox("Stencil Test", &shader->State.Stencil);
                if (shader->State.Stencil)
                {
                    ImGui::SliderInt("Stencil Mask", (int*)&shader->State.StencilMask, 0x00, 0xFF, "%d");
                    auto mode = static_cast<int>(shader->State.StencilTestMode) - static_cast<int>(TestModeType::NEVER);
                    ImGui::Combo("Stencil Test Mode", &mode, "NEVER\0LESS\0EQUAL\0LEQUAL\0GREATER\0NOTEQUAL\0GEQUAL\0ALWAYS\0");
                    shader->State.StencilTestMode = static_cast<TestModeType>(mode + static_cast<int>(TestModeType::NEVER));
                    ImGui::SliderInt("Stencil Ref", &shader->State.StencilRef, 0x00, 0xFF, "%d");
                    ImGui::SliderInt("Stencil Ref Mask", (int*)&shader->State.StencilRefMask, 0x00, 0xFF, "%d");
                }
            }
            { // 混合
                ImGui::Checkbox("Blend", &shader->State.Blend);
                if (shader->State.Blend)
                {
                    static auto items = List<Tuple2<int, BlendFactorType>>({
                         Tuple2<int, BlendFactorType>(0, BlendFactorType::ZERO),
                         Tuple2<int, BlendFactorType>(1, BlendFactorType::ONE),
                         Tuple2<int, BlendFactorType>(2, BlendFactorType::SRC_COLOR),
                         Tuple2<int, BlendFactorType>(3, BlendFactorType::ONE_MINUS_SRC_COLOR),
                         Tuple2<int, BlendFactorType>(4, BlendFactorType::DST_COLOR),
                         Tuple2<int, BlendFactorType>(5, BlendFactorType::ONE_MINUS_DST_COLOR),
                         Tuple2<int, BlendFactorType>(6, BlendFactorType::SRC_ALPHA),
                         Tuple2<int, BlendFactorType>(7, BlendFactorType::ONE_MINUS_SRC_ALPHA),
                         Tuple2<int, BlendFactorType>(8, BlendFactorType::DST_ALPHA),
                         Tuple2<int, BlendFactorType>(9, BlendFactorType::ONE_MINUS_DST_ALPHA),
                         Tuple2<int, BlendFactorType>(10, BlendFactorType::CONSTANT_COLOR),
                         Tuple2<int, BlendFactorType>(11, BlendFactorType::ONE_MINUS_CONSTANT_COLOR),
                         Tuple2<int, BlendFactorType>(12, BlendFactorType::CONSTANT_ALPHA),
                         Tuple2<int, BlendFactorType>(13, BlendFactorType::ONE_MINUS_CONSTANT_ALPHA),
                         Tuple2<int, BlendFactorType>(14, BlendFactorType::SRC_ALPHA_SATURATE), });

                    auto index = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item2 == shader->State.BlendSfactor; })->Item1;
                    ImGui::Combo("Blend Sfactor", &index, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0SRC_ALPHA_SATURATE\0");
                    shader->State.BlendSfactor = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item1 == index; })->Item2;
                    index = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item2 == shader->State.BlendDfactor; })->Item1;
                    ImGui::Combo("Blend Dfactor", &index, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0SRC_ALPHA_SATURATE\0");
                    shader->State.BlendDfactor = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item1 == index; })->Item2;

                    index = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item2 == shader->State.BlendSfactorAlpha; })->Item1;
                    ImGui::Combo("Blend Sfactor Alpha", &index, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0SRC_ALPHA_SATURATE\0");
                    shader->State.BlendSfactorAlpha = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item1 == index; })->Item2;
                    index = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item2 == shader->State.BlendDfactorAlpha; })->Item1;
                    ImGui::Combo("Blend Dfactor Alpha", &index, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0SRC_ALPHA_SATURATE\0");
                    shader->State.BlendDfactorAlpha = items.Find([&](const Tuple2<int, BlendFactorType>& t)->bool {return t.Item1 == index; })->Item2;
                }
            }
            { // 面剔除
                ImGui::Checkbox("Cull Face", &shader->State.CullFace);
                if (shader->State.CullFace)
                {
                    ImGui::Text("Cull Face Mode");
                    ImGui::RadioButton("FRONT", (int*)&shader->State.CullFaceMode, static_cast<int>(CullFaceModeType::FRONT));
                    ImGui::SameLine();
                    ImGui::RadioButton("BACK", (int*)&shader->State.CullFaceMode, static_cast<int>(CullFaceModeType::BACK));
                    ImGui::SameLine();
                    ImGui::RadioButton("FRONT_AND_BACK", (int*)&shader->State.CullFaceMode, static_cast<int>(CullFaceModeType::FRONT_AND_BACK));
                }
            }
            // 正向面
            ImGui::Text("Front Face");
            ImGui::RadioButton("CCW", (int*)&shader->State.FrontFace, GL_CCW);
            ImGui::SameLine();
            ImGui::RadioButton("CW", (int*)&shader->State.FrontFace, GL_CW);

            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }
    bool ConsoleBar::CheckMaterial(int index, stringstream& fmt, ResourceObject* obj)
    {
        auto material = dynamic_cast<Material*>(obj);
        if (!material) return false;
        fmt.str(""); fmt << index << " " << material->Name;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            ImGui::Combo("Hide Flag", (int*)&material->HideFlag, "Normal\0Static\0");
            ImGui::NextColumn();
            for (size_t i = 0; i < material->Shaders.Count(); i++)
            {
                ImGui::NextColumn(); ImGui::NextColumn();
                CheckShader(i, fmt, material->Shaders[i]);
            }
            ImGui::TreePop();
        }
        return true;
    }
    bool ConsoleBar::CheckMesh(int index, stringstream& fmt, ResourceObject* obj)
    {
        auto mesh = dynamic_cast<Mesh*>(obj);
        if (!mesh) return false;
        fmt.str(""); fmt << index << " " << mesh->Name;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            fmt.str(""); fmt << "Vertices:" << mesh->GetVertexCount() << "  Indices:" << mesh->GetIndiceCount();
            ImGui::Text(fmt.str().c_str());
            ImGui::Combo("Hide Flag", (int*)&mesh->HideFlag, "Normal\0Static\0");
            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }
    bool ConsoleBar::CheckTexture(int index, stringstream& fmt, ResourceObject* obj)
    {
        auto texture = dynamic_cast<Texture*>(obj);
        if (!texture) return false;
        fmt.str(""); fmt << index << " " << texture->Name;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            fmt.str(""); fmt << "Type: ";
            if (typeid(*texture) == typeid(Texture2D))
                fmt << "Texture2D";
            else if (typeid(*texture) == typeid(TextureCube))
                fmt << "TextureCube";
            ImGui::Text(fmt.str().c_str());
            ImGui::Combo("Hide Flag", (int*)&texture->HideFlag, "Normal\0Static\0");
            fmt.str(""); fmt << "ID: " << texture->ID;
            ImGui::Text(fmt.str().c_str());
            fmt.str(""); fmt << "width: " << texture->GetWidth() << "  height: " << texture->GetHeight();
            ImGui::Text(fmt.str().c_str());
            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }
    bool ConsoleBar::CheckRenderTexture(int index, stringstream& fmt, ResourceObject* obj)
    {
        auto texture = dynamic_cast<RenderTexture*>(obj);
        if (!texture) return false;
        fmt.str(""); fmt << index << " " << texture->Name;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            ImGui::NextColumn();
            fmt.str("");
            if (typeid(*texture) == typeid(RenderTexture))
                fmt << "Type: " << "RenderTexture";
            //else if (typeid(*texture) == typeid(MultisampleRenderTexture))
            //{
            //    fmt << "Samples: " << dynamic_cast<MultisampleRenderTexture*>(texture)->GetSamples();
            //    ImGui::Text(fmt.str().c_str());
            //    fmt.str(""); fmt << "Type: " << "MultisampleRenderTexture";
            //}
            //else if (typeid(*texture) == typeid(CubeRenderTexture))
            //    fmt << "Type: " << "CubeRenderTexture";
            ImGui::Text(fmt.str().c_str());
            ImGui::Combo("Hide Flag", (int*)&texture->HideFlag, "Normal\0Static\0");
            fmt.str(""); fmt << "FBO ID:" << texture->GetFrameBufferID()
                << "  RBO ID:" << texture->GetRenderBufferObjectID();
            auto& textures = texture->GetTextures();
            for (auto& tex : textures)
            {
                fmt.str(""); fmt << "ID: " << tex->ID;
                ImGui::Text(fmt.str().c_str());
                ImGui::SameLine();
                fmt.str(""); fmt << "  width: " << tex->GetWidth() << "  height: " << tex->GetHeight();
                ImGui::Text(fmt.str().c_str());
            }
            //ImGui::Text(fmt.str().c_str());
            ImGui::NextColumn();
            ImGui::TreePop();
        }
        return true;
    }

    void ConsoleBar::Awake()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(GameSystem::GetGLFWwindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
    void ConsoleBar::Update()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            auto objs = SceneManager::GetActiveScene()->GetAllRootGameObjects(true);
            ImGui::Begin("Hierarchy");
            ImGui::Text("FPS:%.f", Time::FPS());
            ImGui::SameLine();
            ImGui::Text("  DrawCall:%d", GameSystem::DrawCallCounter);
            GameSystem::DrawCallCounter = 0;

            ImGui::Columns(2, "component", true);
            stringstream fmt;
            for (size_t i = 0; i < objs.Count(); i++)
            {
                if (objs[i]->Name == "Console Bar") 
                    continue;
                ImGui::NextColumn(); ImGui::NextColumn();
                CheckGameObject(i, fmt, *objs[i]);
            }
            ImGui::End();
        }
        {
            auto& objs = SceneManager::GetActiveScene()->GetAllResourceObjects();
            ImGui::Begin("Resources");
            ImGui::Columns(2, "component", true);
            CheckResources(objs);
            ImGui::End();
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
        //    ImGui::ShowDemoWindow(&show_demo_window);
        //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        //{
        //    static float f = 0.0f;
        //    static int counter = 0;
        //    static bool isVsyn = false;

        //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //    ImGui::Checkbox("Another Window", &show_another_window);

        //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //    {
        //        counter++;

        //        isVsyn = !isVsyn;
        //        glfwMakeContextCurrent(GameSystem::GetGLFWwindow());// 指定窗口的上下文成为当前线程。
        //        if (isVsyn)
        //            glfwSwapInterval(1);// 交换缓冲区之前要等待的最小屏幕更新数。 垂直同步
        //        else
        //            glfwSwapInterval(0);
        //    }
        //    ImGui::SameLine();
        //    ImGui::Text("counter = %d", counter);

        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //    ImGui::End();
        //}
        //// 3. Show another simple window.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}
    }
    void ConsoleBar::OnGui()
    {
        this->GetGameObject().SetActive(true);
        SetEnable(true);
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    ConsoleBar::~ConsoleBar()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
    }
}