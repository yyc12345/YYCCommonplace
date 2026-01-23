#include <charconv>
#include <cstdint>

int main(int argc, char **argv) {
    const char probe[] = "0";
    const char* first = probe;
    const char* last = first + sizeof(probe);

    {
        std::int8_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::int16_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::int32_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::int64_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::uint8_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::uint16_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::uint32_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
    {
        std::uint64_t value;
        auto rv = std::from_chars(first, last, value, 10);
    }
}
