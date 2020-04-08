#include "CameraGrabApp.h"
#include "OperateINI.h"
#include <QICON>

QCameraGrabApp::QCameraGrabApp(int &argc, char **argv)
	: QApplication(argc, argv)
{
	setWindowIcon(QIcon(":/QOptimizationCut/Control"));
	InitApp();
}

QCameraGrabApp::~QCameraGrabApp()
{
	ExitApp();
}

void QCameraGrabApp::InitApp()
{
	m_Memory.Open();

	ReadIni();
}

void QCameraGrabApp::ExitApp()
{
	pApp->m_Memory.WriteCameraInfo(false);
	m_Memory.Close();
}


void QCameraGrabApp::ReadIni()
{
	novice::COperateINI Ini;
	if (Ini.Attach("CameraParam.ini"))
	{
		m_LeftCameraName = Ini.GetParameterString("CameraParam", "CameraNameL");
		m_RightCameraName = Ini.GetParameterString("CameraParam", "CameraNameR");

		m_ImageWidth = Ini.GetParameterInt("CameraParam", "ImageWidth");
		m_ImageHeight = Ini.GetParameterInt("CameraParam", "ImageHeight");

		m_LeftExpTime = Ini.GetParameterInt("CameraParam", "ExposureTimeL");
		m_RightExpTime = Ini.GetParameterInt("CameraParam", "ExposureTimeR");

		m_LeftGain = Ini.GetParameterInt("CameraParam", "GainL");
		m_RightGain = Ini.GetParameterInt("CameraParam", "GainR");
	}
}

void QCameraGrabApp::WriteIni()
{
	novice::COperateINI Ini;
	if (Ini.Attach("CameraParam.ini"))
	{
		Ini.SetParameterString("CameraParam", "CameraNameL", m_LeftCameraName);
		Ini.SetParameterString("CameraParam", "CameraNameR", m_RightCameraName);

		Ini.SetParameterInt("CameraParam", "ImageWidth", m_ImageWidth);
		Ini.SetParameterInt("CameraParam", "ImageHeight", m_ImageHeight);

		Ini.SetParameterInt("CameraParam", "ExposureTimeL", m_LeftExpTime);
		Ini.SetParameterInt("CameraParam", "ExposureTimeR", m_RightExpTime);

		Ini.SetParameterInt("CameraParam", "GainL", m_LeftGain);
		Ini.SetParameterInt("CameraParam", "GainR", m_RightGain);
	}
}
