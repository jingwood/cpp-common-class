///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "deflate.h"
#include "stream.h"
#include <stdint.h>

namespace ucm {

CompressBaseStream::CompressBaseStream(Stream& stream, const int chunkSize)
: stream(stream), chunkSize(chunkSize) {
	this->buffer = new byte[this->chunkSize];
	
	this->strm = { };
	this->strm.zalloc = Z_NULL;
	this->strm.zfree = Z_NULL;
	this->strm.next_out = this->buffer;
	this->strm.avail_out = this->chunkSize;
}

void CompressBaseStream::release() {
	if (!this->isFlushed) {
		this->flush();
	}
	
	if (this->buffer != NULL) {
		delete [] this->buffer;
		this->buffer = NULL;
	}
}

CompressStream::CompressStream(Stream& stream, const int chunkSize)
: CompressBaseStream::CompressBaseStream(stream, chunkSize) {
	deflateInit(&this->strm, Z_BEST_COMPRESSION);
}

CompressStream::~CompressStream() {
	this->release();
}

uint CompressStream::write(const byte *buffer, const uint length) {
//	FIXME: allow this check
//	if (!this->stream.canWrite) {
//		throw StreamNotAvailableException();
//	}
	
	this->strm.next_in = const_cast<byte*>(buffer);
	this->strm.avail_in = length;

	uint bytesWritten = 0;
	
	do {
		strm.avail_out = this->chunkSize;
		strm.next_out = this->buffer;
		
		int res = deflate(&strm, Z_NO_FLUSH);
		if (res == Z_STREAM_ERROR) break;
		
		const uint bytesFlushed = this->chunkSize - this->strm.avail_out;
		this->stream.write(this->buffer, bytesFlushed);
		bytesWritten += bytesFlushed;

	} while (this->strm.avail_out == 0);
	
	return bytesWritten;
}

void CompressStream::flush() {
	
	do {
		strm.avail_out = this->chunkSize;
		strm.next_out = this->buffer;
		
		int res = deflate(&strm, Z_FINISH);
		if (res == Z_STREAM_ERROR) break;
		
		const uint bytesFlushed = this->chunkSize - this->strm.avail_out;
		this->stream.write(this->buffer, bytesFlushed);
		
	} while (this->strm.avail_out == 0);
	
	deflateEnd(&strm);
	
	this->isFlushed = true;
}

void CompressStream::compress(const byte* data, const uint dataLength, std::vector<byte>& out_data) {
	std::vector<uint8_t> buffer;
	
	constexpr size_t BUFFER_SIZE = 128 * 1024;
	byte tempBuffer[BUFFER_SIZE];
	
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.next_in = const_cast<byte*>(data);
	strm.avail_in = dataLength;
	strm.next_out = tempBuffer;
	strm.avail_out = BUFFER_SIZE;
	
	deflateInit(&strm, Z_BEST_COMPRESSION);
	
	while (strm.avail_in > 0) {
		deflate(&strm, Z_FINISH);
	
		if (strm.avail_out == 0) {
			buffer.insert(buffer.end(), tempBuffer, tempBuffer + BUFFER_SIZE);
			strm.next_out = tempBuffer;
			strm.avail_out = BUFFER_SIZE;
		}
	}
	
	buffer.insert(buffer.end(), tempBuffer, tempBuffer + BUFFER_SIZE - strm.avail_out);
	
	deflateEnd(&strm);
	
	out_data.swap(buffer);
}

void CompressStream::decompress(const byte* data, const uint dataLength, std::vector<byte>& output) {
	std::vector<uint8_t> buffer;
	
	constexpr size_t BUFFER_SIZE = 128 * 1024;
	byte tempBuffer[BUFFER_SIZE];
	
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.next_in = const_cast<byte*>(data);
	strm.avail_in = dataLength;
	strm.next_out = tempBuffer;
	strm.avail_out = BUFFER_SIZE;
	
	inflateInit(&strm);
	
	while (strm.avail_in > 0) {
		inflate(&strm, Z_NO_FLUSH);
		
		if (strm.avail_out == 0) {
			buffer.insert(buffer.end(), tempBuffer, tempBuffer + BUFFER_SIZE);
			strm.next_out = tempBuffer;
			strm.avail_out = BUFFER_SIZE;
		}
	}
	
	buffer.insert(buffer.end(), tempBuffer, tempBuffer + BUFFER_SIZE - strm.avail_out);
	
	inflateEnd(&strm);
	
	output.swap(buffer);
}

////////////////// DecompressStream //////////////////

DecompressStream::DecompressStream(Stream& stream, const int chunkSize)
: CompressBaseStream::CompressBaseStream(stream, chunkSize) {
	
	inflateInit(&this->strm);
}

DecompressStream::~DecompressStream() {
	this->release();
}

uint DecompressStream::write(const byte *buffer, const uint length) {
	//	FIXME: allow this check
	//	if (!this->stream.canWrite) {
	//		throw StreamNotAvailableException();
	//	}
	
	this->strm.next_in = const_cast<byte*>(buffer);
	this->strm.avail_in = length;
	
	uint bytesWritten = 0;
	
	do {
		strm.avail_out = this->chunkSize;
		strm.next_out = this->buffer;
		
		int res = inflate(&strm, Z_NO_FLUSH);
		if (res == Z_STREAM_ERROR) break;
		
		const uint bytesFlushed = this->chunkSize - this->strm.avail_out;
		this->stream.write(this->buffer, bytesFlushed);
		bytesWritten += bytesFlushed;
		
	} while (this->strm.avail_out == 0);
	
	return bytesWritten;
}

void DecompressStream::flush() {
	
	do {
		strm.avail_out = this->chunkSize;
		strm.next_out = this->buffer;
		
		int res = inflate(&strm, Z_FINISH);
		if (res == Z_STREAM_ERROR) break;
		
		const uint bytesFlushed = this->chunkSize - this->strm.avail_out;
		this->stream.write(this->buffer, bytesFlushed);
		
	} while (this->strm.avail_out == 0);
	
	inflateEnd(&strm);
	
	this->isFlushed = true;
}

}
