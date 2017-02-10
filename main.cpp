#include "IVHD.h"
#include "BackupExecutor.h"
#include "backup.h"
#include <memory>
#include <string>
#include <bitset>
#include <sstream>
#include "SystemBackup.h"


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

	BackupExecutor * executor = new BackupExecutor(01,"C",PlanB::BackupStyle::FULL,PlanB::BackupType::System,FALSE,FALSE);



	//executor->setDiskHandle();

	//readBuffer1 = manager->readMBR();

	//for (int i = 0; i < 512; i++)
	//{
	//	std::cout << readBuffer1[i] << std::endl;
	//}



	
	return true;
}

	

