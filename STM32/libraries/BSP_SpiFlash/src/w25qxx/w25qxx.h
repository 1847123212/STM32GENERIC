#ifndef __W25QXX_H__
#define __W25QXX_H__

#include <SPI.h>
//W25Xϵ��/Qϵ��оƬ�б�	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define M25P128	0x202018
#define M25P64	0x202017
#define SST25VF032 0xBF4A
#define AT45DB161  0x1F2600

////////////////////////////////////////////////////////////////////////////////// 
//command list ָ���
#define W25X_WriteStatusReg		0x01 
#define W25X_PageProgram		0x02 
#define W25X_ReadData			0x03 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteEnable		0x06 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9
#define W25X_DummyByte          0xA5 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F
 
#ifdef __cplusplus

#define  W25QXX_BUFFERSIZE 4096
typedef struct {
    uint8_t cs_pin;
    uint16_t type;  //����W25QXXоƬ�ͺ�
	uint8_t  aBuf[W25QXX_BUFFERSIZE];	
} W25QXX_TypeDef;


class W25QXX : public SPIClass 
{
  public:
	W25QXX();
    W25QXX(uint8_t mosi, uint8_t miso, uint8_t sck);
	W25QXX(SPI_TypeDef *instance);
    W25QXX(SPI_TypeDef *instance, uint8_t mosi, uint8_t miso, uint8_t sck);
    void begin(uint8_t cs = 0xff);
	uint16_t readID(void);  	    		//��ȡFLASH ID
	uint32_t jedecDeviceID(void);
	uint8_t	 readSR(void);        		//��ȡ״̬�Ĵ��� 
    void writeSR(uint8_t sr);  			//д״̬�Ĵ���
    void writeEnable(void);  		//дʹ�� 
    void writeDisable(void);		//д����
    void writeNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
    void read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //��ȡflash
    void write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//д��flash
    void eraseChip(void);    	  	//��Ƭ����
    void eraseSector(uint32_t Dst_Addr);	//��������
    void waitBusy(void);           	//�ȴ�����
    void powerDown(void);        	//�������ģʽ
    void WAKEUP(void);				//����
	W25QXX_TypeDef *pdata =&sdata;
  private:   
	void writePage(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
	W25QXX_TypeDef sdata;
};

#endif  //__cplusplus
#endif  //__W25QXX_H















