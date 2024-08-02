#ifndef MATHS_H
#define MATHS_H

#include "vector.h"
#include "matrix.h"

template<typename T0, typename T1>
T0 Interpolate(const T0 &v1, const T0 &v2, const T0 &v3, const Vector<T1, 3> &bc, const float weight = 1.0f) {
    return (v1 * bc[0] + v2 * bc[1] + v3 * bc[2]) / weight;
}

template<typename T0, typename T1>
T0 Interpolate(const T0 &v1, const T0 &v2, const T0 &v3, const std::array<T1, 3> &bc, const float weight = 1.0f) {
    return (v1 * bc[0] + v2 * bc[1] + v3 * bc[2]) / weight;
}



#endif //MATHS_H
