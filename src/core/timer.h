/*
 * timer.h
 *
 *  Created on: Mar 30, 2011
 *      Author: jasonr
 */

#pragma once
#ifndef SGTIMER_H_
#define SGTIMER_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iosfwd>
#include <chrono>

namespace Sg
{

class Timer
{
  public:
    Timer();
    ~Timer() = default;

    void                                                            start();
    void                                                            stop();

    double                                                          elapsedTime() const;

  private:
    friend std::ostream&                                            operator <<(std::ostream &out, const Timer &timer);

    bool                                                            m_stopped;

    std::chrono::time_point<std::chrono::high_resolution_clock>     m_start;
    std::chrono::time_point<std::chrono::high_resolution_clock>     m_stop;
};

}

#endif /* TIMER_H_ */
