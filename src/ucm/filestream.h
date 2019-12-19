///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef filestream_h
#define filestream_h

#include "exception.h"
#include "stream.h"

namespace ucm {

enum FileStreamType {
  Binary,
  Text,
};

enum FileStreamBehavior {
  Read,
  Write,
};

class File;

class FileStream : public Stream
{
private:
  File* file = NULL;
  const char* filename;
  FILE* fileHandler = NULL;
	
public:
  ~FileStream();
  
  inline bool error() const {
    return this->fileHandler == NULL;
  }

	FileStream() { }
  FileStream(File* file, const FileStreamBehavior behavior, const FileStreamType streamType = FileStreamType::Binary);
  FileStream(const char* filename, const FileStreamBehavior behavior,
             const FileStreamType streamType = FileStreamType::Binary);
  FileStream(const char* filename) : filename(filename) { }
  
  void open(const FileStreamBehavior behavior,
            const FileStreamType streamType = FileStreamType::Binary);
  inline void openRead(const FileStreamType streamType = FileStreamType::Binary) {
    this->open(FileStreamBehavior::Read, streamType);
  }
  inline void openWrite(const FileStreamType streamType = FileStreamType::Binary) {
    this->open(FileStreamBehavior::Write, streamType);
  }

	inline bool isOpened() const {
		return this->fileHandler != NULL;
	}
  
  int read(void* buffer, const uint length);
  size_t write(const void* buffer, const size_t length);
	
	bool readLine(char* lineBuffer, const int lineBufferSize) const;
  void writeText(const char* str) const;
	
	void flush() { }
  void close();
	
	size_t getLength() const;
	size_t getPosition() const;
	void setPosition(const size_t pos);
	bool isEnd() const;
  
	inline FILE* getHandler() {
		return this->fileHandler;
	}
};

class FileException : public Exception {
private:
  
public:
  FileException(const char* msg = NULL) : Exception(msg) { }

};

}

#endif /* filestream_h */
