#ifndef CAMERAGRABAPP_H_
#define CAMERAGRABAPP_H_

#include <QApplication>
#include "TopTailSharedMemory.h"

#define pApp static_cast<QCameraGrabApp*>(qApp)

class QCameraGrabApp : public QApplication
{
	Q_OBJECT

public:
	QCameraGrabApp(int &argc, char **argv);
	~QCameraGrabApp();

	void ReadIni();
	void WriteIni();

private:
	void InitApp();
	void ExitApp();
public:

	CTopTailSharedMemory m_Memory;

	QString m_LeftCameraName;
	QString m_RightCameraName;
	
	int     m_LeftExpTime;
	int     m_RightExpTime;

	int     m_LeftGain;
	int     m_RightGain;

	uint       m_ImageWidth;//ÿ֡ͼ����
	uint       m_ImageHeight;//ÿ֡ͼ������
};

#endif