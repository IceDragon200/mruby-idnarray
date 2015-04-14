class NArray
  include Enumerable

  alias :initialize_raw :initialize
  # @param [Integer] type  content type of the NArray
  # @param [Integer] size  number of elements in the NArray
  # @param [Numeric] default  default value to initialize the content elements to
  def initialize(type, size, default = 0, &block)
    @default = default
    initialize_raw(type, size)
    if block
      size.times { |i| self[i] = block.call(i) }
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
      slice_raw(i.start, i.end + (i.exclude_end? ? 0 : 1))
    elsif j
      slice_raw(i, j)
    else
      aget(index) || @default
    end
  end

  # @param [Integer] index
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

end
