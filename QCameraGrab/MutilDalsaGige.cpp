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
		StrInfo = QString::fromLocal8Bit("��������򿪳ɹ�!");
	}
	else
	{
		if (!m_GigELIsOK)
		{
			state += 1;
			Type = 2;
			StrInfo += QString::fromLocal8Bit("�������ʧ��!  ");
		}
		else
		{
			StrInfo += QString::fromLocal8Bit("������򿪳ɹ�!  ");
		}

		if (!m_GigERIsOK)
		{
			state +=2;
			Type = 2;
			StrInfo += QString::fromLocal8Bit("�������ʧ��!");
		}
		else
		{
			StrInfo += QString::fromLocal8Bit("������򿪳ɹ�!");
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

	// д��־	
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
	m_GigE1.m_pAcqDevice->SetFeatureValue("TriggerActivation", "RisingEdge"); //������Ч




	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSelector", "LineStart");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerMode", "Off");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSource", "rotaryEncoder1");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerOverlap", "ReadOut");
	m_GigE2.m_pAcqDevice->SetFeatureValue("triggerLineCount", 1);

	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSelector", "FrameStart");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerMode", "Off");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerSource", "Line4");
	m_GigE2.m_pAcqDevice->SetFeatureValue("TriggerActivation", "RisingEdge"); //������Ч
}

bool CMutilDalsaGige::IsBothGrabbing()
{
	UINT64 NowTm = ::GetTickCount();

	//���3�����������ݣ���˵�����ڲɼ�
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
	//����ÿ����Сʱ�����������һ��ʱ��ͬ������Ϊÿ������ľ�������ʱ�䳤�˻����ۻ������ɺ��ԡ�

	if (!m_bNeedReseSync) return;

	if (IsBothAccessible())
	{
		//Ϊ�˱�֤ͬ��Ч����ͬ���ڼ��ֹ�����ݴ��䡣����Ҫֹͣ�ɼ���
		//OutputDebugString(L"\nֹͣ�ɼ�\n");
		DWORD timeBegin = ::GetTickCount();
		StopGrabbing();
		m_queueImg0.clear();
		m_queueImg1.clear();
		::Sleep(200);

		StartGrabbing();
		m_bNeedReseSync = false;
		DWORD timeEND = ::GetTickCount();
		pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), std::string("�����λ�ɹ�����ʱ")+ std::to_string(timeEND- timeBegin)+"  ms!");
		
	}
	else
	{
		pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), std::string("�����λʧ��!"));
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
	if ((us > 3000) || (us < 4))//�ع�ʱ�䷶ΧΪ4~3000
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
		emit SendCameraInfo(m_GigELIsOK ? 0 : 1, QString::fromLocal8Bit("����������ع� ") + QString::number(us) + (m_GigELIsOK ? QString::fromLocal8Bit("�ɹ���") : QString::fromLocal8Bit("ʧ�ܣ�")));
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

		emit SendCameraInfo(m_GigERIsOK ? 0 : 1, QString::fromLocal8Bit("����������ع� ") + QString::number(us) + (m_GigERIsOK ? QString::fromLocal8Bit("�ɹ���") : QString::fromLocal8Bit("ʧ�ܣ�")));
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
			emit SendCameraInfo(State ? 0 : 1, QString::fromLocal8Bit("������������� ") + QString::number(gain) + (State ? QString::fromLocal8Bit("�ɹ���") : QString::fromLocal8Bit("ʧ�ܣ�")));
			return State;
		}
		emit SendChannelCameraState(0, m_GigELIsOK);
	}

	if (Channel == 1)
	{
		if ((m_GigERIsOK = QDalsaGige::IsAccessible(m_CameraNameRight)))
		{
			bool State = m_GigE2.m_pAcqDevice->SetFeatureValue("Gain", (double)gain);			
			emit SendCameraInfo(State ? 0 : 1, QString::fromLocal8Bit("������������� ") + QString::number(gain) + (State ? QString::fromLocal8Bit("�ɹ���") : QString::fromLocal8Bit("ʧ�ܣ�")));
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
		Device.SetDisplayStatusMode(3);//����ʾ֪ͨ
		Device1.SetDisplayStatusMode(3);//����ʾ֪ͨ
										//���ö�ʱ���ź�
		b = Device.SetFeatureValue("timerSelector", "Timer1");
		b = Device.SetFeatureValue("timerMode", "Off");
		b = Device.SetFeatureValue("timerStartSource", "Timer1End");
		b = Device.SetFeatureValue("timerDelay", 0);
		b = Device.SetFeatureValue("timerDuration", 100);		//��λ��us Ĭ��100
		b = Device.SetFeatureValue("timerMode", "Active");


		//����ʱ����Ϊ����ߴ���Դ
		b = Device.SetFeatureValue("TriggerSelector", "LineStart");
		b = Device.SetFeatureValue("TriggerMode", "On");
		b = Device.SetFeatureValue("TriggerSource", "Timer1End");
		b = Device.SetFeatureValue("TriggerOverlap", "ReadOut");
		b = Device.SetFeatureValue("triggerLineCount", 1);


		//�رճ�ͬ��
		b = Device.SetFeatureValue("TriggerSelector", "FrameStart");
		b = Device.SetFeatureValue("TriggerMode", "Off");

	}
	else
	{
		//�������б�д����ԭʼ��֡ͬ����û�б�д
		Device.SetDisplayStatusMode(3);//����ʾ֪ͨ
		Device1.SetDisplayStatusMode(3);//����ʾ֪ͨ

										//���� Line1 �� Line2 �źŽӿڣ���ɫ1+����ɫ1-����ɫ2+����ɫ2-��
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


		//���� Line4 �źŽӿڣ���ɫ4+����ɫ4-��
		b = Device.SetFeatureValue("LineSelector", "Line4");
		b = Device.SetFeatureValue("LineFormat", "RS422");
		b = Device.SetFeatureValue("LineMode", "Input");
		b = Device.SetFeatureValue("LineInverter", FALSE);
		b = Device.SetFeatureValue("lineDebouncingPeriod", 0);
		b = Device.SetFeatureValue("lineElectricalTermination", "Disabled");



		//�趨 Line1 �� Line2 Ϊ�ߴ���Դ����ͬ��Դ��
		b = Device.SetFeatureValue("TriggerSelector", "LineStart");
		b = Device.SetFeatureValue("TriggerMode", "On");
		b = Device.SetFeatureValue("TriggerSource", "rotaryEncoder1");
		b = Device.SetFeatureValue("TriggerOverlap", "ReadOut");
		b = Device.SetFeatureValue("triggerLineCount", 1);


		b = Device.SetFeatureValue("rotaryEncoderOutputMode", "Motion");	//�������޷���
		b = Device.SetFeatureValue("rotaryEncoderInputASource", "Line1");
		b = Device.SetFeatureValue("rotaryEncoderInputBSource", "Line2");
		b = Device.SetFeatureValue("rotaryEncoderSrcActivation", "AnyEdge");
		b = Device.SetFeatureValue("rotaryEncoderMultiplier", 1);
		b = Device.SetFeatureValue("rotaryEncoderDivider", 1);


		//�趨 Line4 Ϊ֡����Դ����ͬ��Դ��
		b = Device.SetFeatureValue("TriggerSelector", "FrameStart");
		b = Device.SetFeatureValue("TriggerMode", "On");
		b = Device.SetFeatureValue("TriggerSource", "Line4");
		b = Device.SetFeatureValue("TriggerActivation", "RisingEdge"); //������Ч


																	   //ADD

																	   //���� Line1 �� Line2 �źŽӿڣ���ɫ1+����ɫ1-����ɫ2+����ɫ2-��
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


		//���� Line4 �źŽӿڣ���ɫ4+����ɫ4-��
		b1 = Device1.SetFeatureValue("LineSelector", "Line4");
		b1 = Device1.SetFeatureValue("LineFormat", "RS422");
		b1 = Device1.SetFeatureValue("LineMode", "Input");
		b1 = Device1.SetFeatureValue("LineInverter", FALSE);
		b1 = Device1.SetFeatureValue("lineDebouncingPeriod", 0);
		b1 = Device1.SetFeatureValue("lineElectricalTermination", "Disabled");



		//�趨 Line1 �� Line2 Ϊ�ߴ���Դ����ͬ��Դ��
		b1 = Device1.SetFeatureValue("TriggerSelector", "LineStart");
		b1 = Device1.SetFeatureValue("TriggerMode", "On");
		b1 = Device1.SetFeatureValue("TriggerSource", "rotaryEncoder1");
		b1 = Device1.SetFeatureValue("TriggerOverlap", "ReadOut");
		b1 = Device1.SetFeatureValue("triggerLineCount", 1);


		b1 = Device1.SetFeatureValue("rotaryEncoderOutputMode", "Motion");	//�������޷���
		b1 = Device1.SetFeatureValue("rotaryEncoderInputASource", "Line1");
		b1 = Device1.SetFeatureValue("rotaryEncoderInputBSource", "Line2");
		b1 = Device1.SetFeatureValue("rotaryEncoderSrcActivation", "AnyEdge");
		b1 = Device1.SetFeatureValue("rotaryEncoderMultiplier", 1);
		b1 = Device1.SetFeatureValue("rotaryEncoderDivider", 1);


		//�趨 Line4 Ϊ֡����Դ����ͬ��Դ��
		b1 = Device1.SetFeatureValue("TriggerSelector", "FrameStart");
		b1 = Device1.SetFeatureValue("TriggerMode", "On");
		b1 = Device1.SetFeatureValue("TriggerSource", "Line4");
		b1 = Device1.SetFeatureValue("TriggerActivation", "RisingEdge"); //������Ч

	}

	////�趨�ع�ʱ��
	//b = Device.SetFeatureValue("ExposureTime", double(m_ExposureTimeL));		//min��4  max��3000
	//																		//ADD
	//b1 = Device1.SetFeatureValue("ExposureTime", double(m_ExposureTimeR));

	//����Ӳ��ʱ���
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

	DWORD NowTime = ::GetTickCount();  //ϵͳʱ��,�������49�죬18ms�����

	pClass->m_LastFrameHostTimeL = NowTime;

	int SizeX = Buf1.GetWidth();
	int SizeY = Buf1.GetHeight();
	long ImgIndex = Buf1.GetIndex();

	// д�����ڴ�
	
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

	DWORD NowTime = ::GetTickCount();  //ϵͳʱ��,�������49�죬18ms�����

	pClass->m_LastFrameHostTimeR = NowTime;

	int SizeX = Buf2.GetWidth();
	int SizeY = Buf2.GetHeight();
	long ImgIndex = Buf2.GetIndex();

	// д�����ڴ�

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
				pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), "���1������2��ǰ��ɾ�����2!");
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
				pApp->m_Memory.WriteLogInfo(1, novice::CommonOperating::CurrentTimeString(), "���1������2��ǰ��ɾ�����2!");
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