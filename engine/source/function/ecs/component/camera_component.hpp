#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

namespace Airwave
{
enum class ProjectionType
{
    Perspective,
    Orthographic,
};

struct CameraComponent
{
public:
    CameraComponent() = default;
    constexpr static const char *getComponentName() { return "CameraComponent"; }

    glm::mat4 &getProjectionMatrix() const;
    glm::mat4 &getViewMatrix() const;

    void setProjectionType(ProjectionType type);

    void setAspectRatio(float aspect_ratio);
    void setFov(float fov);
    void setNearClip(float near_clip);
    void setFarClip(float far_clip);
    void setOrthoSize(float ortho_size);

    nlohmann::json to_json() const {
        nlohmann::json j;
        j["projection_type"] = static_cast<int>(projection_type);
        j["fov"]             = fov;
        j["near_clip"]       = near_clip;
        j["far_clip"]        = far_clip;
        j["aspect_ratio"]    = aspect_ratio;
        j["ortho_size"]      = ortho_size;
        return j;
    }
    void from_json(const nlohmann::json &j){
        if (j.contains("projection_type"))
        {
            projection_type = static_cast<ProjectionType>(j["projection_type"].get<int>());
        }
        if (j.contains("fov"))
        {
            fov = j["fov"].get<float>();
        }
        if (j.contains("near_clip"))
        {
            near_clip = j["near_clip"].get<float>();
        }
        if (j.contains("far_clip"))
        {
            far_clip = j["far_clip"].get<float>();
        }
        if (j.contains("aspect_ratio"))
        {
            aspect_ratio = j["aspect_ratio"].get<float>();
        }
        if (j.contains("ortho_size"))
        {
            ortho_size = j["ortho_size"].get<float>();
        }
    }

  private:
    ProjectionType projection_type = ProjectionType::Perspective;
    float fov{45.0f};
    float near_clip{0.1f};
    float far_clip{100.0f};
    float aspect_ratio{16.0f / 9.0f};
    float ortho_size{10.0f};

    mutable bool is_dirty{true};
    mutable glm::mat4 projectionMatrix{1.0f};

    mutable glm::mat4 viewMatrix{1.0f};

    friend class CameraSystem;
    friend class RenderSystem;
};

} // namespace Airwave
