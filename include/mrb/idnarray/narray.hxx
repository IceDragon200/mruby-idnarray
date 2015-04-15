#ifndef MRUBY_IDNARRAY_H
#define MRUBY_IDNARRAY_H

#include "idnarray.hxx"

extern "C" const struct mrb_data_type mrb_idnarray_type;
extern "C" mrb_value mrb_narray_value(mrb_state *mrb, NArray *narray);
extern "C" void mrb_narray_check_type(mrb_state *mrb, const NArray *narray, enum NArrayContentType type);
extern "C" mrb_value mrb_narray_new(mrb_state *mrb, enum NArrayContentType type, int size);

#endif
