#ifndef MRUBY_IDNARRAY_H
#define MRUBY_IDNARRAY_H

extern "C" const struct mrb_data_type mrb_narray_type;
extern "C" mrb_value mrb_narray_value(mrb_state *mrb, NArray *narray);

#endif
