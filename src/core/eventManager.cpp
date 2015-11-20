/*
 * eventManager.cpp
 *
 *  Created on: Mar 16, 2011
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <memory>

#include "exception.h"
#include "classPointers.h"
#include "eventManager.h"

namespace Sg
{

EventManager::EventManager() :
    m_thread(std::this_thread::get_id())
{
}

void
EventManager::batchEvents(bool active)
{
    LSGE0 << "Sg::EventManager::batchEvents(" << std::hex << std::showbase << std::this_thread::get_id() << ", "
          << (active ? "true" : "false") << ")" << std::dec << std::noshowbase;

    initializeBatchable();
    auto threadid = std::this_thread::get_id();

    // set new state before possibly emitting bached events below
    // as emission could call something that could call Sg::emit()
    auto calledBatchEmission = m_batchEmissions[threadid];
    m_batchEmissions[threadid] = active;

    // turning off batch mode, process all stored events
    if (!active && calledBatchEmission)
    {
        LSGE1 << "emitting following batches:";
        for (const auto &store : m_storesByThread[threadid])
            LSGE1 << " " << store.first;

        for (const auto &store : m_storesByThread[threadid])
            if (m_batchableMap[store.first])
                store.second->emit();
    }

    // turning on batch mode.  clear all previous stores
    else if (active && !calledBatchEmission)
    {
        for (const auto &store : m_storesByThread[threadid])
            store.second->clear();
    }
}

void
EventManager::unregisterEventType(const std::string &name)
{
    auto i = m_eventsTypeMap.find(name);
    if (i != m_eventsTypeMap.end())
    {
        m_eventsTypeMap.erase(i);
    }
    else
    {
        LSGE0 << "EventManager::unregisterEventType(): warning, could not unregister unknown event type \"" << name  << "\"";
    }

    // TODO : unregister all the existing callbacks of this event
}

void
EventManager::unregisterCallback(const Connection &connection)
{
    if (connection.connected())
        connection.disconnect();
}

void
EventManager::initializeBatchable()
{
    auto threadid = std::this_thread::get_id();
    if (m_batchEmissions.find(threadid) == std::end(m_batchEmissions))
        m_batchEmissions[threadid] = false;
}

}
