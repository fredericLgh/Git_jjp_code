#ifndef QDALSAGIGE_H_
#define QDALSAGIGE_H_

#include "SapClassBasic.h"
#pragma comment(lib,"SapClassBasic.lib")
#include <QString>

class QDalsaGige
{
public:
	QDalsaGige();
	~QDalsaGige();

	static bool FindGigE(const QString &UserName, int &ServerIndex);
	static bool IsAccessible(const QString &UserName);
	static bool IsCameraAvailable(int ServerIndex);


	bool Open(SapXferCallback pFunction, LPVOID pUser, const QString &UserName,
		UINT width = 4096, UINT height = 1, BOOL isUseTurboDrive = FALSE);
	void Close(void);
	bool IsOpen(void);

	bool StartGrab(void);
	void StopGrab(void);
public:
	SapAcqDevice*     m_pAcqDevice;   //Ïà»úµÄlib
	SapBuffer*        m_pBuffers;
	SapTransfer*      m_pXfer;


protected:
	void ClearResource(void);

protected:
	bool              m_bOpen;
};


#endif