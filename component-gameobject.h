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

    [[nodiscard]] Model& model() { return *model_; }
    [[nodiscard]] const Model& model() const { return *model_; }

private:
    std::shared_ptr<Model> model_;
};

class Camera : public Component {
public:
    Camera()    : Component("Camera") { }
    explicit Camera(const float fov, const float aspect_ratio, const float z_near, const float z_far)
                : Component("Camera"), fov(fov), aspect_ratio(aspect_ratio), z_near(z_near), z_far(z_far) { }

    [[nodiscard]] Matrix4x4 GetProjectionMatrix() const;

    float fov {45.0f};
    float aspect_ratio {1.0f};
    float z_near {0.1f};
    float z_far {1000.0f};
};

struct GameObject {
    Transform transform;

    [[nodiscard]] Matrix4x4 GetModelMatrix() const { return transform.GetModelMatrix(); }
};

struct MeshObject : GameObject {
    std::shared_ptr<Mesh> mesh;

    MeshObject() : mesh(std::make_shared<Mesh>()) { }
    explicit MeshObject(const std::shared_ptr<Model>& model) : mesh(std::make_shared<Mesh>(model)) { }
    explicit MeshObject(const std::string& filename) : mesh(std::make_shared<Mesh>(filename)) { }
};

struct CameraObject : GameObject {
    Camera camera;

    CameraObject() : camera(Camera()) { }
    explicit CameraObject(const float fov, const float aspect_ratio, const float z_near, const float z_far)
        : camera(fov, aspect_ratio, z_near, z_far) { }

    [[nodiscard]] Matrix4x4 GetViewMatrix() const;
    [[nodiscard]] Matrix4x4 GetProjectionMatrix() const { return camera.GetProjectionMatrix(); }
};

#endif //GAME_OBJECT_H
