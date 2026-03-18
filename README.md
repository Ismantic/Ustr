# Ustr

Unicode 与 UTF-8 编解码的 C++ 实现，附详细教程。

## 功能

- `IsTrailByte` — 判断 UTF-8 续字节
- `DecodeOneUTF8` / `DecodeUTF8` — UTF-8 字节流 → Unicode 码点
- `EncodeOneUTF8` / `EncodeUTF8` — Unicode 码点 → UTF-8 字节流

支持 1-4 字节编码，覆盖 ASCII、中文、Emoji 等全部 Unicode 范围。

详细的编码原理和实现讲解见 [Ustr.md](Ustr.md)。

## 构建

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## 运行测试

```bash
./build/ustr
```

```
Passed: 14, Failed: 0
```

## License

MIT
