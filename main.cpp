#include "stdafx.h"
#include "IVHD.h"

#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096

using std::ifstream;
using std::ofstream;
using std::string;

int main(void)
{
	IVHD vhd(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024);

	char readBuffer[513] = { 0, };
	if ((!vhd.InitVHD()))
			return -1;
	if ((!vhd.writeFooter()))
			return -1;
	if ((!vhd.writeDynamicHeader()))
			return -1;
	
	vhd.WriteBatMap(); //need Exception

	if ((!vhd.writeFooter()))
		return -1;

	/*vhd.InitVHD();
	vhd.writeFooter();
	vhd.writeDynamicHeader();*/
	

	LARGE_INTEGER offset;
	DWORD readOffset;
	offset.QuadPart = 0;

	HANDLE check = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (check == INVALID_HANDLE_VALUE)
	{
	std::cout << "createfile error" << GetLastError() << std::endl;
	return -1;
	}

	SetFilePointerEx(check, offset, NULL, FILE_BEGIN);
	std::cout << "filePointer error" << GetLastError() << std::endl;

	ReadFile(check, readBuffer, SECTOR_SIZE, &readOffset, 0); // These are not multiples of 512 or 4096, hence the "Invalid argument" error.

	std::cout << "readfile error" << GetLastError() << std::endl;

	ofstream mbrtest;
	mbrtest.open("C:\\hi.mbr", std::ios::binary);
	mbrtest.write(readBuffer, 512);

	//if (!ReadFile(hdevice, buffer, source_diskgeometry.BytesPerSector, NULL, &overlapped))
		//	{
	//std::cout << readBuffer << std::endl;
 	//ifstream hi("\\\\..\\PhysicalDrive2", std::ios::binary);
//	ofstream hello("C:\\hello", std::ios::binary);
//	hi.seekg(510,std::ios::beg)
//	std::copy(std::istreambuf_iterator<char>(hi), std::istreambuf_iterator<char>(), std::ostreambuf_iterator<char>(hello));
//	std::vector<char> buffer ((std::istreambuf_iterator<char>( hi)), (std::istreambuf_iterator<char>()));
//	for (std::vector<char>::iterator it = buffer.begin(); it != buffer.end(); ++it)
//	{
//		std::cout << *it << std::endl;
//	}
	
	// read binary data from file


	return 0;
}

	//HANDLE check = CreateFile(L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy29\\volume.txt", GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);
	/*HANDLE check = CreateFile(L"\\\\?\\C:\\", GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);

	//if (check == INVALID_HANDLE_VALUE)
	//{
	std::cout << "createfile error" << GetLastError() << std::endl;
	//		return -1;
		//}
	RETRIEVAL_POINTERS_BUFFER  retrieval_buffer;
	STARTING_VCN_INPUT_BUFFER  input;
	input.StartingVcn.QuadPart = 0;
	DWORD value;
	if (!(DeviceIoControl(check, FSCTL_GET_RETRIEVAL_POINTERS, &input, sizeof(STARTING_VCN_INPUT_BUFFER), &retrieval_buffer, sizeof(RETRIEVAL_POINTERS_BUFFER), &value, NULL)))
	{
		std::cout << "deviceio" << GetLastError() << std::endl;
		return -1;
	}
	std::cout << retrieval_buffer.Extents->Lcn.QuadPart << std::endl;
	std::cout << retrieval_buffer.Extents->NextVcn.QuadPart << std::endl;
	std::cout << retrieval_buffer.ExtentCount << std::endl;
	*/

