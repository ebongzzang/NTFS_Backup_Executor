#pragma once
#ifndef I_VHD

#define I_VHD
#include "stdafx.h"
#include <vector>
#include <array>
#include "VHD.h"

class IVHD
{

public:
	IVHD(enumVhdType _vhdType, const char * _filePath
		, unsigned long _maxSize);
	bool InitVHD();
	bool writeVHDHeader();
	bool writeMirrorFooter();

	~IVHD();
private:
	enumVhdType vhdType;
	unsigned long maxSize;
	VHD* vhdFile;
	const char * filePath;

	uint64_t writeDataOffset;
	uint64_t writeBATOffset;
};

#endif // !IVHD