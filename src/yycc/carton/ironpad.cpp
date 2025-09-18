#include "ironpad.hpp"
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"

#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

#include "../windows/winfct.hpp"
#include "../string/op.hpp"
#include "../string/reinterpret.hpp"
#include "../encoding/windows.hpp"
#include "../patch/ptr_pad.hpp"
#include "../patch/fopen.hpp"
#include "../macro/class_copy_move.hpp"
#include <filesystem>
#include <cstdarg>
#include <cstdio>
#include <cinttypes>
#include <mutex>

#include "../windows/import_guard_head.hpp"
#include <Windows.h>
#include <DbgHelp.h>
#include "../windows/import_guard_tail.hpp"

#define OP ::yycc::string::op
#define ENC ::yycc::encoding::windows
#define REINTERPRET ::yycc::string::reinterpret
#define WINFCT ::yycc::windows::winfct
#define FOPEN ::yycc::patch::fopen
using namespace std::literals::string_view_literals;

namespace yycc::carton::ironpad {

#pragma region Singleton Guard

    static LONG WINAPI unhandled_exception_handler(LPEXCEPTION_POINTERS);

    /**
     * @brief The "singleton" guard class for unhandled excepetion handler.
     * @details
     * The class making sure that there is only one unhandled exception handler was run in the same process,
     * when unhandled exception occurs, and prevent any futher possible recursive calling (exception in exception handler).
     */
    class SingletonGuard {
    public:
        SingletonGuard() :
            m_CoreMutex(), m_IsRegistered(false), m_IsProcessing(false), m_PrevProcHandler(nullptr), m_UserCallback(nullptr),
            m_SingletonMutex(NULL) {}
        ~SingletonGuard() { shutdown(); }
        YYCC_DELETE_COPY_MOVE(SingletonGuard)

    public:
        /**
		 * @brief Try to register unhandled exception handler.
         * @details There is no bad outcome when calling this function multiple times.
         * @return True if success, otherwise false.
		*/
        bool startup(ExceptionCallback callback) {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            // if we have registered, return
            if (m_IsRegistered) return false;

            // check singleton
            // build mutex string first
            auto mutex_name = OP::printf(u8"Global\\%" PRIu32 ".{61634294-d23c-43f9-8490-b5e09837eede}", GetCurrentProcessId());
            if (!mutex_name.has_value()) return false;
            auto w_mutex_name = ENC::to_wchar(mutex_name.value());
            if (!w_mutex_name.has_value()) return false;
            // create mutex
            m_SingletonMutex = CreateMutexW(NULL, FALSE, w_mutex_name.value().c_str());
            DWORD errcode = GetLastError();
            // check whether be created
            if (m_SingletonMutex == NULL) return false;
            if (errcode == ERROR_ALREADY_EXISTS) {
                CloseHandle(m_SingletonMutex);
                m_SingletonMutex = NULL;
                return false;
            }

            // okey, we can register it.
            // backup old handler
            m_PrevProcHandler = SetUnhandledExceptionFilter(unhandled_exception_handler);
            // set user callback
            m_UserCallback = callback;
            // mark registered
            m_IsRegistered = true;
        }
        /**
		 * @brief Try to unregister unhandled exception handler.
         * @details There is no bad outcome when calling this function multiple times.
         * @return True if success, otherwise false.
		*/
        bool shutdown() {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            // if we are not registered, skip
            if (!m_IsRegistered) return false;

            // unregister handler
            // reset user callback
            m_UserCallback = nullptr;
            // restore old handler
            SetUnhandledExceptionFilter(m_PrevProcHandler);
            m_PrevProcHandler = nullptr;

            // release singleton handler
            if (m_SingletonMutex != NULL) {
                CloseHandle(m_SingletonMutex);
                m_SingletonMutex = NULL;
            }

            // mark unregistered
            m_IsRegistered = false;
            return true;
        }

    public:
        /**
		 * @brief Check whether handler is registered.
		 * @return True if it is, otherwise false.
		*/
        bool is_registered() const {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            return m_IsRegistered;
        }
        /**
		 * @brief Check whether we are processing unhandled exception.
		 * @return True if it is, otherwise false.
		*/
        bool is_processing() const {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            return m_IsProcessing;
        }
        /**
		 * @brief Get the old unhandled exception handler before registering.
		 * @return The fucntion pointer to old unhandled exception handler. May be nullptr.
		*/
        LPTOP_LEVEL_EXCEPTION_FILTER get_prev_proc_handler() const {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            return m_PrevProcHandler;
        }
        /**
		 * @brief Get user specified callback.
		 * @return The function pointer to user callback. nullptr if no associated callback.
		*/
        ExceptionCallback get_user_callback() const {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            return m_UserCallback;
        }

        /**
		 * @brief Try to start process unhandled exception.
		 * @return True if you can start to process.
		 * False means there is already a process running. You should not process it now.
		*/
        bool start_processing() {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            if (m_IsProcessing) return false;
            else {
                m_IsProcessing = true;
                return true;
            }
        }
        /**
		 * @brief Mark current process of unhandled exception has done.
		 * @details This should only be called when start_processing() return true.
		*/
        void stop_processing() {
            std::lock_guard<std::mutex> locker(m_CoreMutex);
            m_IsProcessing = false;
        }

    private:
        /**
		 * @brief The core mutex for keeping this class is in synchronized.
		*/
        mutable std::mutex m_CoreMutex;

        /**
		 * @brief Whether we have registered unhandled exception handler.
		 * True if it is, otherwise false.
		*/
        bool m_IsRegistered;
        /**
		 * @brief Whether we are processing unhandled exception.
		 * True if it is, otherwise false.
		*/
        bool m_IsProcessing;
        /**
		 * @brief User defined callback.
		 * @details It will be called at the tail of unhandled exception handler, because it may raise exception.
		 * We must make sure all log and coredump have been done before calling it.
		*/
        ExceptionCallback m_UserCallback;
        /**
		 * @brief The backup of old unhandled exception handler.
		*/
        LPTOP_LEVEL_EXCEPTION_FILTER m_PrevProcHandler;
        /**
		 * @brief The Windows mutex handle for singleton implementation.
		 * Because we may have many DLLs using YYCC in the same process.
		 * But the unhandled exception handler only need to be registered once.
		*/
        HANDLE m_SingletonMutex;
    };

    /// @brief Core register singleton.
    static SingletonGuard g_SingletonGuard;

#pragma endregion

#pragma region Exception Dumper

    class ExceptionDumper {
    public:
        ExceptionDumper() {}
        ~ExceptionDumper() {}
        YYCC_DELETE_COPY_MOVE(ExceptionDumper)

    public:
        CallbackInfo execute(LPEXCEPTION_POINTERS info) {
            // fetch file path first
            auto log_path = build_file_path(FileKind::LogFile);
            auto coredump_path = build_file_path(FileKind::CoredumpFile);
            // and report their status
            if (log_path.has_value()) {
                log_format_line(nullptr, u8"Crash Log: %s", log_path.value().c_str());
            } else {
                log_write_line(nullptr, u8"Crash occurs, but we can not create crash log!");
            }
            if (coredump_path.has_value()) {
                log_format_line(nullptr, u8"Crash Coredump: %s", coredump_path.value().c_str());
            } else {
                log_write_line(nullptr, u8"Crash occurs, but we can not create coredump!");
            }

            // Write crash log
            {
                // open file stream if possible
                FOPEN::SmartStdFile fs(nullptr);
                if (log_path.has_value()) {
                    fs.reset(FOPEN::fopen(log_path.value().c_str(), u8"wb"));
                }

                // output basic infos
                // record exception type first
                PEXCEPTION_RECORD rec = info->ExceptionRecord;
                log_format_line(fs.get(),
                                u8"Unhandled exception occured at 0x%" PRIXPTR_LPAD PRIXPTR ": %s (%" PRIu32 ").",
                                rec->ExceptionAddress,
                                get_code_message(rec->ExceptionCode),
                                rec->ExceptionCode);
                // special proc for 2 exceptions
                if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || rec->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
                    if (rec->NumberParameters >= 2) {
                        const char8_t* op = rec->ExceptionInformation[0] == 0   ? u8"read"
                                            : rec->ExceptionInformation[0] == 1 ? u8"written"
                                                                                : u8"executed";
                        log_format_line(fs.get(),
                                        u8"The data at memory address 0x%" PRIXPTR_LPAD PRIxPTR " could not be %s.",
                                        rec->ExceptionInformation[1],
                                        op);
                    }
                }

                // output stacktrace
                do_backtrace(fs.get(), info->ContextRecord, 1024);
            }

            // Write coredump
            if (coredump_path.has_value()) {
                do_coredump(coredump_path.value(), info);
            }

            // return path for user callback
            return CallbackInfo{.log_path = std::move(log_path), .coredump_path = std::move(coredump_path)};
        }

    private:
        /**
         * @brief All kind of dumped files.
         */
        enum class FileKind { LogFile, CoredumpFile };
        /**
         * @brief Build path to file stored on disk including exception data with given kind.
         * @param[in] kind The kind of file to be built.
         * @return The built path or nothing if error occurs.
         */
        std::optional<std::u8string> build_file_path(FileKind kind) {
            // build file names like: "error.exe.1234.log" and "error.exe.1234.dmp".
            // "error.exe" is the name of current process. "1234" is current process id.

            // get process name
            std::u8string u8_process_name;
            {
                // get full path of process
                auto u8_process_path = WINFCT::get_module_file_name(NULL);
                if (!u8_process_path.has_value()) return std::nullopt;
                // extract file name from full path by std::filesystem::path
                std::filesystem::path process_path(u8_process_path.value());
                u8_process_name = process_path.filename().u8string();
            }
            // then get process id
            DWORD process_id = GetCurrentProcessId();
            // conbine them as a file name prefix
            auto u8_filename_prefix = OP::printf(u8"%s.%" PRIu32, u8_process_name.c_str(), process_id);
            if (!u8_filename_prefix.has_value()) return std::nullopt;
            // then get file name for log and minidump
            std::u8string u8_filename;
            switch (kind) {
                case FileKind::LogFile:
                    u8_filename = u8_filename_prefix.value() + u8".log";
                    break;
                case FileKind::CoredumpFile:
                    u8_filename = u8_filename_prefix.value() + u8".dmp";
                    break;
                default:
                    u8_filename = u8_filename_prefix.value();
                    break;
            }

            // fetch crash report path
            // get local appdata folder
            auto u8_localappdata_path = WINFCT::get_known_path(WINFCT::KnownDirectory::LocalAppData);
            if (!u8_localappdata_path.has_value()) return std::nullopt;
            // convert to std::filesystem::path
            std::filesystem::path crashreport_path(u8_localappdata_path.value());
            // slash into crash report folder
            crashreport_path /= u8"IronPad";
            // use create function to make sure it is existing
            std::filesystem::create_directories(crashreport_path);

            // build log path and coredump path
            // build std::filesystem::path first
            std::filesystem::path file_path = crashreport_path / u8_filename;
            // output to result
            return file_path.u8string();
        }

        /**
         * @brief Do stack trace for given exception.
         * @details
         * This function will do stack trace with given maximum depth as much as possible
         * and output trace info into given file stream and \c stderr.
         */
        void do_backtrace(FILE* fs, LPCONTEXT context, int maxdepth) {
            // setup loading symbol options
            SymSetOptions(SymGetOptions() | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES); // lazy load symbol, and load line number.

            // setup handle
            HANDLE process = GetCurrentProcess();
            HANDLE thread = GetCurrentThread();

            // init symbol
            if (!SymInitialize(process, 0, TRUE)) {
                // fail to init. return
                log_write_line(fs, u8"Fail to initialize symbol handle for process!");
                return;
            }

            // ========== CORE DUMP ==========
            // prepare frame. setup correct fields
            // references:
            // https://github.com/rust-lang/backtrace-rs/blob/9ed25b581cfd2ee60e5a3b9054fd023bf6dced90/src/backtrace/dbghelp.rs
            // https://sourceforge.net/p/predef/wiki/Architectures/
            DWORD machine_type = 0;
            STACKFRAME64 frame;
            memset(&frame, 0, sizeof(frame));
#if defined(_M_IX86) || defined(__i386__)
            // x86
            machine_type = IMAGE_FILE_MACHINE_I386;
            frame.AddrPC.Offset = context->Eip;
            frame.AddrStack.Offset = context->Esp;
            frame.AddrFrame.Offset = context->Ebp;
#elif defined(_M_AMD64) || defined(__amd64__)
            // amd64
            machine_type = IMAGE_FILE_MACHINE_AMD64;
            frame.AddrPC.Offset = context->Rip;
            frame.AddrStack.Offset = context->Rsp;
            frame.AddrFrame.Offset = context->Rbp;
#elif defined(_M_ARM) || defined(__arm__)
            // arm (32bit)
            machine_type = IMAGE_FILE_MACHINE_ARMNT;
            frame.AddrPC.Offset = context->Pc;
            frame.AddrStack.Offset = context->Sp;
            frame.AddrFrame.Offset = context->R11;
#elif defined(_M_ARM64) || defined(__aarch64__)
            // arm64
            machine_type = IMAGE_FILE_MACHINE_ARM64;
            frame.AddrPC.Offset = context->Pc;
            frame.AddrStack.Offset = context->Sp;
            frame.AddrFrame.Offset = context->DUMMYUNIONNAME.DUMMYSTRUCTNAME.Fp;
#else
#error "Unsupported platform"
            //IA-64 anybody?

#endif
            frame.AddrPC.Mode = AddrModeFlat;
            frame.AddrStack.Mode = AddrModeFlat;
            frame.AddrFrame.Mode = AddrModeFlat;

            // stack walker
            while (StackWalk64(machine_type, process, thread, &frame, context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
                // depth breaker
                --maxdepth;
                if (maxdepth < 0) {
                    log_write_line(fs, u8"..."); // indicate there are some frames not listed
                    break;
                }

                // get module name
                std::u8string module_name(u8"<unknown module>");
                DWORD64 module_base;
                if (module_base = SymGetModuleBase64(process, frame.AddrPC.Offset)) {
                    auto rv = WINFCT::get_module_file_name((HINSTANCE) module_base);
                    if (rv.has_value()) module_name = rv.value();
                }

                // get source file and line
                const char8_t* source_file = u8"<unknown source>";
                DWORD64 source_file_line = 0;
                DWORD dwDisplacement;
                IMAGEHLP_LINE64 winline;
                winline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &dwDisplacement, &winline)) {
                    source_file = REINTERPRET::as_utf8(winline.FileName); // TODO: check whether there is UNICODE file name.
                    source_file_line = winline.LineNumber;
                }

                // write to file
                // MARK: should not use PRIXPTR to print adddress.
                // because Windows always use DWORD64 as the type of address.
                // use PRIX64 instead.
                log_format_line(fs,
                                u8"0x%" PRIXPTR_LPAD PRIX64 "[%s+0x%" PRIXPTR_LPAD PRIX64 "]\t%s#L%" PRIu64,
                                frame.AddrPC.Offset, // memory adress
                                module_name.c_str(),
                                frame.AddrPC.Offset - module_base, // module name + relative address
                                source_file,
                                source_file_line // source file + source line
                );
            }

            // ========== END CORE DUMP ==========

            // free symbol
            SymCleanup(process);
        }

        /**
         * @brief Do coredump for given exception.
         * @details This function will write coredump of given exception into given file path.
         */
        void do_coredump(const std::u8string_view& u8_filename, LPEXCEPTION_POINTERS info) {
            // convert file encoding
            // if convertion failed, return
            auto filename_rv = ENC::to_wchar(u8_filename);
            if (!filename_rv.has_value()) return;
            std::wstring filename = filename_rv.value();

            // open file and write
            HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                MINIDUMP_EXCEPTION_INFORMATION exception_info;
                exception_info.ThreadId = GetCurrentThreadId();
                exception_info.ExceptionPointers = info;
                exception_info.ClientPointers = TRUE;
                MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &exception_info, NULL, NULL);
                CloseHandle(hFile);
            }
        }

    private:
        /**
	     * @brief Get human-readable exception string from given exception code.
	     * @param[in] code Exception code
	     * @return The string view to corresponding exception explanation string.
	    */
        const std::u8string_view get_code_message(DWORD code) {
            switch (code) {
                case EXCEPTION_ACCESS_VIOLATION:
                    return u8"access violation"sv;
                case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
                    return u8"array index out of bound"sv;
                case EXCEPTION_BREAKPOINT:
                    return u8"breakpoint reached"sv;
                case EXCEPTION_DATATYPE_MISALIGNMENT:
                    return u8"misaligned data access"sv;
                case EXCEPTION_FLT_DENORMAL_OPERAND:
                    return u8"operand had denormal value"sv;
                case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                    return u8"floating-point division by zero"sv;
                case EXCEPTION_FLT_INEXACT_RESULT:
                    return u8"no decimal fraction representation for value"sv;
                case EXCEPTION_FLT_INVALID_OPERATION:
                    return u8"invalid floating-point operation"sv;
                case EXCEPTION_FLT_OVERFLOW:
                    return u8"floating-point overflow"sv;
                case EXCEPTION_FLT_STACK_CHECK:
                    return u8"floating-point stack corruption"sv;
                case EXCEPTION_FLT_UNDERFLOW:
                    return u8"floating-point underflow"sv;
                case EXCEPTION_ILLEGAL_INSTRUCTION:
                    return u8"illegal instruction"sv;
                case EXCEPTION_IN_PAGE_ERROR:
                    return u8"inaccessible page"sv;
                case EXCEPTION_INT_DIVIDE_BY_ZERO:
                    return u8"integer division by zero"sv;
                case EXCEPTION_INT_OVERFLOW:
                    return u8"integer overflow"sv;
                case EXCEPTION_INVALID_DISPOSITION:
                    return u8"documentation says this should never happen"sv;
                case EXCEPTION_NONCONTINUABLE_EXCEPTION:
                    return u8"can't continue after a noncontinuable exception"sv;
                case EXCEPTION_PRIV_INSTRUCTION:
                    return u8"attempted to execute a privileged instruction"sv;
                case EXCEPTION_SINGLE_STEP:
                    return u8"one instruction has been executed"sv;
                case EXCEPTION_STACK_OVERFLOW:
                    return u8"stack overflow"sv;
                default:
                    return u8"unknown exception"sv;
            }
        }

        /**
	     * @brief Write error message
	     * @details
	     * This function will write given string into given file stream and \c stderr.
	     * @param[in] fs
	     * The file stream where we write.
	     * If it is nullptr, function will skip writing for file stream.
	     * @param[in] strl The string to be written.
	    */
        void log_write_line(std::FILE* fs, const char8_t* strl) {
            // write to file
            if (fs != nullptr) {
                std::fputs(REINTERPRET::as_ordinary(strl), fs);
                std::fputs("\n", fs);
            }
            // write to stderr
            std::fputs(REINTERPRET::as_ordinary(strl), stderr);
            std::fputs("\n", stderr);
        }

        /**
	     * @brief Format error message.
	     * @details
	     * This function will format message first.
	     * And write them into given file stream and \c stderr.
	     * @param[in] fs
	     * The file stream where we write.
	     * If it is nullptr, function will skip writing for file stream.
	     * @param[in] fmt The format string.
	     * @param[in] ... The argument to be formatted.
	    */
        void log_format_line(std::FILE* fs, const char8_t* fmt, ...) {
            // do format first
            va_list arg;
            va_start(arg, fmt);
            auto fmt_rv = OP::vprintf(fmt, arg);
            va_end(arg);
            // write to file and console
            if (fmt_rv.has_value()) {
                log_write_line(fs, fmt_rv.value().c_str());
            }
        }
    };

#pragma endregion

#pragma region Core Implementation

    static LONG WINAPI unhandled_exception_handler(LPEXCEPTION_POINTERS info) {
        // try to start process current unhandled exception
        // to prevent any possible recursive calling.
        if (!g_SingletonGuard.start_processing()) {
            // process exception
            ExceptionDumper dumper;
            auto pathinfo = dumper.execute(info);

            // call user callback
            ExceptionCallback user_callback = g_SingletonGuard.get_user_callback();
            if (user_callback != nullptr) user_callback(pathinfo);

            // stop process
            g_SingletonGuard.stop_processing();
        }

        // if backup proc can be run, run it
        // otherwise directly return.
        auto prev_proc = g_SingletonGuard.get_prev_proc_handler();
        if (prev_proc != nullptr) {
            return prev_proc(info);
        } else {
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }

#pragma endregion

} // namespace yycc::carton::ironpad

#endif

#pragma region Exposed Function
namespace yycc::carton::ironpad {

    bool startup(ExceptionCallback callback) {
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)
        return g_SingletonGuard.startup(callback);
#else
        // Do nothing
        return false;
#endif
    }

    void shutdown() {
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)
        g_SingletonGuard.shutdown();
#else
        // Do nothing
#endif
    }

} // namespace yycc::carton::ironpad
#pragma endregion
