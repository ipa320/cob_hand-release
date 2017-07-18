#ifndef HAND_BRIDGE_HARDWARE_H_
#define HAND_BRIDGE_HARDWARE_H_

#include <boost/chrono.hpp>
#include "serial_port.h"

class HandBridgeHardware {
    boost::chrono::steady_clock::time_point start;
    SerialPort serial;

public:
    void init(char * param)
    {
        if(!serial.init(param)) exit(1);
    }
    unsigned long time()
    {
        return boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::steady_clock::now() - start).count();
    }
    int read(){
        return serial.readByte();
    }
    void write(uint8_t* data, int length)
    {
        serial.write(data, length);
    }
};

#endif // HAND_BRIDGE_HARDWARE_H_