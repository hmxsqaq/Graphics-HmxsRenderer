#include "tga_handler.h"
#include <fstream>
#include "log.h"

std::unique_ptr<ColorBuffer> TGAHandler::ReadTGAFile(const std::string &filename) {
    // open file
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        LOG_ERROR("TGAReader - cannot open file: " + filename);
        return nullptr;
    }

    // read header
    TGAHeader header;
    in.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!in.good()) {
        LOG_ERROR("TGAReader - cannot read the TGA header");
        return nullptr;
    }

    // load width & height & bpp from header
    const auto width = header.width;
    const auto height = header.height;
    const auto bpp = header.bits_per_pixel >> 3; // /= 8
    if (width <= 0 || height <= 0 || (bpp != GRAYSCALE && bpp != RGB && bpp != RGBA)) {
        LOG_ERROR("TGAReader - bad width/height/bpp value: width " + std::to_string(width) + " height " + std::to_string(height) + " bpp " + std::to_string(bpp));
        return nullptr;
    }

    auto buffer = std::make_unique<ColorBuffer>(width, height, bpp);

    // read frame_data
    if (header.data_type_code == 3 || header.data_type_code == 2) {
        in.read(reinterpret_cast<char *>(buffer->data()), width * height * bpp);
        if (!in.good()) {
            LOG_ERROR("TGAReader - cannot read frame data");
            return nullptr;
        }
    } else if (header.data_type_code == 10 || header.data_type_code == 11) {
        if (!LoadRLE(in, *buffer)) {
            LOG_ERROR("TGAReader - cannot load RLE data");
            return nullptr;
        }
    } else {
        LOG_ERROR("TGAReader - unknown file format: " + std::to_string(header.data_type_code));
        return nullptr;
    }

    if (buffer->size() <= 0) {
        LOG_ERROR("TGAReader - buffer size is invalid");
        return nullptr;
    }

    // flip or not
    if (!(header.image_descriptor & 0x20))
        buffer->FilpVertically();
    if (header.image_descriptor & 0x10)
        buffer->FilpHorizontally();
    return buffer;
}

bool TGAHandler::WriteTGAFile(const std::string &filename, const int width, const int height, const std::uint8_t bpp, const std::uint8_t *data, const bool v_flip, const bool rle) {
    constexpr std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};

    // open file
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        LOG_ERROR("TGAWriter - cannot open file: " + filename);
        return false;
    }

    LOG_INFO("TGAWriter - writing: width " + std::to_string(width) + " height " + std::to_string(height) + " bpp " + std::to_string(bpp));

    // prepare header
    TGAHeader header = {};
    header.bits_per_pixel = bpp << 3; // = * 8
    header.width  = width;
    header.height = height;
    header.data_type_code = bpp == ColorType::GRAYSCALE ? (rle ? 11 : 3) : rle ? 10 : 2;
    header.image_descriptor = v_flip ? 0x00 : 0x20; // top-left or bottom-left origin

    // write header
    out.write(reinterpret_cast<const char *>(&header), sizeof(header));
    if (!out.good()) {
        LOG_ERROR("TGAWriter - cannot write the TGA header");
        return false;
    }

    // write img
    if (!rle) {
        out.write(reinterpret_cast<const char *>(data), width * height * bpp);
        if (!out.good()) {
            LOG_ERROR("TGAWriter - cannot write the TGA data");
            return false;
        }
    } else if (!UnloadRLE(out, width, height, bpp, data)) {
        LOG_ERROR("TGAWriter - cannot unload RLE data");
        return false;
    }
    out.write(reinterpret_cast<const char *>(developer_area_ref), sizeof(developer_area_ref));
    if (!out.good()) {
        LOG_ERROR("TGAWriter - cannot write the developer area ref");
        return false;
    }
    out.write(reinterpret_cast<const char *>(extension_area_ref), sizeof(extension_area_ref));
    if (!out.good()) {
        LOG_ERROR("TGAWriter - cannot write the extension area ref");
        return false;
    }

    // write footer
    out.write(reinterpret_cast<const char *>(footer), sizeof(footer));
    if (!out.good()) {
        LOG_ERROR("TGHandler - cannot write the footer");
        return false;
    }

    LOG_INFO("TGAWriter - " + filename + " has been written successfully");
    return true;
}

bool TGAHandler::LoadRLE(std::ifstream &in, ColorBuffer &buffer) {
    const size_t width = buffer.width(), height = buffer.height();
    const std::uint8_t bpp = buffer.bpp();
    const size_t pixels_count = width * height;
    size_t current_pixel = 0;
    size_t current_byte  = 0;
    Color color_buffer;
    do {
        std::uint8_t chunk_header = in.get();
        if (!in.good()) {
            LOG_ERROR("RLE - an error occurred while reading the frame_data");
            return false;
        }
        if (chunk_header < 128) {
            chunk_header++;
            for (int i = 0; i < chunk_header; i++) {
                in.read(reinterpret_cast<char *>(color_buffer.bgra.data()), bpp);
                if (!in.good()) {
                    LOG_ERROR("RLE - an error occurred while reading the header");
                    return false;
                }
                for (int t = 0; t < bpp; t++) buffer[current_byte++] = color_buffer.bgra[t];
                current_pixel++;
                if (current_pixel > pixels_count) {
                    LOG_ERROR("RLE - too many pixels were read");
                    return false;
                }
            }
        } else {
            chunk_header -= 127;
            in.read(reinterpret_cast<char *>(color_buffer.bgra.data()), bpp);
            if (!in.good()) {
                LOG_ERROR("RLE - an error occurred while reading the header");
                return false;
            }
            for (int i=0; i < chunk_header; i++) {
                for (int t = 0; t < bpp; t++) buffer[current_byte++] = color_buffer.bgra[t];
                current_pixel++;
                if (current_pixel > pixels_count) {
                    LOG_ERROR("RLE - too many pixels were read");
                    return false;
                }
            }
        }
    } while (current_pixel < pixels_count);
    return true;
}

bool TGAHandler::UnloadRLE(std::ofstream &out, const int width, const int height, const std::uint8_t bpp, const std::uint8_t *data) {
    const size_t pixels_count = width * height;
    size_t current_pixel = 0;
    while (current_pixel < pixels_count) {
        const size_t chunk_start = current_pixel * bpp;
        size_t current_byte = current_pixel * bpp;
        std::uint8_t run_length = 1;
        bool raw = true;
        while (current_pixel + run_length < pixels_count && run_length < RLE_MAX_CHUNK_LENGTH) {
            bool is_equal = true;
            for (int t = 0; is_equal && t < bpp; t++) is_equal = data[current_byte + t] == data[current_byte + t + bpp];
            current_byte += bpp;
            if (run_length == 1)
                raw = !is_equal;
            if (raw && is_equal) {
                run_length--;
                break;
            }
            if (!raw && !is_equal)
                break;
            run_length++;
        }
        current_pixel += run_length;
        out.put(static_cast<char>(raw ? run_length - 1 : run_length + 127));
        if (!out.good()) {
            LOG_ERROR("RLE - an error occurred while writing the header");
            return false;
        }
        out.write(reinterpret_cast<const char *>(data + chunk_start), raw ? run_length * bpp : bpp);
        if (!out.good()) {
            LOG_ERROR("RLE - an error occurred while writing the frame_data");
            return false;
        }
    }
    return true;
}
