#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <cstdint>
#include <cassert>
#include <ostream>

struct Color {
    Color() : bgra{0, 0, 0, 255} {}
    Color(const std::uint8_t b, const std::uint8_t g, const std::uint8_t r, const std::uint8_t a) : bgra{b, g, r, a} {}

    std::uint8_t& operator[](const int index)       { assert(index >= 0 && index < 4); return bgra[index]; }
    std::uint8_t  operator[](const int index) const { assert(index >= 0 && index < 4); return bgra[index]; }
    [[nodiscard]] std::uint8_t b() const { return bgra[0]; }
    [[nodiscard]] std::uint8_t g() const { return bgra[1]; }
    [[nodiscard]] std::uint8_t r() const { return bgra[2]; }
    [[nodiscard]] std::uint8_t a() const { return bgra[3]; }

    std::array<std::uint8_t, 4> bgra;
};

inline std::ostream& operator<<(std::ostream &out, const Color &color) {
    out << "B:" << static_cast<int>(color.b()) << " "
        << "G:" << static_cast<int>(color.g()) << " "
        << "R:" << static_cast<int>(color.r()) << " "
        << "A:" << static_cast<int>(color.a());
    return out;
}

#endif //COLOR_H
