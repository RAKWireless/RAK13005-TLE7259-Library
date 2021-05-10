/**
 * @file lin_bus.h
 * @author rakwireless.com
 * @brief TLE7259-3GE LIN bus library
 * @version 0.1
 * @date 2021-05-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include  "lin_bus.h"
/**
  \brief      Constructor for LIN BUS interface
  \details    Initialize  linbus object
  \param[in]  hwSerial    Hardware Serial port
  \param[in]  lin_version LIN protocol version it can be LIN_V1(lin1.x) or LIN_V2(lin2.x)
  \param[in]  enPin       control TLE7259-3GE EN pin
  \param[in]  wkPin       control TLE7259-3GE Wakeup pin
  \param[in]  txPin       linbus Serial TXD pin
  \return     no
*/
lin_bus::lin_bus(HardwareSerial &hwSerial,LIN_version_t lin_version, int enPin, int wkPin, int txPin) :
  _serial(&hwSerial),
  _lin_version(lin_version),
  _enPin(enPin),
  _wkPin(wkPin),
  _txPin(txPin)
{
}
/**
  \brief      Initialize the device as master
  \param[in]  baudrate    Hardware Serial port Baud rate
  \return     If the initialization succeeds return true else return false
*/
bool lin_bus::master(uint16_t baudrate)
{
  _baudrate = baudrate;  
  _baudrate1 = _baudrate1 = 0.692*_baudrate;//_baudrate1 = 9/(13/_baudrate) = 9*_baudrate/13 = 0.6923076923*_baudrate;  
  if((_baudrate<1000)||(_baudrate>20000)) //According to the LINBUS protocol, the baud rate ranges from 1000 to 20000
  {
    return(false); 
  }
   #if (LIN_DEBUG_LEVEL >= 1)
   {
    LIN_DEBUG_SERIAL.printf("_baudrate1 is %d\r\n",_baudrate1);
   } 
   #endif 
  if(begin(_baudrate))
  {
   oneBitPeriod = 1000000.0f/_baudrate;
   sleep(0);
   return true; 
  }
  else
  {
   return false;  
  }  
}
/**
  \brief      Initialize the device as slaver
  \param[in]  baudrate  Hardware Serial port Baud rate
  \param[in]  ident   device id
  \return     If the initialization succeeds return true else return false
*/
bool lin_bus::slave(uint16_t baudrate, uint8_t ident)
{
  _identByte = ident; // saving idet to private variable
  _baudrate = baudrate;  
  _baudrate1 = 0.692*_baudrate;//_baudrate1 = 9/(13/_baudrate) = 9*_baudrate/13 = 0.6923076923*_baudrate;  
  if((_baudrate<1000)||(_baudrate>20000)) //According to the LINBUS protocol, the baud rate ranges from 1000 to 20000
  {
    return(false); 
  }
   #if (LIN_DEBUG_LEVEL >= 1)
   {
    LIN_DEBUG_SERIAL.printf("_baudrate1 is %d\r\n",_baudrate1);
   } 
   #endif 
  if(begin(_baudrate))
  {
   oneBitPeriod = 1000000.0f/_baudrate;
   sleep(1);
   return true; 
  }
  else
  {
   return false;  
  }  
}
/**
  \brief      config the serial port
  \param[in]  baudrate  Hardware Serial port Baud rate
  \return     If the configuration succeeds return true else return false
*/
bool lin_bus::begin(uint16_t baudrate)
{  
 unsigned long begintime,currentTime;
// initialize serial interface   
  _serial->begin(_baudrate, SERIAL_8N1);
//    begintime = millis();
//    while(!(*_serial))
//    {
//      currentTime = millis();
//      if((currentTime - begintime) > 1000)
//      {
//        return false;
//      }
//    }   
//   _serial->setTimeout(2);
   return true;
}
/**
  \brief      Release LIN BUS
  \param[in]  void
  \return     void
*/
void lin_bus::end()
{
  _serial->end();
//  _serial->setTimeout(1000);
}

/**
  \brief      Check if data has arrived on the LIN BUS
  \param[in]  void
  \return     number of bytes in the RX queue, 0 if no data is available
*/
int lin_bus::available()
{
  return _serial->available();
}

/**
  \brief      Get first byte in the RX queue of the LIN bus (does not remove the data from the RX queue)
  \param[in]  void
  \return     first byte in the RX queue
*/
int lin_bus::peek()
{
  return _serial->peek();
}

/**
  \brief      Read first byte in the RX queue of the LIN bus
  \param[in]  void
  \return     first byte in the RX queue
*/
int lin_bus::read(void)
{
  return _serial->read();
}

/**
  \brief      Flush waiting bytes from the TX queue of the LIN bus
  \param[in]  void
  \return     void
*/
void lin_bus::flush()
{
  return _serial->flush();
}

/**
  \brief      Send a byte over the LIN bus
  \param[in]  uint8_t byte to be sent
  \return     void
*/
size_t lin_bus::write(uint8_t b)
{  
  return _serial->write(b);
}

lin_bus::operator bool()
{
  return true;
}
/**
  \brief      config the TLE7259-3GE control pin
  \param[in]  enPin       control TLE7259-3GE EN pin
  \param[in]  wkPin       control TLE7259-3GE Wakeup pin
  \param[in]  txPin       linbus Serial TXD pin
  \return     always return 1
*/
int lin_bus::setPins(int enPin,int wkPin,int txPin )
{
  _enPin = enPin;
  _wkPin = wkPin;
  _txPin = txPin;
  return 1;
}
/**
  \brief      set pin output low
  \param[in]  pin   set pin
  \return     void
*/
void lin_bus::setPinLow(int pin)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
}
/**
  \brief      set pin output high
  \param[in]  pin   set pin
  \return     void
*/
void lin_bus::setPinHigh(int pin)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
}
/**
  \brief      config TLE7259-3GE sleep mode
  \param[in]  sleep_state   set 1 to normal set 0 to sleep
  \return     void
*/
int lin_bus::sleep(uint8_t sleep_state)
{
  if(sleep_state==1){ // Go to Normal mode
    setPinHigh(_enPin);
  }else if(sleep_state==0){ // Go to Sleep mode
    setPinLow(_enPin);
  }
  delayMicroseconds(60); // According to TLE7259-3GE datasheet this is needed for proper working max value is 150 min is 60
  return 1;
}
/**
  \brief      Calculate protected LIN ID.
  \details    Method to calculate the protected LIN identifier as described in LIN2.0 spec "2.3.1.3 Protected identifier field"
  \param[in]  id      frame ID (protection optional)
  \return     protected LIN identifier
*/
uint8_t lin_bus::protectID(uint8_t id)
{
  uint8_t  pid;       // result = protected ID
  uint8_t  tmp;       // temporary variable for calculating parity bits
  // copy (unprotected) ID
  pid = id;
  // protect ID  with parity bits
  pid  = (uint8_t) (pid & 0x3F);                                          // clear upper bit 6&7
  tmp  = (uint8_t) ((pid ^ (pid>>1) ^ (pid>>2) ^ (pid>>4)) & 0x01);       // -> pid[6] = PI0 = ID0^ID1^ID2^ID4
  pid |= (uint8_t) (tmp << 6);
  tmp  = (uint8_t) (~((pid>>1) ^ (pid>>3) ^ (pid>>4) ^ (pid>>5)) & 0x01); // -> pid[6] = PI1 = ~(ID1^ID3^ID4^ID5)
  pid |= (uint8_t) (tmp << 7);
  // return protected identifier
  return pid;
} // lin_bus::protectID()

/**
  \brief      Calculate LIN frame checksum.
  \details    Method to calculate the LIN frame checksum as described in LIN2.0 spec
  \param[in]  id          frame ID  
  \param[in]  data        buffer containing data bytes
  \param[in]  numData     number of data bytes in frame
  \return     calculated frame checksum
*/
uint8_t lin_bus::checkSum(uint8_t id, uint8_t *data, uint8_t numData)
{
  uint16_t chk=0x00;
  // protect the ID
  id = protectID(id);
  // LIN2.x uses extended checksum which includes protected ID, i.e. including parity bits
  // LIN1.x uses classical checksum only over data bytes
  // Diagnostic frames with ID 0x3C and 0x3D/0x7D always use classical checksum (see LIN spec "2.3.1.5 Checkum")
  if (!((_lin_version == LIN_V1) || (id == 0x3C) || (id == 0x7D)))    // if version 2  & no diagnostic frames (0x3C=60 (PID=0x3C) or 0x3D=61 (PID=0x7D))
    chk = (uint16_t) id;

  // loop over data bytes
  for (uint8_t i = 0; i < numData; i++)
  {
    chk += (uint16_t) (data[i]);
    if (chk>255)
      chk -= 255;
  }
  chk = (uint8_t)(0xFF - ((uint8_t) chk));   // bitwise invert

  // return frame checkSum
  return chk;

} // lin_bus::checksum()
/**
  \brief      for generating Synch Break
  \param[in]  no_bits    set serial Pause send bits
  \return     serial Pause send bits time
*/
uint16_t lin_bus::serialPause(int no_bits)
{
  // Calculate delay needed for 13 bits, depends on bound rate
  unsigned int del = oneBitPeriod*no_bits; // delay for number of bits (no-bits) in microseconds, depends on period
  _serial->flush();    //清空串口缓存
  _serial->end();  
  setPinHigh(_txPin);
  delayMicroseconds(oneBitPeriod/2); //delayMicroseconds(oneBitPeriod/2); //
  setPinLow(_txPin);
  delayMicroseconds(del); 
  setPinHigh(_txPin);
  delayMicroseconds(oneBitPeriod); //delayMicroseconds(oneBitPeriod/2); // 
  return del;
}
// PUBLIC METHODS
// WRITE methods   1 to 8 bytes of data: This length is limited by how fast non-crystal clocks typically wander
// Creates a LIN packet and then send it via USART(Serial) interface.
/**
  \brief      write whole package    
  \param[in]  ident          frame ID  
  \param[out]  *data       data buffer pointer  
  \param[out]  data_size   data buffer size  
  \return     max transfer frame use time in milliscond
*/
uint16_t lin_bus::write(uint8_t ident, uint8_t *data, uint8_t data_size)
{
  // Calculate checksum
  uint8_t check = checkSum(ident, data, data_size);
  uint8_t id = protectID(ident);
  #if (LIN_DEBUG_LEVEL >= 1)
   {
    LIN_DEBUG_SERIAL.printf("protectID is %d\r\n",id);
    LIN_DEBUG_SERIAL.printf("CheckSum is %d\r\n",check);
   } 
   #endif 
  // Start interface
  sleep(1); // Go to Normal mode
  // Synch Break
    serialPause(13);
    _serial->begin(_baudrate); // config Serial
    _serial->write(0x55); // write Synch Byte to serial
    _serial->write(id); //_serial->write(ident); // write Identification Byte to serial
    for(int i=0;i<data_size;i++) _serial->write(data[i]); // write data to serial
    _serial->write(check); // write Checksum Byte to serial
    _serial->flush(); //ensure transfer all
    _serial->end(); // clear Serial config  
    sleep(0); // Go to Sleep mode      
    transferTime = (44+10*data_size)*oneBitPeriod*1.4;//(34+(data_size+1)*10)*oneBitPeriod*1.4=(44+10*data_size)*oneBitPeriod*1.4us; //max single frame transfer time
    transferTime = transferTime*0.001;  //us to ms
   #if (LIN_DEBUG_LEVEL >= 1)
   {    
    LIN_DEBUG_SERIAL.printf("_baudrate1 is %d\r\n",_baudrate1);
    LIN_DEBUG_SERIAL.printf("_baudrate is %d\r\n",_baudrate);
    LIN_DEBUG_SERIAL.printf("max transfer Time is %d\r\n",transferTime);
   } 
   #endif 
  return transferTime;
}
/**
  \brief       read data from LIN bus, checksum and ident validation
  \param[out]  *data       data buffer pointer  
  \param[out]  data_size   data buffer size  
  \return      if read data is right return 1 else return 0
*/
int lin_bus::read(uint8_t *data, uint8_t data_size)
{
  uint8_t rec[data_size+3]; 
  uint8_t readData[data_size+1]; 
//    if(_serial->read() != -1)
//    while (!Serial.available()){delay(200);};
    if(_serial->available()>(data_size+3))
    { // Check if there is an event on LIN bus      
      g_head2 = g_head1;
      g_head1 = _serial->read();   
      #if (LIN_DEBUG_LEVEL >= 2)
         {         
          LIN_DEBUG_SERIAL.printf("DEBUG_readBytes g_head1 is: %d\r\n",g_head1);
          LIN_DEBUG_SERIAL.printf("DEBUG_readBytes g_head2 is: %d\r\n",g_head2);
         } 
      #endif     
      if((g_head2==0x55)&&(validateParity(g_head1)))
      {
        _serial->readBytes(readData,data_size+1);
        rec[0] = g_head2;
        rec[1] = g_head1;
        for(int i=0;i<(data_size+1);i++)
        {
          rec[i+2] = readData[i];          
        }          
         #if (LIN_DEBUG_LEVEL >= 2)
         {
          for(int i=0;i<(data_size+3);i++)
          {
            LIN_DEBUG_SERIAL.printf("DEBUG_readBytes data[%d] is %d\r\n",i,rec[i]);
          }
         } 
        #endif  
        if(validateChecksum(rec,data_size+3))
        {
          for(int j=0;j<data_size;j++)
          {
           data[j] = rec[j+2];             
          }
          return 1;
        }  
        return 0;
      }
      else
      {
        return 0;  
      } 
    }
   return 0;
}
/**
  \brief       read data from LIN BUS
  \param[out]  *data       data buffer pointer  
  \param[out]  data_size   data buffer size  
  \return      if read data available return 1 else return 0
*/
int lin_bus::readStream(uint8_t *data,uint8_t data_size)
{
  uint8_t rec[data_size];
//    if(_serial->read() != -1)
  if(_serial->available()>0)
    { // Check if there is an event on LIN bus
      _serial->readBytes(rec,data_size);
      for(int j=0;j<data_size;j++)
      {
        data[j] = rec[j];
       #if (LIN_DEBUG_LEVEL >= 1)
         {
          LIN_DEBUG_SERIAL.printf("DEBUG_readBytes data[%d] is %d\r\n",j,data[j]);
         } 
       #endif        
      }
      return 1;
    } 
  return 0;
}
/**
  \brief      validate LIN frame protect ID
  \param[in]  ident          frame ID   
  \return     if validate is ok return 1 else return 0
*/
uint8_t lin_bus::validateParity(uint8_t ident) 
{
  uint8_t identByte = protectID(_identByte);
//  if(ident == _identByte)
  if(ident == identByte)
  {
   #if (LIN_DEBUG_LEVEL >= 2)
   {
    LIN_DEBUG_SERIAL.printf("validateParity ID is succeed!%d\r\n");
   } 
   #endif  
   return 1; 
  }    
  else
  {
   #if (LIN_DEBUG_LEVEL >= 2)
   {
     LIN_DEBUG_SERIAL.printf("validateParity ID is error!%d\r\n");
   } 
   #endif 
   return 0; 
  }    
}
/**
  \brief      validate LIN frame checksum.  
  \param[in]  *data       data buffer pointer  
  \param[in]  data_size   data buffer size  
  \return     if validate checksum ok return 1 else return 0
*/
uint8_t lin_bus::validateChecksum(uint8_t *data, uint8_t data_size)
{ 
  uint16_t chk=0x00;
  // protect the ID
  uint8_t id = protectID(data[1]);
  uint8_t checksum = data[data_size-1];
  // LIN2.x uses extended checksum which includes protected ID, i.e. including parity bits
  // LIN1.x uses classical checksum only over data bytes
  // Diagnostic frames with ID 0x3C and 0x3D/0x7D always use classical checksum (see LIN spec "2.3.1.5 Checkum")
  if (!((_lin_version == LIN_V1) || (id == 0x3C) || (id == 0x7D)))    // if version 2  & no diagnostic frames (0x3C=60 (PID=0x3C) or 0x3D=61 (PID=0x7D))
    chk = (uint16_t) id;
  // loop over data bytes
  for (uint8_t i = 2; i < (data_size-1); i++)
  {
    chk += (uint16_t) (data[i]);
    if (chk>255)
      chk -= 255;
  }
  chk = (uint8_t)(0xFF - ((uint8_t) chk));   // bitwise invert 
    
   #if (LIN_DEBUG_LEVEL >= 1)
   {
    LIN_DEBUG_SERIAL.printf("DEBUG_receive checksum is %d\r\n",checksum);
    LIN_DEBUG_SERIAL.printf("DEBUG_validate Checksum is %d\r\n",chk);
   } 
   #endif 
  if(chk == checksum)
    return 1;
  else
    return 0;
} 
/**
  \brief      send wakeup frame for waking up all bus participants
  \param[in]  void     
  \return     always return 1
*/
int lin_bus::busWakeUp(void)
{    
  sleep(1); // Go to Normal mode
  setPinHigh(_wkPin);
  delayMicroseconds(80);
  return 1;
}
/**
  \brief      Calculate packet transfertime
  \param[in]  void     
  \return     time the transfer will take
*/
uint16_t lin_bus::waitTransferTime(uint16_t baudrate, uint8_t dataSize)
{    
  uint16_t maxtTansferTime;
  maxtTansferTime = (dataSize*10+34)*(1000.f/baudrate)*1.4;
  return maxtTansferTime;
}
