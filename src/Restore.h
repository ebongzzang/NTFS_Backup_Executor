#ifndef RESTORE_H
#define RESTORE_H
#include "PlanBCommon.h"
using namespace PlanB;

class Restore
{
public:
	Restore(BackupResult * _Source);
	virtual PlanB::JobStatus  exec() = 0;
	virtual PlanB::JobStatus cancel() = 0;  /*Not Implemented */
	~Restore();

protected:

	BackupResult * source;
	BackupType type;
	BackupStyle style;
	JobStatus status;

	long backupKey;
	wstring backupTargetPath;
	wstring resultFilePath;
	string time;
private:
	


};


#endif