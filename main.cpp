#include "stdafx.h"
#include "IVHD.h"
#include "LocalManager.h"
#include <boost\filesystem.hpp>
using std::ifstream;
using std::ofstream;
using std::string;

using boostPath = boost::filesystem::path;
using boostFileStatus = boost::filesystem::file_status;
using namespace boost::filesystem;

int main(void)
{

	IVHD *vhd = new IVHD(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024); //vhdtype, path, maxsize(mb)

	if (!vhd->InitVHD())
		return false;
	if (!vhd->writeVHDHeader())
		return false;
	if (!vhd->writeMirrorFooter())
		return false;
	char * readBuffer1 = nullptr;

	LocalManager * manager = new LocalManager("C");

	manager->setDiskHandle();
	readBuffer1 = manager->readMBR();

	//for (int i = 0; i < 512; i++)
	//{
	//	std::cout << readBuffer1[i] << std::endl;
	//}
	//system("mklink /d shadowtest \\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy8\\");
	HANDLE hi = CreateFile(L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy8", READ_CONTROL | WRITE_OWNER | WRITE_DAC | GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	std::cout << "wow!! error! code:" << GetLastError() << std::endl;
	LARGE_INTEGER filesize;
	GetFileSizeEx(hi, &filesize);
	std::cout << "get filesize" <<GetLastError() << std::endl;
	std::cout << filesize.QuadPart << std::endl;
	LARGE_INTEGER disk_offset;
	DWORD offsetCount=1;
	char readBuffer[65536];
	disk_offset.QuadPart = 65536;
	std::ofstream mbrtest;
	mbrtest.open("D:\\hi.mbr", std::ios::binary);
	clock_t begin, end;
	begin = clock();
	std::cout << "start backup since" << begin << std::endl;
	int i = 0;
	while(1)
	{
		ReadFile(hi, readBuffer, disk_offset.QuadPart, &offsetCount, 0);
		if (offsetCount != 65536)
			break;
		mbrtest.write(readBuffer, 65536);
		i++;
	}
	end = clock();
	std::cout << "수행시간 " << ((end - begin) / CLOCKS_PER_SEC) << "초" << std::endl;


	//if (!ReadFile(hi, readBuffer, disk_offset.QuadPart, &offsetCount, 0)) // These are not multiples of 512 or 4096, hence the "Invalid argument" error.
	//{
	//	std::cout << "readfile error" <<  std::endl;
	//}

	
	return true;
}

	

