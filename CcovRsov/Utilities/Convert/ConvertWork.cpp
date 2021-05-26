#include "AppDefine.h"
#include "ConvertWork.h"


namespace Convert
{
	unsigned short char_to_short(const unsigned char *data)
	{
		return (((*data) << 8) + *(data + 1));
	}

	void short_to_char(const unsigned short data, unsigned char *dest)
	{
		*dest = (data >> 8) & 0xff;
		*(dest+1) = data & 0xff;
	}

	unsigned int char_to_long(const unsigned char *data)
	{
		return ((*data) << 24) + ((*(data + 1)) << 16) + ((*(data + 2)) << 8) + (*(data + 3));
	}

	void long_to_char(const unsigned int data, unsigned char *dest)
	{
		*dest = (unsigned char)((data >> 24) & 0xff);
		*(dest + 1) = (unsigned char)((data >> 16) & 0xff);
		*(dest + 2) = (unsigned char)((data >> 8) & 0xff);
		*(dest + 3) = (unsigned char)(data & 0xff);
	}

	unsigned short char_to_short_LE(const unsigned char *data)
	{
		return (((*(data + 1)) << 8) + (*data));
	}

	void short_to_char_LE(const unsigned short data, unsigned char *dest)
	{
		*(dest + 1) = (data >> 8) & 0xff;
		*dest = data & 0xff;
	}

	unsigned int char_to_long_LE(const unsigned char *data)
	{
		return ((*(data + 3)) << 24) + ((*(data + 2)) << 16) + ((*(data + 1)) << 8) + (*data);
	}

	void long_to_char_LE(const unsigned int data, unsigned char *dest)
	{
		*(dest + 3) = (unsigned char)((data >> 24) & 0xff);
		*(dest + 2) = (unsigned char)((data >> 16) & 0xff);
		*(dest + 1) = (unsigned char)((data >> 8) & 0xff);
		*dest = (unsigned char)(data & 0xff);
	}


	bool ip_string_to_array(const char *src, unsigned char *dest)
	{
		if (nullptr != src && nullptr != dest && 4 <= sizeof((char *)dest))
		{
			char tmp[16] = { 0 };
			memcpy(tmp, src, sizeof(tmp));

			char* szBufTemp = nullptr;
			char* szBuf = strtok_r(tmp, _IP_MARK, &szBufTemp);

			int i = 0;
			while (nullptr != szBuf)
			{
				dest[i] = (unsigned char)atoi(szBuf);
				szBuf = strtok_r(nullptr, _IP_MARK, &szBufTemp);
				i++;
			}

			return true;
		}
		return false;
	}

	bool ip_array_to_string(const unsigned char *src, char *dest)
	{
		if (nullptr != src && 4 <= sizeof(src) && nullptr != dest)
		{
			sprintf(dest, "%u.%u.%u.%u", src[0], src[1], src[2], src[3]);
			return true;
		}
		return false;
	}

	unsigned int ip_string_to_int(char *ip)
	{
		unsigned int rt = 0;

		if (nullptr != ip)
		{
			char* szBufTemp = nullptr;
			char* szBuf = strtok_r(ip, _IP_MARK, &szBufTemp);

			int i = 0;
			while (nullptr != szBuf)
			{
				rt += atoi(szBuf) << ((3 - i) * 8);
				szBuf = strtok_r(nullptr, _IP_MARK, &szBufTemp);
				i++;
			}
		}

		return rt;
	}

	bool ip_int_to_string(const unsigned int ip, char *dest)
	{
		if (nullptr != dest && 15 <= sizeof(dest))
		{
			unsigned int tmp1 = (ip >> 24) & 0xFF;
			unsigned int tmp2 = (ip >> 16) & 0xFF;
			unsigned int tmp3 = (ip >> 8) & 0xFF;
			unsigned int tmp4 = (ip >> 0) & 0xFF;

			sprintf(dest, "%u.%u.%u.%u", tmp1, tmp2, tmp3, tmp4);
			return true;
		}
		return false;
	}


	bool BitSet32(unsigned int src, unsigned int index, unsigned char value)
	{
		if (index < (sizeof(unsigned int) * 8))
		{
			if ((value == 1) || (value == 0))
			{
				/** ��bitֵ */
				src = ((src | (1 << index)) & (~(!value << index)));
				return true;
			}
		}
		return false;
	}

	bool BitGet32(unsigned int src, unsigned int index, unsigned char &value)
	{
		if (index < (sizeof(unsigned int) * 8))
		{
			/** ��ȡbitֵ */
			value = (src &(1 << index)) >> index;
			return true;
		}
		return false;
	}

	unsigned int EncodeBy0203(const unsigned char *src, const unsigned int len, unsigned char *dest)
	{
		unsigned int j = 0;

		/* ��ӿ�ͷ */
		*(dest + j) = (unsigned char)0x02;
		j++;

		/* ת�� */
		for (unsigned int i = 0; i < len; i++)
		{
			switch (*(src + i))
			{
			case (unsigned char)0x02:
				/*��ǰ02�ַ�*/
				*(dest + j) = (unsigned char)0x10;
				j++;
				*(dest + j) = (unsigned char)0x82;
				break;

			case (unsigned char)0x03:
				/*��ǰ03�ַ�*/
				*(dest + j) = (unsigned char)0x10;
				j++;
				*(dest + j) = (unsigned char)0x83;
				break;

			case (unsigned char)0x10:
				/*��ǰ10�ַ�*/
				*(dest + j) = (unsigned char)0x10;
				j++;
				*(dest + j) = (unsigned char)0x90;
				break;

			default:
				*(dest + j) = *(src + i);
				break;
			}
			j++;
		}

		/* ��ӽ�β */
		*(dest + j) = (unsigned char)0x03;
		j++;

		return j;
	}

	unsigned int DecodeBy0203(const unsigned char *src, const unsigned int len, unsigned char *dest)
	{
		unsigned int j = 0, k = 0;

		while (k <= len)
		{
			if ((unsigned char)0x10 == *(src + k))
			{
				/* �ҳ�0x10���ж��Ƿ����ת�壬������ת�� */

				/*Ų������ָ�뵽��һ������*/
				k++;

				if (k == len)
				{
					/*0x10�����һ���ַ������ش���*/
					return(0);
				}

				if (((unsigned char)0x82 != *(src + k)) && ((unsigned char)0x83 != *(src + k)) && ((unsigned char)0x90 != *(src + k)))
				{
					/* ���0x10���Ƿ�����(0x82, 0x83, 0x90)���ַ����������޷�ת�壬����ת�����*/
					return 0;
				}

				/*���10������82��83��90����Ҫ������ֽڽ���ת��*/
				*(dest + j) = *(src + k) - (unsigned char)0x80;
			}
			else
			{
				/*������ת�壬��ֱ�ӿ�������*/
				*(dest + j) = *(src + k);
			}

			j++;
			k++;
		}

		/*����ת�������ݳ��Ȼ��޷�ת��*/
		return j;
	}
}