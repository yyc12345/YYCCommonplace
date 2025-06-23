#pragma once
#include "../macro/feature_probe.hpp"
#include <string_view>

#if defined(YYCC_CPPFEAT_FORMAT)
#include <format>
#endif

/// @brief
namespace yycc::rust::panic {

    // TODO: Move these comments into Doxygen comments of this namespace.

    // YYC MARK:
    // 在使用Rust编写程序后，我深刻地认识到，将错误立即进行处理的必要性。
    // 而在陷入不可恢复错误后，也应当立即退出程序并报告错误，这是对程序健壮性的保证。
    // 因此我引入了这个命名空间，并带来了与Rust中panic!宏等效的宏和函数。

    // YYC MARK:
    // 遗憾的是，我无法改变标准库中的异常机制。
    // 标准库中会抛出异常的内容仍然会抛出异常，我不能阻止它们。
    // 因此我在这里规定，在Stardust项目中，任何C++异常都应立即被视为错误，并导致程序崩溃退出。
    // 也正因为于此，规定不允许注册任何未处理错误的回调，以防止意料之外的继续运行。
    // 而对于我们自己编写的，能控制的代码，则应用本文件中提供的宏来代替异常抛出。
    // 这样一来，我们代码中的非预期行为会使得程序立即退出，并输出错误信息和堆栈。
    // 而标准库中的异常，也会使得程序退出，只不过没有堆栈信息罢了。

    /**
     * @brief 像Rust的panic!宏一样立即使整个程序崩溃。
     * @details 宏参数为需要附加的提示信息。
     */
#define RS_PANIC(msg) ::yycc::rust::panic::panic(__FILE__, __LINE__, (msg))

    /**
     * @brief 像Rust的panic!宏一样立即使整个程序崩溃。
     * @details
     * 宏参数为需要格式化的信息和参数，格式和参数请参考std::format。
     * 因为本质上这个宏就是调用std::format来实现的。
     */
#if defined(YYCC_CPPFEAT_FORMAT)
#if defined(YYCC_CPPFEAT_VA_OPT)
#define RS_PANICF(msg, ...) RS_PANIC(std::format(msg __VA_OPT__(, ) __VA_ARGS__))
#else
#define RS_PANICF(msg, ...) RS_PANIC(std::format(msg, ##__VA_ARGS__))
#endif
#endif

    /**
     * @brief 像Rust的panic!宏一样立即使整个程序崩溃。
     * @details
     * 此函数是宏所调用的真正的崩溃输出函数。
     * 崩溃信息将被写入到stderr中，信息中还包含堆栈信息。
     * @param[in] file Panic时的源代码文件。该参数一般由宏代为填写。
     * @param[in] line Panic时的源代码文件中的行号。该参数一般由宏代为填写。
     * @param[in] msg Panic时需要显示的提示信息。
     */
    [[noreturn]] void panic(const char* file, int line, const std::string_view& msg);

} // namespace yycc::rust::panic
