#pragma once
#ifndef SGTIMESTAMP_H
#define SGTIMESTAMP_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <chrono>

namespace Sg
{

class Timestamp;

}

namespace Core
{

class Archive;

void serialize(Archive &ar, Sg::Timestamp &t);

}

namespace Sg
{

class Timestamp
{
  public:
    Timestamp();
    ~Timestamp();

    void                                                            set();
    std::chrono::time_point<std::chrono::high_resolution_clock>     get() const;

    bool                                                            valid() const;

    long int                                                        delta() const;

    bool                                                            operator !=(const Timestamp &other) const;
    bool                                                            operator ==(const Timestamp &other) const;

    bool                                                            operator <=(const Timestamp &other) const;
    bool                                                            operator >=(const Timestamp &other) const;
    bool                                                            operator <(const Timestamp &other) const;
    bool                                                            operator >(const Timestamp &other) const;

  private:
    bool                                                            m_valid;

    std::chrono::time_point<std::chrono::high_resolution_clock>     m_timestamp;

    friend void Core::serialize(Core::Archive &ar, Timestamp &t);
};

}

#endif
