class NArray
  include Enumerable

  # @!attribute default
  #   @return [Numeric]
  attr_accessor :default

  alias :initialize_raw :initialize
  # @param [Integer] type  content type of the NArray
  # @param [Integer] size  number of elements in the NArray
  # @param [Numeric] default  default value to initialize the content elements to
  def initialize(type, size, default = 0, &block)
    @default = default
    initialize_raw(type, size)
    if block
      size.times { |i| self[i] = block.call(i) }
    elsif @default != 0
      size.times { |i| self[i] = @default }
    end
  end

  # @overload each
  #   @return [Enumerator]
  # @overload each { |element| }
  #   @yieldparam [Numeric] num
  def each
    return to_enum :each unless block_given?
    size.times do |i|
      yield self[i]
    end
  end

  alias :slice_raw :slice
  # @overload slice(index)
  #   Retrieves an element from the NArray
  #   @param [Integer] index
  #   @return [Numeric]
  # @overload slice(range)
  #   Retrieves a range of elements from the NArray
  #   @param [Range] range
  #   @return [NArray]
  # @overload slice(start, length)
  #   Retrieves a range of elements from the NArray (from)
  #   @param [Integer] start
  #   @param [Integer] length
  #   @return [NArray]
  def slice(i, j = nil)
    if Range === i
      j = (i.end - i.begin) + (i.exclude_end? ? 0 : 1)
      slice_raw(i.begin, j)
    elsif j
      slice_raw(i, j)
    else
      aget(i) || @default
    end
  end

  # (see #slice)
  def [](*args)
    slice(*args)
  end

  alias :[]= :aset

  # @yieldparam [Numeric] value
  # @yieldreturn [Numeric] new_value
  # @return [self]
  def map!
    each_with_index do |value, index|
      self.aset(index, yield(value))
    end
    self
  end

  # Converts the NArray to a ruby Array
  #
  # @return [Array<Numeric>]
  def to_a
    result = []
    size.times { |i| result << self[i] }
    result
  end

  # Creates a new resized copy of the NArray
  #
  # @param [Integer] size
  # @return [NArray]
  def resize(size)
    dup.resize!(size)
  end

  # Sets all data in the NArray as the value
  #
  # @param [Integer]
  # @return [self]
  def fill(value)
    map! { |_| value }
    self
  end

  def self.uint8(size, *args, &block)
    new(Type::UINT8, size, *args, &block)
  end

  def self.uint16(size, *args, &block)
    new(Type::UINT16, size, *args, &block)
  end

  def self.uint32(size, *args, &block)
    new(Type::UINT32, size, *args, &block)
  end

  if Type.const_defined?(:UINT64)
    def self.uint64(size, *args, &block)
      new(Type::UINT64, size, *args, &block)
    end
  end

  def self.int8(size, *args, &block)
    new(Type::INT8, size, *args, &block)
  end

  def self.int16(size, *args, &block)
    new(Type::INT16, size, *args, &block)
  end

  def self.int32(size, *args, &block)
    new(Type::INT32, size, *args, &block)
  end

  if Type.const_defined?(:INT64)
    def self.int64(size, *args, &block)
      new(Type::INT64, size, *args, &block)
    end
  end

  def self.float32(size, *args, &block)
    new(Type::FLOAT32, size, *args, &block)
  end

  if Type.const_defined?(:FLOAT64)
    def self.float64(size, *args, &block)
      new(Type::FLOAT64, size, *args, &block)
    end
  end

  # It's broken on newer mruby
  # # generates shorthand methods based on the types available
  # class << self
  #   NArray::Type.constants.each do |constname|
  #     method_name = constname.to_s.downcase
  #     type = Type.const_get(constname)
  #     define_method method_name do |size, *args, &block|
  #       new(type, size, *args, &block)
  #     end
  #   end
  # end
end
