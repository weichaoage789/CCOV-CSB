#include "AppDefine.h"
#include "FFFE.h"


unsigned char CFFFE::FFFEInit(FFFE_CFG *pFFFE, unsigned short BufSize)
{
	/*FFFE_CFG״̬���㸴λ*/
	pFFFE->RawDataRcvBufferSize = BufSize;
	pFFFE->pRawDataRcvBuf = new unsigned char[BufSize * sizeof(unsigned char)];
	if (pFFFE->pRawDataRcvBuf == nullptr)
	{
		return 0;
	}
	pFFFE->RcvState = COM_WAIT_START_FF;
	pFFFE->GotFrame = 0;
	pFFFE->DataCount = 0;

	return 1;
}

unsigned short CFFFE::Encode(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len)
{
	unsigned short SrcPos = 0;
	unsigned short TgtPos = 0;
	unsigned short Pos1 = 0;/*ǰһ��FF��λ��*/
	unsigned short Pos2 = 0;/*��һ��FF��λ��*/
	unsigned short iii = 0;/*��ʱ��������*/
	unsigned short Gap = 0;/*����FF֮����ֽ���*/
	unsigned char Got1stFF = 0;/*1-���ǰһ��FF��0-δ���ǰһ��FF*/
	unsigned char Got2ndFF = 0;/*1-��ú�һ��FF��0-δ��ú�һ��FF*/

	for (SrcPos = 0; SrcPos < Len; SrcPos++)
	{
		if (1 == Got1stFF)
		{
			if (0xFF == pSrc[SrcPos])
			{
				/*������ǰһ��FF���ҵ�ǰ������FF*/
				Got2ndFF = 1;
				Pos2 = SrcPos;
				pTgt[TgtPos] = Pos2 - Pos1;
				TgtPos++;

				for (iii = Pos1 + 1; iii < Pos2; iii++)
				{
					pTgt[TgtPos] = pSrc[iii];
					TgtPos++;
				}

				Got1stFF = 0;
				Got2ndFF = 0;
				Pos1 = 0;
				Pos2 = 0;
				Gap = 0;
			}
			else
			{
				/*������ǰһ��FF���ҵ�ǰ������FF*/
				Gap++;
				if (252 == Gap)
				{
					Got1stFF = 0;
					Gap = 0;
					pTgt[TgtPos] = 0;
					TgtPos++;
					for (iii = Pos1 + 1; iii <= SrcPos; iii++)
					{
						pTgt[TgtPos] = pSrc[iii];
						TgtPos++;
					}
				}
				else
				{
				}
			}
		}
		else
		{
			/*��δ����ǰһ��FF*/
			pTgt[TgtPos] = pSrc[SrcPos];
			TgtPos++;
			if (0xFF == pSrc[SrcPos])
			{
				/*����ǰһ��FF*/
				Got1stFF = 1;
				Pos1 = SrcPos;
				Gap = 0;
			}
			else
			{
			}
		}
	}
	/*�Ѿ�������ǰһ��FF, ��Դ�����ѵ���ĩβ��δ������һ��FF*/
	if (1 == Got1stFF)
	{
		pTgt[TgtPos] = 0;
		TgtPos++;

		for (iii = Pos1 + 1; iii < Len; iii++)
		{
			pTgt[TgtPos] = pSrc[iii];
			TgtPos++;
		}
	}
	else
	{
	}
	return TgtPos;
}

unsigned short CFFFE::Decode(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len)
{
	unsigned short SrcPos = 0;
	unsigned short TgtPos = 0;
	unsigned short NonFFCount = 0;/*����WAIT_NO_FF״̬�Ĵ���*/
	unsigned short NextFFPos = 0;/*��һ��FF�ַ���λ��*/
	unsigned char Char = 0;/*��ǰ�ַ�ֵ*/
	unsigned char State = WAIT_FF_C1;/*״̬*/

	for (SrcPos = 0; SrcPos < Len; SrcPos++)
	{
		Char = pSrc[SrcPos];
		switch (State)
		{
		case WAIT_FF_C1:
			/*�ȴ�FF���е�һFF�ַ�*/

			if (0xFF == Char)
			{
				/*�յ�FF�ַ�*/
				pTgt[TgtPos] = Char;
				TgtPos++;
				State = WAIT_FF_C2;
			}
			else
			{
				/*��ǰ��FF�ַ�*/
				pTgt[TgtPos] = Char;
				TgtPos++;
			}

			break;

		case WAIT_FF_C2:
			/*�ȴ�FF���еڶ�FF�ַ�*/

			if ((0xFD == Char) || (0xFE == Char) || (0xFF == Char))
			{
				State = ABNORMAL_STATE;
			}
			else
			{
				if (0 == Char)
				{
					State = WAIT_NO_FF;
				}
				else
				{
					if (1 == Char)
					{
						pTgt[TgtPos] = 0xff;
						TgtPos++;
						State = WAIT_FF_C1;
					}
					else
					{
						NextFFPos = SrcPos + Char - 1;
						State = WAIT_FF_POS;
					}
				}
			}
			break;

		case WAIT_NO_FF:
			/*��Ϣ�в�����FF�ַ���Ҫ����*/

			NonFFCount++;
			if ((0xFF == Char) && (NonFFCount <= 252))
			{
				/*�����252���ַ�֮�ڴ���FF������ʧ��*/
				State = ABNORMAL_STATE;
			}
			else
			{
				/*���ܷ�FF�ַ�*/
				pTgt[TgtPos] = Char;
				TgtPos++;

				if (252 == NonFFCount)
				{
					NonFFCount = 0;
					State = WAIT_FF_C1;
				}
				else
				{
				}
			}
			break;

		case WAIT_FF_POS:
			/*�ȴ�����FF�ַ���λ��*/

			if (0xFF == Char)
			{
				/*��ǰ�ַ���FF�ַ�*/

				State = ABNORMAL_STATE;
			}
			else
			{
				/*��ǰ�ַ���FF�ַ�*/

				pTgt[TgtPos] = Char;
				TgtPos++;
				if (SrcPos == NextFFPos)
				{
					pTgt[TgtPos] = 0xff;
					TgtPos++;
					State = WAIT_FF_C1;
				}
				else
				{
				}
			}
			break;

		default:
			State = ABNORMAL_STATE;
			break;
		}

		if (State == ABNORMAL_STATE)
		{
			/*�Ƿ�״̬*/
			TgtPos = 0;
			break;
		}
	}

	/*�˳�ʱ��״̬�ж�*/
	if ((State == WAIT_FF_C2) || (State == WAIT_FF_POS))
	{
		TgtPos = 0;
	}

	return TgtPos;
}

unsigned short CFFFE::Recv(FFFE_CFG *pFFFE, unsigned char RcvChar, unsigned char *pData)
{
	unsigned short ReturnVal = 0;
	unsigned short iii = 0;

	/*���н���״̬��*/
	switch (pFFFE->RcvState)
	{
	case COM_WAIT_START_FF:

		/*�ȴ����տ�ʼFF״̬*/
		if (0xFF != RcvChar)
		{

		}
		else
		{
			pFFFE->RcvState = COM_WAIT_START_FE;
		}
		break;
	case COM_WAIT_START_FE:

		/*�ȴ����տ�ʼFE״̬*/
		if (0xFE != RcvChar)
		{
			pFFFE->RcvState = COM_WAIT_START_FF;
		}
		else
		{
			pFFFE->DataCount = 0;
			pFFFE->GotFrame = 0;
			pFFFE->RcvState = COM_WAIT_STOP_FF;
		}
		break;
	case COM_WAIT_STOP_FF:

		/*�ȴ�����ֹͣFF״̬*/
		if (0xFF == RcvChar)
		{
			pFFFE->RcvState = COM_WAIT_STOP_FD;
		}
		else
		{

			if (pFFFE->DataCount >= pFFFE->RawDataRcvBufferSize)
			{
				pFFFE->DataCount = 0;
				pFFFE->GotFrame = 0;
				pFFFE->RcvState = COM_WAIT_START_FF;
			}
			else
			{
				pFFFE->pRawDataRcvBuf[pFFFE->DataCount] = RcvChar;
				pFFFE->DataCount++;

			}
		}
		break;
	case COM_WAIT_STOP_FD:

		/*�ȴ�����ֹͣFD״̬*/
		switch (RcvChar)
		{
		case 0xFF:
			pFFFE->DataCount = 0;
			pFFFE->GotFrame = 0;
			pFFFE->RcvState = COM_WAIT_START_FF;
			break;
		case 0xFE:
			pFFFE->DataCount = 0;
			pFFFE->GotFrame = 0;
			pFFFE->RcvState = COM_WAIT_STOP_FF;
			break;
		case 0xFD:
			pFFFE->GotFrame = 1;
			pFFFE->RcvState = COM_WAIT_START_FF;
			break;
		default:

			if (pFFFE->DataCount >= (pFFFE->RawDataRcvBufferSize - 1))
			{
				pFFFE->DataCount = 0;
				pFFFE->GotFrame = 0;
				pFFFE->RcvState = COM_WAIT_START_FF;
			}
			else
			{
				pFFFE->pRawDataRcvBuf[pFFFE->DataCount] = 0xFF;
				pFFFE->DataCount++;
				pFFFE->pRawDataRcvBuf[pFFFE->DataCount] = RcvChar;
				pFFFE->DataCount++;
				pFFFE->RcvState = COM_WAIT_STOP_FF;
			}
			break;
		}
		break;
	default:
		break;
	}

	/*�յ�����֡*/
	if (1 == pFFFE->GotFrame)
	{
		for (iii = 0; iii < pFFFE->DataCount; iii++)
		{
			pData[iii] = pFFFE->pRawDataRcvBuf[iii];
		}

		ReturnVal = pFFFE->DataCount;
		pFFFE->DataCount = 0;
		pFFFE->GotFrame = 0;
	}
	else
	{
	}

	return ReturnVal;
}

void CFFFE::Free(FFFE_CFG *pFFFE)
{
	if (pFFFE)
	{
		delete[] pFFFE->pRawDataRcvBuf;
		pFFFE->pRawDataRcvBuf = nullptr;
	}
}

unsigned short CFFFE::Pack(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len)
{
	unsigned short PackDataLen = 0;

	/*���FFFE_CFGͷ*/
	pTgt[PackDataLen] = 0xFF;
	PackDataLen++;
	pTgt[PackDataLen] = 0xFE;
	PackDataLen++;

	/*����ת��*/
	PackDataLen += Encode(pSrc, pTgt + PackDataLen, Len);

	/*���FFFDβ*/
	pTgt[PackDataLen] = 0xFF;
	PackDataLen++;
	pTgt[PackDataLen] = 0xFD;
	PackDataLen++;

	return PackDataLen;
}