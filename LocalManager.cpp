#include "LocalManager.h"
#include <memory>
#include <string>
LocalManager::LocalManager(std::string _driveLetter) //example: C
{
	//Convert driveLetter to UNC path, subsequently determine parameter's Physical drive.

	std::string tempString;
	VOLUME_DISK_EXTENTS diskExtents;
	DWORD dwSize;
	std::wstring tempWstring;

	tempString =  + "\\\\.\\" + _driveLetter +  ":";
	tempWstring = s2ws(tempString);

	HANDLE handle = CreateFile(tempWstring.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Local Manager convert drive letter failed.  " << GetLastError() << std::endl;
	}

	DeviceIoControl(handle, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &diskExtents, sizeof(diskExtents), &dwSize,NULL);
	
	tempString = "\\\\.\\PhysicalDrive" + std::to_string(diskExtents.Extents[0].DiskNumber);
	tempWstring = s2ws(tempString);
	drive = tempWstring;

	CloseHandle(handle);

}

LocalManager::~LocalManager()
{
}
HANDLE * LocalManager::getDiskHandle()
{
	if (diskHandle == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}

	return &diskHandle;
}
bool LocalManager::setDiskHandle()
{
	diskHandle = CreateFile(drive.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (diskHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "createfile error" << GetLastError() << std::endl;
		return false;
	}
	return true;
}
char * LocalManager::readMBR() //double pointer
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

	return readBuffer;

	//ofstream mbrtest;
	//mbrtest.open("C:\\hi.mbr", std::ios::binary);
	//mbrtest.write(readBuffer, 512);

}
std::wstring LocalManager::s2ws(const std::string& s)
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


