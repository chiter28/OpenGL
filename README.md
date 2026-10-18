# OpenGL PBR Engine

A learning 3D engine written in C++20 and OpenGL 4.6 (Core Profile), featuring a physically based renderer, glTF loading, and normal mapping.

![Lit view](docs/images/lit.png)

| Normals | Roughness | Base color |
|---|---|---|
| ![Normals](docs/images/normals.png) | ![Roughness](docs/images/roughness.png) | ![Base color](docs/images/base_color.png) |

## Features

### Rendering
- Physically based shading: Cook–Torrance BRDF with GGX distribution, Smith–GGX geometry term, and Schlick Fresnel; metallic–roughness workflow
- Normal mapping in tangent space (TBN), tangents generated with MikkTSpace
- Correct sRGB pipeline: color textures stored as `SRGB8_ALPHA8`, data textures as linear, `GL_FRAMEBUFFER_SRGB` output
- HDR lighting with exposure control and ACES tone mapping (Narkowicz fit)
- Separate opaque and transparent passes (alpha blending, depth writes disabled for transparent geometry)
- Materials uploaded through a `std140` uniform block
- Debug views: base color, normals, metallic, roughness

### Assets
- glTF 2.0 loading via fastgltf: meshes with submeshes, node hierarchy instances, materials, textures, and sampler settings
- Separate CPU-side import data (`ModelData`) and GPU-side runtime representation (`Model`)

### Core
- RAII wrappers for GPU resources (buffers, vertex arrays, textures, shaders) using Direct State Access; copying is disabled, most wrappers are movable
- Event queue with typed dispatch (window, keyboard, mouse)
- Orbit camera (rotate, pan, zoom)
- Frame-time based updates (delta time)
- Minimal immediate-mode UI: rectangles and draggable sliders for runtime parameters

## Building

### Requirements
- Windows, Visual Studio 2022 (MSVC, C++20)
- CMake 3.24 or newer
- GPU with OpenGL 4.6 support

### Dependencies
Fetched automatically by CMake (`FetchContent`):
- GLFW 3.5.1, GLM 1.0.3, fastgltf 0.9.0, Assimp 6.0.5

Bundled in `external/`:
- glad, stb_image, MikkTSpace


### Steps
```
git clone https://github.com/chiter28/OpenGL.git
cd OpenGL
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```
Run `sandbox.exe` from the build output directory. The `Resources` folder is copied next to the executable automatically.

Alternatively, open the folder in Visual Studio 2022 (CMake project) and select `x64-Debug` or `x64-Release`.

## Controls

| Input | Action |
|---|---|
| Alt + left mouse | Rotate camera |
| Alt + middle mouse | Pan camera |
| Alt + right mouse / Alt + scroll | Zoom |
| 1 | Lit |
| 2 | Base color |
| 3 | Normals |
| 4 | Metallic |
| 5 | Roughness |
| W / A / S / D | Change light direction |
| Sliders | Roughness, light intensity, exposure |
| Esc | Quit |

## Architecture

```
engine/
  Core/        Application loop, window, input, demo interface
  Events/      Event types, queue, and dispatcher
  Renderer/
    Resources/ GPU resource wrappers: buffers, vertex layout, textures, shaders
    Geometry/  Meshes, submeshes, primitives
    Materials/ Material data and shader binding
    Models/    CPU import data and GPU runtime models
    Loader/    glTF loader
    Scene/     Camera and lights
    UI/        UI rectangle renderer and sliders
sandbox/       Demo scenes
Resources/     Shaders, meshes, textures
```

Frame flow:
```
App::Run
  -> Demo::OnUpdate(deltaTime)
  -> Demo::OnRender(camera)
       -> Renderer3D::BeginScene(camera, light, exposure)
       -> Renderer3D::DrawModel(model, transform)
       -> Renderer3D::EndScene()
            -> opaque pass
            -> transparent pass
```

## Technical notes

### Color spaces
Lighting is computed in linear space. Albedo textures are stored as sRGB so the GPU converts them to linear on sampling, while normal, metallic, and roughness maps hold data and are stored as linear. The final conversion back to sRGB is done by the hardware through `GL_FRAMEBUFFER_SRGB`, so the shader never applies gamma manually.

### HDR, exposure, and tone mapping
Physically based lighting produces values above 1.0, which an 8-bit framebuffer would clip, flattening highlights and shifting hues. Exposure scales the HDR value first, then ACES tone mapping compresses it into [0, 1]. ACES was chosen over Reinhard because Reinhard never reaches white and lowers mid-tone contrast.

### GPU resource ownership
Each GPU resource is owned by exactly one C++ object. Copying is deleted because copying an OpenGL handle would lead to a double delete; moving transfers the handle and leaves the source empty. Resources are destroyed while the OpenGL context is still alive.

### CPU and GPU model representations
The loader produces `ModelData`, plain CPU data independent of OpenGL. `Model` is built from it and owns the GPU buffers and textures. This keeps the importer testable and free of graphics API calls.

## Roadmap
- [ ] Shadow mapping for directional light (PCF)
- [ ] HDR framebuffer with tone mapping as a post-process pass, bloom
- [ ] Dear ImGui debug UI and GPU timer queries
- [ ] Image-based lighting (irradiance, prefiltered specular, BRDF LUT)
- [ ] Demo scene with multiple objects and frustum culling

## Third-party
- [GLFW](https://www.glfw.org/), [glad](https://github.com/Dav1dde/glad), [GLM](https://github.com/g-truc/glm), [fastgltf](https://github.com/spnda/fastgltf), [stb_image](https://github.com/nothings/stb), [MikkTSpace](https://github.com/mmikk/MikkTSpace)
- Assets: TODO — add source and license for the models and textures in `Resources/`
