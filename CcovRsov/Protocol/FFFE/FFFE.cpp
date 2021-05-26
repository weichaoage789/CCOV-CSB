#include "AppDefine.h"
#include "FFFE.h"


unsigned char CFFFE::FFFEInit(FFFE_CFG *pFFFE, unsigned short BufSize)
{
	/*FFFE_CFG状态清零复位*/
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
	unsigned short Pos1 = 0;/*前一个FF的位置*/
	unsigned short Pos2 = 0;/*后一个FF的位置*/
	unsigned short iii = 0;/*临时计数变量*/
	unsigned short Gap = 0;/*两个FF之间的字节数*/
	unsigned char Got1stFF = 0;/*1-获得前一个FF；0-未获得前一个FF*/
	unsigned char Got2ndFF = 0;/*1-获得后一个FF；0-未获得后一个FF*/

	for (SrcPos = 0; SrcPos < Len; SrcPos++)
	{
		if (1 == Got1stFF)
		{
			if (0xFF == pSrc[SrcPos])
			{
				/*已遇到前一个FF，且当前又遇到FF*/
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
				/*已遇到前一个FF，且当前遇到非FF*/
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
			/*尚未遇到前一个FF*/
			pTgt[TgtPos] = pSrc[SrcPos];
			TgtPos++;
			if (0xFF == pSrc[SrcPos])
			{
				/*遇到前一个FF*/
				Got1stFF = 1;
				Pos1 = SrcPos;
				Gap = 0;
			}
			else
			{
			}
		}
	}
	/*已经遇到了前一个FF, 且源数据已到了末尾仍未遇到后一个FF*/
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
	unsigned short NonFFCount = 0;/*进入WAIT_NO_FF状态的次数*/
	unsigned short NextFFPos = 0;/*下一个FF字符的位置*/
	unsigned char Char = 0;/*当前字符值*/
	unsigned char State = WAIT_FF_C1;/*状态*/

	for (SrcPos = 0; SrcPos < Len; SrcPos++)
	{
		Char = pSrc[SrcPos];
		switch (State)
		{
		case WAIT_FF_C1:
			/*等待FF序列第一FF字符*/

			if (0xFF == Char)
			{
				/*收到FF字符*/
				pTgt[TgtPos] = Char;
				TgtPos++;
				State = WAIT_FF_C2;
			}
			else
			{
				/*当前非FF字符*/
				pTgt[TgtPos] = Char;
				TgtPos++;
			}

			break;

		case WAIT_FF_C2:
			/*等待FF序列第二FF字符*/

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
			/*消息中不再有FF字符需要处理，*/

			NonFFCount++;
			if ((0xFF == Char) && (NonFFCount <= 252))
			{
				/*如果在252个字符之内存在FF，解码失败*/
				State = ABNORMAL_STATE;
			}
			else
			{
				/*接受非FF字符*/
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
			/*等待插入FF字符的位置*/

			if (0xFF == Char)
			{
				/*当前字符是FF字符*/

				State = ABNORMAL_STATE;
			}
			else
			{
				/*当前字符非FF字符*/

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
			/*非法状态*/
			TgtPos = 0;
			break;
		}
	}

	/*退出时的状态判断*/
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

	/*运行接收状态机*/
	switch (pFFFE->RcvState)
	{
	case COM_WAIT_START_FF:

		/*等待接收开始FF状态*/
		if (0xFF != RcvChar)
		{

		}
		else
		{
			pFFFE->RcvState = COM_WAIT_START_FE;
		}
		break;
	case COM_WAIT_START_FE:

		/*等待接收开始FE状态*/
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

		/*等待接收停止FF状态*/
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

		/*等待接收停止FD状态*/
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

	/*收到完整帧*/
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

	/*添加FFFE_CFG头*/
	pTgt[PackDataLen] = 0xFF;
	PackDataLen++;
	pTgt[PackDataLen] = 0xFE;
	PackDataLen++;

	/*数据转义*/
	PackDataLen += Encode(pSrc, pTgt + PackDataLen, Len);

	/*添加FFFD尾*/
	pTgt[PackDataLen] = 0xFF;
	PackDataLen++;
	pTgt[PackDataLen] = 0xFD;
	PackDataLen++;

	return PackDataLen;
}