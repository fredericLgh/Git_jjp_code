#include "QCameraGrab.h"
#include "CameraGrabApp.h"
#include "QTray.h"

#include <QCloseEvent>
#include <QTimer>
#include <QMessageBox>

#include "CommonOperating.h"

QCameraGrab::QCameraGrab(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(size());
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	InitWnd();
	m_pTray = new novice::QTray(this, QIcon(":/QCameraGrab/CameraICON"));
	m_pTray->setToolTip(QString::fromLocal8Bit("相机采集"));
	connect(m_pTray, &novice::QTray::ShowWindow, this, &QCameraGrab::ShowWindow);
	connect(m_pTray, &novice::QTray::HideWindow, this, &QCameraGrab::HideWindow);
	connect(m_pTray, &novice::QTray::ExitWindow, this, &QCameraGrab::ExitWindow);
	connect(ui.m_pBtnHide, &QPushButton::clicked, this, &QCameraGrab::HideWindow);
	connect(ui.m_pBtnExit, &QPushButton::clicked, this, &QCameraGrab::ClickClose);
}

QCameraGrab::~QCameraGrab()
{
	ExitWnd();
}

void QCameraGrab::InitWnd()
{

	connect(ui.m_pBtnSetLeft, &QPushButton::clicked, this, &QCameraGrab::ClickSetLeft);
	connect(ui.m_pBtnSetRight, &QPushButton::clicked, this, &QCameraGrab::ClickSetRight);

	connect(&m_Camera, &CMutilDalsaGige::SendCameraState, this, &QCameraGrab::CameraState);
	connect(&m_Camera, &CMutilDalsaGige::SendChannelCameraState, this, &QCameraGrab::CameraChannelState);
	connect(&m_Camera, &CMutilDalsaGige::SendCameraInfo, this, &QCameraGrab::CameraInfo);

	connect(this, &QCameraGrab::SendReSyncCamera, &m_Camera, &CMutilDalsaGige::GetReSyncCamera);

	App2Wnd();
	m_Camera.Open(pApp->m_LeftCameraName, pApp->m_RightCameraName, pApp->m_ImageWidth, pApp->m_ImageHeight);
	m_Camera.SetExposureTime(0, pApp->m_LeftExpTime);
	m_Camera.SetExposureTime(1, pApp->m_RightExpTime);
	m_Camera.SetGain(0, pApp->m_LeftGain);
	m_Camera.SetGain(1, pApp->m_RightGain);
	m_Camera.StartGrabbing();
}

void QCameraGrab::ExitWnd()
{
	m_Camera.StopGrabbing();
	m_Camera.Close();
	pApp->m_Memory.WriteLogInfo(0, novice::CommonOperating::CurrentTimeString(), "相机进程关闭!");
}

void QCameraGrab::App2Wnd()
{
	ui.m_pLineEditImageHeight->setText(QString::number(pApp->m_ImageHeight));
	ui.m_pLineEditImageWidth->setText(QString::number(pApp->m_ImageWidth));

	ui.m_pLineEditNameLeft->setText(pApp->m_LeftCameraName);
	ui.m_pLineEditNameRight->setText(pApp->m_RightCameraName);

	ui.m_pLineEditExpLeft->setText(QString::number(pApp->m_LeftExpTime));
	ui.m_pLineEditExpRight->setText(QString::number(pApp->m_RightExpTime));

	ui.m_pLineEditGainLeft->setText(QString::number(pApp->m_LeftGain));
	ui.m_pLineEditGainRight->setText(QString::number(pApp->m_RightGain));
}

void QCameraGrab::Wnd2App()
{
	pApp->m_ImageHeight = ui.m_pLineEditImageHeight->text().toInt();
	pApp->m_ImageWidth = ui.m_pLineEditImageWidth->text().toInt();

	pApp->m_LeftCameraName = ui.m_pLineEditNameLeft->text();
	pApp->m_RightCameraName = ui.m_pLineEditNameRight->text();

	pApp->m_LeftExpTime = ui.m_pLineEditExpLeft->text().toInt();
	pApp->m_RightExpTime = ui.m_pLineEditExpRight->text().toInt();

	pApp->m_LeftGain = ui.m_pLineEditGainLeft->text().toInt();
	pApp->m_RightGain = ui.m_pLineEditGainRight->text().toInt();

	pApp->WriteIni();
}

void QCameraGrab::ClickSetLeft()
{
	Wnd2App();
	m_Camera.SetExposureTime(0, pApp->m_LeftExpTime);
	m_Camera.SetGain(0, pApp->m_LeftGain);
}

void QCameraGrab::ClickSetRight()
{
	Wnd2App();
	m_Camera.SetExposureTime(1, pApp->m_RightExpTime);
	m_Camera.SetGain(1, pApp->m_RightGain);
}

void QCameraGrab::CameraState(bool Camera0State, bool Camera1State)
{
	bool State = Camera0State&&Camera1State;
	if (pApp->m_Memory.ReadCameraInfo() != State)
	{
		pApp->m_Memory.WriteCameraInfo(State);

		std::string strInfo;
		std::string LeftInfo = Camera0State ? "开启" : "关闭";
		std::string RightInfo = Camera1State ? "开启" : "关闭";
		strInfo += "相机0"+ LeftInfo + "! " +  "相机1" + RightInfo + " !";
		pApp->m_Memory.WriteLogInfo(0, novice::CommonOperating::CurrentTimeString(), strInfo);

		// 表示是新开的
		if (State)
		{
			// 复位下
			emit SendReSyncCamera(State);
		}

	}


	CameraChannelState(0, Camera0State);
	CameraChannelState(1, Camera1State);
}
void QCameraGrab::CameraChannelState(int Channel, bool CameraState)
{
	if (Channel == 0)
	{
		SetLabelLed(ui.m_pLabelLeftLed, CameraState);
	}
	else if (Channel == 1)
	{
		SetLabelLed(ui.m_pLabelRightLed, CameraState);
	}	
}

void QCameraGrab::CameraInfo(int Type, QString Info)
{
	pApp->m_Memory.WriteLogInfo(Type,novice::CommonOperating::CurrentTimeString(), novice::CommonOperating::QString2String(Info));
}

void QCameraGrab::SetLabelLed(QLabel* pLabel, bool state)
{
	QPalette ple;
	ple.setColor(QPalette::Background, state? QColor(0, 255, 0):QColor(255, 0, 0));
	pLabel->setAutoFillBackground(true);
	pLabel->setPalette(ple);
}

void QCameraGrab::ShowWindow()
{
	show();
}

void QCameraGrab::HideWindow()
{
	hide();
}

void QCameraGrab::ExitWindow()
{
	close();
}

void QCameraGrab::closeEvent(QCloseEvent* event)
{
	event->accept();
}

void QCameraGrab::ClickClose()
{
	if (QMessageBox::Yes == QMessageBox::question(this, QString::fromLocal8Bit("关闭窗口"), QString::fromLocal8Bit("确认要关闭窗口吗?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
	{
		close();
	}
}

void QCameraGrab::mousePressEvent(QMouseEvent *e)
{
	m_LastPoint = e->globalPos();
}

void QCameraGrab::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->globalX() - m_LastPoint.x();
	int dy = e->globalY() - m_LastPoint.y();
	m_LastPoint = e->globalPos();
	move(x() + dx, y() + dy);
}

void QCameraGrab::mouseReleaseEvent(QMouseEvent *e)
{
	int dx = e->globalX() - m_LastPoint.x();
	int dy = e->globalY() - m_LastPoint.y();
	move(x() + dx, y() + dy);
}
