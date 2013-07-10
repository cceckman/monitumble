#monitumble

The overall goal of this project is to change the configured orientation of a screen
(that is, the orientation that the computer uses) to match that of the physical monitor
when the monitor is rotated.
I plan to do this by sticking a couple of accelerometers to the backs of my monitors,
hooking them up to a control module, and hooking that in to the computer to talk to
some software that does the screen orientation change.

This repository will be mostly prototyping for a while.

##Project references

This project makes use of the Arduino Makefile project (https://github.com/sudar/Arduino-Makefile).


#Hardware info

Current protyping hardware:
    2x Freescale MMA7455L accelerometer modules
        SainSmart package: http://www.amazon.com/gp/product/B0057OC62E/
    1x Freeduino (Diecimila clone, probably)
        [ And a couple of MOSFETS and resistors for level conversion. ]
    1x MacBook Pro (no external monitors), Windows 7

Eventual hardware will include
    2x ASUS VS228H-P    (21.5" LCD monitor)
    1x Dual monitor stand (http://www.amazon.com/gp/product/B002R9HQLI/)
    1x Desktop computer, Windows Server 2012 with various hosted VMs including Windows 8.1 and Linux

##Diecimila special pinouts
###Capabilities:
SER             : Serial communication, should be reserved for such to talk with computer.
INT             : Interrupt trigger (low, rising, falling, or change). See http://arduino.cc/en/Reference/AttachInterrupt
PWM             : 8-bit PWM output via http://arduino.cc/en/Reference/AnalogWrite
SPI             : SPI communication (SPI software library required).
LED             : Built-in LED.
I2C             : I2C communication.

###Capabilities by pin:
00 : SER (RX)
01 : SER (TX)
02 : INT (0)
03 : INT (1),    PWM
04 : I2C (SDA)
05 : I2C (SCL),  PWM
06 :             PWM
07 : 
08 :
09 :             PWM
10 : SPI (SS),   PWM
11 : SPI (MOSI), PWM
12 : SPI (MISO)
13 : SPI (SCK),  LED

##MMA7455L Pins and information

"Low voltage operation": 2.4V - 3.6V, 2.8V typ.
    Including supply and signal
-> Should connect via a level shifter
    http://playground.arduino.cc/Main/I2CBi-directionalLevelShifter
    would require 2 MOSFETS and 4 resistors; this is acceptable.
-> Could a voltage divider bring the 3.3V down to 2.8?
    Not so simple; those pullup resistors introduce additional factors.
    Should be safe to run other side at 3.3V.

###Registers:
$01 XOUTL
$01 XOUTH
$02 YOUTL
$03 YOUTH
$04 ZOUTL
$05 ZOUTH
$06 XOUT8   Signed 8-bit value (drop the last 2 bits?)
$07 YOUT8
$08 ZOUT8
$09 STATUS  Status registers:
    [0]     DRDY    Data ready
    [1]     DOVR    Data overwritten
    [2]     PERR    Parity error in trim data
    [2:0]   PERR, DOVR, DRDY
$0A DETSRC  Detection Source Register
    [1:0]   INT1, INT2
    [4:2]   PD[X,Y,Z]   [ Pulse detection event ]
    [7:5]   LD[X,Y,Z]   [ Level detection event ]
$0B TOUT    Temperature output value (optional)
$0C         (Reserved)
$0D I2CAD   I2C device address 
    [6:0]   DAD[6:0]
    [7]     I2CDIS
$0E USRINF  User information (Optional)
$0F WHOAMI  “Who am I” value (Optional)
$10 XOFFL   Offset drift X value
$11 XOFFH   Offset drift X value
$12 YOFFL   Offset drift Y value
$13 YOFFH   Offset drift Y value
$14 ZOFFL   Offset drift Z value
$15 ZOFFH   Offset drift Z value
$16 MCTL    Mode control
    [1:0]   MODE
        00 standby      [ use on machine power down ]
        01 measurement  [ use for polling? ]
        10 level detect [ use for motion sensing? NB: 8g only ]
        11 pulse detect 
            !! Modes are cumulative; all features enabled in mode N are available in N+1
    [4:2]   GLVL        [ Sensitivity ]
        00 8g   16 LSB/g
        01 2g   64 LSB/g
        10 4g   32 LSB/g
    [5]     STON        [ self-test mode on ]
    [6]     DRPD        [ Disable sending data-ready status to INT1 pin ]
$17 INTRST  Interrupt latch reset
    [0]     CLR_INT1
    [1]     CLR_INT2
        !!! NB: Should set to clear, then set to 0 again to enable next detection.
$18 CTL1    Control 1
    [0]     INTPIN
        0: INT1/INT2 in Detection Source Register map to INT1 and INT2 pins !!! Desired configuration
        1: INT1/INT2 in Detection Source Register cross to INT1 and INT2 pins
    [2:1]   INTREG[1:0]   
        00: INT1 for level, INT2 for pulse      !!! Desired configuration
        01: INT1 for pusle, INT2 for level
        10: INT1 for single pulse, INT2 for single or double pulse
    [3]     XDA         [ Disable X detection ]
    [4]     YDA         [ Disable Y detection ]
    [5]     ZDA         [ Disable Z detection ]
    [6]     THOPT       [ Absolute threshold (0), or positive/negative (1) ]
    [7]     DFBW        [ Digital filter bandwidth: 62.5 Hz or 125 Hz]
$19 CTL2    Control 2
    [0]     LDPL        [ Level detection polarity ]
    [1]     PDPL        [ Pulse detection polarity ]
    [2]     DVRO        [ Standard / strong drive strength on SDA/SDO pin ]
        !!! LDPL:THOPT  =>  Trigger condition
            00          =>  |X|  or |Y|  or |Z| > LDTH 
            01          =>   X   or  Y   or  Z  > LDTH
            10          =>  |X| and |Y| and |Z| < LDTH
            11          =>   X  and  Y  and  Z  < LDTH
$1A LDTH    Level detection threshold limit value
$1B PDTH    Pulse detection threshold limit value
$1C PW      Pulse duration value
$1D LT      Latency time value
$1E TW      Time window for 2nd pulse value
$1F         (Reserved)


##System hardware setup
CS  --- 3.3V    [ to enable I2C communication ]
SCL <-> SCL
SDA <-> SDA
GND --- GND
VCC --- 3.3V
INT1<-> IO2     [ sensor 0 ]
INT1<-> IO3     [ sensor 1 ]
====================

