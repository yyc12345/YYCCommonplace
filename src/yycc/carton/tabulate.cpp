#include "tabulate.hpp"
#include "wcwidth.hpp"
#include "../num/safe_op.hpp"
#include "../patch/stream.hpp"
#include "../patch/libcxx/enumerate.hpp"
#include <stdexcept>
#include <ranges>

#define WCWIDTH ::yycc::carton::wcwidth
#define SAFEOP ::yycc::num::safe_op

using namespace yycc::patch::stream;

namespace yycc::carton::tabulate {

#pragma region Tabulate Width

    TabulateWidth::TabulateWidth(size_t n) : widths(n, 0u) {}

    TabulateWidth::~TabulateWidth() {}

    size_t TabulateWidth::get_column_count() const {
        return widths.size();
    }

    size_t TabulateWidth::get_column_width(size_t column_index) const {
        return widths.at(column_index);
    }

    void TabulateWidth::update_column_width(size_t column_index, size_t new_size) {
        auto& width = widths.at(column_index);
        width = std::max(width, new_size);
    }

    void TabulateWidth::clear() {
        std::fill(widths.begin(), widths.end(), 0u);
    }

#pragma endregion

#pragma region Tabulate Cell

    TabulateCell::TabulateCell(const std::u8string_view& text) : text(text), text_width(WCWIDTH::wcswidth(text).value_or(0u)) {}

    TabulateCell::~TabulateCell() {}

    const std::u8string& TabulateCell::get_text() const {
        return text;
    }

    size_t TabulateCell::get_text_width() const {
        return text_width;
    }

#pragma endregion

#pragma region Tabulate

    /// @brief Default separator literal for Tabulate.
    static constexpr char8_t SEPARATOR_BAR[] = u8"---";
    /// @brief A stupid size_t ZERO literal to trigger template type deduce for std::views::iota.
    static constexpr size_t ZERO = 0;

    Tabulate::Tabulate(size_t n) :
        n(n), header_display(true), bar_display(true), prefix_string(), rows_widths(n), header_widths(n), header(n, TabulateCell(u8"")),
        bar(SEPARATOR_BAR), rows() {}

    Tabulate::~Tabulate() {}

    void Tabulate::print(std::ostream& dst) const {
        // Get column count
        auto n = this->get_column_count();

        // Create width recorder for final printing
        // according to whether we show table header and separator bar.
        auto widths = this->rows_widths;
        if (this->header_display) {
            for (auto index : std::views::iota(ZERO, n)) {
                widths.update_column_width(index, this->header_widths.get_column_width(index));
            }
        }
        if (this->bar_display) {
            auto bar_width = this->bar.get_text_width();
            for (auto index : std::views::iota(ZERO, n)) {
                widths.update_column_width(index, bar_width);
            }
        }

        // Get the maximum space char count to build a string filled with spaces,
        // for the convenient about following printing.
        size_t max_space = 1;
        for (auto index : std::views::iota(ZERO, n)) {
            max_space = std::max(max_space, widths.get_column_width(index));
        }
        std::u8string spaces(max_space, u8' ');
        std::u8string_view spaces_view(spaces);

        // Print table
        // Show header
        if (this->header_display) {
            dst << this->prefix_string;
            for (const auto [index, item] : std::views::enumerate(header)) {
                auto diff = SAFEOP::saturating_sub(widths.get_column_width(index), item.get_text_width());
                dst << item.get_text() << spaces_view.substr(0, diff) << " ";
            }
            dst << std::endl;
        }
        // Show bar
        if (this->bar_display) {
            dst << this->prefix_string;
            auto bar_width = this->bar.get_text_width();
            for (auto index : std::views::iota(ZERO, n)) {
                auto diff = SAFEOP::saturating_sub(widths.get_column_width(index), bar_width);
                dst << this->bar.get_text() << spaces_view.substr(0, diff) << " ";
            }
            dst << std::endl;
        }
        // Show data
        for (const auto& row : this->rows) {
            dst << this->prefix_string;
            for (const auto [index, item] : std::views::enumerate(row)) {
                auto diff = SAFEOP::saturating_sub(widths.get_column_width(index), item.get_text_width());
                dst << item.get_text() << spaces_view.substr(0, diff) << " ";
            }
            dst << std::endl;
        }
    }

    size_t Tabulate::get_column_count() const {
        return this->n;
    }

    void Tabulate::show_header(bool show_header) {
        this->header_display = show_header;
    }

    void Tabulate::show_bar(bool show_bar) {
        this->bar_display = show_bar;
    }

    void Tabulate::set_prefix(const std::u8string_view& prefix) {
        this->prefix_string = prefix;
    }

    void Tabulate::set_header(std::initializer_list<std::u8string_view> hdr) {
        // Check data size.
        if (hdr.size() != get_column_count()) {
            throw std::invalid_argument("the size of given header is not equal to column count");
        }

        // Change header data and update header width recorder.
        header.clear();
        header_widths.clear();
        for (const auto [index, item] : std::views::enumerate(hdr)) {
            auto cell = header.emplace_back(item);
            header_widths.update_column_width(index, cell.get_text_width());
        }
    }

    void Tabulate::set_bar(const std::u8string_view& bar) {
        this->bar = bar;
    }

    void Tabulate::add_row(std::initializer_list<std::u8string_view> row) {
        // Check data size.
        if (row.size() != get_column_count()) {
            throw std::invalid_argument("the size of given row is not equal to column count");
        }

        // Prepare inserted row, and update data width recorder.
        Row inserted_row;
        inserted_row.reserve(row.size());
        for (const auto [index, item] : std::views::enumerate(row)) {
            auto cell = inserted_row.emplace_back(item);
            rows_widths.update_column_width(index, cell.get_text_width());
        }

        // Insert row
        rows.emplace_back(std::move(inserted_row));
    }

    void Tabulate::clear() {
        // Clear data and data width recorder.
        rows.clear();
        rows_widths.clear();
    }

#pragma endregion

} // namespace yycc::carton::tabulate
