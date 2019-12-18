///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

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

namespace unvell_common_class {

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

}