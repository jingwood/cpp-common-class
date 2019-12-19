///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef deflate_h
#define deflate_h

#include <vector>
#include "types.h"
#include "stream.h"

extern "C" {
#include "../../inc/zlib.h"
}

#define CHUNK_DEFAULT_SIZE 65536

namespace ucm {

class CompressBaseStream {
protected:
	Stream& stream;
	int chunkSize;
	z_stream strm;
	bool isFlushed = false;
	byte* buffer = NULL;

	CompressBaseStream(Stream& stream, const int chunkSize = CHUNK_DEFAULT_SIZE);
	void release();
	virtual void flush() = 0;
};

class CompressStream : protected CompressBaseStream {
private:

public:
	CompressStream(Stream& stream, const int chunkSize = CHUNK_DEFAULT_SIZE);
	~CompressStream();
	
	uint write(const byte* buffer, const uint length);
	void flush();
	
	static void compress(const byte* data, const uint dataLength, std::vector<byte>& output);
	static void decompress(const byte* data, const uint dataLength, std::vector<byte>& output);
};

class DecompressStream : protected CompressBaseStream {
private:
	
public:
	DecompressStream(Stream& stream, const int chunkSize = CHUNK_DEFAULT_SIZE);
	~DecompressStream();
	
	uint write(const byte* buffer, const uint length);
	void flush();
};

}

#undef CHUNK_DEFAULT_SIZE

#endif /* deflate_h */
