#pragma once
#include "binstore/types.hpp"
#include "binstore/serdes.hpp"
#include "binstore/setting.hpp"
#include "binstore/configuration.hpp"
#include "binstore/storage.hpp"

/**
 * @brief The main namespace for the binstore module.
 * @details
 * The binstore module provides a binary settings storage system that allows
 * applications to persistently store and retrieve configuration settings in
 * a binary format. It includes functionality for:
 * 
 * \li Type-safe serialization and deserialization of various data types
 *     (integers, floating-point numbers, booleans, strings, enums)
 * \li Setting management with unique tokens for access control
 * \li Version control for configuration data with migration strategies
 * \li File and stream-based storage operations
 * \li Comprehensive error handling for robust operation
 * 
 * The main components are:
 * \li types: Basic types and error handling for the module
 * \li serdes: Serialization/deserialization functionality for different data types
 * \li setting: Management of settings with name-based lookup and token-based access
 * \li configuration: Version and settings collection management
 * \li storage: Main storage class for loading/saving settings to/from files or streams
 */
namespace yycc::carton::binstore {}
