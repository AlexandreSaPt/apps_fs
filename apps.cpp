#include <Arduino.h> //To use some of the standart Arduino classes and functions
#include <FlexCAN_T4.h> //Used to create the Communication to the CAN Bus

#define BAUDRATE_CAN 500000
#define APPS_PIN A0
#define ENDIANNESS_CYCLE_NUMBER 5

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;

bool endiannessState = 0; // 0 -> little Endian , 1 -> big Endian (natural order)

short int msgCount = 0;


/**
 * @brief Updates the endianness state
 * @note This function is used to change the endianness state every ENDIANNESS_CYCLE_NUMBER messages sent
 */
void updateEndiannessState(){
    if(msgCount == ENDIANNESS_CYCLE_NUMBER){
        endiannessState = !endiannessState;
        msgCount = 0;
    }
    msgCount += 1;
}

/**
 * @brief Sets up the CAN Bus communication
 * @note This setup doesn't prepare the CAN Bus to receive messages, only to send them
 */
void canSetup(){
    canBus.begin();
    canBus.setBaudRate(BAUDRATE_CAN);
}


/**
 * @brief Sends the APPS value to the CAN Bus
 * @param val The value to be sent to the CAN Bus
 * @note The value is sent in two bytes, so the integer value is split into two bytes (its 2 least significant bytes)
 */
void sendAPPS(int val) {
    //stdint.h comes in the Arduino core
    uint8_t byte1 = (val >> 8) & 0xFF;  // MSB
    uint8_t byte2 = val & 0xFF;         // LSB

    //using CAN_message_t struct from FlexCAN_T4 library
    CAN_message_t msg;
    msg.id = 0x111;
    msg.len = 2;
    msg.buf[0] = endiannessState? byte1 : byte2;
    msg.buf[1] = endiannessState? byte2 : byte1;

    canBus.write(msg);
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
