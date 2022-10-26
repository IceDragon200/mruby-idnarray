#ifndef IDNARRAY_H
#define IDNARRAY_H

#include <stdlib.h>

#define MRB_IDNARRAY_EXTERN extern

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
  enum NArrayContentType type; // type of content
  size_t size;                 // number of elements in the Array
  size_t element_size;         // size of 1 element in the array
  size_t memsize;              // actually allocated size
  void *data;                  // pointer to the data
};

#define IDNARRAY_OK 0
#define IDNARRAY_NO_DATA 1
#define IDNARRAY_OUT_OF_MEMORY 2
#define IDNARRAY_SOURCE_OUT_OF_RANGE 3
#define IDNARRAY_DESTINATION_OUT_OF_RANGE 4
#define IDNARRAY_INVALID_TYPE 5

size_t idnarray_calc_content_type_size(enum NArrayContentType type);
struct NArray* idnarray_alloc();
int idnarray_init(struct NArray* narray, enum NArrayContentType type, size_t size);
struct NArray* idnarray_new(enum NArrayContentType type, size_t size);
int idnarray_free(struct NArray* narray);
int idnarray_aget(struct NArray* narray, size_t index, void *target);
int idnarray_aset(struct NArray* narray, size_t index, void *val);
int idnarray_resize(struct NArray* narray, size_t newsize);
int idnarray_slice(struct NArray* source_narray, struct NArray* dest_narray, size_t start, size_t length);
// struct NArray* idnarray_copy(struct NArray* narray); // just use slice
int idnarray_clear(struct NArray* narray);

#endif
