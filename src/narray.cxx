#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>

#ifndef float32_t
  typedef float float32_t;
#endif
#ifndef float64_t
  typedef double float64_t;
#endif

#define NARRAY_ENABLE_64BIT 0

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
  NArrayContentType type; // type of content
  size_t size;         // number of elements in the Array
  size_t element_size; //
  size_t memsize;      // actually allocated size
  void *data;       // pointer to the data

  static size_t CalcContentTypeSize(enum NArrayContentType type);

  NArray(enum NArrayContentType _type, int _size);
  ~NArray();

  bool Resize(int new_size);
  bool Aget(int index, void *target);
  bool Aset(int index, void *val);
private:
  bool AllocData();
  bool FreeData();
  void RecalculateSizes();
};

#define NArrayReflectGetType(type)
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
    memset(data, 0, memsize);
    return true;
  } else {
    // TODO handle 0 size Array
    return false;
  }
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
  if (index < 0 || (int)size < index) {
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
  if (index < 0 || (int)size < index) {
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

static struct RClass *narray_class;
static struct RClass *narray_type_module;

static void
narray_free(mrb_state *mrb, void *ptr)
{
  if (ptr) {
    mrb_free(mrb, ptr);
  }
}

struct mrb_data_type mrb_narray_type = { "NArray", narray_free };

static NArray*
get_narray(mrb_state *mrb, mrb_value self)
{
  return (NArray*)mrb_data_get_ptr(mrb, self, &mrb_narray_type);
}

/* @class NArray
 * @method initialize(type, size)
 *   @param [Integer] type
 *   @param [Integer] size
 */
static mrb_value
narray_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int type;
  mrb_int size;
  mrb_int dfault = 0;
  NArray *narray = NULL;
  mrb_get_args(mrb, "ii|i", &type, &size, &dfault);

  if (type <= NARRAY_INVALID || type >= NARRAY_CONTENT_TYPE_COUNT) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid NArray type.");
    /* if it somehow manages to get past here */
    return mrb_nil_value();
  }

  narray = new NArray((enum NArrayContentType)type, size);
  mrb_data_init(self, narray, &mrb_narray_type);
  return self;
}

#define MAYBE_RETURN_NIL(__statement__) if (!(__statement__)) return mrb_nil_value()
/* @class NArray
 * @method aget(index)
 *   @param [Integer] index
 *   @return [Numeric, nil]  depends on the internal type, or nil if the type is invalid
 */
static mrb_value
narray_aget(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  NArray *narray = get_narray(mrb, self);
  switch (narray->type) {
    case NARRAY_INVALID: {
      return mrb_nil_value();
    }
    case NARRAY_UINT8: {
      uint8_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
    case NARRAY_UINT16: {
      uint16_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
    case NARRAY_UINT32: {
      uint32_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
#if NARRAY_ENABLE_64BIT
    case NARRAY_UINT64: {
      uint64_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
#endif
    case NARRAY_INT8: {
      int8_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
    case NARRAY_INT16: {
      int16_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
    case NARRAY_INT32: {
      int32_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
#if NARRAY_ENABLE_64BIT
    case NARRAY_INT64: {
      int64_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_fixnum_value((mrb_int)r);
    }
#endif
    case NARRAY_FLOAT32: {
      float32_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_float_value(mrb, (mrb_float)r);
    }
#if NARRAY_ENABLE_64BIT
    case NARRAY_FLOAT64: {
      float64_t r;
      MAYBE_RETURN_NIL(narray->Aget(index, &r));
      return mrb_float_value(mrb, (mrb_float)r);
    }
#endif
    default: {
      return mrb_nil_value();
    }
  }
}

/* @class NArray
 * @method aset(index, value)
 *   @param [Integer] index
 *   @param [Numeric] value
 */
static mrb_value
narray_aset(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_value val;
  mrb_get_args(mrb, "io", &index, &val);
  NArray *narray = get_narray(mrb, self);
  switch (narray->type) {
    case NARRAY_UINT8: {
      uint8_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
    case NARRAY_UINT16: {
      uint16_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
    case NARRAY_UINT32: {
      uint32_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_UINT64: {
      uint64_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
#endif
    case NARRAY_INT8: {
      int8_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
    case NARRAY_INT16: {
      int16_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
    case NARRAY_INT32: {
      int32_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_INT64: {
      int64_t r = mrb_int(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
#endif
    case NARRAY_FLOAT32: {
      float32_t r = mrb_to_flo(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
#if NARRAY_ENABLE_64BIT
    case NARRAY_FLOAT64: {
      float64_t r = mrb_to_flo(mrb, val);
      MAYBE_RETURN_NIL(narray->Aset(index, &r));
    } break;
#endif
    default: {
    } break;
  }
  return mrb_true_value();
}

extern "C" void
mrb_mruby_idnarray_gem_init(mrb_state* mrb)
{
  narray_class = mrb_define_class(mrb, "NArray", mrb->object_class);
  narray_type_module = mrb_define_module_under(mrb, narray_class, "Type");
  MRB_SET_INSTANCE_TT(narray_class, MRB_TT_DATA);

  mrb_define_method(mrb, narray_class, "initialize", narray_initialize, MRB_ARGS_ARG(2, 1));
  mrb_define_method(mrb, narray_class, "aget",       narray_aget,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, narray_class, "aset",       narray_aset,       MRB_ARGS_REQ(2));

  mrb_define_const(mrb, narray_type_module, "UINT8", mrb_fixnum_value(NARRAY_UINT8));
  mrb_define_const(mrb, narray_type_module, "UINT16", mrb_fixnum_value(NARRAY_UINT16));
  mrb_define_const(mrb, narray_type_module, "UINT32", mrb_fixnum_value(NARRAY_UINT32));
#if NARRAY_ENABLE_64BIT
  mrb_define_const(mrb, narray_type_module, "UINT64", mrb_fixnum_value(NARRAY_UINT64));
#endif
  mrb_define_const(mrb, narray_type_module, "INT8", mrb_fixnum_value(NARRAY_INT8));
  mrb_define_const(mrb, narray_type_module, "INT16", mrb_fixnum_value(NARRAY_INT16));
  mrb_define_const(mrb, narray_type_module, "INT32", mrb_fixnum_value(NARRAY_INT32));
#if NARRAY_ENABLE_64BIT
  mrb_define_const(mrb, narray_type_module, "INT64", mrb_fixnum_value(NARRAY_INT64));
#endif
  mrb_define_const(mrb, narray_type_module, "FLOAT32", mrb_fixnum_value(NARRAY_FLOAT32));
#if NARRAY_ENABLE_64BIT
  mrb_define_const(mrb, narray_type_module, "FLOAT64", mrb_fixnum_value(NARRAY_FLOAT64));
#endif
}

extern "C" void
mrb_mruby_idnarray_gem_final(mrb_state* mrb)
{
}
