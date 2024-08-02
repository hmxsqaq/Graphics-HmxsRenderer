#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <cstdint>
#include <cassert>
#include <ostream>
#include <algorithm>
#include <maths/vector.h>

/**
 * @brief basic ldr color struct using 4 bytes to store B, G, R, A.
 */
struct Color {
    Color() : bgra_array{0, 0, 0, 255} {}
    Color(const uint8_t b, const uint8_t g, const uint8_t r, const uint8_t a) : bgra_array{b, g, r, a} {}
    explicit Color(const std::array<uint8_t, 4>& array) : bgra_array(array) {}
    explicit Color(const std::array<uint8_t, 3>& array) : bgra_array{array[0], array[1], array[2], 255} {}
    explicit Color(const Vector3f& vec) : bgra_array{static_cast<uint8_t>(std::clamp(static_cast<int>(vec[0] * 255.0f), 0, 255)),
                                                     static_cast<uint8_t>(std::clamp(static_cast<int>(vec[1] * 255.0f), 0, 255)),
                                                     static_cast<uint8_t>(std::clamp(static_cast<int>(vec[2] * 255.0f), 0, 255)),
                                                     255} {}

    uint8_t& operator[](const int index)       { assert(index >= 0 && index < 4); return bgra_array[index]; }
    uint8_t  operator[](const int index) const { assert(index >= 0 && index < 4); return bgra_array[index]; }

    template <typename T>
    Color operator*(const T scalar) const {
        return {static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[0] * scalar), 0, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[1] * scalar), 0, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[2] * scalar), 0, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[3] * scalar), 0, 255))};
    }

    template <typename T>
    Color operator/(const T scalar) const {
        assert(scalar != 0);
        return {static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[0] / scalar), 0, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[1] / scalar), 0, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[2] / scalar), 0, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(bgra_array[3] / scalar), 0, 255))};
    }

    Color operator+(const Color& other) const {
        return {static_cast<uint8_t>(std::clamp(bgra_array[0] + other[0], 0, 255)),
                static_cast<uint8_t>(std::clamp(bgra_array[1] + other[1], 0, 255)),
                static_cast<uint8_t>(std::clamp(bgra_array[2] + other[2], 0, 255)),
                static_cast<uint8_t>(std::clamp(bgra_array[3] + other[3], 0, 255))};
    }

    Color operator-(const Color& other) const {
        return {static_cast<uint8_t>(std::clamp(bgra_array[0] - other[0], 0, 255)),
                static_cast<uint8_t>(std::clamp(bgra_array[1] - other[1], 0, 255)),
                static_cast<uint8_t>(std::clamp(bgra_array[2] - other[2], 0, 255)),
                static_cast<uint8_t>(std::clamp(bgra_array[3] - other[3], 0, 255))};
    }

    [[nodiscard]] uint8_t B() const { return bgra_array[0]; }
    [[nodiscard]] uint8_t G() const { return bgra_array[1]; }
    [[nodiscard]] uint8_t R() const { return bgra_array[2]; }
    [[nodiscard]] uint8_t A() const { return bgra_array[3]; }

    [[nodiscard]] Vector3f ToRGB() const {
        return {static_cast<float>(R()) / 255.0f, static_cast<float>(G()) / 255.0f, static_cast<float>(B()) / 255.0f};
    }

    std::array<std::uint8_t, 4> bgra_array;

    static Color Black() { return {0, 0, 0, 255}; }
    static Color White() { return {255, 255, 255, 255}; }
    static Color Red()   { return {0, 0, 255, 255}; }
    static Color Green() { return {0, 255, 0, 255}; }
    static Color Blue()  { return {255, 0, 0, 255}; }
};

inline std::ostream& operator<<(std::ostream &out, const Color &color) {
    out << "C["
        << static_cast<int>(color.B()) << ", "
        << static_cast<int>(color.G()) << ", "
        << static_cast<int>(color.R()) << ", "
        << static_cast<int>(color.A()) << "]";
    return out;
}

#endif //COLOR_H
