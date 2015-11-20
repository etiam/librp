#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "node.h"
#include "relationshipType.h"
#include "relationship.h"

namespace Sg
{

Relationship::Relationship(const RelationTypePtr &subject, const RelationshipTypePtr &type, const RelationTypePtr &object) :
    m_subject(subject),
    m_object(object),
    m_type(type)
{
}

Relationship::~Relationship()
{
}

const RelationTypePtr &
Relationship::subject() const
{
    return m_subject;
}

const RelationTypePtr &
Relationship::object() const
{
    return m_object;
}

const RelationshipTypePtr &
Relationship::type() const
{
    return m_type;
}

}
