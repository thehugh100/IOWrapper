#include <inttypes.h>
#include <iostream>
#include <string_view>
#include "modem.h"

uint32_t adler32(unsigned char *data, size_t len) 
{
    uint32_t a = 1, b = 0;
    size_t index;
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

int main() {
    const char data[] = "This is a large array of text\nthat will have to be split over multiple packets.\nHere's another line of data";
    Modem m(32);
    m.setSendFunc([&m](const uint8_t* d, size_t l) { m.receiveData(d, l); });
    m.setReceivedFunc([](const uint8_t* d, size_t l) {
        std::cout << "Data received: " << l << " Bytes." << std::endl;
        std::cout << '"' << std::string_view((const char*)d, l) << '"' << std::endl;
    });
    //std::cout << std::hex << adler32((uint8_t*)data, sizeof(data)) << std::endl;
}