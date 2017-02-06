#include "stdafx.h"
#include "IVHD.h"
#include "LocalManager.h"
#include <memory>
#include <string>
#include <boost/utility/binary.hpp>
#include <bitset>
#include <sstream>
#define BITMAP_CHUNK_SIZE 32*1024 
#define GET_LCN_FROM_BITMAP(i) (((i)/(0x20)))
#define NOT_USED_CLUSTER 4096 *8
using std::ifstream;
using std::ofstream;
using std::string;

int main(void)
{

	IVHD *vhd = new IVHD(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024); //vhdtype, path, maxsize(mb)

	if (!vhd->InitVHD())
		return false;
	if (!vhd->writeVHDHeader())
		return false;
	if (!vhd->writeMirrorFooter())
		return false;
	char * readBuffer1 = new char[4096];

	LocalManager * manager = new LocalManager("C");

	manager->setDiskHandle();
	//readBuffer1 = manager->readMBR();

	//for (int i = 0; i < 512; i++)
	//{
	//	std::cout << readBuffer1[i] << std::endl;
	//}
	HANDLE hi = CreateFile(L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy29", READ_CONTROL | WRITE_OWNER | WRITE_DAC | GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	
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

	bitmapSize = BITMAP_CHUNK_SIZE + sizeof(LARGE_INTEGER) * 2;

	bitmapResult = new VOLUME_BITMAP_BUFFER[bitmapSize];
	HANDLE file = CreateFile(L"\\\\?\\E:\\hi.bit", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);

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

		WriteFile(file, bitmapResult->Buffer, bytesReturned, &bytesWrited, NULL);
		if (TRUE == ret)
		{
			break;
		}

		/* Update the read offset for next request */
		bitmapStart.StartingLcn.QuadPart += bytesReturned * 8;
	}

	auto rawfile = CreateFile(L"\\\\?\\D:\\hi.raw", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);

	clock_t begin, end;
	begin = clock();


	
	LARGE_INTEGER readoffset;
	readoffset.QuadPart = 0;
	std::ifstream infile("E:\\hi.bit", std::ios::binary);

		infile.unsetf(std::ios::skipws);
		std::streampos fileSize;

		infile.seekg(0, std::ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, std::ios::beg);
		
		std::vector<BYTE> vec;
		
		// reserve capacity
		vec.reserve(fileSize);
		std::cout << "filesize" << fileSize << std::endl;
		std::vector <unsigned int> bitmapVector;  //stored non-used cluster offset Vector
		bitmapVector.reserve(fileSize);

		// read the data:
		
		vec.insert(vec.begin(),std::istream_iterator<BYTE>(infile),std::istream_iterator<BYTE>());
		std::vector<BYTE>::iterator it = vec.begin();
		unsigned int temp;
		for ( ; it != vec.end(); it++)
		{
			std::bitset<8> bitset(*it); // 1byte(8bit)
			for (std::size_t i = 0; i < bitset.size(); i++) 
			{
	
				if (bitset.test(i))
				{  
					temp = (((it - vec.begin()) * 8) + i);
					
					bitmapVector.push_back(temp);
					//current byte * sizeof(byte) + bitset index
					//hash
				}
				else
				{
					continue;
				}
			}
		}

		
		DWORD count;
		
		LARGE_INTEGER asdf;
		asdf.QuadPart = 0; //cluster size

		if (!SetFilePointerEx(hi, asdf, NULL, FILE_BEGIN)) //set filepointer on Start location
		std::cout << "first file set error" << GetLastError() << std::endl;

		std::vector<unsigned int>::iterator it2 = bitmapVector.begin();
		LARGE_INTEGER newoffset;
		for ( ; it2 != bitmapVector.end(); ++it2 )
		{
			if (!SetFilePointerEx(hi, asdf, &newoffset, FILE_CURRENT))
			{

				std::cout << "set pointer error" << GetLastError() << std::endl;
	
				std::cout << "value *it:" << *it2 << std::endl;
				std::cout << "asdf.quad" << asdf.QuadPart << std::endl;
				Sleep(3000);
			} 
			if (std::next(it2, 1) != bitmapVector.end())
			{
				auto temp = std::next(it2, 1);
				asdf.QuadPart = (*temp - *it2) * 512;

			}
			else
				break;


			if (!ReadFile(hi, readBuffer1, 4096 , &count, 0))
			std::cout << "read copy" << GetLastError() << std::endl;

			if (!WriteFile(rawfile, readBuffer1, 4096, &count, NULL))
				std::cout << "write copy" << GetLastError() << std::endl;
		


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

	

