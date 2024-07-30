#ifndef TGA_HANDLER_H
#define TGA_HANDLER_H

#include <cstdint>
#include <string>
#include "buffer.h"

#define RLE_MAX_CHUNK_LENGTH 128

#pragma pack(push, 1)
/**
 * @brief Standard TGA header
 */
struct TGAHeader {
    std::uint8_t  id_length = 0;
    std::uint8_t  color_map_type = 0;
    std::uint8_t  data_type_code = 0;
    std::uint16_t color_map_origin = 0;
    std::uint16_t color_map_length = 0;
    std::uint8_t  color_map_depth = 0;
    std::uint16_t x_origin = 0;
    std::uint16_t y_origin = 0;
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::uint8_t  bits_per_pixel = 0;
    std::uint8_t  image_descriptor = 0;
};
#pragma pack(pop)

/**
 * @brief .tga file handler, including rle encoding/decoding
 */
class TGAHandler {
public:
    TGAHandler() = delete;

    static std::unique_ptr<ColorBuffer> ReadTGAFile(const std::string &filename);
    static bool WriteTGAFile(const std::string &filename, int width, int height, std::uint8_t bpp, const std::uint8_t *data, bool v_flip = false, bool rle = true);
private:
    static bool LoadRLE(std::ifstream &in, ColorBuffer &buffer);
    static bool UnloadRLE(std::ofstream &out, int width, int height, std::uint8_t bpp, const std::uint8_t *data);
};

#endif //TGA_HANDLER_H
