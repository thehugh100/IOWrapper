#include "modem.h"
#include <iostream>

Modem::Modem(
    size_t xferChunkSize, 
    Modem_IO_Func_t sendImpl, 
    Modem_IO_Func_t receivedImpl, 
    Modem_IO_Func_t headerImpl)
    : sendImpl(sendImpl), receivedImpl(receivedImpl), headerImpl(headerImpl)
{
    xferChunkSize = 32;
    state = 0;
    targetReceiveSize = 0;
}

void Modem::setSendFunc(Modem_IO_Func_t func) {
    sendImpl = func;
}

void Modem::setReceivedFunc(Modem_IO_Func_t func) {
    receivedImpl = func;
}

void Modem::setReceivedHeaderFunc(Modem_IO_Func_t func) {
    headerImpl = func;
}

void Modem::sendData(const uint8_t* data, size_t length) {
    sendImpl((uint8_t*)&length, sizeof(length));
    for(int i = 0; i < length; i+=xferChunkSize) {
        if(sendImpl) {
            sendImpl(data+i, std::min(length - i, xferChunkSize));
        } else {
            std::cerr << "Modem Error: No implementation for send function!" << std::endl;
            break;
        }
    }
}
void Modem::receiveData(const uint8_t* data, size_t length) {
    for(int i = 0;  i < length; i++) {
        if(state == 0) {
            receiveHeaderBuf.push_back(data[i]);
            if(receiveHeaderBuf.size() >= sizeof(size_t)) {
                targetReceiveSize = *(size_t*)receiveHeaderBuf.data();
                if(headerImpl) {
                    headerImpl(receiveHeaderBuf.data(), receiveHeaderBuf.size());
                }
                state = 1;
            }
        } else if(state == 1) {
            receiveBuf.push_back(data[i]);
            if(receiveBuf.size() >= targetReceiveSize) {
                if(receivedImpl) {
                    receivedImpl(receiveBuf.data(), receiveBuf.size());
                } else {
                    std::cerr << "Modem Error: No implementation for received function!" << std::endl;
                }
                receiveHeaderBuf.clear();
                receiveBuf.clear();
                state = 0;
            }
        }
    }
}