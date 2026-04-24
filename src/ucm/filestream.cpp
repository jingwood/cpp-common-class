///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "filestream.h"
#include "file.h"

#include <stdio.h>
#include <memory>

#if _WIN32
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#define _fopen(filename, access, FILE)    fopen_s(&FILE, filename, access)
#else
#define _fopen(filename, access, FILE)    FILE = fopen(filename, access)
#include <dirent.h>
#endif

namespace ucm {

FileStream::FileStream(File* file, const FileStreamBehavior behavior, const FileStreamType streamType) {
  this->file = file;
  this->filename = file->getFullPathName();

  this->open(behavior, streamType);
}

FileStream::FileStream(const char* filename, const FileStreamBehavior behavior, const FileStreamType streamType) {
  this->filename = filename;
  
  this->open(behavior, streamType);
}

void FileStream::open(const FileStreamBehavior behavior, const FileStreamType streamType) {
  if (this->fileHandler != NULL) {
    throw FileException("file in use");
  }
  
  _fopen(this->filename,
         streamType == FileStreamType::Binary ?
         (behavior == FileStreamBehavior::Read ? "rb" : "wb") :
         (behavior == FileStreamBehavior::Read ? "r" : "w"), this->fileHandler);

	if (this->fileHandler == NULL) {
		fprintf(stderr, "open file error: %s\n", this->filename);
		throw FileException("cannot open file stream");
	}
}

int FileStream::read(void* buffer, const uint length) {
	return (int)fread(buffer, 1, length, this->fileHandler);
}

bool FileStream::readLine(char *lineBuffer, const int lineBufferSize) const {
  char* line = fgets(lineBuffer, lineBufferSize, this->fileHandler);

  if (line != NULL) {
    int len = (int)(size_t)strlen(line);
		
    if (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n')) {
      line[len - 1] = STR_EOF;
			
			if (len > 1 && (line[len - 2] == '\r' || line[len - 2] == '\n')) {
				line[len - 2] = STR_EOF;
			}
    }
    
    return true;
  }
  else
    return false;
}

size_t FileStream::write(const void* buffer, const size_t length) {
	size_t written = fwrite(buffer, 1, length, this->fileHandler);
	if (written != length) {
		throw FileException("file write failed");
	}
	return written;
}

void FileStream::writeText(const char* str) const {
  fputs(str, this->fileHandler);
}

size_t FileStream::getPosition() const {
	long pos = ftell(this->fileHandler);
	if (pos < 0) {
		throw FileException("ftell failed");
	}
	return (size_t)pos;
}

size_t FileStream::getLength() const {
	long cur = ftell(this->fileHandler);
	if (cur < 0 || fseek(this->fileHandler, 0, SEEK_END) != 0) {
		throw FileException("seek to end failed");
	}
	long len = ftell(this->fileHandler);
	if (len < 0) {
		throw FileException("ftell at end failed");
	}
	if (fseek(this->fileHandler, cur, SEEK_SET) != 0) {
		throw FileException("restore seek position failed");
	}
	return (size_t)len;
}

void FileStream::setPosition(const size_t pos) {
	fseek(this->fileHandler, (long)pos, SEEK_SET);
}

bool FileStream::isEnd() const {
	return feof(this->fileHandler) == 1;
}

void FileStream::close() {
  if (this->fileHandler != NULL) {
    fclose(this->fileHandler);
    this->fileHandler = NULL;
  }
  
  if (this->file != NULL) {
    this->file->currentStream = NULL;
  }
}

FileStream::~FileStream()
{
	this->close();
}

}