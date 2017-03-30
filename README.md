# NTFS-Executor-Program(Windows NTFS Backup Program)


NTFS ����̺� �󿡼� VSS(Volume Shadow Copy Service)�� �̿��� Incremental, Diffrential Backup, Clone�� ���� �̹��� ���Ϸ� export �ϴ� ���α׷�


#Main function

1.NTFS Change Journal�� ����� Incremental, Diffrential Backup ���� (not-implemented)

2.NTFS ��Ÿ������ ($MFT - $BITMAP)�� ����� ���� ����(�̹���) ����� ����

3. VHD ��� ����(not-implemented)



#Environment Development

'Windows 7  x64' + 'Visual Studio 2015 Community (MSVC 2015)' + 'Visual C++' + 'Win32 API'


#SourceCode Analysis - /src ����

BackupExecutor - �̱���, ������ Executor�� ����� ������ ����(�ʵ�)

SystemBackup.cpp - MFT�� �����Ͽ� ���(�ʵ�)

FileBackup - �ܼ� ���� ��� (not-implemented)

SystemRestore.cpp - SystemBackup�� ������� Backup Information Result Struct�� �޾� ����(�ʵ�)

main.cpp (�ʵ�)



![alt tag](https://github.com/ebongzzang/NTFS_Backup_Executor/blob/master/NTFS_Backup_Executor_CD.png)

