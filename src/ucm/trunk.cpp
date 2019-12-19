///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <algorithm>

#include "trunk.h"
#include "filestream.h"
#include "deflate.h"

#define UID_GM_SEQUENTIALLY 1
#define UID_GM_RANDOMLY 2
#define UID_GENERATION_METHOD UID_GM_RANDOMLY

namespace ucm {

FileTrunk::~FileTrunk() {
	this->clear();
}

bool FileTrunk::load(FileStream &stream) {
	const size_t streamStartPos = stream.getPosition();
	
	TrunkHeader header;
	int readBytes = stream.read(&header, sizeof(TrunkHeader));
	if (readBytes < sizeof(TrunkHeader)) {
		return false;
	}
	
	stream.setPosition(streamStartPos + header.headerSize);
	
	this->clear();
	
	// read index
	for (uint i = 0; i < header.trunkCount; i++) {
		TrunkIndex index;
		readBytes = stream.read(&index, TrunkIndexSize);
		if (readBytes < TrunkIndexSize) {
			return false;
		}
		indices.push_back(index);
	}
	
	// read data
	for (TrunkIndex& index : this->indices) {
		stream.setPosition(streamStartPos + index.offset);
		index.data = new byte[index.length];
		byte* buffer = const_cast<byte*>(index.data);
		readBytes = stream.read(buffer, index.length);
		
		if (readBytes < (int)index.length) {
			delete [] index.data;
			index.data = NULL;
			index.length = 0;
		}
	}
	
	return true;
}

void FileTrunk::save(FileStream &stream) {
	TrunkHeader header = { };
	header.ver = 0x0100;
	header.flags = 0;
	header.trunkCount = (uint)this->indices.size();
	header.headerSize = sizeof(TrunkHeader);
	
	stream.write(&header, sizeof(TrunkHeader));
		
	// write index
	uint offset = (uint)(sizeof(TrunkHeader) + TrunkIndexSize * this->indices.size());
	
	for (TrunkIndex& index : this->indices) {
		index.offset = offset;
		offset += index.length;
		
		stream.write(&index, TrunkIndexSize);
	}
	
	// write data
	for (const TrunkIndex& index : this->indices) {
		if (index.length > 0 && index.data != NULL) {
			stream.write(index.data, index.length);
		}
	}
}

void FileTrunk::clear() {
	for (TrunkIndex& index : this->indices) {
		if (index.data != NULL) {
			delete [] index.data;
		}
	}
	this->indices.clear();
}

FileTrunk::TrunkIndex* FileTrunk::getTrunkIndex(const uint uid, const uint format) {
	for (TrunkIndex& index : this->indices) {
		if (index.uid == uid) {
			if (format == 0 || index.format == format) {
				return &index;
			}
		}
	}
	
	return NULL;
}

uint FileTrunk::getAvailableUid() {
	int uid;
	
#if UID_GENERATION_METHOD == UID_GM_SEQUENTIALLY
	uid = (int)this->indices.size() + 1;
	
	bool found;
	do {
		found = false;

		for (const TrunkIndex& index : this->indices) {
			if (index.uid == uid) {
				found = true;
				uid++;
				break;
			}
		}
		
	} while (found);
#elif UID_GENERATION_METHOD == UID_GM_RANDOMLY
	
	bool found;
	do {
		uid = ((uint)rand()) % 0xffffffff;
		found = false;
		
		for (const TrunkIndex& index : this->indices) {
			if (index.uid == uid) {
				found = true;
				break;
			}
		}
		
	} while (found);
	
#endif /* UID_GENERATION_METHOD */
	
	return uid;
}

uint FileTrunk::newTrunk(const uint format) {
	const uint uid = this->getAvailableUid();
	TrunkIndex newIndex;
	memset(&newIndex, 0, sizeof(newIndex));
	newIndex.uid = uid;
	newIndex.format = format;
	this->indices.push_back(newIndex);
	return uid;
}

const uint FileTrunk::getTrunkFormat(const uint uid) {
	const TrunkIndex* index = this->getTrunkIndex(uid);
	
	if (index == NULL) {
		return 0;
	}
	
	return index->format;
}

const byte* FileTrunk::getTrunkData(const uint uid, const uint format, size_t* length) {
	TrunkIndex* index = this->getTrunkIndex(uid, format);
	
	if (index == NULL || index->length <= 0 || index->data == NULL) {
		if (length != NULL) {
			*length = 0;
		}
		return NULL;
	}
	
	if (index->trunkFlags & FTF_Compress) {
		MemoryStream ms;
		DecompressStream cs(ms);
		cs.write(index->data, index->length);
		cs.flush();
		
		index->trunkFlags &= ~FTF_Compress;
		delete [] index->data;
		
		index->length = (uint)ms.getLength();
		index->data = new byte[index->length];
		memcpy((void*)index->data, ms.getBuffer(), index->length);
	}
	
	if (length != NULL) {
		*length = index->length;
	}
	
	return index->data;
}

const size_t FileTrunk::getTrunkDataLength(const uint uid, const uint format) {
	const TrunkIndex* index = this->getTrunkIndex(uid, format);
	return (index == NULL || index->length <= 0 || index->data == NULL) ? 0 : index->length;
}

void FileTrunk::setTrunkData(TrunkIndex& index, const byte* data, const uint length) {
	
	if (index.data != NULL) {
		delete [] index.data;
		index.data = NULL;
	}
	
	if (index.trunkFlags & FTF_Compress) {
		MemoryStream ms;
		CompressStream cs(ms);
		cs.write(data, length);
		cs.flush();
		
		index.data = new byte[ms.getLength()];
		index.length = (uint)ms.getLength();
		memcpy((void*)index.data, (void*)ms.getBuffer(), ms.getLength());
		index.compressed = true;
	} else {
		index.data = new byte[length];
		index.length = length;
		memcpy((void*)index.data, (void*)data, length);
	}
}

void FileTrunk::setTrunkData(const uint uid, const uint format, const byte* data, const uint length, uint flags) {
	TrunkIndex* index = this->getTrunkIndex(uid, format);
	
	if (index == NULL) {
		TrunkIndex newIndex;
		memset(&newIndex, 0, sizeof(newIndex));
		this->indices.push_back(newIndex);
		index = &this->indices.back();
	}
	
	index->uid = uid;
	index->format = format;
	index->trunkFlags = flags;
	
	if (data != NULL && length > 0) {
		this->setTrunkData(*index, data, length);
	}
}

bool FileTrunk::deleteTrunk(const uint uid, const uint format) {
	const TrunkIndex* index = this->getTrunkIndex(uid, format);
	
	if (index == NULL) {
		return false;
	}
	
	auto pos = std::find(this->indices.begin(), this->indices.end(), *index);
	
	this->indices.erase(pos);
	
	return true;
}

}

#undef UID_GM_SEQUENTIALLY
#undef UID_GM_RANDOMLY
#undef UID_GENERATION_METHOD
