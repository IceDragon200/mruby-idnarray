assert 'NArray#initialize without default parameter' do
  narray = NArray.new NArray::Type::UINT8, 10
  assert_equal NArray::Type::UINT8, narray.type
  assert_equal 10, narray.size
  assert_equal 1, narray.element_size
  assert_equal 10, narray.memsize
end

assert 'NArray#initialize with default parameter' do
  narray = NArray.new NArray::Type::UINT16, 10, 18
  assert_equal NArray::Type::UINT16, narray.type
  assert_equal 10, narray.size
  assert_equal 2, narray.element_size
  assert_equal 20, narray.memsize
  assert_equal 18, narray.default
  assert_equal 18, narray[0]
  10.times do |i|
    assert_equal 18, narray[i], "element #{i} was not equal to 18"
  end
end

assert 'NArray#initialize_copy' do
  narray = NArray.new NArray::Type::UINT16, 10, 12
  narray_copy = narray.dup

  assert_equal narray.size, narray_copy.size
  assert_equal narray.element_size,  narray_copy.element_size
  assert_equal narray.memsize, narray_copy.memsize
  assert_equal narray.default, narray_copy.default

  # check that it is indeed what we gave it
  assert_equal 12, narray_copy[0]

  # check that the data matches for the arrays
  10.times do |i|
    assert_equal narray[i], narray_copy[i], "expected element #{i} to equal #{narray[i]}"
  end

  # modify the data
  10.times do |i|
    narray_copy[i] = i
  end

  # check that the data doesn't match for the arrays
  # this is done to ensure that both arrays have unique data and are not shared.
  10.times do |i|
    assert_not_equal narray[i], narray_copy[i], "expected element #{i} not to equal #{narray[i]}"
  end
end

assert 'NArray#slice' do
  narray = NArray.new NArray::Type::INT32, 100, 12
  r1 = narray.slice 1
  r2 = narray.slice 1, 40
  r3 = narray.slice 40...90

  assert_equal 12, r1

  assert_equal narray.type, r2.type
  assert_equal 40, r2.size
  4.times do |i|
    assert_equal narray[i + 1], r2[i], "slice(index, length) expected element #{i} to equal #{narray[i + 1]}"
  end

  assert_equal narray.type, r3.type
  assert_equal 50, r3.size
  5.times do |i|
    assert_equal narray[i + 50], r3[i], "slice(range) expected element #{i} to equal #{narray[i + 50]}"
  end
end

assert 'NArray convience methods' do
  n1 = NArray.uint8(32)
  n2 = NArray.uint16(32)
  n3 = NArray.uint32(32)

  n5 = NArray.int8(32)
  n6 = NArray.int16(32)
  n7 = NArray.int32(32)

  n9 = NArray.float32(32)

  assert_equal NArray::Type::UINT8,   n1.type
  assert_equal 1, n1.element_size
  assert_equal NArray::Type::INT8,    n5.type
  assert_equal 1, n5.element_size

  assert_equal NArray::Type::UINT16,  n2.type
  assert_equal 2, n2.element_size

  assert_equal NArray::Type::INT16,   n6.type
  assert_equal 2, n6.element_size

  assert_equal NArray::Type::UINT32,  n3.type
  assert_equal 4, n3.element_size

  assert_equal NArray::Type::INT32,   n7.type
  assert_equal 4, n7.element_size

  assert_equal NArray::Type::FLOAT32, n9.type
  assert_equal 4, n9.element_size

  if NArray::Type.const_defined?(:UINT64)
    n4 = NArray.float64(32)
    assert_equal NArray::Type::UINT64, n10.type
    assert_equal 8, n4.element_size
  end

  if NArray::Type.const_defined?(:INT64)
    n8 = NArray.float64(32)
    assert_equal NArray::Type::INT64, n10.type
    assert_equal 8, n8.element_size
  end

  if NArray::Type.const_defined?(:FLOAT64)
    n10 = NArray.float64(32)
    assert_equal NArray::Type::FLOAT64, n10.type
    assert_equal 8, n10.element_size
  end
  true
end
