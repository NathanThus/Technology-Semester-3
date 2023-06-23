#include <Arduino.h>

class UART
{
private:
    const int dataBits = 8;
    const int stopBits = 1;
    const int parity = 0;

    int baudRate;
    int inputPin;
    int outputPin;

void SampleByte();
int CheckForStartBit();

public:
    UART(int baudRate, int inputPin, int outputPin);

    bool Recieve(char& data);
    void Send(char data);
    void Send(int data);
    void SendLine(String string);
    void SetParity(int parity);

    ~UART() = default;
};
