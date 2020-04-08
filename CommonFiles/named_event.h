/**********************************************************************

Copyright(c) 欧博科技软件部

文件名称：named_event.h
文件描述：命名事件
当前版本：1.1 v
作    者：Squall（朱一）
末次修改：2018-12-20

功能描述：用于进程间通知，注：可以同时通知多个等待对象。

************************************************************************/

#pragma once
#include <exception>
#include <Windows.h>


namespace squall
{

	class named_event
	{
	public:
		HANDLE m_hEvent;

		named_event(LPCWSTR Name)
		{
			//LPSECURITY_ATTRIBUTES   lpEventAttributes,
			//BOOL     bManualReset,
			//BOOL     bInitialState,
			//LPCTSTR  lpName

			//If the function succeeds, the return value is a handle to the event object. 
			//If the named event object existed before the function call, the function returns a handle to the existing object 
			//and GetLastError returns ERROR_ALREADY_EXISTS.
			//If the function fails, the return value is NULL.To get extended error information, call GetLastError.

			//调用 CreateEvent 时，内核对象的使用计数加一。
			m_hEvent = CreateEventW(NULL, TRUE, FALSE, Name);
			if (m_hEvent == NULL)
			{
				throw std::exception("Create Named Event Error.");
			}

		}

		~named_event()
		{
			//调用CloseHandle时，内核对象的使用计数减一。当内核对象的使用计数降为0，内核撤消该对象。
			CloseHandle(m_hEvent);
			m_hEvent = INVALID_HANDLE_VALUE;
		}

		void pulse()
		{
			::PulseEvent(m_hEvent);		//使事件有个短暂（从无效到有效，再到无效，整个过程具有原子性）的脉冲
		}

		void set()
		{
			::SetEvent(m_hEvent);		//使事件变为有信号状态
		}

		void reset()
		{
			::ResetEvent(m_hEvent);		//使事件变为无信号状态
		}

		//等到了信号返回true，超时返回false。
		bool wait(DWORD tm = INFINITE)
		{
			DWORD dwWaitResult = WaitForSingleObject(m_hEvent, tm);

			if (dwWaitResult == WAIT_OBJECT_0)
			{
				return true;
			}
			else if (dwWaitResult == WAIT_TIMEOUT)
			{
				return false;
			}
			else
			{
				throw std::exception("WaitForSingleObject Error.");
				return false;
			}

		}
	};

}

