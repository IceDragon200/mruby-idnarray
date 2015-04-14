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
