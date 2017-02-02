#include "stdafx.h"
#include "IVHD.h"
#include "LocalManager.h"
#include <memory>
#include <string>
#include <boost/utility/binary.hpp>
#include <bitset>

#define BITMAP_CHUNK_SIZE 32*1024 
#define GET_LCN_FROM_BITMAP(i) (((i)/(0x20)))
#define NOT_USED_CLUSTER 4096 *8
using std::ifstream;
using std::ofstream;
using std::string;

static const unsigned char BitsSetTable256[256] =
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
	B6(0), B6(1), B6(1), B6(2)
};

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
	HANDLE hi = CreateFile(L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy22", READ_CONTROL | WRITE_OWNER | WRITE_DAC | GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hi == INVALID_HANDLE_VALUE)
	{
		std::cout << "shadow copy handle error:" << GetLastError() << std::endl;
	}

	STARTING_LCN_INPUT_BUFFER bitmapStart;
	bitmapStart.StartingLcn.QuadPart = 0;
	VOLUME_BITMAP_BUFFER * bitmapResult;
	uint32_t bitmapSize;
	uint32_t bitmapFileCount=0;
	DWORD bytesReturned, bytesWrited;
	bitmapSize = BITMAP_CHUNK_SIZE + (sizeof(LARGE_INTEGER) * 2);
	//bitmapResult = (VOLUME_BITMAP_BUFFER *)malloc(bitmapSize);
	bitmapResult = new VOLUME_BITMAP_BUFFER[bitmapSize];
	auto file = CreateFile(L"\\\\?\\E:\\hi.bit", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		std::cout << "bitmap handle error:" << GetLastError() << std::endl;
	}

	while (1)
	{
		auto ret = DeviceIoControl(hi, FSCTL_GET_VOLUME_BITMAP, &bitmapStart, sizeof(STARTING_LCN_INPUT_BUFFER), bitmapResult, bitmapSize, &bytesReturned, NULL);
		if (bitmapStart.StartingLcn.QuadPart != bitmapResult->StartingLcn.QuadPart)
		{
			std::cout << "GET_VOLUME_BITMAP Failed." << GetLastError() << std::endl;
		}
		if (!ret)
		{
			if (GetLastError() != ERROR_MORE_DATA)
			{
				return false;
			}

		}
		bytesReturned -= sizeof(LARGE_INTEGER) * 2;
		bitmapFileCount += bytesReturned;
		LARGE_INTEGER offset;
		offset.QuadPart = 0;

		WriteFile(file, bitmapResult->Buffer, bytesReturned, &bytesWrited, NULL);
		if (TRUE == ret)
		{
			break;
		}
		/* Update the read offset for next request */
		bitmapStart.StartingLcn.QuadPart += bytesReturned * 8;
	}

	auto readfile = CreateFile(L"\\\\?\\E:\\hi.bit", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);
	auto rawfile = CreateFile(L"\\\\?\\E:\\hi.raw", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);
	BYTE * buff = new BYTE[bitmapSize];
	DWORD count;
	LARGE_INTEGER rawoffset;
	char * readBuffer = new char[32768];
	rawoffset.QuadPart = 0;

	
	clock_t begin, end;
	begin = clock();

	uint32_t i=0;
	std::vector <int> bitmapVector;
	bitmapVector.reserve(bitmapSize);
	BYTE and = 0xff;
	int offset;
	int offset2 = 0;
	while ((bitmapFileCount / bitmapSize) > i)
	{
		ReadFile(readfile, buff, bitmapSize, &count, NULL);
		int zgv;

			for (size_t j = 0; j < bitmapSize; j++)
			{
				std::bitset<8> bitset(buff[j]);
				for(int k=0; k<8; k++)
				{
					if (bitset[k] != 1)
					{
					offset = k + j + offset2 + (i * bitmapSize);
						bitmapVector.push_back(offset);
					
					}
				}
				offset2 += 7;
			}

		//if (bitset.any())
		//{
		//	std::wcout << "all cluster already used." << std::endl;
		//}
		//else
		//{
		//for (size_t j = 0; j < bitset.size(); j++)
		//{
		//	/*if (bitset[j] != 1)
		//	{*/
		//		offset = j + (i * 65536);
		//		//	bitmapvector.push_back(offset);
		//		std::cout << bitset[j] << "is added." << std::endl;
		//	//}

		//}
			//}
					/*if (!ReadFile(hi, readBuffer, 4096, &count, 0))
						std::cout << "read copy" << GetLastError() << std::endl;

					if(!WriteFile(rawfile,readBuffer,4096, &count,NULL))
					std::cout << "write copy" << GetLastError() << std::endl;*/
		i++;
		std::vector<int>::iterator it;
		for (it = bitmapVector.begin(); it != bitmapVector.end(); ++it)
		{
			std::cout << "non-used space: " << *it << std::endl;
			Sleep(1000);
		}
		bitmapVector.clear();
	}
		
	
		/*std::vector<BYTE>::iterator itBEgin = bitmapVector.begin();
		std::vector<BYTE>::iterator itEnd = bitmapVector.end();*/
		
		/*std::wcout << "vector clear" << std::endl;
		bitmapVector.clear();*/

		/*if (!WriteFile(rawfile, &*itBEgin, (sizeof(*itBEgin) * (itEnd - itBEgin)), &count, NULL))
			std::wcout << "it write error" << GetLastError() << std::endl;*/

	end = clock();
	std::cout << "수행시간 " << ((end - begin) / CLOCKS_PER_SEC) << "초" << std::endl;

	//int i = &buff;
	//std::cout << i << std::endl;
	/*
		readtest.seekg(0x4cd00);
		readtest.read(buff, sizeof(BYTE));
		writetest.write(buff, sizeof(BYTE));
		if (buff[0] == BOOST_BINARY(00000000))
		{
			std::cout << "빔" << std::endl;
		}
		readtest.seekg(0xca185);
		readtest.read(buff, sizeof(BYTE));
		std::cout << buff[0] << std::endl;*/



	/*LARGE_INTEGER disk_offset;
	DWORD offsetCount=0;
	char readBuffer[65536];
	disk_offset.QuadPart = 65536;
	std::ofstream mbrtest;
	mbrtest.open("E:\\hi.mbr", std::ios::binary);

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
	
	

	//if (!ReadFile(hi, readBuffer, disk_offset.QuadPart, &offsetCount, 0)) // These are not multiples of 512 or 4096, hence the "Invalid argument" error.
	//{
	//	std::cout << "readfile error" <<  std::endl;
	//}*/

	
	return true;
}

	

