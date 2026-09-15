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
class Mesh;


class Renderer3D
{
public:
    struct RenderCommund
    {
        std::shared_ptr<Mesh> MeshAsset;
        glm::mat4 Transform;
    };
    
    struct SceneData
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        DirectionalLight Light;
    };

public:
    static void BeginScene(const Camera& camera, const DirectionalLight& light);
    static void EndScene();
    static void DrawMesh(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform);

private:
    static void RenderPass(bool transparentPass);

private:
    inline static std::vector<RenderCommund> s_DrawQueue;
    inline static SceneData s_SceneData;
};