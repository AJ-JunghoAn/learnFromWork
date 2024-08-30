#include <iostream>
#include "motorlib.h"

HANDLE hSerial;

// Sending CAN msg 
bool sendCANMessage(uint32_t id, uint8_t* data, uint8_t length) {
    // CAN msg format packing
    char buffer[256];
    int n = snprintf(buffer, sizeof(buffer), "t%03X%d", id, length);
    for (int i = 0; i < length; i++) {
        n += snprintf(buffer + n, sizeof(buffer) - n, "%02X", data[i]);
    }
    snprintf(buffer + n, sizeof(buffer) - n, "\r");

    DWORD bytesWritten;
    if (!WriteFile(hSerial, buffer, strlen(buffer), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port" << std::endl;
        return false;
    }

    return true;
}


// Little Endian
void toLittleEndian(uint32_t value, uint8_t* buffer) {

    buffer[0] = static_cast<uint8_t>(value & 0xFF);          // LSB (Least Significant Byte)
    buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);   // Next byte
    buffer[2] = static_cast<uint8_t>((value >> 16) & 0xFF);  // Next byte
    buffer[3] = static_cast<uint8_t>((value >> 24) & 0xFF);  // Next byte
    buffer[4] = static_cast<uint8_t>((value >> 32) & 0xFF);  // MSB (Most Significant Byte)

}


// Control word - Enable
void sendEnable(uint32_t nodeId, uint16_t controlWord) {

    uint8_t data[6] = { 0x2B, 0x40, 0x60, 0x00 };
    data[4] = 0x2F;
    data[5] = 0x00;
    //sendCANMessage(nodeId, data, 6);

}


// Control word - Disable
void sendDisable(uint32_t nodeId, uint16_t controlWord) {

    uint8_t data[6] = { 0x2B, 0x40, 0x60, 0x00 };
    data[4] = 0x0F;
    data[5] = 0x00;
    //sendCANMessage(nodeId, data, 6);

}


// Speed Mode
void sendSpeedMode(uint32_t nodeId, bool position) {

    uint8_t data[5] = { 0x2F, 0x60, 0x60, 0x00 };
    data[4] = 0x03;  // Speed Mode : 3

    // CAN 메시지를 전송합니다.
    //sendCANMessage(nodeId, data, 5);

}


// Position Mode
void sendPositionMode(uint32_t nodeId, bool position) {

    uint8_t data[5] = { 0x2F, 0x60, 0x60, 0x00 };
    data[4] = 0x01;  // Position Mode : 1
    

    // CAN 메시지를 전송합니다.
    //sendCANMessage(nodeId, data, 5);

}


// Read Actual position
void readCurrentPosition(uint32_t nodeId) {

    uint8_t data[4] = { 0x40, 0x64, 0x60, 0x00 };

    // CAN 메시지를 전송합니다.
    //sendCANMessage(nodeId, data, 4);

}


// Speed
void sendSpeedModeSpeed(uint32_t nodeId) {

    uint32_t SpeedModeSpeed;
    std::cout << "Enter the RPM value (0 to 0xFFFFFF): ";
    std::cin >> std::hex >> SpeedModeSpeed;  // Read as hexadecimal 

    // Ensure position is within the range
    if (SpeedModeSpeed > 0xFFFFFF) {
        std::cerr << "Position value out of range" << std::endl;
        return;
    }

    uint8_t data[8] = { 0x23, 0x81, 0x60 };

    toLittleEndian(SpeedModeSpeed, data + 3); // 입력값에 대한 데이터 처리


    data[6] = 0x00;
    data[7] = 0x00;

    //sendCANMessage(nodeId, data, 8);
}


// Acceleration
void sendmotorAccelration(uint32_t nodeId) {

    uint32_t motorAccel;
    std::cout << "Enter the Motor Accelration value (0 to 0xFFFFFF): ";
    std::cin >> std::hex >> motorAccel;  // Read as hexadecimal 

    // Ensure position is within the range
    if (motorAccel > 0xFFFFFF) {
        std::cerr << "Position value out of range" << std::endl;
        return;
    }

    uint8_t data[8] = { 0x23, 0x83, 0x60 };

    toLittleEndian(motorAccel, data + 3); // 입력값에 대한 데이터 처리


    //data[6] = 0x00;
    //data[7] = 0x00;

    //sendCANMessage(nodeId, data, 8);
}


// Speed Mode STOP
void StopSpeedMode(uint32_t nodeId, bool position) {

    uint8_t data[6] = { 0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01 };
    
    // CAN 메시지를 전송합니다.
    //sendCANMessage(nodeId, data, 5);

}


// Location cache
void sendLocationCache(uint32_t nodeId) {

    uint32_t position;
    std::cout << "Enter the position value (0 to 0xFFFFFF): ";
    std::cin >> std::hex >> position;  // Read as hexadecimal 

    // Ensure position is within the range
    if (position > 0xFFFFFF) {
        std::cerr << "Position value out of range" << std::endl;
        return;
    }

    uint8_t data[8] = { 0x23, 0x7A, 0x60 };

    toLittleEndian(position, data + 3); // Position 입력값에 대한 데이터 처리


    //data[6] = 0x00;
    //data[7] = 0x00;

    //sendCANMessage(nodeId, data, 8);
}


