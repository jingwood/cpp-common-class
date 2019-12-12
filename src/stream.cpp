///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "stream.h"
#include <string.h>

namespace ccm {
	
void Stream::copy(Stream& from, Stream& to) {
	constexpr int bufferSize = 40960;
	byte* buffer = new byte[bufferSize];
	
	int readBytes = 0;
	while ((readBytes = from.read(buffer, bufferSize)) > 0) {
		to.write(buffer, readBytes);
	}
	
	delete [] buffer;
}

MemoryStream::MemoryStream(const uint capacity) {
	this->expand(capacity);
}

MemoryStream::MemoryStream(const byte* input, const uint length)
: MemoryStream() {
	if (length > 0) {
		this->append(input, length);
		this->setPosition(0);
	}
}

MemoryStream::~MemoryStream() {
	this->close();
}

void MemoryStream::expand(size_t needLength) {
	if (this->capacity < needLength) {
		this->capacity = this->capacity + needLength;
		this->capacity += MEMORY_STREAM_BUFFER_SIZE - (this->capacity % MEMORY_STREAM_BUFFER_SIZE);
		
		byte* newBuffer = new byte[this->capacity];
		
		if (this->buffer != NULL) {
			memcpy(newBuffer, this->buffer, this->length);
			delete [] this->buffer;
		}
		
		this->buffer = newBuffer;
	}
}

void MemoryStream::append(const void* buffer, const size_t length) {
	const size_t newLength = this->position + length;
	
	if (newLength > this->capacity) {
		this->expand(newLength);
	}
	
	memcpy(this->buffer + this->position, buffer, length);
	
	this->position += length;
	this->length += length;
}

int MemoryStream::read(void *buffer, const uint length) {
	size_t readBytes = length;
	if (this->position + readBytes > this->length) readBytes = this->length - this->position;
	memcpy(buffer, this->buffer + this->position, readBytes);
	this->position += readBytes;
	return (int)readBytes;
}

size_t MemoryStream::write(const void* buffer, const size_t length) {
	if (this->readonly) {
		throw StreamReadonlyException();
	}
	
	this->append(buffer, length);
	return length;
}

size_t MemoryStream::getLength() const {
	return this->length;
}

size_t MemoryStream::getPosition() const {
	return this->position;
}

void MemoryStream::setPosition(const size_t pos) {
	if (pos <= this->length) {
		this->position = pos;
	} else {
		this->position = this->length;
	}
}

bool MemoryStream::isEnd() const {
	return this->position >= this->length;
}

void MemoryStream::clear() {
	this->setPosition(0);
	this->length = 0;
}

void MemoryStream::close() {
	if (this->buffer != NULL) {
		delete [] this->buffer;
		this->buffer = NULL;
	}
}

ReadonlyMemoryStream::ReadonlyMemoryStream(const byte* buffer, const size_t length)
: MemoryStream() {
	this->buffer = const_cast<byte*>(buffer);
	this->length = length;
	this->readonly = true;
}

}
