#include <Arduino.h> //To use some of the standart Arduino classes and functions
#include <FlexCAN_T4.h> //Used to create the Communication to the CAN Bus

#define BAUDRATE_CAN 500000

#define APPS_PIN 

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

bool endiannessState = 0; // 0 -> little (árabe) , 1 -> big (ordem tuga)

short int msgCount = 0;

void updateEndiannessState(){
    if(msgCount == 5){
        endiannessState = !endiannessState;
        msgCount = 0;
    }
    msgCount += 1;
}



void canSetup(){
    can1.begin();
    can1.setBaudRate(BAUDRATE_CAN);
}


void sendAPPS(int val) {
    //stdint.h comes in the Arduino core
    uint8_t byte1 = (val >> 8) & 0xFF;  // MSB
    uint8_t byte2 = val & 0xFF;         // LSB

    CAN_message_t msg;
    msg.id = 0x111;
    msg.len = 2;
    msg.buf[0] = endiannessState? byte1 : byte2;
    msg.buf[1] = endiannessState? byte2 : byte1;

    can1.write(msg);
}

void setup(){
    pinMode(APPS_PIN, INPUT);
    canSetup();
}

void loop(){
    int apps_val = analogRead(APPS_PIN);

    updateEndiannessState()
    sendAPPS(apps_val);
}
