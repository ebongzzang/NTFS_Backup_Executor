#include "stdafx.h"
#include "IVHD.h"
#include "LocalManager.h"
#include <boost\filesystem.hpp>
using std::ifstream;
using std::ofstream;
using std::string;

using boostPath = boost::filesystem::path;
using boostFileStatus = boost::filesystem::file_status;
using namespace boost::filesystem;
int main(void)
{
	//IVHD vhd(VHD_DISK_TYPE_DYNAMIC, "C:\\test2.vhd", 1024);

	//if ((!vhd.InitVHD()))
	//		return false;
	//if ((!vhd.writeFooter()))
	//		return false;
	//if ((!vhd.writeDynamicHeader()))
	//		return false;
	//vhd.WriteBatMap(); //need Exception
	//if ((!vhd.writeFooter()))
	//	return false;
	//char * readBuffer1 = nullptr;

	//LocalManager * manager = new LocalManager("C");

	//manager->setDiskHandle();
	//readBuffer1 = manager->readMBR();

	//for (int i = 0; i < 512; i++)
	//{
	//	std::cout << readBuffer1[i] << std::endl;
	//}

	boostPath p("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy5\\");
	boostFileStatus s = status(p);

	std::cout << std::boolalpha << is_directory(s) << '\n';

	//directory_entry 

	directory_iterator end_itr; // default construction yields past-the-end
	for (directory_iterator itr(p); itr != end_itr; ++itr)
	{
		//if (is_directory(itr->status()))
		//{
		//	if (find_file(itr->path(), file_name, path_found)) return true;
		//}
		//else if (itr->leaf() == file_name) // see below
		//{
		//	path_found = itr->path();
		//	return true;
		//}
		std::cout << "path" << itr->path() << std::endl;
	}
	return true;
}

	

