#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <utility>
#include "model.h"
#include "maths/maths.h"

class Component {
public:
    explicit Component(std::string name) : name_(std::move(name)) { }

    [[nodiscard]] std::string name() const { return name_; }

private:
    std::string name_;
};

class Transform : public Component {
public:
    Transform() : Component("Transform") { }

    [[nodiscard]] Matrix4x4 GetModelMatrix() const;

    Vector3f position {0, 0, 0};
    Vector3f rotation {0, 0, 0};
    Vector3f scale {1, 1, 1};
};

class Mesh : public Component {
public:
    Mesh()                                              : Component("Mesh"), model_(nullptr) { }
    Mesh(const Mesh& other)                             : Component("Mesh"), model_(other.model_) { }
    explicit Mesh(const std::shared_ptr<Model>& model)  : Component("Mesh"), model_(model) { }
    explicit Mesh(const std::string& filename)          : Component("Mesh"), model_(std::make_shared<Model>(filename)) { }

    void SetModel(const std::shared_ptr<Model>& model) { model_ = model; }

    [[nodiscard]] std::shared_ptr<Model> model() const { return model_; }

private:
    std::shared_ptr<Model> model_;
};

class Camera : public Component {
public:
    Camera() : Component("Camera") { }
    explicit Camera(const float fov, const float aspect_ratio, const float z_near, const float z_far)
        : Component("Camera"), fov(fov), aspect_ratio(aspect_ratio), z_near(z_near), z_far(z_far) { }

    [[nodiscard]] Matrix4x4 GetProjectionMatrix() const;

    float fov {45.0f};
    float aspect_ratio {1.0f};
    float z_near {0.1f};
    float z_far {1000.0f};
};


struct GameObject {
    explicit GameObject(std::string name = "GameObject") : name(std::move(name)) { }

    std::string name;
    Transform transform;

    void SetPosition(const Vector3f& position) { transform.position = position; }
    void SetRotation(const Vector3f& rotation) { transform.rotation = rotation; }
    void SetScale(const Vector3f& scale) { transform.scale = scale; }

    [[nodiscard]] Vector3f GetPosition() const { return transform.position; }
    [[nodiscard]] Vector3f GetRotation() const { return transform.rotation; }
    [[nodiscard]] Vector3f GetScale() const { return transform.scale; }

    [[nodiscard]] Matrix4x4 GetModelMatrix() const { return transform.GetModelMatrix(); }
};

struct MeshObject : GameObject {
    explicit MeshObject(std::string name = "MeshObject") : GameObject(std::move(name)) { }

    std::shared_ptr<Mesh> mesh;
};

struct CameraObject : GameObject {
    explicit CameraObject(std::string name = "CameraObject") : GameObject(std::move(name)) { }

    Camera camera;

    [[nodiscard]] Matrix4x4 GetViewMatrix() const;
    [[nodiscard]] Matrix4x4 GetProjectionMatrix() const { return camera.GetProjectionMatrix(); }
    [[nodiscard]] Vector3f GetViewDirection() const;
};

#endif //GAME_OBJECT_H
