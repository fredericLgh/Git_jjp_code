/****************************************************
Copyright(c) 欧博科技软件部

	文件名称：OperateINI.h
	文件描述：操作配置文件
	作则    : novice
	当前版本：1.0 v
	创建日期：2019-4-30
	末次修改：

功能描述：基于Qt框架，模仿squall,使用QSettings,操作配置文件

******************************************************/

#ifndef OPERATEINI_H_
#define OPERATEINI_H_

#include <QString>
#include <vector>
#include <QRect>


class QSettings;

namespace novice
{
	class COperateINI 
	{

	public:
		COperateINI();
		~COperateINI();

		// 末尾不带"\\"  
		static QString GetAppPath(void);
		static bool IsFileExist(const QString &PathName);


		static bool CreateFolder(const QString& path);
		static void GetFolderFiles(std::vector<QString>& listQstring, const QString path);

		bool Attach(QString FileName, bool FullPathName = false);

		void DevideString(const QString &String, std::vector<QString> &vecString, char ch);

		QString GetParameterString(QString Section, QString Param);
		int GetParameterInt(QString Section, QString Param);
		float GetParameterFloat(QString Section, QString Param);

		//Param=aaa,bbb,ccc 提取到vector中。
		void GetParameterVecString(QString Section, QString Param, std::vector<QString> &vecString);

		//Param=100,101,102 提取到vector中。
		void GetParameterVecInt(QString Section, QString Param, std::vector<int> &vecInt);

		//Param=20.5,40.7,60.3 提取到vector中。
		void GetParameterVecFloat(QString Section, QString Param, std::vector<float> &vecFloat);

		void SetParameterString(QString Section, QString Param, const QString &String);
		void SetParameterInt(QString Section, QString Param, int Value);
		void SetParameterFloat(QString Section, QString Param, float Value);

		void SetParameterQRect(QString Section, QString Param, QRect rect);
		QRect GetParameterQRect(QString Section, QString Param);

	private:
		QString     m_PathName;
		QSettings*  m_pSetting;
	};


};

#endif