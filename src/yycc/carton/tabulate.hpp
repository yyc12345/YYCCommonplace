#pragma once
#include "../macro/class_copy_move.hpp"
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

namespace yycc::carton::tabulate {

    /**
     * @private
     * @brief Assistant class recording column width.
     */
    class TabulateWidth {
    public:
        /**
         * @brief Create width recorder with given column count.
         * @param[in] n Column count.
         */
        TabulateWidth(size_t n);
        ~TabulateWidth();
        YYCC_DEFAULT_COPY_MOVE(TabulateWidth)

    public:
        /**
         * @brief Get column count.
         * @return Column count.
         */
        size_t get_column_count() const;
        /**
         * @brief Get column width of given index.
         * @param[in] column_index Column index for fetching width.
         * @return Column width of given index.
         */
        size_t get_column_width(size_t column_index) const;
        /**
         * @brief Update column width of given index with given value.
         * @details The width of column will be updated to the maximum between given value and old value.
         * @param[in] column_index Column index for updating width.
         * @param[in] new_size New width value.
         */
        void update_column_width(size_t column_index, size_t new_size);
        /**
         * @brief Clear all width data
         * @details All width data will be reset to zero.
         */
        void clear();

    private:
        std::vector<size_t> widths;
    };

    /**
     * @private
     * @brief Assistant class holding table cell data.
     * @details
     * This class holds the data of table cell.
     * Also make a cache for the width this cell's text occupied in console,
     * to avoid duplicated calculation for occupied width.
     */
    class TabulateCell {
    public:
        /**
         * @brief Build cell with given text.
         * @param[in] text Data of cell.
         */
        TabulateCell(const std::u8string_view& text);
        ~TabulateCell();
        YYCC_DEFAULT_COPY_MOVE(TabulateCell)

    public:
        /**
         * @brief Get the text of cell.
         * @return The text of cell.
         */
        const std::u8string& get_text() const;
        /**
         * @brief Get width this cell's text occupied in console.
         * @return The width this cell occupied.
         */
        size_t get_text_width() const;

    private:
        /// @brief The data of cell.
        std::u8string text;
        /// @brief The width cache of this data occupied in console.
        size_t text_width;
    };

    /**
     * @private
     * @brief The type representing one row of data in table.
     */
    using Row = std::vector<TabulateCell>;
    /**
     * @private
     * @brief The type representing row collection in table.
     */
    using Rows = std::vector<Row>;

    /**
     * @brief Main class of Tabulate
     */
    class Tabulate {
    public:
        /**
         * @brief Create Tabulate class with given column count.
         * @details
         * In default, the separator bar of table is 3 dash.
         * Header and separator bar are also shown in default.
         * @param[in] n Column count of table.
         */
        Tabulate(size_t n);
        ~Tabulate();
        YYCC_DEFAULT_COPY_MOVE(Tabulate)

    public:
        /**
         * @brief Print table into given stream.
         * @details In default, stream is \c stdout.
         * @param[in] dst The stream printed into.
         */
        void print(std::ostream& dst = std::cout) const;

        /**
         * @brief Get the column count of table.
         * @return Column count of table.
         */
        size_t get_column_count() const;
        /**
         * @brief Change whether show table header when printing.
         * @param[in] show_header True for showing, otherwise false.
         */
        void show_header(bool show_header);
        /**
         * @brief Change whether show separator bar when printing.
         * @param[in] show_bar True for showing, otherwise false.
         */
        void show_bar(bool show_bar);
        /**
         * @brief Modify the prefix string of table.
         * @details
         * The prefix string of table is the string
         * which will be printed before each lines of output.
         * It is usually used for indent.
         * @param[in] prefix The prefix string.
         */
        void set_prefix(const std::u8string_view& prefix);
        /**
         * @brief Modify the header of table.
         * @param[in] hdr An initializer list holding header texts one by one.
         * @exception std::invalid_argument The size of given header is mismatch with column count.
         */
        void set_header(std::initializer_list<std::u8string_view> hdr);
        /**
         * @brief Modify separator bar string of table.
         * @param[in] bar New separator bar string.
         */
        void set_bar(const std::u8string_view& bar);
        /**
         * @brief Add one data row into table.
         * @param[in] row An initializer list holding row texts one by one.
         * @exception std::invalid_argument The size of given header is mismatch with column count.
         */
        void add_row(std::initializer_list<std::u8string_view> row);
        /**
         * @brief Clear all data rows of table.
         * @details Table header and separator bar will not be changed.
         */
        void clear();

    private:

        /// @brief The column count of table.
        size_t n;

        /// @brief Whether showing table header.
        bool header_display;
        /// @brief Whether showing table separator bar between header and data.
        bool bar_display;
        /// @brief The prefix string presented in each lines of output table.
        std::u8string prefix_string;

        /// @brief Width recorder for header.
        TabulateWidth header_widths;
        /// @brief Width recorder for data.
        TabulateWidth rows_widths;

        /// @brief The header of table.
        Row header;
        /// @brief The separator bar of table.
        TabulateCell bar;
        /// @brief The data of table.
        Rows rows;
    };

}
