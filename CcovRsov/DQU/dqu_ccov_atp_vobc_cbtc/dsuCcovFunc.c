

/************************************************************************
* �ļ���	�� dsuCcovFunc.c
* ��Ȩ˵��	��	
* �汾��	�� 1.0  
* ����ʱ��	�� 2009.9.11
* ����		�� ϵͳ���ɲ�--�߹���
* ��������	�� DSUΪCCOV�ṩ�Ĳ�ѯ���� 
* ʹ��ע��	�� 
* �޸ļ�¼	�� 
2011.1.09,�������
1�����ݴ�����빫����Ŀ�������ݷŵ��������ļ��У�������.h�ļ���
2����������Ƶ����ݽṹ�������޸�
*20200825 lp ��ԭ��RPЭ���е�FS��ȡ��ʼ�������Ƶ�CCOV˽�в�ѯ������
************************************************************************/
#include <stdio.h>
#include <string.h>
#include "dsuCcovFunc.h"
#include "stdarg.h"
#include "stdio.h"
#include "dquDataTypeDefine.h"

/*�ṹ���ڲ����������ܺ�*/
#define DSU_IP_INFO_STRU_SIZE 30  
#define DSU_COMM_PROTCL_STRU_SIZE 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE 12
/*Դ��FAO��34��Ϊ(34+30)��BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE (34)
#define DSU_GATEWAY_INFO_STRU_SIZE 16


DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     /*��ʼ��֮�����ڱ����ʼ��֮������ݵı���*/


/* ��־���� */
void dquLogMsg(INT32 level, const CHAR* fmt, ...)
{
	INT8 s[1000];
	va_list arg_ptr;
	if(level<=LOG_LEVEL)
	{
		va_start(arg_ptr,fmt);
		vsprintf(s,fmt,arg_ptr);
		va_end(arg_ptr);
		//logMsg("%s",s);
	}
}


/*DSU�ṩ�Ĳ�ѯCCOV���������Եĺ���
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*����ֵ��  1-����; 2-����; 0xff-��Ч
*/
UINT8 dsuGetCcovRedOrBlue(const UINT8 CcovIpBuf[])
{
    /*��ʱ����1��ʾ����,��DSU���ݹ����ṩ��ѯ����*/
	UINT16 i = 0;/*ѭ������*/
	UINT16 j = 0;/*ѭ������*/
	UINT8 bReturnValue;/*��������ֵ*/
	DSU_IP_INFO_STRU* pCcovConfigStru;/*CCOV���ݽṹ����Ϣ*/

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*��������ip*/
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
		{		
			if (pCcovConfigStru->IpAddrBlue[j] == CcovIpBuf[j])
			{
				/*�����ֽ�ip��ַ��ȣ���ô�Ա���һ�ֽڣ�ֱ��������*/
			}
			else
			{	
				break;
			}
		}
		/*������һ��IP�����������������ip*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			bReturnValue = CCOV_BLUE_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;
    
	/*û�з��ϵ�����ip���ͱ�������ip*/
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
		{
			if (pCcovConfigStru->IpAddrRed[j] == CcovIpBuf[j])
			{
				/*�����ֽ�ip��ַ��ȣ���ô�Ա���һ�ֽڣ�ֱ��������*/
			}
			else
			{	
				break;
			}
		}
		/*������һ��IP������������غ���ip*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			bReturnValue = CCOV_RED_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	/*����������ip�������ϣ�������Чֵ*/
	bReturnValue = 0xff;
    return bReturnValue;
}

/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*���������WgbIpBuf  WGB��IP��ַ
*����ֵ��  �ҵ���Wgb�豸��IP��ַ������
*/
UINT8 dsuWgbIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 WgbIpBuf[][4], UINT16 WgbBufSize)
{
	/*�����ϵ�ʱ��ѯһ��*/
	UINT16 i = 0;/*ѭ������*/
	UINT8 bReturnValue;/*��������ֵ*/
	UINT8 bRedOrBlueIP;/*������IP��־����*/
	UINT16 tempVobcName; /*��ǰccov����vobc������*/
	UINT16 tempWgbName=0;  /*wgb����*/

	/*���ݵ�ǰccov ip��ѯvobcName�ͺ�������Ϣ*/
	bRedOrBlueIP=dsuGetDevNameByIp(CcovIpBuf,&tempVobcName);
	if ((bRedOrBlueIP!=CCOV_RED_IP)&&(bRedOrBlueIP!=CCOV_BLUE_IP))
	{
		/*��ѯvobcNameʧ�ܣ���������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*��ѯvobcName�ɹ�������ִ��*/
	}

	/*����vobcName��ѯ��ӦwgbName*/	
	for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC==tempVobcName)
		{
			tempWgbName=DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB;
			break;
		}
		else
		{
			/*����ѭ��*/
		}
	}

	if (i==DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*��ѯwgbNameʧ�ܣ���������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*��ѯ�ɹ�������ִ��*/
	}

	/*����wgbName�ͺ�������Ϣ��ѯwgb�豸ip*/
	bReturnValue=dsuGetAllDevIpByName(WgbIpBuf, 4, tempWgbName,bRedOrBlueIP);

	return bReturnValue;
}

/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*���������NtpServerIpBuf  NTP��������IP��ַ
*����ֵ��  �ҵ�NTP������IP��ַ������
*/
UINT8 dsuNtpServerIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NtpServerIpBuf[][4], UINT16 size)
{
	/*�����ϵ�ʱ��ѯһ��*/
	UINT16 i = 0;/*ѭ������*/
	UINT8 bReturnValue = 0;/*��������ֵ*/
	UINT8 bRedOrBlueIP = 0;/*������IP��־����*/
	UINT8 TempRet = 0;  /*��ʱ����ֵ����*/
	UINT16 tempVobcName = 0; /*��ǰccov����vobc������*/
	UINT16 tempNtpName=0;  /*Ntp����*/

	/*���ݵ�ǰip��ѯvobcName�ͺ�������Ϣ*/
	bRedOrBlueIP=dsuGetDevNameByIp(CcovIpBuf,&tempVobcName);
	if ((bRedOrBlueIP!=CCOV_RED_IP)&&(bRedOrBlueIP!=CCOV_BLUE_IP))
	{
		/*��ѯvobcNameʧ�ܣ���������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*��ѯvobcName�ɹ�������ִ��*/
	}

	/*����vobcName��ѯNtpName*/	
	for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC==tempVobcName)
		{
			tempNtpName=DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer;
			break;
		}
		else
		{
			/*����ѭ��*/
		}
	}

	if (i==DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*��ѯNtpNameʧ�ܣ���������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*��ѯ�ɹ�������ִ��*/
	}

	/*����NtpName�ͺ�������Ϣ��ѯ�豸ip*/
	TempRet=dsuGetAllDevIpByName(NtpServerIpBuf, size, tempNtpName, bRedOrBlueIP);

	return TempRet;
}


/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*���������NmsIpBuf  ����������ϵͳ��IP��ַ
UINT16* NmsPort���˿ں�
*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
*/
UINT8 dsuNmsIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NmsIpBuf[],UINT16* NmsPort)
{
	/*�����ϵ�ʱ��ѯһ��*/
	UINT16 i = 0;/*ѭ������*/
	UINT8 bReturnValue;/*��������ֵ*/
	UINT8 bRedOrBlueIP;/*������IP��־����*/
	UINT8 TempRet;  /*��ʱ����ֵ����*/
	UINT16 tempVobcName; /*��ǰccov����vobc������*/
	UINT16 tempNmsName=0;  /*Nms����*/

	/*���ݵ�ǰip��ѯvobcName�ͺ�������Ϣ*/
	bRedOrBlueIP=dsuGetDevNameByIp(CcovIpBuf,&tempVobcName);
	if ((bRedOrBlueIP!=CCOV_RED_IP)&&(bRedOrBlueIP!=CCOV_BLUE_IP))
	{
		/*��ѯvobcNameʧ�ܣ���������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*��ѯvobcName�ɹ�������ִ��*/
	}

	/*����vobcName��ѯNmsName*/	
	for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC==tempVobcName)
		{
			/*nmsName��ֵ*/
			tempNmsName=DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS;
			if (bRedOrBlueIP==CCOV_RED_IP)
			{
				/*�˿ڸ�ֵ����ǰΪ����*/
				*NmsPort=DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort;
			}
			else
			{
				/*�˿ڸ�ֵ����ǰΪ����*/
				*NmsPort=DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort;
			}
			break;
		}
		else
		{
			/*����ѭ��*/
		}
	}

	if (i==DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*��ѯNmsNameʧ�ܣ���������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*��ѯ�ɹ�������ִ��*/
	}

	/*����NmsName�ͺ�������Ϣ��ѯ�豸ip*/
	TempRet=dsuGetDevIpByName(NmsIpBuf,tempNmsName,bRedOrBlueIP);

	if (TempRet==1)
	{
		/*��ѯ�ɹ�*/
		bReturnValue=1;
	}
	else
	{
		/*��ѯʧ��*/
		bReturnValue=0;
	}

	return bReturnValue;
}

/*ͨ��ip��ַ��ѯ�豸���ƺͺ�������Ϣ
*�������: UINT8 CcovIpBuf[]  �豸ip��ַ
*���������UINT16* DevName �豸����
*����ֵ��  1-����; 2-����; 0xff-��Ч
*/
UINT8 dsuGetDevNameByIp(const UINT8 DevIpBuf[],UINT16* DevName)
{
	UINT16 i = 0;
	UINT16 j = 0;
	UINT8 bReturnValue;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*��������ip*/
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
		{
			if (pCcovConfigStru->IpAddrBlue[j] == DevIpBuf[j])
			{
				/*�����ֽ�ip��ַ��ȣ���ô�Ա���һ�ֽڣ�ֱ��������*/
			}
			else
			{	
				break;
			}
		}
		/*������һ��IP�������������*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			*DevName=pCcovConfigStru->DevName;
			bReturnValue = CCOV_BLUE_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;
	/*û�з��ϵ�����ip���ͱ�������ip*/
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
		{
			if (pCcovConfigStru->IpAddrRed[j] == DevIpBuf[j])
			{
				/*�����ֽ�ip��ַ��ȣ���ô�Ա���һ�ֽڣ�ֱ��������*/
			}
			else
			{	
				break;
			}
		}
		/*������һ��IP�������������*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			*DevName=pCcovConfigStru->DevName;
			bReturnValue = CCOV_RED_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	/*����������ip�������ϣ�������Чֵ*/
	bReturnValue = 0xff;
	return bReturnValue;
}

/*ͨ���豸���ƺͺ�������ѯ�豸IP��ַ�Ͷ˿ں�
*�������: UINT16 DevName �豸����
*			int nIPMaxCount:IP��ַ���������
*���������ipInfo �豸IP��ַ��Ϣ����
*����ֵ��  �ҵ���IP����
*/
UINT8 dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, UINT16 DevName)
{
	UINT16 i = 0;   /* ��ʱѭ������ */
	int	nIPCount = 0;  /* IP��ַ���� */
	DSU_IP_INFO_STRU* pCcovConfigStru = NULL;  /* IP��ַ��Ϣָ�� */

	/*��ڲ������*/
	if (NULL == ipInfo)
	{
		/* ���������Ч������-1 */
		return 0;
	}
	else
	{
		/* ��ڲ�����Ч������Ҫ���⴦�� */
	}

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*�����豸ip��ַ��*/
	for (i = 0; i < DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (nIPCount < nIPMaxCount)
		{
			/*
			printf("pCcovConfigStru->DevName = 0x%.4x\n", pCcovConfigStru->DevName);
			printf("DevName = 0x%.4x\n", DevName);
			*/
			if (pCcovConfigStru->DevName == DevName)
			{			
				/* ��ȡ��ǰIP��Ϣ */
				memcpy(&ipInfo[nIPCount], pCcovConfigStru, sizeof(DSU_IP_INFO_STRU));

				/*���ҵ��豸�����سɹ�*/
				nIPCount++;
			}
			else
			{
				/*δ�ҵ��豸���ƣ�����ѭ��*/
			}
		}

		/*��ѯָ�����*/
		pCcovConfigStru++;
	}

	/* �����ҵ���IP��ַ���� */
	return nIPCount;
}

/*ͨ���豸���ƺͺ�������ѯ�豸ip��ַ
*�������:  UINT16 DevName �豸����
			      UINT8 RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������UINT8 CcovIpBuf[]  �豸ip��ַ
*����ֵ��  1-��ѯ�ɹ�;  0-��ѯʧ��
*�޸ļ�¼: �޸�QC689,�Ƴ�,20180816;
*/
UINT8 dsuGetDevIpByName(UINT8 DevIpBuf[], UINT16 DevName, UINT8 RedOrBlue)
{
	UINT16 i = 0;			 /*ѭ�����Ʊ���*/
	UINT8 bReturnValue = 0;  /*��������ֵ*/
	DSU_IP_INFO_STRU *pCcovConfigStru = NULL; /*IP��ַ�ṹ��*/

	/*��ڲ������*/
	if((NULL != DevIpBuf) && ((CCOV_RED_IP == RedOrBlue) || (CCOV_BLUE_IP == RedOrBlue)))
	{
		/*������������Ч,����ִ��*/
		
		/*���ù�����,��CCOV��ͷ��ַ���ֲ�����*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;
	
		/*�����豸ip��ַ��*/
		for(i = 0;i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{
			if(pCcovConfigStru->DevName == DevName)
			{
				/*���ҵ��豸����*/
				if(CCOV_RED_IP == RedOrBlue)
				{
					/*��ǰ����*/
					memcpy(DevIpBuf, pCcovConfigStru->IpAddrRed, DSURP_LEN_IP_ADDR);
				}
				else 
				{
					/*��ǰ����*/
					memcpy(DevIpBuf, pCcovConfigStru->IpAddrBlue, DSURP_LEN_IP_ADDR);
				}
	
				/*���ҵ��豸,���سɹ�*/
				bReturnValue = 1;
				break;
			}
			else
			{
				/*δ�ҵ��豸����,����ѭ��*/
			}
	
			/*��ѯָ�����*/
			pCcovConfigStru++;
		}
	}
	else
	{
		/*���������Ч,����ʧ��*/
		bReturnValue = 0;
	}

	return bReturnValue;
}

/*ͨ���豸���ƺͺ�������ѯ�豸���е�ip��ַ
*�������: UINT16 DevName �豸����
*UINT16 BufSize:IP��ַ��������С
UINT8 RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������UINT8 CcovIpBuf[]  �豸ip��ַ
*����ֵ��  �ҵ���IP��ַ������,0δ���ҵ�,��0���ҳɹ�(�豸IP������)
*�޸ļ�¼: �޸�QC689,��Ԫ��,20190313.
*/
UINT8 dsuGetAllDevIpByName(UINT8 DevIpBuf[][4], UINT16 BufSize, UINT16 DevName, UINT8 RedOrBlue)
{
	UINT16 i = 0;  /*ѭ������*/
	UINT8 Index = 0;  /*��������ֵ*/
	DSU_IP_INFO_STRU *pCcovConfigStru = NULL; /*IP��ַ�ṹ��ָ��*/

	/*��ڲ������*/
	if((CCOV_RED_IP == RedOrBlue) || (CCOV_BLUE_IP == RedOrBlue))
	{
		/*������������Ч,����ִ��*/

		/*���ù�����,��CCOV��ͷ��ַ���ֲ�����*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

		/*�����豸ip��ַ��*/
		for(i = 0;i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{
			if(pCcovConfigStru->DevName == DevName)
			{
				if(Index < BufSize)
				{
					/*���ҵ��豸����*/
					if(CCOV_RED_IP == RedOrBlue)
					{
						/*��ǰ����*/
						memcpy(DevIpBuf[Index], pCcovConfigStru->IpAddrRed, DSURP_LEN_IP_ADDR);
					}
					else 
					{
						/*��ǰ����*/
						memcpy(DevIpBuf[Index], pCcovConfigStru->IpAddrBlue, DSURP_LEN_IP_ADDR);
					}

					Index++;
				}
			}
			else
			{
				/*δ�ҵ��豸����,����ѭ��*/
			}

			/*��ѯָ�����*/
			pCcovConfigStru++;
		}
	}
	else
	{
		/*���������Ч*/
		Index = 0;
	}

	return Index;
}

/*
* �������������ݺ�����IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
UINT8 dsuGetRedDevName(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
    UINT16 i=0;/* ѭ��ʹ�� */
	UINT8 flag=0;
    UINT16 ipInfoLen=0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru=DsuBookIpInfoStru.pIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrRed[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrRed[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrRed[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrRed[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpRedPort)||(port==pTempIpInfoStru[i].RsspRedPort)||
			(port==pTempIpInfoStru[i].SfpRedPort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	
	return retnVal;
}

/*
* ��������������������IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
UINT8 dsuGetBlueDevName(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
	UINT16 i=0;/* ѭ��ʹ�� */
	UINT8 flag=0;
	UINT16 ipInfoLen=0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru=DsuBookIpInfoStru.pIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrBlue[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrBlue[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrBlue[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrBlue[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpBluePort)||(port==pTempIpInfoStru[i].RsspBluePort)||
			(port==pTempIpInfoStru[i].SfpBluePort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	
	return retnVal;
}

/*ͨ���豸���ƺͺ�������ѯ�豸����
*�������: UINT16 DevName �豸����
UINT8 RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������UINT8 DevGateWayBuf[] �豸���ص�ַ
*����ֵ��  1-�ɹ���0-ʧ��
*/
UINT8 dsuGetGateWayByName(UINT8 DevGateWayBuf[],UINT16 DeviceName,UINT8 RedOrBlue)
{
	UINT16 i = 0;
	UINT8 bReturnValue;
	DSU_GATEWAY_INFO_STRU* pCcovConfigStru = NULL;

	/*��ڲ������*/
	if ((RedOrBlue==CCOV_RED_IP)||(RedOrBlue==CCOV_BLUE_IP))
	{
		/*������������Ч������ִ��*/
	}
	else
	{
		/*���������Ч����������ʧ��*/
		bReturnValue=0;
		return bReturnValue;
	}

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pGatewayInfoStru;

	/*�����豸ip��ַ��*/
	/*20200803 lp �޸�ԭ�д���׺�ȱ��*/
	for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
	{
		if (pCcovConfigStru->DeviceName == DeviceName)
		{
			/*���ҵ��豸����*/
			if (RedOrBlue==CCOV_RED_IP)
			{
				/*��ǰ����*/
				memcpy(DevGateWayBuf,pCcovConfigStru->IpAddrGatewayRed,DSURP_LEN_IP_ADDR);
			}
			else 
			{
				/*��ǰ����*/
				memcpy(DevGateWayBuf,pCcovConfigStru->IpAddrGatewayBlue,DSURP_LEN_IP_ADDR);
			}
			/*���ҵ��豸�����سɹ�*/
			bReturnValue=1;
			return bReturnValue;
		}
		else
		{
			/*δ�ҵ��豸���ƣ�����ѭ��*/
		}

		/*��ѯָ�����*/
		pCcovConfigStru++;
	}

	/*û���ҵ�devName�����ز�ѯʧ��*/
	bReturnValue = 0;
	return bReturnValue;
}


/*ͨ���豸���Ʋ�ѯ����UDP����Դ�˿ں�
*�������: UINT16 DevName �豸����
*���������UINT16* SourcePort :UDP����Դ�˿ں�
*����ֵ����ѯ�ɹ�����1�����򷵻�0
*/
UINT8 dsuGetRedSourcePortByName(UINT16 DevName,UINT16* SourcePort)
{
	UINT16 i = 0;   /* ��ʱѭ������ */
	UINT8 rslt = 0;  /* ��ѯ�������ֵ*/
	DSU_IP_INFO_STRU* pCcovConfigStru = NULL;  /* IP��ַ��Ϣָ�� */

	/*��ڲ������*/
	if (NULL == DevName)
	{
		/* ���������Ч������0*/
		rslt = 0;
	}
	else
	{
		/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

		/*�����豸ip��ַ��*/
		for (i = 0; i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{

			if (pCcovConfigStru->DevName == DevName)
			{			
				/*�ҵ�һ������ƥ��ļ��˳�����*/
				*SourcePort=pCcovConfigStru->RedNetSourcePort;
				rslt = 1;
				break;
			}
			else
			{
				/*δ�ҵ��豸���ƣ�����ѭ��*/
			}
			/*��ѯָ�����*/
			pCcovConfigStru++;
		}
	}
	return rslt;
}

/*ͨ���豸���Ʋ�ѯ����UDP����Դ�˿ں�
*�������: UINT16 DevName �豸����
*���������UINT16* SourcePort :UDP����Դ�˿ں�
*����ֵ����ѯ�ɹ�����1�����򷵻�0
*/
UINT8 dsuGetBlueSourcePortByName(UINT16 DevName,UINT16* SourcePort)
{
	UINT16 i = 0;   /* ��ʱѭ������ */
	UINT8 rslt = 0;  /* ��ѯ�������ֵ*/
	DSU_IP_INFO_STRU* pCcovConfigStru = NULL;  /* IP��ַ��Ϣָ�� */

	/*��ڲ������*/
	if (NULL == DevName)
	{
		/* ���������Ч������0*/
		rslt = 0;
	}
	else
	{
		/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

		/*�����豸ip��ַ��*/
		for (i = 0; i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{

			if (pCcovConfigStru->DevName == DevName)
			{			
				/*�ҵ�һ������ƥ��ļ��˳�����*/
				*SourcePort=pCcovConfigStru->BlueNetSourcePort;
				rslt = 1;
				break;
			}
			else
			{
				/*δ�ҵ��豸���ƣ�����ѭ��*/
			}
			/*��ѯָ�����*/
			pCcovConfigStru++;
		}
	}
	return rslt;
}


/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������UINT8* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*�ϲ�˵������CPK���룬BY LJ,20161210
*/
static UINT8 dsuIpInit_CPK(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;

	/*��Ч�Լ��*/
	if(NULL == FSName)
	{
		chReturnValue=0;
	}
	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;
		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}	

	/*��ʼ���豸������ͨ��Э���ϵ��*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}
	

	/*��ʼ��CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}	

	/*��ʼ��ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<DSU_MAX_ZCDSU_COMM_OBJ_NUM;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
				dataOff+=2;

			}

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{

		}
	}	

	/*����ִ��*/
	return chReturnValue;
}

/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������UINT8* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*�ϲ�˵����FAO��HLT���ã�BY LJ,20161210
* ��������:201701
* ������:zlj
*/
static UINT8 dsuIpInit_DFT(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*����ֵ*/
	UINT8 tempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;
	UINT8 tmpDsuMaxZcDsuCommObjNum = 0;/*��ʱ��¼ZCDSU��Ŀ�����*/
	UINT8 cbtcVersionType=0;

	/*��Ч�Լ��*/
	if(NULL == FSName)
	{
		chReturnValue=0;
	}
	
	cbtcVersionType = GetCbtcSysType();

	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;
			
			DsuBookIpInfoStru.pIpInfoStru[i].HLHTId=LongFromChar(pData+dataOff);
			dataOff+=4;
			
			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].BlueNetSourcePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RedNetSourcePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}

	

	/*��ʼ���豸������ͨ��Э���ϵ��*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}	


	/*��ʼ��CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}

	

	/*��ʼ��ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	
	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO);  /*�����������ݵĸ���*/
	
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		
		tmpDsuMaxZcDsuCommObjNum = DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO;  /*�����������ݵĸ���*/
		
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<tmpDsuMaxZcDsuCommObjNum;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
				dataOff+=2;

			}

		}
		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}



	/*��ʼ���豸���ر���Ϣ*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_GATEWAY_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.GatewayInfoLen=(UINT16)(dataLen/DSU_GATEWAY_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pGatewayInfoStru=(DSU_GATEWAY_INFO_STRU*)malloc(sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pGatewayInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pGatewayInfoStru,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen),0,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
		{
			/*��� 2*/
			dataOff+=2;

			/*�豸����2*/
			DsuBookIpInfoStru.pGatewayInfoStru[i].DeviceName = ShortFromChar(pData+dataOff);
			dataOff+=2;

			/*�豸�������� 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*�豸�������� 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*�������� 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrSubnetMask,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{

		}
	}	

	/*����ִ��*/
	return chReturnValue;
}


/*��������: ���ʽӿ�
* �� �� ֵ��1���ɹ�
*           0��ʧ��
* ��������:201701
* ������:zlj
*/
UINT8 dsuCcovIpInit(UINT8*FSName)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	/*��Ч�Լ��*/
	if(NULL != FSName)
	{			
		cbtcVersionType=GetCbtcSysType();

		if (DQU_CBTC_CPK == cbtcVersionType )
		{
		
			rtnValue = dsuIpInit_CPK(FSName);		
		} 
		else if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)|| (DQU_CBTC_DG== cbtcVersionType) )
		{
			rtnValue = dsuIpInit_DFT(FSName);		
		}
		else
		{
			rtnValue = 0;		
		}
	}
	else
	{
		rtnValue = 0;		
	}
	
	return rtnValue; 
}



