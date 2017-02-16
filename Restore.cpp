#include "Restore.h"

Restore::Restore(PlanB::BackupResult * _Source)
	: source(_Source)
{
	backupKey = source->backupKey;
	backupTargetPath = source->backupTargetPath;
	resultFilePath = source->resultFilePath;
	time = source->time;
	type = source->type;
	style = source->style;
	status = source->status;

}

Restore::~Restore()
{

}