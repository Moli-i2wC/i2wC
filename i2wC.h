/*
 * i2wC.h - Single-header compression library
 * 
 * Copyright (c) 2024 [Your Name]
 * 
 * 使用条款 / Terms of Use:
 * 
 * 1. 免费使用 / Free to Use
 *    本库可免费用于个人、教育、商业项目。
 *    This library is free to use for personal, educational, and commercial projects.
 * 
 * 2. 署名要求 / Attribution Required
 *    在任何使用本库的项目中，必须在文档、关于页面或源代码中保留以下声明：
 *    "This product includes i2wC compression library (https://github.com/yourusername/i2wC)"
 *    
 *    Attribution must be retained in documentation, about page, or source code.
 * 
 * 3. 豁免名单 / Exemption List
 *    
 *    请在github发布页中查看list.md 
 * 
 * 4. 免责声明 / Disclaimer
 *    THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <ios>
#include <cstdio>

#if defined(__STRICT_ANSI__)
#define IW_FORCEINLINE
#elif defined(_MSC_VER)
#define IW_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#define IW_FORCEINLINE __inline__ __attribute__((__always_inline__))
#else
#define IW_FORCEINLINE inline
#endif

#ifdef i2WC_NO_INFLATE_APIS
#define i2WC_NO_ARCHIVE_APIS
#endif

#ifdef i2WC_NO_DEFLATE_APIS
#define i2WC_NO_ARCHIVE_WRITING_APIS
#endif

#if defined(__TINYC__) && (defined(__linux) || defined(__linux__))
#define i2WC_NO_TIME
#endif

#if !defined(i2WC_NO_TIME) && !defined(i2WC_NO_ARCHIVE_APIS)
#include <time.h>
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i486) || defined(i386) || defined(__ia64__) || defined(__x86_64__)

#define i2WC_X86_OR_X64_CPU 1
#else
#define i2WC_X86_OR_X64_CPU 0
#endif


#if !defined(i2WC_LITTLE_ENDIAN)
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define i2WC_LITTLE_ENDIAN 1
#else
#define i2WC_LITTLE_ENDIAN 0
#endif

#else

#if i2WC_X86_OR_X64_CPU
#define i2WC_LITTLE_ENDIAN 1
#else
#define i2WC_LITTLE_ENDIAN 0
#endif

#endif
#endif

#if defined(__has_feature)
#if __has_feature(undefined_behavior_sanitizer)
#define i2WC_USE_UNALIGNED_LOADS_AND_STORES 0
#endif
#endif

#if !defined(i2WC_USE_UNALIGNED_LOADS_AND_STORES)
#if i2WC_X86_OR_X64_CPU
#define i2WC_USE_UNALIGNED_LOADS_AND_STORES 0
#define i2WC_UNALIGNED_USE_MEMCPY
#else
#define i2WC_USE_UNALIGNED_LOADS_AND_STORES 0
#endif
#endif

#if defined(_M_X64) || defined(_WIN64) || defined(__MINGW64__) || defined(_LP64) || defined(__LP64__) || defined(__ia64__) || defined(__x86_64__)
#define i2WC_HAS_64BIT_REGISTERS 1
#else
#define i2WC_HAS_64BIT_REGISTERS 0
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	
	typedef unsigned long iw_ulong;
	
	void iw_free(void *p);
	
#define IW_ADLER32_INIT (1)
	iw_ulong iw_adler32(iw_ulong adler, const unsigned char *ptr, size_t buf_len);
	
#define IW_CRC32_INIT (0)
	iw_ulong iw_crc32(iw_ulong crc, const unsigned char *ptr, size_t buf_len);
	
	enum
	{
		IW_DEFAULT_STRATEGY = 0,
		IW_FILTERED = 1,
		IW_HUFFMAN_ONLY = 2,
		IW_RLE = 3,
		IW_FIXED = 4
	};
	
#define IW_DEFLATED 8
	
	typedef void *(*iw_alloc_func)(void *opaque, size_t items, size_t size);
	typedef void (*iw_free_func)(void *opaque, void *address);
	typedef void *(*iw_realloc_func)(void *opaque, void *address, size_t items, size_t size);
	
	enum
	{
		IW_NO_COMPRESSION = 0,
		IW_BEST_SPEED = 1,
		IW_BEST_COMPRESSION = 9,
		IW_UBER_COMPRESSION = 10,
		IW_DEFAULT_LEVEL = 6,
		IW_DEFAULT_COMPRESSION = -1
	};
	
#define IW_VERSION "11.3.1"
#define IW_VERNUM 0xB301
#define IW_VER_MAJOR 11
#define IW_VER_MINOR 3
#define IW_VER_REVISION 1
#define IW_VER_SUBREVISION 0
	
#ifndef i2WC_NO_ZLIB_APIS
	
	enum
	{
		IW_NO_FLUSH = 0,
		IW_PARTIAL_FLUSH = 1,
		IW_SYNC_FLUSH = 2,
		IW_FULL_FLUSH = 3,
		IW_FINISH = 4,
		IW_BLOCK = 5
	};
	
	enum
	{
		IW_OK = 0,
		IW_STREAM_END = 1,
		IW_NEED_DICT = 2,
		IW_ERRNO = -1,
		IW_STREAM_ERROR = -2,
		IW_DATA_ERROR = -3,
		IW_MEM_ERROR = -4,
		IW_BUF_ERROR = -5,
		IW_VERSION_ERROR = -6,
		IW_PARAM_ERROR = -10000
	};
	
#define IW_DEFAULT_WINDOW_BITS 15
	
	struct iw_internal_state;
	
	typedef struct iw_stream_s
	{
		const unsigned char *next_in;
		unsigned int avail_in;
		iw_ulong total_in;
		
		unsigned char *next_out;
		unsigned int avail_out;
		iw_ulong total_out;
		
		char *msg;
		struct iw_internal_state *state;
		
		iw_alloc_func zalloc;
		iw_free_func zfree;
		void *opaque;
		
		int data_type;
		iw_ulong adler;
		iw_ulong reserved;
	} iw_stream;
	
	typedef iw_stream *iw_streamp;
	
	const char *iw_version(void);
	
#ifndef i2WC_NO_DEFLATE_APIS
	
	int iw_deflateInit(iw_streamp pStream, int level);
	
	int iw_deflateInit2(iw_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy);
	
	int iw_deflateReset(iw_streamp pStream);
	
	int iw_deflate(iw_streamp pStream, int flush);
	
	int iw_deflateEnd(iw_streamp pStream);
	
	iw_ulong iw_deflateBound(iw_streamp pStream, iw_ulong source_len);
	
	int iw_compress(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len);
	int iw_compress2(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len, int level);
	
	iw_ulong iw_compressBound(iw_ulong source_len);
	
#endif 
	
#ifndef i2WC_NO_INFLATE_APIS
	
	int iw_inflateInit(iw_streamp pStream);
	
	int iw_inflateInit2(iw_streamp pStream, int window_bits);
	
	int iw_inflateReset(iw_streamp pStream);
	
	int iw_inflate(iw_streamp pStream, int flush);
	
	int iw_inflateEnd(iw_streamp pStream);
	
	int iw_uncompress(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len);
	int iw_uncompress2(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong *pSource_len);
#endif
	
	const char *iw_error(int err);
	
#ifndef i2WC_NO_ZLIB_COMPATIBLE_NAMES
	typedef unsigned char Byte;
	typedef unsigned int uInt;
	typedef iw_ulong uLong;
	typedef Byte Bytef;
	typedef uInt uIntf;
	typedef char charf;
	typedef int intf;
	typedef void *voidpf;
	typedef uLong uLongf;
	typedef void *voidp;
	typedef void *const voidpc;
#define Z_NULL 0
#define Z_NO_FLUSH IW_NO_FLUSH
#define Z_PARTIAL_FLUSH IW_PARTIAL_FLUSH
#define Z_SYNC_FLUSH IW_SYNC_FLUSH
#define Z_FULL_FLUSH IW_FULL_FLUSH
#define Z_FINISH IW_FINISH
#define Z_BLOCK IW_BLOCK
#define Z_OK IW_OK
#define Z_STREAM_END IW_STREAM_END
#define Z_NEED_DICT IW_NEED_DICT
#define Z_ERRNO IW_ERRNO
#define Z_STREAM_ERROR IW_STREAM_ERROR
#define Z_DATA_ERROR IW_DATA_ERROR
#define Z_MEM_ERROR IW_MEM_ERROR
#define Z_BUF_ERROR IW_BUF_ERROR
#define Z_VERSION_ERROR IW_VERSION_ERROR
#define Z_PARAM_ERROR IW_PARAM_ERROR
#define Z_NO_COMPRESSION IW_NO_COMPRESSION
#define Z_BEST_SPEED IW_BEST_SPEED
#define Z_BEST_COMPRESSION IW_BEST_COMPRESSION
#define Z_DEFAULT_COMPRESSION IW_DEFAULT_COMPRESSION
#define Z_DEFAULT_STRATEGY IW_DEFAULT_STRATEGY
#define Z_FILTERED IW_FILTERED
#define Z_HUFFMAN_ONLY IW_HUFFMAN_ONLY
#define Z_RLE IW_RLE
#define Z_FIXED IW_FIXED
#define Z_DEFLATED IW_DEFLATED
#define Z_DEFAULT_WINDOW_BITS IW_DEFAULT_WINDOW_BITS
	typedef void *(*alloc_func)(void *opaque, size_t items, size_t size);
	typedef void (*free_func)(void *opaque, void *address);
	
#define internal_state iw_internal_state
#define z_stream iw_stream
	
#ifndef i2WC_NO_DEFLATE_APIS
	static IW_FORCEINLINE int deflateInit(iw_streamp pStream, int level)
	{
		return iw_deflateInit(pStream, level);
	}
	static IW_FORCEINLINE int deflateInit2(iw_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy)
	{
		return iw_deflateInit2(pStream, level, method, window_bits, mem_level, strategy);
	}
	static IW_FORCEINLINE int deflateReset(iw_streamp pStream)
	{
		return iw_deflateReset(pStream);
	}
	static IW_FORCEINLINE int deflate(iw_streamp pStream, int flush)
	{
		return iw_deflate(pStream, flush);
	}
	static IW_FORCEINLINE int deflateEnd(iw_streamp pStream)
	{
		return iw_deflateEnd(pStream);
	}
	static IW_FORCEINLINE iw_ulong deflateBound(iw_streamp pStream, iw_ulong source_len)
	{
		return iw_deflateBound(pStream, source_len);
	}
	static IW_FORCEINLINE int compress(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len)
	{
		return iw_compress(pDest, pDest_len, pSource, source_len);
	}
	static IW_FORCEINLINE int compress2(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len, int level)
	{
		return iw_compress2(pDest, pDest_len, pSource, source_len, level);
	}
	static IW_FORCEINLINE iw_ulong compressBound(iw_ulong source_len)
	{
		return iw_compressBound(source_len);
	}
#endif
	
#ifndef i2WC_NO_INFLATE_APIS
	static IW_FORCEINLINE int inflateInit(iw_streamp pStream)
	{
		return iw_inflateInit(pStream);
	}
	
	static IW_FORCEINLINE int inflateInit2(iw_streamp pStream, int window_bits)
	{
		return iw_inflateInit2(pStream, window_bits);
	}
	
	static IW_FORCEINLINE int inflateReset(iw_streamp pStream)
	{
		return iw_inflateReset(pStream);
	}
	
	static IW_FORCEINLINE int inflate(iw_streamp pStream, int flush)
	{
		return iw_inflate(pStream, flush);
	}
	
	static IW_FORCEINLINE int inflateEnd(iw_streamp pStream)
	{
		return iw_inflateEnd(pStream);
	}
	
	static IW_FORCEINLINE int uncompress(unsigned char* pDest, iw_ulong* pDest_len, const unsigned char* pSource, iw_ulong source_len)
	{
		return iw_uncompress(pDest, pDest_len, pSource, source_len);
	}
	
	static IW_FORCEINLINE int uncompress2(unsigned char* pDest, iw_ulong* pDest_len, const unsigned char* pSource, iw_ulong* pSource_len)
	{
		return iw_uncompress2(pDest, pDest_len, pSource, pSource_len);
	}
#endif
	
	static IW_FORCEINLINE iw_ulong crc32(iw_ulong crc, const unsigned char *ptr, size_t buf_len)
	{
		return iw_crc32(crc, ptr, buf_len);
	}
	
	static IW_FORCEINLINE iw_ulong adler32(iw_ulong adler, const unsigned char *ptr, size_t buf_len)
	{
		return iw_adler32(adler, ptr, buf_len);
	}
	
#define MAX_WBITS 15
#define MAX_MEM_LEVEL 9
	
	static IW_FORCEINLINE const char* zError(int err)
	{
		return iw_error(err);
	}
#define ZLIB_VERSION IW_VERSION
#define ZLIB_VERNUM IW_VERNUM
#define ZLIB_VER_MAJOR IW_VER_MAJOR
#define ZLIB_VER_MINOR IW_VER_MINOR
#define ZLIB_VER_REVISION IW_VER_REVISION
#define ZLIB_VER_SUBREVISION IW_VER_SUBREVISION
	
#define zlibVersion iw_version
#define zlib_version iw_version()
#endif
	
#endif
	
#ifdef __cplusplus
}
#endif

typedef unsigned char iw_uint8;
typedef int16_t iw_int16;
typedef uint16_t iw_uint16;
typedef uint32_t iw_uint32;
typedef uint32_t iw_uint;
typedef int64_t iw_int64;
typedef uint64_t iw_uint64;
typedef int iw_bool;

#define IW_FALSE (0)
#define IW_TRUE (1)

#ifdef _MSC_VER
#define IW_MACRO_END while (0, 0)
#else
#define IW_MACRO_END while (0)
#endif

#ifdef i2WC_NO_STDIO
#define IW_FILE void *
#else
#include <stdio.h>
#define IW_FILE FILE
#endif

#ifdef i2WC_NO_TIME
typedef struct iw_dummy_time_t_tag
{
	iw_uint32 m_dummy1;
	iw_uint32 m_dummy2;
} iw_dummy_time_t;
#define IW_TIME_T iw_dummy_time_t
#else
#define IW_TIME_T time_t
#endif

#define IW_ASSERT(x) assert(x)

#ifdef i2WC_NO_MALLOC
#define IW_MALLOC(x) NULL
#define IW_FREE(x) (void)x, ((void)0)
#define IW_REALLOC(p, x) NULL
#else
#define IW_MALLOC(x) malloc(x)
#define IW_FREE(x) free(x)
#define IW_REALLOC(p, x) realloc(p, x)
#endif

#define IW_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define IW_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define IW_CLEAR_OBJ(obj) memset(&(obj), 0, sizeof(obj))
#define IW_CLEAR_ARR(obj) memset((obj), 0, sizeof(obj))
#define IW_CLEAR_PTR(obj) memset((obj), 0, sizeof(*obj))

#if i2WC_USE_UNALIGNED_LOADS_AND_STORES && i2WC_LITTLE_ENDIAN
#define IW_READ_LE16(p) *((const iw_uint16 *)(p))
#define IW_READ_LE32(p) *((const iw_uint32 *)(p))
#else
#define IW_READ_LE16(p) ((iw_uint32)(((const iw_uint8 *)(p))[0]) | ((iw_uint32)(((const iw_uint8 *)(p))[1]) << 8U))
#define IW_READ_LE32(p) ((iw_uint32)(((const iw_uint8 *)(p))[0]) | ((iw_uint32)(((const iw_uint8 *)(p))[1]) << 8U) | ((iw_uint32)(((const iw_uint8 *)(p))[2]) << 16U) | ((iw_uint32)(((const iw_uint8 *)(p))[3]) << 24U))
#endif

#define IW_READ_LE64(p) (((iw_uint64)IW_READ_LE32(p)) | (((iw_uint64)IW_READ_LE32((const iw_uint8 *)(p) + sizeof(iw_uint32))) << 32U))

#ifdef __cplusplus
extern "C"
{
#endif
	
	extern void *i2wC_def_alloc_func(void *opaque, size_t items, size_t size);
	extern void i2wC_def_free_func(void *opaque, void *address);
	extern void *i2wC_def_realloc_func(void *opaque, void *address, size_t items, size_t size);
	
#define IW_UINT16_MAX (0xFFFFU)
#define IW_UINT32_MAX (0xFFFFFFFFU)
	
#ifdef __cplusplus
}
#endif


#ifndef i2WC_NO_DEFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
	
#ifndef iwc_z_LESS_MEMORY
#define iwc_z_LESS_MEMORY 0
#endif
	
	enum
	{
		iwc_z_HUFFMAN_ONLY = 0,
		iwc_z_DEFAULT_MAX_PROBES = 128,
		iwc_z_MAX_PROBES_MASK = 0xFFF
	};
	
	enum
	{
		iwc_z_WRITE_ZLIB_HEADER = 0x01000,
		iwc_z_COMPUTE_ADLER32 = 0x02000,
		iwc_z_GREEDY_PARSING_FLAG = 0x04000,
		iwc_z_NONDETERMINISTIC_PARSING_FLAG = 0x08000,
		iwc_z_RLE_MATCHES = 0x10000,
		iwc_z_FILTER_MATCHES = 0x20000,
		iwc_z_FORCE_ALL_STATIC_BLOCKS = 0x40000,
		iwc_z_FORCE_ALL_RAW_BLOCKS = 0x80000
	};
	
	void *iwc_z_compress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);
	
	size_t iwc_z_compress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags);
	
	void *iwc_z_write_image_to_png_file_in_memory_ex(const void *pImage, int w, int h, int num_chans, size_t *pLen_out, iw_uint level, iw_bool flip);
	void *iwc_z_write_image_to_png_file_in_memory(const void *pImage, int w, int h, int num_chans, size_t *pLen_out);
	
	typedef iw_bool (*iwc_z_put_buf_func_ptr)(const void *pBuf, int len, void *pUser);
	
	iw_bool iwc_z_compress_mem_to_output(const void *pBuf, size_t buf_len, iwc_z_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags);
	
	enum
	{
		iwc_z_MAX_HUFF_TABLES = 3,
		iwc_z_MAX_HUFF_SYMBOLS_0 = 288,
		iwc_z_MAX_HUFF_SYMBOLS_1 = 32,
		iwc_z_MAX_HUFF_SYMBOLS_2 = 19,
		iwc_z_LZ_DICT_SIZE = 32768,
		iwc_z_LZ_DICT_SIZE_MASK = iwc_z_LZ_DICT_SIZE - 1,
		iwc_z_MIN_MATCH_LEN = 3,
		iwc_z_MAX_MATCH_LEN = 258
	};
	
	
#if iwc_z_LESS_MEMORY
	enum
	{
		iwc_z_LZ_CODE_BUF_SIZE = 24 * 1024,
		iwc_z_OUT_BUF_SIZE = (iwc_z_LZ_CODE_BUF_SIZE * 13) / 10,
		iwc_z_MAX_HUFF_SYMBOLS = 288,
		iwc_z_LZ_HASH_BITS = 12,
		iwc_z_LEVEL1_HASH_SIZE_MASK = 4095,
		iwc_z_LZ_HASH_SHIFT = (iwc_z_LZ_HASH_BITS + 2) / 3,
		iwc_z_LZ_HASH_SIZE = 1 << iwc_z_LZ_HASH_BITS
	};
#else
	enum
	{
		iwc_z_LZ_CODE_BUF_SIZE = 64 * 1024,
		iwc_z_OUT_BUF_SIZE = (iw_uint)((iwc_z_LZ_CODE_BUF_SIZE * 13) / 10),
		iwc_z_MAX_HUFF_SYMBOLS = 288,
		iwc_z_LZ_HASH_BITS = 15,
		iwc_z_LEVEL1_HASH_SIZE_MASK = 4095,
		iwc_z_LZ_HASH_SHIFT = (iwc_z_LZ_HASH_BITS + 2) / 3,
		iwc_z_LZ_HASH_SIZE = 1 << iwc_z_LZ_HASH_BITS
	};
#endif
	
	typedef enum
	{
		iwc_z_STATUS_BAD_PARAM = -2,
		iwc_z_STATUS_PUT_BUF_FAILED = -1,
		iwc_z_STATUS_OKAY = 0,
		iwc_z_STATUS_DONE = 1
	} iwc_z_status;
	
	typedef enum
	{
		iwc_z_NO_FLUSH = 0,
		iwc_z_SYNC_FLUSH = 2,
		iwc_z_FULL_FLUSH = 3,
		iwc_z_FINISH = 4
	} iwc_z_flush;
	
	typedef struct
	{
		iwc_z_put_buf_func_ptr m_pPut_buf_func;
		void *m_pPut_buf_user;
		iw_uint m_flags, m_max_probes[2];
		int m_greedy_parsing;
		iw_uint m_adler32, m_lookahead_pos, m_lookahead_size, m_dict_size;
		iw_uint8 *m_pLZ_code_buf, *m_pLZ_flags, *m_pOutput_buf, *m_pOutput_buf_end;
		iw_uint m_num_flags_left, m_total_lz_bytes, m_lz_code_buf_dict_pos, m_bits_in, m_bit_buffer;
		iw_uint m_saved_match_dist, m_saved_match_len, m_saved_lit, m_output_flush_ofs, m_output_flush_remaining, m_finished, m_block_index, m_wants_to_finish;
		iwc_z_status m_prev_return_status;
		const void *m_pIn_buf;
		void *m_pOut_buf;
		size_t *m_pIn_buf_size, *m_pOut_buf_size;
		iwc_z_flush m_flush;
		const iw_uint8 *m_pSrc;
		size_t m_src_buf_left, m_out_buf_ofs;
		iw_uint8 m_dict[iwc_z_LZ_DICT_SIZE + iwc_z_MAX_MATCH_LEN - 1];
		iw_uint16 m_huff_count[iwc_z_MAX_HUFF_TABLES][iwc_z_MAX_HUFF_SYMBOLS];
		iw_uint16 m_huff_codes[iwc_z_MAX_HUFF_TABLES][iwc_z_MAX_HUFF_SYMBOLS];
		iw_uint8 m_huff_code_sizes[iwc_z_MAX_HUFF_TABLES][iwc_z_MAX_HUFF_SYMBOLS];
		iw_uint8 m_lz_code_buf[iwc_z_LZ_CODE_BUF_SIZE];
		iw_uint16 m_next[iwc_z_LZ_DICT_SIZE];
		iw_uint16 m_hash[iwc_z_LZ_HASH_SIZE];
		iw_uint8 m_output_buf[iwc_z_OUT_BUF_SIZE];
	} iwc_z_compressor;
	
	iwc_z_status iwc_z_init(iwc_z_compressor *d, iwc_z_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags);
	
	iwc_z_status iwc_z_compress(iwc_z_compressor *d, const void *pIn_buf, size_t *pIn_buf_size, void *pOut_buf, size_t *pOut_buf_size, iwc_z_flush flush);
	
	iwc_z_status iwc_z_compress_buffer(iwc_z_compressor *d, const void *pIn_buf, size_t in_buf_size, iwc_z_flush flush);
	
	iwc_z_status iwc_z_get_prev_return_status(iwc_z_compressor *d);
	iw_uint32 iwc_z_get_adler32(iwc_z_compressor *d);
	
	iw_uint iwc_z_create_comp_flags_from_zip_params(int level, int window_bits, int strategy);
	
#ifndef i2WC_NO_MALLOC
	iwc_z_compressor *iwc_z_compressor_alloc(void);
	void iwc_z_compressor_free(iwc_z_compressor *pComp);
#endif
	
#ifdef __cplusplus
}
#endif

#endif 

#ifndef i2WC_NO_INFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
	
	enum
	{
		iwc_u_FLAG_PARSE_ZLIB_HEADER = 1,
		iwc_u_FLAG_HAS_MORE_INPUT = 2,
		iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF = 4,
		iwc_u_FLAG_COMPUTE_ADLER32 = 8
	};
	
	void *iwc_u_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);
	
#define iwc_u_DECOMPRESS_MEM_TO_MEM_FAILED ((size_t)(-1))
	size_t iwc_u_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags);
	
	typedef int (*iwc_u_put_buf_func_ptr)(const void *pBuf, int len, void *pUser);
	int iwc_u_decompress_mem_to_callback(const void *pIn_buf, size_t *pIn_buf_size, iwc_u_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags);
	
	struct iwc_u_decompressor_tag;
	typedef struct iwc_u_decompressor_tag iwc_u_decompressor;
	
#ifndef i2WC_NO_MALLOC
	iwc_u_decompressor *iwc_u_decompressor_alloc(void);
	void iwc_u_decompressor_free(iwc_u_decompressor *pDecomp);
#endif
	
#define iwc_u_LZ_DICT_SIZE 32768
	
	typedef enum
	{
		iwc_u_STATUS_FAILED_CANNOT_MAKE_PROGRESS = -4,
		
		iwc_u_STATUS_BAD_PARAM = -3,
		
		iwc_u_STATUS_ADLER32_MISMATCH = -2,
		
		iwc_u_STATUS_FAILED = -1,
		
		iwc_u_STATUS_DONE = 0,
		
		iwc_u_STATUS_NEEDS_MORE_INPUT = 1,
		
		iwc_u_STATUS_HAS_MORE_OUTPUT = 2
	} iwc_u_status;
	
#define iwc_u_init(r)     \
	do                    \
	{                     \
	(r)->m_state = 0; \
	}                     \
	IW_MACRO_END
#define iwc_u_get_adler32(r) (r)->m_check_adler32
	
	iwc_u_status iwc_u_decompress(iwc_u_decompressor *r, const iw_uint8 *pIn_buf_next, size_t *pIn_buf_size, iw_uint8 *pOut_buf_start, iw_uint8 *pOut_buf_next, size_t *pOut_buf_size, const iw_uint32 decomp_flags);
	
	enum
	{
		iwc_u_MAX_HUFF_TABLES = 3,
		iwc_u_MAX_HUFF_SYMBOLS_0 = 288,
		iwc_u_MAX_HUFF_SYMBOLS_1 = 32,
		iwc_u_MAX_HUFF_SYMBOLS_2 = 19,
		iwc_u_FAST_LOOKUP_BITS = 10,
		iwc_u_FAST_LOOKUP_SIZE = 1 << iwc_u_FAST_LOOKUP_BITS
	};
	
#if i2WC_HAS_64BIT_REGISTERS
#define iwc_u_USE_64BIT_BITBUF 1
#else
#define iwc_u_USE_64BIT_BITBUF 0
#endif
	
#if iwc_u_USE_64BIT_BITBUF
	typedef iw_uint64 iwc_u_bit_buf_t;
#define iwc_u_BITBUF_SIZE (64)
#else
	typedef iw_uint32 iwc_u_bit_buf_t;
#define iwc_u_BITBUF_SIZE (32)
#endif
	
	struct iwc_u_decompressor_tag
	{
		iw_uint32 m_state, m_num_bits, m_zhdr0, m_zhdr1, m_z_adler32, m_final, m_type, m_check_adler32, m_dist, m_counter, m_num_extra, m_table_sizes[iwc_u_MAX_HUFF_TABLES];
		iwc_u_bit_buf_t m_bit_buf;
		size_t m_dist_from_out_buf_start;
		iw_int16 m_look_up[iwc_u_MAX_HUFF_TABLES][iwc_u_FAST_LOOKUP_SIZE];
		iw_int16 m_tree_0[iwc_u_MAX_HUFF_SYMBOLS_0 * 2];
		iw_int16 m_tree_1[iwc_u_MAX_HUFF_SYMBOLS_1 * 2];
		iw_int16 m_tree_2[iwc_u_MAX_HUFF_SYMBOLS_2 * 2];
		iw_uint8 m_code_size_0[iwc_u_MAX_HUFF_SYMBOLS_0];
		iw_uint8 m_code_size_1[iwc_u_MAX_HUFF_SYMBOLS_1];
		iw_uint8 m_code_size_2[iwc_u_MAX_HUFF_SYMBOLS_2];
		iw_uint8 m_raw_header[4], m_len_codes[iwc_u_MAX_HUFF_SYMBOLS_0 + iwc_u_MAX_HUFF_SYMBOLS_1 + 137];
	};
	
#ifdef __cplusplus
}
#endif

#endif

#ifndef i2WC_NO_ARCHIVE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
	
	enum
	{
		IW_ZIP_MAX_IO_BUF_SIZE = 64 * 1024,
		IW_ZIP_MAX_ARCHIVE_FILENAME_SIZE = 512,
		IW_ZIP_MAX_ARCHIVE_FILE_COMMENT_SIZE = 512
	};
	
	typedef struct
	{
		iw_uint32 m_file_index;
		
		iw_uint64 m_central_dir_ofs;
		
		iw_uint16 m_version_made_by;
		iw_uint16 m_version_needed;
		iw_uint16 m_bit_flag;
		iw_uint16 m_method;
		
		iw_uint32 m_crc32;
		
		iw_uint64 m_comp_size;
		
		iw_uint64 m_uncomp_size;
		
		iw_uint16 m_internal_attr;
		iw_uint32 m_external_attr;
		
		iw_uint64 m_local_header_ofs;
		
		iw_uint32 m_comment_size;
		
		iw_bool m_is_directory;
		
		iw_bool m_is_encrypted;
		
		iw_bool m_is_supported;
		
		char m_filename[IW_ZIP_MAX_ARCHIVE_FILENAME_SIZE];
		
		char m_comment[IW_ZIP_MAX_ARCHIVE_FILE_COMMENT_SIZE];
		
#ifdef i2WC_NO_TIME
		IW_TIME_T m_padding;
#else
		IW_TIME_T m_time;
#endif
	} iw_zip_archive_file_stat;
	
	typedef size_t (*iw_file_read_func)(void *pOpaque, iw_uint64 file_ofs, void *pBuf, size_t n);
	typedef size_t (*iw_file_write_func)(void *pOpaque, iw_uint64 file_ofs, const void *pBuf, size_t n);
	typedef iw_bool (*iw_file_needs_keepalive)(void *pOpaque);
	
	struct iw_zip_internal_state_tag;
	typedef struct iw_zip_internal_state_tag iw_zip_internal_state;
	
	typedef enum
	{
		IW_ZIP_MODE_INVALID = 0,
		IW_ZIP_MODE_READING = 1,
		IW_ZIP_MODE_WRITING = 2,
		IW_ZIP_MODE_WRITING_HAS_BEEN_FINALIZED = 3
	} iw_zip_mode;
	
	typedef enum
	{
		IW_ZIP_FLAG_CASE_SENSITIVE = 0x0100,
		IW_ZIP_FLAG_IGNORE_PATH = 0x0200,
		IW_ZIP_FLAG_COMPRESSED_DATA = 0x0400,
		IW_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY = 0x0800,
		IW_ZIP_FLAG_VALIDATE_LOCATE_FILE_FLAG = 0x1000,
		IW_ZIP_FLAG_VALIDATE_HEADERS_ONLY = 0x2000,
		IW_ZIP_FLAG_WRITE_ZIP64 = 0x4000,
		IW_ZIP_FLAG_WRITE_ALLOW_READING = 0x8000,
		IW_ZIP_FLAG_ASCII_FILENAME = 0x10000,
		IW_ZIP_FLAG_WRITE_HEADER_SET_SIZE = 0x20000,
		IW_ZIP_FLAG_READ_ALLOW_WRITING = 0x40000
	} iw_zip_flags;
	
	typedef enum
	{
		IW_ZIP_TYPE_INVALID = 0,
		IW_ZIP_TYPE_USER,
		IW_ZIP_TYPE_MEMORY,
		IW_ZIP_TYPE_HEAP,
		IW_ZIP_TYPE_FILE,
		IW_ZIP_TYPE_CFILE,
		IW_ZIP_TOTAL_TYPES
	} iw_zip_type;
	
	typedef enum
	{
		IW_ZIP_NO_ERROR = 0,
		IW_ZIP_UNDEFINED_ERROR,
		IW_ZIP_TOO_MANY_FILES,
		IW_ZIP_FILE_TOO_LARGE,
		IW_ZIP_UNSUPPORTED_METHOD,
		IW_ZIP_UNSUPPORTED_ENCRYPTION,
		IW_ZIP_UNSUPPORTED_FEATURE,
		IW_ZIP_FAILED_FINDING_CENTRAL_DIR,
		IW_ZIP_NOT_AN_ARCHIVE,
		IW_ZIP_INVALID_HEADER_OR_CORRUPTED,
		IW_ZIP_UNSUPPORTED_MULTIDISK,
		IW_ZIP_DECOMPRESSION_FAILED,
		IW_ZIP_COMPRESSION_FAILED,
		IW_ZIP_UNEXPECTED_DECOMPRESSED_SIZE,
		IW_ZIP_CRC_CHECK_FAILED,
		IW_ZIP_UNSUPPORTED_CDIR_SIZE,
		IW_ZIP_ALLOC_FAILED,
		IW_ZIP_FILE_OPEN_FAILED,
		IW_ZIP_FILE_CREATE_FAILED,
		IW_ZIP_FILE_WRITE_FAILED,
		IW_ZIP_FILE_READ_FAILED,
		IW_ZIP_FILE_CLOSE_FAILED,
		IW_ZIP_FILE_SEEK_FAILED,
		IW_ZIP_FILE_STAT_FAILED,
		IW_ZIP_INVALID_PARAMETER,
		IW_ZIP_INVALID_FILENAME,
		IW_ZIP_BUF_TOO_SMALL,
		IW_ZIP_INTERNAL_ERROR,
		IW_ZIP_FILE_NOT_FOUND,
		IW_ZIP_ARCHIVE_TOO_LARGE,
		IW_ZIP_VALIDATION_FAILED,
		IW_ZIP_WRITE_CALLBACK_FAILED,
		IW_ZIP_TOTAL_ERRORS
	} iw_zip_error;
	
	typedef struct
	{
		iw_uint64 m_archive_size;
		iw_uint64 m_central_directory_file_ofs;
		
		iw_uint32 m_total_files;
		iw_zip_mode m_zip_mode;
		iw_zip_type m_zip_type;
		iw_zip_error m_last_error;
		
		iw_uint64 m_file_offset_alignment;
		
		iw_alloc_func m_pAlloc;
		iw_free_func m_pFree;
		iw_realloc_func m_pRealloc;
		void *m_pAlloc_opaque;
		
		iw_file_read_func m_pRead;
		iw_file_write_func m_pWrite;
		iw_file_needs_keepalive m_pNeeds_keepalive;
		void *m_pIO_opaque;
		
		iw_zip_internal_state *m_pState;
		
	} iw_zip_archive;
	
	typedef struct
	{
		iw_zip_archive *pZip;
		iw_uint flags;
		
		int status;
		
		iw_uint64 read_buf_size, read_buf_ofs, read_buf_avail, comp_remaining, out_buf_ofs, cur_file_ofs;
		iw_zip_archive_file_stat file_stat;
		void *pRead_buf;
		void *pWrite_buf;
		
		size_t out_blk_remain;
		
		iwc_u_decompressor inflator;
		
#ifdef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
		iw_uint padding;
#else
		iw_uint file_crc32;
#endif
		
	} iw_zip_reader_extract_iter_state;
	
	iw_bool iw_zip_reader_init(iw_zip_archive *pZip, iw_uint64 size, iw_uint flags);
	
	iw_bool iw_zip_reader_init_mem(iw_zip_archive *pZip, const void *pMem, size_t size, iw_uint flags);
	
#ifndef i2WC_NO_STDIO
	
	iw_bool iw_zip_reader_init_file(iw_zip_archive *pZip, const char *pFilename, iw_uint32 flags);
	iw_bool iw_zip_reader_init_file_v2(iw_zip_archive *pZip, const char *pFilename, iw_uint flags, iw_uint64 file_start_ofs, iw_uint64 archive_size);
	
	iw_bool iw_zip_reader_init_cfile(iw_zip_archive *pZip, IW_FILE *pFile, iw_uint64 archive_size, iw_uint flags);
#endif
	
	iw_bool iw_zip_reader_end(iw_zip_archive *pZip);
	
	void iw_zip_zero_struct(iw_zip_archive *pZip);
	
	iw_zip_mode iw_zip_get_mode(iw_zip_archive *pZip);
	iw_zip_type iw_zip_get_type(iw_zip_archive *pZip);
	
	iw_uint iw_zip_reader_get_num_files(iw_zip_archive *pZip);
	
	iw_uint64 iw_zip_get_archive_size(iw_zip_archive *pZip);
	iw_uint64 iw_zip_get_archive_file_start_offset(iw_zip_archive *pZip);
	IW_FILE *iw_zip_get_cfile(iw_zip_archive *pZip);
	
	size_t iw_zip_read_archive_data(iw_zip_archive *pZip, iw_uint64 file_ofs, void *pBuf, size_t n);
	
	iw_zip_error iw_zip_set_last_error(iw_zip_archive *pZip, iw_zip_error err_num);
	iw_zip_error iw_zip_peek_last_error(iw_zip_archive *pZip);
	iw_zip_error iw_zip_clear_last_error(iw_zip_archive *pZip);
	iw_zip_error iw_zip_get_last_error(iw_zip_archive *pZip);
	const char *iw_zip_get_error_string(iw_zip_error iw_err);
	
	iw_bool iw_zip_reader_is_file_a_directory(iw_zip_archive *pZip, iw_uint file_index);
	
	iw_bool iw_zip_reader_is_file_encrypted(iw_zip_archive *pZip, iw_uint file_index);
	
	iw_bool iw_zip_reader_is_file_supported(iw_zip_archive *pZip, iw_uint file_index);
	
	iw_uint iw_zip_reader_get_filename(iw_zip_archive *pZip, iw_uint file_index, char *pFilename, iw_uint filename_buf_size);
	
	int iw_zip_reader_locate_file(iw_zip_archive *pZip, const char *pName, const char *pComment, iw_uint flags);
	iw_bool iw_zip_reader_locate_file_v2(iw_zip_archive *pZip, const char *pName, const char *pComment, iw_uint flags, iw_uint32 *file_index);
	
	iw_bool iw_zip_reader_file_stat(iw_zip_archive *pZip, iw_uint file_index, iw_zip_archive_file_stat *pStat);
	
	iw_bool iw_zip_is_zip64(iw_zip_archive *pZip);
	
	size_t iw_zip_get_central_dir_size(iw_zip_archive *pZip);
	
	iw_bool iw_zip_reader_extract_to_mem_no_alloc(iw_zip_archive *pZip, iw_uint file_index, void *pBuf, size_t buf_size, iw_uint flags, void *pUser_read_buf, size_t user_read_buf_size);
	iw_bool iw_zip_reader_extract_file_to_mem_no_alloc(iw_zip_archive *pZip, const char *pFilename, void *pBuf, size_t buf_size, iw_uint flags, void *pUser_read_buf, size_t user_read_buf_size);
	
	iw_bool iw_zip_reader_extract_to_mem(iw_zip_archive *pZip, iw_uint file_index, void *pBuf, size_t buf_size, iw_uint flags);
	iw_bool iw_zip_reader_extract_file_to_mem(iw_zip_archive *pZip, const char *pFilename, void *pBuf, size_t buf_size, iw_uint flags);
	
	void *iw_zip_reader_extract_to_heap(iw_zip_archive *pZip, iw_uint file_index, size_t *pSize, iw_uint flags);
	void *iw_zip_reader_extract_file_to_heap(iw_zip_archive *pZip, const char *pFilename, size_t *pSize, iw_uint flags);
	
	iw_bool iw_zip_reader_extract_to_callback(iw_zip_archive *pZip, iw_uint file_index, iw_file_write_func pCallback, void *pOpaque, iw_uint flags);
	iw_bool iw_zip_reader_extract_file_to_callback(iw_zip_archive *pZip, const char *pFilename, iw_file_write_func pCallback, void *pOpaque, iw_uint flags);
	
	iw_zip_reader_extract_iter_state *iw_zip_reader_extract_iter_new(iw_zip_archive *pZip, iw_uint file_index, iw_uint flags);
	iw_zip_reader_extract_iter_state *iw_zip_reader_extract_file_iter_new(iw_zip_archive *pZip, const char *pFilename, iw_uint flags);
	size_t iw_zip_reader_extract_iter_read(iw_zip_reader_extract_iter_state *pState, void *pvBuf, size_t buf_size);
	iw_bool iw_zip_reader_extract_iter_free(iw_zip_reader_extract_iter_state *pState);
	
#ifndef i2WC_NO_STDIO
	iw_bool iw_zip_reader_extract_to_file(iw_zip_archive *pZip, iw_uint file_index, const char *pDst_filename, iw_uint flags);
	iw_bool iw_zip_reader_extract_file_to_file(iw_zip_archive *pZip, const char *pArchive_filename, const char *pDst_filename, iw_uint flags);
	
	iw_bool iw_zip_reader_extract_to_cfile(iw_zip_archive *pZip, iw_uint file_index, IW_FILE *File, iw_uint flags);
	iw_bool iw_zip_reader_extract_file_to_cfile(iw_zip_archive *pZip, const char *pArchive_filename, IW_FILE *pFile, iw_uint flags);
#endif
	
#if 0
	typedef void *iw_zip_streaming_extract_state_ptr;
	iw_zip_streaming_extract_state_ptr iw_zip_streaming_extract_begin(iw_zip_archive *pZip, iw_uint file_index, iw_uint flags);
	iw_uint64 iw_zip_streaming_extract_get_size(iw_zip_archive *pZip, iw_zip_streaming_extract_state_ptr pState);
	iw_uint64 iw_zip_streaming_extract_get_cur_ofs(iw_zip_archive *pZip, iw_zip_streaming_extract_state_ptr pState);
	iw_bool iw_zip_streaming_extract_seek(iw_zip_archive *pZip, iw_zip_streaming_extract_state_ptr pState, iw_uint64 new_ofs);
	size_t iw_zip_streaming_extract_read(iw_zip_archive *pZip, iw_zip_streaming_extract_state_ptr pState, void *pBuf, size_t buf_size);
	iw_bool iw_zip_streaming_extract_end(iw_zip_archive *pZip, iw_zip_streaming_extract_state_ptr pState);
#endif
	
	iw_bool iw_zip_validate_file(iw_zip_archive *pZip, iw_uint file_index, iw_uint flags);
	
	iw_bool iw_zip_validate_archive(iw_zip_archive *pZip, iw_uint flags);
	
	iw_bool iw_zip_validate_mem_archive(const void *pMem, size_t size, iw_uint flags, iw_zip_error *pErr);
#ifndef i2WC_NO_STDIO
	iw_bool iw_zip_validate_file_archive(const char *pFilename, iw_uint flags, iw_zip_error *pErr);
#endif
	
	iw_bool iw_zip_end(iw_zip_archive *pZip);
	
	
#ifndef i2WC_NO_ARCHIVE_WRITING_APIS
	
	iw_bool iw_zip_writer_init(iw_zip_archive *pZip, iw_uint64 existing_size);
	iw_bool iw_zip_writer_init_v2(iw_zip_archive *pZip, iw_uint64 existing_size, iw_uint flags);
	
	iw_bool iw_zip_writer_init_heap(iw_zip_archive *pZip, size_t size_to_reserve_at_beginning, size_t initial_allocation_size);
	iw_bool iw_zip_writer_init_heap_v2(iw_zip_archive *pZip, size_t size_to_reserve_at_beginning, size_t initial_allocation_size, iw_uint flags);
	
#ifndef i2WC_NO_STDIO
	iw_bool iw_zip_writer_init_file(iw_zip_archive *pZip, const char *pFilename, iw_uint64 size_to_reserve_at_beginning);
	iw_bool iw_zip_writer_init_file_v2(iw_zip_archive *pZip, const char *pFilename, iw_uint64 size_to_reserve_at_beginning, iw_uint flags);
	iw_bool iw_zip_writer_init_cfile(iw_zip_archive *pZip, IW_FILE *pFile, iw_uint flags);
#endif
	
	iw_bool iw_zip_writer_init_from_reader(iw_zip_archive *pZip, const char *pFilename);
	iw_bool iw_zip_writer_init_from_reader_v2(iw_zip_archive *pZip, const char *pFilename, iw_uint flags);
	
	iw_bool iw_zip_writer_add_mem(iw_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, iw_uint level_and_flags);
	
	iw_bool iw_zip_writer_add_mem_ex(iw_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags,
												  iw_uint64 uncomp_size, iw_uint32 uncomp_crc32);
	
	iw_bool iw_zip_writer_add_mem_ex_v2(iw_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags,
													 iw_uint64 uncomp_size, iw_uint32 uncomp_crc32, IW_TIME_T *last_modified, const char *user_extra_data_local, iw_uint user_extra_data_local_len,
													 const char *user_extra_data_central, iw_uint user_extra_data_central_len);
	
	iw_bool iw_zip_writer_add_read_buf_callback(iw_zip_archive *pZip, const char *pArchive_name, iw_file_read_func read_callback, void *callback_opaque, iw_uint64 max_size,
															 const IW_TIME_T *pFile_time, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags, const char *user_extra_data_local, iw_uint user_extra_data_local_len,
															 const char *user_extra_data_central, iw_uint user_extra_data_central_len);
	
#ifndef i2WC_NO_STDIO
	iw_bool iw_zip_writer_add_file(iw_zip_archive *pZip, const char *pArchive_name, const char *pSrc_filename, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags);
	
	iw_bool iw_zip_writer_add_cfile(iw_zip_archive *pZip, const char *pArchive_name, IW_FILE *pSrc_file, iw_uint64 max_size,
												 const IW_TIME_T *pFile_time, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags, const char *user_extra_data_local, iw_uint user_extra_data_local_len,
												 const char *user_extra_data_central, iw_uint user_extra_data_central_len);
#endif
	
	iw_bool iw_zip_writer_add_from_zip_reader(iw_zip_archive *pZip, iw_zip_archive *pSource_zip, iw_uint src_file_index);
	
	iw_bool iw_zip_writer_finalize_archive(iw_zip_archive *pZip);
	
	iw_bool iw_zip_writer_finalize_heap_archive(iw_zip_archive *pZip, void **ppBuf, size_t *pSize);
	
	iw_bool iw_zip_writer_end(iw_zip_archive *pZip);
	
	iw_bool iw_zip_add_mem_to_archive_file_in_place(const char *pZip_filename, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags);
	iw_bool iw_zip_add_mem_to_archive_file_in_place_v2(const char *pZip_filename, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags, iw_zip_error *pErr);
	
#ifndef i2WC_NO_STDIO
	void *iw_zip_extract_archive_file_to_heap(const char *pZip_filename, const char *pArchive_name, size_t *pSize, iw_uint flags);
	void *iw_zip_extract_archive_file_to_heap_v2(const char *pZip_filename, const char *pArchive_name, const char *pComment, size_t *pSize, iw_uint flags, iw_zip_error *pErr);
#endif
	
#endif
	
#ifdef __cplusplus
}
#endif

#endif

typedef unsigned char iw_validate_uint16[sizeof(iw_uint16) == 2 ? 1 : -1];
typedef unsigned char iw_validate_uint32[sizeof(iw_uint32) == 4 ? 1 : -1];
typedef unsigned char iw_validate_uint64[sizeof(iw_uint64) == 8 ? 1 : -1];

#ifdef __cplusplus
extern "C"
{
#endif
	
	iw_ulong iw_adler32(iw_ulong adler, const unsigned char *ptr, size_t buf_len)
	{
		iw_uint32 i, s1 = (iw_uint32)(adler & 0xffff), s2 = (iw_uint32)(adler >> 16);
		size_t block_len = buf_len % 5552;
		if (!ptr)
			return IW_ADLER32_INIT;
		while (buf_len)
		{
			for (i = 0; i + 7 < block_len; i += 8, ptr += 8)
			{
				s1 += ptr[0], s2 += s1;
				s1 += ptr[1], s2 += s1;
				s1 += ptr[2], s2 += s1;
				s1 += ptr[3], s2 += s1;
				s1 += ptr[4], s2 += s1;
				s1 += ptr[5], s2 += s1;
				s1 += ptr[6], s2 += s1;
				s1 += ptr[7], s2 += s1;
			}
			for (; i < block_len; ++i)
				s1 += *ptr++, s2 += s1;
			s1 %= 65521U, s2 %= 65521U;
			buf_len -= block_len;
			block_len = 5552;
		}
		return (s2 << 16) + s1;
	}
	
#if 0
	iw_ulong iw_crc32(iw_ulong crc, const iw_uint8 *ptr, size_t buf_len)
	{
		static const iw_uint32 s_crc32[16] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
			0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
		iw_uint32 crcu32 = (iw_uint32)crc;
		if (!ptr)
			return IW_CRC32_INIT;
		crcu32 = ~crcu32;
		while (buf_len--)
		{
			iw_uint8 b = *ptr++;
			crcu32 = (crcu32 >> 4) ^ s_crc32[(crcu32 & 0xF) ^ (b & 0xF)];
			crcu32 = (crcu32 >> 4) ^ s_crc32[(crcu32 & 0xF) ^ (b >> 4)];
		}
		return ~crcu32;
	}
#elif defined(USE_EXTERNAL_IWCRC)
	iw_ulong iw_crc32(iw_ulong crc, const iw_uint8 *ptr, size_t buf_len);
#else
	iw_ulong iw_crc32(iw_ulong crc, const iw_uint8 *ptr, size_t buf_len)
	{
		static const iw_uint32 s_crc_table[256] = {
			0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535,
			0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD,
			0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D,
			0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
			0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4,
			0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
			0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC,
			0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
			0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB,
			0xB6662D3D, 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
			0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB,
			0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
			0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA,
			0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE,
			0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
			0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
			0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409,
			0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
			0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739,
			0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
			0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2, 0x1E01F268,
			0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0,
			0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8,
			0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
			0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF,
			0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703,
			0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7,
			0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
			0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE,
			0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
			0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6,
			0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
			0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D,
			0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5,
			0x47B2CF7F, 0x30B5FFE9, 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605,
			0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
			0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
		};
		
		iw_uint32 crc32 = (iw_uint32)crc ^ 0xFFFFFFFF;
		const iw_uint8 *pByte_buf = (const iw_uint8 *)ptr;
		
		while (buf_len >= 4)
		{
			crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[0]) & 0xFF];
			crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[1]) & 0xFF];
			crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[2]) & 0xFF];
			crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[3]) & 0xFF];
			pByte_buf += 4;
			buf_len -= 4;
		}
		
		while (buf_len)
		{
			crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[0]) & 0xFF];
			++pByte_buf;
			--buf_len;
		}
		
		return ~crc32;
	}
#endif
	
	void iw_free(void *p)
	{
		IW_FREE(p);
	}
	
	void *i2wC_def_alloc_func(void *opaque, size_t items, size_t size)
	{
		(void)opaque, (void)items, (void)size;
		return IW_MALLOC(items * size);
	}
	void i2wC_def_free_func(void *opaque, void *address)
	{
		(void)opaque, (void)address;
		IW_FREE(address);
	}
	void *i2wC_def_realloc_func(void *opaque, void *address, size_t items, size_t size)
	{
		(void)opaque, (void)address, (void)items, (void)size;
		return IW_REALLOC(address, items * size);
	}
	
	const char *iw_version(void)
	{
		return IW_VERSION;
	}
	
#ifndef i2WC_NO_ZLIB_APIS
	
#ifndef i2WC_NO_DEFLATE_APIS
	
	int iw_deflateInit(iw_streamp pStream, int level)
	{
		return iw_deflateInit2(pStream, level, IW_DEFLATED, IW_DEFAULT_WINDOW_BITS, 9, IW_DEFAULT_STRATEGY);
	}
	
	int iw_deflateInit2(iw_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy)
	{
		iwc_z_compressor *pComp;
		iw_uint comp_flags = iwc_z_COMPUTE_ADLER32 | iwc_z_create_comp_flags_from_zip_params(level, window_bits, strategy);
		
		if (!pStream)
			return IW_STREAM_ERROR;
		if ((method != IW_DEFLATED) || ((mem_level < 1) || (mem_level > 9)) || ((window_bits != IW_DEFAULT_WINDOW_BITS) && (-window_bits != IW_DEFAULT_WINDOW_BITS)))
			return IW_PARAM_ERROR;
		
		pStream->data_type = 0;
		pStream->adler = IW_ADLER32_INIT;
		pStream->msg = NULL;
		pStream->reserved = 0;
		pStream->total_in = 0;
		pStream->total_out = 0;
		if (!pStream->zalloc)
			pStream->zalloc = i2wC_def_alloc_func;
		if (!pStream->zfree)
			pStream->zfree = i2wC_def_free_func;
		
		pComp = (iwc_z_compressor *)pStream->zalloc(pStream->opaque, 1, sizeof(iwc_z_compressor));
		if (!pComp)
			return IW_MEM_ERROR;
		
		pStream->state = (struct iw_internal_state *)pComp;
		
		if (iwc_z_init(pComp, NULL, NULL, comp_flags) != iwc_z_STATUS_OKAY)
		{
			iw_deflateEnd(pStream);
			return IW_PARAM_ERROR;
		}
		
		return IW_OK;
	}
	
	int iw_deflateReset(iw_streamp pStream)
	{
		if ((!pStream) || (!pStream->state) || (!pStream->zalloc) || (!pStream->zfree))
			return IW_STREAM_ERROR;
		pStream->total_in = pStream->total_out = 0;
		iwc_z_init((iwc_z_compressor *)pStream->state, NULL, NULL, ((iwc_z_compressor *)pStream->state)->m_flags);
		return IW_OK;
	}
	
	int iw_deflate(iw_streamp pStream, int flush)
	{
		size_t in_bytes, out_bytes;
		iw_ulong orig_total_in, orig_total_out;
		int iw_status = IW_OK;
		
		if ((!pStream) || (!pStream->state) || (flush < 0) || (flush > IW_FINISH) || (!pStream->next_out))
			return IW_STREAM_ERROR;
		if (!pStream->avail_out)
			return IW_BUF_ERROR;
		
		if (flush == IW_PARTIAL_FLUSH)
			flush = IW_SYNC_FLUSH;
		
		if (((iwc_z_compressor *)pStream->state)->m_prev_return_status == iwc_z_STATUS_DONE)
			return (flush == IW_FINISH) ? IW_STREAM_END : IW_BUF_ERROR;
		
		orig_total_in = pStream->total_in;
		orig_total_out = pStream->total_out;
		for (;;)
		{
			iwc_z_status defl_status;
			in_bytes = pStream->avail_in;
			out_bytes = pStream->avail_out;
			
			defl_status = iwc_z_compress((iwc_z_compressor *)pStream->state, pStream->next_in, &in_bytes, pStream->next_out, &out_bytes, (iwc_z_flush)flush);
			pStream->next_in += (iw_uint)in_bytes;
			pStream->avail_in -= (iw_uint)in_bytes;
			pStream->total_in += (iw_uint)in_bytes;
			pStream->adler = iwc_z_get_adler32((iwc_z_compressor *)pStream->state);
			
			pStream->next_out += (iw_uint)out_bytes;
			pStream->avail_out -= (iw_uint)out_bytes;
			pStream->total_out += (iw_uint)out_bytes;
			
			if (defl_status < 0)
			{
				iw_status = IW_STREAM_ERROR;
				break;
			}
			else if (defl_status == iwc_z_STATUS_DONE)
			{
				iw_status = IW_STREAM_END;
				break;
			}
			else if (!pStream->avail_out)
				break;
			else if ((!pStream->avail_in) && (flush != IW_FINISH))
			{
				if ((flush) || (pStream->total_in != orig_total_in) || (pStream->total_out != orig_total_out))
					break;
				return IW_BUF_ERROR;
			}
		}
		return iw_status;
	}
	
	int iw_deflateEnd(iw_streamp pStream)
	{
		if (!pStream)
			return IW_STREAM_ERROR;
		if (pStream->state)
		{
			pStream->zfree(pStream->opaque, pStream->state);
			pStream->state = NULL;
		}
		return IW_OK;
	}
	
	iw_ulong iw_deflateBound(iw_streamp pStream, iw_ulong source_len)
	{
		(void)pStream;
		return IW_MAX(128 + (source_len * 110) / 100, 128 + source_len + ((source_len / (31 * 1024)) + 1) * 5);
	}
	
	int iw_compress2(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len, int level)
	{
		int status;
		iw_stream stream;
		memset(&stream, 0, sizeof(stream));
		
		if ((iw_uint64)(source_len | *pDest_len) > 0xFFFFFFFFU)
			return IW_PARAM_ERROR;
		
		stream.next_in = pSource;
		stream.avail_in = (iw_uint32)source_len;
		stream.next_out = pDest;
		stream.avail_out = (iw_uint32)*pDest_len;
		
		status = iw_deflateInit(&stream, level);
		if (status != IW_OK)
			return status;
		
		status = iw_deflate(&stream, IW_FINISH);
		if (status != IW_STREAM_END)
		{
			iw_deflateEnd(&stream);
			return (status == IW_OK) ? IW_BUF_ERROR : status;
		}
		
		*pDest_len = stream.total_out;
		return iw_deflateEnd(&stream);
	}
	
	int iw_compress(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len)
	{
		return iw_compress2(pDest, pDest_len, pSource, source_len, IW_DEFAULT_COMPRESSION);
	}
	
	iw_ulong iw_compressBound(iw_ulong source_len)
	{
		return iw_deflateBound(NULL, source_len);
	}
	
#endif
	
#ifndef i2WC_NO_INFLATE_APIS
	
	typedef struct
	{
		iwc_u_decompressor m_decomp;
		iw_uint m_dict_ofs, m_dict_avail, m_first_call, m_has_flushed;
		int m_window_bits;
		iw_uint8 m_dict[iwc_u_LZ_DICT_SIZE];
		iwc_u_status m_last_status;
	} inflate_state;
	
	int iw_inflateInit2(iw_streamp pStream, int window_bits)
	{
		inflate_state *pDecomp;
		if (!pStream)
			return IW_STREAM_ERROR;
		if ((window_bits != IW_DEFAULT_WINDOW_BITS) && (-window_bits != IW_DEFAULT_WINDOW_BITS))
			return IW_PARAM_ERROR;
		
		pStream->data_type = 0;
		pStream->adler = 0;
		pStream->msg = NULL;
		pStream->total_in = 0;
		pStream->total_out = 0;
		pStream->reserved = 0;
		if (!pStream->zalloc)
			pStream->zalloc = i2wC_def_alloc_func;
		if (!pStream->zfree)
			pStream->zfree = i2wC_def_free_func;
		
		pDecomp = (inflate_state *)pStream->zalloc(pStream->opaque, 1, sizeof(inflate_state));
		if (!pDecomp)
			return IW_MEM_ERROR;
		
		pStream->state = (struct iw_internal_state *)pDecomp;
		
		iwc_u_init(&pDecomp->m_decomp);
		pDecomp->m_dict_ofs = 0;
		pDecomp->m_dict_avail = 0;
		pDecomp->m_last_status = iwc_u_STATUS_NEEDS_MORE_INPUT;
		pDecomp->m_first_call = 1;
		pDecomp->m_has_flushed = 0;
		pDecomp->m_window_bits = window_bits;
		
		return IW_OK;
	}
	
	int iw_inflateInit(iw_streamp pStream)
	{
		return iw_inflateInit2(pStream, IW_DEFAULT_WINDOW_BITS);
	}
	
	int iw_inflateReset(iw_streamp pStream)
	{
		inflate_state *pDecomp;
		if (!pStream)
			return IW_STREAM_ERROR;
		
		pStream->data_type = 0;
		pStream->adler = 0;
		pStream->msg = NULL;
		pStream->total_in = 0;
		pStream->total_out = 0;
		pStream->reserved = 0;
		
		pDecomp = (inflate_state *)pStream->state;
		
		iwc_u_init(&pDecomp->m_decomp);
		pDecomp->m_dict_ofs = 0;
		pDecomp->m_dict_avail = 0;
		pDecomp->m_last_status = iwc_u_STATUS_NEEDS_MORE_INPUT;
		pDecomp->m_first_call = 1;
		pDecomp->m_has_flushed = 0;
		
		return IW_OK;
	}
	
	int iw_inflate(iw_streamp pStream, int flush)
	{
		inflate_state *pState;
		iw_uint n, first_call, decomp_flags = iwc_u_FLAG_COMPUTE_ADLER32;
		size_t in_bytes, out_bytes, orig_avail_in;
		iwc_u_status status;
		
		if ((!pStream) || (!pStream->state))
			return IW_STREAM_ERROR;
		if (flush == IW_PARTIAL_FLUSH)
			flush = IW_SYNC_FLUSH;
		if ((flush) && (flush != IW_SYNC_FLUSH) && (flush != IW_FINISH))
			return IW_STREAM_ERROR;
		
		pState = (inflate_state *)pStream->state;
		if (pState->m_window_bits > 0)
			decomp_flags |= iwc_u_FLAG_PARSE_ZLIB_HEADER;
		orig_avail_in = pStream->avail_in;
		
		first_call = pState->m_first_call;
		pState->m_first_call = 0;
		if (pState->m_last_status < 0)
			return IW_DATA_ERROR;
		
		if (pState->m_has_flushed && (flush != IW_FINISH))
			return IW_STREAM_ERROR;
		pState->m_has_flushed |= (flush == IW_FINISH);
		
		if ((flush == IW_FINISH) && (first_call))
		{
			decomp_flags |= iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF;
			in_bytes = pStream->avail_in;
			out_bytes = pStream->avail_out;
			status = iwc_u_decompress(&pState->m_decomp, pStream->next_in, &in_bytes, pStream->next_out, pStream->next_out, &out_bytes, decomp_flags);
			pState->m_last_status = status;
			pStream->next_in += (iw_uint)in_bytes;
			pStream->avail_in -= (iw_uint)in_bytes;
			pStream->total_in += (iw_uint)in_bytes;
			pStream->adler = iwc_u_get_adler32(&pState->m_decomp);
			pStream->next_out += (iw_uint)out_bytes;
			pStream->avail_out -= (iw_uint)out_bytes;
			pStream->total_out += (iw_uint)out_bytes;
			
			if (status < 0)
				return IW_DATA_ERROR;
			else if (status != iwc_u_STATUS_DONE)
			{
				pState->m_last_status = iwc_u_STATUS_FAILED;
				return IW_BUF_ERROR;
			}
			return IW_STREAM_END;
		}
		
		if (flush != IW_FINISH)
			decomp_flags |= iwc_u_FLAG_HAS_MORE_INPUT;
		
		if (pState->m_dict_avail)
		{
			n = IW_MIN(pState->m_dict_avail, pStream->avail_out);
			memcpy(pStream->next_out, pState->m_dict + pState->m_dict_ofs, n);
			pStream->next_out += n;
			pStream->avail_out -= n;
			pStream->total_out += n;
			pState->m_dict_avail -= n;
			pState->m_dict_ofs = (pState->m_dict_ofs + n) & (iwc_u_LZ_DICT_SIZE - 1);
			return ((pState->m_last_status == iwc_u_STATUS_DONE) && (!pState->m_dict_avail)) ? IW_STREAM_END : IW_OK;
		}
		
		for (;;)
		{
			in_bytes = pStream->avail_in;
			out_bytes = iwc_u_LZ_DICT_SIZE - pState->m_dict_ofs;
			
			status = iwc_u_decompress(&pState->m_decomp, pStream->next_in, &in_bytes, pState->m_dict, pState->m_dict + pState->m_dict_ofs, &out_bytes, decomp_flags);
			pState->m_last_status = status;
			
			pStream->next_in += (iw_uint)in_bytes;
			pStream->avail_in -= (iw_uint)in_bytes;
			pStream->total_in += (iw_uint)in_bytes;
			pStream->adler = iwc_u_get_adler32(&pState->m_decomp);
			
			pState->m_dict_avail = (iw_uint)out_bytes;
			
			n = IW_MIN(pState->m_dict_avail, pStream->avail_out);
			memcpy(pStream->next_out, pState->m_dict + pState->m_dict_ofs, n);
			pStream->next_out += n;
			pStream->avail_out -= n;
			pStream->total_out += n;
			pState->m_dict_avail -= n;
			pState->m_dict_ofs = (pState->m_dict_ofs + n) & (iwc_u_LZ_DICT_SIZE - 1);
			
			if (status < 0)
				return IW_DATA_ERROR;
			else if ((status == iwc_u_STATUS_NEEDS_MORE_INPUT) && (!orig_avail_in))
				return IW_BUF_ERROR;
			else if (flush == IW_FINISH)
			{
				if (status == iwc_u_STATUS_DONE)
					return pState->m_dict_avail ? IW_BUF_ERROR : IW_STREAM_END;
				else if (!pStream->avail_out)
					return IW_BUF_ERROR;
			}
			else if ((status == iwc_u_STATUS_DONE) || (!pStream->avail_in) || (!pStream->avail_out) || (pState->m_dict_avail))
				break;
		}
		
		return ((status == iwc_u_STATUS_DONE) && (!pState->m_dict_avail)) ? IW_STREAM_END : IW_OK;
	}
	
	int iw_inflateEnd(iw_streamp pStream)
	{
		if (!pStream)
			return IW_STREAM_ERROR;
		if (pStream->state)
		{
			pStream->zfree(pStream->opaque, pStream->state);
			pStream->state = NULL;
		}
		return IW_OK;
	}
	int iw_uncompress2(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong *pSource_len)
	{
		iw_stream stream;
		int status;
		memset(&stream, 0, sizeof(stream));
		
		if ((iw_uint64)(*pSource_len | *pDest_len) > 0xFFFFFFFFU)
			return IW_PARAM_ERROR;
		
		stream.next_in = pSource;
		stream.avail_in = (iw_uint32)*pSource_len;
		stream.next_out = pDest;
		stream.avail_out = (iw_uint32)*pDest_len;
		
		status = iw_inflateInit(&stream);
		if (status != IW_OK)
			return status;
		
		status = iw_inflate(&stream, IW_FINISH);
		*pSource_len = *pSource_len - stream.avail_in;
		if (status != IW_STREAM_END)
		{
			iw_inflateEnd(&stream);
			return ((status == IW_BUF_ERROR) && (!stream.avail_in)) ? IW_DATA_ERROR : status;
		}
		*pDest_len = stream.total_out;
		
		return iw_inflateEnd(&stream);
	}
	
	int iw_uncompress(unsigned char *pDest, iw_ulong *pDest_len, const unsigned char *pSource, iw_ulong source_len)
	{
		return iw_uncompress2(pDest, pDest_len, pSource, &source_len);
	}
	
#endif
	
	const char *iw_error(int err)
	{
		static struct
	{
		int m_err;
		const char *m_pDesc;
	} s_error_descs[] = {
		{ IW_OK, "" }, { IW_STREAM_END, "stream end" }, { IW_NEED_DICT, "need dictionary" }, { IW_ERRNO, "file error" }, { IW_STREAM_ERROR, "stream error" }, { IW_DATA_ERROR, "data error" }, { IW_MEM_ERROR, "out of memory" }, { IW_BUF_ERROR, "buf error" }, { IW_VERSION_ERROR, "version error" }, { IW_PARAM_ERROR, "parameter error" }
	};
		iw_uint i;
		for (i = 0; i < sizeof(s_error_descs) / sizeof(s_error_descs[0]); ++i)
			if (s_error_descs[i].m_err == err)
				return s_error_descs[i].m_pDesc;
		return NULL;
	}
	
#endif
	
#ifdef __cplusplus
}
#endif

#ifndef i2WC_NO_DEFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
	
	static const iw_uint16 s_iwc_z_len_sym[256] = {
		257, 258, 259, 260, 261, 262, 263, 264, 265, 265, 266, 266, 267, 267, 268, 268, 269, 269, 269, 269, 270, 270, 270, 270, 271, 271, 271, 271, 272, 272, 272, 272,
		273, 273, 273, 273, 273, 273, 273, 273, 274, 274, 274, 274, 274, 274, 274, 274, 275, 275, 275, 275, 275, 275, 275, 275, 276, 276, 276, 276, 276, 276, 276, 276,
		277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278,
		279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280,
		281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281,
		282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282,
		283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283,
		284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 285
	};
	
	static const iw_uint8 s_iwc_z_len_extra[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0
	};
	
	static const iw_uint8 s_iwc_z_small_dist_sym[512] = {
		0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11,
		11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
	};
	
	static const iw_uint8 s_iwc_z_small_dist_extra[512] = {
		0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7
	};
	
	static const iw_uint8 s_iwc_z_large_dist_sym[128] = {
		0, 0, 18, 19, 20, 20, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
		26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29
	};
	
	static const iw_uint8 s_iwc_z_large_dist_extra[128] = {
		0, 0, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
	};
	
	typedef struct
	{
		iw_uint16 m_key, m_sym_index;
	} iwc_z_sym_freq;
	static iwc_z_sym_freq *iwc_z_radix_sort_syms(iw_uint num_syms, iwc_z_sym_freq *pSyms0, iwc_z_sym_freq *pSyms1)
	{
		iw_uint32 total_passes = 2, pass_shift, pass, i, hist[256 * 2];
		iwc_z_sym_freq *pCur_syms = pSyms0, *pNew_syms = pSyms1;
		IW_CLEAR_ARR(hist);
		for (i = 0; i < num_syms; i++)
		{
			iw_uint freq = pSyms0[i].m_key;
			hist[freq & 0xFF]++;
			hist[256 + ((freq >> 8) & 0xFF)]++;
		}
		while ((total_passes > 1) && (num_syms == hist[(total_passes - 1) * 256]))
			total_passes--;
		for (pass_shift = 0, pass = 0; pass < total_passes; pass++, pass_shift += 8)
		{
			const iw_uint32 *pHist = &hist[pass << 8];
			iw_uint offsets[256], cur_ofs = 0;
			for (i = 0; i < 256; i++)
			{
				offsets[i] = cur_ofs;
				cur_ofs += pHist[i];
			}
			for (i = 0; i < num_syms; i++)
				pNew_syms[offsets[(pCur_syms[i].m_key >> pass_shift) & 0xFF]++] = pCur_syms[i];
			{
				iwc_z_sym_freq *t = pCur_syms;
				pCur_syms = pNew_syms;
				pNew_syms = t;
			}
		}
		return pCur_syms;
	}
	
	static void iwc_z_calculate_minimum_redundancy(iwc_z_sym_freq *A, int n)
	{
		int root, leaf, next, avbl, used, dpth;
		if (n == 0)
			return;
		else if (n == 1)
		{
			A[0].m_key = 1;
			return;
		}
		A[0].m_key += A[1].m_key;
		root = 0;
		leaf = 2;
		for (next = 1; next < n - 1; next++)
		{
			if (leaf >= n || A[root].m_key < A[leaf].m_key)
			{
				A[next].m_key = A[root].m_key;
				A[root++].m_key = (iw_uint16)next;
			}
			else
				A[next].m_key = A[leaf++].m_key;
			if (leaf >= n || (root < next && A[root].m_key < A[leaf].m_key))
			{
				A[next].m_key = (iw_uint16)(A[next].m_key + A[root].m_key);
				A[root++].m_key = (iw_uint16)next;
			}
			else
				A[next].m_key = (iw_uint16)(A[next].m_key + A[leaf++].m_key);
		}
		A[n - 2].m_key = 0;
		for (next = n - 3; next >= 0; next--)
			A[next].m_key = A[A[next].m_key].m_key + 1;
		avbl = 1;
		used = dpth = 0;
		root = n - 2;
		next = n - 1;
		while (avbl > 0)
		{
			while (root >= 0 && (int)A[root].m_key == dpth)
			{
				used++;
				root--;
			}
			while (avbl > used)
			{
				A[next--].m_key = (iw_uint16)(dpth);
				avbl--;
			}
			avbl = 2 * used;
			dpth++;
			used = 0;
		}
	}
	
	enum
	{
		iwc_z_MAX_SUPPORTED_HUFF_CODESIZE = 32
	};
	static void iwc_z_huffman_enforce_max_code_size(int *pNum_codes, int code_list_len, int max_code_size)
	{
		int i;
		iw_uint32 total = 0;
		if (code_list_len <= 1)
			return;
		for (i = max_code_size + 1; i <= iwc_z_MAX_SUPPORTED_HUFF_CODESIZE; i++)
			pNum_codes[max_code_size] += pNum_codes[i];
		for (i = max_code_size; i > 0; i--)
			total += (((iw_uint32)pNum_codes[i]) << (max_code_size - i));
		while (total != (1UL << max_code_size))
		{
			pNum_codes[max_code_size]--;
			for (i = max_code_size - 1; i > 0; i--)
				if (pNum_codes[i])
				{
					pNum_codes[i]--;
					pNum_codes[i + 1] += 2;
					break;
				}
			total--;
		}
	}
	
	static void iwc_z_optimize_huffman_table(iwc_z_compressor *d, int table_num, int table_len, int code_size_limit, int static_table)
	{
		int i, j, l, num_codes[1 + iwc_z_MAX_SUPPORTED_HUFF_CODESIZE];
		iw_uint next_code[iwc_z_MAX_SUPPORTED_HUFF_CODESIZE + 1];
		IW_CLEAR_ARR(num_codes);
		if (static_table)
		{
			for (i = 0; i < table_len; i++)
				num_codes[d->m_huff_code_sizes[table_num][i]]++;
		}
		else
		{
			iwc_z_sym_freq syms0[iwc_z_MAX_HUFF_SYMBOLS], syms1[iwc_z_MAX_HUFF_SYMBOLS], *pSyms;
			int num_used_syms = 0;
			const iw_uint16 *pSym_count = &d->m_huff_count[table_num][0];
			for (i = 0; i < table_len; i++)
				if (pSym_count[i])
				{
					syms0[num_used_syms].m_key = (iw_uint16)pSym_count[i];
					syms0[num_used_syms++].m_sym_index = (iw_uint16)i;
				}
			
			pSyms = iwc_z_radix_sort_syms(num_used_syms, syms0, syms1);
			iwc_z_calculate_minimum_redundancy(pSyms, num_used_syms);
			
			for (i = 0; i < num_used_syms; i++)
				num_codes[pSyms[i].m_key]++;
			
			iwc_z_huffman_enforce_max_code_size(num_codes, num_used_syms, code_size_limit);
			
			IW_CLEAR_ARR(d->m_huff_code_sizes[table_num]);
			IW_CLEAR_ARR(d->m_huff_codes[table_num]);
			for (i = 1, j = num_used_syms; i <= code_size_limit; i++)
				for (l = num_codes[i]; l > 0; l--)
					d->m_huff_code_sizes[table_num][pSyms[--j].m_sym_index] = (iw_uint8)(i);
		}
		
		next_code[1] = 0;
		for (j = 0, i = 2; i <= code_size_limit; i++)
			next_code[i] = j = ((j + num_codes[i - 1]) << 1);
		
		for (i = 0; i < table_len; i++)
		{
			iw_uint rev_code = 0, code, code_size;
			if ((code_size = d->m_huff_code_sizes[table_num][i]) == 0)
				continue;
			code = next_code[code_size]++;
			for (l = code_size; l > 0; l--, code >>= 1)
				rev_code = (rev_code << 1) | (code & 1);
			d->m_huff_codes[table_num][i] = (iw_uint16)rev_code;
		}
	}
	
#define iwc_z_PUT_BITS(b, l)                                       \
	do                                                             \
	{                                                              \
	iw_uint bits = b;                                          \
	iw_uint len = l;                                           \
	IW_ASSERT(bits <= ((1U << len) - 1U));                     \
	d->m_bit_buffer |= (bits << d->m_bits_in);                 \
	d->m_bits_in += len;                                       \
	while (d->m_bits_in >= 8)                                  \
	{                                                          \
	if (d->m_pOutput_buf < d->m_pOutput_buf_end)           \
	*d->m_pOutput_buf++ = (iw_uint8)(d->m_bit_buffer); \
	d->m_bit_buffer >>= 8;                                 \
	d->m_bits_in -= 8;                                     \
	}                                                          \
	}                                                              \
	IW_MACRO_END
	
#define iwc_z_RLE_PREV_CODE_SIZE()                                                                                       \
	{                                                                                                                    \
	if (rle_repeat_count)                                                                                            \
	{                                                                                                                \
	if (rle_repeat_count < 3)                                                                                    \
	{                                                                                                            \
	d->m_huff_count[2][prev_code_size] = (iw_uint16)(d->m_huff_count[2][prev_code_size] + rle_repeat_count); \
	while (rle_repeat_count--)                                                                               \
	packed_code_sizes[num_packed_code_sizes++] = prev_code_size;                                         \
	}                                                                                                            \
	else                                                                                                         \
	{                                                                                                            \
	d->m_huff_count[2][16] = (iw_uint16)(d->m_huff_count[2][16] + 1);                                        \
	packed_code_sizes[num_packed_code_sizes++] = 16;                                                         \
	packed_code_sizes[num_packed_code_sizes++] = (iw_uint8)(rle_repeat_count - 3);                           \
	}                                                                                                            \
	rle_repeat_count = 0;                                                                                        \
	}                                                                                                                \
	}
	
#define iwc_z_RLE_ZERO_CODE_SIZE()                                                         \
	{                                                                                      \
	if (rle_z_count)                                                                   \
	{                                                                                  \
	if (rle_z_count < 3)                                                           \
	{                                                                              \
	d->m_huff_count[2][0] = (iw_uint16)(d->m_huff_count[2][0] + rle_z_count);  \
	while (rle_z_count--)                                                      \
	packed_code_sizes[num_packed_code_sizes++] = 0;                        \
	}                                                                              \
	else if (rle_z_count <= 10)                                                    \
	{                                                                              \
	d->m_huff_count[2][17] = (iw_uint16)(d->m_huff_count[2][17] + 1);          \
	packed_code_sizes[num_packed_code_sizes++] = 17;                           \
	packed_code_sizes[num_packed_code_sizes++] = (iw_uint8)(rle_z_count - 3);  \
	}                                                                              \
	else                                                                           \
	{                                                                              \
	d->m_huff_count[2][18] = (iw_uint16)(d->m_huff_count[2][18] + 1);          \
	packed_code_sizes[num_packed_code_sizes++] = 18;                           \
	packed_code_sizes[num_packed_code_sizes++] = (iw_uint8)(rle_z_count - 11); \
	}                                                                              \
	rle_z_count = 0;                                                               \
	}                                                                                  \
	}
	
	static const iw_uint8 s_iwc_z_packed_code_size_syms_swizzle[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
	
	static void iwc_z_start_dynamic_block(iwc_z_compressor *d)
	{
		int num_lit_codes, num_dist_codes, num_bit_lengths;
		iw_uint i, total_code_sizes_to_pack, num_packed_code_sizes, rle_z_count, rle_repeat_count, packed_code_sizes_index;
		iw_uint8 code_sizes_to_pack[iwc_z_MAX_HUFF_SYMBOLS_0 + iwc_z_MAX_HUFF_SYMBOLS_1], packed_code_sizes[iwc_z_MAX_HUFF_SYMBOLS_0 + iwc_z_MAX_HUFF_SYMBOLS_1], prev_code_size = 0xFF;
		
		d->m_huff_count[0][256] = 1;
		
		iwc_z_optimize_huffman_table(d, 0, iwc_z_MAX_HUFF_SYMBOLS_0, 15, IW_FALSE);
		iwc_z_optimize_huffman_table(d, 1, iwc_z_MAX_HUFF_SYMBOLS_1, 15, IW_FALSE);
		
		for (num_lit_codes = 286; num_lit_codes > 257; num_lit_codes--)
			if (d->m_huff_code_sizes[0][num_lit_codes - 1])
				break;
		for (num_dist_codes = 30; num_dist_codes > 1; num_dist_codes--)
			if (d->m_huff_code_sizes[1][num_dist_codes - 1])
				break;
		
		memcpy(code_sizes_to_pack, &d->m_huff_code_sizes[0][0], num_lit_codes);
		memcpy(code_sizes_to_pack + num_lit_codes, &d->m_huff_code_sizes[1][0], num_dist_codes);
		total_code_sizes_to_pack = num_lit_codes + num_dist_codes;
		num_packed_code_sizes = 0;
		rle_z_count = 0;
		rle_repeat_count = 0;
		
		memset(&d->m_huff_count[2][0], 0, sizeof(d->m_huff_count[2][0]) * iwc_z_MAX_HUFF_SYMBOLS_2);
		for (i = 0; i < total_code_sizes_to_pack; i++)
		{
			iw_uint8 code_size = code_sizes_to_pack[i];
			if (!code_size)
			{
				iwc_z_RLE_PREV_CODE_SIZE();
				if (++rle_z_count == 138)
				{
					iwc_z_RLE_ZERO_CODE_SIZE();
				}
			}
			else
			{
				iwc_z_RLE_ZERO_CODE_SIZE();
				if (code_size != prev_code_size)
				{
					iwc_z_RLE_PREV_CODE_SIZE();
					d->m_huff_count[2][code_size] = (iw_uint16)(d->m_huff_count[2][code_size] + 1);
					packed_code_sizes[num_packed_code_sizes++] = code_size;
				}
				else if (++rle_repeat_count == 6)
				{
					iwc_z_RLE_PREV_CODE_SIZE();
				}
			}
			prev_code_size = code_size;
		}
		if (rle_repeat_count)
		{
			iwc_z_RLE_PREV_CODE_SIZE();
		}
		else
		{
			iwc_z_RLE_ZERO_CODE_SIZE();
		}
		
		iwc_z_optimize_huffman_table(d, 2, iwc_z_MAX_HUFF_SYMBOLS_2, 7, IW_FALSE);
		
		iwc_z_PUT_BITS(2, 2);
		
		iwc_z_PUT_BITS(num_lit_codes - 257, 5);
		iwc_z_PUT_BITS(num_dist_codes - 1, 5);
		
		for (num_bit_lengths = 18; num_bit_lengths >= 0; num_bit_lengths--)
			if (d->m_huff_code_sizes[2][s_iwc_z_packed_code_size_syms_swizzle[num_bit_lengths]])
				break;
		num_bit_lengths = IW_MAX(4, (num_bit_lengths + 1));
		iwc_z_PUT_BITS(num_bit_lengths - 4, 4);
		for (i = 0; (int)i < num_bit_lengths; i++)
			iwc_z_PUT_BITS(d->m_huff_code_sizes[2][s_iwc_z_packed_code_size_syms_swizzle[i]], 3);
		
		for (packed_code_sizes_index = 0; packed_code_sizes_index < num_packed_code_sizes;)
		{
			iw_uint code = packed_code_sizes[packed_code_sizes_index++];
			IW_ASSERT(code < iwc_z_MAX_HUFF_SYMBOLS_2);
			iwc_z_PUT_BITS(d->m_huff_codes[2][code], d->m_huff_code_sizes[2][code]);
			if (code >= 16)
				iwc_z_PUT_BITS(packed_code_sizes[packed_code_sizes_index++], "\02\03\07"[code - 16]);
		}
	}
	
	static void iwc_z_start_static_block(iwc_z_compressor *d)
	{
		iw_uint i;
		iw_uint8 *p = &d->m_huff_code_sizes[0][0];
		
		for (i = 0; i <= 143; ++i)
			*p++ = 8;
		for (; i <= 255; ++i)
			*p++ = 9;
		for (; i <= 279; ++i)
			*p++ = 7;
		for (; i <= 287; ++i)
			*p++ = 8;
		
		memset(d->m_huff_code_sizes[1], 5, 32);
		
		iwc_z_optimize_huffman_table(d, 0, 288, 15, IW_TRUE);
		iwc_z_optimize_huffman_table(d, 1, 32, 15, IW_TRUE);
		
		iwc_z_PUT_BITS(1, 2);
	}
	
	static const iw_uint iw_bitmasks[17] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };
	
#if i2WC_USE_UNALIGNED_LOADS_AND_STORES && i2WC_LITTLE_ENDIAN && i2WC_HAS_64BIT_REGISTERS
	static iw_bool iwc_z_compress_lz_codes(iwc_z_compressor *d)
	{
		iw_uint flags;
		iw_uint8 *pLZ_codes;
		iw_uint8 *pOutput_buf = d->m_pOutput_buf;
		iw_uint8 *pLZ_code_buf_end = d->m_pLZ_code_buf;
		iw_uint64 bit_buffer = d->m_bit_buffer;
		iw_uint bits_in = d->m_bits_in;
		
#define iwc_z_PUT_BITS_FAST(b, l)                    \
		{                                                \
		bit_buffer |= (((iw_uint64)(b)) << bits_in); \
		bits_in += (l);                              \
		}
		
		flags = 1;
		for (pLZ_codes = d->m_lz_code_buf; pLZ_codes < pLZ_code_buf_end; flags >>= 1)
		{
			if (flags == 1)
				flags = *pLZ_codes++ | 0x100;
			
			if (flags & 1)
			{
				iw_uint s0, s1, n0, n1, sym, num_extra_bits;
				iw_uint match_len = pLZ_codes[0];
				iw_uint match_dist = (pLZ_codes[1] | (pLZ_codes[2] << 8));
				pLZ_codes += 3;
				
				IW_ASSERT(d->m_huff_code_sizes[0][s_iwc_z_len_sym[match_len]]);
				iwc_z_PUT_BITS_FAST(d->m_huff_codes[0][s_iwc_z_len_sym[match_len]], d->m_huff_code_sizes[0][s_iwc_z_len_sym[match_len]]);
				iwc_z_PUT_BITS_FAST(match_len & iw_bitmasks[s_iwc_z_len_extra[match_len]], s_iwc_z_len_extra[match_len]);
				
				s0 = s_iwc_z_small_dist_sym[match_dist & 511];
				n0 = s_iwc_z_small_dist_extra[match_dist & 511];
				s1 = s_iwc_z_large_dist_sym[match_dist >> 8];
				n1 = s_iwc_z_large_dist_extra[match_dist >> 8];
				sym = (match_dist < 512) ? s0 : s1;
				num_extra_bits = (match_dist < 512) ? n0 : n1;
				
				IW_ASSERT(d->m_huff_code_sizes[1][sym]);
				iwc_z_PUT_BITS_FAST(d->m_huff_codes[1][sym], d->m_huff_code_sizes[1][sym]);
				iwc_z_PUT_BITS_FAST(match_dist & iw_bitmasks[num_extra_bits], num_extra_bits);
			}
			else
			{
				iw_uint lit = *pLZ_codes++;
				IW_ASSERT(d->m_huff_code_sizes[0][lit]);
				iwc_z_PUT_BITS_FAST(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
				
				if (((flags & 2) == 0) && (pLZ_codes < pLZ_code_buf_end))
				{
					flags >>= 1;
					lit = *pLZ_codes++;
					IW_ASSERT(d->m_huff_code_sizes[0][lit]);
					iwc_z_PUT_BITS_FAST(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
					
					if (((flags & 2) == 0) && (pLZ_codes < pLZ_code_buf_end))
					{
						flags >>= 1;
						lit = *pLZ_codes++;
						IW_ASSERT(d->m_huff_code_sizes[0][lit]);
						iwc_z_PUT_BITS_FAST(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
					}
				}
			}
			
			if (pOutput_buf >= d->m_pOutput_buf_end)
				return IW_FALSE;
			
			memcpy(pOutput_buf, &bit_buffer, sizeof(iw_uint64));
			pOutput_buf += (bits_in >> 3);
			bit_buffer >>= (bits_in & ~7);
			bits_in &= 7;
		}
		
#undef iwc_z_PUT_BITS_FAST
		
		d->m_pOutput_buf = pOutput_buf;
		d->m_bits_in = 0;
		d->m_bit_buffer = 0;
		
		while (bits_in)
		{
			iw_uint32 n = IW_MIN(bits_in, 16);
			iwc_z_PUT_BITS((iw_uint)bit_buffer & iw_bitmasks[n], n);
			bit_buffer >>= n;
			bits_in -= n;
		}
		
		iwc_z_PUT_BITS(d->m_huff_codes[0][256], d->m_huff_code_sizes[0][256]);
		
		return (d->m_pOutput_buf < d->m_pOutput_buf_end);
	}
#else
	static iw_bool iwc_z_compress_lz_codes(iwc_z_compressor *d)
	{
		iw_uint flags;
		iw_uint8 *pLZ_codes;
		
		flags = 1;
		for (pLZ_codes = d->m_lz_code_buf; pLZ_codes < d->m_pLZ_code_buf; flags >>= 1)
		{
			if (flags == 1)
				flags = *pLZ_codes++ | 0x100;
			if (flags & 1)
			{
				iw_uint sym, num_extra_bits;
				iw_uint match_len = pLZ_codes[0], match_dist = (pLZ_codes[1] | (pLZ_codes[2] << 8));
				pLZ_codes += 3;
				
				IW_ASSERT(d->m_huff_code_sizes[0][s_iwc_z_len_sym[match_len]]);
				iwc_z_PUT_BITS(d->m_huff_codes[0][s_iwc_z_len_sym[match_len]], d->m_huff_code_sizes[0][s_iwc_z_len_sym[match_len]]);
				iwc_z_PUT_BITS(match_len & iw_bitmasks[s_iwc_z_len_extra[match_len]], s_iwc_z_len_extra[match_len]);
				
				if (match_dist < 512)
				{
					sym = s_iwc_z_small_dist_sym[match_dist];
					num_extra_bits = s_iwc_z_small_dist_extra[match_dist];
				}
				else
				{
					sym = s_iwc_z_large_dist_sym[match_dist >> 8];
					num_extra_bits = s_iwc_z_large_dist_extra[match_dist >> 8];
				}
				IW_ASSERT(d->m_huff_code_sizes[1][sym]);
				iwc_z_PUT_BITS(d->m_huff_codes[1][sym], d->m_huff_code_sizes[1][sym]);
				iwc_z_PUT_BITS(match_dist & iw_bitmasks[num_extra_bits], num_extra_bits);
			}
			else
			{
				iw_uint lit = *pLZ_codes++;
				IW_ASSERT(d->m_huff_code_sizes[0][lit]);
				iwc_z_PUT_BITS(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
			}
		}
		
		iwc_z_PUT_BITS(d->m_huff_codes[0][256], d->m_huff_code_sizes[0][256]);
		
		return (d->m_pOutput_buf < d->m_pOutput_buf_end);
	}
#endif
	
	static iw_bool iwc_z_compress_block(iwc_z_compressor *d, iw_bool static_block)
	{
		if (static_block)
			iwc_z_start_static_block(d);
		else
			iwc_z_start_dynamic_block(d);
		return iwc_z_compress_lz_codes(d);
	}
	
	static const iw_uint s_iwc_z_num_probes[11] = { 0, 1, 6, 32, 16, 32, 128, 256, 512, 768, 1500 };
	
	static int iwc_z_flush_block(iwc_z_compressor *d, int flush)
	{
		iw_uint saved_bit_buf, saved_bits_in;
		iw_uint8 *pSaved_output_buf;
		iw_bool comp_block_succeeded = IW_FALSE;
		int n, use_raw_block = ((d->m_flags & iwc_z_FORCE_ALL_RAW_BLOCKS) != 0) && (d->m_lookahead_pos - d->m_lz_code_buf_dict_pos) <= d->m_dict_size;
		iw_uint8 *pOutput_buf_start = ((d->m_pPut_buf_func == NULL) && ((*d->m_pOut_buf_size - d->m_out_buf_ofs) >= iwc_z_OUT_BUF_SIZE)) ? ((iw_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs) : d->m_output_buf;
		
		d->m_pOutput_buf = pOutput_buf_start;
		d->m_pOutput_buf_end = d->m_pOutput_buf + iwc_z_OUT_BUF_SIZE - 16;
		
		IW_ASSERT(!d->m_output_flush_remaining);
		d->m_output_flush_ofs = 0;
		d->m_output_flush_remaining = 0;
		
		*d->m_pLZ_flags = (iw_uint8)(*d->m_pLZ_flags >> d->m_num_flags_left);
		d->m_pLZ_code_buf -= (d->m_num_flags_left == 8);
		
		if ((d->m_flags & iwc_z_WRITE_ZLIB_HEADER) && (!d->m_block_index))
		{
			const iw_uint8 cmf = 0x78;
			iw_uint8 flg, flevel = 3;
			iw_uint header, i, iw_un = sizeof(s_iwc_z_num_probes) / sizeof(iw_uint);
			
			for (i = 0; i < iw_un; i++)
				if (s_iwc_z_num_probes[i] == (d->m_flags & 0xFFF))
					break;
			
			if (i < 2)
				flevel = 0;
			else if (i < 6)
				flevel = 1;
			else if (i == 6)
				flevel = 2;
			
			header = cmf << 8 | (flevel << 6);
			header += 31 - (header % 31);
			flg = header & 0xFF;
			
			iwc_z_PUT_BITS(cmf, 8);
			iwc_z_PUT_BITS(flg, 8);
		}
		
		iwc_z_PUT_BITS(flush == iwc_z_FINISH, 1);
		
		pSaved_output_buf = d->m_pOutput_buf;
		saved_bit_buf = d->m_bit_buffer;
		saved_bits_in = d->m_bits_in;
		
		if (!use_raw_block)
			comp_block_succeeded = iwc_z_compress_block(d, (d->m_flags & iwc_z_FORCE_ALL_STATIC_BLOCKS) || (d->m_total_lz_bytes < 48));
		
		if (((use_raw_block) || ((d->m_total_lz_bytes) && ((d->m_pOutput_buf - pSaved_output_buf + 1U) >= d->m_total_lz_bytes))) &&
			((d->m_lookahead_pos - d->m_lz_code_buf_dict_pos) <= d->m_dict_size))
		{
			iw_uint i;
			d->m_pOutput_buf = pSaved_output_buf;
			d->m_bit_buffer = saved_bit_buf, d->m_bits_in = saved_bits_in;
			iwc_z_PUT_BITS(0, 2);
			if (d->m_bits_in)
			{
				iwc_z_PUT_BITS(0, 8 - d->m_bits_in);
			}
			for (i = 2; i; --i, d->m_total_lz_bytes ^= 0xFFFF)
			{
				iwc_z_PUT_BITS(d->m_total_lz_bytes & 0xFFFF, 16);
			}
			for (i = 0; i < d->m_total_lz_bytes; ++i)
			{
				iwc_z_PUT_BITS(d->m_dict[(d->m_lz_code_buf_dict_pos + i) & iwc_z_LZ_DICT_SIZE_MASK], 8);
			}
		}
		else if (!comp_block_succeeded)
		{
			d->m_pOutput_buf = pSaved_output_buf;
			d->m_bit_buffer = saved_bit_buf, d->m_bits_in = saved_bits_in;
			iwc_z_compress_block(d, IW_TRUE);
		}
		
		if (flush)
		{
			if (flush == iwc_z_FINISH)
			{
				if (d->m_bits_in)
				{
					iwc_z_PUT_BITS(0, 8 - d->m_bits_in);
				}
				if (d->m_flags & iwc_z_WRITE_ZLIB_HEADER)
				{
					iw_uint i, a = d->m_adler32;
					for (i = 0; i < 4; i++)
					{
						iwc_z_PUT_BITS((a >> 24) & 0xFF, 8);
						a <<= 8;
					}
				}
			}
			else
			{
				iw_uint i, z = 0;
				iwc_z_PUT_BITS(0, 3);
				if (d->m_bits_in)
				{
					iwc_z_PUT_BITS(0, 8 - d->m_bits_in);
				}
				for (i = 2; i; --i, z ^= 0xFFFF)
				{
					iwc_z_PUT_BITS(z & 0xFFFF, 16);
				}
			}
		}
		
		IW_ASSERT(d->m_pOutput_buf < d->m_pOutput_buf_end);
		
		memset(&d->m_huff_count[0][0], 0, sizeof(d->m_huff_count[0][0]) * iwc_z_MAX_HUFF_SYMBOLS_0);
		memset(&d->m_huff_count[1][0], 0, sizeof(d->m_huff_count[1][0]) * iwc_z_MAX_HUFF_SYMBOLS_1);
		
		d->m_pLZ_code_buf = d->m_lz_code_buf + 1;
		d->m_pLZ_flags = d->m_lz_code_buf;
		d->m_num_flags_left = 8;
		d->m_lz_code_buf_dict_pos += d->m_total_lz_bytes;
		d->m_total_lz_bytes = 0;
		d->m_block_index++;
		
		if ((n = (int)(d->m_pOutput_buf - pOutput_buf_start)) != 0)
		{
			if (d->m_pPut_buf_func)
			{
				*d->m_pIn_buf_size = d->m_pSrc - (const iw_uint8 *)d->m_pIn_buf;
				if (!(*d->m_pPut_buf_func)(d->m_output_buf, n, d->m_pPut_buf_user))
					return (d->m_prev_return_status = iwc_z_STATUS_PUT_BUF_FAILED);
			}
			else if (pOutput_buf_start == d->m_output_buf)
			{
				int bytes_to_copy = (int)IW_MIN((size_t)n, (size_t)(*d->m_pOut_buf_size - d->m_out_buf_ofs));
				memcpy((iw_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs, d->m_output_buf, bytes_to_copy);
				d->m_out_buf_ofs += bytes_to_copy;
				if ((n -= bytes_to_copy) != 0)
				{
					d->m_output_flush_ofs = bytes_to_copy;
					d->m_output_flush_remaining = n;
				}
			}
			else
			{
				d->m_out_buf_ofs += n;
			}
		}
		
		return d->m_output_flush_remaining;
	}
	
#if i2WC_USE_UNALIGNED_LOADS_AND_STORES
#ifdef i2WC_UNALIGNED_USE_MEMCPY
	static iw_uint16 iwc_z_READ_UNALIGNED_WORD(const iw_uint8 *p)
	{
		iw_uint16 ret;
		memcpy(&ret, p, sizeof(iw_uint16));
		return ret;
	}
	static iw_uint16 iwc_z_READ_UNALIGNED_WORD2(const iw_uint16 *p)
	{
		iw_uint16 ret;
		memcpy(&ret, p, sizeof(iw_uint16));
		return ret;
	}
#else
#define iwc_z_READ_UNALIGNED_WORD(p) *(const iw_uint16 *)(p)
#define iwc_z_READ_UNALIGNED_WORD2(p) *(const iw_uint16 *)(p)
#endif
	static IW_FORCEINLINE void iwc_z_find_match(iwc_z_compressor *d, iw_uint lookahead_pos, iw_uint max_dist, iw_uint max_match_len, iw_uint *pMatch_dist, iw_uint *pMatch_len)
	{
		iw_uint dist, pos = lookahead_pos & iwc_z_LZ_DICT_SIZE_MASK, match_len = *pMatch_len, probe_pos = pos, next_probe_pos, probe_len;
		iw_uint num_probes_left = d->m_max_probes[match_len >= 32];
		const iw_uint16 *s = (const iw_uint16 *)(d->m_dict + pos), *p, *q;
		iw_uint16 c01 = iwc_z_READ_UNALIGNED_WORD(&d->m_dict[pos + match_len - 1]), s01 = iwc_z_READ_UNALIGNED_WORD2(s);
		IW_ASSERT(max_match_len <= iwc_z_MAX_MATCH_LEN);
		if (max_match_len <= match_len)
			return;
		for (;;)
		{
			for (;;)
			{
				if (--num_probes_left == 0)
					return;
#define iwc_z_PROBE                                                                             \
				next_probe_pos = d->m_next[probe_pos];                                                      \
				if ((!next_probe_pos) || ((dist = (iw_uint16)(lookahead_pos - next_probe_pos)) > max_dist)) \
				return;                                                                                 \
				probe_pos = next_probe_pos & iwc_z_LZ_DICT_SIZE_MASK;                                       \
				if (iwc_z_READ_UNALIGNED_WORD(&d->m_dict[probe_pos + match_len - 1]) == c01)                \
				break;
				iwc_z_PROBE;
				iwc_z_PROBE;
				iwc_z_PROBE;
			}
			if (!dist)
				break;
			q = (const iw_uint16 *)(d->m_dict + probe_pos);
			if (iwc_z_READ_UNALIGNED_WORD2(q) != s01)
				continue;
			p = s;
			probe_len = 32;
			do
			{
			} while ((iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) && (iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) &&
					 (iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) && (iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) && (--probe_len > 0));
			if (!probe_len)
			{
				*pMatch_dist = dist;
				*pMatch_len = IW_MIN(max_match_len, (iw_uint)iwc_z_MAX_MATCH_LEN);
				break;
			}
			else if ((probe_len = ((iw_uint)(p - s) * 2) + (iw_uint)(*(const iw_uint8 *)p == *(const iw_uint8 *)q)) > match_len)
			{
				*pMatch_dist = dist;
				if ((*pMatch_len = match_len = IW_MIN(max_match_len, probe_len)) == max_match_len)
					break;
				c01 = iwc_z_READ_UNALIGNED_WORD(&d->m_dict[pos + match_len - 1]);
			}
		}
	}
#else
	static IW_FORCEINLINE void iwc_z_find_match(iwc_z_compressor *d, iw_uint lookahead_pos, iw_uint max_dist, iw_uint max_match_len, iw_uint *pMatch_dist, iw_uint *pMatch_len)
	{
		iw_uint dist, pos = lookahead_pos & iwc_z_LZ_DICT_SIZE_MASK, match_len = *pMatch_len, probe_pos = pos, next_probe_pos, probe_len;
		iw_uint num_probes_left = d->m_max_probes[match_len >= 32];
		const iw_uint8 *s = d->m_dict + pos, *p, *q;
		iw_uint8 c0 = d->m_dict[pos + match_len], c1 = d->m_dict[pos + match_len - 1];
		IW_ASSERT(max_match_len <= iwc_z_MAX_MATCH_LEN);
		if (max_match_len <= match_len)
			return;
		for (;;)
		{
			for (;;)
			{
				if (--num_probes_left == 0)
					return;
#define iwc_z_PROBE                                                                               \
				next_probe_pos = d->m_next[probe_pos];                                                        \
				if ((!next_probe_pos) || ((dist = (iw_uint16)(lookahead_pos - next_probe_pos)) > max_dist))   \
				return;                                                                                   \
				probe_pos = next_probe_pos & iwc_z_LZ_DICT_SIZE_MASK;                                         \
				if ((d->m_dict[probe_pos + match_len] == c0) && (d->m_dict[probe_pos + match_len - 1] == c1)) \
				break;
				iwc_z_PROBE;
				iwc_z_PROBE;
				iwc_z_PROBE;
			}
			if (!dist)
				break;
			p = s;
			q = d->m_dict + probe_pos;
			for (probe_len = 0; probe_len < max_match_len; probe_len++)
				if (*p++ != *q++)
					break;
			if (probe_len > match_len)
			{
				*pMatch_dist = dist;
				if ((*pMatch_len = match_len = probe_len) == max_match_len)
					return;
				c0 = d->m_dict[pos + match_len];
				c1 = d->m_dict[pos + match_len - 1];
			}
		}
	}
#endif
	
#if i2WC_USE_UNALIGNED_LOADS_AND_STORES && i2WC_LITTLE_ENDIAN
#ifdef i2WC_UNALIGNED_USE_MEMCPY
	static iw_uint32 iwc_z_READ_UNALIGNED_WORD32(const iw_uint8 *p)
	{
		iw_uint32 ret;
		memcpy(&ret, p, sizeof(iw_uint32));
		return ret;
	}
#else
#define iwc_z_READ_UNALIGNED_WORD32(p) *(const iw_uint32 *)(p)
#endif
	static iw_bool iwc_z_compress_fast(iwc_z_compressor *d)
	{
		iw_uint lookahead_pos = d->m_lookahead_pos, lookahead_size = d->m_lookahead_size, dict_size = d->m_dict_size, total_lz_bytes = d->m_total_lz_bytes, num_flags_left = d->m_num_flags_left;
		iw_uint8 *pLZ_code_buf = d->m_pLZ_code_buf, *pLZ_flags = d->m_pLZ_flags;
		iw_uint cur_pos = lookahead_pos & iwc_z_LZ_DICT_SIZE_MASK;
		
		while ((d->m_src_buf_left) || ((d->m_flush) && (lookahead_size)))
		{
			const iw_uint iwc_z_COMP_FAST_LOOKAHEAD_SIZE = 4096;
			iw_uint dst_pos = (lookahead_pos + lookahead_size) & iwc_z_LZ_DICT_SIZE_MASK;
			iw_uint num_bytes_to_process = (iw_uint)IW_MIN(d->m_src_buf_left, iwc_z_COMP_FAST_LOOKAHEAD_SIZE - lookahead_size);
			d->m_src_buf_left -= num_bytes_to_process;
			lookahead_size += num_bytes_to_process;
			
			while (num_bytes_to_process)
			{
				iw_uint32 n = IW_MIN(iwc_z_LZ_DICT_SIZE - dst_pos, num_bytes_to_process);
				memcpy(d->m_dict + dst_pos, d->m_pSrc, n);
				if (dst_pos < (iwc_z_MAX_MATCH_LEN - 1))
					memcpy(d->m_dict + iwc_z_LZ_DICT_SIZE + dst_pos, d->m_pSrc, IW_MIN(n, (iwc_z_MAX_MATCH_LEN - 1) - dst_pos));
				d->m_pSrc += n;
				dst_pos = (dst_pos + n) & iwc_z_LZ_DICT_SIZE_MASK;
				num_bytes_to_process -= n;
			}
			
			dict_size = IW_MIN(iwc_z_LZ_DICT_SIZE - lookahead_size, dict_size);
			if ((!d->m_flush) && (lookahead_size < iwc_z_COMP_FAST_LOOKAHEAD_SIZE))
				break;
			
			while (lookahead_size >= 4)
			{
				iw_uint cur_match_dist, cur_match_len = 1;
				iw_uint8 *pCur_dict = d->m_dict + cur_pos;
				iw_uint first_trigram = iwc_z_READ_UNALIGNED_WORD32(pCur_dict) & 0xFFFFFF;
				iw_uint hash = (first_trigram ^ (first_trigram >> (24 - (iwc_z_LZ_HASH_BITS - 8)))) & iwc_z_LEVEL1_HASH_SIZE_MASK;
				iw_uint probe_pos = d->m_hash[hash];
				d->m_hash[hash] = (iw_uint16)lookahead_pos;
				
				if (((cur_match_dist = (iw_uint16)(lookahead_pos - probe_pos)) <= dict_size) && ((iwc_z_READ_UNALIGNED_WORD32(d->m_dict + (probe_pos &= iwc_z_LZ_DICT_SIZE_MASK)) & 0xFFFFFF) == first_trigram))
				{
					const iw_uint16 *p = (const iw_uint16 *)pCur_dict;
					const iw_uint16 *q = (const iw_uint16 *)(d->m_dict + probe_pos);
					iw_uint32 probe_len = 32;
					do
					{
					} while ((iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) && (iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) &&
							 (iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) && (iwc_z_READ_UNALIGNED_WORD2(++p) == iwc_z_READ_UNALIGNED_WORD2(++q)) && (--probe_len > 0));
					cur_match_len = ((iw_uint)(p - (const iw_uint16 *)pCur_dict) * 2) + (iw_uint)(*(const iw_uint8 *)p == *(const iw_uint8 *)q);
					if (!probe_len)
						cur_match_len = cur_match_dist ? iwc_z_MAX_MATCH_LEN : 0;
					
					if ((cur_match_len < iwc_z_MIN_MATCH_LEN) || ((cur_match_len == iwc_z_MIN_MATCH_LEN) && (cur_match_dist >= 8U * 1024U)))
					{
						cur_match_len = 1;
						*pLZ_code_buf++ = (iw_uint8)first_trigram;
						*pLZ_flags = (iw_uint8)(*pLZ_flags >> 1);
						d->m_huff_count[0][(iw_uint8)first_trigram]++;
					}
					else
					{
						iw_uint32 s0, s1;
						cur_match_len = IW_MIN(cur_match_len, lookahead_size);
						
						IW_ASSERT((cur_match_len >= iwc_z_MIN_MATCH_LEN) && (cur_match_dist >= 1) && (cur_match_dist <= iwc_z_LZ_DICT_SIZE));
						
						cur_match_dist--;
						
						pLZ_code_buf[0] = (iw_uint8)(cur_match_len - iwc_z_MIN_MATCH_LEN);
#ifdef i2WC_UNALIGNED_USE_MEMCPY
						memcpy(&pLZ_code_buf[1], &cur_match_dist, sizeof(cur_match_dist));
#else
						*(iw_uint16 *)(&pLZ_code_buf[1]) = (iw_uint16)cur_match_dist;
#endif
						pLZ_code_buf += 3;
						*pLZ_flags = (iw_uint8)((*pLZ_flags >> 1) | 0x80);
						
						s0 = s_iwc_z_small_dist_sym[cur_match_dist & 511];
						s1 = s_iwc_z_large_dist_sym[cur_match_dist >> 8];
						d->m_huff_count[1][(cur_match_dist < 512) ? s0 : s1]++;
						
						d->m_huff_count[0][s_iwc_z_len_sym[cur_match_len - iwc_z_MIN_MATCH_LEN]]++;
					}
				}
				else
				{
					*pLZ_code_buf++ = (iw_uint8)first_trigram;
					*pLZ_flags = (iw_uint8)(*pLZ_flags >> 1);
					d->m_huff_count[0][(iw_uint8)first_trigram]++;
				}
				
				if (--num_flags_left == 0)
				{
					num_flags_left = 8;
					pLZ_flags = pLZ_code_buf++;
				}
				
				total_lz_bytes += cur_match_len;
				lookahead_pos += cur_match_len;
				dict_size = IW_MIN(dict_size + cur_match_len, (iw_uint)iwc_z_LZ_DICT_SIZE);
				cur_pos = (cur_pos + cur_match_len) & iwc_z_LZ_DICT_SIZE_MASK;
				IW_ASSERT(lookahead_size >= cur_match_len);
				lookahead_size -= cur_match_len;
				
				if (pLZ_code_buf > &d->m_lz_code_buf[iwc_z_LZ_CODE_BUF_SIZE - 8])
				{
					int n;
					d->m_lookahead_pos = lookahead_pos;
					d->m_lookahead_size = lookahead_size;
					d->m_dict_size = dict_size;
					d->m_total_lz_bytes = total_lz_bytes;
					d->m_pLZ_code_buf = pLZ_code_buf;
					d->m_pLZ_flags = pLZ_flags;
					d->m_num_flags_left = num_flags_left;
					if ((n = iwc_z_flush_block(d, 0)) != 0)
						return (n < 0) ? IW_FALSE : IW_TRUE;
					total_lz_bytes = d->m_total_lz_bytes;
					pLZ_code_buf = d->m_pLZ_code_buf;
					pLZ_flags = d->m_pLZ_flags;
					num_flags_left = d->m_num_flags_left;
				}
			}
			
			while (lookahead_size)
			{
				iw_uint8 lit = d->m_dict[cur_pos];
				
				total_lz_bytes++;
				*pLZ_code_buf++ = lit;
				*pLZ_flags = (iw_uint8)(*pLZ_flags >> 1);
				if (--num_flags_left == 0)
				{
					num_flags_left = 8;
					pLZ_flags = pLZ_code_buf++;
				}
				
				d->m_huff_count[0][lit]++;
				
				lookahead_pos++;
				dict_size = IW_MIN(dict_size + 1, (iw_uint)iwc_z_LZ_DICT_SIZE);
				cur_pos = (cur_pos + 1) & iwc_z_LZ_DICT_SIZE_MASK;
				lookahead_size--;
				
				if (pLZ_code_buf > &d->m_lz_code_buf[iwc_z_LZ_CODE_BUF_SIZE - 8])
				{
					int n;
					d->m_lookahead_pos = lookahead_pos;
					d->m_lookahead_size = lookahead_size;
					d->m_dict_size = dict_size;
					d->m_total_lz_bytes = total_lz_bytes;
					d->m_pLZ_code_buf = pLZ_code_buf;
					d->m_pLZ_flags = pLZ_flags;
					d->m_num_flags_left = num_flags_left;
					if ((n = iwc_z_flush_block(d, 0)) != 0)
						return (n < 0) ? IW_FALSE : IW_TRUE;
					total_lz_bytes = d->m_total_lz_bytes;
					pLZ_code_buf = d->m_pLZ_code_buf;
					pLZ_flags = d->m_pLZ_flags;
					num_flags_left = d->m_num_flags_left;
				}
			}
		}
		
		d->m_lookahead_pos = lookahead_pos;
		d->m_lookahead_size = lookahead_size;
		d->m_dict_size = dict_size;
		d->m_total_lz_bytes = total_lz_bytes;
		d->m_pLZ_code_buf = pLZ_code_buf;
		d->m_pLZ_flags = pLZ_flags;
		d->m_num_flags_left = num_flags_left;
		return IW_TRUE;
	}
#endif
	
	static IW_FORCEINLINE void iwc_z_record_literal(iwc_z_compressor *d, iw_uint8 lit)
	{
		d->m_total_lz_bytes++;
		*d->m_pLZ_code_buf++ = lit;
		*d->m_pLZ_flags = (iw_uint8)(*d->m_pLZ_flags >> 1);
		if (--d->m_num_flags_left == 0)
		{
			d->m_num_flags_left = 8;
			d->m_pLZ_flags = d->m_pLZ_code_buf++;
		}
		d->m_huff_count[0][lit]++;
	}
	
	static IW_FORCEINLINE void iwc_z_record_match(iwc_z_compressor *d, iw_uint match_len, iw_uint match_dist)
	{
		iw_uint32 s0, s1;
		
		IW_ASSERT((match_len >= iwc_z_MIN_MATCH_LEN) && (match_dist >= 1) && (match_dist <= iwc_z_LZ_DICT_SIZE));
		
		d->m_total_lz_bytes += match_len;
		
		d->m_pLZ_code_buf[0] = (iw_uint8)(match_len - iwc_z_MIN_MATCH_LEN);
		
		match_dist -= 1;
		d->m_pLZ_code_buf[1] = (iw_uint8)(match_dist & 0xFF);
		d->m_pLZ_code_buf[2] = (iw_uint8)(match_dist >> 8);
		d->m_pLZ_code_buf += 3;
		
		*d->m_pLZ_flags = (iw_uint8)((*d->m_pLZ_flags >> 1) | 0x80);
		if (--d->m_num_flags_left == 0)
		{
			d->m_num_flags_left = 8;
			d->m_pLZ_flags = d->m_pLZ_code_buf++;
		}
		
		s0 = s_iwc_z_small_dist_sym[match_dist & 511];
		s1 = s_iwc_z_large_dist_sym[(match_dist >> 8) & 127];
		d->m_huff_count[1][(match_dist < 512) ? s0 : s1]++;
		d->m_huff_count[0][s_iwc_z_len_sym[match_len - iwc_z_MIN_MATCH_LEN]]++;
	}
	
	static iw_bool iwc_z_compress_normal(iwc_z_compressor *d)
	{
		const iw_uint8 *pSrc = d->m_pSrc;
		size_t src_buf_left = d->m_src_buf_left;
		iwc_z_flush flush = d->m_flush;
		
		while ((src_buf_left) || ((flush) && (d->m_lookahead_size)))
		{
			iw_uint len_to_move, cur_match_dist, cur_match_len, cur_pos;
			if ((d->m_lookahead_size + d->m_dict_size) >= (iwc_z_MIN_MATCH_LEN - 1))
			{
				iw_uint dst_pos = (d->m_lookahead_pos + d->m_lookahead_size) & iwc_z_LZ_DICT_SIZE_MASK, ins_pos = d->m_lookahead_pos + d->m_lookahead_size - 2;
				iw_uint hash = (d->m_dict[ins_pos & iwc_z_LZ_DICT_SIZE_MASK] << iwc_z_LZ_HASH_SHIFT) ^ d->m_dict[(ins_pos + 1) & iwc_z_LZ_DICT_SIZE_MASK];
				iw_uint num_bytes_to_process = (iw_uint)IW_MIN(src_buf_left, iwc_z_MAX_MATCH_LEN - d->m_lookahead_size);
				const iw_uint8 *pSrc_end = pSrc ? pSrc + num_bytes_to_process : NULL;
				src_buf_left -= num_bytes_to_process;
				d->m_lookahead_size += num_bytes_to_process;
				while (pSrc != pSrc_end)
				{
					iw_uint8 c = *pSrc++;
					d->m_dict[dst_pos] = c;
					if (dst_pos < (iwc_z_MAX_MATCH_LEN - 1))
						d->m_dict[iwc_z_LZ_DICT_SIZE + dst_pos] = c;
					hash = ((hash << iwc_z_LZ_HASH_SHIFT) ^ c) & (iwc_z_LZ_HASH_SIZE - 1);
					d->m_next[ins_pos & iwc_z_LZ_DICT_SIZE_MASK] = d->m_hash[hash];
					d->m_hash[hash] = (iw_uint16)(ins_pos);
					dst_pos = (dst_pos + 1) & iwc_z_LZ_DICT_SIZE_MASK;
					ins_pos++;
				}
			}
			else
			{
				while ((src_buf_left) && (d->m_lookahead_size < iwc_z_MAX_MATCH_LEN))
				{
					iw_uint8 c = *pSrc++;
					iw_uint dst_pos = (d->m_lookahead_pos + d->m_lookahead_size) & iwc_z_LZ_DICT_SIZE_MASK;
					src_buf_left--;
					d->m_dict[dst_pos] = c;
					if (dst_pos < (iwc_z_MAX_MATCH_LEN - 1))
						d->m_dict[iwc_z_LZ_DICT_SIZE + dst_pos] = c;
					if ((++d->m_lookahead_size + d->m_dict_size) >= iwc_z_MIN_MATCH_LEN)
					{
						iw_uint ins_pos = d->m_lookahead_pos + (d->m_lookahead_size - 1) - 2;
						iw_uint hash = ((d->m_dict[ins_pos & iwc_z_LZ_DICT_SIZE_MASK] << (iwc_z_LZ_HASH_SHIFT * 2)) ^ (d->m_dict[(ins_pos + 1) & iwc_z_LZ_DICT_SIZE_MASK] << iwc_z_LZ_HASH_SHIFT) ^ c) & (iwc_z_LZ_HASH_SIZE - 1);
						d->m_next[ins_pos & iwc_z_LZ_DICT_SIZE_MASK] = d->m_hash[hash];
						d->m_hash[hash] = (iw_uint16)(ins_pos);
					}
				}
			}
			d->m_dict_size = IW_MIN(iwc_z_LZ_DICT_SIZE - d->m_lookahead_size, d->m_dict_size);
			if ((!flush) && (d->m_lookahead_size < iwc_z_MAX_MATCH_LEN))
				break;
			
			len_to_move = 1;
			cur_match_dist = 0;
			cur_match_len = d->m_saved_match_len ? d->m_saved_match_len : (iwc_z_MIN_MATCH_LEN - 1);
			cur_pos = d->m_lookahead_pos & iwc_z_LZ_DICT_SIZE_MASK;
			if (d->m_flags & (iwc_z_RLE_MATCHES | iwc_z_FORCE_ALL_RAW_BLOCKS))
			{
				if ((d->m_dict_size) && (!(d->m_flags & iwc_z_FORCE_ALL_RAW_BLOCKS)))
				{
					iw_uint8 c = d->m_dict[(cur_pos - 1) & iwc_z_LZ_DICT_SIZE_MASK];
					cur_match_len = 0;
					while (cur_match_len < d->m_lookahead_size)
					{
						if (d->m_dict[cur_pos + cur_match_len] != c)
							break;
						cur_match_len++;
					}
					if (cur_match_len < iwc_z_MIN_MATCH_LEN)
						cur_match_len = 0;
					else
						cur_match_dist = 1;
				}
			}
			else
			{
				iwc_z_find_match(d, d->m_lookahead_pos, d->m_dict_size, d->m_lookahead_size, &cur_match_dist, &cur_match_len);
			}
			if (((cur_match_len == iwc_z_MIN_MATCH_LEN) && (cur_match_dist >= 8U * 1024U)) || (cur_pos == cur_match_dist) || ((d->m_flags & iwc_z_FILTER_MATCHES) && (cur_match_len <= 5)))
			{
				cur_match_dist = cur_match_len = 0;
			}
			if (d->m_saved_match_len)
			{
				if (cur_match_len > d->m_saved_match_len)
				{
					iwc_z_record_literal(d, (iw_uint8)d->m_saved_lit);
					if (cur_match_len >= 128)
					{
						iwc_z_record_match(d, cur_match_len, cur_match_dist);
						d->m_saved_match_len = 0;
						len_to_move = cur_match_len;
					}
					else
					{
						d->m_saved_lit = d->m_dict[cur_pos];
						d->m_saved_match_dist = cur_match_dist;
						d->m_saved_match_len = cur_match_len;
					}
				}
				else
				{
					iwc_z_record_match(d, d->m_saved_match_len, d->m_saved_match_dist);
					len_to_move = d->m_saved_match_len - 1;
					d->m_saved_match_len = 0;
				}
			}
			else if (!cur_match_dist)
				iwc_z_record_literal(d, d->m_dict[IW_MIN(cur_pos, sizeof(d->m_dict) - 1)]);
			else if ((d->m_greedy_parsing) || (d->m_flags & iwc_z_RLE_MATCHES) || (cur_match_len >= 128))
			{
				iwc_z_record_match(d, cur_match_len, cur_match_dist);
				len_to_move = cur_match_len;
			}
			else
			{
				d->m_saved_lit = d->m_dict[IW_MIN(cur_pos, sizeof(d->m_dict) - 1)];
				d->m_saved_match_dist = cur_match_dist;
				d->m_saved_match_len = cur_match_len;
			}
			d->m_lookahead_pos += len_to_move;
			IW_ASSERT(d->m_lookahead_size >= len_to_move);
			d->m_lookahead_size -= len_to_move;
			d->m_dict_size = IW_MIN(d->m_dict_size + len_to_move, (iw_uint)iwc_z_LZ_DICT_SIZE);
			if ((d->m_pLZ_code_buf > &d->m_lz_code_buf[iwc_z_LZ_CODE_BUF_SIZE - 8]) ||
				((d->m_total_lz_bytes > 31 * 1024) && (((((iw_uint)(d->m_pLZ_code_buf - d->m_lz_code_buf) * 115) >> 7) >= d->m_total_lz_bytes) || (d->m_flags & iwc_z_FORCE_ALL_RAW_BLOCKS))))
			{
				int n;
				d->m_pSrc = pSrc;
				d->m_src_buf_left = src_buf_left;
				if ((n = iwc_z_flush_block(d, 0)) != 0)
					return (n < 0) ? IW_FALSE : IW_TRUE;
			}
		}
		
		d->m_pSrc = pSrc;
		d->m_src_buf_left = src_buf_left;
		return IW_TRUE;
	}
	
	static iwc_z_status iwc_z_flush_output_buffer(iwc_z_compressor *d)
	{
		if (d->m_pIn_buf_size)
		{
			*d->m_pIn_buf_size = d->m_pSrc - (const iw_uint8 *)d->m_pIn_buf;
		}
		
		if (d->m_pOut_buf_size)
		{
			size_t n = IW_MIN(*d->m_pOut_buf_size - d->m_out_buf_ofs, d->m_output_flush_remaining);
			memcpy((iw_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs, d->m_output_buf + d->m_output_flush_ofs, n);
			d->m_output_flush_ofs += (iw_uint)n;
			d->m_output_flush_remaining -= (iw_uint)n;
			d->m_out_buf_ofs += n;
			
			*d->m_pOut_buf_size = d->m_out_buf_ofs;
		}
		
		return (d->m_finished && !d->m_output_flush_remaining) ? iwc_z_STATUS_DONE : iwc_z_STATUS_OKAY;
	}
	
	iwc_z_status iwc_z_compress(iwc_z_compressor *d, const void *pIn_buf, size_t *pIn_buf_size, void *pOut_buf, size_t *pOut_buf_size, iwc_z_flush flush)
	{
		if (!d)
		{
			if (pIn_buf_size)
				*pIn_buf_size = 0;
			if (pOut_buf_size)
				*pOut_buf_size = 0;
			return iwc_z_STATUS_BAD_PARAM;
		}
		
		d->m_pIn_buf = pIn_buf;
		d->m_pIn_buf_size = pIn_buf_size;
		d->m_pOut_buf = pOut_buf;
		d->m_pOut_buf_size = pOut_buf_size;
		d->m_pSrc = (const iw_uint8 *)(pIn_buf);
		d->m_src_buf_left = pIn_buf_size ? *pIn_buf_size : 0;
		d->m_out_buf_ofs = 0;
		d->m_flush = flush;
		
		if (((d->m_pPut_buf_func != NULL) == ((pOut_buf != NULL) || (pOut_buf_size != NULL))) || (d->m_prev_return_status != iwc_z_STATUS_OKAY) ||
			(d->m_wants_to_finish && (flush != iwc_z_FINISH)) || (pIn_buf_size && *pIn_buf_size && !pIn_buf) || (pOut_buf_size && *pOut_buf_size && !pOut_buf))
		{
			if (pIn_buf_size)
				*pIn_buf_size = 0;
			if (pOut_buf_size)
				*pOut_buf_size = 0;
			return (d->m_prev_return_status = iwc_z_STATUS_BAD_PARAM);
		}
		d->m_wants_to_finish |= (flush == iwc_z_FINISH);
		
		if ((d->m_output_flush_remaining) || (d->m_finished))
			return (d->m_prev_return_status = iwc_z_flush_output_buffer(d));
		
#if i2WC_USE_UNALIGNED_LOADS_AND_STORES && i2WC_LITTLE_ENDIAN
		if (((d->m_flags & iwc_z_MAX_PROBES_MASK) == 1) &&
			((d->m_flags & iwc_z_GREEDY_PARSING_FLAG) != 0) &&
			((d->m_flags & (iwc_z_FILTER_MATCHES | iwc_z_FORCE_ALL_RAW_BLOCKS | iwc_z_RLE_MATCHES)) == 0))
		{
			if (!iwc_z_compress_fast(d))
				return d->m_prev_return_status;
		}
		else
#endif
		{
			if (!iwc_z_compress_normal(d))
				return d->m_prev_return_status;
		}
		
		if ((d->m_flags & (iwc_z_WRITE_ZLIB_HEADER | iwc_z_COMPUTE_ADLER32)) && (pIn_buf))
			d->m_adler32 = (iw_uint32)iw_adler32(d->m_adler32, (const iw_uint8 *)pIn_buf, d->m_pSrc - (const iw_uint8 *)pIn_buf);
		
		if ((flush) && (!d->m_lookahead_size) && (!d->m_src_buf_left) && (!d->m_output_flush_remaining))
		{
			if (iwc_z_flush_block(d, flush) < 0)
				return d->m_prev_return_status;
			d->m_finished = (flush == iwc_z_FINISH);
			if (flush == iwc_z_FULL_FLUSH)
			{
				IW_CLEAR_ARR(d->m_hash);
				IW_CLEAR_ARR(d->m_next);
				d->m_dict_size = 0;
			}
		}
		
		return (d->m_prev_return_status = iwc_z_flush_output_buffer(d));
	}
	
	iwc_z_status iwc_z_compress_buffer(iwc_z_compressor *d, const void *pIn_buf, size_t in_buf_size, iwc_z_flush flush)
	{
		IW_ASSERT(d->m_pPut_buf_func);
		return iwc_z_compress(d, pIn_buf, &in_buf_size, NULL, NULL, flush);
	}
	
	iwc_z_status iwc_z_init(iwc_z_compressor *d, iwc_z_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags)
	{
		d->m_pPut_buf_func = pPut_buf_func;
		d->m_pPut_buf_user = pPut_buf_user;
		d->m_flags = (iw_uint)(flags);
		d->m_max_probes[0] = 1 + ((flags & 0xFFF) + 2) / 3;
		d->m_greedy_parsing = (flags & iwc_z_GREEDY_PARSING_FLAG) != 0;
		d->m_max_probes[1] = 1 + (((flags & 0xFFF) >> 2) + 2) / 3;
		if (!(flags & iwc_z_NONDETERMINISTIC_PARSING_FLAG))
			IW_CLEAR_ARR(d->m_hash);
		d->m_lookahead_pos = d->m_lookahead_size = d->m_dict_size = d->m_total_lz_bytes = d->m_lz_code_buf_dict_pos = d->m_bits_in = 0;
		d->m_output_flush_ofs = d->m_output_flush_remaining = d->m_finished = d->m_block_index = d->m_bit_buffer = d->m_wants_to_finish = 0;
		d->m_pLZ_code_buf = d->m_lz_code_buf + 1;
		d->m_pLZ_flags = d->m_lz_code_buf;
		*d->m_pLZ_flags = 0;
		d->m_num_flags_left = 8;
		d->m_pOutput_buf = d->m_output_buf;
		d->m_pOutput_buf_end = d->m_output_buf;
		d->m_prev_return_status = iwc_z_STATUS_OKAY;
		d->m_saved_match_dist = d->m_saved_match_len = d->m_saved_lit = 0;
		d->m_adler32 = 1;
		d->m_pIn_buf = NULL;
		d->m_pOut_buf = NULL;
		d->m_pIn_buf_size = NULL;
		d->m_pOut_buf_size = NULL;
		d->m_flush = iwc_z_NO_FLUSH;
		d->m_pSrc = NULL;
		d->m_src_buf_left = 0;
		d->m_out_buf_ofs = 0;
		if (!(flags & iwc_z_NONDETERMINISTIC_PARSING_FLAG))
			IW_CLEAR_ARR(d->m_dict);
		memset(&d->m_huff_count[0][0], 0, sizeof(d->m_huff_count[0][0]) * iwc_z_MAX_HUFF_SYMBOLS_0);
		memset(&d->m_huff_count[1][0], 0, sizeof(d->m_huff_count[1][0]) * iwc_z_MAX_HUFF_SYMBOLS_1);
		return iwc_z_STATUS_OKAY;
	}
	
	iwc_z_status iwc_z_get_prev_return_status(iwc_z_compressor *d)
	{
		return d->m_prev_return_status;
	}
	
	iw_uint32 iwc_z_get_adler32(iwc_z_compressor *d)
	{
		return d->m_adler32;
	}
	
	iw_bool iwc_z_compress_mem_to_output(const void *pBuf, size_t buf_len, iwc_z_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags)
	{
		iwc_z_compressor *pComp;
		iw_bool succeeded;
		if (((buf_len) && (!pBuf)) || (!pPut_buf_func))
			return IW_FALSE;
		pComp = (iwc_z_compressor *)IW_MALLOC(sizeof(iwc_z_compressor));
		if (!pComp)
			return IW_FALSE;
		succeeded = (iwc_z_init(pComp, pPut_buf_func, pPut_buf_user, flags) == iwc_z_STATUS_OKAY);
		succeeded = succeeded && (iwc_z_compress_buffer(pComp, pBuf, buf_len, iwc_z_FINISH) == iwc_z_STATUS_DONE);
		IW_FREE(pComp);
		return succeeded;
	}
	
	typedef struct
	{
		size_t m_size, m_capacity;
		iw_uint8 *m_pBuf;
		iw_bool m_expandable;
	} iwc_z_output_buffer;
	
	static iw_bool iwc_z_output_buffer_putter(const void *pBuf, int len, void *pUser)
	{
		iwc_z_output_buffer *p = (iwc_z_output_buffer *)pUser;
		size_t new_size = p->m_size + len;
		if (new_size > p->m_capacity)
		{
			size_t new_capacity = p->m_capacity;
			iw_uint8 *pNew_buf;
			if (!p->m_expandable)
				return IW_FALSE;
			do
			{
				new_capacity = IW_MAX(128U, new_capacity << 1U);
			} while (new_size > new_capacity);
			pNew_buf = (iw_uint8 *)IW_REALLOC(p->m_pBuf, new_capacity);
			if (!pNew_buf)
				return IW_FALSE;
			p->m_pBuf = pNew_buf;
			p->m_capacity = new_capacity;
		}
		memcpy((iw_uint8 *)p->m_pBuf + p->m_size, pBuf, len);
		p->m_size = new_size;
		return IW_TRUE;
	}
	
	void *iwc_z_compress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags)
	{
		iwc_z_output_buffer out_buf;
		IW_CLEAR_OBJ(out_buf);
		if (!pOut_len)
			return IW_FALSE;
		else
			*pOut_len = 0;
		out_buf.m_expandable = IW_TRUE;
		if (!iwc_z_compress_mem_to_output(pSrc_buf, src_buf_len, iwc_z_output_buffer_putter, &out_buf, flags))
			return NULL;
		*pOut_len = out_buf.m_size;
		return out_buf.m_pBuf;
	}
	
	size_t iwc_z_compress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags)
	{
		iwc_z_output_buffer out_buf;
		IW_CLEAR_OBJ(out_buf);
		if (!pOut_buf)
			return 0;
		out_buf.m_pBuf = (iw_uint8 *)pOut_buf;
		out_buf.m_capacity = out_buf_len;
		if (!iwc_z_compress_mem_to_output(pSrc_buf, src_buf_len, iwc_z_output_buffer_putter, &out_buf, flags))
			return 0;
		return out_buf.m_size;
	}
	
	iw_uint iwc_z_create_comp_flags_from_zip_params(int level, int window_bits, int strategy)
	{
		iw_uint comp_flags = s_iwc_z_num_probes[(level >= 0) ? IW_MIN(10, level) : IW_DEFAULT_LEVEL] | ((level <= 3) ? iwc_z_GREEDY_PARSING_FLAG : 0);
		if (window_bits > 0)
			comp_flags |= iwc_z_WRITE_ZLIB_HEADER;
		
		if (!level)
			comp_flags |= iwc_z_FORCE_ALL_RAW_BLOCKS;
		else if (strategy == IW_FILTERED)
			comp_flags |= iwc_z_FILTER_MATCHES;
		else if (strategy == IW_HUFFMAN_ONLY)
			comp_flags &= ~iwc_z_MAX_PROBES_MASK;
		else if (strategy == IW_FIXED)
			comp_flags |= iwc_z_FORCE_ALL_STATIC_BLOCKS;
		else if (strategy == IW_RLE)
			comp_flags |= iwc_z_RLE_MATCHES;
		
		return comp_flags;
	}
	
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4204) 
#endif
	
	void *iwc_z_write_image_to_png_file_in_memory_ex(const void *pImage, int w, int h, int num_chans, size_t *pLen_out, iw_uint level, iw_bool flip)
	{
		
		static const iw_uint s_iwc_z_png_num_probes[11] = { 0, 1, 6, 32, 16, 32, 128, 256, 512, 768, 1500 };
		iwc_z_compressor *pComp = (iwc_z_compressor *)IW_MALLOC(sizeof(iwc_z_compressor));
		iwc_z_output_buffer out_buf;
		int i, bpl = w * num_chans, y, z;
		iw_uint32 c;
		*pLen_out = 0;
		if (!pComp)
			return NULL;
		IW_CLEAR_OBJ(out_buf);
		out_buf.m_expandable = IW_TRUE;
		out_buf.m_capacity = 57 + IW_MAX(64, (1 + bpl) * h);
		if (NULL == (out_buf.m_pBuf = (iw_uint8 *)IW_MALLOC(out_buf.m_capacity)))
		{
			IW_FREE(pComp);
			return NULL;
		}
		for (z = 41; z; --z)
			iwc_z_output_buffer_putter(&z, 1, &out_buf);
		iwc_z_init(pComp, iwc_z_output_buffer_putter, &out_buf, s_iwc_z_png_num_probes[IW_MIN(10, level)] | iwc_z_WRITE_ZLIB_HEADER);
		for (y = 0; y < h; ++y)
		{
			iwc_z_compress_buffer(pComp, &z, 1, iwc_z_NO_FLUSH);
			iwc_z_compress_buffer(pComp, (iw_uint8 *)pImage + (flip ? (h - 1 - y) : y) * bpl, bpl, iwc_z_NO_FLUSH);
		}
		if (iwc_z_compress_buffer(pComp, NULL, 0, iwc_z_FINISH) != iwc_z_STATUS_DONE)
		{
			IW_FREE(pComp);
			IW_FREE(out_buf.m_pBuf);
			return NULL;
		}
		*pLen_out = out_buf.m_size - 41;
		{
			static const iw_uint8 chans[] = { 0x00, 0x00, 0x04, 0x02, 0x06 };
			iw_uint8 pnghdr[41] = { 0x89, 0x50, 0x4e, 0x47, 0x0d,
				0x0a, 0x1a, 0x0a, 0x00, 0x00,
				0x00, 0x0d, 0x49, 0x48, 0x44,
				0x52, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x08,
				0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x49, 0x44, 0x41,
				0x54 };
			pnghdr[18] = (iw_uint8)(w >> 8);
			pnghdr[19] = (iw_uint8)w;
			pnghdr[22] = (iw_uint8)(h >> 8);
			pnghdr[23] = (iw_uint8)h;
			pnghdr[25] = chans[num_chans];
			pnghdr[33] = (iw_uint8)(*pLen_out >> 24);
			pnghdr[34] = (iw_uint8)(*pLen_out >> 16);
			pnghdr[35] = (iw_uint8)(*pLen_out >> 8);
			pnghdr[36] = (iw_uint8)*pLen_out;
			c = (iw_uint32)iw_crc32(IW_CRC32_INIT, pnghdr + 12, 17);
			for (i = 0; i < 4; ++i, c <<= 8)
				((iw_uint8 *)(pnghdr + 29))[i] = (iw_uint8)(c >> 24);
			memcpy(out_buf.m_pBuf, pnghdr, 41);
		}
		if (!iwc_z_output_buffer_putter("\0\0\0\0\0\0\0\0\x49\x45\x4e\x44\xae\x42\x60\x82", 16, &out_buf))
		{
			*pLen_out = 0;
			IW_FREE(pComp);
			IW_FREE(out_buf.m_pBuf);
			return NULL;
		}
		c = (iw_uint32)iw_crc32(IW_CRC32_INIT, out_buf.m_pBuf + 41 - 4, *pLen_out + 4);
		for (i = 0; i < 4; ++i, c <<= 8)
			(out_buf.m_pBuf + out_buf.m_size - 16)[i] = (iw_uint8)(c >> 24);
		*pLen_out += 57;
		IW_FREE(pComp);
		return out_buf.m_pBuf;
	}
	void *iwc_z_write_image_to_png_file_in_memory(const void *pImage, int w, int h, int num_chans, size_t *pLen_out)
	{
		return iwc_z_write_image_to_png_file_in_memory_ex(pImage, w, h, num_chans, pLen_out, 6, IW_FALSE);
	}
	
#ifndef i2WC_NO_MALLOC
	iwc_z_compressor *iwc_z_compressor_alloc(void)
	{
		return (iwc_z_compressor *)IW_MALLOC(sizeof(iwc_z_compressor));
	}
	
	void iwc_z_compressor_free(iwc_z_compressor *pComp)
	{
		IW_FREE(pComp);
	}
#endif
	
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	
#ifdef __cplusplus
}
#endif

#endif

#ifndef i2WC_NO_INFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
	
#define iwc_u_MEMCPY(d, s, l) memcpy(d, s, l)
#define iwc_u_MEMSET(p, c, l) memset(p, c, l)
	
#define iwc_u_CR_BEGIN  \
	switch (r->m_state) \
	{                   \
	case 0:
#define iwc_u_CR_RETURN(state_index, result) \
	do                                       \
	{                                        \
	status = result;                     \
	r->m_state = state_index;            \
	goto common_exit;                    \
	case state_index:;                   \
	}                                        \
	IW_MACRO_END
#define iwc_u_CR_RETURN_FOREVER(state_index, result) \
	do                                               \
	{                                                \
	for (;;)                                     \
	{                                            \
	iwc_u_CR_RETURN(state_index, result);    \
	}                                            \
	}                                                \
	IW_MACRO_END
#define iwc_u_CR_FINISH }
	
#define iwc_u_GET_BYTE(state_index, c)                                                                                                                           \
	do                                                                                                                                                           \
	{                                                                                                                                                            \
	while (pIn_buf_cur >= pIn_buf_end)                                                                                                                       \
	{                                                                                                                                                        \
	iwc_u_CR_RETURN(state_index, (decomp_flags & iwc_u_FLAG_HAS_MORE_INPUT) ? iwc_u_STATUS_NEEDS_MORE_INPUT : iwc_u_STATUS_FAILED_CANNOT_MAKE_PROGRESS); \
	}                                                                                                                                                        \
	c = *pIn_buf_cur++;                                                                                                                                      \
	}                                                                                                                                                            \
	IW_MACRO_END
	
#define iwc_u_NEED_BITS(state_index, n)                \
	do                                                 \
	{                                                  \
	iw_uint c;                                     \
	iwc_u_GET_BYTE(state_index, c);                \
	bit_buf |= (((iwc_u_bit_buf_t)c) << num_bits); \
	num_bits += 8;                                 \
	} while (num_bits < (iw_uint)(n))
#define iwc_u_SKIP_BITS(state_index, n)      \
	do                                       \
	{                                        \
	if (num_bits < (iw_uint)(n))         \
	{                                    \
	iwc_u_NEED_BITS(state_index, n); \
	}                                    \
	bit_buf >>= (n);                     \
	num_bits -= (n);                     \
	}                                        \
	IW_MACRO_END
#define iwc_u_GET_BITS(state_index, b, n)    \
	do                                       \
	{                                        \
	if (num_bits < (iw_uint)(n))         \
	{                                    \
	iwc_u_NEED_BITS(state_index, n); \
	}                                    \
	b = bit_buf & ((1 << (n)) - 1);      \
	bit_buf >>= (n);                     \
	num_bits -= (n);                     \
	}                                        \
	IW_MACRO_END
	
#define iwc_u_HUFF_BITBUF_FILL(state_index, pLookUp, pTree)          \
	do                                                               \
	{                                                                \
	temp = pLookUp[bit_buf & (iwc_u_FAST_LOOKUP_SIZE - 1)];      \
	if (temp >= 0)                                               \
	{                                                            \
	code_len = temp >> 9;                                    \
	if ((code_len) && (num_bits >= code_len))                \
	break;                                               \
	}                                                            \
	else if (num_bits > iwc_u_FAST_LOOKUP_BITS)                  \
	{                                                            \
	code_len = iwc_u_FAST_LOOKUP_BITS;                       \
	do                                                       \
	{                                                        \
	temp = pTree[~temp + ((bit_buf >> code_len++) & 1)]; \
	} while ((temp < 0) && (num_bits >= (code_len + 1)));    \
	if (temp >= 0)                                           \
	break;                                               \
	}                                                            \
	iwc_u_GET_BYTE(state_index, c);                              \
	bit_buf |= (((iwc_u_bit_buf_t)c) << num_bits);               \
	num_bits += 8;                                               \
	} while (num_bits < 15);
	
#define iwc_u_HUFF_DECODE(state_index, sym, pLookUp, pTree)                                                                         \
	do                                                                                                                              \
	{                                                                                                                               \
	int temp;                                                                                                                   \
	iw_uint code_len, c;                                                                                                        \
	if (num_bits < 15)                                                                                                          \
	{                                                                                                                           \
	if ((pIn_buf_end - pIn_buf_cur) < 2)                                                                                    \
	{                                                                                                                       \
	iwc_u_HUFF_BITBUF_FILL(state_index, pLookUp, pTree);                                                                \
	}                                                                                                                       \
	else                                                                                                                    \
	{                                                                                                                       \
	bit_buf |= (((iwc_u_bit_buf_t)pIn_buf_cur[0]) << num_bits) | (((iwc_u_bit_buf_t)pIn_buf_cur[1]) << (num_bits + 8)); \
	pIn_buf_cur += 2;                                                                                                   \
	num_bits += 16;                                                                                                     \
	}                                                                                                                       \
	}                                                                                                                           \
	if ((temp = pLookUp[bit_buf & (iwc_u_FAST_LOOKUP_SIZE - 1)]) >= 0)                                                          \
	code_len = temp >> 9, temp &= 511;                                                                                      \
	else                                                                                                                        \
	{                                                                                                                           \
	code_len = iwc_u_FAST_LOOKUP_BITS;                                                                                      \
	do                                                                                                                      \
	{                                                                                                                       \
	temp = pTree[~temp + ((bit_buf >> code_len++) & 1)];                                                                \
	} while (temp < 0);                                                                                                     \
	}                                                                                                                           \
	sym = temp;                                                                                                                 \
	bit_buf >>= code_len;                                                                                                       \
	num_bits -= code_len;                                                                                                       \
	}                                                                                                                               \
	IW_MACRO_END
	
	static void iwc_u_clear_tree(iwc_u_decompressor *r)
	{
		if (r->m_type == 0)
			IW_CLEAR_ARR(r->m_tree_0);
		else if (r->m_type == 1)
			IW_CLEAR_ARR(r->m_tree_1);
		else
			IW_CLEAR_ARR(r->m_tree_2);
	}
	
	iwc_u_status iwc_u_decompress(iwc_u_decompressor *r, const iw_uint8 *pIn_buf_next, size_t *pIn_buf_size, iw_uint8 *pOut_buf_start, iw_uint8 *pOut_buf_next, size_t *pOut_buf_size, const iw_uint32 decomp_flags)
	{
		static const iw_uint16 s_length_base[31] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0 };
		static const iw_uint8 s_length_extra[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0 };
		static const iw_uint16 s_dist_base[32] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577, 0, 0 };
		static const iw_uint8 s_dist_extra[32] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
		static const iw_uint8 s_length_dezigzag[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
		static const iw_uint16 s_min_table_sizes[3] = { 257, 1, 4 };
		
		iw_int16 *pTrees[3];
		iw_uint8 *pCode_sizes[3];
		
		iwc_u_status status = iwc_u_STATUS_FAILED;
		iw_uint32 num_bits, dist, counter, num_extra;
		iwc_u_bit_buf_t bit_buf;
		const iw_uint8 *pIn_buf_cur = pIn_buf_next, *const pIn_buf_end = pIn_buf_next + *pIn_buf_size;
		iw_uint8 *pOut_buf_cur = pOut_buf_next, *const pOut_buf_end = pOut_buf_next ? pOut_buf_next + *pOut_buf_size : NULL;
		size_t out_buf_size_mask = (decomp_flags & iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF) ? (size_t)-1 : ((pOut_buf_next - pOut_buf_start) + *pOut_buf_size) - 1, dist_from_out_buf_start;
		
		if (((out_buf_size_mask + 1) & out_buf_size_mask) || (pOut_buf_next < pOut_buf_start))
		{
			*pIn_buf_size = *pOut_buf_size = 0;
			return iwc_u_STATUS_BAD_PARAM;
		}
		
		pTrees[0] = r->m_tree_0;
		pTrees[1] = r->m_tree_1;
		pTrees[2] = r->m_tree_2;
		pCode_sizes[0] = r->m_code_size_0;
		pCode_sizes[1] = r->m_code_size_1;
		pCode_sizes[2] = r->m_code_size_2;
		
		num_bits = r->m_num_bits;
		bit_buf = r->m_bit_buf;
		dist = r->m_dist;
		counter = r->m_counter;
		num_extra = r->m_num_extra;
		dist_from_out_buf_start = r->m_dist_from_out_buf_start;
		iwc_u_CR_BEGIN
		
		bit_buf = num_bits = dist = counter = num_extra = r->m_zhdr0 = r->m_zhdr1 = 0;
		r->m_z_adler32 = r->m_check_adler32 = 1;
		if (decomp_flags & iwc_u_FLAG_PARSE_ZLIB_HEADER)
		{
			iwc_u_GET_BYTE(1, r->m_zhdr0);
			iwc_u_GET_BYTE(2, r->m_zhdr1);
			counter = (((r->m_zhdr0 * 256 + r->m_zhdr1) % 31 != 0) || (r->m_zhdr1 & 32) || ((r->m_zhdr0 & 15) != 8));
			if (!(decomp_flags & iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF))
				counter |= (((1U << (8U + (r->m_zhdr0 >> 4))) > 32768U) || ((out_buf_size_mask + 1) < (size_t)((size_t)1 << (8U + (r->m_zhdr0 >> 4)))));
			if (counter)
			{
				iwc_u_CR_RETURN_FOREVER(36, iwc_u_STATUS_FAILED);
			}
		}
		
		do
		{
			iwc_u_GET_BITS(3, r->m_final, 3);
			r->m_type = r->m_final >> 1;
			if (r->m_type == 0)
			{
				iwc_u_SKIP_BITS(5, num_bits & 7);
				for (counter = 0; counter < 4; ++counter)
				{
					if (num_bits)
						iwc_u_GET_BITS(6, r->m_raw_header[counter], 8);
					else
						iwc_u_GET_BYTE(7, r->m_raw_header[counter]);
				}
				if ((counter = (r->m_raw_header[0] | (r->m_raw_header[1] << 8))) != (iw_uint)(0xFFFF ^ (r->m_raw_header[2] | (r->m_raw_header[3] << 8))))
				{
					iwc_u_CR_RETURN_FOREVER(39, iwc_u_STATUS_FAILED);
				}
				while ((counter) && (num_bits))
				{
					iwc_u_GET_BITS(51, dist, 8);
					while (pOut_buf_cur >= pOut_buf_end)
					{
						iwc_u_CR_RETURN(52, iwc_u_STATUS_HAS_MORE_OUTPUT);
					}
					*pOut_buf_cur++ = (iw_uint8)dist;
					counter--;
				}
				while (counter)
				{
					size_t n;
					while (pOut_buf_cur >= pOut_buf_end)
					{
						iwc_u_CR_RETURN(9, iwc_u_STATUS_HAS_MORE_OUTPUT);
					}
					while (pIn_buf_cur >= pIn_buf_end)
					{
						iwc_u_CR_RETURN(38, (decomp_flags & iwc_u_FLAG_HAS_MORE_INPUT) ? iwc_u_STATUS_NEEDS_MORE_INPUT : iwc_u_STATUS_FAILED_CANNOT_MAKE_PROGRESS);
					}
					n = IW_MIN(IW_MIN((size_t)(pOut_buf_end - pOut_buf_cur), (size_t)(pIn_buf_end - pIn_buf_cur)), counter);
					iwc_u_MEMCPY(pOut_buf_cur, pIn_buf_cur, n);
					pIn_buf_cur += n;
					pOut_buf_cur += n;
					counter -= (iw_uint)n;
				}
			}
			else if (r->m_type == 3)
			{
				iwc_u_CR_RETURN_FOREVER(10, iwc_u_STATUS_FAILED);
			}
			else
			{
				if (r->m_type == 1)
				{
					iw_uint8 *p = r->m_code_size_0;
					iw_uint i;
					r->m_table_sizes[0] = 288;
					r->m_table_sizes[1] = 32;
					iwc_u_MEMSET(r->m_code_size_1, 5, 32);
					for (i = 0; i <= 143; ++i)
						*p++ = 8;
					for (; i <= 255; ++i)
						*p++ = 9;
					for (; i <= 279; ++i)
						*p++ = 7;
					for (; i <= 287; ++i)
						*p++ = 8;
				}
				else
				{
					for (counter = 0; counter < 3; counter++)
					{
						iwc_u_GET_BITS(11, r->m_table_sizes[counter], "\05\05\04"[counter]);
						r->m_table_sizes[counter] += s_min_table_sizes[counter];
					}
					IW_CLEAR_ARR(r->m_code_size_2);
					for (counter = 0; counter < r->m_table_sizes[2]; counter++)
					{
						iw_uint s;
						iwc_u_GET_BITS(14, s, 3);
						r->m_code_size_2[s_length_dezigzag[counter]] = (iw_uint8)s;
					}
					r->m_table_sizes[2] = 19;
				}
				for (; (int)r->m_type >= 0; r->m_type--)
				{
					int tree_next, tree_cur;
					iw_int16 *pLookUp;
					iw_int16 *pTree;
					iw_uint8 *pCode_size;
					iw_uint i, j, used_syms, total, sym_index, next_code[17], total_syms[16];
					pLookUp = r->m_look_up[r->m_type];
					pTree = pTrees[r->m_type];
					pCode_size = pCode_sizes[r->m_type];
					IW_CLEAR_ARR(total_syms);
					iwc_u_MEMSET(pLookUp, 0, sizeof(r->m_look_up[0]));
					iwc_u_clear_tree(r);
					for (i = 0; i < r->m_table_sizes[r->m_type]; ++i)
						total_syms[pCode_size[i]]++;
					used_syms = 0, total = 0;
					next_code[0] = next_code[1] = 0;
					for (i = 1; i <= 15; ++i)
					{
						used_syms += total_syms[i];
						next_code[i + 1] = (total = ((total + total_syms[i]) << 1));
					}
					if ((65536 != total) && (used_syms > 1))
					{
						iwc_u_CR_RETURN_FOREVER(35, iwc_u_STATUS_FAILED);
					}
					for (tree_next = -1, sym_index = 0; sym_index < r->m_table_sizes[r->m_type]; ++sym_index)
					{
						iw_uint rev_code = 0, l, cur_code, code_size = pCode_size[sym_index];
						if (!code_size)
							continue;
						cur_code = next_code[code_size]++;
						for (l = code_size; l > 0; l--, cur_code >>= 1)
							rev_code = (rev_code << 1) | (cur_code & 1);
						if (code_size <= iwc_u_FAST_LOOKUP_BITS)
						{
							iw_int16 k = (iw_int16)((code_size << 9) | sym_index);
							while (rev_code < iwc_u_FAST_LOOKUP_SIZE)
							{
								pLookUp[rev_code] = k;
								rev_code += (1 << code_size);
							}
							continue;
						}
						if (0 == (tree_cur = pLookUp[rev_code & (iwc_u_FAST_LOOKUP_SIZE - 1)]))
						{
							pLookUp[rev_code & (iwc_u_FAST_LOOKUP_SIZE - 1)] = (iw_int16)tree_next;
							tree_cur = tree_next;
							tree_next -= 2;
						}
						rev_code >>= (iwc_u_FAST_LOOKUP_BITS - 1);
						for (j = code_size; j > (iwc_u_FAST_LOOKUP_BITS + 1); j--)
						{
							tree_cur -= ((rev_code >>= 1) & 1);
							if (!pTree[-tree_cur - 1])
							{
								pTree[-tree_cur - 1] = (iw_int16)tree_next;
								tree_cur = tree_next;
								tree_next -= 2;
							}
							else
								tree_cur = pTree[-tree_cur - 1];
						}
						tree_cur -= ((rev_code >>= 1) & 1);
						pTree[-tree_cur - 1] = (iw_int16)sym_index;
					}
					if (r->m_type == 2)
					{
						for (counter = 0; counter < (r->m_table_sizes[0] + r->m_table_sizes[1]);)
						{
							iw_uint s;
							iwc_u_HUFF_DECODE(16, dist, r->m_look_up[2], r->m_tree_2);
							if (dist < 16)
							{
								r->m_len_codes[counter++] = (iw_uint8)dist;
								continue;
							}
							if ((dist == 16) && (!counter))
							{
								iwc_u_CR_RETURN_FOREVER(17, iwc_u_STATUS_FAILED);
							}
							num_extra = "\02\03\07"[dist - 16];
							iwc_u_GET_BITS(18, s, num_extra);
							s += "\03\03\013"[dist - 16];
							iwc_u_MEMSET(r->m_len_codes + counter, (dist == 16) ? r->m_len_codes[counter - 1] : 0, s);
							counter += s;
						}
						if ((r->m_table_sizes[0] + r->m_table_sizes[1]) != counter)
						{
							iwc_u_CR_RETURN_FOREVER(21, iwc_u_STATUS_FAILED);
						}
						iwc_u_MEMCPY(r->m_code_size_0, r->m_len_codes, r->m_table_sizes[0]);
						iwc_u_MEMCPY(r->m_code_size_1, r->m_len_codes + r->m_table_sizes[0], r->m_table_sizes[1]);
					}
				}
				for (;;)
				{
					iw_uint8 *pSrc;
					for (;;)
					{
						if (((pIn_buf_end - pIn_buf_cur) < 4) || ((pOut_buf_end - pOut_buf_cur) < 2))
						{
							iwc_u_HUFF_DECODE(23, counter, r->m_look_up[0], r->m_tree_0);
							if (counter >= 256)
								break;
							while (pOut_buf_cur >= pOut_buf_end)
							{
								iwc_u_CR_RETURN(24, iwc_u_STATUS_HAS_MORE_OUTPUT);
							}
							*pOut_buf_cur++ = (iw_uint8)counter;
						}
						else
						{
							int sym2;
							iw_uint code_len;
#if iwc_u_USE_64BIT_BITBUF
							if (num_bits < 30)
							{
								bit_buf |= (((iwc_u_bit_buf_t)IW_READ_LE32(pIn_buf_cur)) << num_bits);
								pIn_buf_cur += 4;
								num_bits += 32;
							}
#else
							if (num_bits < 15)
							{
								bit_buf |= (((iwc_u_bit_buf_t)IW_READ_LE16(pIn_buf_cur)) << num_bits);
								pIn_buf_cur += 2;
								num_bits += 16;
							}
#endif
							if ((sym2 = r->m_look_up[0][bit_buf & (iwc_u_FAST_LOOKUP_SIZE - 1)]) >= 0)
								code_len = sym2 >> 9;
							else
							{
								code_len = iwc_u_FAST_LOOKUP_BITS;
								do
								{
									sym2 = r->m_tree_0[~sym2 + ((bit_buf >> code_len++) & 1)];
								} while (sym2 < 0);
							}
							counter = sym2;
							bit_buf >>= code_len;
							num_bits -= code_len;
							if (counter & 256)
								break;
							
#if !iwc_u_USE_64BIT_BITBUF
							if (num_bits < 15)
							{
								bit_buf |= (((iwc_u_bit_buf_t)IW_READ_LE16(pIn_buf_cur)) << num_bits);
								pIn_buf_cur += 2;
								num_bits += 16;
							}
#endif
							if ((sym2 = r->m_look_up[0][bit_buf & (iwc_u_FAST_LOOKUP_SIZE - 1)]) >= 0)
								code_len = sym2 >> 9;
							else
							{
								code_len = iwc_u_FAST_LOOKUP_BITS;
								do
								{
									sym2 = r->m_tree_0[~sym2 + ((bit_buf >> code_len++) & 1)];
								} while (sym2 < 0);
							}
							bit_buf >>= code_len;
							num_bits -= code_len;
							
							pOut_buf_cur[0] = (iw_uint8)counter;
							if (sym2 & 256)
							{
								pOut_buf_cur++;
								counter = sym2;
								break;
							}
							pOut_buf_cur[1] = (iw_uint8)sym2;
							pOut_buf_cur += 2;
						}
					}
					if ((counter &= 511) == 256)
						break;
					
					num_extra = s_length_extra[counter - 257];
					counter = s_length_base[counter - 257];
					if (num_extra)
					{
						iw_uint extra_bits;
						iwc_u_GET_BITS(25, extra_bits, num_extra);
						counter += extra_bits;
					}
					
					iwc_u_HUFF_DECODE(26, dist, r->m_look_up[1], r->m_tree_1);
					num_extra = s_dist_extra[dist];
					dist = s_dist_base[dist];
					if (num_extra)
					{
						iw_uint extra_bits;
						iwc_u_GET_BITS(27, extra_bits, num_extra);
						dist += extra_bits;
					}
					
					dist_from_out_buf_start = pOut_buf_cur - pOut_buf_start;
					if ((dist == 0 || dist > dist_from_out_buf_start || dist_from_out_buf_start == 0) && (decomp_flags & iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF))
					{
						iwc_u_CR_RETURN_FOREVER(37, iwc_u_STATUS_FAILED);
					}
					
					pSrc = pOut_buf_start + ((dist_from_out_buf_start - dist) & out_buf_size_mask);
					
					if ((IW_MAX(pOut_buf_cur, pSrc) + counter) > pOut_buf_end)
					{
						while (counter--)
						{
							while (pOut_buf_cur >= pOut_buf_end)
							{
								iwc_u_CR_RETURN(53, iwc_u_STATUS_HAS_MORE_OUTPUT);
							}
							*pOut_buf_cur++ = pOut_buf_start[(dist_from_out_buf_start++ - dist) & out_buf_size_mask];
						}
						continue;
					}
#if i2WC_USE_UNALIGNED_LOADS_AND_STORES
					else if ((counter >= 9) && (counter <= dist))
					{
						const iw_uint8 *pSrc_end = pSrc + (counter & ~7);
						do
						{
#ifdef i2WC_UNALIGNED_USE_MEMCPY
							memcpy(pOut_buf_cur, pSrc, sizeof(iw_uint32) * 2);
#else
							((iw_uint32 *)pOut_buf_cur)[0] = ((const iw_uint32 *)pSrc)[0];
							((iw_uint32 *)pOut_buf_cur)[1] = ((const iw_uint32 *)pSrc)[1];
#endif
							pOut_buf_cur += 8;
						} while ((pSrc += 8) < pSrc_end);
						if ((counter &= 7) < 3)
						{
							if (counter)
							{
								pOut_buf_cur[0] = pSrc[0];
								if (counter > 1)
									pOut_buf_cur[1] = pSrc[1];
								pOut_buf_cur += counter;
							}
							continue;
						}
					}
#endif
					while (counter > 2)
					{
						pOut_buf_cur[0] = pSrc[0];
						pOut_buf_cur[1] = pSrc[1];
						pOut_buf_cur[2] = pSrc[2];
						pOut_buf_cur += 3;
						pSrc += 3;
						counter -= 3;
					}
					if (counter > 0)
					{
						pOut_buf_cur[0] = pSrc[0];
						if (counter > 1)
							pOut_buf_cur[1] = pSrc[1];
						pOut_buf_cur += counter;
					}
				}
			}
		} while (!(r->m_final & 1));
		
		iwc_u_SKIP_BITS(32, num_bits & 7);
		while ((pIn_buf_cur > pIn_buf_next) && (num_bits >= 8))
		{
			--pIn_buf_cur;
			num_bits -= 8;
		}
		bit_buf &= ~(~(iwc_u_bit_buf_t)0 << num_bits);
		IW_ASSERT(!num_bits); 
		
		if (decomp_flags & iwc_u_FLAG_PARSE_ZLIB_HEADER)
		{
			for (counter = 0; counter < 4; ++counter)
			{
				iw_uint s;
				if (num_bits)
					iwc_u_GET_BITS(41, s, 8);
				else
					iwc_u_GET_BYTE(42, s);
				r->m_z_adler32 = (r->m_z_adler32 << 8) | s;
			}
		}
		iwc_u_CR_RETURN_FOREVER(34, iwc_u_STATUS_DONE);
		
		iwc_u_CR_FINISH
		
		common_exit:
		if ((status != iwc_u_STATUS_NEEDS_MORE_INPUT) && (status != iwc_u_STATUS_FAILED_CANNOT_MAKE_PROGRESS))
		{
			while ((pIn_buf_cur > pIn_buf_next) && (num_bits >= 8))
			{
				--pIn_buf_cur;
				num_bits -= 8;
			}
		}
		r->m_num_bits = num_bits;
		r->m_bit_buf = bit_buf & ~(~(iwc_u_bit_buf_t)0 << num_bits);
		r->m_dist = dist;
		r->m_counter = counter;
		r->m_num_extra = num_extra;
		r->m_dist_from_out_buf_start = dist_from_out_buf_start;
		*pIn_buf_size = pIn_buf_cur - pIn_buf_next;
		*pOut_buf_size = pOut_buf_cur - pOut_buf_next;
		if ((decomp_flags & (iwc_u_FLAG_PARSE_ZLIB_HEADER | iwc_u_FLAG_COMPUTE_ADLER32)) && (status >= 0))
		{
			const iw_uint8 *ptr = pOut_buf_next;
			size_t buf_len = *pOut_buf_size;
			iw_uint32 i, s1 = r->m_check_adler32 & 0xffff, s2 = r->m_check_adler32 >> 16;
			size_t block_len = buf_len % 5552;
			while (buf_len)
			{
				for (i = 0; i + 7 < block_len; i += 8, ptr += 8)
				{
					s1 += ptr[0], s2 += s1;
					s1 += ptr[1], s2 += s1;
					s1 += ptr[2], s2 += s1;
					s1 += ptr[3], s2 += s1;
					s1 += ptr[4], s2 += s1;
					s1 += ptr[5], s2 += s1;
					s1 += ptr[6], s2 += s1;
					s1 += ptr[7], s2 += s1;
				}
				for (; i < block_len; ++i)
					s1 += *ptr++, s2 += s1;
				s1 %= 65521U, s2 %= 65521U;
				buf_len -= block_len;
				block_len = 5552;
			}
			r->m_check_adler32 = (s2 << 16) + s1;
			if ((status == iwc_u_STATUS_DONE) && (decomp_flags & iwc_u_FLAG_PARSE_ZLIB_HEADER) && (r->m_check_adler32 != r->m_z_adler32))
				status = iwc_u_STATUS_ADLER32_MISMATCH;
		}
		return status;
	}
	
	void *iwc_u_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags)
	{
		iwc_u_decompressor decomp;
		void *pBuf = NULL, *pNew_buf;
		size_t src_buf_ofs = 0, out_buf_capacity = 0;
		*pOut_len = 0;
		iwc_u_init(&decomp);
		for (;;)
		{
			size_t src_buf_size = src_buf_len - src_buf_ofs, dst_buf_size = out_buf_capacity - *pOut_len, new_out_buf_capacity;
			iwc_u_status status = iwc_u_decompress(&decomp, (const iw_uint8 *)pSrc_buf + src_buf_ofs, &src_buf_size, (iw_uint8 *)pBuf, pBuf ? (iw_uint8 *)pBuf + *pOut_len : NULL, &dst_buf_size,
												   (flags & ~iwc_u_FLAG_HAS_MORE_INPUT) | iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
			if ((status < 0) || (status == iwc_u_STATUS_NEEDS_MORE_INPUT))
			{
				IW_FREE(pBuf);
				*pOut_len = 0;
				return NULL;
			}
			src_buf_ofs += src_buf_size;
			*pOut_len += dst_buf_size;
			if (status == iwc_u_STATUS_DONE)
				break;
			new_out_buf_capacity = out_buf_capacity * 2;
			if (new_out_buf_capacity < 128)
				new_out_buf_capacity = 128;
			pNew_buf = IW_REALLOC(pBuf, new_out_buf_capacity);
			if (!pNew_buf)
			{
				IW_FREE(pBuf);
				*pOut_len = 0;
				return NULL;
			}
			pBuf = pNew_buf;
			out_buf_capacity = new_out_buf_capacity;
		}
		return pBuf;
	}
	
	size_t iwc_u_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags)
	{
		iwc_u_decompressor decomp;
		iwc_u_status status;
		iwc_u_init(&decomp);
		status = iwc_u_decompress(&decomp, (const iw_uint8 *)pSrc_buf, &src_buf_len, (iw_uint8 *)pOut_buf, (iw_uint8 *)pOut_buf, &out_buf_len, (flags & ~iwc_u_FLAG_HAS_MORE_INPUT) | iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
		return (status != iwc_u_STATUS_DONE) ? iwc_u_DECOMPRESS_MEM_TO_MEM_FAILED : out_buf_len;
	}
	
	int iwc_u_decompress_mem_to_callback(const void *pIn_buf, size_t *pIn_buf_size, iwc_u_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags)
	{
		int result = 0;
		iwc_u_decompressor decomp;
		iw_uint8 *pDict = (iw_uint8 *)IW_MALLOC(iwc_u_LZ_DICT_SIZE);
		size_t in_buf_ofs = 0, dict_ofs = 0;
		if (!pDict)
			return iwc_u_STATUS_FAILED;
		memset(pDict, 0, iwc_u_LZ_DICT_SIZE);
		iwc_u_init(&decomp);
		for (;;)
		{
			size_t in_buf_size = *pIn_buf_size - in_buf_ofs, dst_buf_size = iwc_u_LZ_DICT_SIZE - dict_ofs;
			iwc_u_status status = iwc_u_decompress(&decomp, (const iw_uint8 *)pIn_buf + in_buf_ofs, &in_buf_size, pDict, pDict + dict_ofs, &dst_buf_size,
												   (flags & ~(iwc_u_FLAG_HAS_MORE_INPUT | iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF)));
			in_buf_ofs += in_buf_size;
			if ((dst_buf_size) && (!(*pPut_buf_func)(pDict + dict_ofs, (int)dst_buf_size, pPut_buf_user)))
				break;
			if (status != iwc_u_STATUS_HAS_MORE_OUTPUT)
			{
				result = (status == iwc_u_STATUS_DONE);
				break;
			}
			dict_ofs = (dict_ofs + dst_buf_size) & (iwc_u_LZ_DICT_SIZE - 1);
		}
		IW_FREE(pDict);
		*pIn_buf_size = in_buf_ofs;
		return result;
	}
	
#ifndef i2WC_NO_MALLOC
	iwc_u_decompressor *iwc_u_decompressor_alloc(void)
	{
		iwc_u_decompressor *pDecomp = (iwc_u_decompressor *)IW_MALLOC(sizeof(iwc_u_decompressor));
		if (pDecomp)
			iwc_u_init(pDecomp);
		return pDecomp;
	}
	
	void iwc_u_decompressor_free(iwc_u_decompressor *pDecomp)
	{
		IW_FREE(pDecomp);
	}
#endif
	
#ifdef __cplusplus
}
#endif

#endif

#ifndef i2WC_NO_ARCHIVE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
	
#ifdef i2WC_NO_STDIO
#define IW_FILE void *
#else
#include <sys/stat.h>
	
#if defined(_MSC_VER) || defined(__MINGW64__) || defined(__MINGW32__)
	
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef __cplusplus
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
	
	static WCHAR *iw_utf8z_to_widechar(const char *str)
	{
		int reqChars = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
		WCHAR *wStr = (WCHAR *)malloc(reqChars * sizeof(WCHAR));
		MultiByteToWideChar(CP_UTF8, 0, str, -1, wStr, reqChars);
		return wStr;
	}
	
	static FILE *iw_fopen(const char *pFilename, const char *pMode)
	{
		WCHAR *wFilename = iw_utf8z_to_widechar(pFilename);
		WCHAR *wMode = iw_utf8z_to_widechar(pMode);
		FILE *pFile = NULL;
		errno_t err = _wfopen_s(&pFile, wFilename, wMode);
		free(wFilename);
		free(wMode);
		return err ? NULL : pFile;
	}
	
	static FILE *iw_freopen(const char *pPath, const char *pMode, FILE *pStream)
	{
		WCHAR *wPath = iw_utf8z_to_widechar(pPath);
		WCHAR *wMode = iw_utf8z_to_widechar(pMode);
		FILE *pFile = NULL;
		errno_t err = _wfreopen_s(&pFile, wPath, wMode, pStream);
		free(wPath);
		free(wMode);
		return err ? NULL : pFile;
	}
	
#if defined(__MINGW32__)
	static int iw_stat(const char *path, struct _stat *buffer)
	{
		WCHAR *wPath = iw_utf8z_to_widechar(path);
		int res = _wstat(wPath, buffer);
		free(wPath);
		return res;
	}
#else
	static int iw_stat64(const char *path, struct __stat64 *buffer)
	{
		WCHAR *wPath = iw_utf8z_to_widechar(path);
		int res = _wstat64(wPath, buffer);
		free(wPath);
		return res;
	}
#endif
	
#ifndef i2WC_NO_TIME
#include <sys/utime.h>
#endif
#define IW_FOPEN iw_fopen
#define IW_FCLOSE fclose
#define IW_FREAD fread
#define IW_FWRITE fwrite
#define IW_FTELL64 _ftelli64
#define IW_FSEEK64 _fseeki64
#if defined(__MINGW32__)
#define IW_FILE_STAT_STRUCT _stat
#define IW_FILE_STAT iw_stat
#else
#define IW_FILE_STAT_STRUCT _stat64
#define IW_FILE_STAT iw_stat64
#endif
#define IW_FFLUSH fflush
#define IW_FREOPEN iw_freopen
#define IW_DELETE_FILE remove
	
#elif defined(__WATCOMC__)
#ifndef i2WC_NO_TIME
#include <sys/utime.h>
#endif
#define IW_FOPEN(f, m) fopen(f, m)
#define IW_FCLOSE fclose
#define IW_FREAD fread
#define IW_FWRITE fwrite
#define IW_FTELL64 _ftelli64
#define IW_FSEEK64 _fseeki64
#define IW_FILE_STAT_STRUCT stat
#define IW_FILE_STAT stat
#define IW_FFLUSH fflush
#define IW_FREOPEN(f, m, s) freopen(f, m, s)
#define IW_DELETE_FILE remove
	
#elif defined(__TINYC__)
#ifndef i2WC_NO_TIME
#include <sys/utime.h>
#endif
#define IW_FOPEN(f, m) fopen(f, m)
#define IW_FCLOSE fclose
#define IW_FREAD fread
#define IW_FWRITE fwrite
#define IW_FTELL64 ftell
#define IW_FSEEK64 fseek
#define IW_FILE_STAT_STRUCT stat
#define IW_FILE_STAT stat
#define IW_FFLUSH fflush
#define IW_FREOPEN(f, m, s) freopen(f, m, s)
#define IW_DELETE_FILE remove
	
#elif defined(__USE_LARGEFILE64)
#ifndef i2WC_NO_TIME
#include <utime.h>
#endif
#define IW_FOPEN(f, m) fopen64(f, m)
#define IW_FCLOSE fclose
#define IW_FREAD fread
#define IW_FWRITE fwrite
#define IW_FTELL64 ftello64
#define IW_FSEEK64 fseeko64
#define IW_FILE_STAT_STRUCT stat64
#define IW_FILE_STAT stat64
#define IW_FFLUSH fflush
#define IW_FREOPEN(p, m, s) freopen64(p, m, s)
#define IW_DELETE_FILE remove
	
#elif defined(__APPLE__) || defined(__FreeBSD__) || (defined(__linux__) && defined(__x86_64__))
#ifndef i2WC_NO_TIME
#include <utime.h>
#endif
#define IW_FOPEN(f, m) fopen(f, m)
#define IW_FCLOSE fclose
#define IW_FREAD fread
#define IW_FWRITE fwrite
#define IW_FTELL64 ftello
#define IW_FSEEK64 fseeko
#define IW_FILE_STAT_STRUCT stat
#define IW_FILE_STAT stat
#define IW_FFLUSH fflush
#define IW_FREOPEN(p, m, s) freopen(p, m, s)
#define IW_DELETE_FILE remove
	
#else
#pragma message("Using fopen, ftello, fseeko, stat() etc. path for file I/O - this path may not support large files.")
#ifndef i2WC_NO_TIME
#include <utime.h>
#endif
#define IW_FOPEN(f, m) fopen(f, m)
#define IW_FCLOSE fclose
#define IW_FREAD fread
#define IW_FWRITE fwrite
#ifdef __STRICT_ANSI__
#define IW_FTELL64 ftell
#define IW_FSEEK64 fseek
#else
#define IW_FTELL64 ftello
#define IW_FSEEK64 fseeko
#endif
#define IW_FILE_STAT_STRUCT stat
#define IW_FILE_STAT stat
#define IW_FFLUSH fflush
#define IW_FREOPEN(f, m, s) freopen(f, m, s)
#define IW_DELETE_FILE remove
#endif
#endif
	
#define IW_TOLOWER(c) ((((c) >= 'A') && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))
	enum
{
	IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIG = 0x06054b50,
	IW_ZIP_CENTRAL_DIR_HEADER_SIG = 0x02014b50,
	IW_ZIP_LOCAL_DIR_HEADER_SIG = 0x04034b50,
	IW_ZIP_LOCAL_DIR_HEADER_SIZE = 30,
	IW_ZIP_CENTRAL_DIR_HEADER_SIZE = 46,
	IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE = 22,
	
	IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIG = 0x06064b50,
	IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIG = 0x07064b50,
	IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE = 56,
	IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE = 20,
	IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID = 0x0001,
	IW_ZIP_DATA_DESCRIPTOR_ID = 0x08074b50,
	IW_ZIP_DATA_DESCRIPTER_SIZE64 = 24,
	IW_ZIP_DATA_DESCRIPTER_SIZE32 = 16,
	
	IW_ZIP_CDH_SIG_OFS = 0,
	IW_ZIP_CDH_VERSION_MADE_BY_OFS = 4,
	IW_ZIP_CDH_VERSION_NEEDED_OFS = 6,
	IW_ZIP_CDH_BIT_FLAG_OFS = 8,
	IW_ZIP_CDH_METHOD_OFS = 10,
	IW_ZIP_CDH_FILE_TIME_OFS = 12,
	IW_ZIP_CDH_FILE_DATE_OFS = 14,
	IW_ZIP_CDH_CRC32_OFS = 16,
	IW_ZIP_CDH_COMPRESSED_SIZE_OFS = 20,
	IW_ZIP_CDH_DECOMPRESSED_SIZE_OFS = 24,
	IW_ZIP_CDH_FILENAME_LEN_OFS = 28,
	IW_ZIP_CDH_EXTRA_LEN_OFS = 30,
	IW_ZIP_CDH_COMMENT_LEN_OFS = 32,
	IW_ZIP_CDH_DISK_START_OFS = 34,
	IW_ZIP_CDH_INTERNAL_ATTR_OFS = 36,
	IW_ZIP_CDH_EXTERNAL_ATTR_OFS = 38,
	IW_ZIP_CDH_LOCAL_HEADER_OFS = 42,
	
	IW_ZIP_LDH_SIG_OFS = 0,
	IW_ZIP_LDH_VERSION_NEEDED_OFS = 4,
	IW_ZIP_LDH_BIT_FLAG_OFS = 6,
	IW_ZIP_LDH_METHOD_OFS = 8,
	IW_ZIP_LDH_FILE_TIME_OFS = 10,
	IW_ZIP_LDH_FILE_DATE_OFS = 12,
	IW_ZIP_LDH_CRC32_OFS = 14,
	IW_ZIP_LDH_COMPRESSED_SIZE_OFS = 18,
	IW_ZIP_LDH_DECOMPRESSED_SIZE_OFS = 22,
	IW_ZIP_LDH_FILENAME_LEN_OFS = 26,
	IW_ZIP_LDH_EXTRA_LEN_OFS = 28,
	IW_ZIP_LDH_BIT_FLAG_HAS_LOCATOR = 1 << 3,
	
	IW_ZIP_ECDH_SIG_OFS = 0,
	IW_ZIP_ECDH_NUM_THIS_DISK_OFS = 4,
	IW_ZIP_ECDH_NUM_DISK_CDIR_OFS = 6,
	IW_ZIP_ECDH_CDIR_NUM_ENTRIES_ON_DISK_OFS = 8,
	IW_ZIP_ECDH_CDIR_TOTAL_ENTRIES_OFS = 10,
	IW_ZIP_ECDH_CDIR_SIZE_OFS = 12,
	IW_ZIP_ECDH_CDIR_OFS_OFS = 16,
	IW_ZIP_ECDH_COMMENT_SIZE_OFS = 20,
	
	IW_ZIP64_ECDL_SIG_OFS = 0,
	IW_ZIP64_ECDL_NUM_DISK_CDIR_OFS = 4,
	IW_ZIP64_ECDL_REL_OFS_TO_ZIP64_ECDR_OFS = 8,
	IW_ZIP64_ECDL_TOTAL_NUMBER_OF_DISKS_OFS = 16,
	
	IW_ZIP64_ECDH_SIG_OFS = 0,
	IW_ZIP64_ECDH_SIZE_OF_RECORD_OFS = 4,
	IW_ZIP64_ECDH_VERSION_MADE_BY_OFS = 12,
	IW_ZIP64_ECDH_VERSION_NEEDED_OFS = 14,
	IW_ZIP64_ECDH_NUM_THIS_DISK_OFS = 16,
	IW_ZIP64_ECDH_NUM_DISK_CDIR_OFS = 20,
	IW_ZIP64_ECDH_CDIR_NUM_ENTRIES_ON_DISK_OFS = 24,
	IW_ZIP64_ECDH_CDIR_TOTAL_ENTRIES_OFS = 32,
	IW_ZIP64_ECDH_CDIR_SIZE_OFS = 40,
	IW_ZIP64_ECDH_CDIR_OFS_OFS = 48,
	IW_ZIP_VERSION_MADE_BY_DOS_FILESYSTEM_ID = 0,
	IW_ZIP_DOS_DIR_ATTRIBUTE_BITFLAG = 0x10,
	IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_IS_ENCRYPTED = 1,
	IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_COMPRESSED_PATCH_FLAG = 32,
	IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_USES_STRONG_ENCRYPTION = 64,
	IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_LOCAL_DIR_IS_MASKED = 8192,
	IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_UTF8 = 1 << 11
};
	
	typedef struct
	{
		void *m_p;
		size_t m_size, m_capacity;
		iw_uint m_element_size;
	} iw_zip_array;
	
	struct iw_zip_internal_state_tag
	{
		iw_zip_array m_central_dir;
		iw_zip_array m_central_dir_offsets;
		iw_zip_array m_sorted_central_dir_offsets;
		
		iw_uint32 m_init_flags;
		
		iw_bool m_zip64;
		
		iw_bool m_zip64_has_extended_info_fields;
		
		IW_FILE *m_pFile;
		iw_uint64 m_file_archive_start_ofs;
		
		void *m_pMem;
		size_t m_mem_size;
		size_t m_mem_capacity;
	};
	
#define IW_ZIP_ARRAY_SET_ELEMENT_SIZE(array_ptr, element_size) (array_ptr)->m_element_size = element_size
	
#if defined(DEBUG) || defined(_DEBUG)
	static IW_FORCEINLINE iw_uint iw_zip_array_range_check(const iw_zip_array *pArray, iw_uint index)
	{
		IW_ASSERT(index < pArray->m_size);
		return index;
	}
#define IW_ZIP_ARRAY_ELEMENT(array_ptr, element_type, index) ((element_type *)((array_ptr)->m_p))[iw_zip_array_range_check(array_ptr, index)]
#else
#define IW_ZIP_ARRAY_ELEMENT(array_ptr, element_type, index) ((element_type *)((array_ptr)->m_p))[index]
#endif
	
	static IW_FORCEINLINE void iw_zip_array_init(iw_zip_array *pArray, iw_uint32 element_size)
	{
		memset(pArray, 0, sizeof(iw_zip_array));
		pArray->m_element_size = element_size;
	}
	
	static IW_FORCEINLINE void iw_zip_array_clear(iw_zip_archive *pZip, iw_zip_array *pArray)
	{
		pZip->m_pFree(pZip->m_pAlloc_opaque, pArray->m_p);
		memset(pArray, 0, sizeof(iw_zip_array));
	}
	
	static iw_bool iw_zip_array_ensure_capacity(iw_zip_archive *pZip, iw_zip_array *pArray, size_t min_new_capacity, iw_uint growing)
	{
		void *pNew_p;
		size_t new_capacity = min_new_capacity;
		IW_ASSERT(pArray->m_element_size);
		if (pArray->m_capacity >= min_new_capacity)
			return IW_TRUE;
		if (growing)
		{
			new_capacity = IW_MAX(1, pArray->m_capacity);
			while (new_capacity < min_new_capacity)
				new_capacity *= 2;
		}
		if (NULL == (pNew_p = pZip->m_pRealloc(pZip->m_pAlloc_opaque, pArray->m_p, pArray->m_element_size, new_capacity)))
			return IW_FALSE;
		pArray->m_p = pNew_p;
		pArray->m_capacity = new_capacity;
		return IW_TRUE;
	}
	
	static IW_FORCEINLINE iw_bool iw_zip_array_reserve(iw_zip_archive *pZip, iw_zip_array *pArray, size_t new_capacity, iw_uint growing)
	{
		if (new_capacity > pArray->m_capacity)
		{
			if (!iw_zip_array_ensure_capacity(pZip, pArray, new_capacity, growing))
				return IW_FALSE;
		}
		return IW_TRUE;
	}
	
	static IW_FORCEINLINE iw_bool iw_zip_array_resize(iw_zip_archive *pZip, iw_zip_array *pArray, size_t new_size, iw_uint growing)
	{
		if (new_size > pArray->m_capacity)
		{
			if (!iw_zip_array_ensure_capacity(pZip, pArray, new_size, growing))
				return IW_FALSE;
		}
		pArray->m_size = new_size;
		return IW_TRUE;
	}
	
	static IW_FORCEINLINE iw_bool iw_zip_array_ensure_room(iw_zip_archive *pZip, iw_zip_array *pArray, size_t n)
	{
		return iw_zip_array_reserve(pZip, pArray, pArray->m_size + n, IW_TRUE);
	}
	
	static IW_FORCEINLINE iw_bool iw_zip_array_push_back(iw_zip_archive *pZip, iw_zip_array *pArray, const void *pElements, size_t n)
	{
		size_t orig_size = pArray->m_size;
		if (!iw_zip_array_resize(pZip, pArray, orig_size + n, IW_TRUE))
			return IW_FALSE;
		if (n > 0)
			memcpy((iw_uint8 *)pArray->m_p + orig_size * pArray->m_element_size, pElements, n * pArray->m_element_size);
		return IW_TRUE;
	}
	
#ifndef i2WC_NO_TIME
	static IW_TIME_T iw_zip_dos_to_time_t(int dos_time, int dos_date)
	{
		struct tm tm;
		memset(&tm, 0, sizeof(tm));
		tm.tm_isdst = -1;
		tm.tm_year = ((dos_date >> 9) & 127) + 1980 - 1900;
		tm.tm_mon = ((dos_date >> 5) & 15) - 1;
		tm.tm_mday = dos_date & 31;
		tm.tm_hour = (dos_time >> 11) & 31;
		tm.tm_min = (dos_time >> 5) & 63;
		tm.tm_sec = (dos_time << 1) & 62;
		return mktime(&tm);
	}
	
#ifndef i2WC_NO_ARCHIVE_WRITING_APIS
	static void iw_zip_time_t_to_dos_time(IW_TIME_T time, iw_uint16 *pDOS_time, iw_uint16 *pDOS_date)
	{
#ifdef _MSC_VER
		struct tm tm_struct;
		struct tm *tm = &tm_struct;
		errno_t err = localtime_s(tm, &time);
		if (err)
		{
			*pDOS_date = 0;
			*pDOS_time = 0;
			return;
		}
#else
		struct tm *tm = localtime(&time);
#endif
		
		*pDOS_time = (iw_uint16)(((tm->tm_hour) << 11) + ((tm->tm_min) << 5) + ((tm->tm_sec) >> 1));
		*pDOS_date = (iw_uint16)(((tm->tm_year + 1900 - 1980) << 9) + ((tm->tm_mon + 1) << 5) + tm->tm_mday);
	}
#endif
	
#ifndef i2WC_NO_STDIO
#ifndef i2WC_NO_ARCHIVE_WRITING_APIS
	static iw_bool iw_zip_get_file_modified_time(const char *pFilename, IW_TIME_T *pTime)
	{
		struct IW_FILE_STAT_STRUCT file_stat;
		
		if (IW_FILE_STAT(pFilename, &file_stat) != 0)
			return IW_FALSE;
		
		*pTime = file_stat.st_mtime;
		
		return IW_TRUE;
	}
#endif
	
	static iw_bool iw_zip_set_file_times(const char *pFilename, IW_TIME_T access_time, IW_TIME_T modified_time)
	{
		struct utimbuf t;
		
		memset(&t, 0, sizeof(t));
		t.actime = access_time;
		t.modtime = modified_time;
		
		return !utime(pFilename, &t);
	}
#endif
#endif
	
	static IW_FORCEINLINE iw_bool iw_zip_set_error(iw_zip_archive *pZip, iw_zip_error err_num)
	{
		if (pZip)
			pZip->m_last_error = err_num;
		return IW_FALSE;
	}
	
	static iw_bool iw_zip_reader_init_internal(iw_zip_archive *pZip, iw_uint flags)
	{
		(void)flags;
		if ((!pZip) || (pZip->m_pState) || (pZip->m_zip_mode != IW_ZIP_MODE_INVALID))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (!pZip->m_pAlloc)
			pZip->m_pAlloc = i2wC_def_alloc_func;
		if (!pZip->m_pFree)
			pZip->m_pFree = i2wC_def_free_func;
		if (!pZip->m_pRealloc)
			pZip->m_pRealloc = i2wC_def_realloc_func;
		
		pZip->m_archive_size = 0;
		pZip->m_central_directory_file_ofs = 0;
		pZip->m_total_files = 0;
		pZip->m_last_error = IW_ZIP_NO_ERROR;
		
		if (NULL == (pZip->m_pState = (iw_zip_internal_state *)pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, sizeof(iw_zip_internal_state))))
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		
		memset(pZip->m_pState, 0, sizeof(iw_zip_internal_state));
		IW_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_central_dir, sizeof(iw_uint8));
		IW_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_central_dir_offsets, sizeof(iw_uint32));
		IW_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_sorted_central_dir_offsets, sizeof(iw_uint32));
		pZip->m_pState->m_init_flags = flags;
		pZip->m_pState->m_zip64 = IW_FALSE;
		pZip->m_pState->m_zip64_has_extended_info_fields = IW_FALSE;
		
		pZip->m_zip_mode = IW_ZIP_MODE_READING;
		
		return IW_TRUE;
	}
	
	static IW_FORCEINLINE iw_bool iw_zip_reader_filename_less(const iw_zip_array *pCentral_dir_array, const iw_zip_array *pCentral_dir_offsets, iw_uint l_index, iw_uint r_index)
	{
		const iw_uint8 *pL = &IW_ZIP_ARRAY_ELEMENT(pCentral_dir_array, iw_uint8, IW_ZIP_ARRAY_ELEMENT(pCentral_dir_offsets, iw_uint32, l_index)), *pE;
		const iw_uint8 *pR = &IW_ZIP_ARRAY_ELEMENT(pCentral_dir_array, iw_uint8, IW_ZIP_ARRAY_ELEMENT(pCentral_dir_offsets, iw_uint32, r_index));
		iw_uint l_len = IW_READ_LE16(pL + IW_ZIP_CDH_FILENAME_LEN_OFS), r_len = IW_READ_LE16(pR + IW_ZIP_CDH_FILENAME_LEN_OFS);
		iw_uint8 l = 0, r = 0;
		pL += IW_ZIP_CENTRAL_DIR_HEADER_SIZE;
		pR += IW_ZIP_CENTRAL_DIR_HEADER_SIZE;
		pE = pL + IW_MIN(l_len, r_len);
		while (pL < pE)
		{
			if ((l = IW_TOLOWER(*pL)) != (r = IW_TOLOWER(*pR)))
				break;
			pL++;
			pR++;
		}
		return (pL == pE) ? (l_len < r_len) : (l < r);
	}
	
#define IW_SWAP_UINT32(a, b) \
	do                       \
	{                        \
	iw_uint32 t = a;     \
	a = b;               \
	b = t;               \
	}                        \
	IW_MACRO_END
	
	static void iw_zip_reader_sort_central_dir_offsets_by_filename(iw_zip_archive *pZip)
	{
		iw_zip_internal_state *pState = pZip->m_pState;
		const iw_zip_array *pCentral_dir_offsets = &pState->m_central_dir_offsets;
		const iw_zip_array *pCentral_dir = &pState->m_central_dir;
		iw_uint32 *pIndices;
		iw_uint32 start, end;
		const iw_uint32 size = pZip->m_total_files;
		
		if (size <= 1U)
			return;
		
		pIndices = &IW_ZIP_ARRAY_ELEMENT(&pState->m_sorted_central_dir_offsets, iw_uint32, 0);
		
		start = (size - 2U) >> 1U;
		for (;;)
		{
			iw_uint64 child, root = start;
			for (;;)
			{
				if ((child = (root << 1U) + 1U) >= size)
					break;
				child += (((child + 1U) < size) && (iw_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets, pIndices[child], pIndices[child + 1U])));
				if (!iw_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets, pIndices[root], pIndices[child]))
					break;
				IW_SWAP_UINT32(pIndices[root], pIndices[child]);
				root = child;
			}
			if (!start)
				break;
			start--;
		}
		
		end = size - 1;
		while (end > 0)
		{
			iw_uint64 child, root = 0;
			IW_SWAP_UINT32(pIndices[end], pIndices[0]);
			for (;;)
			{
				if ((child = (root << 1U) + 1U) >= end)
					break;
				child += (((child + 1U) < end) && iw_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets, pIndices[child], pIndices[child + 1U]));
				if (!iw_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets, pIndices[root], pIndices[child]))
					break;
				IW_SWAP_UINT32(pIndices[root], pIndices[child]);
				root = child;
			}
			end--;
		}
	}
	
	static iw_bool iw_zip_reader_locate_header_sig(iw_zip_archive *pZip, iw_uint32 record_sig, iw_uint32 record_size, iw_int64 *pOfs)
	{
		iw_int64 cur_file_ofs;
		iw_uint32 buf_u32[4096 / sizeof(iw_uint32)];
		iw_uint8 *pBuf = (iw_uint8 *)buf_u32;
		
		if (pZip->m_archive_size < record_size)
			return IW_FALSE;
		
		cur_file_ofs = IW_MAX((iw_int64)pZip->m_archive_size - (iw_int64)sizeof(buf_u32), 0);
		for (;;)
		{
			int i, n = (int)IW_MIN(sizeof(buf_u32), pZip->m_archive_size - cur_file_ofs);
			
			if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pBuf, n) != (iw_uint)n)
				return IW_FALSE;
			
			for (i = n - 4; i >= 0; --i)
			{
				iw_uint s = IW_READ_LE32(pBuf + i);
				if (s == record_sig)
				{
					if ((pZip->m_archive_size - (cur_file_ofs + i)) >= record_size)
						break;
				}
			}
			
			if (i >= 0)
			{
				cur_file_ofs += i;
				break;
			}
			
			if ((!cur_file_ofs) || ((pZip->m_archive_size - cur_file_ofs) >= ((iw_uint64)(IW_UINT16_MAX) + record_size)))
				return IW_FALSE;
			
			cur_file_ofs = IW_MAX(cur_file_ofs - (sizeof(buf_u32) - 3), 0);
		}
		
		*pOfs = cur_file_ofs;
		return IW_TRUE;
	}
	
	static iw_bool iw_zip_reader_eocd64_valid(iw_zip_archive *pZip, uint64_t offset, uint8_t *buf)
	{
		if (pZip->m_pRead(pZip->m_pIO_opaque, offset, buf, IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE) == IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE)
		{
			if (IW_READ_LE32(buf + IW_ZIP64_ECDH_SIG_OFS) == IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIG)
			{
				return IW_TRUE;
			}
		}
		
		return IW_FALSE;
	}
	
	static iw_bool iw_zip_reader_read_central_dir(iw_zip_archive *pZip, iw_uint flags)
	{
		iw_uint cdir_size = 0, cdir_entries_on_this_disk = 0, num_this_disk = 0, cdir_disk_index = 0;
		iw_uint64 cdir_ofs = 0, eocd_ofs = 0, archive_ofs = 0;
		iw_int64 cur_file_ofs = 0;
		const iw_uint8 *p;
		
		iw_uint32 buf_u32[4096 / sizeof(iw_uint32)];
		iw_uint8 *pBuf = (iw_uint8 *)buf_u32;
		iw_bool sort_central_dir = ((flags & IW_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY) == 0);
		iw_uint32 zip64_end_of_central_dir_locator_u32[(IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pZip64_locator = (iw_uint8 *)zip64_end_of_central_dir_locator_u32;
		
		iw_uint32 zip64_end_of_central_dir_header_u32[(IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pZip64_end_of_central_dir = (iw_uint8 *)zip64_end_of_central_dir_header_u32;
		
		iw_uint64 zip64_end_of_central_dir_ofs = 0;
		
		if (pZip->m_archive_size < IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
		
		if (!iw_zip_reader_locate_header_sig(pZip, IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIG, IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE, &cur_file_ofs))
			return iw_zip_set_error(pZip, IW_ZIP_FAILED_FINDING_CENTRAL_DIR);
		
		eocd_ofs = cur_file_ofs;
		if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pBuf, IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE) != IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
		
		if (IW_READ_LE32(pBuf + IW_ZIP_ECDH_SIG_OFS) != IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIG)
			return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
		
		if (cur_file_ofs >= (IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE + IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE))
		{
			if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs - IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE, pZip64_locator, IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE) == IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE)
			{
				if (IW_READ_LE32(pZip64_locator + IW_ZIP64_ECDL_SIG_OFS) == IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIG)
				{
					pZip->m_pState->m_zip64 = IW_TRUE;
				}
			}
		}
		
		if (pZip->m_pState->m_zip64)
		{
			if (cur_file_ofs < IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE +
				IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE)
				return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
			
			zip64_end_of_central_dir_ofs = cur_file_ofs -
			IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE -
			IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE;
			
			if (!iw_zip_reader_eocd64_valid(pZip, zip64_end_of_central_dir_ofs,
											pZip64_end_of_central_dir))
			{
				zip64_end_of_central_dir_ofs = IW_READ_LE64(
															pZip64_locator + IW_ZIP64_ECDL_REL_OFS_TO_ZIP64_ECDR_OFS);
				
				if (zip64_end_of_central_dir_ofs >
					(pZip->m_archive_size - IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE))
					return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
				
				if (!iw_zip_reader_eocd64_valid(pZip, zip64_end_of_central_dir_ofs,
												pZip64_end_of_central_dir))
					return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
			}
		}
		
		pZip->m_total_files = IW_READ_LE16(pBuf + IW_ZIP_ECDH_CDIR_TOTAL_ENTRIES_OFS);
		cdir_entries_on_this_disk = IW_READ_LE16(pBuf + IW_ZIP_ECDH_CDIR_NUM_ENTRIES_ON_DISK_OFS);
		num_this_disk = IW_READ_LE16(pBuf + IW_ZIP_ECDH_NUM_THIS_DISK_OFS);
		cdir_disk_index = IW_READ_LE16(pBuf + IW_ZIP_ECDH_NUM_DISK_CDIR_OFS);
		cdir_size = IW_READ_LE32(pBuf + IW_ZIP_ECDH_CDIR_SIZE_OFS);
		cdir_ofs = IW_READ_LE32(pBuf + IW_ZIP_ECDH_CDIR_OFS_OFS);
		
		if (pZip->m_pState->m_zip64)
		{
			iw_uint32 zip64_total_num_of_disks = IW_READ_LE32(pZip64_locator + IW_ZIP64_ECDL_TOTAL_NUMBER_OF_DISKS_OFS);
			iw_uint64 zip64_cdir_total_entries = IW_READ_LE64(pZip64_end_of_central_dir + IW_ZIP64_ECDH_CDIR_TOTAL_ENTRIES_OFS);
			iw_uint64 zip64_cdir_total_entries_on_this_disk = IW_READ_LE64(pZip64_end_of_central_dir + IW_ZIP64_ECDH_CDIR_NUM_ENTRIES_ON_DISK_OFS);
			iw_uint64 zip64_size_of_end_of_central_dir_record = IW_READ_LE64(pZip64_end_of_central_dir + IW_ZIP64_ECDH_SIZE_OF_RECORD_OFS);
			iw_uint64 zip64_size_of_central_directory = IW_READ_LE64(pZip64_end_of_central_dir + IW_ZIP64_ECDH_CDIR_SIZE_OFS);
			
			if (zip64_size_of_end_of_central_dir_record < (IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE - 12))
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
			
			if (zip64_total_num_of_disks != 1U)
				return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_MULTIDISK);
			
			if (zip64_cdir_total_entries > IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
			
			pZip->m_total_files = (iw_uint32)zip64_cdir_total_entries;
			
			if (zip64_cdir_total_entries_on_this_disk > IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
			
			cdir_entries_on_this_disk = (iw_uint32)zip64_cdir_total_entries_on_this_disk;
			
			if (zip64_size_of_central_directory > IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_CDIR_SIZE);
			
			cdir_size = (iw_uint32)zip64_size_of_central_directory;
			
			num_this_disk = IW_READ_LE32(pZip64_end_of_central_dir + IW_ZIP64_ECDH_NUM_THIS_DISK_OFS);
			
			cdir_disk_index = IW_READ_LE32(pZip64_end_of_central_dir + IW_ZIP64_ECDH_NUM_DISK_CDIR_OFS);
			
			cdir_ofs = IW_READ_LE64(pZip64_end_of_central_dir + IW_ZIP64_ECDH_CDIR_OFS_OFS);
		}
		
		if (pZip->m_total_files != cdir_entries_on_this_disk)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_MULTIDISK);
		
		if (((num_this_disk | cdir_disk_index) != 0) && ((num_this_disk != 1) || (cdir_disk_index != 1)))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_MULTIDISK);
		
		if (cdir_size < (iw_uint64)pZip->m_total_files * IW_ZIP_CENTRAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		if ((cdir_ofs + (iw_uint64)cdir_size) > pZip->m_archive_size)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		if (eocd_ofs < cdir_ofs + cdir_size)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		archive_ofs = eocd_ofs - (cdir_ofs + cdir_size);
		if (pZip->m_pState->m_zip64)
		{
			if (archive_ofs < IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE +
				IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE)
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
			
			archive_ofs -= IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE +
			IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE;
		}
		
		if ((pZip->m_zip_type == IW_ZIP_TYPE_FILE || pZip->m_zip_type == IW_ZIP_TYPE_CFILE ||
			 pZip->m_zip_type == IW_ZIP_TYPE_USER) && pZip->m_pState->m_file_archive_start_ofs == 0)
		{
			pZip->m_pState->m_file_archive_start_ofs = archive_ofs;
			pZip->m_archive_size -= archive_ofs;
		}
		
		pZip->m_central_directory_file_ofs = cdir_ofs;
		
		if (pZip->m_total_files)
		{
			iw_uint i, n;
			if ((!iw_zip_array_resize(pZip, &pZip->m_pState->m_central_dir, cdir_size, IW_FALSE)) ||
				(!iw_zip_array_resize(pZip, &pZip->m_pState->m_central_dir_offsets, pZip->m_total_files, IW_FALSE)))
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			
			if (sort_central_dir)
			{
				if (!iw_zip_array_resize(pZip, &pZip->m_pState->m_sorted_central_dir_offsets, pZip->m_total_files, IW_FALSE))
					return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			if (pZip->m_pRead(pZip->m_pIO_opaque, cdir_ofs, pZip->m_pState->m_central_dir.m_p, cdir_size) != cdir_size)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
			
			p = (const iw_uint8 *)pZip->m_pState->m_central_dir.m_p;
			for (n = cdir_size, i = 0; i < pZip->m_total_files; ++i)
			{
				iw_uint total_header_size, disk_index, bit_flags, filename_size, ext_data_size;
				iw_uint64 comp_size, decomp_size, local_header_ofs;
				
				if ((n < IW_ZIP_CENTRAL_DIR_HEADER_SIZE) || (IW_READ_LE32(p) != IW_ZIP_CENTRAL_DIR_HEADER_SIG))
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				
				IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir_offsets, iw_uint32, i) = (iw_uint32)(p - (const iw_uint8 *)pZip->m_pState->m_central_dir.m_p);
				
				if (sort_central_dir)
					IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_sorted_central_dir_offsets, iw_uint32, i) = i;
				
				comp_size = IW_READ_LE32(p + IW_ZIP_CDH_COMPRESSED_SIZE_OFS);
				decomp_size = IW_READ_LE32(p + IW_ZIP_CDH_DECOMPRESSED_SIZE_OFS);
				local_header_ofs = IW_READ_LE32(p + IW_ZIP_CDH_LOCAL_HEADER_OFS);
				filename_size = IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS);
				ext_data_size = IW_READ_LE16(p + IW_ZIP_CDH_EXTRA_LEN_OFS);
				
				if ((!pZip->m_pState->m_zip64_has_extended_info_fields) &&
					(ext_data_size) &&
					(IW_MAX(IW_MAX(comp_size, decomp_size), local_header_ofs) == IW_UINT32_MAX))
				{
					iw_uint32 extra_size_remaining = ext_data_size;
					
					if (extra_size_remaining)
					{
						const iw_uint8 *pExtra_data;
						void *buf = NULL;
						
						if (IW_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_size + ext_data_size > n)
						{
							buf = IW_MALLOC(ext_data_size);
							if (buf == NULL)
								return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
							
							if (pZip->m_pRead(pZip->m_pIO_opaque, cdir_ofs + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_size, buf, ext_data_size) != ext_data_size)
							{
								IW_FREE(buf);
								return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
							}
							
							pExtra_data = (iw_uint8 *)buf;
						}
						else
						{
							pExtra_data = p + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_size;
						}
						
						do
						{
							iw_uint32 field_id;
							iw_uint32 field_data_size;
							
							if (extra_size_remaining < (sizeof(iw_uint16) * 2))
							{
								IW_FREE(buf);
								return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
							}
							
							field_id = IW_READ_LE16(pExtra_data);
							field_data_size = IW_READ_LE16(pExtra_data + sizeof(iw_uint16));
							
							if ((field_data_size + sizeof(iw_uint16) * 2) > extra_size_remaining)
							{
								IW_FREE(buf);
								return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
							}
							
							if (field_id == IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID)
							{
								pZip->m_pState->m_zip64 = IW_TRUE;
								pZip->m_pState->m_zip64_has_extended_info_fields = IW_TRUE;
								break;
							}
							
							pExtra_data += sizeof(iw_uint16) * 2 + field_data_size;
							extra_size_remaining = extra_size_remaining - sizeof(iw_uint16) * 2 - field_data_size;
						} while (extra_size_remaining);
						
						IW_FREE(buf);
					}
				}
				
				if ((comp_size != IW_UINT32_MAX) && (decomp_size != IW_UINT32_MAX))
				{
					if (((!IW_READ_LE32(p + IW_ZIP_CDH_METHOD_OFS)) && (decomp_size != comp_size)) || (decomp_size && !comp_size))
						return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				}
				
				disk_index = IW_READ_LE16(p + IW_ZIP_CDH_DISK_START_OFS);
				if ((disk_index == IW_UINT16_MAX) || ((disk_index != num_this_disk) && (disk_index != 1)))
					return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_MULTIDISK);
				
				if (comp_size != IW_UINT32_MAX)
				{
					if (((iw_uint64)IW_READ_LE32(p + IW_ZIP_CDH_LOCAL_HEADER_OFS) + IW_ZIP_LOCAL_DIR_HEADER_SIZE + comp_size) > pZip->m_archive_size)
						return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				}
				
				bit_flags = IW_READ_LE16(p + IW_ZIP_CDH_BIT_FLAG_OFS);
				if (bit_flags & IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_LOCAL_DIR_IS_MASKED)
					return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_ENCRYPTION);
				
				if ((total_header_size = IW_ZIP_CENTRAL_DIR_HEADER_SIZE + IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS) + IW_READ_LE16(p + IW_ZIP_CDH_EXTRA_LEN_OFS) + IW_READ_LE16(p + IW_ZIP_CDH_COMMENT_LEN_OFS)) > n)
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				
				n -= total_header_size;
				p += total_header_size;
			}
		}
		
		if (sort_central_dir)
			iw_zip_reader_sort_central_dir_offsets_by_filename(pZip);
		
		return IW_TRUE;
	}
	
	void iw_zip_zero_struct(iw_zip_archive *pZip)
	{
		if (pZip)
			IW_CLEAR_PTR(pZip);
	}
	
	static iw_bool iw_zip_reader_end_internal(iw_zip_archive *pZip, iw_bool set_last_error)
	{
		iw_bool status = IW_TRUE;
		
		if (!pZip)
			return IW_FALSE;
		
		if ((!pZip->m_pState) || (!pZip->m_pAlloc) || (!pZip->m_pFree) || (pZip->m_zip_mode != IW_ZIP_MODE_READING))
		{
			if (set_last_error)
				pZip->m_last_error = IW_ZIP_INVALID_PARAMETER;
			
			return IW_FALSE;
		}
		
		if (pZip->m_pState)
		{
			iw_zip_internal_state *pState = pZip->m_pState;
			pZip->m_pState = NULL;
			
			iw_zip_array_clear(pZip, &pState->m_central_dir);
			iw_zip_array_clear(pZip, &pState->m_central_dir_offsets);
			iw_zip_array_clear(pZip, &pState->m_sorted_central_dir_offsets);
			
#ifndef i2WC_NO_STDIO
			if (pState->m_pFile)
			{
				if (pZip->m_zip_type == IW_ZIP_TYPE_FILE)
				{
					if (IW_FCLOSE(pState->m_pFile) == EOF)
					{
						if (set_last_error)
							pZip->m_last_error = IW_ZIP_FILE_CLOSE_FAILED;
						status = IW_FALSE;
					}
				}
				pState->m_pFile = NULL;
			}
#endif
			
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
		}
		pZip->m_zip_mode = IW_ZIP_MODE_INVALID;
		
		return status;
	}
	
	iw_bool iw_zip_reader_end(iw_zip_archive *pZip)
	{
		return iw_zip_reader_end_internal(pZip, IW_TRUE);
	}
	iw_bool iw_zip_reader_init(iw_zip_archive *pZip, iw_uint64 size, iw_uint flags)
	{
		if ((!pZip) || (!pZip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (!iw_zip_reader_init_internal(pZip, flags))
			return IW_FALSE;
		
		pZip->m_zip_type = IW_ZIP_TYPE_USER;
		pZip->m_archive_size = size;
		
		if (!iw_zip_reader_read_central_dir(pZip, flags))
		{
			iw_zip_reader_end_internal(pZip, IW_FALSE);
			return IW_FALSE;
		}
		
		return IW_TRUE;
	}
	
	static size_t iw_zip_mem_read_func(void *pOpaque, iw_uint64 file_ofs, void *pBuf, size_t n)
	{
		iw_zip_archive *pZip = (iw_zip_archive *)pOpaque;
		size_t s = (file_ofs >= pZip->m_archive_size) ? 0 : (size_t)IW_MIN(pZip->m_archive_size - file_ofs, n);
		memcpy(pBuf, (const iw_uint8 *)pZip->m_pState->m_pMem + file_ofs, s);
		return s;
	}
	
	iw_bool iw_zip_reader_init_mem(iw_zip_archive *pZip, const void *pMem, size_t size, iw_uint flags)
	{
		if (!pMem)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (size < IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
		
		if (!iw_zip_reader_init_internal(pZip, flags))
			return IW_FALSE;
		
		pZip->m_zip_type = IW_ZIP_TYPE_MEMORY;
		pZip->m_archive_size = size;
		pZip->m_pRead = iw_zip_mem_read_func;
		pZip->m_pIO_opaque = pZip;
		pZip->m_pNeeds_keepalive = NULL;
		
#ifdef __cplusplus
		pZip->m_pState->m_pMem = const_cast<void *>(pMem);
#else
		pZip->m_pState->m_pMem = (void *)pMem;
#endif
		
		pZip->m_pState->m_mem_size = size;
		
		if (!iw_zip_reader_read_central_dir(pZip, flags))
		{
			iw_zip_reader_end_internal(pZip, IW_FALSE);
			return IW_FALSE;
		}
		
		return IW_TRUE;
	}
	
#ifndef i2WC_NO_STDIO
	static size_t iw_zip_file_read_func(void *pOpaque, iw_uint64 file_ofs, void *pBuf, size_t n)
	{
		iw_zip_archive *pZip = (iw_zip_archive *)pOpaque;
		iw_int64 cur_ofs = IW_FTELL64(pZip->m_pState->m_pFile);
		
		file_ofs += pZip->m_pState->m_file_archive_start_ofs;
		
		if (((iw_int64)file_ofs < 0) || (((cur_ofs != (iw_int64)file_ofs)) && (IW_FSEEK64(pZip->m_pState->m_pFile, (iw_int64)file_ofs, SEEK_SET))))
			return 0;
		
		return IW_FREAD(pBuf, 1, n, pZip->m_pState->m_pFile);
	}
	
	iw_bool iw_zip_reader_init_file(iw_zip_archive *pZip, const char *pFilename, iw_uint32 flags)
	{
		return iw_zip_reader_init_file_v2(pZip, pFilename, flags, 0, 0);
	}
	
	iw_bool iw_zip_reader_init_file_v2(iw_zip_archive *pZip, const char *pFilename, iw_uint flags, iw_uint64 file_start_ofs, iw_uint64 archive_size)
	{
		iw_uint64 file_size;
		IW_FILE *pFile;
		
		if ((!pZip) || (!pFilename) || ((archive_size) && (archive_size < IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pFile = IW_FOPEN(pFilename, (flags & IW_ZIP_FLAG_READ_ALLOW_WRITING ) ? "r+b" : "rb");
		if (!pFile)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_OPEN_FAILED);
		
		file_size = archive_size;
		if (!file_size)
		{
			if (IW_FSEEK64(pFile, 0, SEEK_END))
			{
				IW_FCLOSE(pFile);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_SEEK_FAILED);
			}
			
			file_size = IW_FTELL64(pFile);
		}
		
		if (file_size < IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)
		{
			IW_FCLOSE(pFile);
			return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
		}
		
		if (!iw_zip_reader_init_internal(pZip, flags))
		{
			IW_FCLOSE(pFile);
			return IW_FALSE;
		}
		
		pZip->m_zip_type = IW_ZIP_TYPE_FILE;
		pZip->m_pRead = iw_zip_file_read_func;
		pZip->m_pIO_opaque = pZip;
		pZip->m_pState->m_pFile = pFile;
		pZip->m_archive_size = file_size;
		pZip->m_pState->m_file_archive_start_ofs = file_start_ofs;
		
		if (!iw_zip_reader_read_central_dir(pZip, flags))
		{
			iw_zip_reader_end_internal(pZip, IW_FALSE);
			return IW_FALSE;
		}
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_reader_init_cfile(iw_zip_archive *pZip, IW_FILE *pFile, iw_uint64 archive_size, iw_uint flags)
	{
		iw_uint64 cur_file_ofs;
		
		if ((!pZip) || (!pFile))
			return iw_zip_set_error(pZip, IW_ZIP_FILE_OPEN_FAILED);
		
		cur_file_ofs = IW_FTELL64(pFile);
		
		if (!archive_size)
		{
			if (IW_FSEEK64(pFile, 0, SEEK_END))
				return iw_zip_set_error(pZip, IW_ZIP_FILE_SEEK_FAILED);
			
			archive_size = IW_FTELL64(pFile) - cur_file_ofs;
			
			if (archive_size < IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)
				return iw_zip_set_error(pZip, IW_ZIP_NOT_AN_ARCHIVE);
		}
		
		if (!iw_zip_reader_init_internal(pZip, flags))
			return IW_FALSE;
		
		pZip->m_zip_type = IW_ZIP_TYPE_CFILE;
		pZip->m_pRead = iw_zip_file_read_func;
		
		pZip->m_pIO_opaque = pZip;
		pZip->m_pState->m_pFile = pFile;
		pZip->m_archive_size = archive_size;
		pZip->m_pState->m_file_archive_start_ofs = cur_file_ofs;
		
		if (!iw_zip_reader_read_central_dir(pZip, flags))
		{
			iw_zip_reader_end_internal(pZip, IW_FALSE);
			return IW_FALSE;
		}
		
		return IW_TRUE;
	}
	
#endif
	
	static IW_FORCEINLINE const iw_uint8 *iw_zip_get_cdh(iw_zip_archive *pZip, iw_uint file_index)
	{
		if ((!pZip) || (!pZip->m_pState) || (file_index >= pZip->m_total_files))
			return NULL;
		return &IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir, iw_uint8, IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir_offsets, iw_uint32, file_index));
	}
	
	iw_bool iw_zip_reader_is_file_encrypted(iw_zip_archive *pZip, iw_uint file_index)
	{
		iw_uint m_bit_flag;
		const iw_uint8 *p = iw_zip_get_cdh(pZip, file_index);
		if (!p)
		{
			iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			return IW_FALSE;
		}
		
		m_bit_flag = IW_READ_LE16(p + IW_ZIP_CDH_BIT_FLAG_OFS);
		return (m_bit_flag & (IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_IS_ENCRYPTED | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_USES_STRONG_ENCRYPTION)) != 0;
	}
	
	iw_bool iw_zip_reader_is_file_supported(iw_zip_archive *pZip, iw_uint file_index)
	{
		iw_uint bit_flag;
		iw_uint method;
		
		const iw_uint8 *p = iw_zip_get_cdh(pZip, file_index);
		if (!p)
		{
			iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			return IW_FALSE;
		}
		
		method = IW_READ_LE16(p + IW_ZIP_CDH_METHOD_OFS);
		bit_flag = IW_READ_LE16(p + IW_ZIP_CDH_BIT_FLAG_OFS);
		
		if ((method != 0) && (method != IW_DEFLATED))
		{
			iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_METHOD);
			return IW_FALSE;
		}
		
		if (bit_flag & (IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_IS_ENCRYPTED | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_USES_STRONG_ENCRYPTION))
		{
			iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_ENCRYPTION);
			return IW_FALSE;
		}
		
		if (bit_flag & IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_COMPRESSED_PATCH_FLAG)
		{
			iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_FEATURE);
			return IW_FALSE;
		}
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_reader_is_file_a_directory(iw_zip_archive *pZip, iw_uint file_index)
	{
		iw_uint filename_len, attribute_mapping_id, external_attr;
		const iw_uint8 *p = iw_zip_get_cdh(pZip, file_index);
		if (!p)
		{
			iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			return IW_FALSE;
		}
		
		filename_len = IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS);
		if (filename_len)
		{
			if (*(p + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_len - 1) == '/')
				return IW_TRUE;
		}
		
		attribute_mapping_id = IW_READ_LE16(p + IW_ZIP_CDH_VERSION_MADE_BY_OFS) >> 8;
		(void)attribute_mapping_id;
		
		external_attr = IW_READ_LE32(p + IW_ZIP_CDH_EXTERNAL_ATTR_OFS);
		if ((external_attr & IW_ZIP_DOS_DIR_ATTRIBUTE_BITFLAG) != 0)
		{
			return IW_TRUE;
		}
		
		return IW_FALSE;
	}
	
	static iw_bool iw_zip_file_stat_internal(iw_zip_archive *pZip, iw_uint file_index, const iw_uint8 *pCentral_dir_header, iw_zip_archive_file_stat *pStat, iw_bool *pFound_zip64_extra_data)
	{
		iw_uint n;
		const iw_uint8 *p = pCentral_dir_header;
		
		if (pFound_zip64_extra_data)
			*pFound_zip64_extra_data = IW_FALSE;
		
		if ((!p) || (!pStat))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pStat->m_file_index = file_index;
		pStat->m_central_dir_ofs = IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir_offsets, iw_uint32, file_index);
		pStat->m_version_made_by = IW_READ_LE16(p + IW_ZIP_CDH_VERSION_MADE_BY_OFS);
		pStat->m_version_needed = IW_READ_LE16(p + IW_ZIP_CDH_VERSION_NEEDED_OFS);
		pStat->m_bit_flag = IW_READ_LE16(p + IW_ZIP_CDH_BIT_FLAG_OFS);
		pStat->m_method = IW_READ_LE16(p + IW_ZIP_CDH_METHOD_OFS);
#ifndef i2WC_NO_TIME
		pStat->m_time = iw_zip_dos_to_time_t(IW_READ_LE16(p + IW_ZIP_CDH_FILE_TIME_OFS), IW_READ_LE16(p + IW_ZIP_CDH_FILE_DATE_OFS));
#endif
		pStat->m_crc32 = IW_READ_LE32(p + IW_ZIP_CDH_CRC32_OFS);
		pStat->m_comp_size = IW_READ_LE32(p + IW_ZIP_CDH_COMPRESSED_SIZE_OFS);
		pStat->m_uncomp_size = IW_READ_LE32(p + IW_ZIP_CDH_DECOMPRESSED_SIZE_OFS);
		pStat->m_internal_attr = IW_READ_LE16(p + IW_ZIP_CDH_INTERNAL_ATTR_OFS);
		pStat->m_external_attr = IW_READ_LE32(p + IW_ZIP_CDH_EXTERNAL_ATTR_OFS);
		pStat->m_local_header_ofs = IW_READ_LE32(p + IW_ZIP_CDH_LOCAL_HEADER_OFS);
		
		n = IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS);
		n = IW_MIN(n, IW_ZIP_MAX_ARCHIVE_FILENAME_SIZE - 1);
		memcpy(pStat->m_filename, p + IW_ZIP_CENTRAL_DIR_HEADER_SIZE, n);
		pStat->m_filename[n] = '\0';
		
		n = IW_READ_LE16(p + IW_ZIP_CDH_COMMENT_LEN_OFS);
		n = IW_MIN(n, IW_ZIP_MAX_ARCHIVE_FILE_COMMENT_SIZE - 1);
		pStat->m_comment_size = n;
		memcpy(pStat->m_comment, p + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS) + IW_READ_LE16(p + IW_ZIP_CDH_EXTRA_LEN_OFS), n);
		pStat->m_comment[n] = '\0';
		
		pStat->m_is_directory = iw_zip_reader_is_file_a_directory(pZip, file_index);
		pStat->m_is_encrypted = iw_zip_reader_is_file_encrypted(pZip, file_index);
		pStat->m_is_supported = iw_zip_reader_is_file_supported(pZip, file_index);
		
		if (IW_MAX(IW_MAX(pStat->m_comp_size, pStat->m_uncomp_size), pStat->m_local_header_ofs) == IW_UINT32_MAX)
		{
			iw_uint32 extra_size_remaining = IW_READ_LE16(p + IW_ZIP_CDH_EXTRA_LEN_OFS);
			
			if (extra_size_remaining)
			{
				const iw_uint8 *pExtra_data = p + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS);
				
				do
				{
					iw_uint32 field_id;
					iw_uint32 field_data_size;
					
					if (extra_size_remaining < (sizeof(iw_uint16) * 2))
						return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
					
					field_id = IW_READ_LE16(pExtra_data);
					field_data_size = IW_READ_LE16(pExtra_data + sizeof(iw_uint16));
					
					if ((field_data_size + sizeof(iw_uint16) * 2) > extra_size_remaining)
						return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
					
					if (field_id == IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID)
					{
						const iw_uint8 *pField_data = pExtra_data + sizeof(iw_uint16) * 2;
						iw_uint32 field_data_remaining = field_data_size;
						
						if (pFound_zip64_extra_data)
							*pFound_zip64_extra_data = IW_TRUE;
						
						if (pStat->m_uncomp_size == IW_UINT32_MAX)
						{
							if (field_data_remaining < sizeof(iw_uint64))
								return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
							
							pStat->m_uncomp_size = IW_READ_LE64(pField_data);
							pField_data += sizeof(iw_uint64);
							field_data_remaining -= sizeof(iw_uint64);
						}
						
						if (pStat->m_comp_size == IW_UINT32_MAX)
						{
							if (field_data_remaining < sizeof(iw_uint64))
								return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
							
							pStat->m_comp_size = IW_READ_LE64(pField_data);
							pField_data += sizeof(iw_uint64);
							field_data_remaining -= sizeof(iw_uint64);
						}
						
						if (pStat->m_local_header_ofs == IW_UINT32_MAX)
						{
							if (field_data_remaining < sizeof(iw_uint64))
								return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
							
							pStat->m_local_header_ofs = IW_READ_LE64(pField_data);
							pField_data += sizeof(iw_uint64);
							field_data_remaining -= sizeof(iw_uint64);
						}
						
						break;
					}
					
					pExtra_data += sizeof(iw_uint16) * 2 + field_data_size;
					extra_size_remaining = extra_size_remaining - sizeof(iw_uint16) * 2 - field_data_size;
				} while (extra_size_remaining);
			}
		}
		
		return IW_TRUE;
	}
	
	static IW_FORCEINLINE iw_bool iw_zip_string_equal(const char *pA, const char *pB, iw_uint len, iw_uint flags)
	{
		iw_uint i;
		if (flags & IW_ZIP_FLAG_CASE_SENSITIVE)
			return 0 == memcmp(pA, pB, len);
		for (i = 0; i < len; ++i)
			if (IW_TOLOWER(pA[i]) != IW_TOLOWER(pB[i]))
				return IW_FALSE;
		return IW_TRUE;
	}
	
	static IW_FORCEINLINE int iw_zip_filename_compare(const iw_zip_array *pCentral_dir_array, const iw_zip_array *pCentral_dir_offsets, iw_uint l_index, const char *pR, iw_uint r_len)
	{
		const iw_uint8 *pL = &IW_ZIP_ARRAY_ELEMENT(pCentral_dir_array, iw_uint8, IW_ZIP_ARRAY_ELEMENT(pCentral_dir_offsets, iw_uint32, l_index)), *pE;
		iw_uint l_len = IW_READ_LE16(pL + IW_ZIP_CDH_FILENAME_LEN_OFS);
		iw_uint8 l = 0, r = 0;
		pL += IW_ZIP_CENTRAL_DIR_HEADER_SIZE;
		pE = pL + IW_MIN(l_len, r_len);
		while (pL < pE)
		{
			if ((l = IW_TOLOWER(*pL)) != (r = IW_TOLOWER(*pR)))
				break;
			pL++;
			pR++;
		}
		return (pL == pE) ? (int)(l_len - r_len) : (l - r);
	}
	
	static iw_bool iw_zip_locate_file_binary_search(iw_zip_archive *pZip, const char *pFilename, iw_uint32 *pIndex)
	{
		iw_zip_internal_state *pState = pZip->m_pState;
		const iw_zip_array *pCentral_dir_offsets = &pState->m_central_dir_offsets;
		const iw_zip_array *pCentral_dir = &pState->m_central_dir;
		iw_uint32 *pIndices = &IW_ZIP_ARRAY_ELEMENT(&pState->m_sorted_central_dir_offsets, iw_uint32, 0);
		const iw_uint32 size = pZip->m_total_files;
		const iw_uint filename_len = (iw_uint)strlen(pFilename);
		
		if (pIndex)
			*pIndex = 0;
		
		if (size)
		{
			iw_int64 l = 0, h = (iw_int64)size - 1;
			
			while (l <= h)
			{
				iw_int64 m = l + ((h - l) >> 1);
				iw_uint32 file_index = pIndices[(iw_uint32)m];
				
				int comp = iw_zip_filename_compare(pCentral_dir, pCentral_dir_offsets, file_index, pFilename, filename_len);
				if (!comp)
				{
					if (pIndex)
						*pIndex = file_index;
					return IW_TRUE;
				}
				else if (comp < 0)
					l = m + 1;
				else
					h = m - 1;
			}
		}
		
		return iw_zip_set_error(pZip, IW_ZIP_FILE_NOT_FOUND);
	}
	
	int iw_zip_reader_locate_file(iw_zip_archive *pZip, const char *pName, const char *pComment, iw_uint flags)
	{
		iw_uint32 index;
		if (!iw_zip_reader_locate_file_v2(pZip, pName, pComment, flags, &index))
			return -1;
		else
			return (int)index;
	}
	
	iw_bool iw_zip_reader_locate_file_v2(iw_zip_archive *pZip, const char *pName, const char *pComment, iw_uint flags, iw_uint32 *pIndex)
	{
		iw_uint file_index;
		size_t name_len, comment_len;
		
		if (pIndex)
			*pIndex = 0;
		
		if ((!pZip) || (!pZip->m_pState) || (!pName))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (((pZip->m_pState->m_init_flags & IW_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY) == 0) &&
			(pZip->m_zip_mode == IW_ZIP_MODE_READING) &&
			((flags & (IW_ZIP_FLAG_IGNORE_PATH | IW_ZIP_FLAG_CASE_SENSITIVE)) == 0) && (!pComment) && (pZip->m_pState->m_sorted_central_dir_offsets.m_size))
		{
			return iw_zip_locate_file_binary_search(pZip, pName, pIndex);
		}
		
		name_len = strlen(pName);
		if (name_len > IW_UINT16_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		comment_len = pComment ? strlen(pComment) : 0;
		if (comment_len > IW_UINT16_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		for (file_index = 0; file_index < pZip->m_total_files; file_index++)
		{
			const iw_uint8 *pHeader = &IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir, iw_uint8, IW_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir_offsets, iw_uint32, file_index));
			iw_uint filename_len = IW_READ_LE16(pHeader + IW_ZIP_CDH_FILENAME_LEN_OFS);
			const char *pFilename = (const char *)pHeader + IW_ZIP_CENTRAL_DIR_HEADER_SIZE;
			if (filename_len < name_len)
				continue;
			if (comment_len)
			{
				iw_uint file_extra_len = IW_READ_LE16(pHeader + IW_ZIP_CDH_EXTRA_LEN_OFS), file_comment_len = IW_READ_LE16(pHeader + IW_ZIP_CDH_COMMENT_LEN_OFS);
				const char *pFile_comment = pFilename + filename_len + file_extra_len;
				if ((file_comment_len != comment_len) || (!iw_zip_string_equal(pComment, pFile_comment, file_comment_len, flags)))
					continue;
			}
			if ((flags & IW_ZIP_FLAG_IGNORE_PATH) && (filename_len))
			{
				int ofs = filename_len - 1;
				do
				{
					if ((pFilename[ofs] == '/') || (pFilename[ofs] == '\\') || (pFilename[ofs] == ':'))
						break;
				} while (--ofs >= 0);
				ofs++;
				pFilename += ofs;
				filename_len -= ofs;
			}
			if ((filename_len == name_len) && (iw_zip_string_equal(pName, pFilename, filename_len, flags)))
			{
				if (pIndex)
					*pIndex = file_index;
				return IW_TRUE;
			}
		}
		
		return iw_zip_set_error(pZip, IW_ZIP_FILE_NOT_FOUND);
	}
	
	static iw_bool iw_zip_reader_extract_to_mem_no_alloc1(iw_zip_archive *pZip, iw_uint file_index, void *pBuf, size_t buf_size, iw_uint flags, void *pUser_read_buf, size_t user_read_buf_size, const iw_zip_archive_file_stat *st)
	{
		int status = iwc_u_STATUS_DONE;
		iw_uint64 needed_size, cur_file_ofs, comp_remaining, out_buf_ofs = 0, read_buf_size, read_buf_ofs = 0, read_buf_avail;
		iw_zip_archive_file_stat file_stat;
		void *pRead_buf;
		iw_uint32 local_header_u32[(IW_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pLocal_header = (iw_uint8 *)local_header_u32;
		iwc_u_decompressor inflator;
		
		if ((!pZip) || (!pZip->m_pState) || ((buf_size) && (!pBuf)) || ((user_read_buf_size) && (!pUser_read_buf)) || (!pZip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (st)
		{
			file_stat = *st;
		}
		else if (!iw_zip_reader_file_stat(pZip, file_index, &file_stat))
			return IW_FALSE;
		
		if ((file_stat.m_is_directory) || (!file_stat.m_comp_size))
			return IW_TRUE;
		
		if (file_stat.m_bit_flag & (IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_IS_ENCRYPTED | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_USES_STRONG_ENCRYPTION | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_COMPRESSED_PATCH_FLAG))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_ENCRYPTION);
		
		if ((!(flags & IW_ZIP_FLAG_COMPRESSED_DATA)) && (file_stat.m_method != 0) && (file_stat.m_method != IW_DEFLATED))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_METHOD);
		
		needed_size = (flags & IW_ZIP_FLAG_COMPRESSED_DATA) ? file_stat.m_comp_size : file_stat.m_uncomp_size;
		if (buf_size < needed_size)
			return iw_zip_set_error(pZip, IW_ZIP_BUF_TOO_SMALL);
		
		cur_file_ofs = file_stat.m_local_header_ofs;
		if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pLocal_header, IW_ZIP_LOCAL_DIR_HEADER_SIZE) != IW_ZIP_LOCAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
		
		if (IW_READ_LE32(pLocal_header) != IW_ZIP_LOCAL_DIR_HEADER_SIG)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		cur_file_ofs += (iw_uint64)(IW_ZIP_LOCAL_DIR_HEADER_SIZE) + IW_READ_LE16(pLocal_header + IW_ZIP_LDH_FILENAME_LEN_OFS) + IW_READ_LE16(pLocal_header + IW_ZIP_LDH_EXTRA_LEN_OFS);
		if ((cur_file_ofs + file_stat.m_comp_size) > pZip->m_archive_size)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		if ((flags & IW_ZIP_FLAG_COMPRESSED_DATA) || (!file_stat.m_method))
		{
			if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pBuf, (size_t)needed_size) != needed_size)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
			
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
			if ((flags & IW_ZIP_FLAG_COMPRESSED_DATA) == 0)
			{
				if (iw_crc32(IW_CRC32_INIT, (const iw_uint8 *)pBuf, (size_t)file_stat.m_uncomp_size) != file_stat.m_crc32)
					return iw_zip_set_error(pZip, IW_ZIP_CRC_CHECK_FAILED);
			}
#endif
			
			return IW_TRUE;
		}
		
		iwc_u_init(&inflator);
		
		if (pZip->m_pState->m_pMem)
		{
			pRead_buf = (iw_uint8 *)pZip->m_pState->m_pMem + cur_file_ofs;
			read_buf_size = read_buf_avail = file_stat.m_comp_size;
			comp_remaining = 0;
		}
		else if (pUser_read_buf)
		{
			if (!user_read_buf_size)
				return IW_FALSE;
			pRead_buf = (iw_uint8 *)pUser_read_buf;
			read_buf_size = user_read_buf_size;
			read_buf_avail = 0;
			comp_remaining = file_stat.m_comp_size;
		}
		else
		{
			read_buf_size = IW_MIN(file_stat.m_comp_size, (iw_uint64)IW_ZIP_MAX_IO_BUF_SIZE);
			if (((sizeof(size_t) == sizeof(iw_uint32))) && (read_buf_size > 0x7FFFFFFF))
				return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			
			if (NULL == (pRead_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, (size_t)read_buf_size)))
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			
			read_buf_avail = 0;
			comp_remaining = file_stat.m_comp_size;
		}
		
		do
		{
			size_t in_buf_size, out_buf_size = (size_t)(file_stat.m_uncomp_size - out_buf_ofs);
			if ((!read_buf_avail) && (!pZip->m_pState->m_pMem))
			{
				read_buf_avail = IW_MIN(read_buf_size, comp_remaining);
				if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pRead_buf, (size_t)read_buf_avail) != read_buf_avail)
				{
					status = iwc_u_STATUS_FAILED;
					iw_zip_set_error(pZip, IW_ZIP_DECOMPRESSION_FAILED);
					break;
				}
				cur_file_ofs += read_buf_avail;
				comp_remaining -= read_buf_avail;
				read_buf_ofs = 0;
			}
			in_buf_size = (size_t)read_buf_avail;
			status = iwc_u_decompress(&inflator, (iw_uint8 *)pRead_buf + read_buf_ofs, &in_buf_size, (iw_uint8 *)pBuf, (iw_uint8 *)pBuf + out_buf_ofs, &out_buf_size, iwc_u_FLAG_USING_NON_WRAPPING_OUTPUT_BUF | (comp_remaining ? iwc_u_FLAG_HAS_MORE_INPUT : 0));
			read_buf_avail -= in_buf_size;
			read_buf_ofs += in_buf_size;
			out_buf_ofs += out_buf_size;
		} while (status == iwc_u_STATUS_NEEDS_MORE_INPUT);
		
		if (status == iwc_u_STATUS_DONE)
		{
			if (out_buf_ofs != file_stat.m_uncomp_size)
			{
				iw_zip_set_error(pZip, IW_ZIP_UNEXPECTED_DECOMPRESSED_SIZE);
				status = iwc_u_STATUS_FAILED;
			}
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
			else if (iw_crc32(IW_CRC32_INIT, (const iw_uint8 *)pBuf, (size_t)file_stat.m_uncomp_size) != file_stat.m_crc32)
			{
				iw_zip_set_error(pZip, IW_ZIP_CRC_CHECK_FAILED);
				status = iwc_u_STATUS_FAILED;
			}
#endif
		}
		
		if ((!pZip->m_pState->m_pMem) && (!pUser_read_buf))
			pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
		
		return status == iwc_u_STATUS_DONE;
	}
	
	iw_bool iw_zip_reader_extract_to_mem_no_alloc(iw_zip_archive *pZip, iw_uint file_index, void *pBuf, size_t buf_size, iw_uint flags, void *pUser_read_buf, size_t user_read_buf_size)
	{
		return iw_zip_reader_extract_to_mem_no_alloc1(pZip, file_index, pBuf, buf_size, flags, pUser_read_buf, user_read_buf_size, NULL);
	}
	
	iw_bool iw_zip_reader_extract_file_to_mem_no_alloc(iw_zip_archive *pZip, const char *pFilename, void *pBuf, size_t buf_size, iw_uint flags, void *pUser_read_buf, size_t user_read_buf_size)
	{
		iw_uint32 file_index;
		if (!iw_zip_reader_locate_file_v2(pZip, pFilename, NULL, flags, &file_index))
			return IW_FALSE;
		return iw_zip_reader_extract_to_mem_no_alloc1(pZip, file_index, pBuf, buf_size, flags, pUser_read_buf, user_read_buf_size, NULL);
	}
	
	iw_bool iw_zip_reader_extract_to_mem(iw_zip_archive *pZip, iw_uint file_index, void *pBuf, size_t buf_size, iw_uint flags)
	{
		return iw_zip_reader_extract_to_mem_no_alloc1(pZip, file_index, pBuf, buf_size, flags, NULL, 0, NULL);
	}
	
	iw_bool iw_zip_reader_extract_file_to_mem(iw_zip_archive *pZip, const char *pFilename, void *pBuf, size_t buf_size, iw_uint flags)
	{
		return iw_zip_reader_extract_file_to_mem_no_alloc(pZip, pFilename, pBuf, buf_size, flags, NULL, 0);
	}
	
	void *iw_zip_reader_extract_to_heap(iw_zip_archive *pZip, iw_uint file_index, size_t *pSize, iw_uint flags)
	{
		iw_zip_archive_file_stat file_stat;
		iw_uint64 alloc_size;
		void *pBuf;
		
		if (pSize)
			*pSize = 0;
		
		if (!iw_zip_reader_file_stat(pZip, file_index, &file_stat))
			return NULL;
		
		alloc_size = (flags & IW_ZIP_FLAG_COMPRESSED_DATA) ? file_stat.m_comp_size : file_stat.m_uncomp_size;
		if (((sizeof(size_t) == sizeof(iw_uint32))) && (alloc_size > 0x7FFFFFFF))
		{
			iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			return NULL;
		}
		
		if (NULL == (pBuf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, (size_t)alloc_size)))
		{
			iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			return NULL;
		}
		
		if (!iw_zip_reader_extract_to_mem_no_alloc1(pZip, file_index, pBuf, (size_t)alloc_size, flags, NULL, 0, &file_stat))
		{
			pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
			return NULL;
		}
		
		if (pSize)
			*pSize = (size_t)alloc_size;
		return pBuf;
	}
	
	void *iw_zip_reader_extract_file_to_heap(iw_zip_archive *pZip, const char *pFilename, size_t *pSize, iw_uint flags)
	{
		iw_uint32 file_index;
		if (!iw_zip_reader_locate_file_v2(pZip, pFilename, NULL, flags, &file_index))
		{
			if (pSize)
				*pSize = 0;
			return IW_FALSE;
		}
		return iw_zip_reader_extract_to_heap(pZip, file_index, pSize, flags);
	}
	
	iw_bool iw_zip_reader_extract_to_callback(iw_zip_archive *pZip, iw_uint file_index, iw_file_write_func pCallback, void *pOpaque, iw_uint flags)
	{
		int status = iwc_u_STATUS_DONE;
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
		iw_uint file_crc32 = IW_CRC32_INIT;
#endif
		iw_uint64 read_buf_size, read_buf_ofs = 0, read_buf_avail, comp_remaining, out_buf_ofs = 0, cur_file_ofs;
		iw_zip_archive_file_stat file_stat;
		void *pRead_buf = NULL;
		void *pWrite_buf = NULL;
		iw_uint32 local_header_u32[(IW_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pLocal_header = (iw_uint8 *)local_header_u32;
		
		if ((!pZip) || (!pZip->m_pState) || (!pCallback) || (!pZip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (!iw_zip_reader_file_stat(pZip, file_index, &file_stat))
			return IW_FALSE;
		
		if ((file_stat.m_is_directory) || (!file_stat.m_comp_size))
			return IW_TRUE;
		
		if (file_stat.m_bit_flag & (IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_IS_ENCRYPTED | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_USES_STRONG_ENCRYPTION | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_COMPRESSED_PATCH_FLAG))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_ENCRYPTION);
		
		if ((!(flags & IW_ZIP_FLAG_COMPRESSED_DATA)) && (file_stat.m_method != 0) && (file_stat.m_method != IW_DEFLATED))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_METHOD);
		
		cur_file_ofs = file_stat.m_local_header_ofs;
		if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pLocal_header, IW_ZIP_LOCAL_DIR_HEADER_SIZE) != IW_ZIP_LOCAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
		
		if (IW_READ_LE32(pLocal_header) != IW_ZIP_LOCAL_DIR_HEADER_SIG)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		cur_file_ofs += (iw_uint64)(IW_ZIP_LOCAL_DIR_HEADER_SIZE) + IW_READ_LE16(pLocal_header + IW_ZIP_LDH_FILENAME_LEN_OFS) + IW_READ_LE16(pLocal_header + IW_ZIP_LDH_EXTRA_LEN_OFS);
		if ((cur_file_ofs + file_stat.m_comp_size) > pZip->m_archive_size)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		if (pZip->m_pState->m_pMem)
		{
			pRead_buf = (iw_uint8 *)pZip->m_pState->m_pMem + cur_file_ofs;
			read_buf_size = read_buf_avail = file_stat.m_comp_size;
			comp_remaining = 0;
		}
		else
		{
			read_buf_size = IW_MIN(file_stat.m_comp_size, (iw_uint64)IW_ZIP_MAX_IO_BUF_SIZE);
			if (NULL == (pRead_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, (size_t)read_buf_size)))
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			
			read_buf_avail = 0;
			comp_remaining = file_stat.m_comp_size;
		}
		
		if ((flags & IW_ZIP_FLAG_COMPRESSED_DATA) || (!file_stat.m_method))
		{
			if (pZip->m_pState->m_pMem)
			{
				if (((sizeof(size_t) == sizeof(iw_uint32))) && (file_stat.m_comp_size > IW_UINT32_MAX))
					return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
				
				if (pCallback(pOpaque, out_buf_ofs, pRead_buf, (size_t)file_stat.m_comp_size) != file_stat.m_comp_size)
				{
					iw_zip_set_error(pZip, IW_ZIP_WRITE_CALLBACK_FAILED);
					status = iwc_u_STATUS_FAILED;
				}
				else if (!(flags & IW_ZIP_FLAG_COMPRESSED_DATA))
				{
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
					file_crc32 = (iw_uint32)iw_crc32(file_crc32, (const iw_uint8 *)pRead_buf, (size_t)file_stat.m_comp_size);
#endif
				}
				
				cur_file_ofs += file_stat.m_comp_size;
				out_buf_ofs += file_stat.m_comp_size;
				comp_remaining = 0;
			}
			else
			{
				while (comp_remaining)
				{
					read_buf_avail = IW_MIN(read_buf_size, comp_remaining);
					if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pRead_buf, (size_t)read_buf_avail) != read_buf_avail)
					{
						iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
						status = iwc_u_STATUS_FAILED;
						break;
					}
					
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
					if (!(flags & IW_ZIP_FLAG_COMPRESSED_DATA))
					{
						file_crc32 = (iw_uint32)iw_crc32(file_crc32, (const iw_uint8 *)pRead_buf, (size_t)read_buf_avail);
					}
#endif
					
					if (pCallback(pOpaque, out_buf_ofs, pRead_buf, (size_t)read_buf_avail) != read_buf_avail)
					{
						iw_zip_set_error(pZip, IW_ZIP_WRITE_CALLBACK_FAILED);
						status = iwc_u_STATUS_FAILED;
						break;
					}
					
					cur_file_ofs += read_buf_avail;
					out_buf_ofs += read_buf_avail;
					comp_remaining -= read_buf_avail;
				}
			}
		}
		else
		{
			iwc_u_decompressor inflator;
			iwc_u_init(&inflator);
			
			if (NULL == (pWrite_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, iwc_u_LZ_DICT_SIZE)))
			{
				iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
				status = iwc_u_STATUS_FAILED;
			}
			else
			{
				do
				{
					iw_uint8 *pWrite_buf_cur = (iw_uint8 *)pWrite_buf + (out_buf_ofs & (iwc_u_LZ_DICT_SIZE - 1));
					size_t in_buf_size, out_buf_size = iwc_u_LZ_DICT_SIZE - (out_buf_ofs & (iwc_u_LZ_DICT_SIZE - 1));
					if ((!read_buf_avail) && (!pZip->m_pState->m_pMem))
					{
						read_buf_avail = IW_MIN(read_buf_size, comp_remaining);
						if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pRead_buf, (size_t)read_buf_avail) != read_buf_avail)
						{
							iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
							status = iwc_u_STATUS_FAILED;
							break;
						}
						cur_file_ofs += read_buf_avail;
						comp_remaining -= read_buf_avail;
						read_buf_ofs = 0;
					}
					
					in_buf_size = (size_t)read_buf_avail;
					status = iwc_u_decompress(&inflator, (const iw_uint8 *)pRead_buf + read_buf_ofs, &in_buf_size, (iw_uint8 *)pWrite_buf, pWrite_buf_cur, &out_buf_size, comp_remaining ? iwc_u_FLAG_HAS_MORE_INPUT : 0);
					read_buf_avail -= in_buf_size;
					read_buf_ofs += in_buf_size;
					
					if (out_buf_size)
					{
						if (pCallback(pOpaque, out_buf_ofs, pWrite_buf_cur, out_buf_size) != out_buf_size)
						{
							iw_zip_set_error(pZip, IW_ZIP_WRITE_CALLBACK_FAILED);
							status = iwc_u_STATUS_FAILED;
							break;
						}
						
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
						file_crc32 = (iw_uint32)iw_crc32(file_crc32, pWrite_buf_cur, out_buf_size);
#endif
						if ((out_buf_ofs += out_buf_size) > file_stat.m_uncomp_size)
						{
							iw_zip_set_error(pZip, IW_ZIP_DECOMPRESSION_FAILED);
							status = iwc_u_STATUS_FAILED;
							break;
						}
					}
				} while ((status == iwc_u_STATUS_NEEDS_MORE_INPUT) || (status == iwc_u_STATUS_HAS_MORE_OUTPUT));
			}
		}
		
		if ((status == iwc_u_STATUS_DONE) && (!(flags & IW_ZIP_FLAG_COMPRESSED_DATA)))
		{
			if (out_buf_ofs != file_stat.m_uncomp_size)
			{
				iw_zip_set_error(pZip, IW_ZIP_UNEXPECTED_DECOMPRESSED_SIZE);
				status = iwc_u_STATUS_FAILED;
			}
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
			else if (file_crc32 != file_stat.m_crc32)
			{
				iw_zip_set_error(pZip, IW_ZIP_DECOMPRESSION_FAILED);
				status = iwc_u_STATUS_FAILED;
			}
#endif
		}
		
		if (!pZip->m_pState->m_pMem)
			pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
		
		if (pWrite_buf)
			pZip->m_pFree(pZip->m_pAlloc_opaque, pWrite_buf);
		
		return status == iwc_u_STATUS_DONE;
	}
	
	iw_bool iw_zip_reader_extract_file_to_callback(iw_zip_archive *pZip, const char *pFilename, iw_file_write_func pCallback, void *pOpaque, iw_uint flags)
	{
		iw_uint32 file_index;
		if (!iw_zip_reader_locate_file_v2(pZip, pFilename, NULL, flags, &file_index))
			return IW_FALSE;
		
		return iw_zip_reader_extract_to_callback(pZip, file_index, pCallback, pOpaque, flags);
	}
	
	iw_zip_reader_extract_iter_state *iw_zip_reader_extract_iter_new(iw_zip_archive *pZip, iw_uint file_index, iw_uint flags)
	{
		iw_zip_reader_extract_iter_state *pState;
		iw_uint32 local_header_u32[(IW_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pLocal_header = (iw_uint8 *)local_header_u32;
		
		if ((!pZip) || (!pZip->m_pState))
			return NULL;
		
		pState = (iw_zip_reader_extract_iter_state *)pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, sizeof(iw_zip_reader_extract_iter_state));
		if (!pState)
		{
			iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			return NULL;
		}
		
		if (!iw_zip_reader_file_stat(pZip, file_index, &pState->file_stat))
		{
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
			return NULL;
		}
		
		if (pState->file_stat.m_bit_flag & (IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_IS_ENCRYPTED | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_USES_STRONG_ENCRYPTION | IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_COMPRESSED_PATCH_FLAG))
		{
			iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_ENCRYPTION);
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
			return NULL;
		}
		
		if ((!(flags & IW_ZIP_FLAG_COMPRESSED_DATA)) && (pState->file_stat.m_method != 0) && (pState->file_stat.m_method != IW_DEFLATED))
		{
			iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_METHOD);
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
			return NULL;
		}
		
		pState->pZip = pZip;
		pState->flags = flags;
		
		pState->status = iwc_u_STATUS_DONE;
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
		pState->file_crc32 = IW_CRC32_INIT;
#endif
		pState->read_buf_ofs = 0;
		pState->out_buf_ofs = 0;
		pState->pRead_buf = NULL;
		pState->pWrite_buf = NULL;
		pState->out_blk_remain = 0;
		
		pState->cur_file_ofs = pState->file_stat.m_local_header_ofs;
		if (pZip->m_pRead(pZip->m_pIO_opaque, pState->cur_file_ofs, pLocal_header, IW_ZIP_LOCAL_DIR_HEADER_SIZE) != IW_ZIP_LOCAL_DIR_HEADER_SIZE)
		{
			iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
			return NULL;
		}
		
		if (IW_READ_LE32(pLocal_header) != IW_ZIP_LOCAL_DIR_HEADER_SIG)
		{
			iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
			return NULL;
		}
		
		pState->cur_file_ofs += (iw_uint64)(IW_ZIP_LOCAL_DIR_HEADER_SIZE) + IW_READ_LE16(pLocal_header + IW_ZIP_LDH_FILENAME_LEN_OFS) + IW_READ_LE16(pLocal_header + IW_ZIP_LDH_EXTRA_LEN_OFS);
		if ((pState->cur_file_ofs + pState->file_stat.m_comp_size) > pZip->m_archive_size)
		{
			iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
			return NULL;
		}
		
		if (pZip->m_pState->m_pMem)
		{
			pState->pRead_buf = (iw_uint8 *)pZip->m_pState->m_pMem + pState->cur_file_ofs;
			pState->read_buf_size = pState->read_buf_avail = pState->file_stat.m_comp_size;
			pState->comp_remaining = pState->file_stat.m_comp_size;
		}
		else
		{
			if (!((flags & IW_ZIP_FLAG_COMPRESSED_DATA) || (!pState->file_stat.m_method)))
			{
				pState->read_buf_size = IW_MIN(pState->file_stat.m_comp_size, (iw_uint64)IW_ZIP_MAX_IO_BUF_SIZE);
				if (NULL == (pState->pRead_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, (size_t)pState->read_buf_size)))
				{
					iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
					pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
					return NULL;
				}
			}
			else
			{
				pState->read_buf_size = 0;
			}
			pState->read_buf_avail = 0;
			pState->comp_remaining = pState->file_stat.m_comp_size;
		}
		
		if (!((flags & IW_ZIP_FLAG_COMPRESSED_DATA) || (!pState->file_stat.m_method)))
		{
			iwc_u_init(&pState->inflator);
			
			if (NULL == (pState->pWrite_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, iwc_u_LZ_DICT_SIZE)))
			{
				iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
				if (pState->pRead_buf)
					pZip->m_pFree(pZip->m_pAlloc_opaque, pState->pRead_buf);
				pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
				return NULL;
			}
		}
		
		return pState;
	}
	
	iw_zip_reader_extract_iter_state *iw_zip_reader_extract_file_iter_new(iw_zip_archive *pZip, const char *pFilename, iw_uint flags)
	{
		iw_uint32 file_index;
		
		if (!iw_zip_reader_locate_file_v2(pZip, pFilename, NULL, flags, &file_index))
			return NULL;
		
		return iw_zip_reader_extract_iter_new(pZip, file_index, flags);
	}
	
	size_t iw_zip_reader_extract_iter_read(iw_zip_reader_extract_iter_state *pState, void *pvBuf, size_t buf_size)
	{
		size_t copied_to_caller = 0;
		
		if ((!pState) || (!pState->pZip) || (!pState->pZip->m_pState) || (!pvBuf))
			return 0;
		
		if ((pState->flags & IW_ZIP_FLAG_COMPRESSED_DATA) || (!pState->file_stat.m_method))
		{
			copied_to_caller = (size_t)IW_MIN(buf_size, pState->comp_remaining);
			
			if (pState->pZip->m_pState->m_pMem)
			{
				memcpy(pvBuf, pState->pRead_buf, copied_to_caller);
				pState->pRead_buf = ((iw_uint8 *)pState->pRead_buf) + copied_to_caller;
			}
			else
			{
				if (pState->pZip->m_pRead(pState->pZip->m_pIO_opaque, pState->cur_file_ofs, pvBuf, copied_to_caller) != copied_to_caller)
				{
					iw_zip_set_error(pState->pZip, IW_ZIP_FILE_READ_FAILED);
					pState->status = iwc_u_STATUS_FAILED;
					copied_to_caller = 0;
				}
			}
			
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
			if (!(pState->flags & IW_ZIP_FLAG_COMPRESSED_DATA))
				pState->file_crc32 = (iw_uint32)iw_crc32(pState->file_crc32, (const iw_uint8 *)pvBuf, copied_to_caller);
#endif
			
			pState->cur_file_ofs += copied_to_caller;
			pState->out_buf_ofs += copied_to_caller;
			pState->comp_remaining -= copied_to_caller;
		}
		else
		{
			do
			{
				iw_uint8 *pWrite_buf_cur = (iw_uint8 *)pState->pWrite_buf + (pState->out_buf_ofs & (iwc_u_LZ_DICT_SIZE - 1));
				
				size_t in_buf_size, out_buf_size = iwc_u_LZ_DICT_SIZE - (pState->out_buf_ofs & (iwc_u_LZ_DICT_SIZE - 1));
				
				if (!pState->out_blk_remain)
				{
					if ((!pState->read_buf_avail) && (!pState->pZip->m_pState->m_pMem))
					{
						pState->read_buf_avail = IW_MIN(pState->read_buf_size, pState->comp_remaining);
						if (pState->pZip->m_pRead(pState->pZip->m_pIO_opaque, pState->cur_file_ofs, pState->pRead_buf, (size_t)pState->read_buf_avail) != pState->read_buf_avail)
						{
							iw_zip_set_error(pState->pZip, IW_ZIP_FILE_READ_FAILED);
							pState->status = iwc_u_STATUS_FAILED;
							break;
						}
						
						pState->cur_file_ofs += pState->read_buf_avail;
						pState->comp_remaining -= pState->read_buf_avail;
						pState->read_buf_ofs = 0;
					}
					
					in_buf_size = (size_t)pState->read_buf_avail;
					pState->status = iwc_u_decompress(&pState->inflator, (const iw_uint8 *)pState->pRead_buf + pState->read_buf_ofs, &in_buf_size, (iw_uint8 *)pState->pWrite_buf, pWrite_buf_cur, &out_buf_size, pState->comp_remaining ? iwc_u_FLAG_HAS_MORE_INPUT : 0);
					pState->read_buf_avail -= in_buf_size;
					pState->read_buf_ofs += in_buf_size;
					
					pState->out_blk_remain = out_buf_size;
				}
				
				if (pState->out_blk_remain)
				{
					size_t to_copy = IW_MIN((buf_size - copied_to_caller), pState->out_blk_remain);
					
					memcpy((iw_uint8 *)pvBuf + copied_to_caller, pWrite_buf_cur, to_copy);
					
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
					pState->file_crc32 = (iw_uint32)iw_crc32(pState->file_crc32, pWrite_buf_cur, to_copy);
#endif
					
					pState->out_blk_remain -= to_copy;
					
					if ((pState->out_buf_ofs += to_copy) > pState->file_stat.m_uncomp_size)
					{
						iw_zip_set_error(pState->pZip, IW_ZIP_DECOMPRESSION_FAILED);
						pState->status = iwc_u_STATUS_FAILED;
						break;
					}
					
					copied_to_caller += to_copy;
				}
			} while ((copied_to_caller < buf_size) && ((pState->status == iwc_u_STATUS_NEEDS_MORE_INPUT) || (pState->status == iwc_u_STATUS_HAS_MORE_OUTPUT)));
		}
		
		return copied_to_caller;
	}
	
	iw_bool iw_zip_reader_extract_iter_free(iw_zip_reader_extract_iter_state *pState)
	{
		int status;
		
		if ((!pState) || (!pState->pZip) || (!pState->pZip->m_pState))
			return IW_FALSE;
		
		if ((pState->status == iwc_u_STATUS_DONE) && (!(pState->flags & IW_ZIP_FLAG_COMPRESSED_DATA)))
		{
			if (pState->out_buf_ofs != pState->file_stat.m_uncomp_size)
			{
				iw_zip_set_error(pState->pZip, IW_ZIP_UNEXPECTED_DECOMPRESSED_SIZE);
				pState->status = iwc_u_STATUS_FAILED;
			}
#ifndef i2WC_DISABLE_ZIP_READER_CRC32_CHECKS
			else if (pState->file_crc32 != pState->file_stat.m_crc32)
			{
				iw_zip_set_error(pState->pZip, IW_ZIP_DECOMPRESSION_FAILED);
				pState->status = iwc_u_STATUS_FAILED;
			}
#endif
		}
		
		if (!pState->pZip->m_pState->m_pMem)
			pState->pZip->m_pFree(pState->pZip->m_pAlloc_opaque, pState->pRead_buf);
		if (pState->pWrite_buf)
			pState->pZip->m_pFree(pState->pZip->m_pAlloc_opaque, pState->pWrite_buf);
		
		status = pState->status;
		
		pState->pZip->m_pFree(pState->pZip->m_pAlloc_opaque, pState);
		
		return status == iwc_u_STATUS_DONE;
	}
	
#ifndef i2WC_NO_STDIO
	static size_t iw_zip_file_write_callback(void *pOpaque, iw_uint64 ofs, const void *pBuf, size_t n)
	{
		(void)ofs;
		
		return IW_FWRITE(pBuf, 1, n, (IW_FILE *)pOpaque);
	}
	
	iw_bool iw_zip_reader_extract_to_file(iw_zip_archive *pZip, iw_uint file_index, const char *pDst_filename, iw_uint flags)
	{
		iw_bool status;
		iw_zip_archive_file_stat file_stat;
		IW_FILE *pFile;
		
		if (!iw_zip_reader_file_stat(pZip, file_index, &file_stat))
			return IW_FALSE;
		
		if ((file_stat.m_is_directory) || (!file_stat.m_is_supported))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_FEATURE);
		
		pFile = IW_FOPEN(pDst_filename, "wb");
		if (!pFile)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_OPEN_FAILED);
		
		status = iw_zip_reader_extract_to_callback(pZip, file_index, iw_zip_file_write_callback, pFile, flags);
		
		if (IW_FCLOSE(pFile) == EOF)
		{
			if (status)
				iw_zip_set_error(pZip, IW_ZIP_FILE_CLOSE_FAILED);
			
			status = IW_FALSE;
		}
		
#if !defined(i2WC_NO_TIME) && !defined(i2WC_NO_STDIO)
		if (status)
			iw_zip_set_file_times(pDst_filename, file_stat.m_time, file_stat.m_time);
#endif
		
		return status;
	}
	
	iw_bool iw_zip_reader_extract_file_to_file(iw_zip_archive *pZip, const char *pArchive_filename, const char *pDst_filename, iw_uint flags)
	{
		iw_uint32 file_index;
		if (!iw_zip_reader_locate_file_v2(pZip, pArchive_filename, NULL, flags, &file_index))
			return IW_FALSE;
		
		return iw_zip_reader_extract_to_file(pZip, file_index, pDst_filename, flags);
	}
	
	iw_bool iw_zip_reader_extract_to_cfile(iw_zip_archive *pZip, iw_uint file_index, IW_FILE *pFile, iw_uint flags)
	{
		iw_zip_archive_file_stat file_stat;
		
		if (!iw_zip_reader_file_stat(pZip, file_index, &file_stat))
			return IW_FALSE;
		
		if ((file_stat.m_is_directory) || (!file_stat.m_is_supported))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_FEATURE);
		
		return iw_zip_reader_extract_to_callback(pZip, file_index, iw_zip_file_write_callback, pFile, flags);
	}
	
	iw_bool iw_zip_reader_extract_file_to_cfile(iw_zip_archive *pZip, const char *pArchive_filename, IW_FILE *pFile, iw_uint flags)
	{
		iw_uint32 file_index;
		if (!iw_zip_reader_locate_file_v2(pZip, pArchive_filename, NULL, flags, &file_index))
			return IW_FALSE;
		
		return iw_zip_reader_extract_to_cfile(pZip, file_index, pFile, flags);
	}
#endif
	
	static size_t iw_zip_compute_crc32_callback(void *pOpaque, iw_uint64 file_ofs, const void *pBuf, size_t n)
	{
		iw_uint32 *p = (iw_uint32 *)pOpaque;
		(void)file_ofs;
		*p = (iw_uint32)iw_crc32(*p, (const iw_uint8 *)pBuf, n);
		return n;
	}
	
	iw_bool iw_zip_validate_file(iw_zip_archive *pZip, iw_uint file_index, iw_uint flags)
	{
		iw_zip_archive_file_stat file_stat;
		iw_zip_internal_state *pState;
		const iw_uint8 *pCentral_dir_header;
		iw_bool found_zip64_ext_data_in_cdir = IW_FALSE;
		iw_bool found_zip64_ext_data_in_ldir = IW_FALSE;
		iw_uint32 local_header_u32[(IW_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pLocal_header = (iw_uint8 *)local_header_u32;
		iw_uint64 local_header_ofs = 0;
		iw_uint32 local_header_filename_len, local_header_extra_len, local_header_crc32;
		iw_uint64 local_header_comp_size, local_header_uncomp_size;
		iw_uint32 uncomp_crc32 = IW_CRC32_INIT;
		iw_bool has_data_descriptor;
		iw_uint32 local_header_bit_flags;
		
		iw_zip_array file_data_array;
		iw_zip_array_init(&file_data_array, 1);
		
		if ((!pZip) || (!pZip->m_pState) || (!pZip->m_pAlloc) || (!pZip->m_pFree) || (!pZip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (file_index > pZip->m_total_files)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pState = pZip->m_pState;
		
		pCentral_dir_header = iw_zip_get_cdh(pZip, file_index);
		
		if (!iw_zip_file_stat_internal(pZip, file_index, pCentral_dir_header, &file_stat, &found_zip64_ext_data_in_cdir))
			return IW_FALSE;
		
		if ((file_stat.m_is_directory) || (!file_stat.m_uncomp_size))
			return IW_TRUE;
		
		if (file_stat.m_is_encrypted)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_ENCRYPTION);
		
		if ((file_stat.m_method != 0) && (file_stat.m_method != IW_DEFLATED))
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_METHOD);
		
		if (!file_stat.m_is_supported)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_FEATURE);
		
		local_header_ofs = file_stat.m_local_header_ofs;
		if (pZip->m_pRead(pZip->m_pIO_opaque, local_header_ofs, pLocal_header, IW_ZIP_LOCAL_DIR_HEADER_SIZE) != IW_ZIP_LOCAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
		
		if (IW_READ_LE32(pLocal_header) != IW_ZIP_LOCAL_DIR_HEADER_SIG)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		local_header_filename_len = IW_READ_LE16(pLocal_header + IW_ZIP_LDH_FILENAME_LEN_OFS);
		local_header_extra_len = IW_READ_LE16(pLocal_header + IW_ZIP_LDH_EXTRA_LEN_OFS);
		local_header_comp_size = IW_READ_LE32(pLocal_header + IW_ZIP_LDH_COMPRESSED_SIZE_OFS);
		local_header_uncomp_size = IW_READ_LE32(pLocal_header + IW_ZIP_LDH_DECOMPRESSED_SIZE_OFS);
		local_header_crc32 = IW_READ_LE32(pLocal_header + IW_ZIP_LDH_CRC32_OFS);
		local_header_bit_flags = IW_READ_LE16(pLocal_header + IW_ZIP_LDH_BIT_FLAG_OFS);
		has_data_descriptor = (local_header_bit_flags & 8) != 0;
		
		if (local_header_filename_len != strlen(file_stat.m_filename))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		if ((local_header_ofs + IW_ZIP_LOCAL_DIR_HEADER_SIZE + local_header_filename_len + local_header_extra_len + file_stat.m_comp_size) > pZip->m_archive_size)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		if (!iw_zip_array_resize(pZip, &file_data_array, IW_MAX(local_header_filename_len, local_header_extra_len), IW_FALSE))
		{
			iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			goto handle_failure;
		}
		
		if (local_header_filename_len)
		{
			if (pZip->m_pRead(pZip->m_pIO_opaque, local_header_ofs + IW_ZIP_LOCAL_DIR_HEADER_SIZE, file_data_array.m_p, local_header_filename_len) != local_header_filename_len)
			{
				iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
				goto handle_failure;
			}
			
			if (memcmp(file_stat.m_filename, file_data_array.m_p, local_header_filename_len) != 0)
			{
				iw_zip_set_error(pZip, IW_ZIP_VALIDATION_FAILED);
				goto handle_failure;
			}
		}
		
		if ((local_header_extra_len) && ((local_header_comp_size == IW_UINT32_MAX) || (local_header_uncomp_size == IW_UINT32_MAX)))
		{
			iw_uint32 extra_size_remaining = local_header_extra_len;
			const iw_uint8 *pExtra_data = (const iw_uint8 *)file_data_array.m_p;
			
			if (pZip->m_pRead(pZip->m_pIO_opaque, local_header_ofs + IW_ZIP_LOCAL_DIR_HEADER_SIZE + local_header_filename_len, file_data_array.m_p, local_header_extra_len) != local_header_extra_len)
			{
				iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
				goto handle_failure;
			}
			
			do
			{
				iw_uint32 field_id, field_data_size, field_total_size;
				
				if (extra_size_remaining < (sizeof(iw_uint16) * 2))
				{
					iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
					goto handle_failure;
				}
				
				field_id = IW_READ_LE16(pExtra_data);
				field_data_size = IW_READ_LE16(pExtra_data + sizeof(iw_uint16));
				field_total_size = field_data_size + sizeof(iw_uint16) * 2;
				
				if (field_total_size > extra_size_remaining)
				{
					iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
					goto handle_failure;
				}
				
				if (field_id == IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID)
				{
					const iw_uint8 *pSrc_field_data = pExtra_data + sizeof(iw_uint32);
					
					if (field_data_size < sizeof(iw_uint64) * 2)
					{
						iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
						goto handle_failure;
					}
					
					local_header_uncomp_size = IW_READ_LE64(pSrc_field_data);
					local_header_comp_size = IW_READ_LE64(pSrc_field_data + sizeof(iw_uint64));
					
					found_zip64_ext_data_in_ldir = IW_TRUE;
					break;
				}
				
				pExtra_data += field_total_size;
				extra_size_remaining -= field_total_size;
			} while (extra_size_remaining);
		}
		
		if ((has_data_descriptor) && (!local_header_comp_size) && (!local_header_crc32))
		{
			iw_uint8 descriptor_buf[32];
			iw_bool has_id;
			const iw_uint8 *pSrc;
			iw_uint32 file_crc32;
			iw_uint64 comp_size = 0, uncomp_size = 0;
			
			iw_uint32 num_descriptor_uint32s = ((pState->m_zip64) || (found_zip64_ext_data_in_ldir)) ? 6 : 4;
			
			if (pZip->m_pRead(pZip->m_pIO_opaque, local_header_ofs + IW_ZIP_LOCAL_DIR_HEADER_SIZE + local_header_filename_len + local_header_extra_len + file_stat.m_comp_size, descriptor_buf, sizeof(iw_uint32) * num_descriptor_uint32s) != (sizeof(iw_uint32) * num_descriptor_uint32s))
			{
				iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
				goto handle_failure;
			}
			
			has_id = (IW_READ_LE32(descriptor_buf) == IW_ZIP_DATA_DESCRIPTOR_ID);
			pSrc = has_id ? (descriptor_buf + sizeof(iw_uint32)) : descriptor_buf;
			
			file_crc32 = IW_READ_LE32(pSrc);
			
			if ((pState->m_zip64) || (found_zip64_ext_data_in_ldir))
			{
				comp_size = IW_READ_LE64(pSrc + sizeof(iw_uint32));
				uncomp_size = IW_READ_LE64(pSrc + sizeof(iw_uint32) + sizeof(iw_uint64));
			}
			else
			{
				comp_size = IW_READ_LE32(pSrc + sizeof(iw_uint32));
				uncomp_size = IW_READ_LE32(pSrc + sizeof(iw_uint32) + sizeof(iw_uint32));
			}
			
			if ((file_crc32 != file_stat.m_crc32) || (comp_size != file_stat.m_comp_size) || (uncomp_size != file_stat.m_uncomp_size))
			{
				iw_zip_set_error(pZip, IW_ZIP_VALIDATION_FAILED);
				goto handle_failure;
			}
		}
		else
		{
			if ((local_header_crc32 != file_stat.m_crc32) || (local_header_comp_size != file_stat.m_comp_size) || (local_header_uncomp_size != file_stat.m_uncomp_size))
			{
				iw_zip_set_error(pZip, IW_ZIP_VALIDATION_FAILED);
				goto handle_failure;
			}
		}
		
		iw_zip_array_clear(pZip, &file_data_array);
		
		if ((flags & IW_ZIP_FLAG_VALIDATE_HEADERS_ONLY) == 0)
		{
			if (!iw_zip_reader_extract_to_callback(pZip, file_index, iw_zip_compute_crc32_callback, &uncomp_crc32, 0))
				return IW_FALSE;
			
			if (uncomp_crc32 != file_stat.m_crc32)
			{
				iw_zip_set_error(pZip, IW_ZIP_VALIDATION_FAILED);
				return IW_FALSE;
			}
		}
		
		return IW_TRUE;
		
		handle_failure:
		iw_zip_array_clear(pZip, &file_data_array);
		return IW_FALSE;
	}
	
	iw_bool iw_zip_validate_archive(iw_zip_archive *pZip, iw_uint flags)
	{
		iw_zip_internal_state *pState;
		iw_uint32 i;
		
		if ((!pZip) || (!pZip->m_pState) || (!pZip->m_pAlloc) || (!pZip->m_pFree) || (!pZip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pState = pZip->m_pState;
		
		if (!pState->m_zip64)
		{
			if (pZip->m_total_files > IW_UINT16_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
			
			if (pZip->m_archive_size > IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
		}
		else
		{
			if (pState->m_central_dir.m_size >= IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
		}
		
		for (i = 0; i < pZip->m_total_files; i++)
		{
			if (IW_ZIP_FLAG_VALIDATE_LOCATE_FILE_FLAG & flags)
			{
				iw_uint32 found_index;
				iw_zip_archive_file_stat stat;
				
				if (!iw_zip_reader_file_stat(pZip, i, &stat))
					return IW_FALSE;
				
				if (!iw_zip_reader_locate_file_v2(pZip, stat.m_filename, NULL, 0, &found_index))
					return IW_FALSE;
				
				if (found_index != i)
					return iw_zip_set_error(pZip, IW_ZIP_VALIDATION_FAILED);
			}
			
			if (!iw_zip_validate_file(pZip, i, flags))
				return IW_FALSE;
		}
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_validate_mem_archive(const void *pMem, size_t size, iw_uint flags, iw_zip_error *pErr)
	{
		iw_bool success = IW_TRUE;
		iw_zip_archive zip;
		iw_zip_error actual_err = IW_ZIP_NO_ERROR;
		
		if ((!pMem) || (!size))
		{
			if (pErr)
				*pErr = IW_ZIP_INVALID_PARAMETER;
			return IW_FALSE;
		}
		
		iw_zip_zero_struct(&zip);
		
		if (!iw_zip_reader_init_mem(&zip, pMem, size, flags))
		{
			if (pErr)
				*pErr = zip.m_last_error;
			return IW_FALSE;
		}
		
		if (!iw_zip_validate_archive(&zip, flags))
		{
			actual_err = zip.m_last_error;
			success = IW_FALSE;
		}
		
		if (!iw_zip_reader_end_internal(&zip, success))
		{
			if (!actual_err)
				actual_err = zip.m_last_error;
			success = IW_FALSE;
		}
		
		if (pErr)
			*pErr = actual_err;
		
		return success;
	}
	
#ifndef i2WC_NO_STDIO
	iw_bool iw_zip_validate_file_archive(const char *pFilename, iw_uint flags, iw_zip_error *pErr)
	{
		iw_bool success = IW_TRUE;
		iw_zip_archive zip;
		iw_zip_error actual_err = IW_ZIP_NO_ERROR;
		
		if (!pFilename)
		{
			if (pErr)
				*pErr = IW_ZIP_INVALID_PARAMETER;
			return IW_FALSE;
		}
		
		iw_zip_zero_struct(&zip);
		
		if (!iw_zip_reader_init_file_v2(&zip, pFilename, flags, 0, 0))
		{
			if (pErr)
				*pErr = zip.m_last_error;
			return IW_FALSE;
		}
		
		if (!iw_zip_validate_archive(&zip, flags))
		{
			actual_err = zip.m_last_error;
			success = IW_FALSE;
		}
		
		if (!iw_zip_reader_end_internal(&zip, success))
		{
			if (!actual_err)
				actual_err = zip.m_last_error;
			success = IW_FALSE;
		}
		
		if (pErr)
			*pErr = actual_err;
		
		return success;
	}
#endif
	
#ifndef i2WC_NO_ARCHIVE_WRITING_APIS
	
	static IW_FORCEINLINE void iw_write_le16(iw_uint8 *p, iw_uint16 v)
	{
		p[0] = (iw_uint8)v;
		p[1] = (iw_uint8)(v >> 8);
	}
	static IW_FORCEINLINE void iw_write_le32(iw_uint8 *p, iw_uint32 v)
	{
		p[0] = (iw_uint8)v;
		p[1] = (iw_uint8)(v >> 8);
		p[2] = (iw_uint8)(v >> 16);
		p[3] = (iw_uint8)(v >> 24);
	}
	static IW_FORCEINLINE void iw_write_le64(iw_uint8 *p, iw_uint64 v)
	{
		iw_write_le32(p, (iw_uint32)v);
		iw_write_le32(p + sizeof(iw_uint32), (iw_uint32)(v >> 32));
	}
	
#define IW_WRITE_LE16(p, v) iw_write_le16((iw_uint8 *)(p), (iw_uint16)(v))
#define IW_WRITE_LE32(p, v) iw_write_le32((iw_uint8 *)(p), (iw_uint32)(v))
#define IW_WRITE_LE64(p, v) iw_write_le64((iw_uint8 *)(p), (iw_uint64)(v))
	
	static size_t iw_zip_heap_write_func(void *pOpaque, iw_uint64 file_ofs, const void *pBuf, size_t n)
	{
		iw_zip_archive *pZip = (iw_zip_archive *)pOpaque;
		iw_zip_internal_state *pState = pZip->m_pState;
		iw_uint64 new_size = IW_MAX(file_ofs + n, pState->m_mem_size);
		
		if (!n)
			return 0;
		
		if ((sizeof(size_t) == sizeof(iw_uint32)) && (new_size > 0x7FFFFFFF))
		{
			iw_zip_set_error(pZip, IW_ZIP_FILE_TOO_LARGE);
			return 0;
		}
		
		if (new_size > pState->m_mem_capacity)
		{
			void *pNew_block;
			size_t new_capacity = IW_MAX(64, pState->m_mem_capacity);
			
			while (new_capacity < new_size)
				new_capacity *= 2;
			
			if (NULL == (pNew_block = pZip->m_pRealloc(pZip->m_pAlloc_opaque, pState->m_pMem, 1, new_capacity)))
			{
				iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
				return 0;
			}
			
			pState->m_pMem = pNew_block;
			pState->m_mem_capacity = new_capacity;
		}
		memcpy((iw_uint8 *)pState->m_pMem + file_ofs, pBuf, n);
		pState->m_mem_size = (size_t)new_size;
		return n;
	}
	
	static iw_bool iw_zip_writer_end_internal(iw_zip_archive *pZip, iw_bool set_last_error)
	{
		iw_zip_internal_state *pState;
		iw_bool status = IW_TRUE;
		
		if ((!pZip) || (!pZip->m_pState) || (!pZip->m_pAlloc) || (!pZip->m_pFree) || ((pZip->m_zip_mode != IW_ZIP_MODE_WRITING) && (pZip->m_zip_mode != IW_ZIP_MODE_WRITING_HAS_BEEN_FINALIZED)))
		{
			if (set_last_error)
				iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			return IW_FALSE;
		}
		
		pState = pZip->m_pState;
		pZip->m_pState = NULL;
		iw_zip_array_clear(pZip, &pState->m_central_dir);
		iw_zip_array_clear(pZip, &pState->m_central_dir_offsets);
		iw_zip_array_clear(pZip, &pState->m_sorted_central_dir_offsets);
		
#ifndef i2WC_NO_STDIO
		if (pState->m_pFile)
		{
			if (pZip->m_zip_type == IW_ZIP_TYPE_FILE)
			{
				if (IW_FCLOSE(pState->m_pFile) == EOF)
				{
					if (set_last_error)
						iw_zip_set_error(pZip, IW_ZIP_FILE_CLOSE_FAILED);
					status = IW_FALSE;
				}
			}
			
			pState->m_pFile = NULL;
		}
#endif
		
		if ((pZip->m_pWrite == iw_zip_heap_write_func) && (pState->m_pMem))
		{
			pZip->m_pFree(pZip->m_pAlloc_opaque, pState->m_pMem);
			pState->m_pMem = NULL;
		}
		
		pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
		pZip->m_zip_mode = IW_ZIP_MODE_INVALID;
		return status;
	}
	
	iw_bool iw_zip_writer_init_v2(iw_zip_archive *pZip, iw_uint64 existing_size, iw_uint flags)
	{
		iw_bool zip64 = (flags & IW_ZIP_FLAG_WRITE_ZIP64) != 0;
		
		if ((!pZip) || (pZip->m_pState) || (!pZip->m_pWrite) || (pZip->m_zip_mode != IW_ZIP_MODE_INVALID))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (flags & IW_ZIP_FLAG_WRITE_ALLOW_READING)
		{
			if (!pZip->m_pRead)
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		}
		
		if (pZip->m_file_offset_alignment)
		{
			if (pZip->m_file_offset_alignment & (pZip->m_file_offset_alignment - 1))
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		}
		
		if (!pZip->m_pAlloc)
			pZip->m_pAlloc = i2wC_def_alloc_func;
		if (!pZip->m_pFree)
			pZip->m_pFree = i2wC_def_free_func;
		if (!pZip->m_pRealloc)
			pZip->m_pRealloc = i2wC_def_realloc_func;
		
		pZip->m_archive_size = existing_size;
		pZip->m_central_directory_file_ofs = 0;
		pZip->m_total_files = 0;
		
		if (NULL == (pZip->m_pState = (iw_zip_internal_state *)pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, sizeof(iw_zip_internal_state))))
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		
		memset(pZip->m_pState, 0, sizeof(iw_zip_internal_state));
		
		IW_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_central_dir, sizeof(iw_uint8));
		IW_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_central_dir_offsets, sizeof(iw_uint32));
		IW_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_sorted_central_dir_offsets, sizeof(iw_uint32));
		
		pZip->m_pState->m_zip64 = zip64;
		pZip->m_pState->m_zip64_has_extended_info_fields = zip64;
		
		pZip->m_zip_type = IW_ZIP_TYPE_USER;
		pZip->m_zip_mode = IW_ZIP_MODE_WRITING;
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_init(iw_zip_archive *pZip, iw_uint64 existing_size)
	{
		return iw_zip_writer_init_v2(pZip, existing_size, 0);
	}
	
	iw_bool iw_zip_writer_init_heap_v2(iw_zip_archive *pZip, size_t size_to_reserve_at_beginning, size_t initial_allocation_size, iw_uint flags)
	{
		pZip->m_pWrite = iw_zip_heap_write_func;
		pZip->m_pNeeds_keepalive = NULL;
		
		if (flags & IW_ZIP_FLAG_WRITE_ALLOW_READING)
			pZip->m_pRead = iw_zip_mem_read_func;
		
		pZip->m_pIO_opaque = pZip;
		
		if (!iw_zip_writer_init_v2(pZip, size_to_reserve_at_beginning, flags))
			return IW_FALSE;
		
		pZip->m_zip_type = IW_ZIP_TYPE_HEAP;
		
		if (0 != (initial_allocation_size = IW_MAX(initial_allocation_size, size_to_reserve_at_beginning)))
		{
			if (NULL == (pZip->m_pState->m_pMem = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, initial_allocation_size)))
			{
				iw_zip_writer_end_internal(pZip, IW_FALSE);
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			pZip->m_pState->m_mem_capacity = initial_allocation_size;
		}
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_init_heap(iw_zip_archive *pZip, size_t size_to_reserve_at_beginning, size_t initial_allocation_size)
	{
		return iw_zip_writer_init_heap_v2(pZip, size_to_reserve_at_beginning, initial_allocation_size, 0);
	}
	
#ifndef i2WC_NO_STDIO
	static size_t iw_zip_file_write_func(void *pOpaque, iw_uint64 file_ofs, const void *pBuf, size_t n)
	{
		iw_zip_archive *pZip = (iw_zip_archive *)pOpaque;
		iw_int64 cur_ofs = IW_FTELL64(pZip->m_pState->m_pFile);
		
		file_ofs += pZip->m_pState->m_file_archive_start_ofs;
		
		if (((iw_int64)file_ofs < 0) || (((cur_ofs != (iw_int64)file_ofs)) && (IW_FSEEK64(pZip->m_pState->m_pFile, (iw_int64)file_ofs, SEEK_SET))))
		{
			iw_zip_set_error(pZip, IW_ZIP_FILE_SEEK_FAILED);
			return 0;
		}
		
		return IW_FWRITE(pBuf, 1, n, pZip->m_pState->m_pFile);
	}
	
	iw_bool iw_zip_writer_init_file(iw_zip_archive *pZip, const char *pFilename, iw_uint64 size_to_reserve_at_beginning)
	{
		return iw_zip_writer_init_file_v2(pZip, pFilename, size_to_reserve_at_beginning, 0);
	}
	
	iw_bool iw_zip_writer_init_file_v2(iw_zip_archive *pZip, const char *pFilename, iw_uint64 size_to_reserve_at_beginning, iw_uint flags)
	{
		IW_FILE *pFile;
		
		pZip->m_pWrite = iw_zip_file_write_func;
		pZip->m_pNeeds_keepalive = NULL;
		
		if (flags & IW_ZIP_FLAG_WRITE_ALLOW_READING)
			pZip->m_pRead = iw_zip_file_read_func;
		
		pZip->m_pIO_opaque = pZip;
		
		if (!iw_zip_writer_init_v2(pZip, size_to_reserve_at_beginning, flags))
			return IW_FALSE;
		
		if (NULL == (pFile = IW_FOPEN(pFilename, (flags & IW_ZIP_FLAG_WRITE_ALLOW_READING) ? "w+b" : "wb")))
		{
			iw_zip_writer_end(pZip);
			return iw_zip_set_error(pZip, IW_ZIP_FILE_OPEN_FAILED);
		}
		
		pZip->m_pState->m_pFile = pFile;
		pZip->m_zip_type = IW_ZIP_TYPE_FILE;
		
		if (size_to_reserve_at_beginning)
		{
			iw_uint64 cur_ofs = 0;
			char buf[4096];
			
			IW_CLEAR_ARR(buf);
			
			do
			{
				size_t n = (size_t)IW_MIN(sizeof(buf), size_to_reserve_at_beginning);
				if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_ofs, buf, n) != n)
				{
					iw_zip_writer_end(pZip);
					return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
				}
				cur_ofs += n;
				size_to_reserve_at_beginning -= n;
			} while (size_to_reserve_at_beginning);
		}
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_init_cfile(iw_zip_archive *pZip, IW_FILE *pFile, iw_uint flags)
	{
		pZip->m_pWrite = iw_zip_file_write_func;
		pZip->m_pNeeds_keepalive = NULL;
		
		if (flags & IW_ZIP_FLAG_WRITE_ALLOW_READING)
			pZip->m_pRead = iw_zip_file_read_func;
		
		pZip->m_pIO_opaque = pZip;
		
		if (!iw_zip_writer_init_v2(pZip, 0, flags))
			return IW_FALSE;
		
		pZip->m_pState->m_pFile = pFile;
		pZip->m_pState->m_file_archive_start_ofs = IW_FTELL64(pZip->m_pState->m_pFile);
		pZip->m_zip_type = IW_ZIP_TYPE_CFILE;
		
		return IW_TRUE;
	}
#endif
	
	iw_bool iw_zip_writer_init_from_reader_v2(iw_zip_archive *pZip, const char *pFilename, iw_uint flags)
	{
		iw_zip_internal_state *pState;
		
		if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != IW_ZIP_MODE_READING))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (flags & IW_ZIP_FLAG_WRITE_ZIP64)
		{
			if (!pZip->m_pState->m_zip64)
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		}
		
		if (pZip->m_pState->m_zip64)
		{
			if (pZip->m_total_files == IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		else
		{
			if (pZip->m_total_files == IW_UINT16_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
			
			if ((pZip->m_archive_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + IW_ZIP_LOCAL_DIR_HEADER_SIZE) > IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_TOO_LARGE);
		}
		
		pState = pZip->m_pState;
		
		if (pState->m_pFile)
		{
#ifdef i2WC_NO_STDIO
			(void)pFilename;
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
#else
			if (pZip->m_pIO_opaque != pZip)
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			
			if (pZip->m_zip_type == IW_ZIP_TYPE_FILE &&
				!(flags & IW_ZIP_FLAG_READ_ALLOW_WRITING) )
			{
				if (!pFilename)
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
				
				if (NULL == (pState->m_pFile = IW_FREOPEN(pFilename, "r+b", pState->m_pFile)))
				{
					iw_zip_reader_end_internal(pZip, IW_FALSE);
					return iw_zip_set_error(pZip, IW_ZIP_FILE_OPEN_FAILED);
				}
			}
			
			pZip->m_pWrite = iw_zip_file_write_func;
			pZip->m_pNeeds_keepalive = NULL;
#endif
		}
		else if (pState->m_pMem)
		{
			if (pZip->m_pIO_opaque != pZip)
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			
			pState->m_mem_capacity = pState->m_mem_size;
			pZip->m_pWrite = iw_zip_heap_write_func;
			pZip->m_pNeeds_keepalive = NULL;
		}
		else if (!pZip->m_pWrite)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pZip->m_archive_size = pZip->m_central_directory_file_ofs;
		pZip->m_central_directory_file_ofs = 0;
		
		iw_zip_array_clear(pZip, &pZip->m_pState->m_sorted_central_dir_offsets);
		
		pZip->m_zip_mode = IW_ZIP_MODE_WRITING;
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_init_from_reader(iw_zip_archive *pZip, const char *pFilename)
	{
		return iw_zip_writer_init_from_reader_v2(pZip, pFilename, 0);
	}
	
	iw_bool iw_zip_writer_add_mem(iw_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, iw_uint level_and_flags)
	{
		return iw_zip_writer_add_mem_ex(pZip, pArchive_name, pBuf, buf_size, NULL, 0, level_and_flags, 0, 0);
	}
	
	typedef struct
	{
		iw_zip_archive *m_pZip;
		iw_uint64 m_cur_archive_file_ofs;
		iw_uint64 m_comp_size;
	} iw_zip_writer_add_state;
	
	static iw_bool iw_zip_writer_add_put_buf_callback(const void *pBuf, int len, void *pUser)
	{
		iw_zip_writer_add_state *pState = (iw_zip_writer_add_state *)pUser;
		if ((int)pState->m_pZip->m_pWrite(pState->m_pZip->m_pIO_opaque, pState->m_cur_archive_file_ofs, pBuf, len) != len)
			return IW_FALSE;
		
		pState->m_cur_archive_file_ofs += len;
		pState->m_comp_size += len;
		return IW_TRUE;
	}
	
#define IW_ZIP64_MAX_LOCAL_EXTRA_FIELD_SIZE (sizeof(iw_uint16) * 2 + sizeof(iw_uint64) * 2)
#define IW_ZIP64_MAX_CENTRAL_EXTRA_FIELD_SIZE (sizeof(iw_uint16) * 2 + sizeof(iw_uint64) * 3)
	static iw_uint32 iw_zip_writer_create_zip64_extra_data(iw_uint8 *pBuf, iw_uint64 *pUncomp_size, iw_uint64 *pComp_size, iw_uint64 *pLocal_header_ofs)
	{
		iw_uint8 *pDst = pBuf;
		iw_uint32 field_size = 0;
		
		IW_WRITE_LE16(pDst + 0, IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID);
		IW_WRITE_LE16(pDst + 2, 0);
		pDst += sizeof(iw_uint16) * 2;
		
		if (pUncomp_size)
		{
			IW_WRITE_LE64(pDst, *pUncomp_size);
			pDst += sizeof(iw_uint64);
			field_size += sizeof(iw_uint64);
		}
		
		if (pComp_size)
		{
			IW_WRITE_LE64(pDst, *pComp_size);
			pDst += sizeof(iw_uint64);
			field_size += sizeof(iw_uint64);
		}
		
		if (pLocal_header_ofs)
		{
			IW_WRITE_LE64(pDst, *pLocal_header_ofs);
			pDst += sizeof(iw_uint64);
			field_size += sizeof(iw_uint64);
		}
		
		IW_WRITE_LE16(pBuf + 2, field_size);
		
		return (iw_uint32)(pDst - pBuf);
	}
	
	static iw_bool iw_zip_writer_create_local_dir_header(iw_zip_archive *pZip, iw_uint8 *pDst, iw_uint16 filename_size, iw_uint16 extra_size, iw_uint64 uncomp_size, iw_uint64 comp_size, iw_uint32 uncomp_crc32, iw_uint16 method, iw_uint16 bit_flags, iw_uint16 dos_time, iw_uint16 dos_date)
	{
		(void)pZip;
		memset(pDst, 0, IW_ZIP_LOCAL_DIR_HEADER_SIZE);
		IW_WRITE_LE32(pDst + IW_ZIP_LDH_SIG_OFS, IW_ZIP_LOCAL_DIR_HEADER_SIG);
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_VERSION_NEEDED_OFS, method ? 20 : 0);
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_BIT_FLAG_OFS, bit_flags);
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_METHOD_OFS, method);
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_FILE_TIME_OFS, dos_time);
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_FILE_DATE_OFS, dos_date);
		IW_WRITE_LE32(pDst + IW_ZIP_LDH_CRC32_OFS, uncomp_crc32);
		IW_WRITE_LE32(pDst + IW_ZIP_LDH_COMPRESSED_SIZE_OFS, IW_MIN(comp_size, IW_UINT32_MAX));
		IW_WRITE_LE32(pDst + IW_ZIP_LDH_DECOMPRESSED_SIZE_OFS, IW_MIN(uncomp_size, IW_UINT32_MAX));
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_FILENAME_LEN_OFS, filename_size);
		IW_WRITE_LE16(pDst + IW_ZIP_LDH_EXTRA_LEN_OFS, extra_size);
		return IW_TRUE;
	}
	
	static iw_bool iw_zip_writer_create_central_dir_header(iw_zip_archive *pZip, iw_uint8 *pDst,
														   iw_uint16 filename_size, iw_uint16 extra_size, iw_uint16 comment_size,
														   iw_uint64 uncomp_size, iw_uint64 comp_size, iw_uint32 uncomp_crc32,
														   iw_uint16 method, iw_uint16 bit_flags, iw_uint16 dos_time, iw_uint16 dos_date,
														   iw_uint64 local_header_ofs, iw_uint32 ext_attributes)
	{
		(void)pZip;
		memset(pDst, 0, IW_ZIP_CENTRAL_DIR_HEADER_SIZE);
		IW_WRITE_LE32(pDst + IW_ZIP_CDH_SIG_OFS, IW_ZIP_CENTRAL_DIR_HEADER_SIG);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_VERSION_NEEDED_OFS, method ? 20 : 0);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_BIT_FLAG_OFS, bit_flags);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_METHOD_OFS, method);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_FILE_TIME_OFS, dos_time);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_FILE_DATE_OFS, dos_date);
		IW_WRITE_LE32(pDst + IW_ZIP_CDH_CRC32_OFS, uncomp_crc32);
		IW_WRITE_LE32(pDst + IW_ZIP_CDH_COMPRESSED_SIZE_OFS, IW_MIN(comp_size, IW_UINT32_MAX));
		IW_WRITE_LE32(pDst + IW_ZIP_CDH_DECOMPRESSED_SIZE_OFS, IW_MIN(uncomp_size, IW_UINT32_MAX));
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_FILENAME_LEN_OFS, filename_size);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_EXTRA_LEN_OFS, extra_size);
		IW_WRITE_LE16(pDst + IW_ZIP_CDH_COMMENT_LEN_OFS, comment_size);
		IW_WRITE_LE32(pDst + IW_ZIP_CDH_EXTERNAL_ATTR_OFS, ext_attributes);
		IW_WRITE_LE32(pDst + IW_ZIP_CDH_LOCAL_HEADER_OFS, IW_MIN(local_header_ofs, IW_UINT32_MAX));
		return IW_TRUE;
	}
	
	static iw_bool iw_zip_writer_add_to_central_dir(iw_zip_archive *pZip, const char *pFilename, iw_uint16 filename_size,
													const void *pExtra, iw_uint16 extra_size, const void *pComment, iw_uint16 comment_size,
													iw_uint64 uncomp_size, iw_uint64 comp_size, iw_uint32 uncomp_crc32,
													iw_uint16 method, iw_uint16 bit_flags, iw_uint16 dos_time, iw_uint16 dos_date,
													iw_uint64 local_header_ofs, iw_uint32 ext_attributes,
													const char *user_extra_data, iw_uint user_extra_data_len)
	{
		iw_zip_internal_state *pState = pZip->m_pState;
		iw_uint32 central_dir_ofs = (iw_uint32)pState->m_central_dir.m_size;
		size_t orig_central_dir_size = pState->m_central_dir.m_size;
		iw_uint8 central_dir_header[IW_ZIP_CENTRAL_DIR_HEADER_SIZE];
		
		if (!pZip->m_pState->m_zip64)
		{
			if (local_header_ofs > 0xFFFFFFFF)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_TOO_LARGE);
		}
		
		if (((iw_uint64)pState->m_central_dir.m_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_size + extra_size + user_extra_data_len + comment_size) >= IW_UINT32_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_CDIR_SIZE);
		
		if (!iw_zip_writer_create_central_dir_header(pZip, central_dir_header, filename_size, (iw_uint16)(extra_size + user_extra_data_len), comment_size, uncomp_size, comp_size, uncomp_crc32, method, bit_flags, dos_time, dos_date, local_header_ofs, ext_attributes))
			return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
		
		if ((!iw_zip_array_push_back(pZip, &pState->m_central_dir, central_dir_header, IW_ZIP_CENTRAL_DIR_HEADER_SIZE)) ||
			(!iw_zip_array_push_back(pZip, &pState->m_central_dir, pFilename, filename_size)) ||
			(!iw_zip_array_push_back(pZip, &pState->m_central_dir, pExtra, extra_size)) ||
			(!iw_zip_array_push_back(pZip, &pState->m_central_dir, user_extra_data, user_extra_data_len)) ||
			(!iw_zip_array_push_back(pZip, &pState->m_central_dir, pComment, comment_size)) ||
			(!iw_zip_array_push_back(pZip, &pState->m_central_dir_offsets, &central_dir_ofs, 1)))
		{
			iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		}
		
		return IW_TRUE;
	}
	
	static iw_bool iw_zip_writer_validate_archive_name(const char *pArchive_name)
	{
		if (*pArchive_name == '/')
			return IW_FALSE;
		
		return IW_TRUE;
	}
	
	static iw_uint iw_zip_writer_compute_padding_needed_for_file_alignment(iw_zip_archive *pZip)
	{
		iw_uint32 n;
		if (!pZip->m_file_offset_alignment)
			return 0;
		n = (iw_uint32)(pZip->m_archive_size & (pZip->m_file_offset_alignment - 1));
		return (iw_uint)((pZip->m_file_offset_alignment - n) & (pZip->m_file_offset_alignment - 1));
	}
	
	static iw_bool iw_zip_writer_write_zeros(iw_zip_archive *pZip, iw_uint64 cur_file_ofs, iw_uint32 n)
	{
		char buf[4096];
		memset(buf, 0, IW_MIN(sizeof(buf), n));
		while (n)
		{
			iw_uint32 s = IW_MIN(sizeof(buf), n);
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_file_ofs, buf, s) != s)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_file_ofs += s;
			n -= s;
		}
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_add_mem_ex(iw_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags,
									 iw_uint64 uncomp_size, iw_uint32 uncomp_crc32)
	{
		return iw_zip_writer_add_mem_ex_v2(pZip, pArchive_name, pBuf, buf_size, pComment, comment_size, level_and_flags, uncomp_size, uncomp_crc32, NULL, NULL, 0, NULL, 0);
	}
	
	iw_bool iw_zip_writer_add_mem_ex_v2(iw_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size,
										iw_uint level_and_flags, iw_uint64 uncomp_size, iw_uint32 uncomp_crc32, IW_TIME_T *last_modified,
										const char *user_extra_data, iw_uint user_extra_data_len, const char *user_extra_data_central, iw_uint user_extra_data_central_len)
	{
		iw_uint16 method = 0, dos_time = 0, dos_date = 0;
		iw_uint level, ext_attributes = 0, num_alignment_padding_bytes;
		iw_uint64 local_dir_header_ofs = pZip->m_archive_size, cur_archive_file_ofs = pZip->m_archive_size, comp_size = 0;
		size_t archive_name_size;
		iw_uint8 local_dir_header[IW_ZIP_LOCAL_DIR_HEADER_SIZE];
		iwc_z_compressor *pComp = NULL;
		iw_bool store_data_uncompressed;
		iw_zip_internal_state *pState;
		iw_uint8 *pExtra_data = NULL;
		iw_uint32 extra_size = 0;
		iw_uint8 extra_data[IW_ZIP64_MAX_CENTRAL_EXTRA_FIELD_SIZE];
		iw_uint16 bit_flags = 0;
		
		if ((int)level_and_flags < 0)
			level_and_flags = IW_DEFAULT_LEVEL;
		
		if (uncomp_size || (buf_size && !(level_and_flags & IW_ZIP_FLAG_COMPRESSED_DATA)))
			bit_flags |= IW_ZIP_LDH_BIT_FLAG_HAS_LOCATOR;
		
		if (!(level_and_flags & IW_ZIP_FLAG_ASCII_FILENAME))
			bit_flags |= IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_UTF8;
		
		level = level_and_flags & 0xF;
		store_data_uncompressed = ((!level) || (level_and_flags & IW_ZIP_FLAG_COMPRESSED_DATA));
		
		if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != IW_ZIP_MODE_WRITING) || ((buf_size) && (!pBuf)) || (!pArchive_name) || ((comment_size) && (!pComment)) || (level > IW_UBER_COMPRESSION))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pState = pZip->m_pState;
		
		if (pState->m_zip64)
		{
			if (pZip->m_total_files == IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		else
		{
			if (pZip->m_total_files == IW_UINT16_MAX)
			{
				pState->m_zip64 = IW_TRUE;
			}
			if (((iw_uint64)buf_size > 0xFFFFFFFF) || (uncomp_size > 0xFFFFFFFF))
			{
				pState->m_zip64 = IW_TRUE;
			}
		}
		
		if ((!(level_and_flags & IW_ZIP_FLAG_COMPRESSED_DATA)) && (uncomp_size))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (!iw_zip_writer_validate_archive_name(pArchive_name))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_FILENAME);
		
#ifndef i2WC_NO_TIME
		if (last_modified != NULL)
		{
			iw_zip_time_t_to_dos_time(*last_modified, &dos_time, &dos_date);
		}
		else
		{
			IW_TIME_T cur_time;
			time(&cur_time);
			iw_zip_time_t_to_dos_time(cur_time, &dos_time, &dos_date);
		}
#else
		(void)last_modified;
#endif
		
		if (!(level_and_flags & IW_ZIP_FLAG_COMPRESSED_DATA))
		{
			uncomp_crc32 = (iw_uint32)iw_crc32(IW_CRC32_INIT, (const iw_uint8 *)pBuf, buf_size);
			uncomp_size = buf_size;
			if (uncomp_size <= 3)
			{
				level = 0;
				store_data_uncompressed = IW_TRUE;
			}
		}
		
		archive_name_size = strlen(pArchive_name);
		if (archive_name_size > IW_UINT16_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_FILENAME);
		
		num_alignment_padding_bytes = iw_zip_writer_compute_padding_needed_for_file_alignment(pZip);
		
		if (((iw_uint64)pState->m_central_dir.m_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + archive_name_size + IW_ZIP64_MAX_CENTRAL_EXTRA_FIELD_SIZE + comment_size) >= IW_UINT32_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_CDIR_SIZE);
		
		if (!pState->m_zip64)
		{
			if ((pZip->m_archive_size + num_alignment_padding_bytes + IW_ZIP_LOCAL_DIR_HEADER_SIZE + archive_name_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + archive_name_size + comment_size + user_extra_data_len +
				 pState->m_central_dir.m_size + IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE + user_extra_data_central_len + IW_ZIP_DATA_DESCRIPTER_SIZE32) > 0xFFFFFFFF)
			{
				pState->m_zip64 = IW_TRUE;
			}
		}
		
		if ((archive_name_size) && (pArchive_name[archive_name_size - 1] == '/'))
		{
			ext_attributes |= IW_ZIP_DOS_DIR_ATTRIBUTE_BITFLAG;
			
			if ((buf_size) || (uncomp_size))
				return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		}
		
		if ((!iw_zip_array_ensure_room(pZip, &pState->m_central_dir, IW_ZIP_CENTRAL_DIR_HEADER_SIZE + archive_name_size + comment_size + (pState->m_zip64 ? IW_ZIP64_MAX_CENTRAL_EXTRA_FIELD_SIZE : 0))) || (!iw_zip_array_ensure_room(pZip, &pState->m_central_dir_offsets, 1)))
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		
		if ((!store_data_uncompressed) && (buf_size))
		{
			if (NULL == (pComp = (iwc_z_compressor *)pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, sizeof(iwc_z_compressor))))
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		}
		
		if (!iw_zip_writer_write_zeros(pZip, cur_archive_file_ofs, num_alignment_padding_bytes))
		{
			pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
			return IW_FALSE;
		}
		
		local_dir_header_ofs += num_alignment_padding_bytes;
		if (pZip->m_file_offset_alignment)
		{
			IW_ASSERT((local_dir_header_ofs & (pZip->m_file_offset_alignment - 1)) == 0);
		}
		cur_archive_file_ofs += num_alignment_padding_bytes;
		
		IW_CLEAR_ARR(local_dir_header);
		
		if (!store_data_uncompressed || (level_and_flags & IW_ZIP_FLAG_COMPRESSED_DATA))
		{
			method = IW_DEFLATED;
		}
		
		if (pState->m_zip64)
		{
			if (uncomp_size >= IW_UINT32_MAX || local_dir_header_ofs >= IW_UINT32_MAX)
			{
				pExtra_data = extra_data;
				extra_size = iw_zip_writer_create_zip64_extra_data(extra_data, (uncomp_size >= IW_UINT32_MAX) ? &uncomp_size : NULL,
																   (uncomp_size >= IW_UINT32_MAX) ? &comp_size : NULL, (local_dir_header_ofs >= IW_UINT32_MAX) ? &local_dir_header_ofs : NULL);
			}
			
			if (!iw_zip_writer_create_local_dir_header(pZip, local_dir_header, (iw_uint16)archive_name_size, (iw_uint16)(extra_size + user_extra_data_len), 0, 0, 0, method, bit_flags, dos_time, dos_date))
				return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, local_dir_header_ofs, local_dir_header, sizeof(local_dir_header)) != sizeof(local_dir_header))
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += sizeof(local_dir_header);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pArchive_name, archive_name_size) != archive_name_size)
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			cur_archive_file_ofs += archive_name_size;
			
			if (pExtra_data != NULL)
			{
				if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, extra_data, extra_size) != extra_size)
					return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
				
				cur_archive_file_ofs += extra_size;
			}
		}
		else
		{
			if ((comp_size > IW_UINT32_MAX) || (cur_archive_file_ofs > IW_UINT32_MAX))
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
			if (!iw_zip_writer_create_local_dir_header(pZip, local_dir_header, (iw_uint16)archive_name_size, (iw_uint16)user_extra_data_len, 0, 0, 0, method, bit_flags, dos_time, dos_date))
				return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, local_dir_header_ofs, local_dir_header, sizeof(local_dir_header)) != sizeof(local_dir_header))
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += sizeof(local_dir_header);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pArchive_name, archive_name_size) != archive_name_size)
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			cur_archive_file_ofs += archive_name_size;
		}
		
		if (user_extra_data_len > 0)
		{
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, user_extra_data, user_extra_data_len) != user_extra_data_len)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += user_extra_data_len;
		}
		
		if (store_data_uncompressed)
		{
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pBuf, buf_size) != buf_size)
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			
			cur_archive_file_ofs += buf_size;
			comp_size = buf_size;
		}
		else if (buf_size)
		{
			iw_zip_writer_add_state state;
			
			state.m_pZip = pZip;
			state.m_cur_archive_file_ofs = cur_archive_file_ofs;
			state.m_comp_size = 0;
			
			if ((iwc_z_init(pComp, iw_zip_writer_add_put_buf_callback, &state, iwc_z_create_comp_flags_from_zip_params(level, -15, IW_DEFAULT_STRATEGY)) != iwc_z_STATUS_OKAY) ||
				(iwc_z_compress_buffer(pComp, pBuf, buf_size, iwc_z_FINISH) != iwc_z_STATUS_DONE))
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
				return iw_zip_set_error(pZip, IW_ZIP_COMPRESSION_FAILED);
			}
			
			comp_size = state.m_comp_size;
			cur_archive_file_ofs = state.m_cur_archive_file_ofs;
		}
		
		pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
		pComp = NULL;
		
		if (uncomp_size)
		{
			iw_uint8 local_dir_footer[IW_ZIP_DATA_DESCRIPTER_SIZE64];
			iw_uint32 local_dir_footer_size = IW_ZIP_DATA_DESCRIPTER_SIZE32;
			
			IW_ASSERT(bit_flags & IW_ZIP_LDH_BIT_FLAG_HAS_LOCATOR);
			
			IW_WRITE_LE32(local_dir_footer + 0, IW_ZIP_DATA_DESCRIPTOR_ID);
			IW_WRITE_LE32(local_dir_footer + 4, uncomp_crc32);
			if (pExtra_data == NULL)
			{
				if (comp_size > IW_UINT32_MAX)
					return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
				
				IW_WRITE_LE32(local_dir_footer + 8, comp_size);
				IW_WRITE_LE32(local_dir_footer + 12, uncomp_size);
			}
			else
			{
				IW_WRITE_LE64(local_dir_footer + 8, comp_size);
				IW_WRITE_LE64(local_dir_footer + 16, uncomp_size);
				local_dir_footer_size = IW_ZIP_DATA_DESCRIPTER_SIZE64;
			}
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, local_dir_footer, local_dir_footer_size) != local_dir_footer_size)
				return IW_FALSE;
			
			cur_archive_file_ofs += local_dir_footer_size;
		}
		
		if (pExtra_data != NULL)
		{
			extra_size = iw_zip_writer_create_zip64_extra_data(extra_data, (uncomp_size >= IW_UINT32_MAX) ? &uncomp_size : NULL,
															   (uncomp_size >= IW_UINT32_MAX) ? &comp_size : NULL, (local_dir_header_ofs >= IW_UINT32_MAX) ? &local_dir_header_ofs : NULL);
		}
		
		if (!iw_zip_writer_add_to_central_dir(pZip, pArchive_name, (iw_uint16)archive_name_size, pExtra_data, (iw_uint16)extra_size, pComment,
											  comment_size, uncomp_size, comp_size, uncomp_crc32, method, bit_flags, dos_time, dos_date, local_dir_header_ofs, ext_attributes,
											  user_extra_data_central, user_extra_data_central_len))
			return IW_FALSE;
		
		pZip->m_total_files++;
		pZip->m_archive_size = cur_archive_file_ofs;
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_add_read_buf_callback(iw_zip_archive *pZip, const char *pArchive_name, iw_file_read_func read_callback, void *callback_opaque, iw_uint64 max_size, const IW_TIME_T *pFile_time, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags,
												const char *user_extra_data, iw_uint user_extra_data_len, const char *user_extra_data_central, iw_uint user_extra_data_central_len)
	{
		iw_uint16 gen_flags;
		iw_uint uncomp_crc32 = IW_CRC32_INIT, level, num_alignment_padding_bytes;
		iw_uint16 method = 0, dos_time = 0, dos_date = 0, ext_attributes = 0;
		iw_uint64 local_dir_header_ofs, cur_archive_file_ofs = pZip->m_archive_size, uncomp_size = 0, comp_size = 0;
		size_t archive_name_size;
		iw_uint8 local_dir_header[IW_ZIP_LOCAL_DIR_HEADER_SIZE];
		iw_uint8 *pExtra_data = NULL;
		iw_uint32 extra_size = 0;
		iw_uint8 extra_data[IW_ZIP64_MAX_CENTRAL_EXTRA_FIELD_SIZE];
		iw_zip_internal_state *pState;
		iw_uint64 file_ofs = 0, cur_archive_header_file_ofs;
		
		if ((int)level_and_flags < 0)
			level_and_flags = IW_DEFAULT_LEVEL;
		level = level_and_flags & 0xF;
		
		gen_flags = (level_and_flags & IW_ZIP_FLAG_WRITE_HEADER_SET_SIZE) ? 0 : IW_ZIP_LDH_BIT_FLAG_HAS_LOCATOR;
		
		if (!(level_and_flags & IW_ZIP_FLAG_ASCII_FILENAME))
			gen_flags |= IW_ZIP_GENERAL_PURPOSE_BIT_FLAG_UTF8;
		
		if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != IW_ZIP_MODE_WRITING) || (!pArchive_name) || ((comment_size) && (!pComment)) || (level > IW_UBER_COMPRESSION))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pState = pZip->m_pState;
		
		if ((!pState->m_zip64) && (max_size > IW_UINT32_MAX))
		{
			pState->m_zip64 = IW_TRUE;
		}
		
		if (level_and_flags & IW_ZIP_FLAG_COMPRESSED_DATA)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (!iw_zip_writer_validate_archive_name(pArchive_name))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_FILENAME);
		
		if (pState->m_zip64)
		{
			if (pZip->m_total_files == IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		else
		{
			if (pZip->m_total_files == IW_UINT16_MAX)
			{
				pState->m_zip64 = IW_TRUE;
			}
		}
		
		archive_name_size = strlen(pArchive_name);
		if (archive_name_size > IW_UINT16_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_FILENAME);
		
		num_alignment_padding_bytes = iw_zip_writer_compute_padding_needed_for_file_alignment(pZip);
		
		if (((iw_uint64)pState->m_central_dir.m_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + archive_name_size + IW_ZIP64_MAX_CENTRAL_EXTRA_FIELD_SIZE + comment_size) >= IW_UINT32_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_CDIR_SIZE);
		
		if (!pState->m_zip64)
		{
			if ((pZip->m_archive_size + num_alignment_padding_bytes + IW_ZIP_LOCAL_DIR_HEADER_SIZE + archive_name_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + archive_name_size + comment_size + user_extra_data_len + pState->m_central_dir.m_size + IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE + 1024 + IW_ZIP_DATA_DESCRIPTER_SIZE32 + user_extra_data_central_len) > 0xFFFFFFFF)
			{
				pState->m_zip64 = IW_TRUE;
			}
		}
		
#ifndef i2WC_NO_TIME
		if (pFile_time)
		{
			iw_zip_time_t_to_dos_time(*pFile_time, &dos_time, &dos_date);
		}
#else
		(void)pFile_time;
#endif
		
		if (max_size <= 3)
			level = 0;
		
		if (!iw_zip_writer_write_zeros(pZip, cur_archive_file_ofs, num_alignment_padding_bytes))
		{
			return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
		}
		
		cur_archive_file_ofs += num_alignment_padding_bytes;
		local_dir_header_ofs = cur_archive_file_ofs;
		
		if (pZip->m_file_offset_alignment)
		{
			IW_ASSERT((cur_archive_file_ofs & (pZip->m_file_offset_alignment - 1)) == 0);
		}
		
		if (max_size && level)
		{
			method = IW_DEFLATED;
		}
		
		IW_CLEAR_ARR(local_dir_header);
		if (pState->m_zip64)
		{
			if (max_size >= IW_UINT32_MAX || local_dir_header_ofs >= IW_UINT32_MAX)
			{
				pExtra_data = extra_data;
				if (level_and_flags & IW_ZIP_FLAG_WRITE_HEADER_SET_SIZE)
					extra_size = iw_zip_writer_create_zip64_extra_data(extra_data, (max_size >= IW_UINT32_MAX) ? &uncomp_size : NULL,
																	   (max_size >= IW_UINT32_MAX) ? &comp_size : NULL,
																	   (local_dir_header_ofs >= IW_UINT32_MAX) ? &local_dir_header_ofs : NULL);
				else
					extra_size = iw_zip_writer_create_zip64_extra_data(extra_data, NULL,
																	   NULL,
																	   (local_dir_header_ofs >= IW_UINT32_MAX) ? &local_dir_header_ofs : NULL);
			}
			
			if (!iw_zip_writer_create_local_dir_header(pZip, local_dir_header, (iw_uint16)archive_name_size, (iw_uint16)(extra_size + user_extra_data_len), 0, 0, 0, method, gen_flags, dos_time, dos_date))
				return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, local_dir_header, sizeof(local_dir_header)) != sizeof(local_dir_header))
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += sizeof(local_dir_header);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pArchive_name, archive_name_size) != archive_name_size)
			{
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			
			cur_archive_file_ofs += archive_name_size;
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, extra_data, extra_size) != extra_size)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += extra_size;
		}
		else
		{
			if ((comp_size > IW_UINT32_MAX) || (cur_archive_file_ofs > IW_UINT32_MAX))
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
			if (!iw_zip_writer_create_local_dir_header(pZip, local_dir_header, (iw_uint16)archive_name_size, (iw_uint16)user_extra_data_len, 0, 0, 0, method, gen_flags, dos_time, dos_date))
				return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, local_dir_header, sizeof(local_dir_header)) != sizeof(local_dir_header))
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += sizeof(local_dir_header);
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pArchive_name, archive_name_size) != archive_name_size)
			{
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			
			cur_archive_file_ofs += archive_name_size;
		}
		
		if (user_extra_data_len > 0)
		{
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, user_extra_data, user_extra_data_len) != user_extra_data_len)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			cur_archive_file_ofs += user_extra_data_len;
		}
		
		if (max_size)
		{
			void *pRead_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, IW_ZIP_MAX_IO_BUF_SIZE);
			if (!pRead_buf)
			{
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			if (!level)
			{
				while (1)
				{
					size_t n = read_callback(callback_opaque, file_ofs, pRead_buf, IW_ZIP_MAX_IO_BUF_SIZE);
					if (n == 0)
						break;
					
					if ((n > IW_ZIP_MAX_IO_BUF_SIZE) || (file_ofs + n > max_size))
					{
						pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
						return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
					}
					if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pRead_buf, n) != n)
					{
						pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
						return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
					}
					file_ofs += n;
					uncomp_crc32 = (iw_uint32)iw_crc32(uncomp_crc32, (const iw_uint8 *)pRead_buf, n);
					cur_archive_file_ofs += n;
				}
				uncomp_size = file_ofs;
				comp_size = uncomp_size;
			}
			else
			{
				iw_bool result = IW_FALSE;
				iw_zip_writer_add_state state;
				iwc_z_compressor *pComp = (iwc_z_compressor *)pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, sizeof(iwc_z_compressor));
				if (!pComp)
				{
					pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
					return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
				}
				
				state.m_pZip = pZip;
				state.m_cur_archive_file_ofs = cur_archive_file_ofs;
				state.m_comp_size = 0;
				
				if (iwc_z_init(pComp, iw_zip_writer_add_put_buf_callback, &state, iwc_z_create_comp_flags_from_zip_params(level, -15, IW_DEFAULT_STRATEGY)) != iwc_z_STATUS_OKAY)
				{
					pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
					pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
					return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
				}
				
				for (;;)
				{
					iwc_z_status status;
					iwc_z_flush flush = iwc_z_NO_FLUSH;
					
					size_t n = read_callback(callback_opaque, file_ofs, pRead_buf, IW_ZIP_MAX_IO_BUF_SIZE);
					if ((n > IW_ZIP_MAX_IO_BUF_SIZE) || (file_ofs + n > max_size))
					{
						iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
						break;
					}
					
					file_ofs += n;
					uncomp_crc32 = (iw_uint32)iw_crc32(uncomp_crc32, (const iw_uint8 *)pRead_buf, n);
					
					if (pZip->m_pNeeds_keepalive != NULL && pZip->m_pNeeds_keepalive(pZip->m_pIO_opaque))
						flush = iwc_z_FULL_FLUSH;
					
					if (n == 0)
						flush = iwc_z_FINISH;
					
					status = iwc_z_compress_buffer(pComp, pRead_buf, n, flush);
					if (status == iwc_z_STATUS_DONE)
					{
						result = IW_TRUE;
						break;
					}
					else if (status != iwc_z_STATUS_OKAY)
					{
						iw_zip_set_error(pZip, IW_ZIP_COMPRESSION_FAILED);
						break;
					}
				}
				
				pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
				
				if (!result)
				{
					pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
					return IW_FALSE;
				}
				
				uncomp_size = file_ofs;
				comp_size = state.m_comp_size;
				cur_archive_file_ofs = state.m_cur_archive_file_ofs;
			}
			
			pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
		}
		
		if (!(level_and_flags & IW_ZIP_FLAG_WRITE_HEADER_SET_SIZE))
		{
			iw_uint8 local_dir_footer[IW_ZIP_DATA_DESCRIPTER_SIZE64];
			iw_uint32 local_dir_footer_size = IW_ZIP_DATA_DESCRIPTER_SIZE32;
			
			IW_WRITE_LE32(local_dir_footer + 0, IW_ZIP_DATA_DESCRIPTOR_ID);
			IW_WRITE_LE32(local_dir_footer + 4, uncomp_crc32);
			if (pExtra_data == NULL)
			{
				if (comp_size > IW_UINT32_MAX)
					return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
				
				IW_WRITE_LE32(local_dir_footer + 8, comp_size);
				IW_WRITE_LE32(local_dir_footer + 12, uncomp_size);
			}
			else
			{
				IW_WRITE_LE64(local_dir_footer + 8, comp_size);
				IW_WRITE_LE64(local_dir_footer + 16, uncomp_size);
				local_dir_footer_size = IW_ZIP_DATA_DESCRIPTER_SIZE64;
			}
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, local_dir_footer, local_dir_footer_size) != local_dir_footer_size)
				return IW_FALSE;
			
			cur_archive_file_ofs += local_dir_footer_size;
		}
		
		if (level_and_flags & IW_ZIP_FLAG_WRITE_HEADER_SET_SIZE)
		{
			if (pExtra_data != NULL)
			{
				extra_size = iw_zip_writer_create_zip64_extra_data(extra_data, (max_size >= IW_UINT32_MAX) ? &uncomp_size : NULL,
																   (max_size >= IW_UINT32_MAX) ? &comp_size : NULL, (local_dir_header_ofs >= IW_UINT32_MAX) ? &local_dir_header_ofs : NULL);
			}
			
			if (!iw_zip_writer_create_local_dir_header(pZip, local_dir_header,
													   (iw_uint16)archive_name_size, (iw_uint16)(extra_size + user_extra_data_len),
													   (max_size >= IW_UINT32_MAX) ? IW_UINT32_MAX : uncomp_size,
													   (max_size >= IW_UINT32_MAX) ? IW_UINT32_MAX : comp_size,
													   uncomp_crc32, method, gen_flags, dos_time, dos_date))
				return iw_zip_set_error(pZip, IW_ZIP_INTERNAL_ERROR);
			
			cur_archive_header_file_ofs = local_dir_header_ofs;
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_header_file_ofs, local_dir_header, sizeof(local_dir_header)) != sizeof(local_dir_header))
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			if (pExtra_data != NULL)
			{
				cur_archive_header_file_ofs += sizeof(local_dir_header);
				
				if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_header_file_ofs, pArchive_name, archive_name_size) != archive_name_size)
				{
					return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
				}
				
				cur_archive_header_file_ofs += archive_name_size;
				
				if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_header_file_ofs, extra_data, extra_size) != extra_size)
					return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
				
				cur_archive_header_file_ofs += extra_size;
			}
		}
		
		if (pExtra_data != NULL)
		{
			extra_size = iw_zip_writer_create_zip64_extra_data(extra_data, (uncomp_size >= IW_UINT32_MAX) ? &uncomp_size : NULL,
															   (uncomp_size >= IW_UINT32_MAX) ? &comp_size : NULL, (local_dir_header_ofs >= IW_UINT32_MAX) ? &local_dir_header_ofs : NULL);
		}
		
		if (!iw_zip_writer_add_to_central_dir(pZip, pArchive_name, (iw_uint16)archive_name_size, pExtra_data, (iw_uint16)extra_size, pComment, comment_size,
											  uncomp_size, comp_size, uncomp_crc32, method, gen_flags, dos_time, dos_date, local_dir_header_ofs, ext_attributes,
											  user_extra_data_central, user_extra_data_central_len))
			return IW_FALSE;
		
		pZip->m_total_files++;
		pZip->m_archive_size = cur_archive_file_ofs;
		
		return IW_TRUE;
	}
	
#ifndef i2WC_NO_STDIO
	
	static size_t iw_file_read_func_stdio(void *pOpaque, iw_uint64 file_ofs, void *pBuf, size_t n)
	{
		IW_FILE *pSrc_file = (IW_FILE *)pOpaque;
		iw_int64 cur_ofs = IW_FTELL64(pSrc_file);
		
		if (((iw_int64)file_ofs < 0) || (((cur_ofs != (iw_int64)file_ofs)) && (IW_FSEEK64(pSrc_file, (iw_int64)file_ofs, SEEK_SET))))
			return 0;
		
		return IW_FREAD(pBuf, 1, n, pSrc_file);
	}
	
	iw_bool iw_zip_writer_add_cfile(iw_zip_archive *pZip, const char *pArchive_name, IW_FILE *pSrc_file, iw_uint64 max_size, const IW_TIME_T *pFile_time, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags,
									const char *user_extra_data, iw_uint user_extra_data_len, const char *user_extra_data_central, iw_uint user_extra_data_central_len)
	{
		return iw_zip_writer_add_read_buf_callback(pZip, pArchive_name, iw_file_read_func_stdio, pSrc_file, max_size, pFile_time, pComment, comment_size, level_and_flags,
												   user_extra_data, user_extra_data_len, user_extra_data_central, user_extra_data_central_len);
	}
	
	iw_bool iw_zip_writer_add_file(iw_zip_archive *pZip, const char *pArchive_name, const char *pSrc_filename, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags)
	{
		IW_FILE *pSrc_file = NULL;
		iw_uint64 uncomp_size = 0;
		IW_TIME_T file_modified_time;
		IW_TIME_T *pFile_time = NULL;
		iw_bool status;
		
		memset(&file_modified_time, 0, sizeof(file_modified_time));
		
#if !defined(i2WC_NO_TIME) && !defined(i2WC_NO_STDIO)
		pFile_time = &file_modified_time;
		if (!iw_zip_get_file_modified_time(pSrc_filename, &file_modified_time))
			return iw_zip_set_error(pZip, IW_ZIP_FILE_STAT_FAILED);
#endif
		
		pSrc_file = IW_FOPEN(pSrc_filename, "rb");
		if (!pSrc_file)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_OPEN_FAILED);
		
		IW_FSEEK64(pSrc_file, 0, SEEK_END);
		uncomp_size = IW_FTELL64(pSrc_file);
		IW_FSEEK64(pSrc_file, 0, SEEK_SET);
		
		status = iw_zip_writer_add_cfile(pZip, pArchive_name, pSrc_file, uncomp_size, pFile_time, pComment, comment_size, level_and_flags, NULL, 0, NULL, 0);
		
		IW_FCLOSE(pSrc_file);
		
		return status;
	}
#endif
	
	static iw_bool iw_zip_writer_update_zip64_extension_block(iw_zip_array *pNew_ext, iw_zip_archive *pZip, const iw_uint8 *pExt, iw_uint32 ext_len, iw_uint64 *pComp_size, iw_uint64 *pUncomp_size, iw_uint64 *pLocal_header_ofs, iw_uint32 *pDisk_start)
	{
		if (!iw_zip_array_reserve(pZip, pNew_ext, ext_len + 64, IW_FALSE))
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		
		iw_zip_array_resize(pZip, pNew_ext, 0, IW_FALSE);
		
		if ((pUncomp_size) || (pComp_size) || (pLocal_header_ofs) || (pDisk_start))
		{
			iw_uint8 new_ext_block[64];
			iw_uint8 *pDst = new_ext_block;
			iw_write_le16(pDst, IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID);
			iw_write_le16(pDst + sizeof(iw_uint16), 0);
			pDst += sizeof(iw_uint16) * 2;
			
			if (pUncomp_size)
			{
				iw_write_le64(pDst, *pUncomp_size);
				pDst += sizeof(iw_uint64);
			}
			
			if (pComp_size)
			{
				iw_write_le64(pDst, *pComp_size);
				pDst += sizeof(iw_uint64);
			}
			
			if (pLocal_header_ofs)
			{
				iw_write_le64(pDst, *pLocal_header_ofs);
				pDst += sizeof(iw_uint64);
			}
			
			if (pDisk_start)
			{
				iw_write_le32(pDst, *pDisk_start);
				pDst += sizeof(iw_uint32);
			}
			
			iw_write_le16(new_ext_block + sizeof(iw_uint16), (iw_uint16)((pDst - new_ext_block) - sizeof(iw_uint16) * 2));
			
			if (!iw_zip_array_push_back(pZip, pNew_ext, new_ext_block, pDst - new_ext_block))
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		}
		
		if ((pExt) && (ext_len))
		{
			iw_uint32 extra_size_remaining = ext_len;
			const iw_uint8 *pExtra_data = pExt;
			
			do
			{
				iw_uint32 field_id, field_data_size, field_total_size;
				
				if (extra_size_remaining < (sizeof(iw_uint16) * 2))
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				
				field_id = IW_READ_LE16(pExtra_data);
				field_data_size = IW_READ_LE16(pExtra_data + sizeof(iw_uint16));
				field_total_size = field_data_size + sizeof(iw_uint16) * 2;
				
				if (field_total_size > extra_size_remaining)
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				
				if (field_id != IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID)
				{
					if (!iw_zip_array_push_back(pZip, pNew_ext, pExtra_data, field_total_size))
						return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
				}
				
				pExtra_data += field_total_size;
				extra_size_remaining -= field_total_size;
			} while (extra_size_remaining);
		}
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_add_from_zip_reader(iw_zip_archive *pZip, iw_zip_archive *pSource_zip, iw_uint src_file_index)
	{
		iw_uint n, bit_flags, num_alignment_padding_bytes, src_central_dir_following_data_size;
		iw_uint64 src_archive_bytes_remaining, local_dir_header_ofs;
		iw_uint64 cur_src_file_ofs, cur_dst_file_ofs;
		iw_uint32 local_header_u32[(IW_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(iw_uint32) - 1) / sizeof(iw_uint32)];
		iw_uint8 *pLocal_header = (iw_uint8 *)local_header_u32;
		iw_uint8 new_central_header[IW_ZIP_CENTRAL_DIR_HEADER_SIZE];
		size_t orig_central_dir_size;
		iw_zip_internal_state *pState;
		void *pBuf;
		const iw_uint8 *pSrc_central_header;
		iw_zip_archive_file_stat src_file_stat;
		iw_uint32 src_filename_len, src_comment_len, src_ext_len;
		iw_uint32 local_header_filename_size, local_header_extra_len;
		iw_uint64 local_header_comp_size, local_header_uncomp_size;
		iw_bool found_zip64_ext_data_in_ldir = IW_FALSE;
		
		if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != IW_ZIP_MODE_WRITING) || (!pSource_zip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pState = pZip->m_pState;
		
		if ((pSource_zip->m_pState->m_zip64) && (!pZip->m_pState->m_zip64))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (NULL == (pSrc_central_header = iw_zip_get_cdh(pSource_zip, src_file_index)))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (IW_READ_LE32(pSrc_central_header + IW_ZIP_CDH_SIG_OFS) != IW_ZIP_CENTRAL_DIR_HEADER_SIG)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		src_filename_len = IW_READ_LE16(pSrc_central_header + IW_ZIP_CDH_FILENAME_LEN_OFS);
		src_comment_len = IW_READ_LE16(pSrc_central_header + IW_ZIP_CDH_COMMENT_LEN_OFS);
		src_ext_len = IW_READ_LE16(pSrc_central_header + IW_ZIP_CDH_EXTRA_LEN_OFS);
		src_central_dir_following_data_size = src_filename_len + src_ext_len + src_comment_len;
		
		if ((pState->m_central_dir.m_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + src_central_dir_following_data_size + 32) >= IW_UINT32_MAX)
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_CDIR_SIZE);
		
		num_alignment_padding_bytes = iw_zip_writer_compute_padding_needed_for_file_alignment(pZip);
		
		if (!pState->m_zip64)
		{
			if (pZip->m_total_files == IW_UINT16_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		else
		{
			if (pZip->m_total_files == IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		
		if (!iw_zip_file_stat_internal(pSource_zip, src_file_index, pSrc_central_header, &src_file_stat, NULL))
			return IW_FALSE;
		
		cur_src_file_ofs = src_file_stat.m_local_header_ofs;
		cur_dst_file_ofs = pZip->m_archive_size;
		
		if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs, pLocal_header, IW_ZIP_LOCAL_DIR_HEADER_SIZE) != IW_ZIP_LOCAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
		
		if (IW_READ_LE32(pLocal_header) != IW_ZIP_LOCAL_DIR_HEADER_SIG)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
		
		cur_src_file_ofs += IW_ZIP_LOCAL_DIR_HEADER_SIZE;
		
		local_header_filename_size = IW_READ_LE16(pLocal_header + IW_ZIP_LDH_FILENAME_LEN_OFS);
		local_header_extra_len = IW_READ_LE16(pLocal_header + IW_ZIP_LDH_EXTRA_LEN_OFS);
		local_header_comp_size = IW_READ_LE32(pLocal_header + IW_ZIP_LDH_COMPRESSED_SIZE_OFS);
		local_header_uncomp_size = IW_READ_LE32(pLocal_header + IW_ZIP_LDH_DECOMPRESSED_SIZE_OFS);
		src_archive_bytes_remaining = src_file_stat.m_comp_size + local_header_filename_size + local_header_extra_len;
		
		if ((local_header_extra_len) && ((local_header_comp_size == IW_UINT32_MAX) || (local_header_uncomp_size == IW_UINT32_MAX)))
		{
			iw_zip_array file_data_array;
			const iw_uint8 *pExtra_data;
			iw_uint32 extra_size_remaining = local_header_extra_len;
			
			iw_zip_array_init(&file_data_array, 1);
			if (!iw_zip_array_resize(pZip, &file_data_array, local_header_extra_len, IW_FALSE))
			{
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, src_file_stat.m_local_header_ofs + IW_ZIP_LOCAL_DIR_HEADER_SIZE + local_header_filename_size, file_data_array.m_p, local_header_extra_len) != local_header_extra_len)
			{
				iw_zip_array_clear(pZip, &file_data_array);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
			}
			
			pExtra_data = (const iw_uint8 *)file_data_array.m_p;
			
			do
			{
				iw_uint32 field_id, field_data_size, field_total_size;
				
				if (extra_size_remaining < (sizeof(iw_uint16) * 2))
				{
					iw_zip_array_clear(pZip, &file_data_array);
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				}
				
				field_id = IW_READ_LE16(pExtra_data);
				field_data_size = IW_READ_LE16(pExtra_data + sizeof(iw_uint16));
				field_total_size = field_data_size + sizeof(iw_uint16) * 2;
				
				if (field_total_size > extra_size_remaining)
				{
					iw_zip_array_clear(pZip, &file_data_array);
					return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
				}
				
				if (field_id == IW_ZIP64_EXTENDED_INFORMATION_FIELD_HEADER_ID)
				{
					const iw_uint8 *pSrc_field_data = pExtra_data + sizeof(iw_uint32);
					
					if (field_data_size < sizeof(iw_uint64) * 2)
					{
						iw_zip_array_clear(pZip, &file_data_array);
						return iw_zip_set_error(pZip, IW_ZIP_INVALID_HEADER_OR_CORRUPTED);
					}
					
					local_header_uncomp_size = IW_READ_LE64(pSrc_field_data);
					local_header_comp_size = IW_READ_LE64(pSrc_field_data + sizeof(iw_uint64));
					
					found_zip64_ext_data_in_ldir = IW_TRUE;
					break;
				}
				
				pExtra_data += field_total_size;
				extra_size_remaining -= field_total_size;
			} while (extra_size_remaining);
			
			iw_zip_array_clear(pZip, &file_data_array);
		}
		
		if (!pState->m_zip64)
		{
			iw_uint64 approx_new_archive_size = cur_dst_file_ofs + num_alignment_padding_bytes + IW_ZIP_LOCAL_DIR_HEADER_SIZE + src_archive_bytes_remaining + (sizeof(iw_uint32) * 4) +
			pState->m_central_dir.m_size + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + src_central_dir_following_data_size + IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE + 64;
			
			if (approx_new_archive_size >= IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
		}
		
		if (!iw_zip_writer_write_zeros(pZip, cur_dst_file_ofs, num_alignment_padding_bytes))
			return IW_FALSE;
		
		cur_dst_file_ofs += num_alignment_padding_bytes;
		
		local_dir_header_ofs = cur_dst_file_ofs;
		if (pZip->m_file_offset_alignment)
		{
			IW_ASSERT((local_dir_header_ofs & (pZip->m_file_offset_alignment - 1)) == 0);
		}
		
		if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_dst_file_ofs, pLocal_header, IW_ZIP_LOCAL_DIR_HEADER_SIZE) != IW_ZIP_LOCAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
		
		cur_dst_file_ofs += IW_ZIP_LOCAL_DIR_HEADER_SIZE;
		
		if (NULL == (pBuf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, (size_t)IW_MAX(32U, IW_MIN((iw_uint64)IW_ZIP_MAX_IO_BUF_SIZE, src_archive_bytes_remaining)))))
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		
		while (src_archive_bytes_remaining)
		{
			n = (iw_uint)IW_MIN((iw_uint64)IW_ZIP_MAX_IO_BUF_SIZE, src_archive_bytes_remaining);
			if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs, pBuf, n) != n)
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
			}
			cur_src_file_ofs += n;
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_dst_file_ofs, pBuf, n) != n)
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			cur_dst_file_ofs += n;
			
			src_archive_bytes_remaining -= n;
		}
		
		bit_flags = IW_READ_LE16(pLocal_header + IW_ZIP_LDH_BIT_FLAG_OFS);
		if (bit_flags & 8)
		{
			if ((pSource_zip->m_pState->m_zip64) || (found_zip64_ext_data_in_ldir))
			{
				if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs, pBuf, (sizeof(iw_uint32) * 6)) != (sizeof(iw_uint32) * 6))
				{
					pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
					return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
				}
				
				n = sizeof(iw_uint32) * ((IW_READ_LE32(pBuf) == IW_ZIP_DATA_DESCRIPTOR_ID) ? 6 : 5);
			}
			else
			{
				iw_bool has_id;
				
				if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs, pBuf, sizeof(iw_uint32) * 4) != sizeof(iw_uint32) * 4)
				{
					pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
					return iw_zip_set_error(pZip, IW_ZIP_FILE_READ_FAILED);
				}
				
				has_id = (IW_READ_LE32(pBuf) == IW_ZIP_DATA_DESCRIPTOR_ID);
				
				if (pZip->m_pState->m_zip64)
				{
					const iw_uint8 *pSrc_descriptor = (const iw_uint8 *)pBuf + (has_id ? sizeof(iw_uint32) : 0);
					const iw_uint32 src_crc32 = IW_READ_LE32(pSrc_descriptor);
					const iw_uint64 src_comp_size = IW_READ_LE32(pSrc_descriptor + sizeof(iw_uint32));
					const iw_uint64 src_uncomp_size = IW_READ_LE32(pSrc_descriptor + 2 * sizeof(iw_uint32));
					
					iw_write_le32((iw_uint8 *)pBuf, IW_ZIP_DATA_DESCRIPTOR_ID);
					iw_write_le32((iw_uint8 *)pBuf + sizeof(iw_uint32) * 1, src_crc32);
					iw_write_le64((iw_uint8 *)pBuf + sizeof(iw_uint32) * 2, src_comp_size);
					iw_write_le64((iw_uint8 *)pBuf + sizeof(iw_uint32) * 4, src_uncomp_size);
					
					n = sizeof(iw_uint32) * 6;
				}
				else
				{
					n = sizeof(iw_uint32) * (has_id ? 4 : 3);
				}
			}
			
			if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_dst_file_ofs, pBuf, n) != n)
			{
				pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			}
			
			cur_src_file_ofs += n;
			cur_dst_file_ofs += n;
		}
		pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
		
		orig_central_dir_size = pState->m_central_dir.m_size;
		
		memcpy(new_central_header, pSrc_central_header, IW_ZIP_CENTRAL_DIR_HEADER_SIZE);
		
		if (pState->m_zip64)
		{
			const iw_uint8 *pSrc_ext = pSrc_central_header + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + src_filename_len;
			iw_zip_array new_ext_block;
			
			iw_zip_array_init(&new_ext_block, sizeof(iw_uint8));
			
			IW_WRITE_LE32(new_central_header + IW_ZIP_CDH_COMPRESSED_SIZE_OFS, IW_UINT32_MAX);
			IW_WRITE_LE32(new_central_header + IW_ZIP_CDH_DECOMPRESSED_SIZE_OFS, IW_UINT32_MAX);
			IW_WRITE_LE32(new_central_header + IW_ZIP_CDH_LOCAL_HEADER_OFS, IW_UINT32_MAX);
			
			if (!iw_zip_writer_update_zip64_extension_block(&new_ext_block, pZip, pSrc_ext, src_ext_len, &src_file_stat.m_comp_size, &src_file_stat.m_uncomp_size, &local_dir_header_ofs, NULL))
			{
				iw_zip_array_clear(pZip, &new_ext_block);
				return IW_FALSE;
			}
			
			IW_WRITE_LE16(new_central_header + IW_ZIP_CDH_EXTRA_LEN_OFS, new_ext_block.m_size);
			
			if (!iw_zip_array_push_back(pZip, &pState->m_central_dir, new_central_header, IW_ZIP_CENTRAL_DIR_HEADER_SIZE))
			{
				iw_zip_array_clear(pZip, &new_ext_block);
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			if (!iw_zip_array_push_back(pZip, &pState->m_central_dir, pSrc_central_header + IW_ZIP_CENTRAL_DIR_HEADER_SIZE, src_filename_len))
			{
				iw_zip_array_clear(pZip, &new_ext_block);
				iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			if (!iw_zip_array_push_back(pZip, &pState->m_central_dir, new_ext_block.m_p, new_ext_block.m_size))
			{
				iw_zip_array_clear(pZip, &new_ext_block);
				iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			if (!iw_zip_array_push_back(pZip, &pState->m_central_dir, pSrc_central_header + IW_ZIP_CENTRAL_DIR_HEADER_SIZE + src_filename_len + src_ext_len, src_comment_len))
			{
				iw_zip_array_clear(pZip, &new_ext_block);
				iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
			
			iw_zip_array_clear(pZip, &new_ext_block);
		}
		else
		{
			if (cur_dst_file_ofs > IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
			
			if (local_dir_header_ofs >= IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_ARCHIVE_TOO_LARGE);
			
			IW_WRITE_LE32(new_central_header + IW_ZIP_CDH_LOCAL_HEADER_OFS, local_dir_header_ofs);
			
			if (!iw_zip_array_push_back(pZip, &pState->m_central_dir, new_central_header, IW_ZIP_CENTRAL_DIR_HEADER_SIZE))
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			
			if (!iw_zip_array_push_back(pZip, &pState->m_central_dir, pSrc_central_header + IW_ZIP_CENTRAL_DIR_HEADER_SIZE, src_central_dir_following_data_size))
			{
				iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
				return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
			}
		}
		
		if (pState->m_central_dir.m_size >= IW_UINT32_MAX)
		{
			iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
			return iw_zip_set_error(pZip, IW_ZIP_UNSUPPORTED_CDIR_SIZE);
		}
		
		n = (iw_uint32)orig_central_dir_size;
		if (!iw_zip_array_push_back(pZip, &pState->m_central_dir_offsets, &n, 1))
		{
			iw_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size, IW_FALSE);
			return iw_zip_set_error(pZip, IW_ZIP_ALLOC_FAILED);
		}
		
		pZip->m_total_files++;
		pZip->m_archive_size = cur_dst_file_ofs;
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_finalize_archive(iw_zip_archive *pZip)
	{
		iw_zip_internal_state *pState;
		iw_uint64 central_dir_ofs, central_dir_size;
		iw_uint8 hdr[256];
		
		if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != IW_ZIP_MODE_WRITING))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		pState = pZip->m_pState;
		
		if (pState->m_zip64)
		{
			if ((iw_uint64)pState->m_central_dir.m_size >= IW_UINT32_MAX)
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		else
		{
			if ((pZip->m_total_files > IW_UINT16_MAX) || ((pZip->m_archive_size + pState->m_central_dir.m_size + IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE) > IW_UINT32_MAX))
				return iw_zip_set_error(pZip, IW_ZIP_TOO_MANY_FILES);
		}
		
		central_dir_ofs = 0;
		central_dir_size = 0;
		if (pZip->m_total_files)
		{
			central_dir_ofs = pZip->m_archive_size;
			central_dir_size = pState->m_central_dir.m_size;
			pZip->m_central_directory_file_ofs = central_dir_ofs;
			if (pZip->m_pWrite(pZip->m_pIO_opaque, central_dir_ofs, pState->m_central_dir.m_p, (size_t)central_dir_size) != central_dir_size)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			pZip->m_archive_size += central_dir_size;
		}
		
		if (pState->m_zip64)
		{
			iw_uint64 rel_ofs_to_zip64_ecdr = pZip->m_archive_size;
			
			IW_CLEAR_ARR(hdr);
			IW_WRITE_LE32(hdr + IW_ZIP64_ECDH_SIG_OFS, IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIG);
			IW_WRITE_LE64(hdr + IW_ZIP64_ECDH_SIZE_OF_RECORD_OFS, IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE - sizeof(iw_uint32) - sizeof(iw_uint64));
			IW_WRITE_LE16(hdr + IW_ZIP64_ECDH_VERSION_MADE_BY_OFS, 0x031E);
			IW_WRITE_LE16(hdr + IW_ZIP64_ECDH_VERSION_NEEDED_OFS, 0x002D);
			IW_WRITE_LE64(hdr + IW_ZIP64_ECDH_CDIR_NUM_ENTRIES_ON_DISK_OFS, pZip->m_total_files);
			IW_WRITE_LE64(hdr + IW_ZIP64_ECDH_CDIR_TOTAL_ENTRIES_OFS, pZip->m_total_files);
			IW_WRITE_LE64(hdr + IW_ZIP64_ECDH_CDIR_SIZE_OFS, central_dir_size);
			IW_WRITE_LE64(hdr + IW_ZIP64_ECDH_CDIR_OFS_OFS, central_dir_ofs);
			if (pZip->m_pWrite(pZip->m_pIO_opaque, pZip->m_archive_size, hdr, IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE) != IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			pZip->m_archive_size += IW_ZIP64_END_OF_CENTRAL_DIR_HEADER_SIZE;
			
			IW_CLEAR_ARR(hdr);
			IW_WRITE_LE32(hdr + IW_ZIP64_ECDL_SIG_OFS, IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIG);
			IW_WRITE_LE64(hdr + IW_ZIP64_ECDL_REL_OFS_TO_ZIP64_ECDR_OFS, rel_ofs_to_zip64_ecdr);
			IW_WRITE_LE32(hdr + IW_ZIP64_ECDL_TOTAL_NUMBER_OF_DISKS_OFS, 1);
			if (pZip->m_pWrite(pZip->m_pIO_opaque, pZip->m_archive_size, hdr, IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE) != IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE)
				return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
			
			pZip->m_archive_size += IW_ZIP64_END_OF_CENTRAL_DIR_LOCATOR_SIZE;
		}
		
		IW_CLEAR_ARR(hdr);
		IW_WRITE_LE32(hdr + IW_ZIP_ECDH_SIG_OFS, IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIG);
		IW_WRITE_LE16(hdr + IW_ZIP_ECDH_CDIR_NUM_ENTRIES_ON_DISK_OFS, IW_MIN(IW_UINT16_MAX, pZip->m_total_files));
		IW_WRITE_LE16(hdr + IW_ZIP_ECDH_CDIR_TOTAL_ENTRIES_OFS, IW_MIN(IW_UINT16_MAX, pZip->m_total_files));
		IW_WRITE_LE32(hdr + IW_ZIP_ECDH_CDIR_SIZE_OFS, IW_MIN(IW_UINT32_MAX, central_dir_size));
		IW_WRITE_LE32(hdr + IW_ZIP_ECDH_CDIR_OFS_OFS, IW_MIN(IW_UINT32_MAX, central_dir_ofs));
		
		if (pZip->m_pWrite(pZip->m_pIO_opaque, pZip->m_archive_size, hdr, IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE) != IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE)
			return iw_zip_set_error(pZip, IW_ZIP_FILE_WRITE_FAILED);
		
#ifndef i2WC_NO_STDIO
		if ((pState->m_pFile) && (IW_FFLUSH(pState->m_pFile) == EOF))
			return iw_zip_set_error(pZip, IW_ZIP_FILE_CLOSE_FAILED);
#endif
		
		pZip->m_archive_size += IW_ZIP_END_OF_CENTRAL_DIR_HEADER_SIZE;
		
		pZip->m_zip_mode = IW_ZIP_MODE_WRITING_HAS_BEEN_FINALIZED;
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_finalize_heap_archive(iw_zip_archive *pZip, void **ppBuf, size_t *pSize)
	{
		if ((!ppBuf) || (!pSize))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		*ppBuf = NULL;
		*pSize = 0;
		
		if ((!pZip) || (!pZip->m_pState))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (pZip->m_pWrite != iw_zip_heap_write_func)
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		if (!iw_zip_writer_finalize_archive(pZip))
			return IW_FALSE;
		
		*ppBuf = pZip->m_pState->m_pMem;
		*pSize = pZip->m_pState->m_mem_size;
		pZip->m_pState->m_pMem = NULL;
		pZip->m_pState->m_mem_size = pZip->m_pState->m_mem_capacity = 0;
		
		return IW_TRUE;
	}
	
	iw_bool iw_zip_writer_end(iw_zip_archive *pZip)
	{
		return iw_zip_writer_end_internal(pZip, IW_TRUE);
	}
	
#ifndef i2WC_NO_STDIO
	iw_bool iw_zip_add_mem_to_archive_file_in_place(const char *pZip_filename, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags)
	{
		return iw_zip_add_mem_to_archive_file_in_place_v2(pZip_filename, pArchive_name, pBuf, buf_size, pComment, comment_size, level_and_flags, NULL);
	}
	
	iw_bool iw_zip_add_mem_to_archive_file_in_place_v2(const char *pZip_filename, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, iw_uint16 comment_size, iw_uint level_and_flags, iw_zip_error *pErr)
	{
		iw_bool status, created_new_archive = IW_FALSE;
		iw_zip_archive zip_archive;
		struct IW_FILE_STAT_STRUCT file_stat;
		iw_zip_error actual_err = IW_ZIP_NO_ERROR;
		
		iw_zip_zero_struct(&zip_archive);
		if ((int)level_and_flags < 0)
			level_and_flags = IW_DEFAULT_LEVEL;
		
		if ((!pZip_filename) || (!pArchive_name) || ((buf_size) && (!pBuf)) || ((comment_size) && (!pComment)) || ((level_and_flags & 0xF) > IW_UBER_COMPRESSION))
		{
			if (pErr)
				*pErr = IW_ZIP_INVALID_PARAMETER;
			return IW_FALSE;
		}
		
		if (!iw_zip_writer_validate_archive_name(pArchive_name))
		{
			if (pErr)
				*pErr = IW_ZIP_INVALID_FILENAME;
			return IW_FALSE;
		}
		
		if (IW_FILE_STAT(pZip_filename, &file_stat) != 0)
		{
			if (!iw_zip_writer_init_file_v2(&zip_archive, pZip_filename, 0, level_and_flags))
			{
				if (pErr)
					*pErr = zip_archive.m_last_error;
				return IW_FALSE;
			}
			
			created_new_archive = IW_TRUE;
		}
		else
		{
			if (!iw_zip_reader_init_file_v2(&zip_archive, pZip_filename, level_and_flags | IW_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY | IW_ZIP_FLAG_READ_ALLOW_WRITING, 0, 0))
			{
				if (pErr)
					*pErr = zip_archive.m_last_error;
				return IW_FALSE;
			}
			
			if (!iw_zip_writer_init_from_reader_v2(&zip_archive, pZip_filename, level_and_flags | IW_ZIP_FLAG_READ_ALLOW_WRITING))
			{
				if (pErr)
					*pErr = zip_archive.m_last_error;
				
				iw_zip_reader_end_internal(&zip_archive, IW_FALSE);
				
				return IW_FALSE;
			}
		}
		
		status = iw_zip_writer_add_mem_ex(&zip_archive, pArchive_name, pBuf, buf_size, pComment, comment_size, level_and_flags, 0, 0);
		actual_err = zip_archive.m_last_error;
		
		if (!iw_zip_writer_finalize_archive(&zip_archive))
		{
			if (!actual_err)
				actual_err = zip_archive.m_last_error;
			
			status = IW_FALSE;
		}
		
		if (!iw_zip_writer_end_internal(&zip_archive, status))
		{
			if (!actual_err)
				actual_err = zip_archive.m_last_error;
			
			status = IW_FALSE;
		}
		
		if ((!status) && (created_new_archive))
		{
			int ignoredStatus = IW_DELETE_FILE(pZip_filename);
			(void)ignoredStatus;
		}
		
		if (pErr)
			*pErr = actual_err;
		
		return status;
	}
	
	void *iw_zip_extract_archive_file_to_heap_v2(const char *pZip_filename, const char *pArchive_name, const char *pComment, size_t *pSize, iw_uint flags, iw_zip_error *pErr)
	{
		iw_uint32 file_index;
		iw_zip_archive zip_archive;
		void *p = NULL;
		
		if (pSize)
			*pSize = 0;
		
		if ((!pZip_filename) || (!pArchive_name))
		{
			if (pErr)
				*pErr = IW_ZIP_INVALID_PARAMETER;
			
			return NULL;
		}
		
		iw_zip_zero_struct(&zip_archive);
		if (!iw_zip_reader_init_file_v2(&zip_archive, pZip_filename, flags | IW_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY, 0, 0))
		{
			if (pErr)
				*pErr = zip_archive.m_last_error;
			
			return NULL;
		}
		
		if (iw_zip_reader_locate_file_v2(&zip_archive, pArchive_name, pComment, flags, &file_index))
		{
			p = iw_zip_reader_extract_to_heap(&zip_archive, file_index, pSize, flags);
		}
		
		iw_zip_reader_end_internal(&zip_archive, p != NULL);
		
		if (pErr)
			*pErr = zip_archive.m_last_error;
		
		return p;
	}
	
	void *iw_zip_extract_archive_file_to_heap(const char *pZip_filename, const char *pArchive_name, size_t *pSize, iw_uint flags)
	{
		return iw_zip_extract_archive_file_to_heap_v2(pZip_filename, pArchive_name, NULL, pSize, flags, NULL);
	}
	
#endif
	
#endif
	
	iw_zip_mode iw_zip_get_mode(iw_zip_archive *pZip)
	{
		return pZip ? pZip->m_zip_mode : IW_ZIP_MODE_INVALID;
	}
	
	iw_zip_type iw_zip_get_type(iw_zip_archive *pZip)
	{
		return pZip ? pZip->m_zip_type : IW_ZIP_TYPE_INVALID;
	}
	
	iw_zip_error iw_zip_set_last_error(iw_zip_archive *pZip, iw_zip_error err_num)
	{
		iw_zip_error prev_err;
		
		if (!pZip)
			return IW_ZIP_INVALID_PARAMETER;
		
		prev_err = pZip->m_last_error;
		
		pZip->m_last_error = err_num;
		return prev_err;
	}
	
	iw_zip_error iw_zip_peek_last_error(iw_zip_archive *pZip)
	{
		if (!pZip)
			return IW_ZIP_INVALID_PARAMETER;
		
		return pZip->m_last_error;
	}
	
	iw_zip_error iw_zip_clear_last_error(iw_zip_archive *pZip)
	{
		return iw_zip_set_last_error(pZip, IW_ZIP_NO_ERROR);
	}
	
	iw_zip_error iw_zip_get_last_error(iw_zip_archive *pZip)
	{
		iw_zip_error prev_err;
		
		if (!pZip)
			return IW_ZIP_INVALID_PARAMETER;
		
		prev_err = pZip->m_last_error;
		
		pZip->m_last_error = IW_ZIP_NO_ERROR;
		return prev_err;
	}
	
	const char *iw_zip_get_error_string(iw_zip_error iw_err)
	{
		switch (iw_err)
		{
		case IW_ZIP_NO_ERROR:
			return "no error";
		case IW_ZIP_UNDEFINED_ERROR:
			return "undefined error";
		case IW_ZIP_TOO_MANY_FILES:
			return "too many files";
		case IW_ZIP_FILE_TOO_LARGE:
			return "file too large";
		case IW_ZIP_UNSUPPORTED_METHOD:
			return "unsupported method";
		case IW_ZIP_UNSUPPORTED_ENCRYPTION:
			return "unsupported encryption";
		case IW_ZIP_UNSUPPORTED_FEATURE:
			return "unsupported feature";
		case IW_ZIP_FAILED_FINDING_CENTRAL_DIR:
			return "failed finding central directory";
		case IW_ZIP_NOT_AN_ARCHIVE:
			return "not a ZIP archive";
		case IW_ZIP_INVALID_HEADER_OR_CORRUPTED:
			return "invalid header or archive is corrupted";
		case IW_ZIP_UNSUPPORTED_MULTIDISK:
			return "unsupported multidisk archive";
		case IW_ZIP_DECOMPRESSION_FAILED:
			return "decompression failed or archive is corrupted";
		case IW_ZIP_COMPRESSION_FAILED:
			return "compression failed";
		case IW_ZIP_UNEXPECTED_DECOMPRESSED_SIZE:
			return "unexpected decompressed size";
		case IW_ZIP_CRC_CHECK_FAILED:
			return "CRC-32 check failed";
		case IW_ZIP_UNSUPPORTED_CDIR_SIZE:
			return "unsupported central directory size";
		case IW_ZIP_ALLOC_FAILED:
			return "allocation failed";
		case IW_ZIP_FILE_OPEN_FAILED:
			return "file open failed";
		case IW_ZIP_FILE_CREATE_FAILED:
			return "file create failed";
		case IW_ZIP_FILE_WRITE_FAILED:
			return "file write failed";
		case IW_ZIP_FILE_READ_FAILED:
			return "file read failed";
		case IW_ZIP_FILE_CLOSE_FAILED:
			return "file close failed";
		case IW_ZIP_FILE_SEEK_FAILED:
			return "file seek failed";
		case IW_ZIP_FILE_STAT_FAILED:
			return "file stat failed";
		case IW_ZIP_INVALID_PARAMETER:
			return "invalid parameter";
		case IW_ZIP_INVALID_FILENAME:
			return "invalid filename";
		case IW_ZIP_BUF_TOO_SMALL:
			return "buffer too small";
		case IW_ZIP_INTERNAL_ERROR:
			return "internal error";
		case IW_ZIP_FILE_NOT_FOUND:
			return "file not found";
		case IW_ZIP_ARCHIVE_TOO_LARGE:
			return "archive is too large";
		case IW_ZIP_VALIDATION_FAILED:
			return "validation failed";
		case IW_ZIP_WRITE_CALLBACK_FAILED:
			return "write callback failed";
		case IW_ZIP_TOTAL_ERRORS:
			return "total errors";
		default:
			break;
		}
		
		return "unknown error";
	}
	
	iw_bool iw_zip_is_zip64(iw_zip_archive *pZip)
	{
		if ((!pZip) || (!pZip->m_pState))
			return IW_FALSE;
		
		return pZip->m_pState->m_zip64;
	}
	
	size_t iw_zip_get_central_dir_size(iw_zip_archive *pZip)
	{
		if ((!pZip) || (!pZip->m_pState))
			return 0;
		
		return pZip->m_pState->m_central_dir.m_size;
	}
	
	iw_uint iw_zip_reader_get_num_files(iw_zip_archive *pZip)
	{
		return pZip ? pZip->m_total_files : 0;
	}
	
	iw_uint64 iw_zip_get_archive_size(iw_zip_archive *pZip)
	{
		if (!pZip)
			return 0;
		return pZip->m_archive_size;
	}
	
	iw_uint64 iw_zip_get_archive_file_start_offset(iw_zip_archive *pZip)
	{
		if ((!pZip) || (!pZip->m_pState))
			return 0;
		return pZip->m_pState->m_file_archive_start_ofs;
	}
	
	IW_FILE *iw_zip_get_cfile(iw_zip_archive *pZip)
	{
		if ((!pZip) || (!pZip->m_pState))
			return 0;
		return pZip->m_pState->m_pFile;
	}
	
	size_t iw_zip_read_archive_data(iw_zip_archive *pZip, iw_uint64 file_ofs, void *pBuf, size_t n)
	{
		if ((!pZip) || (!pZip->m_pState) || (!pBuf) || (!pZip->m_pRead))
			return iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
		
		return pZip->m_pRead(pZip->m_pIO_opaque, file_ofs, pBuf, n);
	}
	
	iw_uint iw_zip_reader_get_filename(iw_zip_archive *pZip, iw_uint file_index, char *pFilename, iw_uint filename_buf_size)
	{
		iw_uint n;
		const iw_uint8 *p = iw_zip_get_cdh(pZip, file_index);
		if (!p)
		{
			if (filename_buf_size)
				pFilename[0] = '\0';
			iw_zip_set_error(pZip, IW_ZIP_INVALID_PARAMETER);
			return 0;
		}
		n = IW_READ_LE16(p + IW_ZIP_CDH_FILENAME_LEN_OFS);
		if (filename_buf_size)
		{
			n = IW_MIN(n, filename_buf_size - 1);
			memcpy(pFilename, p + IW_ZIP_CENTRAL_DIR_HEADER_SIZE, n);
			pFilename[n] = '\0';
		}
		return n + 1;
	}
	
	iw_bool iw_zip_reader_file_stat(iw_zip_archive *pZip, iw_uint file_index, iw_zip_archive_file_stat *pStat)
	{
		return iw_zip_file_stat_internal(pZip, file_index, iw_zip_get_cdh(pZip, file_index), pStat, NULL);
	}
	
	iw_bool iw_zip_end(iw_zip_archive *pZip)
	{
		if (!pZip)
			return IW_FALSE;
		
		if (pZip->m_zip_mode == IW_ZIP_MODE_READING)
			return iw_zip_reader_end(pZip);
#ifndef i2WC_NO_ARCHIVE_WRITING_APIS
		else if ((pZip->m_zip_mode == IW_ZIP_MODE_WRITING) || (pZip->m_zip_mode == IW_ZIP_MODE_WRITING_HAS_BEEN_FINALIZED))
			return iw_zip_writer_end(pZip);
#endif
		
		return IW_FALSE;
	}
	
#ifdef __cplusplus
}
#endif

namespace i2wC {
	
	enum class Error {
		OK = 0,
		INVALID_PARAMETER,
		BUFFER_TOO_SMALL,
		COMPRESSION_FAILED,
		DECOMPRESSION_FAILED,
		CRC_MISMATCH,
		VERSION_MISMATCH,
		MAGIC_MISMATCH,
		FILE_READ_FAILED,
		FILE_WRITE_FAILED,
		OUT_OF_MEMORY,
		SECTION_NOT_FOUND,
		UNKNOWN_SECTION_TYPE,
		INTERNAL_ERROR,
		ZIP_BOMB_DETECTED
	};
	
	struct AdvancedConfig {
		int lzHashBits = 14;
		size_t lzDictSize = 65536;
		int minMatchLen = 4;
		int maxMatchLen = 260;
		int fastLookupBits = 11;
		uint32_t adler32Init = 0x2C57;
		size_t adler32Chunk = 4444;
		size_t zipMaxIOBufSize = 128 * 1024;
		uint8_t zlibCMF = 0x68;
		uint8_t zlibFLevel = 2;
		iw_uint probes[11] = { 0, 2, 8, 48, 24, 48, 192, 384, 768, 1024, 2048 };
		
		static AdvancedConfig defaultConfig() { return AdvancedConfig(); }
		
		AdvancedConfig& setLZHashBits(int bits) { lzHashBits = bits; return *this; }
		AdvancedConfig& setLZDictSize(size_t size) { lzDictSize = size; return *this; }
		AdvancedConfig& setMinMatchLen(int len) { minMatchLen = len; return *this; }
		AdvancedConfig& setMaxMatchLen(int len) { maxMatchLen = len; return *this; }
		AdvancedConfig& setFastLookupBits(int bits) { fastLookupBits = bits; return *this; }
		AdvancedConfig& setAdler32Init(uint32_t init) { adler32Init = init; return *this; }
		AdvancedConfig& setAdler32Chunk(size_t chunk) { adler32Chunk = chunk; return *this; }
		AdvancedConfig& setZipMaxIOBufSize(size_t size) { zipMaxIOBufSize = size; return *this; }
		AdvancedConfig& setZlibCMF(uint8_t cmf) { zlibCMF = cmf; return *this; }
		AdvancedConfig& setZlibFLevel(uint8_t level) { zlibFLevel = level; return *this; }
		AdvancedConfig& setProbeLevel(int level, iw_uint probes) {
			if (level >= 0 && level <= 10) this->probes[level] = probes;
			return *this;
		}
		AdvancedConfig& setAllProbes(const iw_uint p[11]) {
			for (int i = 0; i < 11; ++i) probes[i] = p[i];
			return *this;
		}
	};
	
	namespace internal {
		
		inline const iw_uint* getProbesArray(const AdvancedConfig* config = nullptr) {
			static const iw_uint defaultProbes[11] = { 0, 2, 8, 48, 24, 48, 192, 384, 768, 1024, 2048 };
			return config ? config->probes : defaultProbes;
		}
		
		inline iw_uint getProbeForLevel(int level, const AdvancedConfig* config = nullptr) {
			const iw_uint* probes = getProbesArray(config);
			if (level < 0) level = 0;
			if (level > 10) level = 10;
			return probes[level];
		}
		
		inline int getLZHashBits(const AdvancedConfig* config = nullptr) {
			return config ? config->lzHashBits : 14;
		}
		
		inline size_t getLZDictSize(const AdvancedConfig* config = nullptr) {
			return config ? config->lzDictSize : 65536;
		}
		
		inline int getMinMatchLen(const AdvancedConfig* config = nullptr) {
			return config ? config->minMatchLen : 4;
		}
		
		inline int getMaxMatchLen(const AdvancedConfig* config = nullptr) {
			return config ? config->maxMatchLen : 260;
		}
		
		inline int getFastLookupBits(const AdvancedConfig* config = nullptr) {
			return config ? config->fastLookupBits : 11;
		}
		
		inline uint32_t getAdler32Init(const AdvancedConfig* config = nullptr) {
			return config ? config->adler32Init : 0x2C57;
		}
		
		inline size_t getAdler32Chunk(const AdvancedConfig* config = nullptr) {
			return config ? config->adler32Chunk : 4444;
		}
		
		inline size_t getZipMaxIOBufSize(const AdvancedConfig* config = nullptr) {
			return config ? config->zipMaxIOBufSize : 128 * 1024;
		}
		
		inline uint8_t getZlibCMF(const AdvancedConfig* config = nullptr) {
			return config ? config->zlibCMF : 0x68;
		}
		
		inline uint8_t getZlibFLevel(const AdvancedConfig* config = nullptr) {
			return config ? config->zlibFLevel : 2;
		}
		
	}
	
	class ErrorHandler {
	public:
		using ErrorCallback = void(*)(Error error, const char* message, void* userData);
		
		static ErrorHandler& instance() {
			static ErrorHandler handler;
			return handler;
		}
		
		void setCallback(ErrorCallback callback, void* userData = nullptr) {
			callback_ = callback;
			userData_ = userData;
		}
		
		void report(Error error, const char* message) {
			lastError_ = error;
			lastMessage_ = message;
			if (callback_) {
				callback_(error, message, userData_);
			}
		}
		
		Error getLastError() const { return lastError_; }
		const char* getLastMessage() const { return lastMessage_; }
		
		void clear() {
			lastError_ = Error::OK;
			lastMessage_ = nullptr;
		}
		
		static const char* errorString(Error error) {
			switch (error) {
				case Error::OK: return "OK";
				case Error::INVALID_PARAMETER: return "Invalid parameter";
				case Error::BUFFER_TOO_SMALL: return "Buffer too small";
				case Error::COMPRESSION_FAILED: return "Compression failed";
				case Error::DECOMPRESSION_FAILED: return "Decompression failed";
				case Error::CRC_MISMATCH: return "CRC mismatch";
				case Error::VERSION_MISMATCH: return "Version mismatch";
				case Error::MAGIC_MISMATCH: return "Magic number mismatch";
				case Error::FILE_READ_FAILED: return "File read failed";
				case Error::FILE_WRITE_FAILED: return "File write failed";
				case Error::OUT_OF_MEMORY: return "Out of memory";
				case Error::SECTION_NOT_FOUND: return "Section not found";
				case Error::UNKNOWN_SECTION_TYPE: return "Unknown section type";
				case Error::INTERNAL_ERROR: return "Internal error";
				case Error::ZIP_BOMB_DETECTED: return "Zip bomb detected";
				default: return "Unknown error";
			}
		}
		
	private:
		ErrorHandler() : callback_(nullptr), userData_(nullptr), lastError_(Error::OK), lastMessage_(nullptr) {}
		ErrorCallback callback_;
		void* userData_;
		Error lastError_;
		const char* lastMessage_;
	};
	
	class Result {
	public:
		Result() : error_(Error::OK) {}
		Result(Error err) : error_(err) {}
		Result(Error err, const std::string& msg) : error_(err), message_(msg) {}
		
		bool ok() const { return error_ == Error::OK; }
		operator bool() const { return error_ == Error::OK; }
		
		Error error() const { return error_; }
		const std::string& message() const { return message_; }
		
	private:
		Error error_;
		std::string message_;
	};
	
	struct DecompressConfig {
		size_t maxOutputSize = 100 * 1024 * 1024;
		size_t initialBufferSize = 0;
		size_t expansionFactor = 2;
		size_t maxExpansions = 20;
		size_t maxCompressionRatio = 100;
	};
	
	class DecompressGuard {
	public:
		DecompressGuard(const DecompressConfig& config = DecompressConfig())
		: config_(config), bytesProduced_(0), expansionCount_(0) {}
		
		bool checkExpansion(size_t newSize) {
			if (newSize > config_.maxOutputSize) {
				char msg[128];
				std::snprintf(msg, sizeof(msg), "Decompression size %zu exceeds limit %zu", newSize, config_.maxOutputSize);
				ErrorHandler::instance().report(Error::ZIP_BOMB_DETECTED, msg);
				return false;
			}
			expansionCount_++;
			if (expansionCount_ > config_.maxExpansions) {
				ErrorHandler::instance().report(Error::ZIP_BOMB_DETECTED, "Too many buffer expansions");
				return false;
			}
			return true;
		}
		
		bool checkRatio(size_t compressedSize, size_t decompressedSize) {
			if (compressedSize == 0) return true;
			size_t ratio = decompressedSize / compressedSize;
			if (ratio > config_.maxCompressionRatio) {
				char msg[128];
				std::snprintf(msg, sizeof(msg), "Compression ratio %zu:1 exceeds limit %zu:1", ratio, config_.maxCompressionRatio);
				ErrorHandler::instance().report(Error::ZIP_BOMB_DETECTED, msg);
				return false;
			}
			return true;
		}
		
		void addBytes(size_t bytes) { bytesProduced_ += bytes; }
		size_t getInitialSize(size_t inputSize) const { return config_.initialBufferSize > 0 ? config_.initialBufferSize : inputSize * 4; }
		size_t getExpandedSize(size_t currentSize) const { return currentSize * config_.expansionFactor; }
		const DecompressConfig& config() const { return config_; }
		
	private:
		DecompressConfig config_;
		size_t bytesProduced_;
		size_t expansionCount_;
	};
	
	static inline Result compress(const void* data, size_t size, std::vector<unsigned char>& out, 
								  int level = 6, const AdvancedConfig* config = nullptr)
	{
		if (!data || size == 0) {
			ErrorHandler::instance().report(Error::INVALID_PARAMETER, "compress: null data or zero size");
			return Error::INVALID_PARAMETER;
		}
		
		(void)config;
		
		iw_ulong destLen = iw_compressBound((iw_ulong)size);
		out.resize(destLen);
		int ret = iw_compress2(out.data(), &destLen, (const unsigned char*)data, (iw_ulong)size, level);
		if (ret != IW_OK) {
			if (ret == IW_MEM_ERROR) {
				ErrorHandler::instance().report(Error::OUT_OF_MEMORY, "compress: out of memory");
				return Error::OUT_OF_MEMORY;
			}
			ErrorHandler::instance().report(Error::COMPRESSION_FAILED, "compress: compression failed");
			return Error::COMPRESSION_FAILED;
		}
		out.resize(destLen);
		ErrorHandler::instance().clear();
		return Error::OK;
	}
	
	static inline Result uncompress(const void* data, size_t size, std::vector<unsigned char>& out,
									const DecompressConfig& config = DecompressConfig())
	{
		if (!data || size == 0) {
			ErrorHandler::instance().report(Error::INVALID_PARAMETER, "uncompress: null data or zero size");
			return Error::INVALID_PARAMETER;
		}
		DecompressGuard guard(config);
		size_t destLen = guard.getInitialSize(size);
		if (!guard.checkExpansion(destLen)) return Error::ZIP_BOMB_DETECTED;
		out.resize(destLen);
		int ret = iw_uncompress(out.data(), (iw_ulong*)&destLen, (const unsigned char*)data, (iw_ulong)size);
		while (ret == IW_BUF_ERROR) {
			size_t newSize = guard.getExpandedSize(destLen);
			if (!guard.checkExpansion(newSize)) {
				out.clear();
				return Error::ZIP_BOMB_DETECTED;
			}
			destLen = newSize;
			out.resize(destLen);
			ret = iw_uncompress(out.data(), (iw_ulong*)&destLen, (const unsigned char*)data, (iw_ulong)size);
		}
		if (ret != IW_OK) {
			if (ret == IW_MEM_ERROR) {
				ErrorHandler::instance().report(Error::OUT_OF_MEMORY, "uncompress: out of memory");
				return Error::OUT_OF_MEMORY;
			}
			if (ret == IW_DATA_ERROR) {
				ErrorHandler::instance().report(Error::DECOMPRESSION_FAILED, "uncompress: corrupted data");
				return Error::DECOMPRESSION_FAILED;
			}
			ErrorHandler::instance().report(Error::DECOMPRESSION_FAILED, "uncompress: decompression failed");
			return Error::DECOMPRESSION_FAILED;
		}
		out.resize(destLen);
		guard.addBytes(destLen);
		ErrorHandler::instance().clear();
		return Error::OK;
	}
	
	static inline std::vector<unsigned char> compress(const std::vector<unsigned char>& in, int level = 6)
	{
		std::vector<unsigned char> out;
		compress(in.data(), in.size(), out, level);
		return out;
	}
	
	static inline std::vector<unsigned char> uncompress(const std::vector<unsigned char>& in)
	{
		std::vector<unsigned char> out;
		uncompress(in.data(), in.size(), out);
		return out;
	}
	
	class Compressor {
	public:
		
		explicit Compressor(int level = 6) : level_(clampLevel(level)) {}
		
		void setLevel(int level) { level_ = clampLevel(level); }
		int getLevel() const { return level_; }
		void setAdvancedConfig(const AdvancedConfig* config) { config_ = config; }
		const AdvancedConfig* getAdvancedConfig() const { return config_; }
		
		Result compress(const void* data, size_t size, std::vector<unsigned char>& out) {
			return i2wC::compress(data, size, out, level_);
		}
		
		Result compress(const std::vector<unsigned char>& input, std::vector<unsigned char>& out) {
			return i2wC::compress(input.data(), input.size(), out, level_);
		}
		
		std::vector<unsigned char> compress(const std::vector<unsigned char>& input) {
			std::vector<unsigned char> out;
			i2wC::compress(input.data(), input.size(), out, level_);
			return out;
		}
		
		Result uncompress(const void* data, size_t size, std::vector<unsigned char>& out) {
			return i2wC::uncompress(data, size, out);
		}
		
		Result uncompress(const std::vector<unsigned char>& input, std::vector<unsigned char>& out) {
			return i2wC::uncompress(input.data(), input.size(), out);
		}
		
		std::vector<unsigned char> uncompress(const std::vector<unsigned char>& input) {
			std::vector<unsigned char> out;
			i2wC::uncompress(input.data(), input.size(), out);
			return out;
		}
		
		static bool isValidLevel(int level) { return level >= 0 && level <= 10; }
		
		static const char* getLevelName(int level) {
			switch (clampLevel(level)) {
				case 0:  return "No Compression";
				case 1:  return "Fastest";
			case 2:
			case 3:
			case 4:
				case 5:  return "Fast";
				case 6:  return "Default";
			case 7:
				case 8:  return "Better";
				case 9:  return "Best";
				case 10: return "Uber";
				default: return "Unknown";
			}
		}
		
	private:
		int level_;
		const AdvancedConfig* config_;
		DecompressConfig decompressConfig_;
		
		static int clampLevel(int level) { return level < 0 ? 0 : (level > 10 ? 10 : level); }
	};
	
	class BinaryFile {
	public:
		static constexpr uint32_t DEFAULT_MAGIC_HEADER = 0x43325749;
		static constexpr uint32_t DEFAULT_MAGIC_FOOTER = 0x49573243;
		static constexpr uint16_t CURRENT_VERSION = 0x0100;
		static constexpr uint16_t FLAG_COMPRESSED = 0x0001;
		
		enum class SectionType : uint8_t {
			IDENTIFIER = 0x01,
			CONTENT    = 0x02,
			CUSTOM     = 0x03
		};
		
#pragma pack(push, 1)
		struct Header {
	uint32_t magic;
	uint16_t version;
	uint16_t flags;
	uint32_t sectionCount;
	uint8_t  reserved[4];
	Header() : magic(DEFAULT_MAGIC_HEADER), version(CURRENT_VERSION), flags(0), sectionCount(0) {
		std::memset(reserved, 0, sizeof(reserved));
	}
};
		
		struct SectionEntry {
			SectionType type;
			uint8_t     reserved[3];
			uint64_t    offset;
			uint64_t    size;
			uint32_t    checksum;
			SectionEntry() : type(SectionType::CUSTOM), offset(0), size(0), checksum(0) {
				std::memset(reserved, 0, sizeof(reserved));
			}
			SectionEntry(SectionType t, uint64_t off, uint64_t sz, uint32_t chk = 0)
			: type(t), offset(off), size(sz), checksum(chk) {
				std::memset(reserved, 0, sizeof(reserved));
			}
		};
		
		struct Footer {
			uint32_t checksum;
			uint32_t magicEnd;
			Footer() : checksum(0), magicEnd(DEFAULT_MAGIC_FOOTER) {}
		};
#pragma pack(pop)
		
		static_assert(sizeof(Header) == 16, "Header must be 16 bytes");
		static_assert(sizeof(SectionEntry) == 24, "SectionEntry must be 24 bytes");
		static_assert(sizeof(Footer) == 8, "Footer must be 8 bytes");
		
		BinaryFile() : compressor_(IW_DEFAULT_LEVEL) { header_.flags = FLAG_COMPRESSED; }
		explicit BinaryFile(int compressionLevel) : compressor_(compressionLevel) { header_.flags = FLAG_COMPRESSED; }
		
		void setHeaderMagic(uint32_t magic) { header_.magic = magic; }
		uint32_t getHeaderMagic() const { return header_.magic; }
		
		void setFooterMagic(uint32_t magic) { footerMagic_ = magic; }
		uint32_t getFooterMagic() const { return footerMagic_; }
		
		void setVersion(uint16_t version) { header_.version = version; }
		uint16_t getVersion() const { return header_.version; }
		
		void setDecompressConfig(const DecompressConfig& config) { decompressConfig_ = config; }
		const DecompressConfig& getDecompressConfig() const { return decompressConfig_; }
		
		Result setIdentifier(const std::vector<unsigned char>& data) { identifierData_ = data; return Error::OK; }
		Result getIdentifier(std::vector<unsigned char>& out) const { out = identifierData_; return Error::OK; }
		
		Result setContent(const std::vector<unsigned char>& data) { contentData_ = data; return Error::OK; }
		Result getContent(std::vector<unsigned char>& out) const { out = contentData_; return Error::OK; }
		
		size_t addCustomSection(const std::vector<unsigned char>& data) {
			customSections_.push_back(data);
			return customSections_.size() - 1;
		}
		
		Result getCustomSection(size_t index, std::vector<unsigned char>& out) const {
			if (index >= customSections_.size()) {
				char msg[64];
				std::snprintf(msg, sizeof(msg), "Index %zu out of range (max %zu)", index, customSections_.size());
				ErrorHandler::instance().report(Error::SECTION_NOT_FOUND, msg);
				return Error::SECTION_NOT_FOUND;
			}
			out = customSections_[index];
			return Error::OK;
		}
		
		size_t getCustomSectionCount() const { return customSections_.size(); }
		
		void clear() {
			identifierData_.clear();
			contentData_.clear();
			customSections_.clear();
		}
		
		void setCompressionLevel(int level) { compressor_.setLevel(level); }
		int getCompressionLevel() const { return compressor_.getLevel(); }
		
		void setCompressionEnabled(bool enabled) {
			if (enabled) header_.flags |= FLAG_COMPRESSED;
			else header_.flags &= ~FLAG_COMPRESSED;
		}
		
		bool isCompressionEnabled() const { return (header_.flags & FLAG_COMPRESSED) != 0; }
		
		Result serialize(std::vector<unsigned char>& out) {
			std::vector<unsigned char> result;
			uint32_t sectionCount = 0;
			if (!identifierData_.empty()) sectionCount++;
			if (!contentData_.empty()) sectionCount++;
			sectionCount += static_cast<uint32_t>(customSections_.size());
			header_.sectionCount = sectionCount;
			
			std::vector<SectionEntry> entries;
			uint64_t currentOffset = sizeof(Header) + sectionCount * sizeof(SectionEntry);
			
			if (!identifierData_.empty()) {
				entries.emplace_back(SectionType::IDENTIFIER, currentOffset, identifierData_.size(), crc32(identifierData_));
				currentOffset += identifierData_.size();
			}
			
			if (!contentData_.empty()) {
				std::vector<unsigned char> dataToWrite;
				if (header_.flags & FLAG_COMPRESSED) {
					Result r = compressor_.compress(contentData_, dataToWrite);
					if (!r) {
						ErrorHandler::instance().report(Error::COMPRESSION_FAILED, "serialize: content compression failed");
						return Error::COMPRESSION_FAILED;
					}
				} else {
					dataToWrite = contentData_;
				}
				entries.emplace_back(SectionType::CONTENT, currentOffset, dataToWrite.size(), crc32(dataToWrite));
				compressedContentCache_ = std::move(dataToWrite);
				currentOffset += compressedContentCache_.size();
			}
			
			for (const auto& data : customSections_) {
				entries.emplace_back(SectionType::CUSTOM, currentOffset, data.size(), crc32(data));
				currentOffset += data.size();
			}
			
			result.insert(result.end(), reinterpret_cast<unsigned char*>(&header_), reinterpret_cast<unsigned char*>(&header_) + sizeof(Header));
			for (const auto& entry : entries) {
				result.insert(result.end(), reinterpret_cast<const unsigned char*>(&entry), reinterpret_cast<const unsigned char*>(&entry) + sizeof(SectionEntry));
			}
			if (!identifierData_.empty()) result.insert(result.end(), identifierData_.begin(), identifierData_.end());
			if (!contentData_.empty()) result.insert(result.end(), compressedContentCache_.begin(), compressedContentCache_.end());
			for (const auto& data : customSections_) result.insert(result.end(), data.begin(), data.end());
			
			Footer footer;
			footer.checksum = crc32(result);
			footer.magicEnd = footerMagic_;
			result.insert(result.end(), reinterpret_cast<unsigned char*>(&footer), reinterpret_cast<unsigned char*>(&footer) + sizeof(Footer));
			out = std::move(result);
			return Error::OK;
		}
		
		std::vector<unsigned char> serialize() {
			std::vector<unsigned char> out;
			serialize(out);
			return out;
		}
		
		Result deserialize(const void* data, size_t size) {
			const unsigned char* ptr = static_cast<const unsigned char*>(data);
			const unsigned char* end = ptr + size;
			
			if (size < sizeof(Header) + sizeof(Footer)) {
				ErrorHandler::instance().report(Error::INVALID_PARAMETER, "deserialize: data too small");
				return Error::INVALID_PARAMETER;
			}
			
			std::memcpy(&header_, ptr, sizeof(Header));
			ptr += sizeof(Header);
			
			if (header_.magic != DEFAULT_MAGIC_HEADER) {
				ErrorHandler::instance().report(Error::MAGIC_MISMATCH, "deserialize: invalid header magic");
				return Error::MAGIC_MISMATCH;
			}
			
			if (header_.version != CURRENT_VERSION) {
				char msg[64];
				std::snprintf(msg, sizeof(msg), "Version mismatch (file v%u.%u, library v%u.%u)",
							  (header_.version >> 8) & 0xFF, header_.version & 0xFF,
							  (CURRENT_VERSION >> 8) & 0xFF, CURRENT_VERSION & 0xFF);
				ErrorHandler::instance().report(Error::VERSION_MISMATCH, msg);
			}
			
			if (ptr + header_.sectionCount * sizeof(SectionEntry) > end) {
				ErrorHandler::instance().report(Error::INVALID_PARAMETER, "deserialize: section table exceeds data size");
				return Error::INVALID_PARAMETER;
			}
			
			std::vector<SectionEntry> entries(header_.sectionCount);
			for (uint32_t i = 0; i < header_.sectionCount; ++i) {
				std::memcpy(&entries[i], ptr, sizeof(SectionEntry));
				ptr += sizeof(SectionEntry);
			}
			
			clear();
			
			for (const auto& entry : entries) {
				if (ptr + entry.size > end) {
					ErrorHandler::instance().report(Error::INVALID_PARAMETER, "deserialize: section data exceeds data size");
					return Error::INVALID_PARAMETER;
				}
				
				uint32_t calcChecksum = crc32(ptr, entry.size);
				if (calcChecksum != entry.checksum) {
					ErrorHandler::instance().report(Error::CRC_MISMATCH, "deserialize: section checksum mismatch");
				}
				
				std::vector<unsigned char> sectionData(ptr, ptr + entry.size);
				
				switch (entry.type) {
				case SectionType::IDENTIFIER:
					identifierData_ = std::move(sectionData);
					break;
				case SectionType::CONTENT:
					if (header_.flags & FLAG_COMPRESSED) {
						DecompressGuard guard(decompressConfig_);
						if (!guard.checkRatio(entry.size, entry.size * 10)) {
							return Error::ZIP_BOMB_DETECTED;
						}
						Result r = compressor_.uncompress(sectionData, contentData_);
						if (!r) {
							ErrorHandler::instance().report(Error::DECOMPRESSION_FAILED, "deserialize: content decompression failed");
							return Error::DECOMPRESSION_FAILED;
						}
					} else {
						contentData_ = std::move(sectionData);
					}
					break;
				case SectionType::CUSTOM:
					customSections_.push_back(std::move(sectionData));
					break;
				default:
					char msg[48];
					std::snprintf(msg, sizeof(msg), "Unknown section type 0x%02X", static_cast<unsigned>(entry.type));
					ErrorHandler::instance().report(Error::UNKNOWN_SECTION_TYPE, msg);
					break;
				}
				ptr += entry.size;
			}
			
			if (ptr + sizeof(Footer) > end) {
				ErrorHandler::instance().report(Error::INVALID_PARAMETER, "deserialize: footer exceeds data size");
				return Error::INVALID_PARAMETER;
			}
			
			Footer footer;
			std::memcpy(&footer, ptr, sizeof(Footer));
			
			if (footer.magicEnd != DEFAULT_MAGIC_FOOTER && footer.magicEnd != footerMagic_) {
				ErrorHandler::instance().report(Error::MAGIC_MISMATCH, "deserialize: invalid footer magic");
			}
			
			ErrorHandler::instance().clear();
			return Error::OK;
		}
		
		Result deserialize(const std::vector<unsigned char>& data) { return deserialize(data.data(), data.size()); }
		
		Result saveToFile(const std::string& filename) {
			std::vector<unsigned char> data;
			Result r = serialize(data);
			if (!r) return r;
			FILE* f = std::fopen(filename.c_str(), "wb");
			if (!f) {
				ErrorHandler::instance().report(Error::FILE_WRITE_FAILED, "saveToFile: cannot open file");
				return Error::FILE_WRITE_FAILED;
			}
			size_t written = std::fwrite(data.data(), 1, data.size(), f);
			std::fclose(f);
			if (written != data.size()) {
				ErrorHandler::instance().report(Error::FILE_WRITE_FAILED, "saveToFile: incomplete write");
				return Error::FILE_WRITE_FAILED;
			}
			return Error::OK;
		}
		
		Result loadFromFile(const std::string& filename) {
			FILE* f = std::fopen(filename.c_str(), "rb");
			if (!f) {
				ErrorHandler::instance().report(Error::FILE_READ_FAILED, "loadFromFile: cannot open file");
				return Error::FILE_READ_FAILED;
			}
			std::fseek(f, 0, SEEK_END);
			long size = std::ftell(f);
			std::fseek(f, 0, SEEK_SET);
			std::vector<unsigned char> data(size);
			size_t read = std::fread(data.data(), 1, size, f);
			std::fclose(f);
			if (read != static_cast<size_t>(size)) {
				ErrorHandler::instance().report(Error::FILE_READ_FAILED, "loadFromFile: incomplete read");
				return Error::FILE_READ_FAILED;
			}
			return deserialize(data);
		}
		
		void printStructure(std::ostream& os = std::cout) const {
			os << "=== BinaryFile Structure ===\n";
			os << "Header:\n";
			os << "  Magic:   0x" << std::hex << header_.magic << std::dec << "\n";
			os << "  Version: " << ((header_.version >> 8) & 0xFF) << "." << (header_.version & 0xFF) << "\n";
			os << "  Flags:   0x" << std::hex << header_.flags << std::dec;
			if (header_.flags & FLAG_COMPRESSED) os << " [COMPRESSED]";
			os << "\n";
			os << "  Sections: " << header_.sectionCount << "\n";
			os << "\nSections:\n";
			os << "  Identifier: " << identifierData_.size() << " bytes\n";
			os << "  Content:    " << contentData_.size() << " bytes\n";
			os << "  Custom:     " << customSections_.size() << " section(s)\n";
			os << "============================\n";
		}
		
	private:
		Header header_;
		uint32_t footerMagic_ = DEFAULT_MAGIC_FOOTER;
		Compressor compressor_;
		DecompressConfig decompressConfig_;
		std::vector<unsigned char> identifierData_;
		std::vector<unsigned char> contentData_;
		std::vector<unsigned char> compressedContentCache_;
		std::vector<std::vector<unsigned char>> customSections_;
		
		static uint32_t crc32(const void* data, size_t size) {
			return iw_crc32(IW_CRC32_INIT, static_cast<const unsigned char*>(data), size);
		}
		static uint32_t crc32(const std::vector<unsigned char>& data) { return crc32(data.data(), data.size()); }
	};
	
}

#endif
