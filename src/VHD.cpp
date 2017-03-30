#include "VHD.h"
#include "stdafx.h"
VHD::VHD(const char * _filePath, enumVhdType _vhdType, unsigned long _maxSize)//Get attribute
:	vhdType(_vhdType), filePath(_filePath),maxSize(_maxSize)
{
	VHDHandle = new ofstream();
	VHDHandle->open(filePath, std::ios::binary); //open vhd file
}
bool VHD::isSucceed()
{
	if (VHDHandle->is_open())
		return true;
	else
		return false;
}

bool VHD::writeFooter() //
{
	if (vhdType != VHD_DISK_TYPE_DYNAMIC)
	{
		return false;
	}

	memcpy(footer.cookie, VHD_FOOTER_COOKIE, sizeof(footer.cookie));
	footer.features = _byteswap_ulong(VHD_FEATURES_RESERVED);
	footer.version = _byteswap_ulong(VHD_VERSION);
	footer.dataOffset = _byteswap_uint64(sizeof(vhdFooter)); //indicate dynamicHeader offset
	footer.timeStamp = _byteswap_ulong(((unsigned int)time(NULL))); //unixTime
	memcpy(footer.creatorApp, VHD_CREATOR_TOOL, sizeof(footer.creatorApp)); //plan
	footer.creatorVersion = _byteswap_ulong(VHD_CREATOR_VERSION);
	footer.creatorOs = _byteswap_ulong(VHD_CREATOR_OS);
	footer.originalSize = _byteswap_uint64(maxSize);
	footer.currentSize = _byteswap_uint64(maxSize);
	footer.diskGeometry = _byteswap_ulong((unsigned int)(cal_chs()));
	footer.diskType = _byteswap_ulong(vhdType);
	footer.checksum = 0;
	if ((VHDHandle->tellp()) < 0x200)
	{
		UuidCreate(&footer.id);
	}
	footer.savedState = 0;
	memset(footer.footerReserved, 0, 427);
	if ((VHDHandle->tellp()) < 0x200)
	{
		footer.checksum = _byteswap_ulong(calculate_checksum((unsigned char*)&footer, sizeof(vhdFooter)));
	}

	if ((VHDHandle->write(reinterpret_cast<char *>(&footer), sizeof(vhdFooter))))
		return true;
	else {
		std::cout << "Write vhdFooter Failed." << "reason   " + GetLastError() << std::endl;
		return false;
	}

}

bool VHD::writeDynamicHeader(void) //dynamic disk only
{
	if (static_cast<int>(VHDHandle->tellp()) != 512)
	{
		std::cout << "Dynamic header position error." << std::endl;
		return false;

	}

	memcpy(dynamicHeader.cookie, VHD_HEADER_COOKIE, sizeof(dynamicHeader.cookie));
	dynamicHeader.dataoffset = 0xFFFFFFFFFFFFFFFFLLU; //unused
	dynamicHeader.tableoffset = _byteswap_uint64(sizeof(vhdDynamicHeader) + sizeof(vhdFooter)); //BAT offset
	dynamicHeader.header_version = _byteswap_ulong(0x00010000);
	dynamicHeader.table_entries = _byteswap_ulong(converBytetoMB(maxSize) / 2); //number of BAT entries
	dynamicHeader.blocksize = _byteswap_ulong(0x00200000); // 2MB
	dynamicHeader.checksum = 0;
	memset(dynamicHeader.ParentUuid, 0, sizeof(dynamicHeader.ParentUuid));
	dynamicHeader.ParentTimestamp = 0;
	dynamicHeader.reserved = 0;
	memset(dynamicHeader.parent_unicodename, 0, sizeof(dynamicHeader.parent_unicodename));
	memset(dynamicHeader.ParentLocatorEntry, 0, sizeof(vhdDynamicHeader::ParentLocatorEntry));

	memset(dynamicHeader.dynamicHeaderReserved, 0, 256);

	dynamicHeader.checksum = _byteswap_ulong(calculate_checksum((unsigned char*)&dynamicHeader, sizeof(vhdDynamicHeader)));

	if ((VHDHandle->write(reinterpret_cast<char *>((&dynamicHeader)), sizeof(vhdDynamicHeader))))
		return true;
	else
	{
		std::cout << "Write dynamicHeader Failed." << "reason" + GetLastError() << std::endl;
		return false;
	};
}

bool VHD::writeBatMap()
{
	//std::cout << converBytetoMB(maxSize) << std::endl;
	//std::cout << convertEndian(dynamicHeader.table_entries) << std::endl;
	blockalloctable.resize(_byteswap_ulong(dynamicHeader.table_entries));
	blockalloctable.assign(_byteswap_ulong(dynamicHeader.table_entries), static_cast<unsigned long>(0xFFFFFFFF));

	for (auto &x : blockalloctable)
	{
		VHDHandle->write(reinterpret_cast<char *>(&x), sizeof(unsigned long));
	}
	writeDataOffset = VHDHandle->tellp();

	writeDataOffset += 1; //tellp() + 1 = starting
	std::cout << writeDataOffset << std::endl;

	return true;

}

unsigned int VHD::cal_chs(void)
{
	uint64_t totalSectors = maxSize / (512);
	unsigned int cylinderTimesHeads;
	unsigned char heads;
	unsigned char sectorsPerTrack;

	if (totalSectors > 65535 * 16 * 255)
	{
		totalSectors = 65535 * 16 * 255;
	}

	if (totalSectors >= 65535 * 16 * 63)
	{
		sectorsPerTrack = 255;
		heads = 16;
		cylinderTimesHeads = (unsigned int)(totalSectors / sectorsPerTrack);
	}
	else
	{
		sectorsPerTrack = 17;
		cylinderTimesHeads = (unsigned int)(totalSectors / sectorsPerTrack);

		heads = (cylinderTimesHeads + 1023) / 1024;

		if (heads < 4)
		{
			heads = 4;
		}
		if (cylinderTimesHeads >= ((unsigned int)heads * 1024) || heads > 16)
		{
			sectorsPerTrack = 31;
			heads = 16;
			cylinderTimesHeads = (unsigned int)(totalSectors / sectorsPerTrack);
		}
		if (cylinderTimesHeads >= ((unsigned int)heads * 1024))
		{
			sectorsPerTrack = 63;
			heads = 16;
			cylinderTimesHeads = (unsigned int)(totalSectors / sectorsPerTrack);
		}
	}
	unsigned short cylinders = cylinderTimesHeads / heads;

	unsigned int chs = (cylinders << 16) + (heads << 8) + sectorsPerTrack;
	return chs;
}

unsigned int VHD::calculate_checksum(const unsigned char * data, size_t dsize)
{
	unsigned int checksum = 0;
	for (size_t i = 0; i<dsize; ++i)
	{
		checksum += data[i];
	}
	return ~checksum;
}

VHD::~VHD()
{
	delete filePath;
	delete VHDHandle;
}
