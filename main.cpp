#include "IVHD.h"
#include "BackupExecutor.h"
#include "backup.h"
#include <memory>
#include <string>
#include <bitset>
#include <sstream>
#include "SystemBackup.h"
#include "RestoreExecutor.h"


#define NOT_USED_CLUSTER 4096 *8
using std::ifstream;
using std::ofstream;
using std::string;

int main(void)
{

	//IVHD *vhd = new IVHD(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024); //vhdtype, path, maxsize(mb)

	//if (!vhd->InitVHD())
	//	return false;

	//if (!vhd->writeVHDHeader())
	//	return false;

	//if (!vhd->writeMirrorFooter())
	//	return false;

	//char * readBuffer1 = new char[4096];
	clock_t begin, end;
	begin = clock();

	BackupExecutor * BackupEx= new BackupExecutor(01,"Q",PlanB::BackupStyle::FULL,PlanB::BackupType::System,FALSE, FALSE);
	BackupResult * result;
	result = new BackupResult;
	BackupEx->exec();
	RestoreExecutor * RestoreEx = new RestoreExecutor(result);
	end = clock();

	std::cout << "수행시간 " << ((end - begin) / CLOCKS_PER_SEC) << "초" << std::endl;





	
	return true;
}

	

