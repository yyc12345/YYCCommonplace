#include <charconv>
#include <system_error>

int main(int argc, char **argv) {
    std::from_chars_result result {
        .ptr = nullptr,
        .ec = std::errc{},
    };
}
