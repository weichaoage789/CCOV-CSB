/************************************************************************
*@ �ļ���	    �� FFFE_CFG.h
*@ ��Ȩ˵��	��
*@ �汾��	    �� 1.0
*@ ����ʱ��	�� 2020.4
*@ ����		    �� HHY
*@ ��������	��FFFE_CFG��
*@ ʹ��ע��	��
*@ �޸ļ�¼	��

************************************************************************/

#ifndef _FFFE_H
#define _FFFE_H


/*FFFE_CFG��ת��״̬*/
#define WAIT_FF_C1 0x00
#define WAIT_FF_C2 0x01
#define WAIT_NO_FF 0x02
#define WAIT_FF_POS 0x03
#define ABNORMAL_STATE 0x04

/*FFFE_CFG���ݽ���״̬*/
#define COM_WAIT_START_FF 0x00
#define COM_WAIT_START_FE 0x01
#define COM_WAIT_STOP_FF 0x02
#define COM_WAIT_STOP_FD 0x03


typedef struct
{
	unsigned char *pRawDataRcvBuf;/*δ��ת�����ݽ��ջ�����*/
	unsigned short RawDataRcvBufferSize;/*δ��ת�����ݽ��ջ�������С*/
	unsigned char RcvState;/*FFFE_CFG���ݽ���״̬*/
	unsigned char GotFrame;/*FFFE_CFG�Ƿ��յ�������֡*/
	unsigned short DataCount;/* �յ�����֮֡ǰ�����յ���FFFE_CFG�����ܳ��ȣ�ÿ���յ�����֡������*/
} FFFE_CFG;


class CFFFE
{
public:
	static CFFFE &GetInstance()
	{
		static CFFFE instance;
		return instance;
	}

	/*
	* ����������  FFFE_CFG�ṹ���ʼ������
	* ���������  unsigned short FFFEBufferSize, ���ڽ���һ֡���ݵĻ���������
	*             PORT_STATE_STRU *pFFFEStatStru, FFFE_CFG״̬�ṹ��
	* ����ֵ��    0,����ִ��ʧ��
	*             1,����ִ�гɹ�
	*/
	unsigned char FFFEInit(FFFE_CFG *pFFFE, unsigned short BufSize);

	/*
	* ���������� �����ݽ���FFFE_CFGת�崦������ͷFFFE_CFG��βFFFD����
	* ��������� const unsigned char *pSrc    ����(��FFFE_CFGת��)
	*            unsigned short SrcLen        ����(��FFFE_CFGת��)�ĳ���
	* ��������� unsigned char *pTgt          ����(����FFFE_CFGת��)
	* ����ֵ��   ת���ĳ���
	*/
	unsigned short Encode(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len);

	/*
	* ����������  ������FFFE_CFGת�崦�������ݣ��Ѿ�ȥ����ͷFFFE_CFG��βFFFD����ԭΪԭʼ����
	* ���������  const unsigned char *pSrc,    ����(FFFE_CFGת��)
	*             unsigned short SrcLen,        ����(FFFE_CFGת��)�ĳ���
	* ���������  unsigned char *pTgt,          ����(ȥ��FFFE_CFGת��)
	* ����ֵ��    ����0,  ת��ɹ�,����ת�������ݳ���
	*             0,   �в���ת����ַ���ת��ʧ�ܡ�����ת�������ݳ���Ϊ0
	*/
	unsigned short Decode(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len);

	/*
	* ����������   ���ַ����մ���ת������ݣ������֡FFFE_CFG���ģ�����FFFE_CFG��FFFD����
	* ���������   unsigned char RcvChar,      ��ǰ���յ��ַ�
	*              PORT_STATE_STRU *pFFFEStatStru,
	* ���������   unsigned char *pRawDataRcv,  ���ݽ��ջ�����ָ��
	* ����ֵ��     ����0,  ��ʾ�յ�����֡�����ݳ��ȣ������յ�������֡���������ջ�������
	*              0,   δ�յ���֡���ݣ�
	*/

	unsigned short Recv(FFFE_CFG *pFFFE, unsigned char RcvChar, unsigned char *pData);

	/*
	* ����������  FFFE_CFG״̬������
	* ���������  PORT_STATE_STRU *pFFFEStatStru, FFFE_CFG״̬�ṹ��
	* ����ֵ��
	*/
	void Free(FFFE_CFG *pFFFE);

	/*
	* ���������� �����ݽ���FFFE_CFGת�崦��,��ͷFFFE_CFG��βFFFD
	* ��������� const unsigned char *pSrc    ����(��FFFE_CFGת��)
	*            unsigned short SrcLen        ����(��FFFE_CFGת��)�ĳ���
	* ��������� unsigned char *pTgt          ����(����FFFE_CFGת��),�����FFFE_CFGͷ��FFFDβ
	* ����ֵ��   ת���ĳ���
	*/
	unsigned short Pack(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len);

private:
	CFFFE() {}
};


#endif