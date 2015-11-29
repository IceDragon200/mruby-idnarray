#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include "idnarray.hxx"

size_t
NArray::CalcContentTypeSize(enum NArrayContentType type)
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

NArray::NArray(enum NArrayContentType _type, int _size)
{
  data = NULL;
  type = _type;
  size = _size;
  AllocData();
}

NArray::~NArray()
{
  FreeData();
}

void
NArray::RecalculateSizes()
{
  element_size = CalcContentTypeSize(type);
  memsize = element_size * size;
}

bool
NArray::AllocData()
{
  FreeData();
  RecalculateSizes();
  if (memsize) {
    data = malloc(memsize);
    return true;
  } else {
    // TODO handle 0 size Array
    return false;
  }
}

bool
NArray::ClearData()
{
  memset(data, 0, memsize);
  return true;
}

bool
NArray::FreeData()
{
  if (data) {
    free(data);
    data = NULL;
    return true;
  }
  return false;
}

// I hope to the heaven's you pass a valid target, or this will punch you
// in the face
bool NArray::Aget(int index, void *target)
{
// ERR MA GAWD
#define SetTheThing(__type__) *((__type__*)target) = ((__type__*)data)[index]

  assert(target);
  if (index < 0 || (int)size <= index) {
    return false;
  } else {
    switch (type) {
      case NARRAY_INVALID:
        return false;
      case NARRAY_UINT8:
        SetTheThing(uint8_t);
        break;
      case NARRAY_UINT16:
        SetTheThing(uint16_t);
        break;
      case NARRAY_UINT32:
        SetTheThing(uint32_t);
        break;
#if NARRAY_ENABLE_64BIT
      case NARRAY_UINT64:
        SetTheThing(uint64_t);
        break;
#endif
      case NARRAY_INT8:
        SetTheThing(int8_t);
        break;
      case NARRAY_INT16:
        SetTheThing(int16_t);
        break;
      case NARRAY_INT32:
        SetTheThing(int32_t);
        break;
#if NARRAY_ENABLE_64BIT
      case NARRAY_INT64:
        SetTheThing(int64_t);
        break;
#endif
      case NARRAY_FLOAT32:
        SetTheThing(float32_t);
        break;
#if NARRAY_ENABLE_64BIT
      case NARRAY_FLOAT64:
        SetTheThing(float64_t);
        break;
#endif
      default:
        return false;
    }
  }
  return true;
}

bool NArray::Aset(int index, void *value)
{
#define SetTheData(__type__) ((__type__*)data)[index] = *((__type__*)value)
  assert(value);
  if (index < 0 || (int)size <= index) {
    return false;
  } else {
    switch (type) {
      case NARRAY_UINT8: {
        SetTheData(uint8_t);
      } break;
      case NARRAY_UINT16: {
        SetTheData(uint16_t);
      } break;
      case NARRAY_UINT32: {
        SetTheData(uint32_t);
      } break;
#if NARRAY_ENABLE_64BIT
      case NARRAY_UINT64: {
        SetTheData(uint64_t);
      } break;
#endif
      case NARRAY_INT8: {
        SetTheData(int8_t);
      } break;
      case NARRAY_INT16: {
        SetTheData(int16_t);
      } break;
      case NARRAY_INT32: {
        SetTheData(int32_t);
      } break;
#if NARRAY_ENABLE_64BIT
      case NARRAY_INT64: {
        SetTheData(int64_t);
      } break;
#endif
      case NARRAY_FLOAT32: {
        SetTheData(float32_t);
      } break;
#if NARRAY_ENABLE_64BIT
      case NARRAY_FLOAT64: {
        SetTheData(float64_t);
      } break;
#endif
      default: {
        return false;
      }
    }
  }
  return true;
}

NArray*
NArray::Slice(int start, int length)
{
  if (start < 0 || ((int)size < (start + length))) {
    return NULL;
  }
  NArray *result = new NArray(type, length);
#define DO_COPY(__type__) memcpy(result->data, &(((__type__*)data)[start]), result->memsize)
  switch (type) {
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
      delete result;
      return NULL;
    }
  }
  return result;
}

NArray*
NArray::Copy()
{
  NArray *result = new NArray(type, size);
  result->ClearData();
  memcpy(result->data, data, memsize);
  return result;
}

void
NArray::Resize(size_t newsize)
{
  const size_t oldmemsize = memsize;
  void *olddata = data;
  data = NULL;
  size = newsize;
  AllocData();
  ClearData();
  if (olddata != NULL)
  {
    const size_t copymemsize = oldmemsize < memsize ? oldmemsize : memsize;
    memcpy(data, olddata, copymemsize);
    free(olddata);
  }
}
