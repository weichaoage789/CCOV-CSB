#include "DataLoad.h"


UINT8 dataReadTemp(CHAR *dataName, DQU_READ_DATA_STOR_STRU *FsStorStu)
{
	UINT32 CRCresult = 0;   /*校验码*/
	UINT32  pDataOffset = DFS_DATA_HEAD_SIZE;
	UINT32 i = 0;
	FILE *pfile = NULL;
	UINT8  rtnValue = 2; /*默认返回值,分步处理*/
	UINT8  funcRtn = 0; /*函数返回值*/
	static UINT8 MainIndex = 0; /*主控制流程索引*/
	static UINT32 loadCrcCode = 0; /*生成的CRC效验码*/
	static UINT32 FileLengths = 0; /*文件的长度*/
	UINT8 *pData = FsStorStu->DataBuf;

	DH  dh;
	volatile UINT8 *pDataAddr;
	pDataAddr = (volatile UINT8 *)dataName;

	/*入口参数检查*/
	if ((NULL != dataName) && (NULL != FsStorStu) && (NULL != FsStorStu->DataBuf) && (NULL != FsStorStu->DfsDH))
	{
		if (0 == MainIndex)
		{
#if CBTC_WINDOWS

			/*读取文件*/
			pfile = fopen(dataName, "rb");
			if (0 == pfile)
			{
				/*打开文件失败，例如找不到文件（sjg）*/
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
				return rtnValue;
			}

			FileLengths = fread(FsStorStu->DataBuf, sizeof(UINT8), DFS_MAX_FILE_LEN, pfile);
			if ((0 == FileLengths) || (DFS_MAX_FILE_LEN <= FileLengths))
			{
				/*printf("读文件失败！");*/
				fileErrNo |= ERR_FILE_LENTH;
				fclose(pfile);
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*读取成功，继续执行*/
				DHFromByte(&dh, pData);
				FileLengths = dh.dataLength;
			}
			fclose(pfile);

#elif CBTC_VXWORKS

			/*读取文件*/
			pfile = fopen(dataName, "rb");
			if (0 == pfile)
			{
				/*读取文件失败，例如找不到文件（sjg）*/
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
				return rtnValue;
			}

			FileLengths = fread(FsStorStu->DataBuf, sizeof(UINT8), DFS_MAX_FILE_LEN, pfile);
			if (0 == FileLengths)
			{
				/*printf("读文件失败！");*/
				fileErrNo |= ERR_FILE_LENTH;
				fclose(pfile);
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*读取成功，继续执行*/
				DHFromByte(&dh, pData);
				FileLengths = dh.dataLength;
			}
			fclose(pfile);

#elif CBTC_MOTOROLA 

			/*先读取一个文件头，获取文件长度*/
			for (i = 0; i < DFS_DATA_HEAD_SIZE; i++)
			{
				FsStorStu->DataBuf[i] = *pDataAddr;
				pDataAddr++;
			}

			/*解析文件头*/
			DHFromByte(&dh, pData);
			FileLengths = dh.dataLength;

			/*判定文件头长度有效性*/
			if (DFS_MAX_FILE_LEN <= FileLengths)
			{
				/*数据文件太长*/
				fileErrNo |= LOAD_FILE_LENGTH_ERR;
				rtnValue = 0;
				MainIndex = 4;
			}
			else
			{
				MainIndex = 1;

				/*读取数据*/
				for (i = DFS_DATA_HEAD_SIZE; i < FileLengths; i++)
				{
					FsStorStu->DataBuf[i] = *pDataAddr;
					pDataAddr++;
				}
			}
#endif
		}

		/*一次性读取*/
		if (READ_MODE_ONCE == FsStorStu->ReadMode)
		{
			/*CRC计算,前4个字节是CRC，因此，计算的时候从第4个字节开始*/
			CRCresult = Crc32(&FsStorStu->DataBuf[4], FileLengths - 4);;

			/*CRC校验*/
			if (CRCresult != LongFromChar(&FsStorStu->DataBuf[0]))
			{
				/*printf("校验失败，版本号不一致！");*/
				fileErrNo |= ERR_CRC_CHECK;
				rtnValue = 0;
			}
			else
			{
				/*解析数据头*/
				DHFromByte(&FsStorStu->DfsDH->dataHead, pData);
				/*解析数据索引*/
				for (i = 0; i < FsStorStu->DfsDH->dataHead.indexNumber; i++)
				{
					DIFromByte(&FsStorStu->DfsDH->dataIndex[i], pData + pDataOffset);
					pDataOffset += DFS_DATA_INDEX_SIZE;
				}

				rtnValue = 1;
			}

			/*函数处理失败*/
			MainIndex = 4;

		}
		else if (READ_MODE_TIMES == FsStorStu->ReadMode)
		{
			if (1 == MainIndex)
			{
				/*分步读取数据*/
				funcRtn = ReadCycle(dataName, FsStorStu->DataBuf, FsStorStu->ReadLength, FsStorStu->ReadMode, FileLengths);
				if (0x55 == funcRtn)
				{
					MainIndex = 2;
				}
				else if (0xaa == funcRtn)
				{
					/*函数处理失败*/
					MainIndex = 4;
					fileErrNo |= ERR_FILE_READ;
					rtnValue = 0;
				}
				else
				{
					/*继续分步处理*/
				}
			}
			else if (2 == MainIndex)
			{
				/*生成CRC效验码*/
				funcRtn = BigFileCrc32(&FsStorStu->DataBuf[4], FileLengths - 4, 200, &loadCrcCode);
				if (0x55 == funcRtn)
				{
					MainIndex = 3;
				}
				else if (0xaa == funcRtn)
				{
					/*函数处理失败*/
					MainIndex = 4;
					rtnValue = 0;
				}
				else
				{
					/*继续分步处理*/
				}
			}
			else if (3 == MainIndex)
			{
				/*CRC校验*/
				if (loadCrcCode != LongFromChar(&FsStorStu->DataBuf[0]))
				{
					/*函数处理失败*/
					fileErrNo |= ERR_CRC_CHECK;
					rtnValue = 0;
				}
				else
				{
					/*解析数据头*/
					DHFromByte(&FsStorStu->DfsDH->dataHead, pData);
					/*解析数据索引*/
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

		/*恢复本函数初值*/
		if (4 == MainIndex)
		{
			loadCrcCode = 0;
			FileLengths = 0;
			MainIndex = 0;
		}
	}
	else
	{
		/*返回失败*/
		rtnValue = 0;
	}

	return rtnValue;
}