/**---------------------------------------------------------------------------
 *
 * @file    log.h
 * @brief   Logging with lepto
 *
 * Logging functions analog to Qt.
 * There are following basic functions:
 *
 *    lInfo(msg, ...)
 *    lCaution(msg, ...)
 *    lWarning(msg, ...)
 *    lCritical(msg, ...)
 *    lFatal(msg, ...)
 *
 * @date      20140107
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/

#include <lepto/lepto.h>
#include <lepto/log.h>
#include <lepto/log_private.h>
#include <lepto/logPrinter.h>
#include <lepto/ring.hpp>

#include <cstddef>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT) || IS_ENABLED(CONFIG_LEPTO_LOG_ANSI_COLORS)
#include <lepto/ansi.h>
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_TIMESTAMPS)
#include <sys/time.h>
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT) && IS_ENABLED(CONFIG_LEPTO_LOG_DOWNSIZE)
#error Having LEPTO_LOG_DOWNSIZE and LEPTO_LOG_USE_PRETTY_PRINT enabled at the same time does not make sense.
#endif

#if !IS_ENABLED(CONFIG_LEPTO_LOG_DIRECT_PRINT)
    static CRing<SLogEntry> logs(CONFIG_LEPTO_LOG_MAX_ENTRIES);
    static bool logOverflow = false;

    void leptoInitLog()
    {
    }
#else
    void leptoInitLog()
    {
    }
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT)
void lVLogPretty(const char* file, int line, ELogCode code, const char *format, va_list list)
#else
void lVLogSimple(ELogCode code, const char *format, va_list list)
#endif
{
#if !IS_ENABLED(CONFIG_LEPTO_LOG_DIRECT_PRINT)
    if (!logs.pushable())
    {
        logOverflow = true;
        return;
    }

    int leIndex = logs.tryReserve();
    SLogEntry *le = static_cast<SLogEntry *>(logs.reservedEntry(leIndex));

    if (!le)
    {
        return;
    }
#else
    SLogEntry sle;
    SLogEntry *le = &sle;
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT)
    le->file = file;
    le->line = line;
#if IS_ENABLED(CONFIG_LEPTO_LOG_TIMESTAMPS)
    le->timeStamp = clock() / 1000;
#endif
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_CALLBACK)
    le->code = code;
#endif

#if !IS_ENABLED(CONFIG_LEPTO_LOG_SILENT)
    vsnprintf(&le->logString[0], CONFIG_LEPTO_LOG_MAX_STRING_LENGTH, format, list);
#endif

    if (toCategory(code) == ELogCategory::Fatal)
    {
#if defined(STM32)
        abort();
#else
        printf("FATAL: %s\n", le->logString);
        throw(le->logString);
#endif
    }

#if !IS_ENABLED(CONFIG_LEPTO_LOG_DIRECT_PRINT)
    logs.pushReserved(leIndex);
#else
#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT)
    logPrintPretty(le);
#else
    logPrintSimple(le);
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_CALLBACK)
    logCallBack(le);
#endif
#endif
}

#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT)
void lLogPretty(const char* file, int line, ELogCode code, const char *format, ...)
{
    va_list list;
    va_start(list, format);
    lVLogPretty(file, line, code, format, list);
    va_end(list);
}
#else
void lLogSimple(ELogCode code, const char *format, ...)
{
    va_list list;
    va_start(list, format);
    lVLogSimple(code, format, list);
    va_end(list);
}
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_SIGNAL)
#error CONFIG_LEPTO_LOG_SIGNAL is obsolete. Use CONFIG_BIWAK_LOG_SIGNAL
#endif

void logEventLoop()
{
#if !IS_ENABLED(CONFIG_LEPTO_LOG_DIRECT_PRINT)
    const SLogEntry* le = nullptr;
    while ((le = static_cast<const SLogEntry *>(logs.frontEntry())))
    {
        if ((int)toCategory(le->code) < (int)ELogCategory::Function)
        {
#if IS_ENABLED(CONFIG_LEPTO_LOG_PRETTY_PRINT)
            logPrintPretty(le);
#else
            logPrintSimple(le);
#endif
        }

#if IS_ENABLED(CONFIG_LEPTO_LOG_CALLBACK)
        logCallBack(le);
#endif

#if IS_ENABLED(CONFIG_LEPTO_LOG_SIGNAL)
        m_signalLog.emitSignal(le->code);
#endif
        logs.dropFront();
    }

    if (logOverflow)
    {
        logOverflow = false;
        lCritical("LOF");
    }
#endif
}

#if IS_ENABLED(CONFIG_LEPTO_LOG_CALLBACK)
__attribute__((weak))
void logCallBack(const SLogEntry *le)
{
    (void)le;
};
#endif
