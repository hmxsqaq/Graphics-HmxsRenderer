#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include "buffer.h"
#include "maths/vector.h"

class Model {
public:
    Model() = delete;
    explicit Model(const std::string &filename);

    [[nodiscard]] const ColorBuffer* diffuse_map() const { return diffuse_map_.get(); }
    [[nodiscard]] const ColorBuffer* normal_map() const { return normal_map_.get(); }
    [[nodiscard]] const ColorBuffer* specular_map() const { return specular_map_.get(); }
    [[nodiscard]] size_t vertices_size() const { return vertices_.size(); }
    [[nodiscard]] size_t faces_size() const { return vertex_indices_.size() / 3; }
    [[nodiscard]] Vector3f vertex(const size_t i) const { return vertices_[i]; }
    [[nodiscard]] Vector3f vertex(const size_t face_index, const size_t vertex_index) const { return vertices_[vertex_indices_[face_index * 3 + vertex_index]]; }
    [[nodiscard]] Vector2f uv(const size_t face_index, const size_t vertex_index) const { return tex_coords_[tex_coord_indices_[face_index * 3 + vertex_index]]; }
    [[nodiscard]] Vector3f normal(const size_t face_index, const size_t vertex_index) const { return normals_[normal_indices_[face_index * 3 + vertex_index]]; }
    [[nodiscard]] Vector3f normal(const Vector2f &uvf) const;

private:
    static std::unique_ptr<ColorBuffer> LoadTGAImage(const std::string &filename, const std::string &suffix);

    std::vector<Vector3f> vertices_;
    std::vector<Vector2f> tex_coords_;
    std::vector<Vector3f> normals_;
    std::vector<int> vertex_indices_;
    std::vector<int> tex_coord_indices_;
    std::vector<int> normal_indices_;
    std::unique_ptr<ColorBuffer> diffuse_map_;
    std::unique_ptr<ColorBuffer> normal_map_;
    std::unique_ptr<ColorBuffer> specular_map_;
};

#endif //MODEL_H
