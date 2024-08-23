#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "utility/log.h"
#include "tga_handler.h"

Model::Model(const std::string &filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vector3f vert;
            for (int i = 0; i < 3; i++) iss >> vert[i];
            vertices_.push_back(vert);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vector2f uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            tex_coords_.push_back({uv[0], 1 - uv[1]});
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vector3f norm;
            for (int i = 0; i < 3; i++) iss >> norm[i];
            normals_.push_back(norm.Normalize());
        }  else if (!line.compare(0, 2, "f ")) {
            int f, t, n;
            iss >> trash;
            int counter = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                vertex_indices_.push_back(--f);
                tex_coord_indices_.push_back(--t);
                normal_indices_.push_back(--n);
                counter++;
            }
            if (counter != 3) {
                LOG_ERROR("Model - the model file is supposed to be triangulated");
                return;
            }
        }
    }
    diffuse_map_ = LoadTGAImage(filename, "_diffuse.tga");
    specular_map_ = LoadTGAImage(filename, "_spec.tga");
    normal_map_ = LoadTGAImage(filename, "_nm.tga");
    normal_map_tangent_ = LoadTGAImage(filename, "_nm_tangent.tga");
    LOG_INFO("model:" + filename + " load success");
    LOG_INFO("v-" + std::to_string(vertices_size()) + " f-" + std::to_string(faces_size()) + " vt-" + std::to_string(tex_coords_.size()) + " vn-" + std::to_string(normals_.size()));
    LOG_INFO("diffuse_map:        " + std::to_string(diffuse_map_->width()) + " x " + std::to_string(diffuse_map_->height()) + " / " + std::to_string(diffuse_map_->bpp() * 8));
    LOG_INFO("specular_map:       " + std::to_string(specular_map_->width()) + " x " + std::to_string(specular_map_->height()) + " / " + std::to_string(specular_map_->bpp() * 8));
    LOG_INFO("normal_map:         " + std::to_string(normal_map_->width()) + " x " + std::to_string(normal_map_->height()) + " / " + std::to_string(normal_map_->bpp() * 8));
    LOG_INFO("normal_map_tangent: " + std::to_string(normal_map_tangent_->width()) + " x " + std::to_string(normal_map_tangent_->height()) + " / " + std::to_string(normal_map_tangent_->bpp() * 8));
}

Vector3f Model::normal(const Vector2f &uvf) const {
    Color color = normal_map_->GetPixel(
        static_cast<size_t>(uvf[0] * static_cast<float>(normal_map_tangent_->width())),
        static_cast<size_t>(uvf[1] * static_cast<float>(normal_map_tangent_->height())));
    return Vector3f{static_cast<float>(color[2]), static_cast<float>(color[1]), static_cast<float>(color[0])} * 2.0 / 255.0 - Vector3f{1, 1, 1}; // mappped from [0, 255] to [-1, 1]
}

Vector3f Model::normal_tangent(const Vector2f &uvf) const {
    Color color = normal_map_tangent_->GetPixel(
        static_cast<size_t>(uvf[0] * static_cast<float>(normal_map_tangent_->width())),
        static_cast<size_t>(uvf[1] * static_cast<float>(normal_map_tangent_->height())));
    return Vector3f{static_cast<float>(color[2]), static_cast<float>(color[1]), static_cast<float>(color[0])} * 2.0 / 255.0 - Vector3f{1, 1, 1}; // mappped from [0, 255] to [-1, 1]
}

std::unique_ptr<ColorBuffer> Model::LoadTGAImage(const std::string &filename, const std::string &suffix) {
    const size_t dot = filename.find_last_of('.');
    if (dot == std::string::npos) return nullptr;
    const std::string texture_file_name = filename.substr(0, dot) + suffix;
    return TGAHandler::ReadTGAFile(texture_file_name);
}
