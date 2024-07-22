#include "game_object.h"

Matrix4x4 Transform::GetModelMatrix() const {
    const Vector3f radian = rotation * M_PI / 180.0;
    const Matrix4x4 rotate_x {
        {1, 0, 0, 0},
        {0, std::cos(radian[0]), -std::sin(radian[0]), 0},
        {0, std::sin(radian[0]), std::cos(radian[0]), 0},
        {0, 0, 0, 1}
    };
    const Matrix4x4 rotate_y {
        {std::cos(radian[1]), 0, std::sin(radian[1]), 0},
        {0, 1, 0, 0},
        {-std::sin(radian[1]), 0, std::cos(radian[1]), 0},
        {0, 0, 0, 1}
    };
    const Matrix4x4 rotate_z {
        {std::cos(radian[2]), -std::sin(radian[2]), 0, 0},
        {std::sin(radian[2]), std::cos(radian[2]), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    const Matrix4x4 rotation_mat = rotate_z * rotate_x * rotate_y;
    const Matrix4x4 scale_mat {
        {scale[0], 0, 0, 0},
        {0, scale[1], 0, 0},
        {0, 0, scale[2], 0},
        {0, 0, 0, 1}
    };
    const Matrix4x4 translate_mat {
        {1, 0, 0, position[0]},
        {0, 1, 0, position[1]},
        {0, 0, 1, position[2]},
        {0, 0, 0, 1}
    };

    return translate_mat * rotation_mat * scale_mat;
}
