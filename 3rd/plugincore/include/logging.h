/*!
* @brief   
* @project plugincore
* @file    logging.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-14 18:40
*/
//==============================================================================
#ifndef PLUGINCORE_LOGGING_H
#define PLUGINCORE_LOGGING_H
//==============================================================================

#include <stdio.h>
#include <stdarg.h>
#include <mutex>




std::mutex g_mtx;

int log_printf(void (*on_log)(const char *, void *, int), void *pUser, int priority, const char *fmt,
               ...) {
    va_list va;
    char *s;
    int len;

    std::unique_lock<std::mutex> lock(g_mtx);
    if (on_log) {
        len = strlen(fmt) + 512;
        s = (char *)malloc(len + sizeof(char));
        if (!s) {
            return -1;
        }

        va_start(va, fmt);
        vsnprintf(s, len, fmt, va);
        va_end(va);
        s[len - 1] = '\0';

        on_log(s, pUser, priority);

        free(s);
    }

    return 0;
}

#include <sstream>
#include <string>
#include <random>

unsigned int random_char() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

std::string generate_hex(const unsigned int len) {
    std::stringstream ss;
    for (auto i=0; i<len; i++)
    {
        const auto rc = random_char();
        std::stringstream hexstream;
        hexstream << std::hex << rc;
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}


//==============================================================================
#endif // PLUGINCORE_LOGGING_H
//==============================================================================
