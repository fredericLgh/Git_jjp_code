#include "MutilDalsaGige.h"
#include "CameraGrabApp.h"
#include "TopTailCutMem.h"

#include "CommonOperating.h"
#include <QTimer>
#include <iostream>

#define MAX_LIST_LENGTH			30
#define FRAMEDEV  100

// 0,125-10
#define GAIN_MIN 1
#define GAIN_MAX 10

CMutilDalsaGige::CMutilDalsaGige()
	: m_queueImg0(100)
	, m_queueImg1(100)
	, m_IsUseTurboDrive(true)
{
	m_LastFrameHostTimeL = 0;
	m_LastFrameHostTimeR = 0;
	m_GigELIsOK = false;
	m_GigERIsOK = false;

	m_bNeedReseSync = false;
	m_CameraSyncError = false;
	m_CameraNameLeft = "Camera00";
	m_CameraNameRight = "Camera01";
	m_pTimer = new QTimer(this);
	
	connect(m_pTimer, &QTimer::timeout, this, &CMutilDalsaGige::TimeOut);
	m_pTimer->start(100);

}


CMutilDalsaGige::~CMutilDalsaGige()
{
}

void CMutilDalsaGige::TimeOut()
{
	IsBothAccessible();
	emit SendCameraState(m_GigELIsOK, m_GigERIsOK);
	if (m_bNeedReseSync)
	{
		ReSyncCamera();
	}
}


int  CMutilDalsaGige::Open(const QString &UserName0, const QString &UserName1, UINT width, UINT height)
{
	m_CameraNameLeft = UserName0;
	m_CameraNameRight = UserName1;

	
	m_GigELIsOK = m_GigE1.Open(XferCallback1, this, m_CameraNameLeft,
		width, height, m_IsUseTurboDrive);
	
	m_GigERIsOK = m_GigE2.Open(XferCallback2, this, m_CameraNameRight,
		width, height, m_IsUseTurboDrive);

	emit SendCameraState(m_GigELIsOK, m_GigERIsOK);
	int state = 0;

	int Type = 0;
	QString StrInfo;
	if (m_GigELIsOK&&m_GigERIsOK)
	{	
		SetupCamera();
		StrInfo = QString::fromLocal8Bit("左右相机打开成功!");
	}
	else
	{
		if (!m_GigELIsOK)
		{
			state += 1;
			Type = 2;
			StrInfo += QString::fromLocal8Bit("左相机打开失败!  ");
		}
		else
		{
			StrInfo += QString::fromLocal8Bit("左相机打开成功!  ");
		}

		if (!m_GigERIsOK)
		{
			state +=2;
			Type = 2;
			StrInfo += QString::fromLocal8Bit("右相机打开失败!");
		}
		else
		{
			StrInfo += QString::fromLocal8Bit("右相机打开成功!");
		}
	}

	emit SendCameraInfo(Type, StrInfo);
	return state;
}

void CMutilDalsaGige::Close()
{
	if (m_GigE1.IsOpen())
	{
		m_GigE1.Close();
	}

	if (m_GigE2.IsOpen())
	{
		m_GigE2.Close();
	}

	// 写日志	
}

bool CMutilDalsaGige::IsBothOpen()
{
	return (m_GigE1.IsOpen() && m_GigE2.IsOpen());
}

void CMutilDalsaGige::StartGrabbing()
{
	if (IsBothAccessible())
	{
		SetupCamera();
		m_GigELIsOK = m_GigE1.StartGrab();
		m_GigERIsOK = m_GigE2.StartGrab();
	}
}

void CMutilDalsaGige::StopGrabbing()
{
	if (QDalsaGige::IsAccessible(m_CameraNameLeft))
	{
		m_GigE1.StopGrab();
	}

	if (QDalsaGige::IsAccessible(m_CameraNameRight))
	{
		m_GigE2.StopGrab();
	}

	if (!m_GigE1.IsOpen() || !m_GigE2.IsOpen())
	{
		return;
	}

	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerSelector", "LineStart");
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerMode", "Off");
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerSource", "rotaryEncoder1");
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerOverlap", "ReadOut");
	m_GigE1.m_pAcqDevice->SetFeatureValue("triggerLineCount", 1);



	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerSelector", "FrameStart");
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerMode", "Off");
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerSource", "Line4");
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerActivation", "RisingEdge"); //上升有效




	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSelector", "LineStart");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerMode", "Off");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSource", "rotaryEncoder1");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerOverlap", "ReadOut");
	m_GigE2.m_pAcqDevice->SetFeatureValue("triggerLineCount", 1);

	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSelector", "FrameStart");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerMode", "Off");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSource", "Line4");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerActivation", "RisingEdge"); //上升有效
}

bool CMutilDalsaGige::IsBothGrabbing()
{
	UINT64 NowTm = ::GetTickCount();

	//如果3秒内有收数据，则说明正在采集
	bool b1 = NowTm - m_LastFrameHostTimeL < 3000 ? true : false;
	bool b2 = NowTm - m_LastFrameHostTimeR < 3000 ? true : false;

	return (b1 && b2);
}

bool CMutilDalsaGige::IsBothAccessible()
{
	double Dus;
	if (m_GigE1.IsOpen())
	{
		m_GigELIsOK = QDalsaGige::IsAccessible(m_CameraNameLeft);
		
		if (m_GigELIsOK)
		{
			m_GigELIsOK = m_GigE1.m_pAcqDevice->GetFeatureValue("ExposureTime", &Dus);
		}	
	}
	else
	{
		m_GigELIsOK = false;
	}

	if (m_GigE2.IsOpen())
	{
		m_GigERIsOK = QDalsaGige::IsAccessible(m_CameraNameRight);
		if (m_GigERIsOK)
		{
			m_GigERIsOK = m_GigE2.m_pAcqDevice->GetFeatureValue("ExposureTime", &Dus);
		}
	}
	else
	{
		m_GigERIsOK = false;
	}


	return m_GigELIsOK&&m_GigERIsOK;
}

void CMutilDalsaGige::GetReSyncCamera(bool IsResync)
{
	if (IsResync&&IsResync != m_bNeedReseSync)
	{
		m_bNeedReseSync = IsResync;
	}
}

void CMutilDalsaGige::ReSyncCamera()
{
	//建议每隔半小时，对相机进行一次时间同步。因为每个相机的晶振都有误差，时间长了会有累积误差，不可忽略。

	if (!m_bNeedReseSync) return;

	if (IsBothAccessible())
	{
		//为了保证同步效果，同步期间禁止有数据传输。所以要停止采集。
		//OutputDebugString(L"\n停止采集\n");
		DWORD timeBegin = ::GetTickCount();
		StopGrabbing();
		m_queueImg0.clear();
		m_queueImg1.clear();
		::Sleep(200);

		StartGrabbing();
		m_bNeedReseSync = false;
		DWORD timeEND = ::GetTickCount();
		pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), std::string("相机复位成功，耗时")+ std::to_string(timeEND- timeBegin)+"  ms!");
		
	}
	else
	{
		pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), std::string("相机复位失败!"));
	}
}

void CMutilDalsaGige::CheckAndSolveSyncError()
{
	if (m_CameraSyncError)
	{
		ReSyncCamera();
	}
}

bool CMutilDalsaGige::SetExposureTime(int Channel, int us)
{
	if ((us > 3000) || (us < 4))//曝光时间范围为4~3000
		return false;
	if (Channel == 0)
	{
		if (QDalsaGige::IsAccessible(m_CameraNameLeft))
		{
			m_GigELIsOK = m_GigE1.m_pAcqDevice->SetFeatureValue("ExposureTime", double(us));	
		}
		else
		{
			m_GigELIsOK = false;
		}

		emit SendChannelCameraState(0, m_GigELIsOK);
		emit SendCameraInfo(m_GigELIsOK ? 0 : 1, QString::fromLocal8Bit("左相机设置曝光 ") + QString::number(us) + (m_GigELIsOK ? QString::fromLocal8Bit("成功！") : QString::fromLocal8Bit("失败！")));
		return m_GigELIsOK;
	}

	if (Channel == 1)
	{
		if (QDalsaGige::IsAccessible(m_CameraNameRight))
		{
			m_GigERIsOK = m_GigE2.m_pAcqDevice->SetFeatureValue("ExposureTime", double(us));
			
		}
		else
		{
			m_GigERIsOK = false;
		}
		emit SendChannelCameraState(1, m_GigERIsOK);

		emit SendCameraInfo(m_GigERIsOK ? 0 : 1, QString::fromLocal8Bit("右相机设置曝光 ") + QString::number(us) + (m_GigERIsOK ? QString::fromLocal8Bit("成功！") : QString::fromLocal8Bit("失败！")));
		return m_GigERIsOK;
	}

	return false;
}

bool CMutilDalsaGige::GetExposureTime(int Channel, int& us)
{
	double Dus;
	if (Channel == 0)
	{
		if (QDalsaGige::IsAccessible(m_CameraNameLeft))
		{
			
			m_GigELIsOK = m_GigE1.m_pAcqDevice->GetFeatureValue("ExposureTime", &Dus);
			if (m_GigELIsOK)
			{
				us = Dus;
			}
		}
		else
		{
			m_GigELIsOK = false;
		}

		return m_GigELIsOK;
	}

	if (Channel == 1)
	{
		if (QDalsaGige::IsAccessible(m_CameraNameRight))
		{	
			m_GigERIsOK = m_GigE2.m_pAcqDevice->GetFeatureValue("ExposureTime", &Dus);
			if (m_GigERIsOK)
			{
				us = Dus;
			}
		}
		else
		{
			m_GigERIsOK = false;
		}
		return m_GigERIsOK;
	}

	return false;
}

bool CMutilDalsaGige::SetGain(int Channel, int gain)
{
	gain = gain < GAIN_MIN ? GAIN_MIN : gain;
	gain = gain > GAIN_MAX ? GAIN_MAX : gain;
	if (Channel == 0)
	{
		if ((m_GigELIsOK = QDalsaGige::IsAccessible(m_CameraNameLeft)))
		{
			bool State = m_GigE1.m_pAcqDevice->SetFeatureValue("Gain", (double)gain);	
			emit SendCameraInfo(State ? 0 : 1, QString::fromLocal8Bit("左相机设置增益 ") + QString::number(gain) + (State ? QString::fromLocal8Bit("成功！") : QString::fromLocal8Bit("失败！")));
			return State;
		}
		emit SendChannelCameraState(0, m_GigELIsOK);
	}

	if (Channel == 1)
	{
		if ((m_GigERIsOK = QDalsaGige::IsAccessible(m_CameraNameRight)))
		{
			bool State = m_GigE2.m_pAcqDevice->SetFeatureValue("Gain", (double)gain);			
			emit SendCameraInfo(State ? 0 : 1, QString::fromLocal8Bit("右相机设置增益 ") + QString::number(gain) + (State ? QString::fromLocal8Bit("成功！") : QString::fromLocal8Bit("失败！")));
			return State;
		}
		emit SendChannelCameraState(1, m_GigERIsOK);
	}

	return false;
}

bool CMutilDalsaGige::GetGain(int Channel, int& gain)
{
	double DGain;
	if (Channel == 0)
	{
		if (QDalsaGige::IsAccessible(m_CameraNameLeft))
		{
			if (m_GigE1.m_pAcqDevice->GetFeatureValue("Gain", &DGain))
			{
				gain = DGain;
				return true;
			}
		}
	}

	if (Channel == 1)
	{
		if (QDalsaGige::IsAccessible(m_CameraNameRight))
		{
			if (m_GigE1.m_pAcqDevice->GetFeatureValue("Gain", &DGain))
			{
				gain = DGain;
				return true;
			}
		}
	}

	return false;
}


void CMutilDalsaGige::SetupCamera(void)
{
	SapAcqDevice &Device = *m_GigE1.m_pAcqDevice;
	BOOL b;

	SapAcqDevice &Device1 = *m_GigE2.m_pAcqDevice;
	BOOL b1;

	const bool InternalClockTrigger = false;

	if (InternalClockTrigger)
	{
		Device.SetDisplayStatusMode(3);//不显示通知
		Device1.SetDisplayStatusMode(3);//不显示通知
										//设置定时器信号
		b = Device.SetFeatureValue("timerSelector", "Timer1");
		b = Device.SetFeatureValue("timerMode", "Off");
		b = Device.SetFeatureValue("timerStartSource", "Timer1End");
		b = Device.SetFeatureValue("timerDelay", 0);
		b = Device.SetFeatureValue("timerDuration", 100);		//单位：us 默认100
		b = Device.SetFeatureValue("timerMode", "Active");


		//将定时器作为相机线触发源
		b = Device.SetFeatureValue("TriggerSelector", "LineStart");
		b = Device.SetFeatureValue("TriggerMode", "On");
		b = Device.SetFeatureValue("TriggerSource", "Timer1End");
		b = Device.SetFeatureValue("TriggerOverlap", "ReadOut");
		b = Device.SetFeatureValue("triggerLineCount", 1);


		//关闭场同步
		b = Device.SetFeatureValue("TriggerSelector", "FrameStart");
		b = Device.SetFeatureValue("TriggerMode", "Off");

	}
	else
	{
		//测宽程序中编写，在原始的帧同步中没有编写
		Device.SetDisplayStatusMode(3);//不显示通知
		Device1.SetDisplayStatusMode(3);//不显示通知

										//设置 Line1 和 Line2 信号接口（绿色1+，蓝色1-，黄色2+，灰色2-）
		b = Device.SetFeatureValue("LineSelector", "Line1");
		b = Device.SetFeatureValue("LineFormat", "RS422");
		b = Device.SetFeatureValue("LineInverter", FALSE);
		b = Device.SetFeatureValue("lineDebouncingPeriod", 0);
		b = Device.SetFeatureValue("lineElectricalTermination", "Disabled");

		b = Device.SetFeatureValue("LineSelector", "Line2");
		b = Device.SetFeatureValue("LineFormat", "RS422");
		b = Device.SetFeatureValue("LineInverter", FALSE);
		b = Device.SetFeatureValue("lineDebouncingPeriod", 0);
		b = Device.SetFeatureValue("lineElectricalTermination", "Disabled");


		//设置 Line4 信号接口（紫色4+，橙色4-）
		b = Device.SetFeatureValue("LineSelector", "Line4");
		b = Device.SetFeatureValue("LineFormat", "RS422");
		b = Device.SetFeatureValue("LineMode", "Input");
		b = Device.SetFeatureValue("LineInverter", FALSE);
		b = Device.SetFeatureValue("lineDebouncingPeriod", 0);
		b = Device.SetFeatureValue("lineElectricalTermination", "Disabled");



		//设定 Line1 与 Line2 为线触发源（行同步源）
		b = Device.SetFeatureValue("TriggerSelector", "LineStart");
		b = Device.SetFeatureValue("TriggerMode", "On");
		b = Device.SetFeatureValue("TriggerSource", "rotaryEncoder1");
		b = Device.SetFeatureValue("TriggerOverlap", "ReadOut");
		b = Device.SetFeatureValue("triggerLineCount", 1);


		b = Device.SetFeatureValue("rotaryEncoderOutputMode", "Motion");	//编码器无方向
		b = Device.SetFeatureValue("rotaryEncoderInputASource", "Line1");
		b = Device.SetFeatureValue("rotaryEncoderInputBSource", "Line2");
		b = Device.SetFeatureValue("rotaryEncoderSrcActivation", "AnyEdge");
		b = Device.SetFeatureValue("rotaryEncoderMultiplier", 1);
		b = Device.SetFeatureValue("rotaryEncoderDivider", 1);


		//设定 Line4 为帧触发源（场同步源）
		b = Device.SetFeatureValue("TriggerSelector", "FrameStart");
		b = Device.SetFeatureValue("TriggerMode", "On");
		b = Device.SetFeatureValue("TriggerSource", "Line4");
		b = Device.SetFeatureValue("TriggerActivation", "RisingEdge"); //上升有效


																	   //ADD

																	   //设置 Line1 和 Line2 信号接口（绿色1+，蓝色1-，黄色2+，灰色2-）
		b1 = Device1.SetFeatureValue("LineSelector", "Line1");
		b1 = Device1.SetFeatureValue("LineFormat", "RS422");
		b1 = Device1.SetFeatureValue("LineInverter", FALSE);
		b1 = Device1.SetFeatureValue("lineDebouncingPeriod", 0);
		b1 = Device1.SetFeatureValue("lineElectricalTermination", "Disabled");

		b1 = Device1.SetFeatureValue("LineSelector", "Line2");
		b1 = Device1.SetFeatureValue("LineFormat", "RS422");
		b1 = Device1.SetFeatureValue("LineInverter", FALSE);
		b1 = Device1.SetFeatureValue("lineDebouncingPeriod", 0);
		b1 = Device1.SetFeatureValue("lineElectricalTermination", "Disabled");


		//设置 Line4 信号接口（紫色4+，橙色4-）
		b1 = Device1.SetFeatureValue("LineSelector", "Line4");
		b1 = Device1.SetFeatureValue("LineFormat", "RS422");
		b1 = Device1.SetFeatureValue("LineMode", "Input");
		b1 = Device1.SetFeatureValue("LineInverter", FALSE);
		b1 = Device1.SetFeatureValue("lineDebouncingPeriod", 0);
		b1 = Device1.SetFeatureValue("lineElectricalTermination", "Disabled");



		//设定 Line1 与 Line2 为线触发源（行同步源）
		b1 = Device1.SetFeatureValue("TriggerSelector", "LineStart");
		b1 = Device1.SetFeatureValue("TriggerMode", "On");
		b1 = Device1.SetFeatureValue("TriggerSource", "rotaryEncoder1");
		b1 = Device1.SetFeatureValue("TriggerOverlap", "ReadOut");
		b1 = Device1.SetFeatureValue("triggerLineCount", 1);


		b1 = Device1.SetFeatureValue("rotaryEncoderOutputMode", "Motion");	//编码器无方向
		b1 = Device1.SetFeatureValue("rotaryEncoderInputASource", "Line1");
		b1 = Device1.SetFeatureValue("rotaryEncoderInputBSource", "Line2");
		b1 = Device1.SetFeatureValue("rotaryEncoderSrcActivation", "AnyEdge");
		b1 = Device1.SetFeatureValue("rotaryEncoderMultiplier", 1);
		b1 = Device1.SetFeatureValue("rotaryEncoderDivider", 1);


		//设定 Line4 为帧触发源（场同步源）
		b1 = Device1.SetFeatureValue("TriggerSelector", "FrameStart");
		b1 = Device1.SetFeatureValue("TriggerMode", "On");
		b1 = Device1.SetFeatureValue("TriggerSource", "Line4");
		b1 = Device1.SetFeatureValue("TriggerActivation", "RisingEdge"); //上升有效

	}

	////设定曝光时间
	//b = Device.SetFeatureValue("ExposureTime", double(m_ExposureTimeL));		//min：4  max：3000
	//																		//ADD
	//b1 = Device1.SetFeatureValue("ExposureTime", double(m_ExposureTimeR));

	//重置硬件时间戳
	Device.SetFeatureValue("GevTimestampControlReset", TRUE);
	//ADD
	Device1.SetFeatureValue("GevTimestampControlReset", TRUE);
}

void CMutilDalsaGige::XferCallback1(SapXferCallbackInfo *pInfo)
{
	ASSERT(pInfo->GetEventType() == SapXferPair::EventEndOfFrame);
	CMutilDalsaGige *pClass = (CMutilDalsaGige *)pInfo->GetContext();
	SapBuffer &Buf1 = *(pClass->m_GigE1.m_pBuffers);
	SapAcqDevice &Device1 = *(pClass->m_GigE1.m_pAcqDevice);
	if (pClass->m_bNeedReseSync)
		return;

	DWORD NowTime = ::GetTickCount();  //系统时钟,最多运行49天，18ms的误差

	pClass->m_LastFrameHostTimeL = NowTime;

	int SizeX = Buf1.GetWidth();
	int SizeY = Buf1.GetHeight();
	long ImgIndex = Buf1.GetIndex();

	// 写共享内存
	
	FrameInfo* pFrame = nullptr;
	long WirteIndex = 0;
	if (pApp->m_Memory.WriteFrameRef(pFrame, WirteIndex) && pFrame != nullptr)
	{
		Buf1.Read(0, SizeX * SizeY, pFrame->ImageBuff);
		pFrame->ImageWidth = SizeX;
		pFrame->ImageHeight = SizeY;
		pFrame->Channel = 0;
		pFrame->BlockID = ImgIndex;
		pFrame->FrameTm = NowTime;
		pClass->PushFrame(0, std::make_pair(pFrame, WirteIndex));
	}
}

void CMutilDalsaGige::XferCallback2(SapXferCallbackInfo *pInfo)
{
	ASSERT(pInfo->GetEventType() == SapXferPair::EventEndOfFrame);
	CMutilDalsaGige *pClass = (CMutilDalsaGige *)pInfo->GetContext();
	SapBuffer &Buf2 = *(pClass->m_GigE2.m_pBuffers);
	SapAcqDevice &Device2 = *(pClass->m_GigE2.m_pAcqDevice);
	if (pClass->m_bNeedReseSync)
		return;

	DWORD NowTime = ::GetTickCount();  //系统时钟,最多运行49天，18ms的误差

	pClass->m_LastFrameHostTimeR = NowTime;

	int SizeX = Buf2.GetWidth();
	int SizeY = Buf2.GetHeight();
	long ImgIndex = Buf2.GetIndex();

	// 写共享内存

	FrameInfo* pFrame = nullptr;
	long WirteIndex = 0;
	if (pApp->m_Memory.WriteFrameRef(pFrame, WirteIndex) && pFrame != nullptr)
	{
		Buf2.Read(0, SizeX * SizeY, pFrame->ImageBuff);
		pFrame->ImageWidth = SizeX;
		pFrame->ImageHeight = SizeY;
		pFrame->Channel = 0;
		pFrame->BlockID = ImgIndex;
		pFrame->FrameTm = NowTime;
		pClass->PushFrame(1, std::make_pair(pFrame, WirteIndex));
	}
}

void CMutilDalsaGige::PushFrame(int Channel, std::pair<FrameInfo*, long> pFrameInfo)
{
	ASSERT(Channel == 0 || Channel == 1);
	
	if (Channel == 0)
	{
		m_queueImg0.push(pFrameInfo);

		if (m_queueImg1.empty())
			return;

		auto& refLeft = m_queueImg0.back();
		ULONG TimeStamp = refLeft.first->FrameTm;
		while (!m_queueImg1.empty())
		{
			auto refRight = m_queueImg1.front();
			ULONG ItemTime2 = refRight.first->FrameTm;
			LONG AbsDev = ItemTime2 > TimeStamp ? ItemTime2 - TimeStamp : TimeStamp - ItemTime2;
			if (AbsDev < FRAMEDEV)
			{
				pApp->m_Memory.WritePackData(0, refLeft.second, refRight.second);
				m_queueImg0.pop();
				m_queueImg1.pop();
				break;
			}
			else if (ItemTime2 < TimeStamp)
			{
				pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), "相机1触发，2在前，删除相机2!");
				m_bNeedReseSync = true;
				break;
			}
			else
			{
				m_bNeedReseSync = true;
				break;
			}
		}

	}
	else
	{
		m_queueImg1.push(pFrameInfo);

		if (m_queueImg0.empty())
			return;

		auto& refRight = m_queueImg1.back();
		ULONG TimeStamp = refRight.first->FrameTm;
		while (!m_queueImg0.empty())
		{
			auto refLeft = m_queueImg0.front();
			ULONG ItemTime1 = refLeft.first->FrameTm;
			LONG AbsDev = ItemTime1 > TimeStamp ? ItemTime1 - TimeStamp : TimeStamp - ItemTime1;
			if (AbsDev < FRAMEDEV)
			{
				pApp->m_Memory.WritePackData(0, refLeft.second, refRight.second);
				m_queueImg0.pop();
				m_queueImg1.pop();
				break;
			}
			else if (ItemTime1 < TimeStamp)
			{
				pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), "相机1触发，2在前，删除相机2!");
				m_bNeedReseSync = true;
				break;
			}
			else
			{
				m_bNeedReseSync = true;
				break;
			}
		}
	}
}