#pragma once
#ifndef _WIN32
#error CWatchTimer just work on Windows Platform
#endif

#include <windows.h> 

class CWatchTimer
{
public:
    CWatchTimer()
    {
        QueryPerformanceFrequency(&m_frequency);
        m_quadpart = (double)m_frequency.QuadPart; 
    }

    LARGE_INTEGER start()
    {
        QueryPerformanceCounter(&m_timeStart);
        return m_timeStart;
    }

    double elapsed() //������һ��start����ʱ�����ѵ�ʱ�䣬��λ:��,����:΢��
    {
        LARGE_INTEGER timeEnd;      //����ʱ��  
        QueryPerformanceCounter(&timeEnd);
        if (m_quadpart == 0)
            return 0;

        return (timeEnd.QuadPart - m_timeStart.QuadPart) / m_quadpart /** 1000000*/;
    }

private:
    LARGE_INTEGER m_timeStart;
    LARGE_INTEGER m_frequency;       
    double        m_quadpart;        

};