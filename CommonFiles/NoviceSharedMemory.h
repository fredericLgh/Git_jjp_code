#ifndef NOVICESHAREDMEMORY_H
#define NOVICESHAREDMEMORY_H

#include <QSharedMemory>
#include <string>

class CNoviceSharedMemory : protected QSharedMemory
{
	Q_OBJECT

public:
	CNoviceSharedMemory(QObject *parent = 0);
	~CNoviceSharedMemory();

protected:
	bool  Open(const QString& Key, int size);
	void  Close();
	void* GetBuffer();
	const void *GetBuffer() const;
protected:
	bool m_IsOpen;
};
#endif // NOVICESHAREDMEMORY_H
