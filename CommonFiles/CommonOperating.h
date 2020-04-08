/*************************************
   //常规操作



*******************************/


#ifndef COMMONOPERATING_H_
#define COMMONOPERATING_H_

#include <QString>
#include <QDate>
#include <vector>


//注意QDateTime
namespace novice
{
	class CommonOperating
	{
	public:
		CommonOperating();
		~CommonOperating();

		//"yyyy-MM-dd HH:mm:ss"
		static QString DateTime2TimeQStr(const QDateTime& DateTime);

		//"yyyy-MM-dd HH:mm:ss"
		static std::string DateTime2TimeStr(const QDateTime& DateTime);

		//"yyyy-MM-dd HH:mm:ss"
		static void DateTime2TimeChar(const QDateTime& DateTime, char* const p, int Max = 32);

		static QString    CurrentTimeQString();
		static std::string CurrentTimeString();

		//"yyyy-MM-dd HH:mm:ss"
		static QDateTime TimeStr2DateTime(const std::string& str);
		//"yyyy-MM-dd HH:mm:ss"
		static QDateTime TimeQStr2DateTime(const QString& QStr);
		//"yyyy-MM-dd HH:mm:ss"
		static QDateTime TimeChar2DateTime(char* const p);


		//"yyyy-MM-dd HH:mm:ss:zzz"
		static QString DateTime2TimeQStrZ(const QDateTime& DateTime);
		//"yyyy-MM-dd HH:mm:ss:zzz"
		static std::string DateTime2TimeStrZ(const QDateTime& DateTime);
		//"yyyy-MM-dd HH:mm:ss:zzz"
		static void DateTime2TimeCharZ(const QDateTime& DateTime, char* const p, int Max = 32);

		//"yyyy-MM-dd HH:mm:ss:zzz"
		static QDateTime TimeStrZ2DateTime(const std::string& str);
		//"yyyy-MM-dd HH:mm:ss:zzz"
		static QDateTime TimeQStrZ2DateTime(const QString& QStr);
		//"yyyy-MM-dd HH:mm:ss:zzz"
		static QDateTime TimeCharZ2DateTime(char* const p);
		
		static QString    CurrentTimeQStringZ();
		static std::string CurrentTimeStringZ();


		// "ImagPath/yyyyMM/dd/"
		static QString Date2ImageDateDir(const QString& ImagPath, const QDate& Date);

		// "ImagPath/yyyyMM/dd/"
		static QString Date2ImageDateDir(const QString& ImagPath, const QDateTime& DateTime);

		// "ImagPath/yyyyMM/dd/HHmmss/"  
		static QString DateTime2ImageDir(const QString& ImagPath, const QDateTime& DateTime);

		// "ImagPath/yyyyMM/dd/HHmmss/HHmmss"  + L.jpg(R.jpg)
		static QString DateTime2ImageFileFirstQString(const QString& ImagPath, const QDateTime& DateTime);

		// "ImagPath/yyyyMM/dd/HHmmss/HHmmss"  + L.jpg(R.jpg)
		static std::string DateTime2ImageFileFirstString(const QString& ImagPath, const QDateTime& DateTime);

		// "DataPath/(First)yyyyMMdd(End)"
		static QString Date2DataFile(const QString& DataPath, const QDate& Date, const QString& First, const QString& End);

		// "DataPath/(First)yyyyMMdd(End)"
		static QString Date2DataFile(const QString& DataPath, const QDateTime& DateTime, const QString& First, const QString& End);

		// 必须保证str 本身不是乱码
		static	QString String2QString(const std::string& str);

		// 必须保证Qstr 本身不是乱码
		static std::string QString2String(const QString& Qstr);

		static void   QString2char(const QString& QStr, char* const pChar, int MaxLegth);

		static void   char2QString(QString& QStr, const char* const pChar, int MaxLegth);
		

		// 关于文件夹
		class CFileTree
		{
		public:
			CFileTree(const QString& Path)
			{
				m_Path = Path;
			}
			~CFileTree()
			{
				Delete();
			}

			void AddChild(CFileTree* Child)
			{
				m_ChildVector.push_back(Child);
			}
			void Delete()
			{
				if (m_ChildVector.empty())
				{
					return;
				}
				else
				{
					for (auto iter = begin(m_ChildVector); iter != end(m_ChildVector);)
					{
						if (*iter != nullptr)
						{
							(*iter)->Delete();
							delete (*iter);
							*iter = nullptr;
						}
						iter = m_ChildVector.erase(iter);
					}
					
				}
			}
		public:
			QString m_Path;
			std::list<CFileTree*> m_ChildVector;
		};

		static void PushFileTree(const QString& Path, CFileTree& FileTree);

		static void CoutFileTree(const CFileTree& FileTree);

		static void WriteFileQString(const QString& QFileName, const QString& Str);
		static void WriteFileTree(const CFileTree& FileTree, const QString& FileName);

		static void DeleteDirORFile(const QString& Path);
	};
};


#endif