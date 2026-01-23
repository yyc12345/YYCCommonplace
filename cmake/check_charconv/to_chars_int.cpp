#include <charconv>
#include <cstdint>

int main(int argc, char **argv) {
    char buffer[1024];
    char* first = buffer;
    char* last = first + sizeof(buffer);

    {
        std::int8_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::int16_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::int32_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::int64_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::uint8_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::uint16_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::uint32_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
    {
        std::uint64_t value = 0;
        auto rv = std::to_chars(first, last, value, 10);
    }
}
