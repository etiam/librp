/*
 * selectionManager.cpp
 *
 *  Created on: Nov 1, 2012
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "selectionManager.h"

namespace Sg
{

SelectionManager::SelectionManager()
{
}

void
SelectionManager::selectNode(const NodePtr &node)
{
    m_selection.insert(node);
    m_list.clear();
    m_list.reserve(m_selection.size());
    for (const auto &node : m_selection)
        m_list.push_back(node);
}

void
SelectionManager::clearSelection()
{
    m_selection.clear();
    m_list.clear();
}

const NodeList &
SelectionManager::selection() const
{
    return m_list;
}

} /* namespace Sg */
