#pragma once

#include <QtWidgets/QWidget>
#include "ui_QCameraGrab.h"
#include "MutilDalsaGige.h"

namespace  novice
{
	class QTray;
};
class QTimer;

class QCameraGrab : public QWidget
{
	Q_OBJECT

public:
	QCameraGrab(QWidget *parent = Q_NULLPTR);
	~QCameraGrab();

signals:
	void SendReSyncCamera(bool IsReSync);

protected:
	void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
private:
	QPoint m_LastPoint;
private:
	void InitWnd();
	void ExitWnd();

	void App2Wnd();
	void Wnd2App();

	void ClickSetLeft();
	void ClickSetRight();
	void ClickClose();
	

	void CameraState(bool Camera0State, bool Camera1State);
	void CameraChannelState(int Channel, bool CameraState);
	void CameraInfo(int Type, QString Info);

	void SetLabelLed(QLabel* pLabel, bool state);

	void ShowWindow();
	void HideWindow();
	void ExitWindow();

private:
	CMutilDalsaGige  m_Camera;

	novice::QTray*   m_pTray;
private:
	Ui::QCameraGrabClass ui;
};
