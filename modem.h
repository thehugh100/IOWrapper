#pragma once

#include <inttypes.h>
#include <vector>
#include <functional>

typedef std::function<void(const uint8_t* data, size_t length)> Modem_IO_Func_t;

class Modem {
public:
    Modem(
        size_t xferChunkSize = 1024, 
        Modem_IO_Func_t sendImpl = nullptr, 
        Modem_IO_Func_t receivedImpl = nullptr, 
        Modem_IO_Func_t headerImpl = nullptr
    );

    void setSendFunc(Modem_IO_Func_t func);
    void setReceivedFunc(Modem_IO_Func_t func);
    void setReceivedHeaderFunc(Modem_IO_Func_t func);
    void sendData(const uint8_t* data, size_t length);
    void receiveData(const uint8_t* data, size_t length);

private:
    Modem_IO_Func_t sendImpl;
    Modem_IO_Func_t receivedImpl;
    Modem_IO_Func_t headerImpl;

    size_t xferChunkSize;
    std::vector<uint8_t> receiveBuf;
    std::vector<uint8_t> receiveHeaderBuf;
    int targetReceiveSize;
    int state;
};