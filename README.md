# NTFS-Executor-Program(Windows NTFS Backup Program)


NTFS 드라이브 상에서 VSS(Volume Shadow Copy Service)를 이용한 Incremental, Diffrential Backup, Clone을 단일 이미지 파일로 export 하는 프로그램


#Main function

1.NTFS Change Journal를 사용한 Incremental, Diffrential Backup 지원 (not-implemented)

2.NTFS 메타데이터 ($MFT - $BITMAP)을 사용한 단일 파일(이미지) 백업과 복원

3. VHD 출력 지원(not-implemented)



#Environment Development

'Windows 7  x64' + 'Visual Studio 2015 Community (MSVC 2015)' + 'Visual C++' + 'Win32 API'


#SourceCode Analysis - /src 폴더

BackupExecutor - 싱글톤, 각각의 Executor가 백업과 복원을 지시(필독)

SystemBackup.cpp - MFT를 포함하여 백업(필독)

FileBackup - 단순 파일 백업 (not-implemented)

SystemRestore.cpp - SystemBackup의 결과물인 Backup Information Result Struct를 받아 복구(필독)

main.cpp (필독)



![alt tag](https://github.com/ebongzzang/NTFS_Backup_Executor/blob/master/NTFS_Backup_Executor_CD.png)

