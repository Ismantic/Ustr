# Ustr

Ustr 是一个小型 C++17 Unicode 工具库，提供 UTF-8 编解码、结构校验和 Unicode Script 查询。

## 构建与测试

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## 接口

核心接口位于 `src/ustr.h`：

```cpp
ustr::UnicodeText codepoints = ustr::DecodeUTF8("A你😀");
std::string text = ustr::EncodeUTF8(codepoints);
bool valid = ustr::IsStructurallyValid(text);
```

`DecodeOneUTF8` 和 `EncodeOneUTF8` 用于处理单个码点；`unicode_script.h` 提供字符所属 Script 的查询能力。

## 原理文档

UTF-8 的编码格式、合法性检查和编解码实现见《底层实现：文本处理》的
[Unicode 与 UTF-8](https://ismantic.github.io/text/unicode-and-utf8.html)。

## License

[MIT](LICENSE)
