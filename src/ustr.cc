#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdint>

namespace ustr {

void PrintBinary(uint32_t num) {
    std::cout << "Number: " << std::dec << num
              << "  Hex: 0x" << std::hex << std::uppercase << num
              << "  Binary: " << std::bitset<8>(num)
              << std::dec << std::endl;
}

bool IsTrailByte(uint8_t x) {
    return (x & 0xC0) == 0x80;
}

uint32_t DecodeOneUTF8(const std::string& str, size_t* bytes) {
    if (str.empty()) {
        *bytes = 0;
        return 0;
    }

    const uint8_t* data = reinterpret_cast<const uint8_t*>(str.data());
    const size_t size = str.size();

    // 1字节：0xxxxxxx
    if (data[0] < 0x80) {
        *bytes = 1;
        return data[0];
    }

    // 2字节：110xxxxx 10xxxxxx
    if ((data[0] & 0xE0) == 0xC0 && size >= 2 &&
        IsTrailByte(data[1])) {
        *bytes = 2;
        return ((data[0] & 0x1F) << 6) |
               (data[1] & 0x3F);
    }

    // 3字节：1110xxxx 10xxxxxx 10xxxxxx
    if ((data[0] & 0xF0) == 0xE0 && size >= 3 &&
        IsTrailByte(data[1]) && IsTrailByte(data[2])) {
        *bytes = 3;
        return ((data[0] & 0x0F) << 12) |
               ((data[1] & 0x3F) << 6) |
               (data[2] & 0x3F);
    }

    // 4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    if ((data[0] & 0xF8) == 0xF0 && size >= 4 &&
        IsTrailByte(data[1]) && IsTrailByte(data[2]) && IsTrailByte(data[3])) {
        *bytes = 4;
        return ((data[0] & 0x07) << 18) |
               ((data[1] & 0x3F) << 12) |
               ((data[2] & 0x3F) << 6) |
               (data[3] & 0x3F);
    }

    *bytes = 0;
    return 0;
}

std::vector<uint32_t> DecodeUTF8(const std::string& str) {
    std::vector<uint32_t> codepoints;
    size_t pos = 0;

    while (pos < str.size()) {
        size_t bytes;
        uint32_t cp = DecodeOneUTF8(str.substr(pos), &bytes);
        if (bytes == 0) break;
        codepoints.push_back(cp);
        pos += bytes;
    }

    return codepoints;
}

size_t EncodeOneUTF8(uint32_t c, char* output) {
    if (c <= 0x7F) {
        *output = static_cast<char>(c);
        return 1;
    }
    if (c <= 0x7FF) {
        output[1] = 0x80 | (c & 0x3F);
        c >>= 6;
        output[0] = 0xC0 | c;
        return 2;
    }
    if (c <= 0xFFFF) {
        output[2] = 0x80 | (c & 0x3F);
        c >>= 6;
        output[1] = 0x80 | (c & 0x3F);
        c >>= 6;
        output[0] = 0xE0 | c;
        return 3;
    }
    output[3] = 0x80 | (c & 0x3F);
    c >>= 6;
    output[2] = 0x80 | (c & 0x3F);
    c >>= 6;
    output[1] = 0x80 | (c & 0x3F);
    c >>= 6;
    output[0] = 0xF0 | c;
    return 4;
}

std::string EncodeUTF8(const std::vector<uint32_t>& codepoints) {
    std::string result;
    for (uint32_t cp : codepoints) {
        char buffer[4];
        size_t bytes = EncodeOneUTF8(cp, buffer);
        result.append(buffer, bytes);
    }
    return result;
}

} // namespace ustr

// --- tests ---

int passed = 0;
int failed = 0;

void check(bool cond, const std::string& name) {
    if (cond) {
        passed++;
    } else {
        failed++;
        std::cout << "FAIL: " << name << std::endl;
    }
}

void test_ascii() {
    std::string s = "Hello";
    auto cps = ustr::DecodeUTF8(s);
    check(cps.size() == 5, "ascii length");
    check(cps[0] == 'H' && cps[4] == 'o', "ascii values");
    check(ustr::EncodeUTF8(cps) == s, "ascii roundtrip");
}

void test_chinese() {
    std::string s = "你好";
    auto cps = ustr::DecodeUTF8(s);
    check(cps.size() == 2, "chinese length");
    check(cps[0] == 0x4F60, "你 = U+4F60");
    check(cps[1] == 0x597D, "好 = U+597D");
    check(ustr::EncodeUTF8(cps) == s, "chinese roundtrip");
}

void test_emoji() {
    std::string s = "🌟";
    auto cps = ustr::DecodeUTF8(s);
    check(cps.size() == 1, "emoji length");
    check(cps[0] == 0x1F31F, "🌟 = U+1F31F");
    check(ustr::EncodeUTF8(cps) == s, "emoji roundtrip");
}

void test_mixed() {
    std::string s = "Hi你好🌍";
    auto cps = ustr::DecodeUTF8(s);
    check(cps.size() == 5, "mixed length");
    check(cps[0] == 'H', "mixed ascii");
    check(cps[2] == 0x4F60, "mixed chinese");
    check(cps[4] == 0x1F30D, "mixed emoji");
    check(ustr::EncodeUTF8(cps) == s, "mixed roundtrip");
}

void test_trail_byte() {
    check(ustr::IsTrailByte(0x80), "0x80 is trail");
    check(ustr::IsTrailByte(0xBF), "0xBF is trail");
    check(!ustr::IsTrailByte(0x7F), "0x7F not trail");
    check(!ustr::IsTrailByte(0xC0), "0xC0 not trail");
}

int main() {
    test_trail_byte();
    test_ascii();
    test_chinese();
    test_emoji();
    test_mixed();

    std::cout << "\nPassed: " << passed << ", Failed: " << failed << std::endl;
    return failed > 0 ? 1 : 0;
}
