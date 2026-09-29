#pragma once

// std
#include <vector>
#include <memory>


// external
#include <glm/glm.hpp>


// internal
#include "Scene/Light.h"
#include "Materials/MaterialBinder.h"

class Camera;
class DirectionalLight;
class Model;
class Shader;


class Renderer3D
{
public:
    struct RenderCommand
    {
        std::shared_ptr<Model> ModelAsset;
        glm::mat4 Transform;
    };
    
    struct SceneData
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        DirectionalLight Light;
    };

    enum class DebugView : int
    {
        Lit = 0,
        BaseColor,
        Normals,
        Metallic,
        Roughness
    };

public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera, const DirectionalLight& light);
    static void EndScene();
    static void DrawModel(const std::shared_ptr<Model>& model, const glm::mat4& transform);

    static void SetDebugView(DebugView view) { s_DebugView = view; }

private:
    static void RenderPass(bool transparentPass);

private:
    inline static std::shared_ptr<Shader> s_Shader;
    inline static std::unique_ptr<MaterialBinder> s_MaterialBinder;

    inline static std::vector<RenderCommand> s_DrawQueue;
    inline static SceneData s_SceneData;

    
    inline static DebugView s_DebugView = DebugView::Normals;
};