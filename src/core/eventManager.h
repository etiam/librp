/*
 * eventManager.h
 *
 *  Created on: Mar 16, 2011
 *      Author: jasonr
 */

#pragma once
#ifndef SGEVENTMANAGER_H_
#define SGEVENTMANAGER_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <tuple>
#include <thread>
#include <boost/signals2.hpp>
#include <boost/utility.hpp>

#include <ut/make_unique.h>
#include <ut/exempt_ptr.h>

#include "classPointers.h"
#include "logging.h"
#include "exception.h"
#include "genericany.h"
#include "ostreamhelpers.h"

namespace {

class stringstreamdumper : public std::stringstream
{
};

template <typename T>
stringstreamdumper &
operator <<(stringstreamdumper &out, const std::vector<T> &attr)
{
    out << "...";
    return out;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

std::string
argDumper()
{
    return "";
}

#pragma clang diagnostic pop

template<typename T, typename... Args>
std::string
argDumper(const T& t, Args... args)
{
    stringstreamdumper str;
    str << t << ", " << argDumper(args...);
    return str.str();
}

};

namespace Sg
{

using Connection = boost::signals2::connection;

template <typename... Args>
using EventData = std::vector<std::tuple<Args...>>;

template <typename... Args>
using Callback = typename boost::signals2::signal<void (const EventData<Args...> &)>::slot_type;

class EventManager : boost::noncopyable
{
  public:
    EventManager();
    ~EventManager() = default;

    void                    batchEvents(bool active);

    /** Emit an event named @name, passing @object to callbacks.
     * @param name name of event to emit.
     * @param object object to pass to callbacks.
     */
    template <typename... Args>
    void                    emit(const std::string &name, const Args... args);

    /** Register a event name.  Subsequent calls to registerCallback can
     * be called with the name once this is called.
     * @param name name of event to register.
     */
    template <typename... Args>
    void                    registerEventType(const std::string &name, bool canBatch=true);

    /** Unregister a event name.  Subsequent calls to registerCallback
     * with the name will fail once this is called.
     * @param name name of event to register.
     */
    void                    unregisterEventType(const std::string &name);

    /** Register a callback function.
     * @param name name of event.
     * @param callback boost::slot_type function to call.
     */
    template <typename... Args>
    Connection              registerCallback(const std::string &name, const Callback<Args...> &callback);

    /** Unregister a callback function.
     * @param name name of event.
     * @param callback boost::slot_type function previously used in
     * @see registerCallback() call.
     */
    void                    unregisterCallback(const Connection &connection);

  private:
    class EventStore
    {
      public:
        virtual ~EventStore() {};
        virtual void emit() = 0;
        virtual void clear() = 0;
    };

    template <typename... Args>
    class EventStoreImpl : public EventStore
    {
      public:
        using Signal = boost::signals2::signal<void (const Sg::EventData<Args...> &)>;

        EventStoreImpl(const std::string &name, const Signal &signal) :
            m_name(name),
            m_signal(signal) {}

        virtual ~EventStoreImpl() {}

        virtual void clear()
        {
            m_events.clear();
        }

        virtual void emit()
        {
            if (m_events.size() > 0)
            {
                LSGE0 << "Sg::EventManager::emit(\"" << m_name << "\", " << m_events.size() << " batched events)";
                m_signal(m_events);
            }
        }

        std::string                         m_name;
        const Signal &                      m_signal;
        EventData<Args...>                  m_events;
    };

    void                                    initializeBatchable();
    using StoreMap = std::map<std::string, std::unique_ptr<EventStore>>;

    std::map<std::thread::id, bool>         m_batchEmissions;
    std::map<std::string, bool>             m_batchableMap;
    std::map<std::string, generic::any>     m_eventsTypeMap;
    std::map<std::thread::id, StoreMap>     m_storesByThread;

    std::thread::id                         m_thread;
};

template <typename... Args>
void
EventManager::emit(const std::string &name, const Args... args)
{
    using Signal = boost::signals2::signal<void (const Sg::EventData<Args...> &)>;

    if (m_eventsTypeMap.find(name) != m_eventsTypeMap.end())
    {
        decltype(std::unique_ptr<Signal>().get()) signal;
        try
        {
            signal = generic::any_cast<std::unique_ptr<Signal> &>(m_eventsTypeMap[name]).get();
        }
        catch (std::bad_cast &e)
        {
            THROW (TypeExc, "EventManager::emit(): Could not emit event \"" << name  << "\", args do not match event signature.");
        }

        auto threadid = std::this_thread::get_id();
        auto &stores = m_storesByThread[threadid];

        // create a vector storing batched events for this event type if it doesn't exist
        if (stores.find(name) == stores.end())
            stores[name] = std::make_unique<EventStoreImpl<Args...>>(name, *signal);

        auto store = dynamic_cast<EventStoreImpl<Args...> *>(stores[name].get());

        initializeBatchable();

        std::stringstream logmsg;

        #ifdef LOGGING_ENABLED
        auto strargs = ::argDumper(args...);
        strargs = strargs.substr(0, strargs.size()-2);
        logmsg << "Sg::EventManager::emit(\"" << name << "\", " << strargs << ")";
        #endif

        // single emission
        if (!m_batchableMap[name] || !m_batchEmissions[threadid])
        {
            logmsg << " (emiting)";
            store->m_events.clear();
            store->m_events.push_back(std::make_tuple(args...));
            (*signal)(store->m_events);
        }

        // batched emission
        else
        {
            logmsg << " (batching)";
            store->m_events.push_back(std::make_tuple(args...));
        }

        LSGE0 << logmsg.str();
        Log::incIndent();
        LSGE1 << "this_thread=" << std::hex << std::showbase << std::this_thread::get_id() << ", m_thread=" << m_thread << std::dec << std::noshowbase;
        Log::decIndent();
    }
    else
    {
        LSGE0 << "Sg::EventManager::emit() : warning, no event named \"" << name << "\" registered";
    }
}

template <typename... Args>
void
EventManager::registerEventType(const std::string &name, bool canBatch)
{
    using Signal = boost::signals2::signal<void (const Sg::EventData<Args...> &)>;

    if (m_eventsTypeMap.find(name) == m_eventsTypeMap.end())
    {
        m_eventsTypeMap[name] = std::make_unique<Signal>();
        m_batchableMap[name] = canBatch;
        LSGE1 << "Sg::EventManager::registerEventType(\"" << name << "\")";
    }
}

template <typename... Args>
Connection
EventManager::registerCallback(const std::string &name, const Callback<Args...> &callback)
{
    auto it = m_eventsTypeMap.find(name);
    if (it == m_eventsTypeMap.end())
        THROW (KeyExc, "EventManager::registerCallback(): Could not register callback for event \"" << name << "\", it does not exist in event map.");

    using Signal = boost::signals2::signal<void (const Sg::EventData<Args...> &)>;

    try
    {
        auto signal = generic::any_cast<std::unique_ptr<Signal> &>(m_eventsTypeMap[name]).get();
        LSGE1 << "Sg::EventManager::registerCallback(\"" << name << "\")";
        return signal->connect(callback);
    }
    catch (std::bad_cast &e)
    {
        THROW (TypeExc, "EventManager::registerCallback(): Could not register callback for event \"" << name << "\", invalid callback function signature.");
    }
    return boost::signals2::connection();
}

}

#endif // SGEVENTMANAGER_H_
