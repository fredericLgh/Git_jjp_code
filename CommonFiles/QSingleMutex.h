/************************************
     文件名称: QSingleMutex.h
	 作    者: Novice
	 创建日期: 2019-11-12
	 末次修改: 2019-11-12
	 版    本: Ver1.0
     说    明: 缺点，不能像MFC 直接在初始化时候，干掉进程

**************************************/

#ifndef QSINGLEMUTEX_H_
#define QSINGLEMUTEX_H_

#include <QSharedMemory>

class QSingleMutex : public QSharedMemory
{
	Q_OBJECT

public:
	QSingleMutex(const QString& Name,QObject *parent = nullptr);
	~QSingleMutex();

	bool IsSignal();
private:
	QString  m_Key;
};

#endif