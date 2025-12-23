#pragma once
#include "clap/types.hpp"
#include "clap/validator.hpp"
#include "clap/option.hpp"
#include "clap/variable.hpp"
#include "clap/summary.hpp"
#include "clap/application.hpp"
#include "clap/manual.hpp"
#include "clap/parser.hpp"
#include "clap/resolver.hpp"

// TODO: Support multiple arguments for single option.
/**
 * @brief Command Line Argument Parser (CLAP) module for handling command line arguments and environment variables.
 * @details This module provides a comprehensive system for defining, parsing, and validating command line
 * arguments and environment variables. It includes components for defining application metadata,
 * command line options, variables, and utilities for parsing and validation.
 * 
 * The main components include:
 * \li Types: Error types and result types used throughout the module
 * \li Validator: Type-safe validation for command line argument values
 * \li Option: Command line options with short and long names
 * \li Variable: Environment variables that can be captured
 * \li Summary: Application metadata (name, version, author, description)
 * \li Application: Complete application definition with options and variables
 * \li Manual: Help and version information generation
 * \li Parser: Command line argument parsing functionality
 * \li Resolver: Environment variable resolution functionality
 */
namespace yycc::carton::clap {}
