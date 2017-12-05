#ifndef __NANOPB_H__
#define __NANOPB_H__

#include "log.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"

#define NANOPB_INIT(type)   type##_init_default
#define NANOPB_FIELDS(type) type##_fields
#define NANOPB_SIZE(type)   type##_size

#define NANOPB_ENCODE(type, in, inbuf, inlen, outlen_ptr, ret) \
	do { \
		pb_ostream_t stream = pb_ostream_from_buffer((uint8_t*)inbuf, (size_t)inlen); \
		if (!pb_encode(&stream, type##_fields, &in)) { \
		    ERR_LOG(0, 0L, 0, "Encoding %s failed: %s\n", #type, PB_GET_ERROR(&stream)); \
		    ret = -1; \
		} \
		else { \
		    *outlen_ptr = stream.bytes_written; \
		    ret = 0; \
		} \
	} while(0);

#define NANOPB_DECODE(type, inbuf, inlen, out_ptr, ret) \
	do { \
		pb_istream_t stream = pb_istream_from_buffer((uint8_t*)inbuf, (size_t)inlen); \
		if (!pb_decode(&stream, type##_fields, out_ptr)) { \
			ERR_LOG(0, 0L, 0, "Decoding %s failed: %s", #type, PB_GET_ERROR(&stream)); \
			ret = -1; \
		} \
		else { \
			ret = 0; \
		} \
	} while(0);

#endif