/*
 * timer.cpp
 *
 *  Created on: Mar 30, 2011
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>

#include "timer.h"

using namespace std::chrono;

namespace Sg
{

Timer::Timer() :
    m_stopped(false),
    m_start(time_point<high_resolution_clock>()),
    m_stop(time_point<high_resolution_clock>())
{
}

void
Timer::start()
{
    m_stopped = false;
    m_start = high_resolution_clock::now();
}

void
Timer::stop()
{
    m_stopped = true;
    m_stop = high_resolution_clock::now();
}

double
Timer::elapsedTime() const
{
    if(m_stopped)
        return duration_cast<microseconds>(m_stop - m_start).count() / 1000.0f;
    else
        return duration_cast<microseconds>(high_resolution_clock::now() - m_start).count() / 1000.0f;
}

std::ostream &
operator <<(std::ostream &out, const Timer &timer)
{
    if(timer.m_stopped)
        out << duration_cast<microseconds>(timer.m_stop - timer.m_start).count() / 1000.0f;
    else
        out << duration_cast<microseconds>(high_resolution_clock::now() - timer.m_start).count() / 1000.0f;

    return out;
}

}
