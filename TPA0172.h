#ifndef TPA0172
#define TPA0172


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #ifndef I2CWRITE(x)
    #define I2CWRITE(x) Wire.write(x)
  #endif
  #ifndef I2CREAD()
    #define I2CREAD() Wire.read()
  #endif
#else
  #include "WProgram.h"
  #ifndef I2CWRITE(x)
    #define I2CWRITE(x) Wire.send(x)
  #endif
  #ifndef I2CREAD()
    #define I2CREAD() Wire.receive()
  #endif
#endif

#include <inttypes.h>

//#define DEBUGPRINT 1

#define TPA0172ADDRESS0 0x6C
#define TPA0172ADDRESS1 0x6D
#define TPA0172ADDRESS2 0x6E
#define TPA0172ADDRESS3 0x6F

#define BTL_GAIN		0
#define SE_GAIN			2

#define RIGHTGAIN_BTL	0	//Bridged Mode
#define LEFTGAIN_BTL	1	//Bridged Mode

#define RIGHTGAIN_SE	2	//Single-Ended Mode
#define LEFTGAIN_SE		3	//Single-Ended Mode

#define MASK_REGISTER	4

#define CONTROL_REGISTER	5

//Mask Register OR Bitmasks
#define	DISABLE_INT_SHUTDOWN	0x01
#define DISABLE_INT_SEBTL		0x02
#define DISABLE_INT_PCBEEP		0x04

//Mask Register AND Bitmasks
#define ENABLE_INT_SHUTDOWN		0xFE
#define ENABLE_INT_SEBTL		0xFD
#define ENABLE_INT_PCBEEP		0xFB
#define POWERING_UP				0x80	//Read-Only

//Control Register OR Bitmasks
#define	SHUTDOWN				0x01
#define	ENABLE_HEADPHONE_IN		0x02
#define	MUTE					0x08
#define ENABLE_SE				0x20
#define	SELECT_GAIN_REG1		0x40
#define	ENABLE_PCBEEP			0x80

//Control Register AND Bitmasks
#define NOSHUTDOWN				0xFE
#define ENABLE_LINE_IN			0xFD
#define	UNMUTE					0xF7
#define	ENABLE_BTL				0xDF
#define	SELECT_GAIN_REG0		0xBF
#define	DISABLE_PCBEEP			0x7F


class TPA {
public:  
  TPA();
  void begin(uint8_t address);
  void begin(uint8_t address, uint8_t Volume, uint8_t Control);
  void setgain(uint8_t reg, uint8_t gain);
  uint8_t getgain(uint8_t reg);
  void setvolume(uint8_t reg, uint8_t Volume);  //Stereo Volume Control; Sets both channels the same level
  void Increment_Volume(uint8_t reg, uint8_t step);		//Stereo Increment by step value; Increments each channel by the same step
  void Decrement_Volume(uint8_t reg, uint8_t step);		//Stereo Decrement by step value; Decrements each channel by the same step
  void setmask(uint8_t mask);
  uint8_t getmask(void);
  void setcontrol(uint8_t control);
  uint8_t getcontrol(void);
  void mute(void);
  void unmute(void);
  
protected:
  
  
private:
  uint8_t _address;
  uint8_t _TPA_Regs[6];
  void get_registers(void);
  void write_registers(void);
};

#endif