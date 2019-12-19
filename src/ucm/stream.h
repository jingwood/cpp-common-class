///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef stream_h
#define stream_h

#include <stdio.h>
#include "types.h"
#include "exception.h"

namespace ucm {
	
class StreamNotAvailableException : Exception {
	
};

class StreamReadonlyException : Exception {
	
};

class Stream {
public:
	virtual ~Stream() { }
	
	virtual int read(void* buffer, const uint length) = 0;
	virtual size_t write(const void* buffer, const size_t length) = 0;
	virtual void flush() = 0;
	
	virtual size_t getLength() const = 0;
	virtual size_t getPosition() const = 0;
	virtual void setPosition(const size_t pos) = 0;
	virtual bool isEnd() const = 0;
	
	static void copy(Stream& from, Stream& to);
};

class MemoryStream : public Stream {
private:
	
protected:
	size_t length = 0;
	size_t capacity = 0;
	byte* buffer = NULL;
	size_t position = 0;
	bool readonly = false;
	
	void expand(size_t needLength);
	void append(const void* buffer, const size_t length);

	static constexpr uint MEMORY_STREAM_BUFFER_SIZE = 65536;

public:
	MemoryStream(const uint capacity = MEMORY_STREAM_BUFFER_SIZE);
	MemoryStream(const byte* input, const uint length);
	~MemoryStream();
	
	inline const byte* getBuffer() const {
		return this->buffer;
	}
	
	int read(void* buffer, const uint length);
	size_t write(const void* buffer, const size_t length);
	void flush() { }
	
	size_t getLength() const;
	size_t getPosition() const;
	void setPosition(const size_t pos);
	bool isEnd() const;

	void clear();
	void close();
};

class ReadonlyMemoryStream : public MemoryStream {
private:
public:
	ReadonlyMemoryStream(const byte* buffer, const size_t length);
};

}

#endif /* stream_h */
