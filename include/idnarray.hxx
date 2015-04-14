#ifndef IDNARRAY_H
#define IDNARRAY_H

#include <stdlib.h>

#ifndef NARRAY_ENABLE_64BIT
#  define NARRAY_ENABLE_64BIT 0
#endif

#ifndef float32_t
  typedef float float32_t;
#endif
#ifndef float64_t
  typedef double float64_t;
#endif

enum NArrayContentType {
  NARRAY_INVALID = 0,
  NARRAY_UINT8,
  NARRAY_UINT16,
  NARRAY_UINT32,
#if NARRAY_ENABLE_64BIT
  NARRAY_UINT64,
#endif
  NARRAY_INT8,
  NARRAY_INT16,
  NARRAY_INT32,
#if NARRAY_ENABLE_64BIT
  NARRAY_INT64,
#endif
  NARRAY_FLOAT32,
#if NARRAY_ENABLE_64BIT
  NARRAY_FLOAT64,
#endif
  NARRAY_CONTENT_TYPE_COUNT
};

struct NArray {
public:
  enum NArrayContentType type; // type of content
  size_t size;                 // number of elements in the Array
  size_t element_size;         // size of 1 element in the array
  size_t memsize;              // actually allocated size
  void *data;                  // pointer to the data

  static size_t CalcContentTypeSize(enum NArrayContentType type);

  NArray(enum NArrayContentType _type, int _size);
  ~NArray();

  bool Aget(int index, void *target);
  bool Aset(int index, void *val);
  NArray* Slice(int start, int length);
  NArray* Copy();
  bool ClearData();
private:
  bool AllocData();
  bool FreeData();
  void RecalculateSizes();
};

#endif
