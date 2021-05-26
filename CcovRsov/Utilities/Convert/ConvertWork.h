/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  Convert.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CONVERT_H
#define _CONVERT_H


namespace Convert
{
#define _IP_MARK		"."


	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned char *data to unsigned short (BIG Endian).
	  * @param	 :  [in] const unsigned char *data, input data to be converted.
	  * @return	 :  [type] unsigned short, the return value.
	  *-----------------------------------------------------------------------------
	  */
	unsigned short char_to_short(const unsigned char *data);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned short data to unsigned char (BIG Endian).
	  * @param	 :  [in] const unsigned short data, input data to be converted.
	  * @param	 :  [out] unsigned char *dest, data converted output.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void short_to_char(const unsigned short data, unsigned char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned char *data to unsigned int (BIG Endian).
	  * @param	 :  [in] const unsigned char *data, input data to be converted.
	  * @return	 :  [type] unsigned int, the return value.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int char_to_long(const unsigned char *data);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned int data to unsigned char (BIG Endian).
	  * @param	 :  [in] const unsigned int data, input data to be converted.
	  * @param	 :  [out] unsigned char *dest, data converted output.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void long_to_char(const unsigned int data, unsigned char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned char *data to unsigned short (LITTLE Endian).
	  * @param	 :  [in] const unsigned char *data, input data to be converted.
	  * @return	 :  [type] unsigned short, the return value.
	  *-----------------------------------------------------------------------------
	  */
	unsigned short char_to_short_LE(const unsigned char *data);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned short data to unsigned char (LITTLE Endian).
	  * @param	 :  [in] const unsigned short data, input data to be converted.
	  * @param	 :  [out] unsigned char *dest, data converted output.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void short_to_char_LE(const unsigned short data, unsigned char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned char *data to unsigned int (LITTLE Endian).
	  * @param	 :  [in] const unsigned char *data, input data to be converted.
	  * @return	 :  [type] unsigned int, the return value.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int char_to_long_LE(const unsigned char *data);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert unsigned int data to unsigned char (LITTLE Endian).
	  * @param	 :  [in] const unsigned int data, input data to be converted.
	  * @param	 :  [out] unsigned char *dest, data converted output.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void long_to_char_LE(const unsigned int data, unsigned char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  set bit-index to a specific value of a INT value.
	  * @param	 :  [in] unsigned int &src, input source data to be converted.
	  * @param	 :  [in] unsigned int index, index of bit of the INT.
	  * @param	 :  [in] unsigned char value, input data to set.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool BitSet32(unsigned int src, unsigned int index, unsigned char value);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get the specific value of target bit-index of a INT value.
	  * @param	 :  [in] unsigned int &src, input source data.
	  * @param	 :  [in] unsigned int index, index of bit of the INT.
	  * @param	 :  [out] unsigned char value, output data got.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool BitGet32(unsigned int src, unsigned int index, unsigned char &value);


	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert ip from string values to a array.
	  * @param	 :  [int] const char *src, ip string value.
	  * @param	 :  [out] unsigned char *dest, dest array of ip converted.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool ip_string_to_array(const char *src, unsigned char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert ip from array values to a string.
	  * @param	 :  [int] unsigned char *src, ip array value.
	  * @param	 :  [out] char *dest, dest string of ip converted.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool ip_array_to_string(const unsigned char *src, char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert ip from a string value to four integers and put them on the four bytes corresponding to a integer.
	  * @param	 :  [int] char *ip, ip string value.
	  * @return	 :  [type] unsigned int, integer value of ip converted.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int ip_string_to_int(char *ip);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  convert ip from 4 values placed on the 4 bytes corresponding to the integer into a string.
	  * @param	 :  [int] unsigned int ip, ip integer value.
	  * @param	 :  [out] char *dest, dest string of ip converted.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool ip_int_to_string(const unsigned int ip, char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  encode the target data by 0203.
	  * @param	 :  [in] const unsigned char *src, input source data to be encode.
	  * @param	 :  [in] const unsigned int len, input source data length.
	  * @param	 :  [out] unsigned char *dest, output dest data after encoding.
	  * @return	 :  [type] unsigned int, length of the data after encoding.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int EncodeBy0203(const unsigned char *src, const unsigned int len, unsigned char *dest);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  decode the target data by 0203.
	  * @param	 :  [in] const unsigned char *src, input source data to be encode.
	  * @param	 :  [in] const unsigned int len, input source data length.
	  * @param	 :  [out] unsigned char *dest, output dest data after decoding.
	  * @return	 :  [type] unsigned int, length of the data after decoding (0 means error).
	  *-----------------------------------------------------------------------------
	  */
	unsigned int DecodeBy0203(const unsigned char *src, const unsigned int len, unsigned char *dest);
}


#endif /*_CONVERT_H*/
