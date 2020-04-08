#ifndef TOPTAILSHAREDMEMORY_H_
#define TOPTAILSHAREDMEMORY_H_

#include "NoviceSharedMemory.h"
#include "named_event.h"


// 前向声明
class StructDataSpace;
class FrameInfo;
class PackData;
class PackDataOutput;
class StructLogInfo;
class StructLogInfoSpace;

class CTopTailSharedMemory : protected CNoviceSharedMemory
{
	Q_OBJECT

public:
	CTopTailSharedMemory(QObject *parent = Q_NULLPTR);
	~CTopTailSharedMemory();

	bool Open(void);
	void Close(void);

	void WriteFrameImage(UINT Channel, UINT64 BlockID, SYSTEMTIME Tm, BYTE* pBuf, UINT ImageWidth, UINT ImageHeight, long &WriteIndex);
	
	bool WriteFrameRef(FrameInfo*& pFrame, long &WirteIndex);

	void ReadFrameImage(FrameInfo* &pFrame, long &ReadIndex);
	


	void WritePackData(UINT FrameID, long Index0, long Index1);
	void ReadPackData(PackData* &pStereo, long &ReadIndex);
	void ReadPackData(PackDataOutput &Stereo, long &ReadIndex);
	void ResetPackIndex();

	void ReadIndexPackData(PackDataOutput &Stereo, const long ReadIndex);

	void WriteLogInfo(StructLogInfo& LogInfo);
	void WriteLogInfo(StructLogInfo& LogInfo, long &WriteIndex);
	void WriteLogInfo(const int Type, const std::string& TimerStr, const std::string& Info);

	void ReadLogInfo(StructLogInfo* &pLogInfo, long &ReadIndex);

	void ReadIndexLogInfo(StructLogInfo* &pLogInfo, const long &ReadIndex);

	void WriteCameraInfo(bool IsOpen);
	bool ReadCameraInfo();

private:
	StructDataSpace* GetPtrDataSpace(void);
	StructLogInfoSpace*    GetPtrLogInfoSpace(void);
public:
	squall::named_event ImageReady;			//图像采集完毕信号
};


#endif