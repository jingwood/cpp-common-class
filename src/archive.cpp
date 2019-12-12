///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "archive.h"
#include <time.h>

#define FORMAT_TAG_SOBA 0x61626f73
#define FORMAT_TAG_TOBA 0x61626f74

Archive::Archive() {
	static bool randseeked = false;
	if (!randseeked) {
		srand((unsigned int)time(NULL));
		randseeked = true;
	}
}

Archive::~Archive() {
	
}

ChunkEntry* Archive::newChunk(const uint format) {
	ChunkEntry* entry = new ChunkEntry();
	entry->uid = this->trunk.newTrunk(format);
	entry->format = format;
	entry->stream = new MemoryStream();
	return entry;
}

ChunkEntry* Archive::openChunk(const uint uid, const uint format) {
	ChunkEntry* entry = new ChunkEntry();
	entry->uid = uid;
	entry->format = format == 0 ? this->trunk.getTrunkFormat(uid) : format;
	size_t dataLength;
	const byte* buf = this->trunk.getTrunkData(uid, format, &dataLength);
	entry->stream = new MemoryStream(buf, (uint)dataLength);
	return entry;
}

uint Archive::touchChunk(const uint uid, const uint format) {
	if (uid == 0) {
		return this->trunk.newTrunk(format);
	} else {
		this->trunk.setTrunkData(uid, format, NULL, 0);
		return uid;
	}
}

void Archive::updateChunk(ChunkEntry* entry) {
	const uint length = (uint)entry->stream->getLength();
	
#if defined(DEBUG)
	if (length == 0) {
		printf("stream is empty\n");
	}
#endif /* DEBUG */
	
	if (entry->isCompressed) {
		this->trunk.setTrunkData(entry->uid, entry->format,
														 entry->stream->getBuffer(), length);
	} else {
		this->trunk.setTrunkData(entry->uid, entry->format,
														 entry->stream->getBuffer(), length, 0);
	}
}

void Archive::closeChunk(ChunkEntry *entry) {
	if (entry != NULL) {
		delete entry;
	}
}

void Archive::updateAndCloseChunk(ChunkEntry *entry) {
	this->updateChunk(entry);
	this->closeChunk(entry);
}

void Archive::saveChunkData(const uint uid, const uint format, Stream& stream, bool isCompressed) {
	auto chunk = this->openChunk(uid);
	chunk->format = format;
	chunk->isCompressed = isCompressed;
	Stream::copy(stream, *chunk->stream);
	this->updateAndCloseChunk(chunk);
}

void Archive::getTextChunkData(const uint uid, const uint format, string* str) {
	if (str == NULL) return;
	
	size_t dataLength;
	const byte* buf = this->trunk.getTrunkData(uid, format, &dataLength);
	
	if (buf != NULL && dataLength > 0) {
		string::decode(buf, dataLength, str);
	}
}

void Archive::setTextChunkData(const uint uid, const uint format, const string &str) {
	size_t dataLength;
	byte* buf;
	
	string::encode(str, &buf, &dataLength);
	
	this->trunk.setTrunkData(uid, format, buf, (uint)dataLength);
}

bool Archive::deleteChunk(const uint uid, const uint format) {
	return this->trunk.deleteTrunk(uid, format);
}

void Archive::load(const string& path) {
	FileStream stream(path);
	stream.openRead();
	
	ArchiveFileHeader header;
	stream.read(&header, sizeof(ArchiveFileHeader));

	if (header.format != FORMAT_TAG_SOBA
			&& header.format != FORMAT_TAG_TOBA) {
		throw ArchiveFormatInvalidException();
	}
	
	this->fileInfo.format = header.format;
	this->fileInfo.version = header.ver;

	this->trunk.clear();
	this->trunk.load(stream);
}

void Archive::save(const string& path) {
	FileStream stream(path);
	stream.openWrite();
	
	ArchiveFileHeader header = { };
	header.format = FORMAT_TAG_TOBA;
	header.ver = 0x0100;
	header.headerSize = sizeof(ArchiveFileHeader);
	stream.write(&header, header.headerSize);
	
	this->trunk.save(stream);
}

////////////////// ChunkEntry //////////////////

ChunkEntry::ChunkEntry() {
	
}

ChunkEntry::~ChunkEntry() {
	if (this->stream != NULL) {
		delete this->stream;
		this->stream = NULL;
	}
}
