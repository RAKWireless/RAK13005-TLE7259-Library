/**
 * @file lin_bus.cpp
 * @author rakwireless.com
 * @brief TLE7259-3GE LIN bus library
 * @version 0.3
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _LIN_BUS_H_
#define _LIN_BUS_H_

#include <Arduino.h>

#define LIN_DEBUG_SERIAL   Serial       // Serial interface used for debug output
#define LIN_DEBUG_LEVEL    0           // Debug level (0=no output, 1=error msg, 2=sent/received bytes)

/**
//    \brief LIN version of checksum
//*/
typedef enum {
    LIN_V1            = 1,          // LIN protocol version 1
    LIN_V2            = 2           // LIN protocol version 2
} LIN_version_t;
/**
    \brief LIN communication error codes
*/
class lin_bus : public Stream {
  public:
    lin_bus(HardwareSerial &hwSerial,LIN_version_t lin_version, int enPin, int wkPin, int txPin); // Constructor for LIN BUS interface
    bool master(uint16_t baudrate);  // Initialize the device as master
    bool slave(uint16_t baudrate, uint8_t ident);  // Initialize the device as slaver

    int read(uint8_t *data, uint8_t data_size); // read data from LIN bus, checksum and ident validation
	int listen(uint8_t ident,uint8_t *data, uint8_t data_size); // read data from LIN bus slaver node, checksum and ident validation
    int readStream(uint8_t *data,uint8_t data_size);  // read data from LIN bus
    uint16_t write(uint8_t ident, uint8_t *data, uint8_t data_size);  // write whole package  
    uint16_t writeRequest(uint8_t ident); // master write request  
    int writeResponse(uint8_t *data,uint8_t data_size); // slaver write response  
      
    int setPins(int enPin,int wkPin,int txPin );  // config the TLE7259-3GE control pin
    int sleep(uint8_t sleep_state);   // method for controlling transceiver modes (0 - sleep, 1 - normal)   
    int busWakeUp(void);  // send wakeup frame for waking up all bus participants
    uint16_t waitTransferTime(uint16_t baudrate,uint8_t dataSize);  // calculate max frame transfer time

    uint8_t validateParity(uint8_t ident); // for validating Identification Byte
    uint8_t checkSum(uint8_t id, uint8_t *data,uint8_t numData);  // calculate frame checksum
    uint8_t checkSum(uint8_t *data, uint8_t numData);  // calculate frame checksum
    uint8_t validateChecksum(uint8_t *data, uint8_t data_size); // for validating checksum Byte
    uint8_t protectID(uint8_t id); // calculate protected LIN ID    
// Private methods and variables
  private:
   // internal variables
    HardwareSerial* _serial;  //pointer to used serial
    int _enPin;   //control TLE7259-3GE EN pin
    int _wkPin;   //control TLE7259-3GE Wakeup pin
    int _txPin;   //LIN BUS used Serial port TXD pin  

    LIN_version_t  _lin_version;  //LIN version for checksum calculation can be LIN_V1 or LIN_V2
    uint8_t _identByte;   // user defined Identification Byte

    uint16_t oneBitPeriod;    //single bit transfer time in microseconds
    uint16_t transferTime;    //single frame transfer time in milliseconds
    
    uint16_t _baudrate;  //LIN BUS communication Baud rate
    uint16_t _baudrate1; //LIN BUS serial port Baud rate for send break
    uint8_t g_head1 = 0x55;  //rx check head1
    uint8_t g_head2 = 0;  //rx check head2
    void setPinLow(int pin);  // set pin output low
    void setPinHigh(int pin); // set pin output high

    uint16_t serialPause(int no_bits);  // for generating Synch Break
     
    virtual bool begin(uint16_t baudrate); //config LIN BUS serial port
    virtual void end();
    virtual int available();
    virtual int peek();
    virtual int read(void);
    virtual void flush();
    virtual size_t write(uint8_t b);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    virtual operator bool();
};
#endif
