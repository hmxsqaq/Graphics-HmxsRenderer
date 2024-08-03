#include "component-gameobject.h"

#include <utility/log.h>

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

Matrix4x4 Camera::GetProjectionMatrix() const {
    const Matrix4x4 p2o {
        {z_near, 0, 0, 0},
        {0, z_near, 0, 0},
        {0, 0, z_near + z_far, -z_near * z_far},
        {0, 0, 1, 0}
    };

    const float angle = fov / 180.0f * static_cast<float>(M_PI);
    const float t = std::tan(angle / 2) * z_near;
    const float b = -t;
    const float r = t * aspect_ratio;
    const float l = -r;
    const Matrix4x4 o2c {
        {2 / (r - l), 0, 0, -(r + l) / (r - l)},
        {0, 2 / (t - b), 0, -(t + b) / (t - b)},
        {0, 0, 2 / (z_near - z_far), -(z_near + z_far) / (z_near - z_far)},
        {0, 0, 0, 1}
    };

    return o2c * p2o;
}

Matrix4x4 CameraObject::GetViewMatrix() const {
    Vector3f forward = GetViewDirection();
    Vector3f up = {0, 1, 0};
    Vector3f right = up.Cross(forward).Normalize();

    const Matrix4x4 translate_mat {
        {1, 0, 0, -transform.position[0]},
        {0, 1, 0, -transform.position[1]},
        {0, 0, 1, -transform.position[2]},
        {0, 0, 0, 1}
    };

    const Matrix4x4 rotate_mat {
        {right[0], right[1], right[2], 0},
        {up[0], up[1], up[2], 0},
        {-forward[0], -forward[1], -forward[2], 0},
        {0, 0, 0, 1}
    };

    return translate_mat * rotate_mat;
}

Vector3f CameraObject::GetViewDirection() const {
    const Vector3f rotation = transform.rotation * M_PI / 180.0f;
    const Matrix3x3 rotate_x {
        {1, 0, 0},
        {0, std::cos(rotation[0]), -std::sin(rotation[0])},
        {0, std::sin(rotation[0]), std::cos(rotation[0])}
    };
    const Matrix3x3 rotate_y {
        {std::cos(rotation[1]), 0, std::sin(rotation[1])},
        {0, 1, 0},
        {-std::sin(rotation[1]), 0, std::cos(rotation[1])}
    };
    const Matrix3x3 rotate_z {
        {std::cos(rotation[2]), -std::sin(rotation[2]), 0},
        {std::sin(rotation[2]), std::cos(rotation[2]), 0},
        {0, 0, 1}
    };
    const Matrix3x3 rotation_mat = rotate_z * rotate_x * rotate_y;
    return (rotation_mat * Vector3f {0, 0, -1}).Normalize();
}
