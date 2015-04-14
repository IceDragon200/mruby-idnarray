class NArray
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

  def [](index)
    aget(index) || @default
  end

  alias :[]= :aset
end
