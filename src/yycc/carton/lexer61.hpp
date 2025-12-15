#pragma once
#include "../macro/class_copy_move.hpp"
#include <expected>
#include <vector>
#include <string>
#include <string_view>

namespace yycc::carton::lexer61 {

    enum class LexerState { Space, Single, Double, Escape, Normal };

    // @brief Any error occurs when lexer working.
    enum class LexerError {
        UnexpectedEnd, ///< The end of command line is not expected.
    };

    template<typename T>
    using LexerResult = std::expected<T, LexerError>;

    class Lexer61 {
    public:
        Lexer61();
        ~Lexer61();
        YYCC_DEFAULT_COPY_MOVE(Lexer61)

    public:
        LexerResult<std::vector<std::u8string>> lex(const std::u8string_view& cmd);

    private:
        void reset();

        void proc_space();
        void proc_single();
        void proc_double();
        void proc_escape();
        void proc_normal();

        std::vector<std::u8string> m_ArgsCollection; ///< Internal result holder.
        std::u8string m_CurrentArg;                  ///< Holding current building commandline argument.
        char8_t m_CurrentChar;                       ///< Holding current char analysing.
        LexerState m_State;                          ///< Recording current state.
        LexerState m_PrevState;                      ///< Recording previous state.
    };

} // namespace yycc::carton::lexer61
