/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  ZipWork.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _ZIP_WORK_H
#define _ZIP_WORK_H

#include "Utilities/FileWork/zlib/zlib.h"

#define SIZE_CHUNK 10240


namespace FileWork
{
/** @check err code, print an error message and return Z_DATA_ERROR. */
#define CHECK_ERR(err, msg) {											\
    if (err != Z_OK) {																\
        fprintf(stderr, "%s error: %d\n", msg, err);				\
        return Z_DATA_ERROR;												\
    }																							\
}


	/**
	 *-----------------------------------------------------------------------------
	 * @brief		 :  Compress from file source to file dest until EOF on source.
	 * @param	     :  [in] const char *fsrc, source file name.
	 * @param	     :  [out] const char *fdst, dest file name.
	 * @return	     :  [type] int. Z_OK on success; others on failure.
	 *-----------------------------------------------------------------------------
	 */
	int def(const char *fsrc, const char *fdst);

	/**
	 *-----------------------------------------------------------------------------
	 * @brief		 :  Decompress from file source to file dest until stream ends or EOF.
	 * @param	     :  [in] const char *fsrc, source file name.
	 * @param	     :  [out] const char *fdst, dest file name.
	 * @return	     :  [type] int. Z_OK on success; others on failure.
	 *-----------------------------------------------------------------------------
	 */
	int inf(const char *fsrc, const char *fdst);

	/**
	 *-----------------------------------------------------------------------------
	 * @brief		 :  compress gzip data.
	 * @param	     :  [in] void *data, data buffer to be compressed.
	 * @param	     :  [in] uLong dlen, data buffer length.
	 * @param	     :  [out] void *zdata, data buffer to save the compressed datas.
	 * @param	     :  [out] uLong *zdlen, compressed data buffer length.
	 * @return	     :  [type] int. Z_OK on success; others on failure.
	 *-----------------------------------------------------------------------------
	 */
	int gzcompress(void *data, uLong dlen, void *zdata, uLong *zdlen);

	/**
	 *-----------------------------------------------------------------------------
	 * @brief		 :  decompress gzip data.
	 * @param	     :  [in] void *zdata, data buffer to save the compressed datas.
	 * @param	     :  [in] uLong *zdlen, compressed data buffer length.
	 * @param	     :  [out] void *data, data buffer to be compressed.
	 * @param	     :  [out] uLong dlen, data buffer length.
	 * @return	     :  [type] int. Z_OK on success; others on failure.
	 *-----------------------------------------------------------------------------
	 */
	int gzdecompress(void *zdata, uLong zdlen, void *data, uLong *dlen);

	/**
	*-----------------------------------------------------------------------------
	* @brief		     :  compress a file into a gzip file.
	* @param	     :  [in] const char *fsrc, the source file path to be compressed.
	* @param	     :  [out] const char *fdst, output dest path of compressed gzip file.
	* @return	     :  [type] int. Z_OK on success; others on failure.
	*-----------------------------------------------------------------------------
	*/
	int compress_file_to_gzip(const char *fsrc, const char *fdst);
}


#endif  /*_ZIP_WORK_H*/
