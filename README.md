# i2wC.h

**单头文件 | 高性能 | 压缩/解压/ZIP | C++ 友好 | 可定制 | .neod Mod格式**

**Single-header | High-performance | Compression/Decompression/ZIP | C++ Friendly | Customizable | .neod Mod Format**

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

<!-- NEW -->  
**New:** Full support for **.neod mod format** – a high-performance, hash-indexed binary package for game mods, including metadata, structure tables, block/item tables, entity tables, and recipe tables.

### 许可证 / License

**i2wC is open source but requires attribution.**

This library is free to use for personal, educational, and commercial purposes, subject to the following conditions:

1. **Attribution Required**: Any project using this library must include the following notice in its documentation, about page, or source code:
   > "This product includes i2wC compression library (https://github.com/Moli-i2wC/i2wC)"

2. **Exemption List**: The following individuals/organizations are exempt from the attribution requirement:
   - neod

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
- **.neod mod format**: `NeodBuilder` class creates compressed mod packages with metadata, four hash-indexed tables (structures, blocks/items, entities, recipes), and O(1) lookup by key hash. <!-- NEW -->
- **C++ friendly**: `std::vector`-based APIs, exception-free error handling via `Result` type, and a clean namespace `i2wC`.
- **Cross-platform**: Windows (UTF-8 filenames), Linux, macOS, and embedded environments (disable time/stdio features).

### 快速开始 / Quick Start
