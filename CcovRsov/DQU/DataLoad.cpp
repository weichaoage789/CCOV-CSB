#include "DataLoad.h"


UINT8 dataReadTemp(CHAR *dataName, DQU_READ_DATA_STOR_STRU *FsStorStu)
{
	UINT32 CRCresult = 0;   /*У����*/
	UINT32  pDataOffset = DFS_DATA_HEAD_SIZE;
	UINT32 i = 0;
	FILE *pfile = NULL;
	UINT8  rtnValue = 2; /*Ĭ�Ϸ���ֵ,�ֲ�����*/
	UINT8  funcRtn = 0; /*��������ֵ*/
	static UINT8 MainIndex = 0; /*��������������*/
	static UINT32 loadCrcCode = 0; /*���ɵ�CRCЧ����*/
	static UINT32 FileLengths = 0; /*�ļ��ĳ���*/
	UINT8 *pData = FsStorStu->DataBuf;

	DH  dh;
	volatile UINT8 *pDataAddr;
	pDataAddr = (volatile UINT8 *)dataName;

	/*��ڲ������*/
	if ((NULL != dataName) && (NULL != FsStorStu) && (NULL != FsStorStu->DataBuf) && (NULL != FsStorStu->DfsDH))
	{
		if (0 == MainIndex)
		{
#if CBTC_WINDOWS

			/*��ȡ�ļ�*/
			pfile = fopen(dataName, "rb");
			if (0 == pfile)
			{
				/*���ļ�ʧ�ܣ������Ҳ����ļ���sjg��*/
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
				return rtnValue;
			}

			FileLengths = fread(FsStorStu->DataBuf, sizeof(UINT8), DFS_MAX_FILE_LEN, pfile);
			if ((0 == FileLengths) || (DFS_MAX_FILE_LEN <= FileLengths))
			{
				/*printf("���ļ�ʧ�ܣ�");*/
				fileErrNo |= ERR_FILE_LENTH;
				fclose(pfile);
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*��ȡ�ɹ�������ִ��*/
				DHFromByte(&dh, pData);
				FileLengths = dh.dataLength;
			}
			fclose(pfile);

#elif CBTC_VXWORKS

			/*��ȡ�ļ�*/
			pfile = fopen(dataName, "rb");
			if (0 == pfile)
			{
				/*��ȡ�ļ�ʧ�ܣ������Ҳ����ļ���sjg��*/
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
				return rtnValue;
			}

			FileLengths = fread(FsStorStu->DataBuf, sizeof(UINT8), DFS_MAX_FILE_LEN, pfile);
			if (0 == FileLengths)
			{
				/*printf("���ļ�ʧ�ܣ�");*/
				fileErrNo |= ERR_FILE_LENTH;
				fclose(pfile);
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*��ȡ�ɹ�������ִ��*/
				DHFromByte(&dh, pData);
				FileLengths = dh.dataLength;
			}
			fclose(pfile);

#elif CBTC_MOTOROLA 

			/*�ȶ�ȡһ���ļ�ͷ����ȡ�ļ�����*/
			for (i = 0; i < DFS_DATA_HEAD_SIZE; i++)
			{
				FsStorStu->DataBuf[i] = *pDataAddr;
				pDataAddr++;
			}

			/*�����ļ�ͷ*/
			DHFromByte(&dh, pData);
			FileLengths = dh.dataLength;

			/*�ж��ļ�ͷ������Ч��*/
			if (DFS_MAX_FILE_LEN <= FileLengths)
			{
				/*�����ļ�̫��*/
				fileErrNo |= LOAD_FILE_LENGTH_ERR;
				rtnValue = 0;
				MainIndex = 4;
			}
			else
			{
				MainIndex = 1;

				/*��ȡ����*/
				for (i = DFS_DATA_HEAD_SIZE; i < FileLengths; i++)
				{
					FsStorStu->DataBuf[i] = *pDataAddr;
					pDataAddr++;
				}
			}
#endif
		}

		/*һ���Զ�ȡ*/
		if (READ_MODE_ONCE == FsStorStu->ReadMode)
		{
			/*CRC����,ǰ4���ֽ���CRC����ˣ������ʱ��ӵ�4���ֽڿ�ʼ*/
			CRCresult = Crc32(&FsStorStu->DataBuf[4], FileLengths - 4);;

			/*CRCУ��*/
			if (CRCresult != LongFromChar(&FsStorStu->DataBuf[0]))
			{
				/*printf("У��ʧ�ܣ��汾�Ų�һ�£�");*/
				fileErrNo |= ERR_CRC_CHECK;
				rtnValue = 0;
			}
			else
			{
				/*��������ͷ*/
				DHFromByte(&FsStorStu->DfsDH->dataHead, pData);
				/*������������*/
				for (i = 0; i < FsStorStu->DfsDH->dataHead.indexNumber; i++)
				{
					DIFromByte(&FsStorStu->DfsDH->dataIndex[i], pData + pDataOffset);
					pDataOffset += DFS_DATA_INDEX_SIZE;
				}

				rtnValue = 1;
			}

			/*��������ʧ��*/
			MainIndex = 4;

		}
		else if (READ_MODE_TIMES == FsStorStu->ReadMode)
		{
			if (1 == MainIndex)
			{
				/*�ֲ���ȡ����*/
				funcRtn = ReadCycle(dataName, FsStorStu->DataBuf, FsStorStu->ReadLength, FsStorStu->ReadMode, FileLengths);
				if (0x55 == funcRtn)
				{
					MainIndex = 2;
				}
				else if (0xaa == funcRtn)
				{
					/*��������ʧ��*/
					MainIndex = 4;
					fileErrNo |= ERR_FILE_READ;
					rtnValue = 0;
				}
				else
				{
					/*�����ֲ�����*/
				}
			}
			else if (2 == MainIndex)
			{
				/*����CRCЧ����*/
				funcRtn = BigFileCrc32(&FsStorStu->DataBuf[4], FileLengths - 4, 200, &loadCrcCode);
				if (0x55 == funcRtn)
				{
					MainIndex = 3;
				}
				else if (0xaa == funcRtn)
				{
					/*��������ʧ��*/
					MainIndex = 4;
					rtnValue = 0;
				}
				else
				{
					/*�����ֲ�����*/
				}
			}
			else if (3 == MainIndex)
			{
				/*CRCУ��*/
				if (loadCrcCode != LongFromChar(&FsStorStu->DataBuf[0]))
				{
					/*��������ʧ��*/
					fileErrNo |= ERR_CRC_CHECK;
					rtnValue = 0;
				}
				else
				{
					/*��������ͷ*/
					DHFromByte(&FsStorStu->DfsDH->dataHead, pData);
					/*������������*/
					for (i = 0; i < FsStorStu->DfsDH->dataHead.indexNumber; i++)
					{
						DIFromByte(&FsStorStu->DfsDH->dataIndex[i], pData + pDataOffset);
						pDataOffset += DFS_DATA_INDEX_SIZE;
					}

					rtnValue = 1;
				}

				MainIndex = 4;
			}
		}
		else
		{
			rtnValue = 0;
			MainIndex = 4;
		}

		/*�ָ���������ֵ*/
		if (4 == MainIndex)
		{
			loadCrcCode = 0;
			FileLengths = 0;
			MainIndex = 0;
		}
	}
	else
	{
		/*����ʧ��*/
		rtnValue = 0;
	}

	return rtnValue;
}