#pragma once
#ifndef VSSClass_H
#define VSSClass_H
#include <objbase.h>

#include <vss.h>
#include <vswriter.h>
#include <vsbackup.h>

#include <string>

class VSSClass
{

public:
	VSSClass(std::string _driveLetter);
	void takeSnapshot(std::string _driveLetter);
	std::wstring strTowstr(const std::string& s);
	~VSSClass();

private:

	
	
	std::wstring driveLetter;
	IVssBackupComponents *backupComponents;


	VSS_ID snapshotId; //snapshot pid
	VSS_ID snapshotSetId; //snapshot set id
	IVssAsync *async;
	VSS_SNAPSHOT_PROP snapshotProp;
};


#endif
