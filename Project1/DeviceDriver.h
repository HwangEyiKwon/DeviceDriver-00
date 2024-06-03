#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    const int READ_COUNT = 5;

    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;

private:
    void DelayMilliseconds(int number);
};