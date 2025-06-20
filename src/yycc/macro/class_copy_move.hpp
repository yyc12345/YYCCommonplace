#pragma once

/// @brief Explicitly remove copy (\c constructor and \c operator\=) for given class.
#define YYCC_DELETE_COPY(CLSNAME) \
    CLSNAME(const CLSNAME&) = delete; \
    CLSNAME& operator=(const CLSNAME&) = delete;

/// @brief Explicitly remove move (\c constructor and \c operator\=) for given class.
#define YYCC_DELETE_MOVE(CLSNAME) \
    CLSNAME(CLSNAME&&) = delete; \
    CLSNAME& operator=(CLSNAME&&) = delete;

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
    CLSNAME(CLSNAME&&) = default; \
    CLSNAME& operator=(CLSNAME&&) = default;

/// @brief Explicitly set default (copy and move) (\c constructor and \c operator\=) for given class.
#define YYCC_DEFAULT_COPY_MOVE(CLSNAME) \
    YYCC_DEFAULT_COPY(CLSNAME) \
    YYCC_DEFAULT_MOVE(CLSNAME)
