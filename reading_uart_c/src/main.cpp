#include <readUART.h>

int main() {
    inputData tempData;

    // Wyszukaj dostępne porty UART
    std::vector<std::string> uartPorts = readUART::findUARTPorts();

    // Nazwa portu
    const char *portName = uartPorts.at(0).c_str();

    // Konfiguracja UART
    int uartFD = readUART::configureUART(portName);

    // Odczyt danych z UART
    std::string data;
    while (true) {
        try { data = readUART::readFromUART(uartFD); } catch (const std::exception &e) {
        }

        if (!data.empty()) {
            tempData = readUART::getInputData(data);
            std::cout << "Otrzymano: " << tempData.input_1 << ", " << tempData.input_2 << ", " << tempData.input_3 <<
                    std::endl;
        }
    }

    // Zamknięcie portu UART
    close(uartFD);
    return 0;
}
