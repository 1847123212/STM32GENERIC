#include <Arduino.h>

//user board &spiflashconfig file
#if __has_include("bsp.h")
#  include "bsp.h"
#endif

#if __has_include("spiFlashConfig.h")
#  include "spiFlashConfig.h"
#endif

#include "w25qxx.h" 
#include <SPI.h>

//user unconfig spiflash use variant.h define
#ifndef  SPIFLASH_MOSI
# define SPIFLASH_MOSI	MOSI
#endif
#ifndef	SPIFLASH_MISO
#define SPIFLASH_MISO	MISO
#endif
#ifndef	SPIFLASH_SCK
#define SPIFLASH_SCK	SCK
#endif
#ifndef	SPIFLASH_CS
#define SPIFLASH_CS		SS
#endif

 
W25QXX::W25QXX()
       :SPIClass(SPIFLASH_MOSI,SPIFLASH_MISO, SPIFLASH_SCK){};
W25QXX::W25QXX(SPI_TypeDef *instance)
       :SPIClass(instance){};
W25QXX::W25QXX(SPI_TypeDef *instance, uint8_t mosi, uint8_t miso, uint8_t sck)
	   :SPIClass(instance, mosi, miso, sck){};
W25QXX::W25QXX(uint8_t mosi, uint8_t miso, uint8_t sck)
	   :SPIClass(mosi, miso, sck){};

void W25QXX::begin(uint8_t cs)
{
  if (cs == 0xff)
    pdata->cs_pin = SPIFLASH_CS;
  else
    pdata->cs_pin = cs;

  pinMode(pdata->cs_pin,OUTPUT);
  digitalWrite(pdata->cs_pin,HIGH);			//SPI FLASH��ѡ��
  SPIClass::begin();	//��ʼ��SPI
  SPIClass::beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
//  SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//����Ϊ42Mʱ��,����ģʽ 
  pdata->type = this->readID();	//��ȡFLASH ID.
}  

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t W25QXX::readSR(void)   
{  
	uint8_t byte=0;   
	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
	SPIClass::transfer(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPIClass::transfer(0Xff);             //��ȡһ���ֽ�  
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX::writeSR(uint8_t sr)   
{   
	digitalWrite(pdata->cs_pin,LOW);                               //ʹ������   
	SPIClass::transfer(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPIClass::transfer(sr);                    //д��һ���ֽ�  
	digitalWrite(pdata->cs_pin,HIGH);                              //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX::writeEnable(void)   
{
	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_WriteEnable);      //����дʹ��  
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX::writeDisable(void)   
{  
	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_WriteDisable);     //����д��ָֹ��    
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     	      
} 
		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
uint16_t W25QXX::readID(void)
{
	uint32_t temp = 0;	  
	digitalWrite(pdata->cs_pin,LOW);				    
	SPIClass::transfer(0x90);//���Ͷ�ȡID����	    
	SPIClass::transfer(0x00); 	    
	SPIClass::transfer(0x00); 	    
	temp |= SPIClass::transfer(W25X_DummyByte)<< 8;  
	temp |= SPIClass::transfer(W25X_DummyByte);
	digitalWrite(pdata->cs_pin,HIGH);				    
	return temp;
}

uint32_t W25QXX::jedecDeviceID(void)
{
	uint32_t temp = 0;	  
	digitalWrite(pdata->cs_pin,LOW);				    
	SPIClass::transfer(0x9F);//���Ͷ�ȡID����	    
	temp |=SPIClass::transfer(W25X_DummyByte)<<24; 	    
	temp |=SPIClass::transfer(W25X_DummyByte)<<16; 	    
	temp |= SPIClass::transfer(W25X_DummyByte)<< 8;  
	temp |= SPIClass::transfer(W25X_DummyByte);
	digitalWrite(pdata->cs_pin,HIGH);				    
	return temp;
}   		    
   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX::read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_ReadData);         //���Ͷ�ȡ����   
    SPIClass::transfer((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
    SPIClass::transfer((uint8_t)((ReadAddr)>>8));   
    SPIClass::transfer((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIClass::transfer(0XFF);   //ѭ������  
    }
	digitalWrite(pdata->cs_pin,HIGH);  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX::writePage(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    this->writeEnable();                  //SET WEL 
	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_PageProgram);      //����дҳ����   
    SPIClass::transfer((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
    SPIClass::transfer((uint8_t)((WriteAddr)>>8));   
    SPIClass::transfer((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPIClass::transfer(pBuffer[i]);//ѭ��д��  
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ 
	this->waitBusy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX::writeNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		this->writePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
	 
void W25QXX::write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
   	W25QXX_BUF = pdata->aBuf;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		this->read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			this->eraseSector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			this->writeNoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  
		}else
			this->writeNoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   

		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX::eraseChip(void)   
{                                   
    this->writeEnable();                  //SET WEL 
    this->waitBusy();   
  	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_ChipErase);        //����Ƭ��������  
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     	      
	this->waitBusy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void W25QXX::eraseSector(uint32_t Dst_Addr)   
{  
	  
// 	printf("fe:%x\r\n",Dst_Addr); //����falsh�������,������  
 	Dst_Addr*=4096;
    this->writeEnable();                  //SET WEL 	 
    this->waitBusy();   
  	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_SectorErase);      //������������ָ�� 
    SPIClass::transfer((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
    SPIClass::transfer((uint8_t)((Dst_Addr)>>8));   
    SPIClass::transfer((uint8_t)Dst_Addr);  
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     	      
    this->waitBusy();   				   //�ȴ��������
}  
//�ȴ�����
inline void W25QXX::waitBusy(void)   
{   
	while((this->readSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void W25QXX::powerDown(void)   
{ 
  	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_PowerDown);        //���͵�������  
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     	      
//    delayMicroseconds(3);                               //�ȴ�TRES1
    delay(3);
}   
//����
void W25QXX::WAKEUP(void)   
{  
  	digitalWrite(pdata->cs_pin,LOW);                            //ʹ������   
    SPIClass::transfer(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	digitalWrite(pdata->cs_pin,HIGH);                            //ȡ��Ƭѡ     	      
//    delayMicroseconds(3);                               //�ȴ�TRES1
    delay(3);
}   

























