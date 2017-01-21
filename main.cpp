#include "stdafx.h"
#include "IVHD.h"
using std::ifstream;
using std::ofstream;
using std::string;

int main(void)
{
	IVHD * vhd = new IVHD(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024);
	
	//if (!(vhd->InitVHD()))
		//	return -1;
	//	if (!(vhd->writeFooter()))
			//	return -1;
		//	if (!(vhd->writeDynamicHeader()))
				//	return -1;
				vhd->InitVHD();
	vhd->writeFooter();
	vhd->writeDynamicHeader();
	vhd->WriteBatMap();
	vhd->writeFooter();
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

