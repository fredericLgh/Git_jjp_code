#include "TopTailSharedMemory.h"
#include "TopTailCutMem.h"

CTopTailSharedMemory::CTopTailSharedMemory(QObject *parent)
	:ImageReady(L"TopTailImageReady")
{

}

CTopTailSharedMemory::~CTopTailSharedMemory()
{
	Close();
}

bool CTopTailSharedMemory::Open(void)
{
	return CNoviceSharedMemory::Open("TopFlatSharedMemory", sizeof(TopTailMem));
}

void CTopTailSharedMemory::Close(void)
{
	CNoviceSharedMemory::Close();
}

void CTopTailSharedMemory::WriteFrameImage(UINT Channel, UINT64 BlockID, SYSTEMTIME Tm, BYTE* pBuf, UINT ImageWidth, UINT ImageHeight, long &WriteIndex)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

	long Index = p->ImageIndex;
	WriteIndex = Index;		//返回用的

	p->ImageArray[Index].FrameTm ;
	p->ImageArray[Index].Channel = Channel;
	p->ImageArray[Index].BlockID = BlockID;
	p->ImageArray[Index].ImageWidth = ImageWidth;
	p->ImageArray[Index].ImageHeight = ImageHeight;

	Q_ASSERT(ImageWidth*ImageHeight <= IMAGE_WIDTH_MAX*IMAGE_HEIGHT_MAX);
	memcpy(p->ImageArray[Index].ImageBuff, pBuf, ImageWidth*ImageHeight);


	Index++;
	Index %= IMAGE_ARRAY_SIZE;

	p->ImageIndex = Index;
	
}

bool CTopTailSharedMemory::WriteFrameRef(FrameInfo*& pFrame, long &WirteIndex)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return false;
	long Index = p->ImageIndex;

	WirteIndex = Index;
	pFrame = &p->ImageArray[WirteIndex];

	Index++;
	Index %= PACK_ARRAY_SIZE;
	p->ImageIndex = Index;
	return true;
}

void CTopTailSharedMemory::ReadFrameImage(FrameInfo* &pFrame, long &ReadIndex)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

	ReadIndex = (p->ImageIndex - 1 + IMAGE_ARRAY_SIZE) % IMAGE_ARRAY_SIZE;
	pFrame = &p->ImageArray[ReadIndex];
}


void CTopTailSharedMemory::WritePackData(UINT FrameID, long Index0, long Index1)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

	long Index = p->PackIndex;

	p->PackArray[Index].PackID = FrameID;
	p->PackArray[Index].PackIndex[0] = Index0;
	p->PackArray[Index].PackIndex[1] = Index1;

	Index++;
	Index %= PACK_ARRAY_SIZE;

	p->PackIndex = Index;
	ImageReady.pulse();		//通知图片可用
}

void CTopTailSharedMemory::ResetPackIndex()
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

     p->PackIndex =0;
}

void CTopTailSharedMemory::ReadPackData(PackData* &pStereo, long &ReadIndex)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

	ReadIndex = (p->PackIndex - 1 + PACK_ARRAY_SIZE) % PACK_ARRAY_SIZE;
	pStereo = &p->PackArray[ReadIndex];
}


void CTopTailSharedMemory::ReadPackData(PackDataOutput &Stereo, long &ReadIndex)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

	ReadIndex = (p->PackIndex - 1 + PACK_ARRAY_SIZE) % PACK_ARRAY_SIZE;
	PackData *pPack = &p->PackArray[ReadIndex];

	Stereo.PackID = pPack->PackID;

	for (int i = 0; i < CHANNEL_NUM_MAX; i++)
	{
		if (pPack->PackIndex[i] != LONG_MAX)
		{
			Stereo.pPackImage[i] = &p->ImageArray[pPack->PackIndex[i]];
		}
	}
}

void CTopTailSharedMemory::ReadIndexPackData(PackDataOutput &Stereo, const long ReadIndex)
{
	auto p = GetPtrDataSpace();
	if (p == NULL) return;

	if (ReadIndex >= PACK_ARRAY_SIZE) return;

	PackData *pPack = &p->PackArray[ReadIndex];

	Stereo.PackID = pPack->PackID;

	for (int i = 0; i < CHANNEL_NUM_MAX; i++)
	{
		if (pPack->PackIndex[i] != LONG_MAX)
		{
			Stereo.pPackImage[i] = &p->ImageArray[pPack->PackIndex[i]];
		}
	}
}


void CTopTailSharedMemory::WriteLogInfo(StructLogInfo& LogInfo)
{
	auto p = GetPtrLogInfoSpace();
	if (p == NULL) return;

	long Index = p->LogInfoIndex;
	memcpy(&p->LogInfoArray[Index], &LogInfo, sizeof(StructLogInfo));

	Index++;
	Index %= LOGINFO_ARRAY_SIZE;

	p->LogInfoIndex = Index;
}

void CTopTailSharedMemory::WriteLogInfo(StructLogInfo& LogInfo, long &WriteIndex)
{
	auto p = GetPtrLogInfoSpace();
	if (p == NULL) return;

	long Index = p->LogInfoIndex;
	WriteIndex = Index;		//返回用的

	memcpy(&p->LogInfoArray[Index], &LogInfo, sizeof(StructLogInfo));

	Index++;
	Index %= LOGINFO_ARRAY_SIZE;

	p->LogInfoIndex = Index;
}

void CTopTailSharedMemory::WriteLogInfo(const int Type,const std::string& TimerStr, const std::string& Info)
{
	auto p = GetPtrLogInfoSpace();
	if (p == NULL) return;

	long Index = p->LogInfoIndex;
	
	auto& StructInfo = p->LogInfoArray[Index];
	StructInfo.Type= Type;

	int Size = TimerStr.length() > LOGINFO_TIME_BUFSIZE - 1? LOGINFO_TIME_BUFSIZE - 1: TimerStr.length();
	memcpy(&StructInfo.Time, TimerStr.c_str(), Size);
	StructInfo.Time[Size] = 0;

	Size = Info.length() > LOGINFO_INFO_BUFSIZE - 1 ? LOGINFO_INFO_BUFSIZE - 1 : Info.length();
	memcpy(&StructInfo.Info, Info.c_str(), Size);
	StructInfo.Info[Size] = 0;


	Index++;
	Index %= LOGINFO_ARRAY_SIZE;

	p->LogInfoIndex = Index;
}

void CTopTailSharedMemory::ReadLogInfo(StructLogInfo* &pLogInfo, long &ReadIndex)
{
	auto p = GetPtrLogInfoSpace();
	if (p == NULL) return;

	ReadIndex = (p->LogInfoIndex - 1 + LOGINFO_ARRAY_SIZE) % LOGINFO_ARRAY_SIZE;
	pLogInfo =  &p->LogInfoArray[ReadIndex];
}

void CTopTailSharedMemory::ReadIndexLogInfo(StructLogInfo* &pLogInfo, const long &ReadIndex)
{
	auto p = GetPtrLogInfoSpace();
	if (p == NULL) return;
	pLogInfo = &p->LogInfoArray[ReadIndex];
}

void CTopTailSharedMemory::WriteCameraInfo(bool IsOpen)
{
	TopTailMem* pMem = static_cast<TopTailMem*>(CNoviceSharedMemory::GetBuffer());
	if(!pMem) return;

	pMem->OtherInfoSpace.CameraInfo = IsOpen ? 1 : 0;
}

bool CTopTailSharedMemory::ReadCameraInfo()
{
	TopTailMem* pMem = static_cast<TopTailMem*>(CNoviceSharedMemory::GetBuffer());
	if (!pMem) return false;

	return pMem->OtherInfoSpace.CameraInfo == 0 ? false : true;
}

StructDataSpace* CTopTailSharedMemory::GetPtrDataSpace(void)
{
	TopTailMem* pMem = static_cast<TopTailMem*>(CNoviceSharedMemory::GetBuffer());

	return pMem ? &(pMem->DataSpace) : nullptr;
}

StructLogInfoSpace*   CTopTailSharedMemory::GetPtrLogInfoSpace(void)
{
	TopTailMem* pMem = static_cast<TopTailMem*>(CNoviceSharedMemory::GetBuffer());

	return pMem ? &(pMem->LogInfoSpace) : nullptr;
}
