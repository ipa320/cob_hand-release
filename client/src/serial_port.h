#ifndef HAND_BRIDGE_SERIAL_PORT_H_
#define HAND_BRIDGE_SERIAL_PORT_H_

#include "pigpio.h"

#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

class SerialPort {
    int ser_port;
public:
    bool isOpen() const{
        return ser_port >= 0 && serDataAvailable(ser_port) >= 0;
    }
    bool init(char * param)
    {
        unsigned int  baud = 57600;
        if(char * delim = strchr(param, '@')){
            *delim = 0;
            baud = boost::lexical_cast<unsigned int>(delim + 1);
        }
        ser_port = serOpen(param, baud, 0);
        printf("%s opened as %d\n", param, ser_port);
        return isOpen();
    }

    template<typename Duration> int waitData(const Duration& duration){
        boost::chrono::steady_clock::time_point deadline = boost::chrono::steady_clock::now() + duration;
        int len;
        while((len = serDataAvailable(ser_port)) == 0 && boost::chrono::steady_clock::now() < deadline)
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        return len;
    }

    int readByte(){
        return serReadByte(ser_port);
    }
    int read(char *buf, unsigned count){
        return serRead(ser_port, buf, count);
    }
    bool write(uint8_t* data, int length)
    {
        return serWrite(ser_port, reinterpret_cast<char*>(data), length) == 0;
    }
    bool write(const std::string &line){
        return serWrite(ser_port, const_cast<char *>(line.c_str()), line.size()) == 0;
    }
    void close(){
        if(isOpen()) serClose(ser_port);
    }
    SerialPort() : ser_port(-1) {}
    ~SerialPort(){
        close();
    }
};


#endif // HAND_BRIDGE_SERIAL_PORT_H_