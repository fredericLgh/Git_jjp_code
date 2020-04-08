#include "CommonOperating.h"
#include <QDir>
#include <QTextStream>
#include <iostream>

#include <QException>

using namespace novice;

CommonOperating::CommonOperating()
{
}


CommonOperating::~CommonOperating()
{
}

QString CommonOperating::DateTime2TimeQStr(const QDateTime& DateTime)
{
	return	DateTime.toString(QString("yyyy-MM-dd HH:mm:ss"));
}

std::string CommonOperating::DateTime2TimeStr(const QDateTime& DateTime)
{
	auto QStr = DateTime.toString(QString("yyyy-MM-dd HH:mm:ss"));
	return std::string(QStr.toLocal8Bit());
}

QString  CommonOperating::CurrentTimeQString()
{
	return QDateTime::currentDateTime().toString(QString("yyyy-MM-dd HH:mm:ss"));
}

std::string CommonOperating::CurrentTimeString()
{
	auto QStr = QDateTime::currentDateTime().toString(QString("yyyy-MM-dd HH:mm:ss"));
	return std::string(QStr.toLocal8Bit());
}

void CommonOperating::DateTime2TimeChar(const QDateTime& DateTime, char* const p, int Max)
{
	if (!p) return;
	auto& Str = DateTime2TimeStr(DateTime);
	
	int Min = Str.length() > Max - 1 ? Max - 1 : Str.length();
	memcpy(p, Str.c_str(), Min);
	*(p + Min) = 0;
}

QDateTime CommonOperating::TimeStr2DateTime(const std::string& str)
{
	auto QStr = QString::fromLocal8Bit(str.data());
	return QDateTime::fromString(QStr, QString("yyyy-MM-dd HH:mm:ss"));
}

QDateTime CommonOperating::TimeQStr2DateTime(const QString& QStr)
{
	return QDateTime::fromString(QStr, QString("yyyy-MM-dd HH:mm:ss"));
}

QDateTime CommonOperating::TimeChar2DateTime(char* const p)
{
	Q_ASSERT(p);	
	std::string str(p);
	return TimeStr2DateTime(str);
}

QString CommonOperating::DateTime2TimeQStrZ(const QDateTime& DateTime)
{
	return	DateTime.toString(QString("yyyy-MM-dd HH:mm:ss:zzz"));
}

std::string CommonOperating::DateTime2TimeStrZ(const QDateTime& DateTime)
{
	auto QStr = DateTime.toString(QString("yyyy-MM-dd HH:mm:ss:zzz"));
	return std::string(QStr.toLocal8Bit());
}

void CommonOperating::DateTime2TimeCharZ(const QDateTime& DateTime, char* const p, int Max)
{
	if (!p) return;
	auto& Str = DateTime2TimeStr(DateTime);

	int Min = Str.length() > Max - 1 ? Max - 1 : Str.length();
	memcpy(p, Str.c_str(), Min);
	*(p + Min) = 0;
}


QString  CommonOperating::CurrentTimeQStringZ()
{
	return QDateTime::currentDateTime().toString(QString("yyyy-MM-dd HH:mm:ss:zz"));
}

std::string CommonOperating::CurrentTimeStringZ()
{
	auto QStr = QDateTime::currentDateTime().toString(QString("yyyy-MM-dd HH:mm:ss:zz"));
	return std::string(QStr.toLocal8Bit());
}


QDateTime CommonOperating::TimeStrZ2DateTime(const std::string& str)
{
	auto QStr = QString::fromLocal8Bit(str.data());
	return QDateTime::fromString(QStr, QString("yyyy-MM-dd HH:mm:ss:zzz"));
}

QDateTime CommonOperating::TimeQStrZ2DateTime(const QString& QStr)
{
	return QDateTime::fromString(QStr, QString("yyyy-MM-dd HH:mm:ss:zzz"));
}

QDateTime CommonOperating::TimeCharZ2DateTime(char* const p)
{
	Q_ASSERT(p);
	std::string str(p);
	return TimeStr2DateTime(str);
}


QString  CommonOperating::Date2ImageDateDir(const QString& ImagPath, const QDate& Date)
{
	return ImagPath + "/" + Date.toString(QString("yyyyMM/dd/"));
}

QString  CommonOperating::Date2ImageDateDir(const QString& ImagPath, const QDateTime& DateTime)
{
	return ImagPath + "/" + DateTime.date().toString(QString("yyyyMM/dd/"));
}

QString CommonOperating::DateTime2ImageDir(const QString& ImagPath, const QDateTime& DateTime)
{
	return ImagPath + "/" + DateTime.toString(QString("yyyyMM/dd/HHmmss/"));
}

QString CommonOperating::DateTime2ImageFileFirstQString(const QString& ImagPath, const QDateTime& DateTime)
{
	return ImagPath + "/" + DateTime.toString(QString("yyyyMM/dd/HHmmss/HHmmss"));
}

std::string CommonOperating::DateTime2ImageFileFirstString(const QString& ImagPath, const QDateTime& DateTime)
{
	return std::string(DateTime2ImageFileFirstQString(ImagPath, DateTime).toLocal8Bit());
}

QString CommonOperating::Date2DataFile(const QString& DataPath, const QDate& Date, const QString& First, const QString& End)
{
	return  DataPath + "/" + First + Date.toString(QString("yyyyMMdd")) + End;
}

 QString CommonOperating::Date2DataFile(const QString& DataPath, const QDateTime& DateTime, const QString& First, const QString& End)
{
	 return  DataPath + "/" + First + DateTime.date().toString(QString("yyyyMMdd")) + End;
}


 QString CommonOperating::String2QString(const std::string& str)
 {
	 // 必须保证str 本身不是乱码
	 return QString::fromLocal8Bit(str.data());
 }

 std::string CommonOperating::QString2String(const QString& Qstr)
 {
	 // 必须保证Qstr 本身不是乱码
	 return std::string(Qstr.toLocal8Bit());
 }

 void CommonOperating::QString2char(const QString& QStr, char* const pChar, int MaxLegth)
 {
	 if (!pChar) return;

	 auto str = QString2String(QStr);

	 int MinSize = str.length() < MaxLegth ? str.length() : MaxLegth - 1;

	 memcpy(pChar, str.c_str(), MinSize);
	 memset(pChar + MinSize, 0, 1);
 }

 void  CommonOperating::char2QString(QString& QStr, const char* const pChar, int MaxLegth)
 {
	 QStr = "";
	 if (!pChar) return;
	 
	 std::string str(pChar);

	 if (str.length() < MaxLegth)
	 {
		 QStr = QString::fromLocal8Bit(str.data());
	 } 
 }

 void CommonOperating::PushFileTree(const QString& Path, CommonOperating::CFileTree& FileTree)
 {
	 QDir Dir(Path);
	 
	 if (!Dir.exists())
	 {
		 return;
	 }

	 if (Dir.filter() == QDir::Files)
	 {
		 return;
	 }

	 auto& ListFileInfo = Dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	 for (auto& ref : ListFileInfo)
	 {
		 auto pFileTree = new CommonOperating::CFileTree(ref.filePath()); 
		 FileTree.AddChild(pFileTree);

		 if (ref.isDir()) PushFileTree(pFileTree->m_Path, *pFileTree);		
	 }
 }

 void CommonOperating::CoutFileTree(const CFileTree& FileTree)
 {
	 std::cout << std::string(FileTree.m_Path.toLocal8Bit()) << std::endl;

	 for (auto& ref : FileTree.m_ChildVector)
	 {
		 if (ref != nullptr)
		 {
			 CoutFileTree(*ref);
		 }		 
	 }
 }

 void CommonOperating::WriteFileQString(const QString& QFileName, const QString& Str)
 {
	 auto CreateDir = [](const QString& FileName)
	 {
		 // 1. 分解字符串
		 auto index = FileName.lastIndexOf("/");

		 QString PathName(FileName);
		 // 有斜杠
		 if (index != -1)
		 {
			 PathName = (FileName.mid(0, index + 1));
		 }

		 QDir Dir(PathName);
		 // 文件夹不存在
		 if (!Dir.exists(PathName))
		 {
			 if (!Dir.mkpath(PathName))
			 {
				 return false;
			 }
		 }

		 return true;
	 };

	 if (QFileName.isEmpty()) return ;

	 // 转 反斜杠(\\)为斜杠(/)
	 QString FileName = QDir::fromNativeSeparators(QFileName);

	 QFile File(FileName);
	 if (!File.exists())
	 {
		 if (!CreateDir(FileName))
		 {
			 return ;
		 }
	 }
	 if (File.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	 {
		 QTextStream out(&File);
		 out << Str << "\n";
		 File.close();

		 return ;
	 }
	 return ;
 }

 void CommonOperating::WriteFileTree(const CFileTree& FileTree, const QString& FileName)
 {
	 WriteFileQString(FileName, FileTree.m_Path);
	 for (auto& ref : FileTree.m_ChildVector)
	 {
		if (ref != nullptr)
		{
			WriteFileTree(*ref, FileName);
		}
	}
 }

 void CommonOperating::DeleteDirORFile(const QString& Path)
 {
	 
		QDir Dir(Path);

		if (!Dir.exists())
		{
			return;
		}

		if (Dir.filter() == QDir::Files)
		{
			Dir.remove(Path);
			return;
		}

		for (auto& ref : Dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
		{
			if (ref.isDir())
			{
				DeleteDirORFile(ref.filePath());
			}
			else if (ref.isFile())
			{
				Dir.remove(ref.filePath());
			}
		}
		Dir.rmdir(Path);
 }