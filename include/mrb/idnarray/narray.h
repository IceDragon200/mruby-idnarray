#ifndef MRUBY_IDNARRAY_H
#define MRUBY_IDNARRAY_H

#include "idnarray.h"

MRB_IDNARRAY_EXTERN const struct mrb_data_type mrb_idnarray_type;
MRB_IDNARRAY_EXTERN mrb_value mrb_narray_value(mrb_state *mrb, struct NArray *narray);
MRB_IDNARRAY_EXTERN void mrb_narray_check_type(mrb_state *mrb, struct NArray *narray, enum NArrayContentType type);
MRB_IDNARRAY_EXTERN mrb_value mrb_narray_new(mrb_state *mrb, enum NArrayContentType type, size_t size);

#endif
