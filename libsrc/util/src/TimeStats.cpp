#include "TimeStats.h"

TimeStats::TimeStats()
{
   m_unit = MSEC; 
   m_isEnable = 1;
}

TimeStats::TimeStats(Unit unit, int isEnable)
{
    m_isEnable = isEnable;
    m_unit = unit;
}

void TimeStats::enable()
{
    m_isEnable = 1;
}

void TimeStats::disable()
{
    m_isEnable = 0;
}

void TimeStats::start()
{
    if (m_isEnable)
    {
        gettimeofday(&m_start, NULL);
    }
}

long TimeStats::stop()
{
    if (m_isEnable)
    {
        gettimeofday(&m_end, NULL);

        return ((m_end.tv_sec - m_start.tv_sec) * 1000000 + 
                m_end.tv_usec - m_start.tv_usec) / m_unit;
    }

    return 0;
}
