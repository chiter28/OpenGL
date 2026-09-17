#pragma once

// std
#include <vector>
#include <memory>


// external
#include <glm/glm.hpp>


// internal
#include "Renderer/Light.h"



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

public:
    static void Init();

    static void BeginScene(const Camera& camera, const DirectionalLight& light);
    static void EndScene();
    static void DrawModel(const std::shared_ptr<Model>& model, const glm::mat4& transform);

private:
    static void RenderPass(bool transparentPass);

private:
    inline static std::shared_ptr<Shader> s_Shader;

    inline static std::vector<RenderCommand> s_DrawQueue;
    inline static SceneData s_SceneData;
};