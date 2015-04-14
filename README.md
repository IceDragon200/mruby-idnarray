mruby-idnarray
==============

## mrb_config.rb
```ruby
MRuby::Build.new do |conf|
  # ...
  conf.gem '/home/icy/docs/codes/IceDragon/mruby-gems/mruby-idnarray'
  # ...
end
```

## Usage:
```ruby
# Create a new NArray
size = 18
narray = NArray.new NArray::Type::UINT8, size

# Set an element
narray[0] = 4

# Get an element
narray[0] #=> 4

# Slice the NArray 
narray.slice(0, 8) #=> NArray

# Do your Enumerable stuff
narray.each_with_index do |n, i|
  # ... do stuff ...
end
```

## Available Types:
Extracted from `include/idnarray.hxx`

```cpp
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
```

## Defines:
| Name                | Type | Default      | Description                  |
| ------------------- | ---- | ------------ | ---------------------------- |
| NARRAY_ENABLE_64BIT | bool | 0 (disabled) | Enable 64 bit types (uint64, int64, float64) |

## Convience methods:
```ruby
NArray.uint8 size
NArray.uint16 size
NArray.uint32 size
NArray.int8 size
NArray.int16 size
NArray.int32 size
# no, we don't have a float16, so, feel free to implement it and gimme a PR.
NArray.float32 size

# if NARRAY_ENABLE_64BIT was set during the build
NArray.uint64 size
NArray.int64 size
NArray.float64 size
```
