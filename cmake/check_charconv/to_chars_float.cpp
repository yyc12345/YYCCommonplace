#include <charconv>

int main(int argc, char **argv) {
    char buffer[1024];
    char* first = buffer;
    char* last = first + sizeof(buffer);

    {
        float value = 0;
        auto rv = std::to_chars(first, last, value, std::chars_format::general, 6);
    }
    {
        double value = 0;
        auto rv = std::to_chars(first, last, value, std::chars_format::general, 6);
    }
}
