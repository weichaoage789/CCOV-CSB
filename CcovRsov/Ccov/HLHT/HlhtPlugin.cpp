#include "AppDefine.h"
#include "Utilities/Convert/ConvertWork.h"

#include "dfsDataRead.h"
#include "vobcInternalTelegramDef.h"
#include "HlhtPlugin.h"


void CHLHTCtrl::SubsysDataHandle(unsigned char *data, unsigned char systype)
{
	if (nullptr != data)
	{
		CCOV_FileInfo *info = nullptr;

		switch (systype)
		{
		case CCOV_AND_ATP1_TYPE:
			//_Atp1FInfo.Vdsu_status = ;
			info = &_Atp1FInfo;
			break;

		case CCOV_AND_ATP2_TYPE:
			info = &_Atp2FInfo;
			break;

		case CCOV_AND_ATP3_TYPE:
			info = &_Atp3FInfo;
			break;

		case CCOV_AND_ATO1_TYPE:
			info = &_Ato1FInfo;
			break;

		case CCOV_AND_ATO2_TYPE:
			info = &_Ato2FInfo;
			break;

		default:
			break;
		}

		//vSubSystemRecvProcess(data, systype, info);
		if (DATA_DOWNLOAD_SUCESS != info->File_SuccessFlag && DATA_DOWNLOAD_SUCESS != _AtpDLoadState)
		{
			/*准备CCOV向VDSU请求的数据信息*/
			//vCcovToVdsuPrepare(info, &vAtpAskToVdsuMsg, &vAtpAskTag);
			if (0x02 == _AtpDLoadState || 0x03 == _AtoDLoadState)
			{
				info->File_SuccessFlag = _AtpDLoadState;
			}
			else
			{
				info->File_SuccessFlag = 0x02;
			}
		}
	}
}