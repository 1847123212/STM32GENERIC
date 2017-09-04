#ifndef __AT24CXX_H
#define __AT24CXX_H

#include <Wire.h>

//////////////////////////////////////////////////////////////////////////////////	 
 
//24CXX ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767
#define AT24C512	65535
  
#ifdef __cplusplus

typedef struct {
	uint16_t dev;
	uint8_t  devAdr;
} AT24CXX_TypeDef;


class AT24XX:public TwoWireSoft 
{
 public:
    AT24XX(uint16_t _dev = AT24C02);
	AT24XX(uint8_t scl, uint8_t sda);
	void setSDA(uint8_t sda),
		setSCL(uint8_t scl),
		writeOneByte(uint16_t WriteAddr,uint8_t DataToWrite),		
		writeLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len),
		write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite),	
		read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead); 
    uint8_t begin(uint8_t _devAdr=0x50), //��ʼ��IIC	
		begin(int _devAdr), //��ʼ��IIC
		check(void),  //�������
		readOneByte(uint16_t ReadAddr);
	uint32_t readLenByte(uint16_t ReadAddr,uint8_t Len);
	AT24CXX_TypeDef *pdata =&sdata;
 private:
    AT24CXX_TypeDef sdata;
}; 

#endif //__cplusplus
#endif
















