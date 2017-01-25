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
	HANDLE hi = CreateFile(L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy5", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	std::cout << "wow!! error! " << GetLastError() << std::endl;
	LARGE_INTEGER disk_offset;
	DWORD offsetCount;
	char readBuffer[4096];
	disk_offset.QuadPart = 0;


	if (!ReadFile(hi, readBuffer, 4096, &offsetCount, 0)) // These are not multiples of 512 or 4096, hence the "Invalid argument" error.
	{
		std::cout << "readfile error" << GetLastError() << std::endl;
	}
	for (int i = 0; i < 4096; i++)
	{

	}

		std::ofstream mbrtest;
		mbrtest.open("C:\\hi.mbr", std::ios::binary);
		mbrtest.write(readBuffer, 4096);
	return true;
}

	

