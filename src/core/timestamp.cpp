#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <ios>

#include "exception.h"
#include "timestamp.h"

using namespace std::chrono;

namespace Sg
{

Timestamp::Timestamp() :
    m_valid(false)
{
}

Timestamp::~Timestamp()
{
}

void
Timestamp::set()
{
    m_timestamp = high_resolution_clock::now();
    m_valid = true;
}

time_point<high_resolution_clock>
Timestamp::get() const
{
    if (!m_valid)
        THROW (SyntaxExc, "Timestamp::get() : timestamp not set.");
    return m_timestamp;
}

bool
Timestamp::valid() const
{
    return m_valid;
}

long int
Timestamp::delta() const
{
    if (!m_valid)
        THROW (SyntaxExc, "Timestamp::delta() : timestamp not set.");
    return duration_cast<microseconds>(high_resolution_clock::now() - m_timestamp).count();
}

bool
Timestamp::operator !=(const Timestamp &other) const
{
    return m_timestamp != other.m_timestamp;
}

bool
Timestamp::operator ==(const Timestamp &other) const
{
    return m_timestamp == other.m_timestamp;
}

bool
Timestamp::operator <=(const Timestamp &other) const
{
    return m_timestamp <= other.m_timestamp;
}

bool
Timestamp::operator >=(const Timestamp &other) const
{
    return m_timestamp >= other.m_timestamp;
}

bool
Timestamp::operator >(const Timestamp &other) const
{
    return m_timestamp > other.m_timestamp;
}

bool
Timestamp::operator <(const Timestamp &other) const
{
    return m_timestamp < other.m_timestamp;
}

};
