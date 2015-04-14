#include <mruby.h>

enum NArrayContentType {
  NARRAY_INVALID = 0;
  NARRAY_UINT8,
  NARRAY_UINT16,
  NARRAY_UINT32,
  NARRAY_UINT64,
  NARRAY_INT8,
  NARRAY_INT16,
  NARRAY_INT32,
  NARRAY_INT64,
  NARRAY_FLOAT32,
  NARRAY_FLOAT64,
  NARRAY_CONTENT_TYPE_COUNT
};

struct NArray {
  NArray(enum NArrayContentType type, int size);
  ~NArray();
};

static struct RClass *narray_class;

/* @class NArray
 * @method initialize(type, size, default = 0)
 *   @param [Integer] type
 *   @param [Integer] size
 *   @param [Integer] default
 */
static mrb_value
narray_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int type;
  mrb_int size;
  mrb_int dfault = 0;
  mrb_value initproc = mrb_nil_value();
  mrb_get_args(mrb, "ii|i&", &type, &size, &dfault);
  return self;
}

void
mrb_mruby_narray_gem_init(mrb_state* mrb)
{
  narray_class = mrb_define_class(mrb, "NArray", mrb->object_class);
  MRB_SET_INSTANCE_TT(narray_class, MRB_TT_DATA);

  mrb_define_method(mrb, narray_class, "initialize", narray_initialize, MRB_ARGS_ARG(2, 1));
}
