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

  alias :slice_raw :slice
  # @overload slice(i)
  #   @param [Integer] i
  #   @return [Numeric]
  # @overload slice(range)
  #   @param [Range] range
  #   @return [NArray]
  # @overload slice(i, j)
  #   @param [Integer] i
  #   @param [Integer] j
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

  # @yieldparam [Numeric] num
  def each
    return to_enum :each unless block_given?
    size.times do |i|
      yield self[i]
    end
  end

  # @yieldparam [Array<Numeric>] num
  def to_a
    result = []
    size.times { |i| result << self[i] }
    result
  end

  class << self
    Type.constants.each do |constname|
      name = constname.to_s.downcase
      type = Type.const_get(constname)
      define_method name do |size, *args, &block|
        new(type, size, *args, &block)
      end
    end
  end
end
