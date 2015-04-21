class NArray
  module Type
    INVALID = 0

    UINT8 = 1
    UINT16 = 2
    UINT32 = 3
    UINT64 = 4

    UINT8 = 5
    UINT16 = 6
    UINT32 = 7
    UINT64 = 8

    FLOAT32 = 9
    FLOAT64 = 10
  end

  # @param [Type] type
  # @param [Integer] size
  def initialize(type, size)
  end

  # @param [NArray] other
  # @return [self]
  def initialize_copy(other)
  end

  # Will return nil if the index was out of range, NArray is a bit forgiving
  # like ruby's Array.
  #
  # @return [Numeric, nil]
  def aget(index)
  end

  # If the index is out of range, this method will do nothing.
  #
  # @param [Integer]
  # @param [Numeric] value
  # @return [void]
  def aset(index, value)
  end

  # Type of content this NArray has
  #
  # @return [Type]
  def type
  end

  # Number of elements in the array
  #
  # @return [Integer]
  def size
  end

  # Size of the array in bytes
  #
  # @return [Integer]
  def memsize
  end

  # Size of 1 element in the array in bytes
  #
  # @return [Integer]
  def element_size
  end

  # Creates a new NArray slice from the given start position and length.
  # This slice does not share memory with its original.
  #
  # @param [Integer] start
  # @param [Integer] length
  # @return [NArray]
  def slice(start, length)
  end

  # Resets all elements to 0, the size remains unchanged
  #
  # @return [void]
  def clear
  end
end
