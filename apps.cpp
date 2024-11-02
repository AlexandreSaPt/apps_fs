#include <Arduino.h> //To use some of the standart Arduino classes and functions
#include <FlexCAN_T4.h> //Used to create the Communication to the CAN Bus

#define BAUDRATE_CAN 500000

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

void canSetup(){
    can1.begin();
    can1.setBaudRate(BAUDRATE_CAN);
}

void sendAPPS(int val) {
    uint8_t byte1 = (val >> 8) & 0xFF;  // MSB
    uint8_t byte2 = val & 0xFF;         // LSB

    CAN_message_t msg;
    msg.id = 0x111;
    msg.len = 2;
    msg.buf[0] = byte2;
    msg.buf[1] = byte1;

    can1.write(msg);
}

