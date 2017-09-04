//for stm32 gpio bitband io opration
//2017.5.28 for all stm32 chips
//modify by huawei<huaweiwx@sina.com> 2016.9.18

#ifndef __LIKE51_H__
#define __LIKE51_H__

//bitband io opration,ʵ��51���Ƶ�GPIO���ƹ���
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BITBAND_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO address �ڵ�ַӳ��
#if defined(STM32F1)
	#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
	#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
	#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
	#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
	#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
	#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
	#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

	#define GPIOA_IDR_Addr     (GPIOA_BASE+8) //0x40010808 
	#define GPIOB_IDR_Addr     (GPIOB_BASE+8) //0x40010C08 
	#define GPIOC_IDR_Addr     (GPIOC_BASE+8) //0x40011008 
	#define GPIOD_IDR_Addr     (GPIOD_BASE+8) //0x40011408 
	#define GPIOE_IDR_Addr     (GPIOE_BASE+8) //0x40011808 
	#define GPIOF_IDR_Addr     (GPIOF_BASE+8) //0x40011A08 
	#define GPIOG_IDR_Addr     (GPIOG_BASE+8) //0x40011E08 
	
#elif defined(STM32F0)||defined(STM32F2)||defined(STM32F3)||defined(STM32F4)||defined(STM32F7)||\
    defined(STM32L0)||defined(STM32L1)||defined(STM32L4)
	#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
	#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
	#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
	#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
	#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
	#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
	#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
	#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
	#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

	#define GPIOA_IDR_Addr     (GPIOA_BASE+16) //0x40020010 
	#define GPIOB_IDR_Addr     (GPIOB_BASE+16) //0x40020410 
	#define GPIOC_IDR_Addr     (GPIOC_BASE+16) //0x40020810 
	#define GPIOD_IDR_Addr     (GPIOD_BASE+16) //0x40020C10 
	#define GPIOE_IDR_Addr     (GPIOE_BASE+16) //0x40021010 
	#define GPIOF_IDR_Addr     (GPIOF_BASE+16) //0x40021410 
	#define GPIOG_IDR_Addr     (GPIOG_BASE+16) //0x40021810 
	#define GPIOH_IDR_Addr     (GPIOH_BASE+16) //0x40021C10 
	#define GPIOI_IDR_Addr     (GPIOI_BASE+16) //0x40022010
#else
	#error  "for STM32F1/F4/F7"	
#endif
  
//n must be less 16!   ȷ��n��ֵС��16!
//example:  in = PAin(0);  
//          PCout(13) = 1;
    
#define PAout(n)   BITBAND_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BITBAND_ADDR(GPIOA_IDR_Addr,n)  //���� 
#define PORTA      GPIOA->ODR    //��� 
#define PINA       GPIOA->IDR    //���� 

#define PBout(n)   BITBAND_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BITBAND_ADDR(GPIOB_IDR_Addr,n)  //���� 
#define PORTB      GPIOB->ODR    //��� 
#define PINB       GPIOB->IDR    //���� 

#define PCout(n)   BITBAND_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BITBAND_ADDR(GPIOC_IDR_Addr,n)  //���� 
#define PORTC      GPIOC->ODR                  //��� 
#define PINC       GPIOC->IDR                  //���� 

#define PDout(n)   BITBAND_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BITBAND_ADDR(GPIOD_IDR_Addr,n)  //���� 
#define PORTD      GPIOD->ODR    //��� 
#define PIND       GPIOD->IDR    //���� 

#define PEout(n)   BITBAND_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BITBAND_ADDR(GPIOE_IDR_Addr,n)  //����
#define PORTE      GPIOE->ODR    //��� 
#define PINE       GPIOE->IDR    //���� 

#define PFout(n)   BITBAND_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BITBAND_ADDR(GPIOF_IDR_Addr,n)  //����
#define PORTF      GPIOF->ODR    //��� 
#define PINF       GPIOF->IDR    //���� 

#define PGout(n)   BITBAND_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BITBAND_ADDR(GPIOG_IDR_Addr,n)  //����
#define PORTG      GPIOG->ODR    //��� 
#define PING       GPIOG->IDR    //���� 

#define PHout(n)   BITBAND_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BITBAND_ADDR(GPIOH_IDR_Addr,n)  //����
#define PORTH      GPIOH->ODR    //��� 
#define PINH       GPIOH->IDR    //���� 

#define PIout(n)   BITBAND_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BITBAND_ADDR(GPIOI_IDR_Addr,n)  //����
#define PORTI      GPIOI->ODR    //��� 
#define PINI       GPIOI->ODR    //���� 

#endif











