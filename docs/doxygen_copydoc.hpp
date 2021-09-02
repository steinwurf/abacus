/// Acceleration
struct acceleration
{
public:
    /// Similar to math::vector_add_into()
    /// But here the number of bytes consumed will be a multiple of the
    /// accelerations granularity (0 being a valid possibility).
    uint32_t vector_add_into(uint8_t* x, const uint8_t* y, uint32_t size) const;

    /// Similar to math::vector_subtract_into()
    /// But here the number of bytes consumed will be a multiple of the
    /// accelerations granularity (0 being a valid possibility).
    uint32_t vector_subtract_into(uint8_t* x, const uint8_t* y,
                                  uint32_t size) const;

    /// Similar to math::vector_multiply_into()
    /// But here the number of bytes consumed will be a multiple of the
    /// accelerations granularity (0 being a valid possibility).
    uint32_t vector_multiply_into(uint8_t* x, uint32_t constant,
                                  uint32_t size) const;

    /// Similar to math::vector_multiply_add_into()
    /// But here the number of bytes consumed will be a multiple of the
    /// accelerations granularity (0 being a valid possibility).
    uint32_t vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                      uint32_t constant, uint32_t size) const;

    /// Similar to math::vector_multiply_subtract_into()
    /// But here the number of bytes consumed will be a multiple of the
    /// accelerations granularity (0 being a valid possibility).
    uint32_t vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                           uint32_t constant,
                                           uint32_t size) const;

    /// Similar to math::vector_dot_product()
    /// But here the number of bytes consumed will be a multiple of the
    /// accelerations granularity (0 being a valid possibility).
    uint32_t vector_dot_product(uint8_t** x, const uint8_t** y,
                                const uint8_t** constants, uint32_t size,
                                std::size_t x_vectors,
                                std::size_t y_vectors) const;

    /// @return The finite field as an enum.
    fifi::finite_field field() const;

    /// The finite field info object
    fifi::field::binary field_info() const;

    /// The finite field info object
    fifi::field::binary4 field_info() const;

    /// The finite field info object
    fifi::field::binary8 field_info() const;

    /// The finite field info object
    fifi::field::binary16 field_info() const;

    /// The finite field info object
    fifi::field::prime2325 field_info() const;

    /// Returns true if the acceleration was enabled when code was compiled.
    static bool is_compiled();
};

/// Finite Field
struct field
{
public:
    /// Data type used to hold the prime
    using prime_type = uint;

    /// Data type used to hold the degree
    using degree_type = uint;

    /// Data type used to hold the order
    using order_type = uint;

    /// Data type used to store values in this field
    using value_type = uint;

    /// The field as a fifi::finite_field enum
    static constexpr fifi::finite_field field;

    /// The field order i.e. number of field elements
    static constexpr order_type order = 0U;

    /// The maximum decimal value of any field element
    static constexpr order_type max_value = 0U;

    /// The field degree
    static constexpr degree_type degree = 0U;

    /// The data granularity required by the field
    static constexpr uint8_t granularity = sizeof(value_type);

    /// Returns The prime
    /// @return The prime (in decimal representation)
    constexpr uint32_t prime() const;
};
