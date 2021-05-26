/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  COM20020Define.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _COM20020_DEFINE_H
#define _COM20020_DEFINE_H


  /*以下是COM20020的寄存器地址偏移量定义，在PC104系统中，是以字节的方式访问的COM20020， 所以实际
  的物理地址应该是这些地址加上基地址*/
#define 	STATUS_REG			0x00
#define 	DIAG 						0x01
#define 	ADDR_H_PT 			0x02
#define 	ADDR_L_PT 			0x03
#define 	DATA 						0x04
#define 	SUB_ADDR 			0x05
#define 	CONFIG 					0x06
#define 	SUB_DATA				0x07
#define 	IMR 						0x00
#define 	COMMAND 			0x01


#define 	TENTID 		0x00
#define 	NODE_ID 	0x01
#define 	SETUP1 		0x02
#define 	NEXT_ID 	0x03
#define 	SETUP2 		0x04
#define 	TEST_REG	0x03

  /* 允许PAGE0,1接收 */
#define 	ENABLE_PAGE0_RECV   0x84
#define 	ENABLE_PAGE1_RECV   0x8c

#define   	TTA				0x01		/* Command Chaining: Rising Transition on Transmitter Available */
#define   	TRI				0x80		/* Command Chaining: Rising Transition of Receiver Inhibited*/
#define		TMA			0x02		/* transmitter msg ack */

#define     CLEARFLAGS		0x1E		/* Resets the POR and RECON Status Bits */
#define     RESET					0x80		/* set to exec software reset */
#define     DISABLETX			0x01		/* Disable Transmitter */

/* cancel any pending tx command, will set TA to 1 when com20020 next receives the token */
#define     DISABLERX			0x02		/* Disable Receiver. */

#define     LONGPACKET		0x0D		/* Accept Long Packet and Short Packets */
#define     CLEARRECON		0x16		/* Resets the RECON Status Bit */
#define     CLEARPOR			0x0E		/* Resets the POR Status Bit */

#define     TA2						0x20		/* TA if chaining */

/* read/write address pointer high register 2 */
#define     AUTOINC				0x40		/* 1 (autoinc adr ptr), 1 (no autoinc) */
#define     RDDATA				0x80		/* 0 (next access is a write), 1 (read) */
#define     WRDATA				0x7f			/* 0 (next access is a write), 1 (read) */

/* write command register */
#define     CLR_TX_INT		0x00		/* used in command chaining operation */
#define     CLR_RI					0x08		/* clear rx int - used in command chaining op */

#define     RECON					0x04		/* line idle timere time out */
#define     MYRECON			0x80		/* set when lost token timer times out */
#define     EXC_NAK				0x08		/* excessive nak - set if 128 naks have occurred */


#define ARCNET_WAITE_IRQ_CS2		   				_IOW('R', 0x00, int)
#define ARCNET_WRITE_DATA_CS2		    			_IOW('R', 0x01, int)
#define ARCNET_READ_DATA_CS2						_IOW('R', 0x02, int)
#define ARCNET_WAITE_IRQ_CS0		   				_IOW('R', 0x03, int)
#define ARCNET_WRITE_DATA_CS0		    			_IOW('R', 0x04, int)
#define ARCNET_READ_DATA_CS0						_IOW('R', 0x05, int)
#define GET_CS0_ADDR											_IOW('R', 0x06, int)
#define GET_CS2_ADDR											_IOW('R', 0x07, int)
#define ARCNET_WRITE_DATA		    					_IOW('R', 0x08, int)
#define ARCNET_READ_DATA								_IOW('R', 0x09, int)


#endif  /*_COM20020_DEFINE_H*/
