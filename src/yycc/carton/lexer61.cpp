#include "lexer61.hpp"

namespace yycc::carton::lexer61 {

    Lexer61::Lexer61() :
        m_ArgsCollection(), m_CurrentArg(), m_CurrentChar(u8'\0'), m_State(LexerState::Space), m_PrevState(LexerState::Space) {}

    Lexer61::~Lexer61() {}

    LexerResult<std::vector<std::u8string>> Lexer61::lex(const std::u8string_view &cmd) {
        // Clear variables when we start a new lex.
        this->reset();

        // Iterate all chars in commandline one by one.
        for (char8_t c : cmd) {
            m_CurrentChar = c;

            // YYC MARK:
            // All invalid characters should be passed directly to handlers.
            // Because of this, all handler should care this case.
            // After this, UTF8 code unit can directly accepted.
            switch (m_State) {
                case LexerState::Space:
                    proc_space();
                    break;
                case LexerState::Single:
                    proc_single();
                    break;
                case LexerState::Double:
                    proc_double();
                    break;
                case LexerState::Escape:
                    proc_escape();
                    break;
                case LexerState::Normal:
                    proc_normal();
                    break;
            }
        }

        // All chars has been processed.
        // Check the final state.
        bool okey = false;
        switch (m_State) {
            case LexerState::Space:
                // Space state is okey.
                okey = true;
                break;
            case LexerState::Normal:
                // In normal state, we need push current argument into collection,
                // and we can back to space state.
                m_ArgsCollection.emplace_back(m_CurrentArg);
                okey = true;
                break;
            case LexerState::Single:
            case LexerState::Double:
            case LexerState::Escape:
                // Any other states is not expected.
                okey = false;
                break;
        }

        // Check success flag
        if (okey) {
            return this->m_ArgsCollection;
        } else {
            return std::unexpected(LexerError::UnexpectedEnd);
        }
    }

    void Lexer61::reset() {
        // Because these value may be moved, so we need assign them with new value,
        // rather clear them.
        m_ArgsCollection = decltype(Lexer61::m_ArgsCollection)();
        m_CurrentArg = decltype(Lexer61::m_CurrentArg)();
        // Set other values.
        m_CurrentChar = u8'\0';
        m_State = m_PrevState = LexerState::Space;
    }

    void Lexer61::proc_space() {
        switch (m_CurrentChar) {
            case u8'\'':
                m_State = LexerState::Single;
                break;
            case u8'"':
                m_State = LexerState::Double;
                break;
            case u8'\\':
                m_State = LexerState::Escape;
                m_PrevState = LexerState::Normal;
                break;
            case u8' ':
                // Skip blank
                break;
            default:
                m_CurrentArg.push_back(m_CurrentChar);
                m_State = LexerState::Normal;
                break;
        }
    }

    void Lexer61::proc_single() {
        switch (m_CurrentChar) {
            case u8'\'':
                m_State = LexerState::Normal;
                break;
            case u8'"':
                m_CurrentArg.push_back('"');
                break;
            case u8'\\':
                m_State = LexerState::Escape;
                m_PrevState = LexerState::Single;
                break;
            case u8' ':
                m_CurrentArg.push_back(u8' ');
                break;
            default:
                m_CurrentArg.push_back(m_CurrentChar);
                break;
        }
    }

    void Lexer61::proc_double() {
        switch (m_CurrentChar) {
            case u8'\'':
                m_CurrentArg.push_back(u8'\'');
                break;
            case u8'"':
                m_State = LexerState::Normal;
                break;
            case u8'\\':
                m_State = LexerState::Escape;
                m_PrevState = LexerState::Double;
                break;
            case u8' ':
                m_CurrentArg.push_back(u8' ');
                break;
            default:
                m_CurrentArg.push_back(m_CurrentChar);
                break;
        }
    }

    void Lexer61::proc_escape() {
        // Add itself
        m_CurrentArg.push_back(m_CurrentChar);
        // And restore state
        m_State = m_PrevState;
    }

    void Lexer61::proc_normal() {
        switch (m_CurrentChar) {
            case u8'\'':
                m_CurrentArg.push_back(u8'\'');
                break;
            case u8'"':
                m_CurrentArg.push_back(u8'"');
                break;
            case u8'\\':
                m_State = LexerState::Escape;
                m_PrevState = LexerState::Normal;
                break;
            case u8' ':
                m_ArgsCollection.emplace_back(m_CurrentArg);
                m_CurrentArg.clear();
                m_State = LexerState::Space;
                break;
            default:
                m_CurrentArg.push_back(m_CurrentChar);
                break;
        }
    }

} // namespace yycc::carton::lexer61
