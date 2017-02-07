#include "BackupExecutor.h"
#include <memory>
#include <string>
BackupExecutor::BackupExecutor(std::string _driveLetter) //example: C
{
	std::wstring uncPath = convertDriveLetter(_driveLetter);

	HANDLE handle = CreateFile(uncPath.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	//unique_ptr?

	if (handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Convert DriveLetter to UNC path failed.  " << GetLastError() << std::endl;
	}

	determinePhysicalDisk(handle);
	CloseHandle(handle);
}

BackupExecutor::~BackupExecutor()
{
	
}

HANDLE * BackupExecutor::getDiskHandle()
{
	if (diskHandle == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}

	return &diskHandle;
}
bool BackupExecutor::setDiskHandle()
{
	diskHandle = CreateFile(disk.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (diskHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "set DiskHandle Error!" << GetLastError() << std::endl;
		return false;
	}
	return true;
}
char * BackupExecutor::readMBR() //double pointer
{
	disk_offset.QuadPart = 0;
	if (!SetFilePointerEx(diskHandle, disk_offset, NULL, FILE_BEGIN))
	{
		std::cout << "filePointer error" << GetLastError() << std::endl;
		return nullptr;
	}
		

	if (!ReadFile(diskHandle, readBuffer, SECTOR_SIZE, &offsetCount, 0)) // These are not multiples of 512 or 4096, hence the "Invalid argument" error.
	{
		std::cout << "readfile error" << GetLastError() << std::endl;
		return nullptr;
	}

	/*GET_LENGTH_INFORMATION length;
	DWORD count;
	DeviceIoControl(diskHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &length, sizeof(GET_LENGTH_INFORMATION),&count,NULL);
	std::cout << "gem error" << GetLastError() <<std::endl;
	std::cout << length.Length.QuadPart << std::endl;
	*/
 	return readBuffer;

	//ofstream mbrtest;
	//mbrtest.open("C:\\hi.mbr", std::ios::binary);
	//mbrtest.write(readBuffer, 512);

}
std::wstring BackupExecutor::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
std::wstring BackupExecutor::convertDriveLetter(std::string dLetter)
//Convert driveLetter to UNC path
{
	std::string tempString;
	std::wstring tempWstring;

	tempString =+ "\\\\.\\" + dLetter + ":";
	tempWstring = s2ws(tempString);
	return tempWstring;

}

void BackupExecutor::determinePhysicalDisk(HANDLE drive) 	
// determine parameter's Physical disk.
{
	VOLUME_DISK_EXTENTS diskExtents;
	DWORD dwSize;

	if (!DeviceIoControl(drive, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &diskExtents, sizeof(diskExtents), &dwSize, NULL))
		std::cout << "GET_VOLUME_DISK_EXTENTS ERROR! " << GetLastError() << std::endl;

	std::string tempString = "\\\\.\\PhysicalDrive" + std::to_string(diskExtents.Extents[0].DiskNumber);
	std::wstring tempWstring = s2ws(tempString);

	disk = tempWstring;
}

