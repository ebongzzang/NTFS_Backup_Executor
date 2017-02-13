#include "SystemBackup.h"
#include <iostream>
#include <chrono>
#include <time.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <iterator>
#include <bitset>
#include <array>
#include <iomanip>
#define BITMAP_CHUNK_SIZE 32*1024 

SystemBackup::~SystemBackup()
{
	delete[] mbrBuffer;
	CloseHandle(physicalDiskHandle);
}

PlanB::JobStatus SystemBackup::exec()
{
	if (!readMBR())
	{
		std::cout << "ReadMBR Failed." << std::endl;
		return PlanB::JobStatus::FAILED;
	}

	VSS_SNAPSHOT_PROP vssProp;
	vssProp = takeSnapshot();
	
	vssHandle = CreateFile(vssProp.m_pwszSnapshotDeviceObject, READ_CONTROL | WRITE_OWNER | WRITE_DAC | GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	//open vssfolder

	if (vssHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "shadow copy handle error:" << GetLastError() << std::endl;
		return PlanB::JobStatus::FAILED;
	}
	std::wstring filename = writeBitmap();

	exportResultFile(filename);


	return PlanB::JobStatus::SUCCESS;


	
}

bool SystemBackup::readMBR() //TODO : get Sector Size dynamically
{

	physicalDiskHandle = CreateFile(physicalDisk.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (physicalDiskHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "set DiskHandle Error!" << GetLastError() << std::endl;
		return false;
	}

	LARGE_INTEGER disk_offset;

	mbrBuffer = new char[SECTOR_SIZE]; // MBR = first sector of Physicaldisk

	disk_offset.QuadPart = 0;
	if (!SetFilePointerEx(physicalDiskHandle, disk_offset, 0, FILE_BEGIN))
	{
		std::cout << "filePointer error" << GetLastError() << std::endl;
		return false;
	}


	if (!ReadFile(physicalDiskHandle, mbrBuffer, SECTOR_SIZE, &count,NULL))

	{
		std::cout << "readfile error" << GetLastError() << std::endl;
		return false;
	}


	CloseHandle(physicalDiskHandle);
	return true;

	//ofstream mbrtest;
	//mbrtest.open("C:\\hi.mbr", std::ios::binary);
	//mbrtest.write(readBuffer, 512);

}


std::wstring SystemBackup::determinePhysicalDisk(HANDLE drive)
// determine parameter's Physical disk.
{
	VOLUME_DISK_EXTENTS diskExtents;
	DWORD dwSize;

	if (!DeviceIoControl(drive, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &diskExtents, sizeof(diskExtents), &dwSize, NULL))
		std::cout << "GET_VOLUME_DISK_EXTENTS ERROR! " << GetLastError() << std::endl;

	std::string tmpString = "\\\\.\\PhysicalDrive" + std::to_string(diskExtents.Extents[0].DiskNumber);

	return strTowstr(tmpString);
}

std::wstring SystemBackup::writeBitmap()
{
	/*auto nowtime = std::chrono::system_clock::now();
	time_t t = std::chrono::system_clock::to_time_t(nowtime);*/

	std::string filenameTemp = std::to_string(backupKey) + "_" +".bitmap";
	auto filename = strTowstr(filenameTemp);

	HANDLE bitmapOutHandle = CreateFile(filename.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);
	//TODO: change result path Dynamically

	if (bitmapOutHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "bitmap handle error:" << GetLastError() << std::endl;
	}

	STARTING_LCN_INPUT_BUFFER bitmapStart;
	VOLUME_BITMAP_BUFFER * bitmapResultBuf;
	DWORD bytesReturned;

	uint32_t bitmapSize = BITMAP_CHUNK_SIZE + sizeof(LARGE_INTEGER) * 2; //32kb
	bitmapResultBuf = new VOLUME_BITMAP_BUFFER[bitmapSize];
	bitmapStart.StartingLcn.QuadPart = 0;

	while (TRUE)//Read NTFS bitmap per 32kb
	{
		bool bitmap = DeviceIoControl(vssHandle, FSCTL_GET_VOLUME_BITMAP, &bitmapStart, sizeof(STARTING_LCN_INPUT_BUFFER), bitmapResultBuf, bitmapSize, &bytesReturned, NULL);

		if (bitmapStart.StartingLcn.QuadPart != bitmapResultBuf->StartingLcn.QuadPart)
		{
			std::cout << "GET_VOLUME_BITMAP Failed." << GetLastError() << std::endl;
		}

		if (!bitmap)
		{
			if (GetLastError() != ERROR_MORE_DATA) 
			{
				return false;
			}

		}

		bytesReturned -= sizeof(LARGE_INTEGER) * 2;
		WriteFile(bitmapOutHandle, bitmapResultBuf->Buffer, bytesReturned, &count, NULL);
		//Write Bitmap.

		if (bitmap)
		{
			break;
			
		}

		/* Update the read offset for next request */
		bitmapStart.StartingLcn.QuadPart += bytesReturned * 8;
	}
	return filename;
}
void SystemBackup::exportResultFile(std::wstring bitmapFileName)
{

	std::ifstream bitmapFile(bitmapFileName);

	std::streampos fileSize;

	bitmapFile.seekg(0, std::ios::end);
	fileSize = bitmapFile.tellg();
	bitmapFile.seekg(0, std::ios::beg);

	std::vector<BYTE> readFileVector; //stored bitmap file 
	readFileVector.reserve(fileSize);
	// reserve capacity

	std::vector <unsigned int> usedBitVector;  //used cluster vector
	usedBitVector.reserve(fileSize * 8);
	std::vector <unsigned int> nonUsedBitVector;
	nonUsedBitVector.reserve(fileSize * 8);

	LARGE_INTEGER readoffset;
	readoffset.QuadPart = 0;


	readFileVector.insert(readFileVector.begin(), std::istream_iterator<BYTE>(bitmapFile), std::istream_iterator<BYTE>());
	/* read the data*/
	readFileVector.shrink_to_fit();

	unsigned int clusterIndex;
	int i;
	/* if bitmap each bit == 1, put into usedBitVector */
	std::cout << readFileVector.size() << std::endl;
	for (std::vector<BYTE>::iterator fileIt = readFileVector.begin();
		fileIt != readFileVector.end(); ++fileIt) //iterate bitmap Vector
	{

		std::bitset<8> bitset((*fileIt)); // 1byte(8bit)
		for (i = bitset.size() - 1; i >= 0; i--) //Vector Element(byte) to binary(bitset)
		{

			if (bitset[i] == 1)
			{
				clusterIndex = (((fileIt - readFileVector.begin()) * 8) + i);
				usedBitVector.push_back(clusterIndex);
				// byte offset * sizeof(byte) + bitset index = cluster number
			}

			else
			{
				clusterIndex = (((fileIt - readFileVector.begin()) * 8) + i);
				nonUsedBitVector.push_back(clusterIndex);
			}
		}
	}
	usedBitVector.shrink_to_fit();
	nonUsedBitVector.shrink_to_fit();
	std::cout << "used vector size" << usedBitVector.size() << std::endl;
	Sleep(3000);
	std::cout << "non-used vector size" << nonUsedBitVector.size() << std::endl;
	Sleep(3000);
	LARGE_INTEGER vssOffset;
	vssOffset.QuadPart = 0;

	if (!SetFilePointerEx(vssHandle, vssOffset, NULL, FILE_BEGIN)) //Set filepointer on start location
		std::cout << "first file set error" << GetLastError() << std::endl;


	char readBuffer[4096];
	HANDLE rawfile = CreateFile(L"result.raw", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);

	for (std::vector<unsigned int>::iterator bitIt = usedBitVector.begin();
		bitIt != usedBitVector.end(); ++bitIt)
	{

		if (!SetFilePointerEx(vssHandle, vssOffset, 0, FILE_CURRENT))
		{
			std::cout << "set pointer error" << GetLastError() << std::endl;
		}

		if (!ReadFile(vssHandle, readBuffer, CLUSTER_SIZE, &count, 0))
			std::cout << "vssfile read fail." << GetLastError() << std::endl;

		if (!WriteFile(rawfile, readBuffer, CLUSTER_SIZE, &count, NULL))
			std::cout << "bitmap file write fail" << GetLastError() << std::endl;

		if (std::next(bitIt, 1) != usedBitVector.end())
		{
			auto nextOffset = std::next(bitIt, 1);

			if (*nextOffset - *bitIt != 1)
			{
				vssOffset.QuadPart = ((*nextOffset - *bitIt) * CLUSTER_SIZE) - CLUSTER_SIZE;
				/* ((next used offset - current used offset) * 4096) - 4096 */

	/*			nonUsedBitVector.push_back((*bitIt));
				nonUsedBitVector.push_back((*nextOffset - *bitIt));*/

			}

		}

		else
			break;
	}



	std::ofstream nonUsed("non_used.bitmap",std::ios::binary);
	
	for (std::vector<unsigned int>::iterator fileIt5 = nonUsedBitVector.begin();
		fileIt5 != nonUsedBitVector.end(); ++fileIt5) //iterate bitmap Vector
	{
		nonUsed.write(reinterpret_cast<const char *>(&*fileIt5),sizeof(unsigned int));
	}
	std::ifstream bitmapFile2("non_used.bitmap", std::ios::binary);
	std::streampos fileSize2;

	bitmapFile2.seekg(0, std::ios::end);
	fileSize2 = bitmapFile2.tellg();
	bitmapFile2.seekg(0, std::ios::beg);

	std::vector<unsigned int> readFileVector2; //stored bitmap file 
	readFileVector2.reserve(fileSize2 * 8);

	readFileVector2.insert(readFileVector2.begin(), std::istream_iterator<char>(bitmapFile2), std::istream_iterator<char>());
	readFileVector2.shrink_to_fit();

	HANDLE DISK = CreateFile(L"test.rec", GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	HANDLE VSSDISK = CreateFile(L"result.raw", GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	LARGE_INTEGER inputFileSize;
	LARGE_INTEGER tempoffset;
	LARGE_INTEGER newoffset;
	tempoffset.QuadPart = 0;
	GetFileSizeEx(VSSDISK, &inputFileSize);
	int j = 0;
	for (int i = 0; ((inputFileSize.QuadPart / CLUSTER_SIZE) + nonUsedBitVector.size()) > i; i++)
	{
		if (i == static_cast<unsigned int>(readFileVector2.at(j)))
		{
			memset(readBuffer, NULL, CLUSTER_SIZE);
			tempoffset.QuadPart = 4096;
			SetFilePointerEx(VSSDISK, tempoffset, &newoffset, FILE_CURRENT);
			j++;
			if (!WriteFile(DISK, readBuffer, CLUSTER_SIZE, &count, NULL))
				std::cout << "write file error" << GetLastError() << std::endl;
		}
		else
		{
			ReadFile(VSSDISK, readBuffer, CLUSTER_SIZE, &count, NULL);
			if (!WriteFile(DISK, readBuffer, CLUSTER_SIZE, &count, NULL))
				std::cout << "write file error" << GetLastError() << std::endl;
		}
		

	}

}
