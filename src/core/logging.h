/*
 * logging.h
 *
 *  Created on: Jul 21, 2012
 *      Author: jasonr
 */

#ifndef SGLOGGING_H_
#define SGLOGGING_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <log/log.h>

#define LSGC0 LIBLOG_LOG(Log::channel("sg:core"), 0)
#define LSGC1 LIBLOG_LOG(Log::channel("sg:core"), 1)
#define LSGC2 LIBLOG_LOG(Log::channel("sg:core"), 2)
#define LSGC3 LIBLOG_LOG(Log::channel("sg:core"), 3)

#define LSGE0 LIBLOG_LOG(Log::channel("sg:events"), 0)
#define LSGE1 LIBLOG_LOG(Log::channel("sg:events"), 1)
#define LSGE2 LIBLOG_LOG(Log::channel("sg:events"), 2)
#define LSGE3 LIBLOG_LOG(Log::channel("sg:events"), 3)

#define LSGN0 LIBLOG_LOG(Log::channel("sg:node"), 0)
#define LSGN1 LIBLOG_LOG(Log::channel("sg:node"), 1)
#define LSGN2 LIBLOG_LOG(Log::channel("sg:node"), 2)
#define LSGN3 LIBLOG_LOG(Log::channel("sg:node"), 3)

#endif
