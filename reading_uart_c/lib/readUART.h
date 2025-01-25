#include "virables.h"

#ifndef READUART_H
#define READUART_H

struct inputData {
    int input_1;
    int input_2;
    int input_3;
};

namespace readUART {
    int configureUART(const char *portName);

    std::string readFromUART(int uartFD);

    std::vector<std::string> findUARTPorts();

    inputData getInputData(const std::string &input);
}

#endif //READUART_H
