#include "NoviceSharedMemory.h"

CNoviceSharedMemory::CNoviceSharedMemory(QObject *parent)
: QSharedMemory(parent)
, m_IsOpen(false)
{

}

CNoviceSharedMemory::~CNoviceSharedMemory()
{
}

bool CNoviceSharedMemory::Open(const QString& Key, int size)
{
	setKey(Key);

	bool CreateState = create(size);

	// 创建失败并且错误返回不是(Key存在)
	// 表示出异常了
	if ((!CreateState) && (QSharedMemory::AlreadyExists != error()))
	{
		m_IsOpen = false;
		return false;
	}

	if (!isAttached())
	{
		if (!attach())
		{
			m_IsOpen = false;
			return false;
		}

	}

	if (CreateState)
	{
		memset(data(), 0, size);
	}

	m_IsOpen = true;
	return true;
}

void* CNoviceSharedMemory::GetBuffer()
{
	if (!m_IsOpen) return nullptr;

	return data();
}

const void* CNoviceSharedMemory::GetBuffer() const
{
	if (!m_IsOpen) return nullptr;

	return data();
}

void CNoviceSharedMemory::Close()
{
	if (isAttached())
	{
		detach();
	}
}
