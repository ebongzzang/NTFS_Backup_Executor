#include "stdafx.h"
#include <vector>
#include <array>
using std::ifstream;
using std::ofstream;
using std::string;


enum VhdType
{
	VHD_DISK_TYPE_FIXED = 0x00000002,
	VHD_DISK_TYPE_DYNAMIC = 0x00000003,
	VHD_DISK_TYPE_DIFF = 0x00000004
};


class IVHD
{

public:
	//std::iostream * InitVHD(VhdType type, unsigned int maxSize,  );
	IVHD(VhdType _backupType, const char * 
		, unsigned long long _maxSize);
	bool InitVHD();
	bool writeFooter();
	bool writeDynamicHeader();
	bool WriteBatMap();
	bool WriteTailFooter();
	ofstream * getVHDHandle();
	~IVHD();
private:

	struct vhdFooter
	{
		char cookie[8];
		uint32_t features;
		uint32_t version;
		uint64_t dataOffset;
		uint32_t timeStamp;
		char creatorApp[4];
		uint32_t creatorVersion;
		uint32_t creatorOs;
		uint64_t originalSize;
		uint64_t currentSize;
		uint32_t diskGeometry;
		uint32_t diskType;
		uint32_t checksum;
		uuid_t id;
		char savedState;
		char footerReserved[427];
	};

	struct vhdDynamicHeader
	{
		struct VHDParentLocatorEntry
		{
			uint32_t u32Code;
			uint32_t u32DataSpace;
			uint32_t u32DataLength;
			uint32_t u32Reserved;
			uint64_t u64DataOffset;
		};

		char cookie[8];
		uint64_t dataoffset;
		uint64_t tableoffset;
		uint32_t header_version;
		uint32_t table_entries;
		uint32_t blocksize;
		uint32_t checksum;
		uint32_t reserved;
		uint8_t  ParentUuid[16];
		uint32_t ParentTimestamp;
		VHDParentLocatorEntry  ParentLocatorEntry[8];
		char parent_unicodename[512];
		char dynamicHeaderReserved[256];
	};

	const char * absPath;
	ofstream  vhdFile;
	vhdFooter  footer;
	vhdDynamicHeader   dynamicHeader;
	VhdType backupType;
	void * temparray;
	std::array <unsigned long, 2560> blockalloctable2; 
	std::vector <unsigned long> blockalloctable;
	unsigned long long maxSize;
	unsigned int calculate_checksum(const unsigned char * data, size_t dsize);
	unsigned int cal_chs(void);

	

};