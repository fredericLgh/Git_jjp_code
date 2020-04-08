#include "OperateInI.h"
#include <QCoreApplication>
#include <QFileinfo>
#include <QSettings>
#include <iostream>
#include <QDir>
#include <QTextCodec>

novice::COperateINI::COperateINI()
: m_pSetting(NULL)
{

}

novice::COperateINI::~COperateINI()
{
	if (m_pSetting)
	{
		delete m_pSetting;
	}
}

QString novice::COperateINI::GetAppPath(void)
{
	return QCoreApplication::applicationDirPath();
}

bool novice::COperateINI::IsFileExist(const QString &PathName)
{
	QFileInfo fileinfo(PathName);
	return fileinfo.isFile();
}

bool novice::COperateINI::Attach(QString FileName, bool FullPathName)
{
	if (FullPathName)
	{
		m_PathName = FileName;
	}
	else
	{
		m_PathName = GetAppPath() + "/" + FileName;
	}
	
	if (IsFileExist(m_PathName))
	{
		m_pSetting = new QSettings(m_PathName, QSettings::IniFormat);
		return true;
	}

	return false;
}

QString novice::COperateINI::GetParameterString(QString Section, QString Param)
{

	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return QString();

	return (m_pSetting->value(Section + "/" + Param).toString());
}

int  novice::COperateINI::GetParameterInt(QString Section, QString Param)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return 0;
	return (m_pSetting->value(Section + "/" + Param,0).toInt());
}

float  novice::COperateINI::GetParameterFloat(QString Section, QString Param)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return 0;
	return (m_pSetting->value(Section + "/" + Param, 0).toFloat());
}

void novice::COperateINI::DevideString(const QString &String, std::vector<QString> &vecString, char ch)
{
	vecString.clear();
	for (int i = 0; i < String.count();)
	{
		int end = String.indexOf(',', i);
		if (end < 0)
		{
			vecString.push_back(String.mid(i));
			break;
		}
		vecString.push_back(String.mid(i, end - i));
		i = end + 1;
	}
}

void novice::COperateINI::GetParameterVecString(QString Section, QString Param, std::vector<QString> &vecString)
{
	vecString.clear();

	QString str = GetParameterString(Section, Param); 

	if (str.isEmpty()) return;

	DevideString(str, vecString, ',');
}

void novice::COperateINI::GetParameterVecInt(QString Section, QString Param, std::vector<int> &vecInt)
{
	std::vector<QString> VString;
	GetParameterVecString(Section, Param, VString);

	vecInt.clear();
	for (auto ref : VString)
	{
		vecInt.push_back(ref.toInt());
	}
}

void novice::COperateINI::GetParameterVecFloat(QString Section, QString Param, std::vector<float> &vecFloat)
{
	std::vector<QString> VString;
	GetParameterVecString(Section, Param, VString);

	vecFloat.clear();
	for (auto ref : VString)
	{
		vecFloat.push_back(ref.toFloat());
	}
}

void novice::COperateINI::SetParameterString(QString Section, QString Param, const QString &String)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return;
	m_pSetting->setValue(Section + "/" + Param, String);
}

void novice::COperateINI::SetParameterInt(QString Section, QString Param, int Value)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return;
	m_pSetting->setValue(Section + "/" + Param, Value);
}

void novice::COperateINI::SetParameterFloat(QString Section, QString Param, float Value)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return;
	m_pSetting->setValue(Section + "/" + Param, QString("%1").arg(Value));
}

void  novice::COperateINI::SetParameterQRect(QString Section, QString Param, QRect rect)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return;
	m_pSetting->setValue(Section + "/" + Param, rect);
}

QRect  novice::COperateINI::GetParameterQRect(QString Section, QString Param)
{
	if (!m_pSetting || !(m_pSetting->contains(Section + "/" + Param))) return QRect();
	return (m_pSetting->value(Section + "/" + Param).toRect());
}


bool novice::COperateINI::CreateFolder(const QString& path)
{
	QString PathName = path;
	if (PathName.isEmpty()) return false;

	QDir CheckDir;
	if (CheckDir.exists(PathName)) return true;

	int Index = 0;
	while (Index < PathName.size() && Index != -1)
	{
		Index = PathName.indexOf("\\", Index);
		QString pathNameTemp = Index == -1 ? PathName : PathName.mid(0, Index++);

		if (!CheckDir.exists(pathNameTemp) && !CheckDir.mkdir(pathNameTemp))
		{
			break;
		}
	}

	return CheckDir.exists(PathName);
}

void novice::COperateINI::GetFolderFiles(std::vector<QString>& listQstring, const QString path)
{
	listQstring.clear();
	QDir dir(path);
	auto& listName = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
	for (auto ref : listName)
	{
		listQstring.push_back(ref);
	}
}