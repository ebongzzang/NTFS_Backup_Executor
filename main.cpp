#include "stdafx.h"
#include "IVHD.h"
#include "LocalManager.h"


using std::ifstream;
using std::ofstream;
using std::string;

int main(void)
{
	IVHD vhd(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024);

	if ((!vhd.InitVHD()))
			return false;
	if ((!vhd.writeFooter()))
			return false;
	if ((!vhd.writeDynamicHeader()))
			return false;
	vhd.WriteBatMap(); //need Exception
	if ((!vhd.writeFooter()))
		return false;
	void * readBuffer1;

	LocalManager * manager = new LocalManager("C");
	manager->setDiskHandle();
	readBuffer1 = manager->readMBR();


	return true;
}

	

