#include "GlobalDefine.h"

#include <stdio.h>
#include <string.h>

#include "dsuRpFunc.h"
#include "dsuCcovFunc.h"
#include "stdarg.h"
#include "stdio.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"

#define DSU_GATEWAY_INFO_STRU_SIZE 16


DSU_GATEWAY_SHEET_STRU DsuGatewaySheetStru;


/* ��־���� */
void dquLogMsg(int level, const char* fmt, ...)
{
	char s[1000];
	va_list arg_ptr;
	if (level <= LOG_LEVEL)
	{
		va_start(arg_ptr, fmt);
		vsprintf(s, fmt, arg_ptr);
		va_end(arg_ptr);
	}
}


/*DSU�ṩ�Ĳ�ѯCCOV���������Եĺ���
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*����ֵ��  1-����; 2-����; 0xff-��Ч
*/
unsigned char dsuGetCcovRedOrBlue(const unsigned char CcovIpBuf[])
{
	/*��ʱ����1��ʾ����,��DSU���ݹ����ṩ��ѯ����*/
	unsigned short i = 0;/*ѭ������*/
	unsigned short j = 0;/*ѭ������*/
	unsigned char bReturnValue;/*��������ֵ*/
	DSU_IP_INFO_STRU* pCcovConfigStru;/*CCOV���ݽṹ����Ϣ*/

									  /*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*��������ip*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
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
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
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
unsigned char dsuWgbIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char WgbIpBuf[][4], unsigned short WgbBufSize)
{
	/*�����ϵ�ʱ��ѯһ��*/
	unsigned short i = 0;/*ѭ������*/
	unsigned char bReturnValue;/*��������ֵ*/
	unsigned char bRedOrBlueIP;/*������IP��־����*/
	unsigned short tempVobcName; /*��ǰccov����vobc������*/
	unsigned short tempWgbName = 0;  /*wgb����*/

							 /*���ݵ�ǰccov ip��ѯvobcName�ͺ�������Ϣ*/
	bRedOrBlueIP = dsuGetDevNameByIp(CcovIpBuf, &tempVobcName);
	if ((bRedOrBlueIP != CCOV_RED_IP) && (bRedOrBlueIP != CCOV_BLUE_IP))
	{
		/*��ѯvobcNameʧ�ܣ���������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*��ѯvobcName�ɹ�������ִ��*/
	}

	/*����vobcName��ѯ��ӦwgbName*/
	for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC == tempVobcName)
		{
			tempWgbName = DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB;
			break;
		}
		else
		{
			/*����ѭ��*/
		}
	}

	if (i == DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*��ѯwgbNameʧ�ܣ���������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*��ѯ�ɹ�������ִ��*/
	}

	/*����wgbName�ͺ�������Ϣ��ѯwgb�豸ip*/
	bReturnValue = dsuGetAllDevIpByName(WgbIpBuf, 4, tempWgbName, bRedOrBlueIP);

	return bReturnValue;
}

/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*���������NtpServerIpBuf  NTP��������IP��ַ
*����ֵ��  �ҵ�NTP������IP��ַ������
*/
unsigned char dsuNtpServerIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NtpServerIpBuf[][4], unsigned short size)
{
	/*�����ϵ�ʱ��ѯһ��*/
	unsigned short i = 0;/*ѭ������*/
	unsigned char bReturnValue = 0;/*��������ֵ*/
	unsigned char bRedOrBlueIP = 0;/*������IP��־����*/
	unsigned char TempRet = 0;  /*��ʱ����ֵ����*/
	unsigned short tempVobcName = 0; /*��ǰccov����vobc������*/
	unsigned short tempNtpName = 0;  /*Ntp����*/

							 /*���ݵ�ǰip��ѯvobcName�ͺ�������Ϣ*/
	bRedOrBlueIP = dsuGetDevNameByIp(CcovIpBuf, &tempVobcName);
	if ((bRedOrBlueIP != CCOV_RED_IP) && (bRedOrBlueIP != CCOV_BLUE_IP))
	{
		/*��ѯvobcNameʧ�ܣ���������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*��ѯvobcName�ɹ�������ִ��*/
	}

	/*����vobcName��ѯNtpName*/
	for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC == tempVobcName)
		{
			tempNtpName = DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer;
			break;
		}
		else
		{
			/*����ѭ��*/
		}
	}

	if (i == DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*��ѯNtpNameʧ�ܣ���������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*��ѯ�ɹ�������ִ��*/
	}

	/*����NtpName�ͺ�������Ϣ��ѯ�豸ip*/
	TempRet = dsuGetAllDevIpByName(NtpServerIpBuf, size, tempNtpName, bRedOrBlueIP);

	return TempRet;
}


/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*���������NmsIpBuf  ����������ϵͳ��IP��ַ
unsigned short* NmsPort���˿ں�
*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
*/
unsigned char dsuNmsIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NmsIpBuf[], unsigned short* NmsPort)
{
	/*�����ϵ�ʱ��ѯһ��*/
	unsigned short i = 0;/*ѭ������*/
	unsigned char bReturnValue;/*��������ֵ*/
	unsigned char bRedOrBlueIP;/*������IP��־����*/
	unsigned char TempRet;  /*��ʱ����ֵ����*/
	unsigned short tempVobcName; /*��ǰccov����vobc������*/
	unsigned short tempNmsName = 0;  /*Nms����*/

							 /*���ݵ�ǰip��ѯvobcName�ͺ�������Ϣ*/
	bRedOrBlueIP = dsuGetDevNameByIp(CcovIpBuf, &tempVobcName);
	if ((bRedOrBlueIP != CCOV_RED_IP) && (bRedOrBlueIP != CCOV_BLUE_IP))
	{
		/*��ѯvobcNameʧ�ܣ���������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*��ѯvobcName�ɹ�������ִ��*/
	}

	/*����vobcName��ѯNmsName*/
	for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC == tempVobcName)
		{
			/*nmsName��ֵ*/
			tempNmsName = DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS;
			if (bRedOrBlueIP == CCOV_RED_IP)
			{
				/*�˿ڸ�ֵ����ǰΪ����*/
				*NmsPort = DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort;
			}
			else
			{
				/*�˿ڸ�ֵ����ǰΪ����*/
				*NmsPort = DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort;
			}
			break;
		}
		else
		{
			/*����ѭ��*/
		}
	}

	if (i == DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*��ѯNmsNameʧ�ܣ���������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*��ѯ�ɹ�������ִ��*/
	}

	/*����NmsName�ͺ�������Ϣ��ѯ�豸ip*/
	TempRet = dsuGetDevIpByName(NmsIpBuf, tempNmsName, bRedOrBlueIP);

	if (TempRet == 1)
	{
		/*��ѯ�ɹ�*/
		bReturnValue = 1;
	}
	else
	{
		/*��ѯʧ��*/
		bReturnValue = 0;
	}

	return bReturnValue;
}

/*ͨ��ip��ַ��ѯ�豸���ƺͺ�������Ϣ
*�������: unsigned char CcovIpBuf[]  �豸ip��ַ
*���������unsigned short* DevName �豸����
*����ֵ��  1-����; 2-����; 0xff-��Ч
*/
unsigned char dsuGetDevNameByIp(const unsigned char DevIpBuf[], unsigned short* DevName)
{
	unsigned short i = 0;
	unsigned short j = 0;
	unsigned char bReturnValue;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*��������ip*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
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
			*DevName = pCcovConfigStru->DevName;
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
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
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
			*DevName = pCcovConfigStru->DevName;
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
*�������: unsigned short DevName �豸����
*			int nIPMaxCount:IP��ַ���������
*���������ipInfo �豸IP��ַ��Ϣ����
*����ֵ��  �ҵ���IP����
*/
unsigned char dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, unsigned short DevName)
{
	unsigned short i = 0;   /* ��ʱѭ������ */
	int	nIPCount = 0;  /* IP��ַ���� */
	DSU_IP_INFO_STRU* pCcovConfigStru = nullptr;  /* IP��ַ��Ϣָ�� */

											   /*��ڲ������*/
	if (nullptr == ipInfo)
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
*�������: unsigned short DevName �豸����
unsigned char RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������unsigned char CcovIpBuf[]  �豸ip��ַ
*����ֵ��  1-�ɹ���0-ʧ��
*/
unsigned char dsuGetDevIpByName(unsigned char DevIpBuf[], unsigned short DevName, unsigned char RedOrBlue)
{
	unsigned short i = 0;
	unsigned char bReturnValue;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*��ڲ������*/
	if ((RedOrBlue == CCOV_RED_IP) || (RedOrBlue == CCOV_BLUE_IP))
	{
		/*������������Ч������ִ��*/
	}
	else
	{
		/*���������Ч����������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*�����豸ip��ַ��*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (pCcovConfigStru->DevName == DevName)
		{
			/*���ҵ��豸����*/
			if (RedOrBlue == CCOV_RED_IP)
			{
				/*��ǰ����*/
				memcpy(DevIpBuf, pCcovConfigStru->IpAddrRed, DSURP_LEN_IP_ADDR);
			}
			else
			{
				/*��ǰ����*/
				memcpy(DevIpBuf, pCcovConfigStru->IpAddrBlue, DSURP_LEN_IP_ADDR);
			}

			/*���ҵ��豸�����سɹ�*/
			bReturnValue = 1;
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

/*ͨ���豸���ƺͺ�������ѯ�豸���е�ip��ַ
*�������: unsigned short DevName �豸����
*unsigned short BufSize:IP��ַ��������С
unsigned char RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������unsigned char CcovIpBuf[]  �豸ip��ַ
*����ֵ��  �ҵ���IP��ַ������,0δ���ҵ�,��0���ҳɹ�(�豸IP������)
*/
unsigned char dsuGetAllDevIpByName(unsigned char DevIpBuf[][4], unsigned short BufSize, unsigned short DevName, unsigned char RedOrBlue)
{
	unsigned short i = 0;
	unsigned char bReturnValue;
	unsigned char Index = 0;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*��ڲ������*/
	if ((RedOrBlue == CCOV_RED_IP) || (RedOrBlue == CCOV_BLUE_IP))
	{
		/*������������Ч������ִ��*/
	}
	else
	{
		/*���������Ч����������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*�����豸ip��ַ��*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (pCcovConfigStru->DevName == DevName)
		{
			if (Index<BufSize)
			{
				/*���ҵ��豸����*/
				if (RedOrBlue == CCOV_RED_IP)
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
			/*δ�ҵ��豸���ƣ�����ѭ��*/
		}

		/*��ѯָ�����*/
		pCcovConfigStru++;
	}

	/*û���ҵ�devName������ֵ���ҵ�������*/
	return Index;
}

/*
* �������������ݺ�����IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������unsigned short port �˿ں�
*           unsigned char* ipAdd  IP��ַ
* ���������devName �豸����  unsigned short*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
unsigned char dsuGetRedDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName)
{
	unsigned char retnVal = 0;
	unsigned short i = 0;/* ѭ��ʹ�� */
	unsigned char flag = 0;
	unsigned short ipInfoLen = 0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen = DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru = DsuBookIpInfoStru.pIpInfoStru;

	for (i = 0; i<ipInfoLen; i++)
	{
		if ((ipAdd[0] == pTempIpInfoStru[i].IpAddrRed[0]) && (ipAdd[1] == pTempIpInfoStru[i].IpAddrRed[1]) &&
			(ipAdd[2] == pTempIpInfoStru[i].IpAddrRed[2]) && (ipAdd[3] == pTempIpInfoStru[i].IpAddrRed[3]))
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}
		if ((flag == 1) && ((port == pTempIpInfoStru[i].RpRedPort) || (port == pTempIpInfoStru[i].RsspRedPort) ||
			(port == pTempIpInfoStru[i].SfpRedPort)))
		{
			*devName = pTempIpInfoStru[i].DevName;
			retnVal = 1;
			break;
		}
		else
		{
			retnVal = 0;
		}
	}

	return retnVal;
}

/*
* ��������������������IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������unsigned short port �˿ں�
*           unsigned char* ipAdd  IP��ַ
* ���������devName �豸����  unsigned short*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
unsigned char dsuGetBlueDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName)
{
	unsigned char retnVal = 0;
	unsigned short i = 0;/* ѭ��ʹ�� */
	unsigned char flag = 0;
	unsigned short ipInfoLen = 0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen = DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru = DsuBookIpInfoStru.pIpInfoStru;

	for (i = 0; i<ipInfoLen; i++)
	{
		if ((ipAdd[0] == pTempIpInfoStru[i].IpAddrBlue[0]) && (ipAdd[1] == pTempIpInfoStru[i].IpAddrBlue[1]) &&
			(ipAdd[2] == pTempIpInfoStru[i].IpAddrBlue[2]) && (ipAdd[3] == pTempIpInfoStru[i].IpAddrBlue[3]))
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}
		if ((flag == 1) && ((port == pTempIpInfoStru[i].RpBluePort) || (port == pTempIpInfoStru[i].RsspBluePort) ||
			(port == pTempIpInfoStru[i].SfpBluePort)))
		{
			*devName = pTempIpInfoStru[i].DevName;
			retnVal = 1;
			break;
		}
		else
		{
			retnVal = 0;
		}
	}

	return retnVal;
}

/*ͨ���豸���ƺͺ�������ѯ�豸����
*�������: unsigned short DevName �豸����
unsigned char RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������unsigned char DevGateWayBuf[] �豸���ص�ַ
*����ֵ��  1-�ɹ���0-ʧ��
*/
unsigned char dsuGetGateWayByName(unsigned char DevGateWayBuf[], unsigned short DeviceName, unsigned char RedOrBlue)
{
	unsigned short i = 0;
	unsigned char bReturnValue;
	DSU_GATEWAY_INFO_STRU* pCcovConfigStru = nullptr;

	/*��ڲ������*/
	if ((RedOrBlue == CCOV_RED_IP) || (RedOrBlue == CCOV_BLUE_IP))
	{
		/*������������Ч������ִ��*/
	}
	else
	{
		/*���������Ч����������ʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*���ù���������CCOV��ͷ��ַ���ֲ�����*/
	pCcovConfigStru = DsuGatewaySheetStru.pGatewayInfoStru;

	/*�����豸ip��ַ��*/
	for (i = 0; i<DsuGatewaySheetStru.GatewayInfoLen; i++)
	{
		if (pCcovConfigStru->DeviceName == DeviceName)
		{
			/*���ҵ��豸����*/
			if (RedOrBlue == CCOV_RED_IP)
			{
				/*��ǰ����*/
				memcpy(DevGateWayBuf, pCcovConfigStru->IpAddrGatewayRed, DSURP_LEN_IP_ADDR);
			}
			else
			{
				/*��ǰ����*/
				memcpy(DevGateWayBuf, pCcovConfigStru->IpAddrGatewayBlue, DSURP_LEN_IP_ADDR);
			}
			//logMsg("IPPP=%d,%d,%d,%d\n", DevGateWayBuf[0], DevGateWayBuf[1], DevGateWayBuf[2], DevGateWayBuf[3], 0, 0);
			/*���ҵ��豸�����سɹ�*/
			bReturnValue = 1;
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

/*
* �����������豸���ر��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������unsigned char* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*          			 0��ʧ�ܣ������д���
*/

unsigned char dsuGatewayInit(char*FSName)
{
	unsigned char chReturnValue = 1; /*����ֵ*/
	unsigned char tempRet;
	unsigned char* pData = nullptr;       /*���ڶ�ȡ���ݵ�ָ��*/
	unsigned int dataLen = 0;        /*���ݳ���*/
	unsigned int dataOff = 0;        /*����ƫ����*/
	unsigned int i;

	/*��Ч�Լ��*/
	if (nullptr == FSName)
	{
		chReturnValue = 0;
	}

	/*��ʼ���豸���ر���Ϣ*/
	/*��ȡ����*/
	tempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_GATEWAY_ID, &pData, &dataLen);
	if (tempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuGatewaySheetStru.GatewayInfoLen = (unsigned short)(dataLen / DSU_GATEWAY_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuGatewaySheetStru.pGatewayInfoStru = (DSU_GATEWAY_INFO_STRU*)malloc(sizeof(DSU_GATEWAY_INFO_STRU)*(DsuGatewaySheetStru.GatewayInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuGatewaySheetStru.pGatewayInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuGatewaySheetStru.pGatewayInfoStru, 0, sizeof(DSU_GATEWAY_INFO_STRU)*(DsuGatewaySheetStru.GatewayInfoLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuGatewaySheetStru.GatewayInfoLen; i++)
		{
			/*��� 2*/
			dataOff += 2;

			/*�豸����2*/
			DsuGatewaySheetStru.pGatewayInfoStru[i].DeviceName = ShortFromChar(pData + dataOff);
			dataOff += 2;

			/*�豸�������� 4*/
			memcpy(DsuGatewaySheetStru.pGatewayInfoStru[i].IpAddrGatewayRed, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			/*�豸�������� 4*/
			memcpy(DsuGatewaySheetStru.pGatewayInfoStru[i].IpAddrGatewayBlue, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			/*�������� 4*/
			memcpy(DsuGatewaySheetStru.pGatewayInfoStru[i].IpAddrSubnetMask, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{

		}
	}

	/*����ִ��*/
	return chReturnValue;

}

