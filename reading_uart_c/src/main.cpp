#include <readUART.h>

int main() {
    // Wyszukaj dostępne porty UART
    std::cout << "Szukam dostępnych portów UART..." << std::endl;
    std::vector<std::string> uartPorts = readUART::findUARTPorts();

    if (uartPorts.empty()) {
        std::cout << "Nie znaleziono żadnych portów UART." << std::endl;
        exit(1);
    } else {
        std::cout << "Znaleziono następujące porty UART:" << std::endl;
        for (const auto &port: uartPorts) {
            std::cout << " - " << port << std::endl;
        }
    }

    // Nazwa portu i prędkość transmisji
    const char *portName = uartPorts.at(0).c_str(); // W systemie Linux /dev/ttyS0 to często pierwszy port szeregowy

    // Konfiguracja UART
    int uartFD = readUART::configureUART(portName);
    if (uartFD == -1) {
        return 1; // Nie udało się skonfigurować UART
    }

    // Odczyt danych z UART
    std::cout << "Oczekiwanie na dane z UART..." << std::endl;
    std::string data;
    while (true) {
        try { data = readUART::readFromUART(uartFD); } catch (const std::exception &e) {
        }

        if (!data.empty()) {
            std::cout << "Otrzymano: " << data << std::endl;
        }
    }

    // Zamknięcie portu UART
    close(uartFD);
    return 0;
}
