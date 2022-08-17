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
        char* buf = new char[256];
        readlink("/proc/self/exe", buf, 256);
        if (buf[0] == 0)
            throw std::runtime_error("Path finding failed for unknown reason");

        path = string(buf);
        path = path.substr(0, path.find_last_of('/'));
    }

    return path;
}

#endif //SABT_GETPATH_H
