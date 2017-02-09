#ifndef BACKUP_H
#include <objbase.h>

#include <vss.h>
#include <vswriter.h>
#include <vsbackup.h>
#define BACKUP_H

class Backup
{
public:
	Backup();
	void takeSnapshot(std::string _driveLetter);
	~Backup();

private:

};


#endif