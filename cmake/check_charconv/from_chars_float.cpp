#include <charconv>

int main(int argc, char **argv) {
    const char probe[] = "0.0";
    const char* first = probe;
    const char* last = first + sizeof(probe);

    {
        float value;
        auto rv = std::from_chars(first, last, value, std::chars_format::general);
    }
    {
        double value;
        auto rv = std::from_chars(first, last, value, std::chars_format::general);
    }
}
