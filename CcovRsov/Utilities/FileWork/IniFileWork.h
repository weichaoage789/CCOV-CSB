/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CfgHelper.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _INI_FILE_WORK_H
#define _INI_FILE_WORK_H


namespace FileWork
{
#define SECTION_MAX_LEN					256
#define STRVALUE_MAX_LEN				256
#define LINE_CONTENT_MAX_LEN		256


	/**
  *-----------------------------------------------------------------------------
  * @brief		 :  Remove formating characters from target string.
  * @param	 :  [in] char *src£¬source string.
  * @return	 :  [type]
  *-----------------------------------------------------------------------------
  */
	void Trim(char *src);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  Read data value from INI file.
	  * @param	 :  [in] char *section.
	  * @param	 :  [in] char *key.
	  * @param	 :  [out] char *val.
	  * @param	 :  [in] char *file.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void IniReadValue(char* section, const char* key, char* val, const char* file);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  Read string value from INI file.
	  * @param	 :  [in] char *section.
	  * @param	 :  [in] char *key.
	  * @param	 :  [out] char *val.
	  * @param	 :  [in] char *file.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool ReadStringValue(const char* section, const char* key, char* val, const char* file);

	/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Read integer value from INI file.
		  * @param	 :  [in] char *section.
		  * @param	 :  [in] char *key.
		  * @param	 :  [in] char *file.
		  * @return	 :  [type] int, the INT value read out.
		  *-----------------------------------------------------------------------------
		  */
	int ReadIntValue(const char* section, const char* key, const char* file);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  write data value to INI file.
	  * @param	 :  [in] const char *section.
	  * @param	 :  [in] const char *key.
	  * @param	 :  [in] const char *val.
	  * @param	 :  [in] const char *file.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void IniWriteValue(const char* section, const char* key, const char* val, const char* file);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  write string value to INI file.
	  * @param	 :  [in] const char *section.
	  * @param	 :  [in] const char *key.
	  * @param	 :  [in] const char *val.
	  * @param	 :  [in] const char *file.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool writeStringVlaue(const char* section, const char* key, const char* val, const char* file);

	/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  write integer value to INI file.
		  * @param	 :  [in] const char *section.
		  * @param	 :  [in] const char *key.
		  * @param	 :  [in] const char val.
		  * @param	 :  [in] const char *file.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
	bool writeIntValue(const char* section, const char* key, const int val, const char* file);
}


#endif  /*_INI_FILE_WORK_H*/
