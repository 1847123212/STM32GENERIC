#ifndef __WIRESOFT_H
#define __WIRESOFT_H
//IIC software simulate driver by huaweiwx@sina.com 2017.6.28
# ifdef __cplusplus

typedef struct {
    uint8_t sdapin;
	uint8_t sclpin;
} TWOWIRESOFT_TypeDef;

class TwoWireSoft {
 public:
    TwoWireSoft();
    TwoWireSoft(uint8_t scl, uint8_t  sda);
	void setSDA(uint8_t sda),
		setSCL(uint8_t scl),
		begin(void),                //��ʼ��IIC��IO��				 
		start(void),
		stop(void),	  			//����IICֹͣ�ź�
		sendByte(uint8_t txd),			//IIC����һ���ֽ�
		ack(void),					//IIC����ACK�ź�
		nAck(void),				//IIC������ACK�ź�
		writeOneByte(uint8_t daddr,uint8_t addr,uint8_t data);
	uint8_t readByte(unsigned char ack), //IIC��ȡһ���ֽ�
		waitAck(void), 				//IIC�ȴ�ACK�ź�
		readOneByte(uint8_t daddr,uint8_t addr);
	TWOWIRESOFT_TypeDef *pdev = &sdev;	
 private:
	TWOWIRESOFT_TypeDef sdev;
};
# endif //__cplusplus
#endif
















