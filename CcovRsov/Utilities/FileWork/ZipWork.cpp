#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ZipWork.h"


namespace FileWork
{
	int def(const char *fsrc, const char *fdst)
	{
		int ret = -1;
		int flush = Z_NO_FLUSH;
		z_stream strm = { Z_NULL };
		unsigned char in[SIZE_CHUNK];
		unsigned char out[SIZE_CHUNK];

		FILE *src = fopen(fsrc, "r");
		FILE *dst = fopen(fdst, "w+");
		assert(src && dst);

		/** @allocate deflate state */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;

		ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
		assert(ret == Z_OK);

		/** @compress until end of file */
		do {
			strm.avail_in = fread(in, 1, SIZE_CHUNK, src);
			if (strm.avail_in == 0) break;
			strm.next_in = in;

			/** @distinguish the end of the file or the middle of the file */
			flush = feof(src) ? Z_FINISH : Z_NO_FLUSH;

			/** @run deflate() on input until output buffer not full, finish compression if all of source has been read in */
			do {
				strm.avail_out = SIZE_CHUNK;
				strm.next_out = out;

				ret = deflate(&strm, flush);
				/** @state not clobbered */
				assert(ret != Z_STREAM_ERROR);

				size_t wlen = fwrite(out, 1, sizeof(out) - strm.avail_out, dst);
				assert(wlen == sizeof(out) - strm.avail_out);

			} while (strm.avail_out == 0);

			/** @all input will be used */
			assert(strm.avail_in == 0);
			/** @done when last data in file processed */
		} while (flush != Z_FINISH);

		/** @stream will be complete */
		assert(ret == Z_STREAM_END);
		/** @clean up and return */
		(void)deflateEnd(&strm);

		fclose(src);
		fclose(dst);
		unlink(fsrc);
		return Z_OK;
	}

	int inf(const char *fsrc, const char *fdst)
	{
		int ret = -1;
		z_stream strm = { Z_NULL };
		unsigned char in[SIZE_CHUNK];
		unsigned char out[SIZE_CHUNK];

		FILE *src = fopen(fsrc, "r");
		FILE *dst = fopen(fdst, "w+");
		assert(src && dst);

		/** @allocate inflate state */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;

		ret = inflateInit(&strm);
		assert(ret == Z_OK);

		/** @decompress until deflate stream ends or end of file */
		do {
			strm.avail_in = fread(in, 1, SIZE_CHUNK, src);
			if (strm.avail_in == 0) break;
			strm.next_in = in;

			/** @run inflate() on input until output buffer not full */
			do {
				strm.avail_out = SIZE_CHUNK;
				strm.next_out = out;

				ret = inflate(&strm, Z_NO_FLUSH);
				/** @state not clobbered */
				assert(ret != Z_STREAM_ERROR);

				switch (ret) {
				case Z_NEED_DICT:
					/** @and fall through */
					ret = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					goto __end;
				}

				size_t wlen = fwrite(out, 1, sizeof(out) - strm.avail_out, dst);
				assert(wlen == sizeof(out) - strm.avail_out);

			} while (strm.avail_out == 0);

			/** @done when inflate() says it's done */
		} while (ret != Z_STREAM_END);

		/** @clean up and return */
	__end:
		(void)inflateEnd(&strm);
		fclose(src);
		fclose(dst);
		unlink(fsrc);
		return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
	}


	int gzcompress(void *data, uLong dlen, void *zdata, uLong *zdlen)
	{
		int err = 0;
		z_stream c_stream = { 0 };

		if (data && dlen > 0)
		{
			c_stream.zalloc = Z_NULL;
			c_stream.zfree = Z_NULL;
			c_stream.opaque = Z_NULL;

			/** @use param 'MAX_WBITS+16' so that gzip headers are contained */
			err = deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
			CHECK_ERR(err, "deflateInit");

			c_stream.next_in = (Bytef *)data;
			c_stream.avail_in = dlen;
			c_stream.next_out = (Bytef *)zdata;
			c_stream.avail_out = *zdlen;

			while (c_stream.total_in != dlen && c_stream.total_out < *zdlen)
			{
				/** @force small buffers */
				c_stream.avail_in = c_stream.avail_out = 1;
				err = deflate(&c_stream, Z_NO_FLUSH);
				CHECK_ERR(err, "deflate");
			}

			/** @Finish the stream, still forcing small buffers: */
			for (;;)
			{
				c_stream.avail_out = 1;
				err = deflate(&c_stream, Z_FINISH);
				if (err == Z_STREAM_END) break;
				CHECK_ERR(err, "deflate");
			}

			err = deflateEnd(&c_stream);
			CHECK_ERR(err, "deflateEnd");

			*zdlen = c_stream.total_out;
			return Z_OK;
		}

		return Z_DATA_ERROR;
	}

	int gzdecompress(void *zdata, uLong zdlen, void *data, uLong *dlen)
	{
		int err = 0;
		z_stream d_stream = { 0 };

		if (zdata && zdlen > 0)
		{
			d_stream.zalloc = Z_NULL;
			d_stream.zfree = Z_NULL;
			d_stream.opaque = Z_NULL;
			d_stream.next_in = (Bytef *)zdata;
			d_stream.avail_in = 0;
			d_stream.next_out = (Bytef *)data;

			err = inflateInit2(&d_stream, MAX_WBITS + 16);
			CHECK_ERR(err, "inflateInit");
			
			while (d_stream.total_out < *dlen && d_stream.total_in < zdlen) 
			{
				/** @force small buffers */
				d_stream.avail_in = d_stream.avail_out = 1;
				err = inflate(&d_stream, Z_NO_FLUSH);
				if (err == Z_STREAM_END) break;
				CHECK_ERR(err, "inflate");
			}

			err = inflateEnd(&d_stream);
			CHECK_ERR(err, "inflateEnd");

			*dlen = d_stream.total_out;
			return Z_OK;
		}

		return Z_DATA_ERROR;
	}

	int compress_file_to_gzip(const char *fsrc, const char *fdst)
	{
		uLong flen, clen;
		FILE *fp = nullptr;
		unsigned char *fbuf = nullptr;
		unsigned char *cbuf = nullptr;

		if ((fp = fopen(fsrc, "rb")) == nullptr)
		{
			//printf("%s: can not open %s!\n", __func__, fsrc);
			goto __error;
		}

		/** @load file content to buffer*/
		fseek(fp, 0L, SEEK_END);
		flen = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		fbuf = (unsigned char *)malloc(flen * sizeof(unsigned char));
		if (nullptr == fbuf)
		{
			//printf("%s: no enough memory!\n", __func__);
			goto __error;
		}
		fread(fbuf, sizeof(unsigned char), flen, fp);
		fclose(fp);
		fp = nullptr;

		/** @compute compressBound*/
		clen = compressBound(flen);
		cbuf = (unsigned char *)malloc(clen * sizeof(unsigned char));
		if (nullptr == cbuf)
		{
			//printf("%s: no enough memory!\n", __func__);
			goto __error;
		}

		if (gzcompress(fbuf, flen, cbuf, &clen))
		{
			//printf("%s: compress %s failed!\n", __func__, fsrc);
			goto __error;
		}

		if (nullptr == fdst)
		{
			goto __error;
		}

		if ((fp = fopen(fdst, "wb")) == nullptr)
		{
			//printf("%s: can not open %s!\n", __func__, fdst);
			goto __error;
		}
		fwrite(cbuf, sizeof(unsigned char), clen, fp);

		fclose(fp);
		free(fbuf);
		free(cbuf);
		unlink(fsrc);
		return Z_OK;

	__error:
		if (fp) fclose(fp);
		if (fbuf) free(fbuf);
		if (cbuf) free(cbuf);
		return Z_DATA_ERROR;
	}
}