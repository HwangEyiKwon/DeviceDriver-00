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

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int prevReadValue = 0;
    int curReadValue = 0;

    for (int currentReadCount = 0; currentReadCount < READ_COUNT; currentReadCount++) {
        this_thread::sleep_for(chrono::milliseconds(200));
        
        int curReadValue = (int)(m_hardware->read(address));
        if (prevReadValue != curReadValue && currentReadCount != 0) {
            throw ReadFailException();
        }
        prevReadValue = curReadValue;
    }

    return curReadValue;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}