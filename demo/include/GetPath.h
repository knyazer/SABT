//
// Created by knyaz on 15/08/22.
//

#ifndef SABT_GETPATH_H
#define SABT_GETPATH_H

#include <unistd.h>
#include <string>

using std::string;

/**
 * Gets and returns path to the executable. Calls /proc/self/exe only one time, thus saving a bit of performance.
 * Linux only.
 */
static inline string getPath() {
    static string path;

    if (path.empty()) {
        char* buff = new char[256];
        auto len = readlink("/proc/self/exe", buff, 256);
        path = string(buff);
        path = path.substr(0, path.find_last_of('/'));
    }

    return path;
}

#endif //SABT_GETPATH_H
