#include "readUART.h"

int readUART::configureUART(const char *portName) {
    int uartFD = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uartFD == -1) {
        std::cerr << "Nie udało się otworzyć portu UART: " << portName << std::endl;
        exit(-1); // Nie udało się skonfigurować UART
    }

    struct termios options;
    tcgetattr(uartFD, &options);

    // Ustawienie prędkości transmisji
    cfsetispeed(&options, virables::baudRate);
    cfsetospeed(&options, virables::baudRate);

    // Konfiguracja: 8N1 (8 bitów danych, brak parzystości, 1 bit stopu)
    options.c_cflag &= ~PARENB; // Bez parzystości
    options.c_cflag &= ~CSTOPB; // 1 bit stopu
    options.c_cflag &= ~CSIZE; // Czyszczenie maski wielkości danych
    options.c_cflag |= CS8; // 8 bitów danych

    // Włącz odbiór i ustaw tryb bez kontroli sprzętowej
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // Wyłącz sterowanie programowe
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Tryb surowy
    options.c_oflag &= ~OPOST; // Tryb surowy (wyjście)

    if (tcsetattr(uartFD, TCSANOW, &options) != 0) {
        std::cerr << "Błąd konfiguracji UART" << std::endl;
        close(uartFD);
        return -1;
    }

    return uartFD;
}

// Funkcja do odczytu danych z UART
std::string readUART::readFromUART(int uartFD) {
    static std::string buffer; // Bufor dla danych z UART
    char tempBuffer[256]; // Tymczasowy bufor do odczytu
    memset(tempBuffer, 0, sizeof(tempBuffer));

    // Odczyt danych do tymczasowego bufora
    int bytesRead = read(uartFD, tempBuffer, sizeof(tempBuffer) - 1);

    if (bytesRead < 0) {
        //std::cerr << "Błąd odczytu z UART: " << strerror(errno) << std::endl;
        return "";
    }

    if (bytesRead == 0) {
        // Brak danych, zwróć pusty string
        return "";
    }

    // Dodanie odczytanych danych do bufora
    buffer += std::string(tempBuffer, bytesRead);

    // Sprawdzanie, czy w buforze są pełne linie
    size_t pos = buffer.find('\n');
    if (pos != std::string::npos) {
        // Znaleziono koniec linii
        std::string line = buffer.substr(0, pos); // Wyodrębnij linię
        buffer.erase(0, pos + 1); // Usuń przetworzoną linię z bufora
        return line; // Zwróć kompletną linię
    }

    // Jeśli brak pełnej linii, zwróć pusty string
    return "";
}


// Funkcja do wyszukiwania dostępnych portów UART
std::vector<std::string> readUART::findUARTPorts() {
    std::vector<std::string> ports;
    char buffer[128];

    // Uruchomienie komendy systemowej za pomocą popen
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(virables::command.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "Nie udało się wykonać komendy: " << virables::command << std::endl;
        return ports;
    }

    // Odczyt danych wyjściowych komendy
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        std::string port(buffer);
        port.erase(port.find_last_not_of(" \n\r\t") + 1); // Usunięcie białych znaków na końcu
        ports.push_back(port);
    }

    if (ports.empty()) {
        std::cout << "Nie znaleziono żadnych portów UART." << std::endl;
        exit(1);
    } else {
        std::cout << "Znaleziono następujące porty UART:" << std::endl;
        for (const auto &port: ports) {
            std::cout << " - " << port << std::endl;
        }
    }

    return ports;
}

inputData readUART::getInputData(const std::string &input) {
    inputData result;
    std::stringstream ss(input);
    std::string temp;
    char delimiter = ';';

    try {
        // Odczytaj pierwszą wartość
        if (std::getline(ss, temp, delimiter)) {
            result.input_1 = std::stoi(temp);
        } else {
            throw std::runtime_error("Brak pierwszej wartości w danych wejściowych.");
        }

        // Odczytaj drugą wartość
        if (std::getline(ss, temp, delimiter)) {
            result.input_2 = std::stoi(temp);
        } else {
            throw std::runtime_error("Brak drugiej wartości w danych wejściowych.");
        }

        // Odczytaj trzecią wartość (do końca linii)
        if (std::getline(ss, temp)) {
            result.input_3 = std::stoi(temp);
        } else {
            throw std::runtime_error("Brak trzeciej wartości w danych wejściowych.");
        }
    } catch (const std::invalid_argument &e) {
    } catch (const std::out_of_range &e) {
    }

    return result;
}
