#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <utility>
#include "model.h"
#include "maths/maths.h"

class Component {
public:
    Component() = default;
    explicit Component(std::string name) : name_(std::move(name)) { }
    virtual ~Component() = default;
    [[nodiscard]] std::string name() const { return name_; }
private:
    std::string name_;
};

class Transform final : Component {
public:
    Transform() : Component("Transform") { }
    [[nodiscard]] Matrix4x4 GetModelMatrix() const;
    Vector3f position {0, 0, 0};
    Vector3f rotation {0, 0, 0};
    Vector3f scale {1, 1, 1};
};

class Mesh final : Component {
public:
    Mesh() : Component("Mesh"), model_(nullptr) { }
    Mesh(const Mesh& other) : Component("Mesh"), model_(other.model_) { }
    explicit Mesh(const std::shared_ptr<Model>& model) : Component("Mesh"), model_(model) { }
    explicit Mesh(const std::string& filename) : Component("Mesh"), model_(std::make_shared<Model>(filename)) { }

    void SetModel(const std::shared_ptr<Model>& model) { model_ = model; }

    [[nodiscard]] Model& model() { return *model_; }
    [[nodiscard]] const Model& model() const { return *model_; }

private:
    std::shared_ptr<Model> model_;
};

struct GameObject {
    Transform transform;
    [[nodiscard]] Matrix4x4 GetModelMatrix() const { return transform.GetModelMatrix(); }
};

struct MeshObject : GameObject {
    Mesh mesh;
    MeshObject() : mesh(Mesh()) { }
    explicit MeshObject(const std::shared_ptr<Model>& model) : mesh(model) { }
    explicit MeshObject(const std::string& filename) : mesh(filename) { }
};

#endif //GAME_OBJECT_H
