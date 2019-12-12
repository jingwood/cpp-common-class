///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef file_h
#define file_h

#include <stdio.h>

#include "string.h"
#include "exception.h"
#include "stream.h"

#if _WIN32
#define strcpy strcpy_s
#endif // WIN32

#if _WIN32
#define PATH_SPLITTER '\\'
#define PATH_SPLITTER_STR "\\"
#else
#define PATH_SPLITTER '/'
#define PATH_SPLITTER_STR "/"
#include <dirent.h>

#endif

#ifdef PATH_MAX
#define MAX_PATH_LENGTH PATH_MAX
#else
#define MAX_PATH_LENGTH 260
#endif // PATH_MAX

enum FileStreamType {
  Binary,
  Text,
};

enum FileStreamBehavior {
  Read,
  Write,
};

class FileStream;

class File {
  friend FileStream;
  
private:
  string fullPathName;
	string fileName;
	string baseName;
  string extension;
	string path;
  
  FileStream* currentStream = NULL;
  
public:
	File();
	File(const string& name);

	void create(const string& name);

	const string& getFullPathName() const;
  const string& getFileName() const;
	const string& getBaseName() const;
	const string& getExtension() const;
	const string& getPath() const;

	bool isExist() const;
  
  FileStream& open(const FileStreamBehavior behavior, const FileStreamType streamType = FileStreamType::Binary);
	void close();
  
  inline FileStream& openRead(const FileStreamType streamType = FileStreamType::Binary) {
    return this->open(FileStreamBehavior::Read, streamType);
  }
	
  inline FileStream& openWrite(const FileStreamType streamType = FileStreamType::Binary) {
    return this->open(FileStreamBehavior::Write, streamType);
  }
	
	static FileStream openRead(const string& filename, const FileStreamType streamType = FileStreamType::Binary);
  
  static void readTextFile(const char* filename, string& str);
  static void writeTextFile(const char* filename, const char* str);
	
	static void writeBinaryFile(const char* filename, void* buffer, const int len);
};

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

#endif /* file_h */
