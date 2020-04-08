#ifndef TOPTAILCUTMEM_H_
#define TOPTAILCUTMEM_H_

#define IMAGE_WIDTH_MAX  4096
#define IMAGE_HEIGHT_MAX 2000
#define CHANNEL_NUM_MAX   2
#define IMAGE_ARRAY_SIZE  100
#define PACK_ARRAY_SIZE   50

#define LOGINFO_ARRAY_SIZE 100
#define LOGINFO_TIME_BUFSIZE 32
#define LOGINFO_INFO_BUFSIZE 256

#include <windows.h>


struct FrameInfo
{
	FrameInfo()
	{
		memset(&FrameTm, 0, sizeof(FrameTm));
		BlockID = 0;
		Channel = 0;
		ImageWidth = 0;
		ImageHeight = 0;
		memset(&ImageBuff, 0, sizeof(ImageBuff));
	}

	unsigned long        FrameTm;
	unsigned long long   BlockID;
	unsigned int         Channel;
	unsigned int         ImageWidth;
	unsigned int         ImageHeight;
	unsigned char        ImageBuff[IMAGE_WIDTH_MAX*IMAGE_HEIGHT_MAX];
};

struct PackData
{
	PackData()
	{
		PackID = 0;

		for (int i = 0; i < CHANNEL_NUM_MAX; i++)
		{
			PackIndex[i] = LONG_MAX;
		}
	}

	unsigned PackID;
	long PackIndex[CHANNEL_NUM_MAX];
};


struct PackDataOutput
{
	PackDataOutput()
	{
		for (int i = 0; i < CHANNEL_NUM_MAX; i++)
		{
			pPackImage[i] = nullptr;
		}
	}
	unsigned PackID;
	FrameInfo* pPackImage[CHANNEL_NUM_MAX];
};

//图像数据信息
struct StructDataSpace
{
	// 原始数据
	FrameInfo ImageArray[IMAGE_ARRAY_SIZE];
	long ImageIndex;

	// 打包数据
	PackData PackArray[PACK_ARRAY_SIZE];
	long PackIndex;
};

// 日志信息
struct StructLogInfo
{
	StructLogInfo()
	{
		Type = 0;
		memset(Time, 0, sizeof(Time));
		memset(Info, 0, sizeof(Info));
	}
	int   Type;  // 类型  0 信息，1警告， 2错误
	char  Time[LOGINFO_TIME_BUFSIZE];
	char  Info[LOGINFO_INFO_BUFSIZE];
};

struct StructLogInfoSpace
{
	StructLogInfo LogInfoArray[LOGINFO_ARRAY_SIZE];
	long    LogInfoIndex;
};

struct StructOtherInfoSpace
{
	int CameraInfo;//  相机打开标志
	int OtherInfo[19]; //其他方便扩展
};

struct TopTailMem
{
	StructDataSpace DataSpace;
	StructLogInfoSpace    LogInfoSpace;
	StructOtherInfoSpace  OtherInfoSpace;
};

#endif
