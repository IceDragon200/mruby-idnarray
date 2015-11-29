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
  /* unsigned int */
  NARRAY_UINT8 = 1,
  NARRAY_UINT16 = 2,
  NARRAY_UINT32 = 3,
#if NARRAY_ENABLE_64BIT
  NARRAY_UINT64 = 4,
#endif
  /* signed int */
  NARRAY_INT8 = 5,
  NARRAY_INT16 = 6,
  NARRAY_INT32 = 7,
#if NARRAY_ENABLE_64BIT
  NARRAY_INT64 = 8,
#endif
  /* float / double */
  NARRAY_FLOAT32 = 9,
#if NARRAY_ENABLE_64BIT
  NARRAY_FLOAT64 = 10,
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
  void Resize(size_t newsize);
  NArray* Slice(int start, int length);
  NArray* Copy();
  bool ClearData();
private:
  bool AllocData();
  bool FreeData();
  void RecalculateSizes();
};

#endif
