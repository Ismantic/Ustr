#include "ustr.h"

#include "test.h"

namespace ustr {

TEST(UstrTest, UTF8RoundTrip) {
    const std::string input = "A你😀";
    const UnicodeText text = DecodeUTF8(input);

    EXPECT_EQ(3U, text.size());
    EXPECT_EQ(0x41U, text[0]);
    EXPECT_EQ(0x4F60U, text[1]);
    EXPECT_EQ(0x1F600U, text[2]);
    EXPECT_EQ(input, EncodeUTF8(text));
}

TEST(UstrTest, StructuralValidation) {
    EXPECT_TRUE(IsStructurallyValid("A你😀"));
    EXPECT_FALSE(IsStructurallyValid(std::string("\xC0\x80", 2)));
    EXPECT_FALSE(IsStructurallyValid(std::string("\xE4\xBD", 2)));
}

TEST(UstrTest, OneCharLength) {
    EXPECT_EQ(1U, OneCharLen("A"));
    EXPECT_EQ(3U, OneCharLen("你"));
    EXPECT_EQ(4U, OneCharLen("😀"));
}

} // namespace ustr
