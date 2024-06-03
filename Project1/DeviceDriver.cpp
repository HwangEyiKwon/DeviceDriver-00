#include "DeviceDriver.h"
#include <thread>
#include <chrono>

using namespace std;

class ReadFailException : public exception {
public:
    char const* what() const override {
        return "Read Fail Exception Occured!!!!!\n";
    }
};
class WriteFailException : public exception {
public:
    char const* what() const override {
        return "Write Fail Exception Occured!!!!!\n";
    }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int firstReadValue = (int)(m_hardware->read(address));

    for (int currentReadCount = 1; currentReadCount < READ_COUNT; currentReadCount++) {
        DelayMilliseconds(200);
        
        int curReadValue = (int)(m_hardware->read(address));
        if (firstReadValue != curReadValue) {
            throw ReadFailException();
        }
    }

    return firstReadValue;
}

void DeviceDriver::write(long address, int data)
{
    int readValue = (int)(m_hardware->read(address));
    if (readValue != 0xFF) {
        throw WriteFailException();
    }
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::DelayMilliseconds(int number)
{
    this_thread::sleep_for(chrono::milliseconds(number));
}