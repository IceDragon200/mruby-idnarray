#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <stdbool.h>
#include "idnarray.h"

size_t
idnarray_calc_content_type_size(enum NArrayContentType type)
{
  switch (type) {
    case NARRAY_INVALID:
      return 0;
    case NARRAY_UINT8:
      return sizeof(uint8_t);
    case NARRAY_UINT16:
      return sizeof(uint16_t);
    case NARRAY_UINT32:
      return sizeof(uint32_t);
#if NARRAY_ENABLE_64BIT
    case NARRAY_UINT64:
      return sizeof(uint64_t);
#endif
    case NARRAY_INT8:
      return sizeof(int8_t);
    case NARRAY_INT16:
      return sizeof(int16_t);
    case NARRAY_INT32:
      return sizeof(int32_t);
#if NARRAY_ENABLE_64BIT
    case NARRAY_INT64:
      return sizeof(int64_t);
#endif
    case NARRAY_FLOAT32:
      return sizeof(float32_t);
#if NARRAY_ENABLE_64BIT
    case NARRAY_FLOAT64:
      return sizeof(float64_t);
#endif
    default:
      return 0;
  }
}

static bool
alloc_data(struct NArray *narray)
{
  // shouldn't be allocated yet.
  assert(narray->data == NULL);

  narray->element_size = idnarray_calc_content_type_size(narray->type);
  narray->memsize = narray->element_size * narray->size;

  if (narray->memsize) {
    narray->data = malloc(narray->memsize);
  } else {
    return IDNARRAY_NO_DATA;
  }

  // Should be allocated
  if (narray->data) {
    return IDNARRAY_OK;
  }
  return IDNARRAY_OUT_OF_MEMORY;
}

struct NArray*
idnarray_alloc()
{
  return malloc(sizeof(struct NArray));
}

int
idnarray_init(struct NArray *narray, enum NArrayContentType type, size_t size)
{
  narray->size = size;
  narray->data = NULL;
  narray->type = type;

  return alloc_data(narray);
}

int
idnarray_free(struct NArray *narray)
{
  if (narray->data) {
    free(narray->data);
  }
  free(narray);
  return IDNARRAY_OK;
}

struct NArray*
idnarray_new(enum NArrayContentType type, size_t size)
{
  struct NArray* narray = idnarray_alloc();
  if (narray) {
    const int status = idnarray_init(narray, type, size);

    if (status == IDNARRAY_OK || status == IDNARRAY_NO_DATA) {
      return narray;
    }
    idnarray_free(narray);
  }
  return NULL;
}

int
idnarray_aget(struct NArray* narray, size_t index, void *target)
{
// Welp that's a thing
#define get_data_by_type(__type__) *((__type__*)target) = ((__type__*)narray->data)[index]

  if (!target) {
    // not really, but we can use this status here
    return IDNARRAY_DESTINATION_OUT_OF_RANGE;
  }

  if (index >= narray->size) {
    return IDNARRAY_SOURCE_OUT_OF_RANGE;
  }

  switch (narray->type) {
    case NARRAY_INVALID:
      return false;
    case NARRAY_UINT8:
      get_data_by_type(uint8_t);
      break;
    case NARRAY_UINT16:
      get_data_by_type(uint16_t);
      break;
    case NARRAY_UINT32:
      get_data_by_type(uint32_t);
      break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_UINT64:
      get_data_by_type(uint64_t);
      break;
#endif
    case NARRAY_INT8:
      get_data_by_type(int8_t);
      break;
    case NARRAY_INT16:
      get_data_by_type(int16_t);
      break;
    case NARRAY_INT32:
      get_data_by_type(int32_t);
      break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_INT64:
      get_data_by_type(int64_t);
      break;
#endif
    case NARRAY_FLOAT32:
      get_data_by_type(float32_t);
      break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_FLOAT64:
      get_data_by_type(float64_t);
      break;
#endif
    default:
      return IDNARRAY_INVALID_TYPE;
  }
  return IDNARRAY_OK;
}

int
idnarray_aset(struct NArray* narray, size_t index, void *val)
{
#define set_data_by_type(__type__) ((__type__*)narray->data)[index] = *((__type__*)val)

  if (!val) {
    return IDNARRAY_DESTINATION_OUT_OF_RANGE;
  }

  if (index >= narray->size) {
    return IDNARRAY_SOURCE_OUT_OF_RANGE;
  }

  switch (narray->type) {
    case NARRAY_UINT8: {
      set_data_by_type(uint8_t);
    } break;
    case NARRAY_UINT16: {
      set_data_by_type(uint16_t);
    } break;
    case NARRAY_UINT32: {
      set_data_by_type(uint32_t);
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_UINT64: {
      set_data_by_type(uint64_t);
    } break;
#endif
    case NARRAY_INT8: {
      set_data_by_type(int8_t);
    } break;
    case NARRAY_INT16: {
      set_data_by_type(int16_t);
    } break;
    case NARRAY_INT32: {
      set_data_by_type(int32_t);
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_INT64: {
      set_data_by_type(int64_t);
    } break;
#endif
    case NARRAY_FLOAT32: {
      set_data_by_type(float32_t);
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_FLOAT64: {
      set_data_by_type(float64_t);
    } break;
#endif
    default: {
      return IDNARRAY_INVALID_TYPE;
    }
  }
  return IDNARRAY_OK;
}

int
idnarray_resize(struct NArray* narray, size_t newsize)
{
  const size_t oldsize = narray->size;
  const size_t oldmemsize = narray->memsize;
  void *olddata = narray->data;

  narray->data = NULL;
  narray->size = newsize;

  const int status = alloc_data(narray);

  if (status == IDNARRAY_OK) {
    memset(narray->data, 0, narray->memsize);

    if (olddata)
    {
      const size_t copymemsize = oldmemsize < narray->memsize ? oldmemsize : narray->memsize;
      memcpy(narray->data, olddata, copymemsize);
      free(olddata);
    }
    return IDNARRAY_OK;
  }

  // Restore original data before returning
  narray->size = oldsize;
  narray->memsize = oldmemsize;
  narray->data = olddata;

  return status;
}

int
idnarray_slice(struct NArray* source_narray, struct NArray* dest_narray, size_t start, size_t length)
{
  if (start >= source_narray->size) {
    return IDNARRAY_SOURCE_OUT_OF_RANGE;
  }

  if ((start + length) > source_narray->size) {
    return IDNARRAY_SOURCE_OUT_OF_RANGE;
  }

  if (length > dest_narray->size) {
    return IDNARRAY_DESTINATION_OUT_OF_RANGE;
  }

#define DO_COPY(__type__) memcpy(dest_narray->data, &(((__type__*)source_narray->data)[start]), dest_narray->memsize)

  switch (source_narray->type) {
    case NARRAY_UINT8: {
      DO_COPY(uint8_t);
    } break;
    case NARRAY_UINT16: {
      DO_COPY(uint16_t);
    } break;
    case NARRAY_UINT32: {
      DO_COPY(uint32_t);
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_UINT64: {
      DO_COPY(uint64_t);
    } break;
#endif
    case NARRAY_INT8: {
      DO_COPY(int8_t);
    } break;
    case NARRAY_INT16: {
      DO_COPY(int16_t);
    } break;
    case NARRAY_INT32: {
      DO_COPY(int32_t);
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_INT64: {
      DO_COPY(int64_t);
    } break;
#endif
    case NARRAY_FLOAT32: {
      DO_COPY(float32_t);
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_FLOAT64: {
      DO_COPY(float64_t);
    } break;
#endif
    default: {
      return IDNARRAY_INVALID_TYPE;
    }
  }
  return IDNARRAY_OK;
}

int
idnarray_clear(struct NArray* narray)
{
  if (narray->data) {
    memset(narray->data, 0, narray->memsize);
  }
  return IDNARRAY_NO_DATA;
}
