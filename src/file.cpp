///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "file.h"
#include <stdio.h>
#include <string.h>

#if _WIN32
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#define _fopen(filename, access, FILE)    fopen_s(&FILE, filename, access)
#else
#define _fopen(filename, access, FILE)    FILE = fopen(filename, access)
#include <dirent.h>
#endif

File::File() {
	
}

File::File(const string& fullPathName) {
	this->create(fullPathName);
}

void File::create(const string& fullPathName) {
	this->fullPathName = fullPathName;
	
	int index = -1;
	
	// path & file name
	index = fullPathName.lastIndexOf(PATH_SPLITTER);
	if (index < 0) {
		fileName = fullPathName;
	}	else {
		fullPathName.substring(0, index, path);
		if (index < fullPathName.length() - 1) {
			fullPathName.substring(index + 1, fileName);
		}
	}
	
	// base name & extension
	index = fileName.lastIndexOf('.');
	
	if (index < 0) {
		baseName = fileName;
	} else {
		fileName.substring(0, index, baseName);
		if (index < fileName.length() - 1) {
			fileName.substring(index + 1, extension);
		}
	}
}

const string& File::getFullPathName() const {
	return this->fullPathName;
}

const string& File::getFileName() const {
	return this->fileName;
}

const string& File::getBaseName() const {
  return this->baseName;
}

const string& File::getExtension() const {
  return this->extension;
}

const string& File::getPath() const {
  return this->path;
}

FileStream File::openRead(const string& filename, const FileStreamType streamType) {
	FileStream fs(filename);
	fs.openRead(streamType);
	return fs;
}

void File::readTextFile(const char* filename, string& str) {
  FileStream stream(filename);
	
	try {
		stream.openRead(FileStreamType::Text);
	} catch (const FileException&) {
		fprintf(stderr, "file read error: %s\n", filename);
		return;
	}
	
	int len = (int)stream.getLength();
	
	str.clear();
	str.expand(len);

	char* buffer = new char[len + 1];
  stream.read(buffer, len);
	str.append(buffer);
	delete [] buffer;
	
  stream.close();
}

void File::writeTextFile(const char *filename, const char* str) {
  FileStream stream(filename);
  stream.openWrite(FileStreamType::Text);
  stream.writeText(str);
  stream.close();
}

void File::writeBinaryFile(const char* filename, void* buffer, const int len) {
	FileStream file(filename);
	file.openWrite();
	file.write(buffer, len);
	file.close();
}

bool File::isExist() const {
#if _WIN32
	return PathFileExistsA(this->fullPathName) == 1;
#endif // _WIN32
	
	return true;
}

FileStream& File::open(const FileStreamBehavior behavior, const FileStreamType streamType) {
  if (this->currentStream != NULL) {
		if (this->currentStream->isOpened()) {
			throw FileException("file in use");
		}

		delete this->currentStream;
		this->currentStream = NULL;
	}
	
	this->currentStream = new FileStream(this->path);
	this->currentStream->open(behavior, streamType);
	
	return *this->currentStream;
}

void File::close() {
	if (this->currentStream != NULL) {
		this->currentStream->close();
		delete this->currentStream;
		this->currentStream = NULL;
	}
}

//////////////////// FileStream ////////////////////

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
  fwrite(buffer, length, 1, this->fileHandler);
	return length;
}

void FileStream::writeText(const char* str) const {
  fputs(str, this->fileHandler);
}

size_t FileStream::getPosition() const {
	return ftell(this->fileHandler);
}

size_t FileStream::getLength() const {
	long cur = ftell(this->fileHandler);
	fseek(this->fileHandler, 0, SEEK_END);
	long len = ftell(this->fileHandler);
	fseek(this->fileHandler, cur, SEEK_SET);
	return len;
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
