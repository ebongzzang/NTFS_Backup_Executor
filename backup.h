#ifndef BACKUP_H
#define BACKUP_H

#include <string>
#include <objbase.h>
#include "PlanBCommon.h"
#include <vss.h>
#include <vswriter.h>
#include <vsbackup.h>
#include <iostream>

class Backup
{
public:
	Backup(long _backupKey, std::wstring _sourcePath, std::wstring _destinationPath,
		bool _isCompressed, bool _isSplited);

	VSS_SNAPSHOT_PROP takeSnapshot();

	//virtual void changeJournal();  /*Not Implemented */
	virtual PlanB::BackupResult*  exec() = 0;
	virtual PlanB::JobStatus cancel() = 0; /*Not Implemented */
	virtual void compress(); /*Not Implemented */
	virtual void split(); /*Not Implemented */
	virtual ~Backup();

protected:
	long backupKey;
	std::wstring sourcePath;
	std::wstring destinationPath;
	bool isCompressed;
	bool isSplited;
	PlanB::BackupResult * result;


	std::wstring strTowstr(const std::string& s);
	std::wstring convertDriveLettertoUNC(std::string dLetter);
	string wstrTostr(const std::wstring& wstr);

private:
};


#endif