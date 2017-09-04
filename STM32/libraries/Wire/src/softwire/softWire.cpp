#include <Arduino.h>
#include "softWire.h"

//////////////////////////////////////////////////////////////////////////////////	 
//IIC software simulate driver by huaweiwx@sina.com 2017.6.28	   
////////////////////////////////////////////////////////////////////////////////// 

TwoWireSoft::TwoWireSoft(){
}

TwoWireSoft::TwoWireSoft(uint8_t scl,uint8_t sda){
		pdev>sdapin = sda;
		pdev>sclpin = scl;
}

void TwoWireSoft::setSDA(uint8_t sda){
		pdev>sdapin = sda;
}

void TwoWireSoft::setSCL(uint8_t scl){
		pdev>sclpin = scl;
}


//��ʼ��IIC
void TwoWireSoft::begin(void)
{			
  pinMode(pdev>sdapin,OUTPUT);
  pinMode(pdev>sclpin,OUTPUT);
  digitalWrite(pdev>sclpin,HIGH);
  digitalWrite(pdev>sdapin,HIGH);
}

//����IIC��ʼ�ź�
void TwoWireSoft::start(void)
{
	pinMode(pdev>sdapin,OUTPUT);     //sda�����
	digitalWrite(pdev>sdapin,HIGH);	  	  
	digitalWrite(pdev>sclpin,HIGH);
	delayMicroseconds(4);
 	digitalWrite(pdev>sdapin,LOW);//START:when CLK is high,DATA change form high to low 
	delayMicroseconds(4);
	digitalWrite(pdev>sclpin,LOW);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void TwoWireSoft::stop(void)
{
	pinMode(pdev>sdapin,OUTPUT);//sda�����
	digitalWrite(pdev>sclpin,LOW);
	digitalWrite(pdev>sdapin,LOW);//STOP:when CLK is high DATA change form low to high
 	delayMicroseconds(4);
	digitalWrite(pdev>sclpin,HIGH); 
	digitalWrite(pdev>sdapin,HIGH);//����I2C���߽����ź�
	delayMicroseconds(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t TwoWireSoft::waitAck(void)
{
	uint8_t ucErrTime=0;
	pinMode(pdev>sdapin,INPUT);      //SDA����Ϊ����  
	digitalWrite(pdev>sdapin,HIGH);delayMicroseconds(1);	   
	digitalWrite(pdev>sclpin,HIGH);delayMicroseconds(1);	 
	while(digitalRead(pdev>sdapin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			this->stop();
			return 1;
		}
	}
	digitalWrite(pdev>sclpin,LOW);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void TwoWireSoft::ack(void)
{
	digitalWrite(pdev>sclpin,LOW);
	pinMode(pdev>sdapin,OUTPUT);
	digitalWrite(pdev>sdapin,LOW);
	delayMicroseconds(2);
	digitalWrite(pdev>sclpin,HIGH);
	delayMicroseconds(2);
	digitalWrite(pdev>sclpin,LOW);
}
//������ACKӦ��		    
void TwoWireSoft::nAck(void)
{
	digitalWrite(pdev>sclpin,LOW);
	pinMode(pdev>sdapin,OUTPUT);
	digitalWrite(pdev>sdapin,HIGH);
	delayMicroseconds(2);
	digitalWrite(pdev>sclpin,HIGH);
	delayMicroseconds(2);
	digitalWrite(pdev>sclpin,LOW);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void TwoWireSoft::sendByte(uint8_t txd)
{                        
    uint8_t t;   
	pinMode(pdev>sdapin,OUTPUT); 	    
    digitalWrite(pdev>sclpin,LOW);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        digitalWrite(pdev>sdapin,(txd&0x80)>>7);
        txd<<=1; 	  
		delayMicroseconds(2);   //��TEA5767��������ʱ���Ǳ����
		digitalWrite(pdev>sclpin,HIGH);
		delayMicroseconds(2); 
		digitalWrite(pdev>sclpin,LOW);	
		delayMicroseconds(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t TwoWireSoft::readByte(unsigned char ack)
{
	unsigned char i,receive=0;
	pinMode(pdev>sdapin,INPUT);//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        digitalWrite(pdev>sclpin,LOW); 
        delayMicroseconds(2);
		digitalWrite(pdev>sclpin,HIGH);
        receive<<=1;
        if(digitalRead(pdev>sdapin))receive++;   
		delayMicroseconds(1); 
    }					 
    if (!ack)
        this->nAck();//����nACK
    else
        this->ack(); //����ACK   
    return receive;
}



























