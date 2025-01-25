#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#ifndef READUART_H
#define READUART_H

namespace readUART {
    inline int baudRate = 9600;

    int configureUART(const char *portName);
    std::string readFromUART(int uartFD);
    std::vector<std::string> findUARTPorts();
}

#endif //READUART_H
