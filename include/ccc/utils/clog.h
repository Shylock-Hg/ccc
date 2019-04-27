/*! \brief standard log information handle lib
 *  \author Shylock Hg
 *  \date 2018-04-14
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NDEBUG

#include <stdio.h>

#define LOG_VERBOSITY_ERROR 0
#define LOG_VERBOSITY_WARN 1
#define LOG_VERBOSITY_INFO 2
#define LOG_VERBOSITY_DEBUG 3
#define LOG_VERBOSITY_ANY 0xFFFFU

#if LOG_VERBOSITY_ERROR >= LOG_VERBOSITY_WARN || \
    LOG_VERBOSITY_WARN >= LOG_VERBOSITY_INFO ||  \
    LOG_VERBOSITY_INFO >= LOG_VERBOSITY_DEBUG || \
    LOG_VERBOSITY_DEBUG >= LOG_VERBOSITY_ANY
#error "Incorrect verbosity order!"
#endif

typedef enum {
    log_verbosity_error = LOG_VERBOSITY_ERROR,
    log_verbosity_warn = LOG_VERBOSITY_WARN,
    log_verbosity_info = LOG_VERBOSITY_INFO,
    log_verbosity_debug = LOG_VERBOSITY_DEBUG,
    log_verbosity_any = LOG_VERBOSITY_ANY
} log_verbosity_t;

// option -- verbosity
#ifndef LOG_VERBOSITY
#define LOG_VERBOSITY LOG_VERBOSITY_ANY  //!< any default
#endif
// option -- color
#ifndef WITH_LOG_COLORFUL
#define WITH_LOG_COLORFUL 1  //!< enable colorful ouptut default
#endif
// option --location
#ifndef WITH_LOG_LOCATION
#define WITH_LOG_LOCATION 1  //!< eanble location information output default
#endif
// option -- file
#ifndef LOG_FILE
#define LOG_FILE stderr  //!< log to stderr default
#endif

#if WITH_LOG_COLORFUL
#define LOG_COLOR_ERROR "\033[1;31m"
#define LOG_COLOR_WARN "\033[1;35m"
#define LOG_COLOR_BOLD "\033[1m"
#define LOG_COLOR_END "\033[0m"
#else
#define LOG_COLOR_ERROR
#define LOG_COLOR_WARN
#define LOG_COLOR_BOLD
#define LOG_COLOR_END
#endif

#if WITH_LOG_LOCATION
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define LOG_LOCATION_LINE TOSTRING(__LINE__)
#define LOG_LOCATION_FILE __FILE__
#else
#define LOG_LOCATION_LINE
#define LOG_LOCATION_FILE
#endif

#define LOG_ERROR "error"
#define LOG_WARN "warn"
#define LOG_INFO "info"
#define LOG_DEBUG "debug"

// inner implementation
#define _LOGD(format, ...)                                             \
    do {                                                               \
        fprintf(LOG_FILE,                                              \
                LOG_LOCATION_FILE ":" LOG_LOCATION_LINE ": " LOG_DEBUG \
                                  ": " format,                         \
                ##__VA_ARGS__);                                        \
    } while (0);

#define _LOGI(format, ...)                                            \
    do {                                                              \
        fprintf(LOG_FILE,                                             \
                LOG_LOCATION_FILE ":" LOG_LOCATION_LINE ": " LOG_INFO \
                                  ": " format,                        \
                ##__VA_ARGS__);                                       \
    } while (0);

#define _LOGW(format, ...)                                                \
    do {                                                                  \
        fprintf(LOG_FILE,                                                 \
                LOG_COLOR_BOLD LOG_LOCATION_FILE                          \
                ":" LOG_LOCATION_LINE                                     \
                ": " LOG_COLOR_WARN LOG_WARN LOG_COLOR_END LOG_COLOR_BOLD \
                ": " format,                                              \
                ##__VA_ARGS__);                                           \
        fprintf(LOG_FILE, LOG_COLOR_END);                                 \
    } while (0);

#define _LOGE(format, ...)                                                  \
    do {                                                                    \
        fprintf(LOG_FILE,                                                   \
                LOG_COLOR_BOLD LOG_LOCATION_FILE                            \
                ":" LOG_LOCATION_LINE                                       \
                ": " LOG_COLOR_ERROR LOG_ERROR LOG_COLOR_END LOG_COLOR_BOLD \
                ": " format,                                                \
                ##__VA_ARGS__);                                             \
        fprintf(LOG_FILE, LOG_COLOR_END);                                   \
    } while (0);

/*! \brief printf log to
 *  \param format format string \NOTE must be literal for concating
 * */
#if LOG_VERBOSITY_DEBUG < LOG_VERBOSITY
#define LOGD(format, ...) _LOGD(format, ##__VA_ARGS__)
#define LOGI(format, ...) _LOGI(format, ##__VA_ARGS__)
#define LOGW(format, ...) _LOGW(format, ##__VA_ARGS__)
#define LOGE(format, ...) _LOGE(format, ##__VA_ARGS__)
#elif LOG_VERBOSITY_INFO < LOG_VERBOSITY
#define LOGD(format, ...)
#define LOGI(format, ...) _LOGI(format, ##__VA_ARGS__)
#define LOGW(format, ...) _LOGW(format, ##__VA_ARGS__)
#define LOGE(format, ...) _LOGE(format, ##__VA_ARGS__)
#elif LOG_VERBOSITY_WARN < LOG_VERBOSITY
#define LOGD(format, ...)
#define LOGI(format, ...)
#define LOGW(format, ...) _LOGW(format, ##__VA_ARGS__)
#define LOGE(format, ...) _LOGE(format, ##__VA_ARGS__)
#elif LOG_VERBOSITY_ERROR < LOG_VERBOSITY
#define LOGD(format, ...)
#define LOGI(format, ...)
#define LOGW(format, ...)
#define LOGE(format, ...) _LOGE(format, ##__VA_ARGS__)
#else
#warning "Useless log!"
#define LOGD(format, ...)
#define LOGI(format, ...)
#define LOGW(format, ...)
#define LOGE(format, ...)
#endif  //!< verbosity conditional compile

// undefine to anti expose inner function implementation directly
/*
#undef _LOGE
#undef _LOGW
#undef _LOGI
#undef _LOGD
*/

#endif  //!< #ifndef NDBUG

#ifdef __cplusplus
}
#endif
