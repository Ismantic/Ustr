#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace ustr {

using UnicodeText = std::vector<uint32_t>;

// Return the expected length of a UTF-8 sequence from its leading byte.
// This does not validate the complete sequence.
inline size_t OneCharLen(const char *src) {
    return "\1\1\1\1\1\1\1\1\1\1\1\1\2\2\3\4"
        [(*src & 0xFF) >> 4];
}

inline bool IsTrailByte(char x) {
    return static_cast<signed char>(x) < -0x40;
}

bool IsStructurallyValid(std::string_view str);

// Decode one UTF-8 character and store the number of consumed bytes in mblen.
uint32_t DecodeOneUTF8(const char *begin, const char *end, size_t *mblen);

inline uint32_t DecodeOneUTF8(std::string_view input, size_t *mblen) {
    return DecodeOneUTF8(input.data(), input.data() + input.size(), mblen);
}

// Encode one Unicode code point into output and return the byte count.
size_t EncodeOneUTF8(uint32_t c, char *output);

UnicodeText DecodeUTF8(std::string_view utf8);

std::string EncodeUTF8(const UnicodeText &utext);

} // namespace ustr
