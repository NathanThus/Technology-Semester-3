#include <Arduino.h>

class UART
{
private:
    const int BaudRate = 0;
    const int InputPin = 0;
    const int OutputPin = 0;

    // Time Values
    const unsigned long SampleTime = 0;
    const unsigned long TimePerBit = 0;
    
    // Sampling
    const int NumberOfSamples = 0;

    // Reciever
    void SampleByte();
    int CheckForStartBit();
    void Reset();
    char RetrieveData();

    // Inbound Validation
    bool CheckParity(int parityData);
    bool ValidateByte();
    bool ValidateStartBits();
    bool ValidateStopBits();
    int PreCalculateParityData();

    // Transmitter

public:
    UART(int baudRate, int inputPin, int outputPin, int samples);

    bool Receive(char& data);
    void Send(char data);
    void Send(int data);
    void SendLine(String string);

    ~UART() = default;
};
