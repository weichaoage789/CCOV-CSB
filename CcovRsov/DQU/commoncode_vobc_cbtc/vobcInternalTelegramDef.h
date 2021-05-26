
/************************************************************************
* 文件名	： vobcInternalTelegramDef.h
* 版权说明	：	
* 版本号	： 1.3 
* 创建时间	： 2009.9.9
* 作者		： 车载与协议部	
* 功能描述	： VOBC内部所有的内网报文类型定义 
* 使用注意	： 
* 修改记录	： 
	2009-09-28 刘宏杰 V1.1
		将ATP应用故障记录数据的宏定义由原来的VOBC_1_ATP_ERR_TO_LOGGER修改为VOBC_ATP_ERR_TO_LOGGER
    2013-11-26 xisheng.xia V1.2
		1)增加一帧ATP记录数据,用于记录速度相关信息(0x4E,0x4F);
        2)增加冗余ATO相关帧(复用TMS,不做修改);
        3)增加与MSS直接通信相关帧(0x0E,0x0F);
    2013-11-29 xxs zl V1.3
        1)增加一帧ATP记录数据,用于记录速度相关信息(0x5F);
        2)增加冗余ATO,ATP到ATO2的信息帧(复用TMS,因当前硬件情况下,ATP不可能同时收到TMS、ATO2信息,因此可复用);
    2013-11-29 zl zyy sjq V1.4
        1)增加一帧两端用于周期中的测速信息通信帧(0x39);
        2)增加冗余ATO两机都需要进行运行、报警报文的记录,增加ATO2的运行、报文信息帧类型(0x5E、0x5D);
    2013-11-29 zl wjl bf  V1.5
        1)增加冗余ATO下平台的记录报文信息帧类型(0x5B、0x5C);
	2015-03-05 wwp  V1.6
		1)增加一帧VOBC到AOM1、AOM2的数据信息帧类型
		2)增加一帧从AOM到VOBC的数据信息帧类型
************************************************************************/
#ifndef _VOBC_INTERNAL_TELEGRAM_DEF_H
#define _VOBC_INTERNAL_TELEGRAM_DEF_H


/*外网数据是特指VOBC使用我们自己的安全协议时，对外发出的数据类型，只可能有ZC1,ZC2,DSU三种数据，本来可以定为1种外网数据，但是考虑到主机若把这些数据都叫外网数据，则通信板对三机的数据进行表决时就要比较整个报文，而把他们分开成不同的种类后，通信板只需要查看报文类型即可方便进行表决。因为中间各通信环节不需要知道报文是发送给ZC还是DSU的，故只命名为外网数据1，2，3*/

  /* 外网数据1 */
#define     VOBC_1_TO_OUT_NET       0x01

  /* 外网数据2 */
#define     VOBC_2_TO_OUT_NET       0x02

  /* 外网数据3 */
#define     VOBC_3_TO_OUT_NET       0x03


/*以下数据也是VOBC发送给外网的数据。
给ATS的运行数据、故障报警数据，每周期只发送一帧，这些数据都是发向ATS网关计算机的同一个端口，考虑到第一次上电时，发送给ATS的数据需要经过冗余层处理，冗余层序列号为0时，将发送3个0报文，故这类报文可能有5种。
VOBC发送给ATS的NTP校时数据也发到了ATS网关计算机，但是目的端口不一样，故也对报文类型进行了区分，这种数据，不需要经过冗余层，故不需要增加3种0报文类型。
VOBC发送给CI的数据每周期发送1帧，也需要经过冗余层处理，同理需要考虑增加3个0报文类型*/

  /* VOBC发给ATS */
#define     VOBC_TO_ATS1            0x04
#define     VOBC_TO_ATS2            0x05
#define     VOBC_TO_ATS3            0x06
#define     VOBC_TO_ATS4            0x07
#define     VOBC_TO_ATS5            0x08

  /* VOBC发给ATS的NTP，红蓝网都发 */
#define     VOBC_TO_ATS_NTP         0x09

  /* VOBC发给CI */
#define     VOBC_TO_CI1             0x0A
#define     VOBC_TO_CI2             0x0B
#define     VOBC_TO_CI3             0x0C
#define     VOBC_TO_CI4             0x0D


#define     VOBC_TO_MSS1            0x0E
#define     VOBC_TO_MSS2            0x0F

/*VOBC发送给AOM,红蓝网都发*/
#define     VOBC_TO_AOM1            0x10
#define     VOBC_TO_AOM2            0x11

/* ATP发给COMM的数据类型 ,全自动下载新增,黄成,20180611*/
#define     VOBC_TO_COMM_DOWNLOAD   0x12
#define     ATO_TO_TRDP             0x1A
#define     TRDP_TO_ATO             0x1B
#define     TRDP_TO_LOGGER          0x1C

/*VOBC发送给PSC的数据类型帧*/
#define   	VOBC_TO_PSC				0x1D
#define		VOBC_TO_PSC2			0x1E

/*以下是来自ATS和CI的数据，从通信控制器层面就区分开这些数据类型后便于主机平台将这些不同的数据填写到应用程序相应的缓冲区中。
ATS发送给VOBC的NTP数据无须进行冗余层的处理，所以和正常的ATS数据要区分开。
CI发送给VOBC的数据需要使用RSSP协议，所以也需要和ATS的数据区分开 */

  /* ATS发给VOBC */
#define     VOBC_FROM_ATS           0x20

  /* ATS发给VOBC的NTP */
#define     VOBC_FROM_ATS_NTP       0x21

  /* CI发给VOBC */
#define     VOBC_FROM_CI            0x22


/* 来自外网的使用我们自己的安全通信协议的ZC1,2及DSU的数据都被叫做来自外网的数据，因为通信控制器将此数据发送给通信板，通信板再发送给主机时不需要进行表决，即使数据类型相同，对于主机来讲也会正常接收，这类数据被主机接收完毕后，都将放在指定的缓冲区中，不会和其它的来自外网的数据混淆 */
 
/*来自外网的使用我们自己的安全通信协议的ZC1,2及DSU的数据*/
#define     VOBC_FROM_OUT_NET       0x23

/* AOM发给VOBC */
#define     VOBC_FROM_AOM           0x24

/* PSC发送给VOBC */
#define		VOBC_FROM_PSC			0x25

/*以下数据无须区分是接收还是发送，因为这些数据在中间环节传输时，接收和发送的物理通道都不同，中间环节可以知道哪个通道是接收，哪个通道是发送。*/

  /* 来往另外一端VOBC的数据 */
#define     VOBC_ANOTHER_VOBC       0x30

  /* 来往ATO的数据，只有红网有此数据 */
#define     ATP_ATO                 0x31

  /* 来往MMI的数据 */
#define     ATP_MMI                 0x32

  /* 来往车辆TMS的数据，只有蓝网有此数据 */
#define     ATP_TMS                 0x33

  /* 来往ATO2的数据(冗余)，只有蓝网有此数据 */
#define     ATP_ATO2                0x33

  /* 来往红网CCOV(includeing WGB)的数据，用于通信控制器和WGB的诊断 */
#define     ATP_RED_CCOV            0x34

  /* 来往蓝网CCOV(includeing WGB)的数据，用于通信控制器和WGB的诊断 */
#define     ATP_BLUE_CCOV           0x35

  /* 来往记录系统的数据，此数据是为了主机和记录系统间校时使用，暂时主机将年月日等信息放在应用记录数据1中，记录系统用此时间来校正其时间即可*/
#define     ATP_LOGGER              0x36

  /* 来往ATP和车辆段网管计算机间红网无线测试的数据，用于库内无线测试*/
#define     ATP_RED_RADIO_TEST      0x37

  /* 来往ATP和车辆段网管计算机间蓝网无线测试的数据，用于库内无线测试*/
#define     ATP_BLUE_RADIO_TEST     0x38

  /* 来往另外一端VOBC的数据(周期中给测速用数据) */
#define     VOBC_ANOTHER_VOBC_SDU   0x39

/*以下这些数据都是各子系统发送给记录系统的记录数据。记录系统还需要把某些数据转发给便携式测试仪。
ATP及ATO的记录数据有许多是运行数据的记录，数据量较大，不需要发送给便携式测试仪，但是ATP及ATO的故障报警信息（含ATP，ATO设备的软件版本号，设备故障代码及运行过程中的故障及轮径、车组号、1，2短，雷达校正系数、ATO输出的牵引制动值）需要每周期均发送给便携式测试仪，故专门为ATP及ATO设计了故障记录数据。ATO，ATP的运行记录数据不需要发送给便携式测试仪。
其余的记录数据都含有故障报警信息或少量的运行记录数据以及软件版本号，都需要发送给便携式测试仪，以便在线显示各种详细的故障信息和各软件的版本号。因为记录系统和便携式测试仪之间使用的是串口，传输的数据有限，这些数据可以由记录系统特殊处理，每隔1秒或更长时间发送给便携式测试仪。
另外记录系统还需要将来自通信板的MMI数据（含有一些其它的运行数据）发送给便携式测试仪，以便进行室内的仿真测试。
记录系统自身的故障报警数据，需要转发给便携式测试仪，其中需要包含记录系统的软件版本。
对于通信控制器的记录数据有比较特殊的处理，实际上通信控制器现在和记录系统是合在一起的，但是当1端工作时，1端要得知位于2端的通信控制器的WGB的状态，故这些记录数据都是先发到已激活端的通信板，然后再由通信板转发给已激活端的记录系统进行记录。初期调试时，可能需要记录所有的外网数据，如果仍旧采用此方法，通信板的负担太重，故如果是要记录红(蓝)网的外网数据时，则由红(蓝)网通信控制器通过消息队列或其它形式直接发送给红(蓝)网记录软件，即红蓝网的外网数据都在本端记录系统上进行记录。正式运行时，所有的外网数据都是由地面APM记录的，车上原则上不对外网数据进行记录，只有在确认记录系统的能力能满足要求时，我们才可能将所有的红蓝网数据在车载设备的两端都进行记录，以便和地面APM的数据进行比较*/

  /* 红网CCOV(including WGB)记录数据 */
#define VOBC_RED_CCOV_TO_LOGGER     0x40

  /* 蓝网CCOV(including WGB)记录数据 */
#define VOBC_BLUE_CCOV_TO_LOGGER    0x41

  /*通信板1记录数据1 */
#define VOBC_COM_1_TO_LOGGER1        0x42

  /* 通信板2记录数据 1*/
#define VOBC_COM_2_TO_LOGGER1        0x43

  /* MMI记录数据 */
#define VOBC_MMI_TO_LOGGER          0x44

  /* ATO故障记录数据 */
#define VOBC_ATO_ERR_TO_LOGGER      0x45

  /* ATO运行记录数据 */
#define VOBC_ATO_TO_LOGGER          0x46

  /* ATP应用故障记录数据 */
#define VOBC_ATP_ERR_TO_LOGGER    	0x47

  /* ATP应用记录数据1 */
#define VOBC_1_ATP_TO_LOGGER        0x48

  /* ATP应用记录数据2 */
#define VOBC_2_ATP_TO_LOGGER        0x49

  /* A机平台记录数据 */
#define VOBC_TMR_A_TO_LOGGER        0x4A

  /* B机平台记录数据 */
#define VOBC_TMR_B_TO_LOGGER        0x4B

  /* C机平台记录数据 */
#define VOBC_TMR_C_TO_LOGGER        0x4C

  /* 记录系统自身的记录数据，需要转发给便携式测试仪。*/
#define LOGGER_TO_TESTER            0x4D

  /*ATP测速相关信息记录数据帧,设计为不表决,仅A机向通信板发送*/
#define VOBC_SDU_A_TO_LOGGER        0x4E

  /*ATP测速相关信息记录数据帧,预留B机*/
#define VOBC_SDU_B_TO_LOGGER        0x4F

  /*ATP测速相关信息记录数据帧,预留B机*/
#define VOBC_SDU_C_TO_LOGGER        0x5F

  /*ATO2运行记录数据*/
#define VOBC_ATO2_TO_LOGGER			0x5E

  /*ATO2故障记录数据*/
#define VOBC_ATO2_ERR_TO_LOGGER     0x5D

  /*ATO1的冗余平台记录数据*/
#define VOBC_ATORPM1_TO_LOGGER      0x5C

  /*ATO2的冗余平台记录数据*/
#define VOBC_ATORPM2_TO_LOGGER      0x5B



/*便携式测试仪也可以更新ATP的数据，但是这个更新是单向的，只是便携式测试仪向ATP发送数据。*/

  /* 便携式测试仪更新ATP参数 */
#define VOBC_ATP_PARAMETER_UPDATE   0x50


/*以下数据类型用于主机和通信板之间*/

  /* TMR命令数据，用于 启动 通信板->主机的数据发送 */
#define TMR_CMD_TO_COMAB_START_SEND 0x60

  /* TMR命令数据，用于 停止 通信板->主机的数据发送 */
#define TMR_CMD_TO_COMAB_STOP_SEND  0x61


/*以下数据类型用于上电时BOOTLOADER软件对各系统进行的软件和数据更新*/

  /* 主机A软件更新的来往数据 */
#define VOBC_ATP_A_SW_UPDATE        0x70

  /* 主机B软件更新的来往数据 */
#define VOBC_ATP_B_SW_UPDATE        0x71

  /* 主机C软件更新的来往数据 */
#define VOBC_ATP_C_SW_UPDATE        0x72

  /* 通信机A软件更新的来往数据 */
#define VOBC_COMA_SW_UPDATE         0x73

  /* 通信机B软件更新的来往数据 */
#define VOBC_COMB_SW_UPDATE         0x74

  /* 通信控制器软件更新的来往数据 */
#define VOBC_CCOV_SW_UPDATE         0x75

  /* 记录系统软件更新的来往数据 */
#define VOBC_LOGGER_SW_UPDATE       0x76

  /* MMI软件更新的来往数据 */
#define VOBC_MMI_SW_UPDATE          0x77

  /* ATO软件更新的来往数据 */
#define VOBC_ATO_SW_UPDATE          0x78

/* ADD START 20180316 xb 互联互通FAO from LCF-400*/
/*CCOV发给MMI的版本号数据*/
#define VOBC_RED_CCOV_TO_MMI    0x86

/*CCOV发给MMI的版本号数据*/

#define VOBC_BLUE_CCOV_TO_MMI     0x87
/* ADD END 20180316 xb 互联互通FAO from LCF-400*/


/*AOM发给外网的数据*/
#define AOM_TO_OUT_NET				0x88

/*AOM发送给内网的数据*/
#define AOM_TO_IN_NET				0x90

/* VOBC软件更新的来往数据 */
#define HOST_TO_VOBC_SW_UPDATE          0x80

/*  VOBC全自动下载更新的来往数据,20190419 by sds  */
#define VOBC_TO_HOST_SW_UPDATE          0x81

/*以下数据类型为RSOV发给通信板的数据更新报文,20150724增加*/
#define RSOV_DATA_UPDATE_CMD    0x80
/*以下数据类型为通信板反馈给RSOV的数据更新成功类型,20150724增加*/
#define RSOV_DATA_UPDATE_SUCCEED    0x81
/*以下数据类型为通信板反馈给RSOV的数据更新失败类型,20150724增加*/
#define RSOV_DATA_UPDATE_FAIL    0x82

/* ADD START 20180316 xb 互联互通FAO from LCF-400*/
/* ATP应用记录数据3 */
#define VOBC_3_ATP_TO_LOGGER		0x51

/* ATO MA记录数据 */
#define VOBC_ATO_MA_TO_LOGGER          0x52

/* ATO2 MA记录数据 */
#define VOBC_ATO2_MA_TO_LOGGER          0x53
/* ADD END 20180316 xb 互联互通FAO from LCF-400*/

/*ATO的TMS记录数据*/
#define VOBC_ATO_TMS_TO_LOGGER      0x54

/*20151208新增互联互通下载数据帧类型*/
/*互联互通帧类型区分*/
/*ATP1与CCOV往来数据类型*/
#define CCOV_AND_ATP1_TYPE		0x91
/*ATP2与CCOV往来数据类型*/
#define CCOV_AND_ATP2_TYPE		0x92
/*ATP3与CCOV往来数据类型*/
#define CCOV_AND_ATP3_TYPE		0x93
/*ATO1与CCOV往来数据类型*/
#define CCOV_AND_ATO1_TYPE		0x94
/*ATO2与CCOV往来数据类型*/
#define CCOV_AND_ATO2_TYPE		0x95
/*ATP与NDSU往来数据类型*/
#define NDSU_AND_ATP_TYPE		0x96
/*ATO与NDSU往来数据类型*/
#define NDSU_AND_ATO_TYPE		0x97

/*通信板1记录数据2 */
#define VOBC_COM_1_TO_LOGGER2        0x98

/* 通信板2记录数据 2*/
#define VOBC_COM_2_TO_LOGGER2        0x99


/*20150314新增通信对象标识*/
enum AomAcsbMutualMsgTypeEnum
{
    AOM_RESERVE_TYPE            = 0x00,
    VOBC_TO_AOM_RED_MSG_TYPE    = 0x01, /*VOBC至AOM红网报文*/
    VOBC_TO_AOM_BLU_MSG_TYPE    = 0x02, /*VOBC至AOM蓝网报文*/
    AOM_TO_VOBC_MSG_TYPE        = 0x03, /*AOM至VOBC报文*/
    AOM_TO_AOM_MSG_TYPE         = 0x04, /*AOM至AOM报文*/
    AOM_TO_CSB_MSG_TYPE         = 0x05, /*AOM至CSB报文*/
    AOM_TO_TIAS_MSG_TYPE        = 0x06, /*AOM至TIAS报文*/
    CSB_TO_AOM_MSG_TYPE         = 0x07, /*CSB至AOM报文*/
    TIAS_TO_AOM_BLU_MSG_TYPE    = 0x08, /*TIAS至AOM蓝网报文*/
    TIAS_TO_AOM_RED_MSG_TYPE    = 0x09, /*TIAS至AOM红网报文*/
    AOM_TO_MSS_MSG_TYPE         = 0x0A, /*AOM至MSS报文*/
    MSS_TO_AOM_RED_MSG_TYPE     = 0x0B, /*MSS至AOM红网报文*/
    MSS_TO_AOM_BLU_MSG_TYPE     = 0x0C,  /*MSS至AOM蓝网报文*/
	/* ADD START 20180316 xb 互联互通FAO from LCF-400*/
    AOM1_TO_CSB_LOGGER_MSG_TYPE     = 0x0D, /*AOM1运行记录数据报文*/
    AOM2_TO_CSB_LOGGER_MSG_TYPE     = 0x0E, /*AOM2运行记录数据报文*/
    AOM1RMP_TO_CSB_LOGGER_MSG_TYPE  = 0x0F, /*AOM1的冗余平台记录数据报文*/
    AOM2RMP_TO_CSB_LOGGER_MSG_TYPE  = 0x10, /*AOM2的冗余平台记录数据报文*/
    AOM1_TO_CSB_SFP_RECORD_TYPE     = 0X11, /*AOM SFP记录数据报文*/
	/* ADD END 20180316 xb 互联互通FAO from LCF-400*/
    RES_AOM_AND_CSB_MSG_TYPE
};
#endif
