#include "DalsaGige.h"


QDalsaGige::QDalsaGige()
{
	m_pAcqDevice = NULL;
	m_pBuffers = NULL;
	m_pXfer = NULL;
	m_bOpen = FALSE;
}


QDalsaGige::~QDalsaGige()
{
}

bool QDalsaGige::FindGigE(const QString &UserName, int &ServerIndex)
{
	bool FindSuccess = FALSE;

	int ServerCount = SapManager::GetServerCount();
	for (int i = 0; i < ServerCount; i++)
	{
		if (SapManager::GetResourceCount(i, SapManager::ResourceAcqDevice) > 0)
		{
			char strUserName[256] = { 0 };
			SapManager::GetResourceName(i, SapManager::ResourceAcqDevice, 0, strUserName);

			std::string StdStrUserName(strUserName);
			if (StdStrUserName == std::string(UserName.toLocal8Bit()))
			{
				ServerIndex = i;
				FindSuccess = TRUE;
				break;
			}
		}
	}

	return FindSuccess;
}


bool QDalsaGige::IsAccessible(const QString &UserName)
{
	int SIndex;
	if (FindGigE(UserName, SIndex))
	{
		return SapManager::IsServerAccessible(SIndex);
	}
	else
	{
		return FALSE;
	}
}

bool QDalsaGige::IsCameraAvailable(int ServerIndex)
{
	return SapManager::IsResourceAvailable(ServerIndex, SapManager::ResourceAcqDevice, 0);
}

bool QDalsaGige::Open(SapXferCallback pFunction, LPVOID pUser,
	const QString &UserName, UINT width, UINT height, BOOL isUseTurboDrive)
{
	m_bOpen = FALSE;

	int ServerIndex;
	if (!FindGigE(UserName, ServerIndex))
	{
		return FALSE;
	}

	SapLocation loc(ServerIndex);
	m_pAcqDevice = new SapAcqDevice(loc);
	m_pBuffers = new SapBuffer(10, m_pAcqDevice);
	m_pXfer = new SapAcqDeviceToBuf(m_pAcqDevice, m_pBuffers, pFunction, pUser);

	if (!m_pAcqDevice->Create())
	{
		ClearResource();
		return FALSE;
	}

	m_pAcqDevice->SetFeatureValue("Width", width);
	m_pAcqDevice->SetFeatureValue("Height", height);
	m_pAcqDevice->SetFeatureValue("PixelFormat", "Mono8");
	//	m_pAcqDevice->SetFeatureValue("BinningAverageMode", "False");
	//	m_pAcqDevice->SetFeatureValue("BinningHorizontal", 1);
	//	m_pAcqDevice->SetFeatureValue("BinningVertical", 1);
	m_pAcqDevice->SetFeatureValue("TestPattern", "Off");
	BOOL b = m_pAcqDevice->SetFeatureValue("turboTransferEnable", isUseTurboDrive);

	//ADD
	//m_pAcqDevice->SetFeatureValue("PixelFormat", "Mono8");
	m_pAcqDevice->SetFeatureValue("OffsetX", 0);
	//m_pAcqDevice->SetFeatureValue("Width", theApp.m_ImageWidth);
	//m_pAcqDevice->SetFeatureValue("Height", theApp.m_ImageHeight);

	//m_pAcqDevice->SetFeatureValue("TestPattern", "Off");


	int FeatureCount;
	BOOL b1 = m_pAcqDevice->GetFeatureCount(&FeatureCount);

	for (auto i = 0; i < FeatureCount; i++)
	{
		char FeatureName[256] = { 0 };	//捕获服务器名称
		m_pAcqDevice->GetFeatureNameByIndex(i, FeatureName, 256);

		/*CString str;
		str.Format(L"\n (%d) %s\n", i, CStringW(FeatureName));
		OutputDebugStringW(str);*/
	}

	if (!m_pBuffers->Create())
	{
		m_pAcqDevice->Destroy();
		ClearResource();
		return FALSE;
	}


	if (!m_pXfer->Create())
	{
		m_pBuffers->Destroy();
		m_pAcqDevice->Destroy();
		ClearResource();
		return FALSE;
	}

	m_bOpen = TRUE;

	//OutputDebugStringW(L"\n GigE  ");
	//OutputDebugStringW(UserName);
	//OutputDebugStringW(L" Opened!\n");
	return TRUE;
}

void QDalsaGige::Close(void)
{
	if (IsOpen())
	{
		m_pXfer->Destroy();
		m_pBuffers->Destroy();
		m_pAcqDevice->Destroy();
		ClearResource();
		m_bOpen = FALSE;

		//OutputDebugStringW(L"\n GigE Camera Closed!");
	}

}

bool QDalsaGige::IsOpen(void)
{
	return m_bOpen;
}

bool QDalsaGige::StartGrab(void)
{
	m_pBuffers->Clear();
	return m_pXfer->Grab();
}

void QDalsaGige::StopGrab(void)
{
	if (m_pXfer->IsGrabbing())
	{
		m_pXfer->Freeze();
	}
}

void QDalsaGige::ClearResource(void)
{
	if (m_pXfer)
	{
		delete m_pXfer;
		m_pXfer = NULL;
	}

	if (m_pBuffers)
	{
		delete m_pBuffers;
		m_pBuffers = NULL;
	}

	if (m_pAcqDevice)
	{
		delete m_pAcqDevice;
		m_pAcqDevice = NULL;
	}
}

