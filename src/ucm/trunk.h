///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef trunk_h
#define trunk_h

#include "types.h"
#include "file.h"

#include <stdio.h>
#include <vector>

namespace unvell_common_class {

class FileTrunk {
private:
	struct TrunkHeader {
		uint headerSize;
		ushort ver;
		ushort flags;
		uint trunkCount;
		uint _reserved;
	};
	
	static constexpr uint TrunkIndexSize = (uint)sizeof(uint) * 6;
	
	struct TrunkIndex {
		uint uid;
		uint format;
		uint offset;
		uint length;
		ushort trunkFlags;
		ushort userFlags;
		uint _reserved;
		
		struct {
			const byte* data = NULL;
			bool compressed = false;
		};

		bool operator==(const TrunkIndex& t2) const {
			return this->uid == t2.uid && this->format == t2.format;
		}

	};
	
	std::vector<TrunkIndex> indices;
	TrunkIndex* getTrunkIndex(const uint uid, const uint format = 0);
	void setTrunkData(TrunkIndex& index, const byte* data, const uint length);
	
public:
	enum Flags {
		FTF_None = 0,
		FTF_Compress = 0x1,
		
		FTF__Default = FTF_Compress,
	};
	
	~FileTrunk();
	
	inline const std::vector<TrunkIndex>& getIndices() const {
		return this->indices;
	}
	
	bool load(FileStream& stream);
	void save(FileStream& stream);
	void clear();
	
	inline uint getCount() const { return (uint)this->indices.size(); }
	uint getAvailableUid();
	uint newTrunk(const uint format = 0);
	const uint getTrunkFormat(const uint uid);
	const byte* getTrunkData(const uint uid, const uint format = 0, size_t* length = NULL);
	const size_t getTrunkDataLength(const uint uid, const uint format = 0);
	void setTrunkData(const uint uid, const uint format, const byte* data, const uint length, uint flags = FTF__Default);
	
	bool deleteTrunk(const uint uid, const uint format = 0);
};

}

#endif /* trunk_h */
