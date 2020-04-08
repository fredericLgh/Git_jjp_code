#include "QTray.h"

novice::QTray::QTray(QWidget *parent, const QIcon& icon)
  : QSystemTrayIcon(parent)
  , m_pMenu(nullptr)
  , m_pActionShow(nullptr)
  , m_pActionHide(nullptr)
  , m_pActionExit(nullptr)
{
	Q_ASSERT(parent);
	m_pMenu = new QMenu(parent);

	InitAction();

	this->setContextMenu(m_pMenu);
	this->setIcon(icon);

	connect(this, &novice::QTray::activated, this, &novice::QTray::OnActivated);
	this->show();
}

novice::QTray::~QTray()
{

}

void novice::QTray::InitAction()
{
	m_pActionShow = new QAction(QString::fromLocal8Bit("ÏÔÊ¾"), m_pMenu);
	m_pActionHide = new QAction(QString::fromLocal8Bit("Òþ²Ø"), m_pMenu);
	m_pActionExit = new QAction(QString::fromLocal8Bit("ÍË³ö"), m_pMenu);

	m_pMenu->addAction(m_pActionShow);
	m_pMenu->addAction(m_pActionHide);

	m_pMenu->addSeparator();

	m_pMenu->addAction(m_pActionExit);

	connect(m_pActionShow, &QAction::triggered, this, &novice::QTray::ShowWindow);
	connect(m_pActionHide, &QAction::triggered, this, &novice::QTray::HideWindow);
	connect(m_pActionExit, &QAction::triggered, this, &novice::QTray::ExitWindow);

	connect(this, &novice::QTray::activated, this, &novice::QTray::OnActivated);
}

void novice::QTray::OnActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
	case QSystemTrayIcon::Context:
		break;
	case QSystemTrayIcon::DoubleClick:
		break;
	case QSystemTrayIcon::Trigger:		
		emit ShowWindow();
		break;
	case QSystemTrayIcon::MiddleClick:	
		break;
	default:
		break;
	}
}


void novice::QTray::SetShowIcon(const QIcon& icon)
{
	Q_ASSERT(m_pActionShow);
	m_pActionShow->setIcon(icon);
}

void novice::QTray::SetHideIcon(const QIcon& icon)
{
	Q_ASSERT(m_pActionHide);
	m_pActionHide->setIcon(icon);
}

void novice::QTray::SetExitIcon(const QIcon& icon)
{
	Q_ASSERT(m_pActionExit);
	m_pActionExit->setIcon(icon);
}
