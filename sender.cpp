// Li-Fi sender  -  REFERENCE CODE (Windows, compile with g++ in MSYS2)
//
// Reads a text file and sends its characters to the ESP32 over a COM port.
// After each character the ESP32 replies with '.', so we send one
// character at a time and wait for that acknowledgement.
//
// Compile:  g++ sender.cpp -o sender.exe
// Run:      sender.exe COM3 message.txt

#include <windows.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: sender.exe COMx message.txt\n";
        return 1;
    }

    // Read the whole text file
    std::ifstream file(argv[2], std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << argv[2] << "\n";
        return 1;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    // Open the COM port
    std::string port = std::string("\\\\.\\") + argv[1];
    HANDLE h = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        std::cerr << "Cannot open " << argv[1] << "\n";
        return 1;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(h, &dcb);
    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity   = NOPARITY;
    SetCommState(h, &dcb);

    COMMTIMEOUTS to = {0};
    to.ReadTotalTimeoutConstant = 2000;   // wait up to 2 s for the ack
    to.WriteTotalTimeoutConstant = 1000;
    SetCommTimeouts(h, &to);

    Sleep(2000);   // ESP32 resets when the port opens

    std::cout << "Sending " << text.size() << " characters...\n";
    for (size_t i = 0; i < text.size(); i++) {
        DWORD n = 0;
        WriteFile(h, &text[i], 1, &n, NULL);

        char ack = 0;
        ReadFile(h, &ack, 1, &n, NULL);
        if (n == 0) {
            std::cerr << "No acknowledgement at character " << i << "\n";
            break;
        }
    }
    std::cout << "Done.\n";

    CloseHandle(h);
    return 0;
}
