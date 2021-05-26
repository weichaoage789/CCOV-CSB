
/************************************************************************
* �ļ���	�� vobcInternalTelegramDef.h
* ��Ȩ˵��	��	
* �汾��	�� 1.3 
* ����ʱ��	�� 2009.9.9
* ����		�� ������Э�鲿	
* ��������	�� VOBC�ڲ����е������������Ͷ��� 
* ʹ��ע��	�� 
* �޸ļ�¼	�� 
	2009-09-28 ����� V1.1
		��ATPӦ�ù��ϼ�¼���ݵĺ궨����ԭ����VOBC_1_ATP_ERR_TO_LOGGER�޸�ΪVOBC_ATP_ERR_TO_LOGGER
    2013-11-26 xisheng.xia V1.2
		1)����һ֡ATP��¼����,���ڼ�¼�ٶ������Ϣ(0x4E,0x4F);
        2)��������ATO���֡(����TMS,�����޸�);
        3)������MSSֱ��ͨ�����֡(0x0E,0x0F);
    2013-11-29 xxs zl V1.3
        1)����һ֡ATP��¼����,���ڼ�¼�ٶ������Ϣ(0x5F);
        2)��������ATO,ATP��ATO2����Ϣ֡(����TMS,��ǰӲ�������,ATP������ͬʱ�յ�TMS��ATO2��Ϣ,��˿ɸ���);
    2013-11-29 zl zyy sjq V1.4
        1)����һ֡�������������еĲ�����Ϣͨ��֡(0x39);
        2)��������ATO��������Ҫ�������С��������ĵļ�¼,����ATO2�����С�������Ϣ֡����(0x5E��0x5D);
    2013-11-29 zl wjl bf  V1.5
        1)��������ATO��ƽ̨�ļ�¼������Ϣ֡����(0x5B��0x5C);
	2015-03-05 wwp  V1.6
		1)����һ֡VOBC��AOM1��AOM2��������Ϣ֡����
		2)����һ֡��AOM��VOBC��������Ϣ֡����
************************************************************************/
#ifndef _VOBC_INTERNAL_TELEGRAM_DEF_H
#define _VOBC_INTERNAL_TELEGRAM_DEF_H


/*������������ָVOBCʹ�������Լ��İ�ȫЭ��ʱ�����ⷢ�����������ͣ�ֻ������ZC1,ZC2,DSU�������ݣ��������Զ�Ϊ1���������ݣ����ǿ��ǵ�����������Щ���ݶ����������ݣ���ͨ�Ű�����������ݽ��б��ʱ��Ҫ�Ƚ��������ģ��������Ƿֿ��ɲ�ͬ�������ͨ�Ű�ֻ��Ҫ�鿴�������ͼ��ɷ�����б������Ϊ�м��ͨ�Ż��ڲ���Ҫ֪�������Ƿ��͸�ZC����DSU�ģ���ֻ����Ϊ��������1��2��3*/

  /* ��������1 */
#define     VOBC_1_TO_OUT_NET       0x01

  /* ��������2 */
#define     VOBC_2_TO_OUT_NET       0x02

  /* ��������3 */
#define     VOBC_3_TO_OUT_NET       0x03


/*��������Ҳ��VOBC���͸����������ݡ�
��ATS���������ݡ����ϱ������ݣ�ÿ����ֻ����һ֡����Щ���ݶ��Ƿ���ATS���ؼ������ͬһ���˿ڣ����ǵ���һ���ϵ�ʱ�����͸�ATS��������Ҫ��������㴦����������к�Ϊ0ʱ��������3��0���ģ������౨�Ŀ�����5�֡�
VOBC���͸�ATS��NTPУʱ����Ҳ������ATS���ؼ����������Ŀ�Ķ˿ڲ�һ������Ҳ�Ա������ͽ��������֣��������ݣ�����Ҫ��������㣬�ʲ���Ҫ����3��0�������͡�
VOBC���͸�CI������ÿ���ڷ���1֡��Ҳ��Ҫ��������㴦��ͬ����Ҫ��������3��0��������*/

  /* VOBC����ATS */
#define     VOBC_TO_ATS1            0x04
#define     VOBC_TO_ATS2            0x05
#define     VOBC_TO_ATS3            0x06
#define     VOBC_TO_ATS4            0x07
#define     VOBC_TO_ATS5            0x08

  /* VOBC����ATS��NTP������������ */
#define     VOBC_TO_ATS_NTP         0x09

  /* VOBC����CI */
#define     VOBC_TO_CI1             0x0A
#define     VOBC_TO_CI2             0x0B
#define     VOBC_TO_CI3             0x0C
#define     VOBC_TO_CI4             0x0D


#define     VOBC_TO_MSS1            0x0E
#define     VOBC_TO_MSS2            0x0F

/*VOBC���͸�AOM,����������*/
#define     VOBC_TO_AOM1            0x10
#define     VOBC_TO_AOM2            0x11

/* ATP����COMM���������� ,ȫ�Զ���������,�Ƴ�,20180611*/
#define     VOBC_TO_COMM_DOWNLOAD   0x12
#define     ATO_TO_TRDP             0x1A
#define     TRDP_TO_ATO             0x1B
#define     TRDP_TO_LOGGER          0x1C

/*VOBC���͸�PSC����������֡*/
#define   	VOBC_TO_PSC				0x1D
#define		VOBC_TO_PSC2			0x1E

/*����������ATS��CI�����ݣ���ͨ�ſ�������������ֿ���Щ�������ͺ��������ƽ̨����Щ��ͬ��������д��Ӧ�ó�����Ӧ�Ļ������С�
ATS���͸�VOBC��NTP����������������Ĵ������Ժ�������ATS����Ҫ���ֿ���
CI���͸�VOBC��������Ҫʹ��RSSPЭ�飬����Ҳ��Ҫ��ATS���������ֿ� */

  /* ATS����VOBC */
#define     VOBC_FROM_ATS           0x20

  /* ATS����VOBC��NTP */
#define     VOBC_FROM_ATS_NTP       0x21

  /* CI����VOBC */
#define     VOBC_FROM_CI            0x22


/* ����������ʹ�������Լ��İ�ȫͨ��Э���ZC1,2��DSU�����ݶ��������������������ݣ���Ϊͨ�ſ������������ݷ��͸�ͨ�Ű壬ͨ�Ű��ٷ��͸�����ʱ����Ҫ���б������ʹ����������ͬ��������������Ҳ���������գ��������ݱ�����������Ϻ󣬶�������ָ���Ļ������У�������������������������ݻ��� */
 
/*����������ʹ�������Լ��İ�ȫͨ��Э���ZC1,2��DSU������*/
#define     VOBC_FROM_OUT_NET       0x23

/* AOM����VOBC */
#define     VOBC_FROM_AOM           0x24

/* PSC���͸�VOBC */
#define		VOBC_FROM_PSC			0x25

/*�����������������ǽ��ջ��Ƿ��ͣ���Ϊ��Щ�������м价�ڴ���ʱ�����պͷ��͵�����ͨ������ͬ���м价�ڿ���֪���ĸ�ͨ���ǽ��գ��ĸ�ͨ���Ƿ��͡�*/

  /* ��������һ��VOBC������ */
#define     VOBC_ANOTHER_VOBC       0x30

  /* ����ATO�����ݣ�ֻ�к����д����� */
#define     ATP_ATO                 0x31

  /* ����MMI������ */
#define     ATP_MMI                 0x32

  /* ��������TMS�����ݣ�ֻ�������д����� */
#define     ATP_TMS                 0x33

  /* ����ATO2������(����)��ֻ�������д����� */
#define     ATP_ATO2                0x33

  /* ��������CCOV(includeing WGB)�����ݣ�����ͨ�ſ�������WGB����� */
#define     ATP_RED_CCOV            0x34

  /* ��������CCOV(includeing WGB)�����ݣ�����ͨ�ſ�������WGB����� */
#define     ATP_BLUE_CCOV           0x35

  /* ������¼ϵͳ�����ݣ���������Ϊ�������ͼ�¼ϵͳ��Уʱʹ�ã���ʱ�����������յ���Ϣ����Ӧ�ü�¼����1�У���¼ϵͳ�ô�ʱ����У����ʱ�伴��*/
#define     ATP_LOGGER              0x36

  /* ����ATP�ͳ��������ܼ������������߲��Ե����ݣ����ڿ������߲���*/
#define     ATP_RED_RADIO_TEST      0x37

  /* ����ATP�ͳ��������ܼ�������������߲��Ե����ݣ����ڿ������߲���*/
#define     ATP_BLUE_RADIO_TEST     0x38

  /* ��������һ��VOBC������(�����и�����������) */
#define     VOBC_ANOTHER_VOBC_SDU   0x39

/*������Щ���ݶ��Ǹ���ϵͳ���͸���¼ϵͳ�ļ�¼���ݡ���¼ϵͳ����Ҫ��ĳЩ����ת������Яʽ�����ǡ�
ATP��ATO�ļ�¼������������������ݵļ�¼���������ϴ󣬲���Ҫ���͸���Яʽ�����ǣ�����ATP��ATO�Ĺ��ϱ�����Ϣ����ATP��ATO�豸������汾�ţ��豸���ϴ��뼰���й����еĹ��ϼ��־�������š�1��2�̣��״�У��ϵ����ATO�����ǣ���ƶ�ֵ����Ҫÿ���ھ����͸���Яʽ�����ǣ���ר��ΪATP��ATO����˹��ϼ�¼���ݡ�ATO��ATP�����м�¼���ݲ���Ҫ���͸���Яʽ�����ǡ�
����ļ�¼���ݶ����й��ϱ�����Ϣ�����������м�¼�����Լ�����汾�ţ�����Ҫ���͸���Яʽ�����ǣ��Ա�������ʾ������ϸ�Ĺ�����Ϣ�͸�����İ汾�š���Ϊ��¼ϵͳ�ͱ�Яʽ������֮��ʹ�õ��Ǵ��ڣ�������������ޣ���Щ���ݿ����ɼ�¼ϵͳ���⴦��ÿ��1������ʱ�䷢�͸���Яʽ�����ǡ�
�����¼ϵͳ����Ҫ������ͨ�Ű��MMI���ݣ�����һЩ�������������ݣ����͸���Яʽ�����ǣ��Ա�������ڵķ�����ԡ�
��¼ϵͳ����Ĺ��ϱ������ݣ���Ҫת������Яʽ�����ǣ�������Ҫ������¼ϵͳ������汾��
����ͨ�ſ������ļ�¼�����бȽ�����Ĵ���ʵ����ͨ�ſ��������ںͼ�¼ϵͳ�Ǻ���һ��ģ����ǵ�1�˹���ʱ��1��Ҫ��֪λ��2�˵�ͨ�ſ�������WGB��״̬������Щ��¼���ݶ����ȷ����Ѽ���˵�ͨ�Ű壬Ȼ������ͨ�Ű�ת�����Ѽ���˵ļ�¼ϵͳ���м�¼�����ڵ���ʱ��������Ҫ��¼���е��������ݣ�����Ծɲ��ô˷�����ͨ�Ű�ĸ���̫�أ��������Ҫ��¼��(��)������������ʱ�����ɺ�(��)��ͨ�ſ�����ͨ����Ϣ���л�������ʽֱ�ӷ��͸���(��)����¼����������������������ݶ��ڱ��˼�¼ϵͳ�Ͻ��м�¼����ʽ����ʱ�����е��������ݶ����ɵ���APM��¼�ģ�����ԭ���ϲ����������ݽ��м�¼��ֻ����ȷ�ϼ�¼ϵͳ������������Ҫ��ʱ�����ǲſ��ܽ����еĺ����������ڳ����豸�����˶����м�¼���Ա�͵���APM�����ݽ��бȽ�*/

  /* ����CCOV(including WGB)��¼���� */
#define VOBC_RED_CCOV_TO_LOGGER     0x40

  /* ����CCOV(including WGB)��¼���� */
#define VOBC_BLUE_CCOV_TO_LOGGER    0x41

  /*ͨ�Ű�1��¼����1 */
#define VOBC_COM_1_TO_LOGGER1        0x42

  /* ͨ�Ű�2��¼���� 1*/
#define VOBC_COM_2_TO_LOGGER1        0x43

  /* MMI��¼���� */
#define VOBC_MMI_TO_LOGGER          0x44

  /* ATO���ϼ�¼���� */
#define VOBC_ATO_ERR_TO_LOGGER      0x45

  /* ATO���м�¼���� */
#define VOBC_ATO_TO_LOGGER          0x46

  /* ATPӦ�ù��ϼ�¼���� */
#define VOBC_ATP_ERR_TO_LOGGER    	0x47

  /* ATPӦ�ü�¼����1 */
#define VOBC_1_ATP_TO_LOGGER        0x48

  /* ATPӦ�ü�¼����2 */
#define VOBC_2_ATP_TO_LOGGER        0x49

  /* A��ƽ̨��¼���� */
#define VOBC_TMR_A_TO_LOGGER        0x4A

  /* B��ƽ̨��¼���� */
#define VOBC_TMR_B_TO_LOGGER        0x4B

  /* C��ƽ̨��¼���� */
#define VOBC_TMR_C_TO_LOGGER        0x4C

  /* ��¼ϵͳ����ļ�¼���ݣ���Ҫת������Яʽ�����ǡ�*/
#define LOGGER_TO_TESTER            0x4D

  /*ATP���������Ϣ��¼����֡,���Ϊ�����,��A����ͨ�Ű巢��*/
#define VOBC_SDU_A_TO_LOGGER        0x4E

  /*ATP���������Ϣ��¼����֡,Ԥ��B��*/
#define VOBC_SDU_B_TO_LOGGER        0x4F

  /*ATP���������Ϣ��¼����֡,Ԥ��B��*/
#define VOBC_SDU_C_TO_LOGGER        0x5F

  /*ATO2���м�¼����*/
#define VOBC_ATO2_TO_LOGGER			0x5E

  /*ATO2���ϼ�¼����*/
#define VOBC_ATO2_ERR_TO_LOGGER     0x5D

  /*ATO1������ƽ̨��¼����*/
#define VOBC_ATORPM1_TO_LOGGER      0x5C

  /*ATO2������ƽ̨��¼����*/
#define VOBC_ATORPM2_TO_LOGGER      0x5B



/*��Яʽ������Ҳ���Ը���ATP�����ݣ�������������ǵ���ģ�ֻ�Ǳ�Яʽ��������ATP�������ݡ�*/

  /* ��Яʽ�����Ǹ���ATP���� */
#define VOBC_ATP_PARAMETER_UPDATE   0x50


/*����������������������ͨ�Ű�֮��*/

  /* TMR�������ݣ����� ���� ͨ�Ű�->���������ݷ��� */
#define TMR_CMD_TO_COMAB_START_SEND 0x60

  /* TMR�������ݣ����� ֹͣ ͨ�Ű�->���������ݷ��� */
#define TMR_CMD_TO_COMAB_STOP_SEND  0x61


/*�����������������ϵ�ʱBOOTLOADER����Ը�ϵͳ���е���������ݸ���*/

  /* ����A������µ��������� */
#define VOBC_ATP_A_SW_UPDATE        0x70

  /* ����B������µ��������� */
#define VOBC_ATP_B_SW_UPDATE        0x71

  /* ����C������µ��������� */
#define VOBC_ATP_C_SW_UPDATE        0x72

  /* ͨ�Ż�A������µ��������� */
#define VOBC_COMA_SW_UPDATE         0x73

  /* ͨ�Ż�B������µ��������� */
#define VOBC_COMB_SW_UPDATE         0x74

  /* ͨ�ſ�����������µ��������� */
#define VOBC_CCOV_SW_UPDATE         0x75

  /* ��¼ϵͳ������µ��������� */
#define VOBC_LOGGER_SW_UPDATE       0x76

  /* MMI������µ��������� */
#define VOBC_MMI_SW_UPDATE          0x77

  /* ATO������µ��������� */
#define VOBC_ATO_SW_UPDATE          0x78

/* ADD START 20180316 xb ������ͨFAO from LCF-400*/
/*CCOV����MMI�İ汾������*/
#define VOBC_RED_CCOV_TO_MMI    0x86

/*CCOV����MMI�İ汾������*/

#define VOBC_BLUE_CCOV_TO_MMI     0x87
/* ADD END 20180316 xb ������ͨFAO from LCF-400*/


/*AOM��������������*/
#define AOM_TO_OUT_NET				0x88

/*AOM���͸�����������*/
#define AOM_TO_IN_NET				0x90

/* VOBC������µ��������� */
#define HOST_TO_VOBC_SW_UPDATE          0x80

/*  VOBCȫ�Զ����ظ��µ���������,20190419 by sds  */
#define VOBC_TO_HOST_SW_UPDATE          0x81

/*������������ΪRSOV����ͨ�Ű�����ݸ��±���,20150724����*/
#define RSOV_DATA_UPDATE_CMD    0x80
/*������������Ϊͨ�Ű巴����RSOV�����ݸ��³ɹ�����,20150724����*/
#define RSOV_DATA_UPDATE_SUCCEED    0x81
/*������������Ϊͨ�Ű巴����RSOV�����ݸ���ʧ������,20150724����*/
#define RSOV_DATA_UPDATE_FAIL    0x82

/* ADD START 20180316 xb ������ͨFAO from LCF-400*/
/* ATPӦ�ü�¼����3 */
#define VOBC_3_ATP_TO_LOGGER		0x51

/* ATO MA��¼���� */
#define VOBC_ATO_MA_TO_LOGGER          0x52

/* ATO2 MA��¼���� */
#define VOBC_ATO2_MA_TO_LOGGER          0x53
/* ADD END 20180316 xb ������ͨFAO from LCF-400*/

/*ATO��TMS��¼����*/
#define VOBC_ATO_TMS_TO_LOGGER      0x54

/*20151208����������ͨ��������֡����*/
/*������ͨ֡��������*/
/*ATP1��CCOV������������*/
#define CCOV_AND_ATP1_TYPE		0x91
/*ATP2��CCOV������������*/
#define CCOV_AND_ATP2_TYPE		0x92
/*ATP3��CCOV������������*/
#define CCOV_AND_ATP3_TYPE		0x93
/*ATO1��CCOV������������*/
#define CCOV_AND_ATO1_TYPE		0x94
/*ATO2��CCOV������������*/
#define CCOV_AND_ATO2_TYPE		0x95
/*ATP��NDSU������������*/
#define NDSU_AND_ATP_TYPE		0x96
/*ATO��NDSU������������*/
#define NDSU_AND_ATO_TYPE		0x97

/*ͨ�Ű�1��¼����2 */
#define VOBC_COM_1_TO_LOGGER2        0x98

/* ͨ�Ű�2��¼���� 2*/
#define VOBC_COM_2_TO_LOGGER2        0x99


/*20150314����ͨ�Ŷ����ʶ*/
enum AomAcsbMutualMsgTypeEnum
{
    AOM_RESERVE_TYPE            = 0x00,
    VOBC_TO_AOM_RED_MSG_TYPE    = 0x01, /*VOBC��AOM��������*/
    VOBC_TO_AOM_BLU_MSG_TYPE    = 0x02, /*VOBC��AOM��������*/
    AOM_TO_VOBC_MSG_TYPE        = 0x03, /*AOM��VOBC����*/
    AOM_TO_AOM_MSG_TYPE         = 0x04, /*AOM��AOM����*/
    AOM_TO_CSB_MSG_TYPE         = 0x05, /*AOM��CSB����*/
    AOM_TO_TIAS_MSG_TYPE        = 0x06, /*AOM��TIAS����*/
    CSB_TO_AOM_MSG_TYPE         = 0x07, /*CSB��AOM����*/
    TIAS_TO_AOM_BLU_MSG_TYPE    = 0x08, /*TIAS��AOM��������*/
    TIAS_TO_AOM_RED_MSG_TYPE    = 0x09, /*TIAS��AOM��������*/
    AOM_TO_MSS_MSG_TYPE         = 0x0A, /*AOM��MSS����*/
    MSS_TO_AOM_RED_MSG_TYPE     = 0x0B, /*MSS��AOM��������*/
    MSS_TO_AOM_BLU_MSG_TYPE     = 0x0C,  /*MSS��AOM��������*/
	/* ADD START 20180316 xb ������ͨFAO from LCF-400*/
    AOM1_TO_CSB_LOGGER_MSG_TYPE     = 0x0D, /*AOM1���м�¼���ݱ���*/
    AOM2_TO_CSB_LOGGER_MSG_TYPE     = 0x0E, /*AOM2���м�¼���ݱ���*/
    AOM1RMP_TO_CSB_LOGGER_MSG_TYPE  = 0x0F, /*AOM1������ƽ̨��¼���ݱ���*/
    AOM2RMP_TO_CSB_LOGGER_MSG_TYPE  = 0x10, /*AOM2������ƽ̨��¼���ݱ���*/
    AOM1_TO_CSB_SFP_RECORD_TYPE     = 0X11, /*AOM SFP��¼���ݱ���*/
	/* ADD END 20180316 xb ������ͨFAO from LCF-400*/
    RES_AOM_AND_CSB_MSG_TYPE
};
#endif
