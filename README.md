| <center><img src="./assets/rakstar.jpg" alt="RAKstar" width=25%></center>  | ![RAKWireless](./assets/RAK-Whirls.png) | [![Build Status](https://github.com/RAKWireless/RAK13005-TLE7259-Library/workflows/RAK%20Library%20Build%20CI/badge.svg)](https://github.com/RAKWireless/RAK13005-TLE7259-Library/actions) |
| -- | -- | -- |

# TLE7259

The TLE7259 is a LIN bus transceiver chip from Infineon. This library provides basic support for LIN bus master and slave functionality.

[*RAKwireless RAK13005 LIN bus interface*](https://docs.rakwireless.com/Product-Categories/WisBlock/#wisblock-io)

# Documentation

* **[Product Repository](https://github.com/RAKWireless/RAK13005-TLE7259-Library)** - Product repository for the RAKWireless RAK13005 LIN Bus module.
* **[Documentation](https://docs.rakwireless.com/Product-Categories/WisBlock/#wisblock-io)** - Documentation and Quick Start Guide for the RAK13005 LIN Bus module.

# Installation

In Arduino IDE open Sketch->Include Library->Manage Libraries then search for RAK13005.    

In PlatformIO open PlatformIO Home, switch to libraries and search for RAK13005. 
Or install the library project depend by adding 
```log
lib_deps =
  rakwireless/RAKwireless TLE7259 LIN Bus library
```
into **`platformio.ini`**

For manual installation download the archive, unzip it and place the RAK13005-TLE7259-Library folder into the library directory.    
In Arduino IDE this is usually <arduinosketchfolder>/libraries/    
In PlatformIO this is usually <user/.platformio/lib>     

# Usage

The library provides a lin_bus class, which allows communication to the TLE7259 transceiver IC. Check out the examples how to setup a LIN Master or LIN Slave node.    

## This class provides the following methods:

**lin_bus(HardwareSerial &hwSerial, LIN_version_t lin_version, int enPin, int wkPin, int txPin);**    
Constructor for LIN BUS interface. Initialize lin_bus object    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| in        | hwSerial | Hardware Serial port |
| in        | lin_version | LIN protocol version it can be LIN_V1(lin1.x) or LIN_V2(lin2.x) | 
| in        | enPin | control TLE7259-3GE EN pin | 
| in        | wkPin | control TLE7259-3GE Wakeup pin | 
| in         | txPin | linbus Serial TXD pin | 
|  return |  | none  | 

**bool master(uint16_t baudrate);**
Initialize the device as master    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| in          | baudrate  | Hardware Serial port Baud rate  | 
|  return | | If the initialization succeeds return true else return false  | 

**bool slave(uint16_t baudrate, uint8_t ident);**
Initialize the device as slaver     
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| in          | baudrate | Hardware Serial port Baud rate | 
| in          | ident | device id | 
|  return | | If the initialization succeeds return true else return false | 

**int read(uint8_t \*data, uint8_t data_size);**
Read data from LIN bus, checksum and ident validation    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| out          | *data | data buffer pointer |  
| out          | data_size | data buffer size |  
|  return | | if read data is right return 1 else return 0 |

**int readStream(uint8_t \*data,uint8_t data_size);**
Read data stream from LIN BUS    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| out          | *data | data buffer pointer |   
| out          | data_size | data buffer size |   
|  return | | if read data available return 1 else return 0 | 

**uint16_t write(uint8_t ident, uint8_t \*data, uint8_t data_size);**
Write whole packet bbb    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| in          | ident | frame ID |   
| out          | *data | data buffer pointer |   
| out          | data_size | data buffer size |   
|  return | | max transfer frame use time in milliscond | 

**int setPins(int enPin,int wkPin,int txPin );**
Configure the TLE7259-3GE control pin    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
| in          | enPin | control TLE7259-3GE EN pin | 
| in          | wkPin | control TLE7259-3GE Wakeup pin | 
| in          | txPin | linbus Serial TXD pin | 
|  return | | always return 1 | 

**int sleep(uint8_t sleep_state);**    
Parameters:    
Configure TLE7259-3GE sleep mode (0 - sleep, 1 - normal operation)     
| Direction | Name | Function | 
| --------- | ---- | -------- |
| in          | sleep_state | set 1 to normal operation, set 0 to sleep | 
|  return | | void | 

**int busWakeUp(void);**
Send wakeup frame for waking up all bus participants    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
|  return | | always return 1 | 

**uint16_t waitTransferTime(uint16_t baudrate,uint8_t dataSize);**
Calculate packet transfertime    
Parameters:    
| Direction | Name | Function | 
| --------- | ---- | -------- |
|  return | | time the transfer will take |

