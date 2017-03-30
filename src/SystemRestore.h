#ifndef SYSTEMRESTORE_H
#define SYSTEMRESTORE_H
#include "Restore.h"
#include "PlanBCommon.h"
using namespace PlanB;
class SystemRestore : public Restore
{
public:
	SystemRestore(BackupResult * _Source);
	virtual PlanB::JobStatus  exec();
	virtual PlanB::JobStatus cancel();  /*Not Implemented */
	~SystemRestore();

private:

};




#endif // !
