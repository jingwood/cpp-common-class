///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef archive_h
#define archive_h

#include "stream.h"
#include "trunk.h"

namespace unvell_common_class {

class ChunkEntry;
class ArchiveInfo;

class Archive {
	friend ArchiveInfo;
	
private:
	FileTrunk trunk;
	
	struct ArchiveFileHeader {
		uint format;
		ushort ver;
		ushort flags;
		uint headerSize;
		uint reserved;
	};

public:
	Archive();
	~Archive();
	
	struct {
		uint format = 0;
		uint version = 0x0100;
	} fileInfo;

	ChunkEntry* newChunk(const uint format = 0);
	ChunkEntry* openChunk(const uint uid, const uint format = 0);
	uint touchChunk(const uint uid, const uint format = 0);
	void updateChunk(ChunkEntry* entry);
	void closeChunk(ChunkEntry* entry);
	void updateAndCloseChunk(ChunkEntry* entry);
	void saveChunkData(const uint uid, const uint format, Stream& stream, bool isCompressed = true);
	
	void getTextChunkData(const uint uid, const uint format, string* str);
	void setTextChunkData(const uint uid, const uint format, const string& str);
	
	bool deleteChunk(const uint uid, const uint format = 0);

	void load(const string& path);
	void save(const string& path);
};

class ChunkEntry {
	friend Archive;
	
private:
	ChunkEntry();
	~ChunkEntry();

public:
	uint uid = 0;
	uint format = 0;
	MemoryStream* stream = NULL;
	bool isCompressed = true;
};

class ArchiveInfo {
private:
	const Archive& archive;
	
public:
	ArchiveInfo(const Archive& archive) : archive(archive) {
		
		for (const auto& index : this->archive.trunk.getIndices()) {
			totalDataBytes += index.length;
		}
		
		logicalArchiveSize = sizeof(Archive::ArchiveFileHeader)
			+ this->totalDataBytes
			+ this->archive.trunk.getCount() * sizeof(uint) * 6 // trunk index
			+ sizeof(uint) * 4; // trunk header
	}
	
	uint totalDataBytes = 0;
	uint logicalArchiveSize = 0;
	
	inline const FileTrunk& getTrunks() const {
		return this->archive.trunk;
	}
};

class ArchiveFormatInvalidException : public Exception {
	
};

}

#endif /* archive_h */
