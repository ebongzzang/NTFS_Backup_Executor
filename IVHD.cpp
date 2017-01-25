#include "IVHD.h"


IVHD::IVHD(enumVhdType _vhdType, const char * _filePath, unsigned long _maxSizeMB) //Get attribute
	:vhdType(_vhdType), filePath(_filePath), maxSize(convertMBtoByte(_maxSizeMB))
{
	
}

bool IVHD::InitVHD() //make VHD file						
{
	vhdFile = new VHD(filePath,vhdType,maxSize);
	if (!vhdFile->isSucceed())
	{
		std::cout << "error: VHD file was not opened." << std::endl;
		return false;
	}
	return true;
}
bool IVHD::writeVHDHeader()
{
	if(!vhdFile->writeFooter())
		return false;
	if(!vhdFile->writeDynamicHeader())
		return false;
	if (!vhdFile->writeBatMap())
		return false;
	return true;
}

bool IVHD::writeMirrorFooter()
{
	vhdFile->writeFooter();
	return true;
}
	
IVHD::~IVHD()
{
	delete vhdFile;
}
