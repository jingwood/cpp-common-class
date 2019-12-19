///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef file_h
#define file_h

#include "string.h"
#include "exception.h"
#include "stream.h"
#include "filestream.h"

#if _WIN32
#define strcpy strcpy_s
#endif /* _WIN32 */

#if _WIN32
#define PATH_SPLITTER '\\'
#define PATH_SPLITTER_STR "\\"
#else
#define PATH_SPLITTER '/'
#define PATH_SPLITTER_STR "/"
#include <dirent.h>
#endif /* _WIN32 */

#ifdef PATH_MAX
#define MAX_PATH_LENGTH PATH_MAX
#else
#define MAX_PATH_LENGTH 260
#endif // PATH_MAX

namespace ucm {

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

}

#endif /* file_h */
