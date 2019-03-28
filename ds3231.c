#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>

//regaddr,seconds,minutes,hours,weekdays,days,months,yeas
char buf[]={0x00,0x00,0x43,0x15,0x05,0x01,0x03,0x19};
char  *str[]  ={"SUN","Mon","Tues","Wed","Thur","Fri","Sat"};

void pcf8563SetTime(char * pybuf)
{
    buf[0] = 0;
    buf[1] = (pybuf[0]-0x30)*16 + (pybuf[1]-0x30);
    buf[2] = (pybuf[2]-0x30)*16 + (pybuf[3]-0x30);
    buf[3] = (pybuf[4]-0x30)*16 + (pybuf[5]-0x30);
    buf[4] = (pybuf[6]-0x30);
    buf[5] = (pybuf[7]-0x30)*16 + (pybuf[8]-0x30);
    buf[6] = (pybuf[9]-0x30)*16 + (pybuf[10]-0x30);
    buf[7] = (pybuf[11]-0x30)*16 + (pybuf[12]-0x30);
    bcm2835_i2c_write(buf,8);
}

void pcf8563ReadTime(char * pybuf)
{
    buf[0] = 0;
    bcm2835_i2c_write_read_rs(buf ,1, buf,7);

    pybuf[2] = (buf[6]/16) + 0x30;	//year
    pybuf[3] = (buf[6]%16) + 0x30;	//year
    pybuf[4] = '-';
    pybuf[5] = (buf[5]&0x1F)/16+0x30; //mouth
    pybuf[6] = (buf[5]&0x1F)%16+0x30; //mouth
    pybuf[7] = '-';
    pybuf[8] = (buf[4]&0x3F)/16+0x30; //day
    pybuf[9] = (buf[4]&0x3F)%16+0x30; //day
    pybuf[10] = ' ';
    pybuf[11] = (buf[2]&0x3F)/16+0x30; //hour
    pybuf[12] = (buf[2]&0x3F)%16+0x30; //hour
    pybuf[13] = ':';
    pybuf[14] = (buf[1]&0x7F)/16+0x30; //min
    pybuf[15] = (buf[1]&0x7F)%16+0x30; //min
    pybuf[16] = ':';
    pybuf[17] = (buf[0]&0x7F)/16+0x30; //sec
    pybuf[18] = (buf[0]&0x7F)%16+0x30; //sec
}

uint8_t ds3231_init()
{
    if (!bcm2835_i2c_begin(2))
    {
        return 1;
    }
    bcm2835_i2c_setSlaveAddress(0x68);
    bcm2835_i2c_set_baudrate(10000);
    return 0;
}

void ds3231_over()
{
    bcm2835_delay(1000);
    bcm2835_i2c_end(2);
}


