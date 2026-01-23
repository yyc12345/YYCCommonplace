#include <charconv>
#include <system_error>

int main(int argc, char **argv) {
    std::to_chars_result result {
        .ptr = nullptr,
        .ec = std::errc{},
    };
}
