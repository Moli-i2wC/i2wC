# i2wC.h

**单头文件 | 高性能 | 压缩/解压/ZIP | C++ 友好 | 可定制**

**Single-header | High-performance | Compression/Decompression/ZIP | C++ Friendly | Customizable**

[![License](https://img.shields.io/badge/License-Attribution--Required-yellow.svg)](LICENSE)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.cppreference.com/w/cpp/11)
[![Single Header](https://img.shields.io/badge/Single-Header-orange.svg)](#)

[English](#english) | [中文](#中文)

---

## 目录 / Table of Contents

1. [概述 / Overview](#概述--overview)
2. [许可证 / License](#许可证--license)
3. [特性 / Features](#特性--features)
4. [快速开始 / Quick Start](#快速开始--quick-start)
5. [API 概览 / API Overview](#api-概览--api-overview)
6. [功能裁剪宏 / Feature Control Macros](#功能裁剪宏--feature-control-macros)
7. [压缩配置宏 / Compression Configuration Macros](#压缩配置宏--compression-configuration-macros)
8. [安全特性 / Safety Features](#安全特性--safety-features)
9. [编译指南 / Building](#编译指南--building)
10. [下载 / Download](#下载--download)

---

## English

### 概述 / Overview

`i2wC.h` is a self-contained, single-header library providing high-performance deflate compression, decompression, and ZIP archive handling. It features a modern C++ interface with comprehensive safety guards, customizable compression parameters, and a structured binary container format.

### 许可证 / License

**i2wC is open source but requires attribution.**

This library is free to use for personal, educational, and commercial purposes, subject to the following conditions:

1. **Attribution Required**: Any project using this library must include the following notice in its documentation, about page, or source code:
   > "This product includes i2wC compression library (https://github.com/Moli-i2wC/i2wC)"

2. **Exemption List**: The following individuals/organizations are exempt from the attribution requirement:
   - None

3. **Request Exemption**: To request exemption from the attribution requirement, please contact: [your email]

4. **Disclaimer**: THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

### 特性 / Features

- **Single-header**: Just `#include "i2wC.h"` and you're ready to go.
- **No external dependencies**: Uses only standard C/C++ libraries.
- **High performance**: Optimized for x86/x64 with unaligned access, 64-bit bit buffers, and fast lookup tables.
- **Complete ZIP support**: Read/write ZIP archives, including Zip64, data descriptors, and streaming extraction.
- **Safety first**: Built-in protection against ZIP bombs (configurable size limits, expansion tracking, compression ratio checks).
- **Customizable**: Runtime configuration for hash sizes, dictionary sizes, match lengths, probe counts, and more.
- **Binary container**: `BinaryFile` class provides a structured format with magic numbers, versioning, sections, and CRC32 integrity checks.
- **C++ friendly**: `std::vector`-based APIs, exception-free error handling via `Result` type, and a clean namespace `i2wC`.
- **Cross-platform**: Windows (UTF-8 filenames), Linux, macOS, and embedded environments (disable time/stdio features).

### 快速开始 / Quick Start

```
#include "i2wC.h"

int main() {
    // Simple compression
    std::vector<unsigned char> data = { /* your data */ };
    std::vector<unsigned char> compressed = i2wC::compress(data, 6);
    std::vector<unsigned char> decompressed = i2wC::uncompress(compressed);

    // Using Compressor class
    i2wC::Compressor comp(9);
    comp.setAdvancedConfig(&i2wC::AdvancedConfig::defaultConfig().setLZHashBits(12));
    std::vector<unsigned char> result = comp.compress(data);

    // Binary container
    i2wC::BinaryFile bf;
    bf.setIdentifier({0x01, 0x02, 0x03});
    bf.setContent(data);
    bf.addCustomSection({0xFF, 0xEE});
    bf.saveToFile("archive.bin");

    return 0;
}
```

### API 概览 / API Overview

| 组件 / Component | 描述 / Description |
|-----------|-------------|
| `compress()` / `uncompress()` | 操作 `std::vector<unsigned char>` 的高层函数 / High-level functions operating on `std::vector<unsigned char>` |
| `Compressor` | 有状态的压缩器，支持可配置级别和高级参数 / Stateful compressor with configurable level and advanced parameters |
| `BinaryFile` | 结构化容器，包含头部、标识区、内容区、自定义区和尾部 / Structured container with header, identifier, content, custom sections, and footer |
| `Result` / `Error` | 无异常错误处理，支持自定义回调 / Exception-free error handling with customizable callbacks |
| `DecompressConfig` | 解压安全限制（最大尺寸、扩容因子、压缩比）/ Safety limits for decompression (max size, expansion factor, ratio) |
| `AdvancedConfig` | 压缩内部参数的运行时定制 / Runtime customization of compression internals |

### 功能裁剪宏 / Feature Control Macros

在包含 `i2wC.h` 之前定义以下宏，可裁剪不需要的功能，减小最终二进制体积。

Define the following macros **before** including `i2wC.h` to strip unwanted features and reduce binary size.

| 宏 / Macro | 作用 / Purpose |
|--------|------|
| `I2WC_NO_STDIO` | 禁用所有文件 I/O 功能（fopen/fread/fwrite 等）/ Disable all file I/O functions |
| `I2WC_NO_TIME` | 禁用时间相关功能（文件修改时间等）/ Disable time-related functions |
| `I2WC_NO_ARCHIVE_APIS` | 禁用所有 ZIP 归档读写功能 / Disable all ZIP archive read/write functions |
| `I2WC_NO_ARCHIVE_WRITING_APIS` | 仅禁用 ZIP 归档写入功能（保留读取）/ Disable only ZIP writing (reading remains) |
| `I2WC_NO_DEFLATE_APIS` | 禁用压缩功能（tdefl）/ Disable compression (deflate) |
| `I2WC_NO_INFLATE_APIS` | 禁用解压功能（tinfl）/ Disable decompression (inflate) |
| `I2WC_NO_ZLIB_APIS` | 禁用 zlib 风格的流式 API / Disable zlib-style streaming API |
| `I2WC_NO_MALLOC` | 禁用默认内存分配函数（需用户提供自定义分配器）/ Disable default malloc/free (user must provide custom allocator) |
| `I2WC_LESS_MEMORY` | 使用更少内存（牺牲少量压缩速度）/ Use less memory (slightly slower compression) |

推荐的最小化配置 / Recommended minimal configuration:
```
#define I2WC_NO_STDIO
#define I2WC_NO_TIME
#define I2WC_NO_ARCHIVE_APIS
#define I2WC_NO_ZLIB_APIS
#include "i2wC.h"
```

### 压缩配置宏 / Compression Configuration Macros

以下宏可在编译时调整压缩算法的内部参数，以平衡性能、内存和压缩率。

The following macros can be defined at compile time to tune internal compression parameters for performance, memory, or ratio.

| 宏 / Macro | 默认值 / Default | 描述 / Description |
|--------|---------|------|
| `I2WC_LZ_HASH_BITS` | 14 | LZ77 哈希表的位数（大小 = 2^bits）/ LZ77 hash table bits (size = 2^bits) |
| `I2WC_LZ_DICT_SIZE` | 65536 | LZ77 滑动窗口大小 / LZ77 dictionary window size |
| `I2WC_MIN_MATCH_LEN` | 4 | 最小匹配长度 / Minimum match length |
| `I2WC_MAX_MATCH_LEN` | 260 | 最大匹配长度 / Maximum match length |
| `I2WC_LZ_CODE_BUF_SIZE` | 48*1024 | LZ77 代码缓冲区大小 / LZ77 code buffer size |
| `I2WC_OUT_BUF_SIZE` | (I2WC_LZ_CODE_BUF_SIZE * 14)/10 | 输出缓冲区大小 / Output buffer size |
| `I2WC_FAST_LOOKUP_BITS` | 11 | 霍夫曼快速查表位数 / Huffman fast lookup table bits |
| `I2WC_ADLER32_INIT` | 0x2C57 | Adler32 校验初始值 / Adler32 initial value |
| `I2WC_ADLER32_CHUNK` | 4444 | Adler32 计算的分块大小 / Adler32 chunk size |
| `I2WC_MAX_PROBES_LEVEL0` ~ `I2WC_MAX_PROBES_LEVEL10` | 见源码 | 各压缩级别的哈希探测次数 / Hash probes per compression level |
| `I2WC_ZLIB_CMF` | 0x68 | zlib 头部 CMF 字节 / zlib header CMF byte |
| `I2WC_ZIP_MAX_IO_BUF_SIZE` | 128*1024 | ZIP 读写缓冲区大小 / ZIP I/O buffer size |

### 安全特性 / Safety Features

| 防护项 / Guard | 默认值 / Default | 描述 / Description |
|-------|---------|-------------|
| 最大输出尺寸 / Max output size | 100 MB | 防止内存耗尽 / Prevents memory exhaustion |
| 最大扩容次数 / Max expansions | 20 | 限制缓冲区增长迭代 / Limits buffer growth iterations |
| 压缩比上限 / Compression ratio limit | 100:1 | 检测高压缩率炸弹 / Detects highly compressible bombs |
| CRC32 校验 / CRC32 validation | 启用 / Enabled | 确保数据完整性 / Ensures data integrity |

### 编译指南 / Building

```
# 基础编译 / Basic compilation
g++ -std=c++11 -O2 your_file.cpp -o your_program

# 最小化构建（无文件 I/O，无时间函数）/ Minimal build (no file I/O, no time functions)
g++ -std=c++11 -O2 -DI2WC_NO_STDIO -DI2WC_NO_TIME your_file.cpp -o your_program
```

### 下载 / Download

最新版本 / Latest version: [i2wC.h](https://github.com/Moli-i2wC/i2wC/blob/main/i2wC.h)

---

## 中文

### 概述

`i2wC.h` 是一个自包含的单头文件库，提供高性能 deflate 压缩、解压以及 ZIP 归档处理功能。它具有现代化的 C++ 接口、全面的安全防护、可定制的压缩参数，以及一个结构化的二进制容器格式。

### 许可证

**i2wC 可开源使用，但需要声明使用。**

本库可免费用于个人、教育、商业项目，但需遵守以下条件：

1. **署名要求**：任何使用本库的项目，必须在文档、关于页面或源代码中保留以下声明：
   > "This product includes i2wC compression library (https://github.com/Moli-i2wC/i2wC)"

2. **豁免名单**：以下个人/组织可免于署名要求：
   - 暂无

3. **申请豁免**：如需申请豁免署名要求，请联系：[你的邮箱]

4. **免责声明**：本软件按"原样"提供，不提供任何明示或暗示的保证，包括但不限于适销性、特定用途适用性和非侵权性的保证。

### 特性

- **单头文件**：只需 `#include "i2wC.h"` 即可使用。
- **无外部依赖**：仅使用标准 C/C++ 库。
- **高性能**：针对 x86/x64 优化，支持未对齐访问、64 位位缓存和快速查找表。
- **完整的 ZIP 支持**：读写 ZIP 归档，包括 Zip64、数据描述符和流式提取。
- **安全优先**：内置 ZIP 炸弹防护（可配置大小限制、扩容跟踪、压缩比检查）。
- **可定制**：运行时配置哈希大小、字典大小、匹配长度、探测次数等。
- **二进制容器**：`BinaryFile` 类提供结构化格式，包含魔数、版本、分段和 CRC32 完整性校验。
- **C++ 友好**：基于 `std::vector` 的 API，通过 `Result` 类型实现无异常错误处理，命名空间 `i2wC` 清晰隔离。
- **跨平台**：支持 Windows（UTF-8 文件名）、Linux、macOS 及嵌入式环境（可禁用时间/文件功能）。

### 快速开始

```
#include "i2wC.h"

int main() {
    // 简单压缩
    std::vector<unsigned char> data = { /* 你的数据 */ };
    std::vector<unsigned char> compressed = i2wC::compress(data, 6);
    std::vector<unsigned char> decompressed = i2wC::uncompress(compressed);

    // 使用 Compressor 类
    i2wC::Compressor comp(9);
    comp.setAdvancedConfig(&i2wC::AdvancedConfig::defaultConfig().setLZHashBits(12));
    std::vector<unsigned char> result = comp.compress(data);

    // 二进制容器
    i2wC::BinaryFile bf;
    bf.setIdentifier({0x01, 0x02, 0x03});
    bf.setContent(data);
    bf.addCustomSection({0xFF, 0xEE});
    bf.saveToFile("archive.bin");

    return 0;
}
```

### API 概览

| 组件 | 描述 |
|------|------|
| `compress()` / `uncompress()` | 操作 `std::vector<unsigned char>` 的高层函数 |
| `Compressor` | 有状态的压缩器，支持可配置级别和高级参数 |
| `BinaryFile` | 结构化容器，包含头部、标识区、内容区、自定义区和尾部 |
| `Result` / `Error` | 无异常错误处理，支持自定义回调 |
| `DecompressConfig` | 解压安全限制（最大尺寸、扩容因子、压缩比） |
| `AdvancedConfig` | 压缩内部参数的运行时定制 |

### 功能裁剪宏

在包含 `i2wC.h` 之前定义以下宏，可裁剪不需要的功能，减小最终二进制体积。

| 宏 | 作用 |
|--------|------|
| `I2WC_NO_STDIO` | 禁用所有文件 I/O 功能（fopen/fread/fwrite 等） |
| `I2WC_NO_TIME` | 禁用时间相关功能（文件修改时间等） |
| `I2WC_NO_ARCHIVE_APIS` | 禁用所有 ZIP 归档读写功能 |
| `I2WC_NO_ARCHIVE_WRITING_APIS` | 仅禁用 ZIP 归档写入功能（保留读取） |
| `I2WC_NO_DEFLATE_APIS` | 禁用压缩功能（tdefl） |
| `I2WC_NO_INFLATE_APIS` | 禁用解压功能（tinfl） |
| `I2WC_NO_ZLIB_APIS` | 禁用 zlib 风格的流式 API |
| `I2WC_NO_MALLOC` | 禁用默认内存分配函数（需用户提供自定义分配器） |
| `I2WC_LESS_MEMORY` | 使用更少内存（牺牲少量压缩速度） |

推荐的最小化配置：
```
#define I2WC_NO_STDIO
#define I2WC_NO_TIME
#define I2WC_NO_ARCHIVE_APIS
#define I2WC_NO_ZLIB_APIS
#include "i2wC.h"
```

### 压缩配置宏

以下宏可在编译时调整压缩算法的内部参数，以平衡性能、内存和压缩率。

| 宏 | 默认值 | 描述 |
|--------|---------|------|
| `I2WC_LZ_HASH_BITS` | 14 | LZ77 哈希表的位数（大小 = 2^bits） |
| `I2WC_LZ_DICT_SIZE` | 65536 | LZ77 滑动窗口大小 |
| `I2WC_MIN_MATCH_LEN` | 4 | 最小匹配长度 |
| `I2WC_MAX_MATCH_LEN` | 260 | 最大匹配长度 |
| `I2WC_LZ_CODE_BUF_SIZE` | 48*1024 | LZ77 代码缓冲区大小 |
| `I2WC_OUT_BUF_SIZE` | (I2WC_LZ_CODE_BUF_SIZE * 14)/10 | 输出缓冲区大小 |
| `I2WC_FAST_LOOKUP_BITS` | 11 | 霍夫曼快速查表位数 |
| `I2WC_ADLER32_INIT` | 0x2C57 | Adler32 校验初始值 |
| `I2WC_ADLER32_CHUNK` | 4444 | Adler32 计算的分块大小 |
| `I2WC_MAX_PROBES_LEVEL0` ~ `I2WC_MAX_PROBES_LEVEL10` | 见源码 | 各压缩级别的哈希探测次数 |
| `I2WC_ZLIB_CMF` | 0x68 | zlib 头部 CMF 字节 |
| `I2WC_ZIP_MAX_IO_BUF_SIZE` | 128*1024 | ZIP 读写缓冲区大小 |

### 安全特性

| 防护项 | 默认值 | 描述 |
|--------|--------|------|
| 最大输出尺寸 | 100 MB | 防止内存耗尽 |
| 最大扩容次数 | 20 | 限制缓冲区增长迭代 |
| 压缩比上限 | 100:1 | 检测高压缩率炸弹 |
| CRC32 校验 | 启用 | 确保数据完整性 |

### 编译指南

```
# 基础编译
g++ -std=c++11 -O2 your_file.cpp -o your_program

# 最小化构建（无文件 I/O，无时间函数）
g++ -std=c++11 -O2 -DI2WC_NO_STDIO -DI2WC_NO_TIME your_file.cpp -o your_program
```

### 下载

最新版本：[i2wC.h](https://github.com/Moli-i2wC/i2wC/blob/main/i2wC.h)

---

**Made with ❤️ for performance and simplicity.**
