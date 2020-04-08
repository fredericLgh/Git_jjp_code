#ifndef MUTILDALSAGIGE_H_
#define MUTILDALSAGIGE_H_

#include <QObject>

#include "DalsaGige.h"
#include "CycleQueue.h"

class FrameInfo;
class QTimer;

class CMutilDalsaGige: public QObject
{
	Q_OBJECT

public:
	CMutilDalsaGige();
	~CMutilDalsaGige();

signals:
	void SendCameraState(bool State0, bool State1);
	void SendChannelCameraState(int Channel, bool State1);
	void SendCameraInfo(int Type, QString str);

private slots:
    void TimeOut();

public:
	int  Open(const QString &UserName0, const QString &UserName1, UINT width, UINT height); 
	void Close();
	bool IsBothOpen();
	void StartGrabbing();
	void StopGrabbing();
	bool IsBothGrabbing();
	bool IsBothAccessible();
	void ReSyncCamera();
	void CheckAndSolveSyncError();

	bool SetExposureTime(int Channel, int us);
	bool GetExposureTime(int Channel, int& us);
	bool SetGain(int Channel, int us);
	bool GetGain(int Channel, int& us);


	void SetupCamera(void);

	void PushFrame(int Channel,std::pair<FrameInfo*,long> pFrameInfo);

public:
	static void XferCallback1(SapXferCallbackInfo *pInfo);
	static void XferCallback2(SapXferCallbackInfo *pInfo);

public slots:
	void GetReSyncCamera(bool IsResync);

private:
	bool m_CameraSyncError;
	bool m_bNeedReseSync;
	QDalsaGige m_GigE1;  //两个相机
	QDalsaGige m_GigE2;

	bool       m_IsUseTurboDrive;
	QString    m_CameraNameLeft;
	QString    m_CameraNameRight;

	uint       m_SamplingFrequency;//采样率

	UINT64     m_LastFrameHostTimeL; //从系统开始到当前回调的间隔
	UINT64     m_LastFrameHostTimeR;

	bool       m_GigELIsOK; //相机状态
	bool       m_GigERIsOK;

private:
	CCycleQueue<std::pair<FrameInfo*, long>>     m_queueImg0;
	CCycleQueue<std::pair<FrameInfo*, long>>     m_queueImg1;
	
	QTimer*   m_pTimer;
};

#endif

