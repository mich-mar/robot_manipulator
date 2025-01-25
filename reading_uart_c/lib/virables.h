#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

#ifndef VIRABLES_H
#define VIRABLES_H

namespace virables {
    inline int baudRate = 9600;
    inline const std::string &command = "ls /dev/ttyACM*";
}

#endif //VIRABLES_H
