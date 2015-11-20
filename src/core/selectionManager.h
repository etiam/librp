/*
 * selectionManager.h
 *
 *  Created on: Nov 1, 2012
 *      Author: jasonr
 */

#ifndef SELECTIONMANAGER_H_
#define SELECTIONMANAGER_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <set>

#include "classPointers.h"
#include "global.h"
#include "nodeManager.h"

namespace Sg
{

class SelectionManager
{
  public:
    SelectionManager();
    ~SelectionManager() = default;

    void                    selectNode(const NodePtr &node);
    void                    clearSelection();

    const NodeList &        selection() const;

  private:
    std::set<NodePtr>       m_selection;
    NodeList                m_list;
};

} /* namespace Sg */

#endif /* SELECTIONMANAGER_H_ */
