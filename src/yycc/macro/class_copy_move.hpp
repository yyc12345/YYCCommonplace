#pragma once

/// @brief Explicitly remove copy (\c constructor and \c operator\=) for given class.
#define YYCC_DELETE_COPY(CLSNAME) \
    CLSNAME(const CLSNAME&) = delete; \
    CLSNAME& operator=(const CLSNAME&) = delete;

/// @brief Explicitly remove move (\c constructor and \c operator\=) for given class.
#define YYCC_DELETE_MOVE(CLSNAME) \
    CLSNAME(CLSNAME&&) noexcept = delete; \
    CLSNAME& operator=(CLSNAME&&) noexcept = delete;

/// @brief Explicitly remove (copy and move) (\c constructor and \c operator\=) for given class.
#define YYCC_DELETE_COPY_MOVE(CLSNAME) \
    YYCC_DELETE_COPY(CLSNAME) \
    YYCC_DELETE_MOVE(CLSNAME)

/// @brief Explicitly set default copy (\c constructor and \c operator\=) for given class.
#define YYCC_DEFAULT_COPY(CLSNAME) \
    CLSNAME(const CLSNAME&) = default; \
    CLSNAME& operator=(const CLSNAME&) = default;

/// @brief Explicitly set default move (\c constructor and \c operator\=) for given class.
#define YYCC_DEFAULT_MOVE(CLSNAME) \
    CLSNAME(CLSNAME&&) noexcept = default; \
    CLSNAME& operator=(CLSNAME&&) noexcept = default;

/// @brief Explicitly set default (copy and move) (\c constructor and \c operator\=) for given class.
#define YYCC_DEFAULT_COPY_MOVE(CLSNAME) \
    YYCC_DEFAULT_COPY(CLSNAME) \
    YYCC_DEFAULT_MOVE(CLSNAME)

/// @brief Make declaration of copy (\c constructor and \c operator\=) for given class to avoid typo.
#define YYCC_DECL_COPY(CLSNAME) \
    CLSNAME(const CLSNAME&); \
    CLSNAME& operator=(const CLSNAME&);

/// @brief Make declaration of move (\c constructor and \c operator\=) for given class to avoid typo.
#define YYCC_DECL_MOVE(CLSNAME) \
    CLSNAME(CLSNAME&&) noexcept; \
    CLSNAME& operator=(CLSNAME&&) noexcept;

/// @brief Make declaration of copy and move (\c constructor and \c operator\=) for given class to avoid typo.
#define YYCC_DECL_COPY_MOVE(CLSNAME) \
    YYCC_DECL_COPY(CLSNAME) \
    YYCC_DECL_MOVE(CLSNAME)

/// @brief Make implementation signature of copy \c constrctor for given class and right operand name to avoid typo.
#define YYCC_IMPL_COPY_CTOR(CLSNAME, RHS) \
    CLSNAME::CLSNAME(const CLSNAME& RHS)

/// @brief Make implementation signature of copy \c operator\= for given class and right operand name to avoid typo.
#define YYCC_IMPL_COPY_OPER(CLSNAME, RHS) \
    CLSNAME& CLSNAME::operator=(const CLSNAME& RHS)

/// @brief Make implementation signature of move \c constrctor for given class and right operand name to avoid typo.
#define YYCC_IMPL_MOVE_CTOR(CLSNAME, RHS) \
    CLSNAME::CLSNAME(CLSNAME&& RHS) noexcept

/// @brief Make implementation signature of move \c operator\= for given class and right operand name to avoid typo.
#define YYCC_IMPL_MOVE_OPER(CLSNAME, RHS) \
    CLSNAME& CLSNAME::operator=(CLSNAME&& RHS) noexcept
