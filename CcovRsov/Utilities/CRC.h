/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CRC.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CRC_H
#define _CRC_H

#ifdef __cplusplus
extern "C" {
#endif


#define BITMASK(X) ((unsigned int)1 << (X))


	typedef struct {
		unsigned char order;
		unsigned char refin;
		unsigned char refout;
		unsigned int poly;
		unsigned int crcinit;
		unsigned int crcxor;
		unsigned int crcmask;
		unsigned int crchighbit;
		unsigned int crctab[256];
	} CRC_CFG;


	/**
	 *-----------------------------------------------------------------------------
     * @brief		 :  reflect the bitnum bits of the source data.
     * @param	     :  [in & out] unsigned int *src, input & output data.
     * @param	     :  [in] const unsigned int bitnum, number of bits to reverse.
     * @return	     :  [type]
     *-----------------------------------------------------------------------------
	 */
	void Reflect(unsigned int *src, const unsigned int bitnum);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		  :  general CRC table generating function.
	  * @param	  :  [in] CRC_CFG &cfg, input CRC config info.
	  * @return	  :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void GenerateCrcTable(CRC_CFG *cfg);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  CRC initial function.
	  * @param	 :  [in] CRC_CFG *cfg, input CRC config info.
	  * @param	 :  [in] unsigned char order, 生成多项式的阶数.
	  * @param	 :  [in] unsigned int poly, 生成多项式.
	  * @param	 :  [in] unsigned int crcinit, 寄存器初始值.
	  * @param	 :  [in] unsigned int crcxor,  异或系数，用于在CRC返回前进行异或，得到最终的CRC.
	  * @param	 :  [in] unsigned char refin, 输入字节反转标志。0 -- 不反转；1 -- 反转.
	  * @param	 :  [in] unsigned char refout, 在进行异或前CRC反转标志。0 -- 不反转；1 -- 反转.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  * @note                 :  When using multiple types of CRC, the CRC_CFG structure needs to be defined and initialized separately
	  * @CRC_CCITT      : order = 16;  poly=0x1021;          crcinit = 0xFFFF;           crcxor=0;                      refin =0;   refout=0;
	  * @CRC16              : order = 16;  poly=0x8005;          crcinit = 0x0;                 crcxor=0x0;                  refin =1;   refout=1;
	  * @CRC32              : order = 32;  poly=0x4c11db7;    crcinit = 0xFFFFFFFF;   crcxor=0xFFFFFFFF;    refin =1;   refout=1;
	  */
	void Init(CRC_CFG *cfg, unsigned char order, unsigned int poly, unsigned int crcinit, unsigned int crcxor, unsigned char refin, unsigned char refout);

	/**
      *-----------------------------------------------------------------------------
      * @brief		 :  CRC16 verification function.
      * @param	 :  [in] const unsigned char *data, input source data.
      * @param	 :  [in] const unsigned int len, input source data length.
      * @param	 :  [in] CRC_CFG *cfg, input CRC config info.
      * @return	 :  [type] unsigned short, output the verification result.
      *-----------------------------------------------------------------------------
      * @note       :  Before calling this function, you need to call the Init() function to initialize cfg.
      */
	unsigned int Crc(const unsigned char *data, const unsigned int len, CRC_CFG *cfg);

	/**
      *-----------------------------------------------------------------------------
      * @brief		 :  CRC16 verification function.
      * @param	 :  [in] const unsigned char *data, input source data.
      * @param	 :  [in] const unsigned int len, input source data length.
      * @return	 :  [type] unsigned short, output the verification result.
      *-----------------------------------------------------------------------------
      */
	unsigned short Crc16(const unsigned char *data, const unsigned int len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  CRC32 verification function.
	  * @param	 :  [in] const unsigned char *data, input source data.
	  * @param	 :  [in] const unsigned int len, input source data length.
	  * @param	 :  [in] unsigned int caclnum, input verification times ( 0 for single check).
	  * @param	 :  [out] unsigned int *crcrslt, output the verification result.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	int Crc_32(const unsigned char *data, const unsigned int len, unsigned int caclnum, unsigned int *crcrslt);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  CRC32 verification function.
	  * @param	 :  [in] const unsigned char *data, input source data.
	  * @param	 :  [in] const unsigned int len, input source data length.
	  * @return	 :  [type] unsigned int, 
	  *-----------------------------------------------------------------------------
	  */
	unsigned int Crc32(const unsigned char *data, const unsigned int len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  CRC32 verification function for big file.
	  * @param	 :  [in] const unsigned char *data, input source data.
	  * @param	 :  [in] const unsigned int len, input source data length.
	  * @param	 :  [in] unsigned int caclnum, input verification times ( 0 for single check).
	  * @param	 :  [out] unsigned int *crcrslt, output the verification result.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	unsigned char BigFileCrc32(const unsigned char *data, const unsigned int len, unsigned char caclnum, unsigned int *crcrslt);


#ifdef __cplusplus
}
#endif

#endif  /*_CRC_H*/