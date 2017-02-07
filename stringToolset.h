#pragma once
#include "stdafx.h"
#ifndef STRINGTOOL_H
#define STRINGTOOL_H


	std::wstring strTowstr(const std::string& s)
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
	std::wstring convertDriveLetter(std::string dLetter)
		//Convert driveLetter to UNC path
	{
		std::string tmpString;
		std::wstring tmpWstring;

		tmpString = +"\\\\.\\" + dLetter + ":";
		tmpWstring = strTowstr(tmpString);
		return tmpWstring;

	}

	std::wstring determinePhysicalDisk(HANDLE drive)
		// determine parameter's Physical disk.
	{
		VOLUME_DISK_EXTENTS diskExtents;
		DWORD dwSize;

		if (!DeviceIoControl(drive, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &diskExtents, sizeof(diskExtents), &dwSize, NULL))
			std::cout << "GET_VOLUME_DISK_EXTENTS ERROR! " << GetLastError() << std::endl;

		std::string tmpString = "\\\\.\\PhysicalDrive" + std::to_string(diskExtents.Extents[0].DiskNumber);

		return strTowstr(tmpString);
	}

#endif