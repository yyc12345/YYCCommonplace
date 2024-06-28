#include "ExceptionHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "WinFctHelper.hpp"
#include "ConsoleHelper.hpp"
#include "StringHelper.hpp"
#include "IOHelper.hpp"
#include "EncodingHelper.hpp"
#include "FsPathPatch.hpp"
#include <filesystem>
#include <cstdarg>
#include <cstdio>
#include <cinttypes>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <DbgHelp.h>
#include "WinImportSuffix.hpp"

namespace YYCC::ExceptionHelper {

	/**
	 * @brief True if the exception handler already registered, otherwise false.
	 * @details
	 * This variable is designed to prevent multiple register operation
	 * because unhandled exception handler should only be registered once.
	 * \n
	 * Register function should check whether this variable is false before registering,
	 * and set this variable to true after registing.
	 * Unregister as well as should do the same check.
	*/
	static bool g_IsRegistered = false;
	/**
	 * @brief True if a exception handler is running, otherwise false.
	 * @details
	 * This variable is served for blocking possible infinity recursive exception handling.
	 * \n
	 * When entering unhandled exception handler, we must check whether this variable is true.
	 * If it is true, it mean that there is another unhandled exception handler running.
	 * Then we should exit immediately.
	 * Otherwise, this variable should be set to true indicating we are processing unhandled exception.
	 * After processing exception, at the end of unhandled exception handler,
	 * we should restore this value to false.
	 *
	*/
	static bool g_IsProcessing = false;
	/**
	 * @brief The backup of original exception handler.
	 * @details
	 * This variable was set when registering unhandled exception handler.
	 * And will be used when unregistering for restoring.
	*/
	static LPTOP_LEVEL_EXCEPTION_FILTER g_ProcBackup;

#pragma region Exception Handler Implementation

	/**
	 * @brief Get human-readable exception string from given exception code.
	 * @param[in] code Exception code
	 * @return The const string pointer to corresponding exception explanation string.
	*/
	static const yycc_char8_t* UExceptionGetCodeName(DWORD code) {
		switch (code) {
			case EXCEPTION_ACCESS_VIOLATION:
				return YYCC_U8("access violation");
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				return YYCC_U8("array index out of bound");
			case EXCEPTION_BREAKPOINT:
				return YYCC_U8("breakpoint reached");
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				return YYCC_U8("misaligned data access");
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				return YYCC_U8("operand had denormal value");
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				return YYCC_U8("floating-point division by zero");
			case EXCEPTION_FLT_INEXACT_RESULT:
				return YYCC_U8("no decimal fraction representation for value");
			case EXCEPTION_FLT_INVALID_OPERATION:
				return YYCC_U8("invalid floating-point operation");
			case EXCEPTION_FLT_OVERFLOW:
				return YYCC_U8("floating-point overflow");
			case EXCEPTION_FLT_STACK_CHECK:
				return YYCC_U8("floating-point stack corruption");
			case EXCEPTION_FLT_UNDERFLOW:
				return YYCC_U8("floating-point underflow");
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				return YYCC_U8("illegal instruction");
			case EXCEPTION_IN_PAGE_ERROR:
				return YYCC_U8("inaccessible page");
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				return YYCC_U8("integer division by zero");
			case EXCEPTION_INT_OVERFLOW:
				return YYCC_U8("integer overflow");
			case EXCEPTION_INVALID_DISPOSITION:
				return YYCC_U8("documentation says this should never happen");
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				return YYCC_U8("can't continue after a noncontinuable exception");
			case EXCEPTION_PRIV_INSTRUCTION:
				return YYCC_U8("attempted to execute a privileged instruction");
			case EXCEPTION_SINGLE_STEP:
				return YYCC_U8("one instruction has been executed");
			case EXCEPTION_STACK_OVERFLOW:
				return YYCC_U8("stack overflow");
			default:
				return YYCC_U8("unknown exception");
		}
	}

	/**
	 * @brief Error log (including backtrace) used output function with format feature
	 * @details
	 * This function will format message first.
	 * And write them into given file stream and stderr.
	 * @param[in] fs
	 * The file stream where we write.
	 * If it is nullptr, function will skip writing for file stream.
	 * @param[in] fmt The format string.
	 * @param[in] ... The argument to be formatted.
	*/
	static void UExceptionErrLogFormatLine(std::FILE* fs, const yycc_char8_t* fmt, ...) {
		// write to file
		if (fs != nullptr) {
			va_list arg1;
			va_start(arg1, fmt);
			std::vfprintf(fs, EncodingHelper::ToNative(fmt), arg1);
			std::fputs("\n", fs);
			va_end(arg1);
		}
		// write to stderr
		va_list arg2;
		va_start(arg2, fmt);
		ConsoleHelper::ErrWriteLine(YYCC::StringHelper::VPrintf(fmt, arg2).c_str());
		va_end(arg2);
	}

	/**
	 * @brief Error log (including backtrace) used output function
	 * @details
	 * This function will write given string into given file stream and stderr.
	 * @param[in] fs
	 * The file stream where we write.
	 * If it is nullptr, function will skip writing for file stream.
	 * @param[in] strl The string to be written.
	*/
	static void UExceptionErrLogWriteLine(std::FILE* fs, const yycc_char8_t* strl) {
		// write to file
		if (fs != nullptr) {
			std::fputs(EncodingHelper::ToNative(strl), fs);
			std::fputs("\n", fs);
		}
		// write to stderr
		ConsoleHelper::ErrWriteLine(strl);
	}

	static void UExceptionBacktrace(FILE* fs, LPCONTEXT context, int maxdepth) {
		// setup loading symbol options
		SymSetOptions(SymGetOptions() | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);	// lazy load symbol, and load line number.

		// setup handle
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		// init symbol
		if (!SymInitialize(process, 0, TRUE)) {
			// fail to init. return
			UExceptionErrLogWriteLine(fs, YYCC_U8("Fail to initialize symbol handle for process!"));
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
		while (StackWalk64(machine_type, process, thread, &frame, context,
			0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {

			// depth breaker
			--maxdepth;
			if (maxdepth < 0) {
				UExceptionErrLogWriteLine(fs, YYCC_U8("..."));		// indicate there are some frames not listed
				break;
			}

			// get module name
			const yycc_char8_t* module_name = YYCC_U8("<unknown module>");
			yycc_u8string module_name_raw;
			DWORD64 module_base;
			if (module_base = SymGetModuleBase64(process, frame.AddrPC.Offset)) {
				if (WinFctHelper::GetModuleFileName((HINSTANCE)module_base, module_name_raw)) {
					module_name = module_name_raw.c_str();
				}
			}

			// get source file and line
			const yycc_char8_t* source_file = YYCC_U8("<unknown source>");
			DWORD64 source_file_line = 0;
			DWORD dwDisplacement;
			IMAGEHLP_LINE64 winline;
			winline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &dwDisplacement, &winline)) {
				source_file = EncodingHelper::ToUTF8(winline.FileName); // TODO: check whether there is UNICODE file name.
				source_file_line = winline.LineNumber;
			}

			// write to file
			UExceptionErrLogFormatLine(fs, YYCC_U8("0x%" PRI_XPTR_LEFT_PADDING PRIXPTR "[%s+0x%" PRI_XPTR_LEFT_PADDING PRIXPTR "]\t%s#L%" PRIu64),
				frame.AddrPC.Offset, // memory adress
				module_name, frame.AddrPC.Offset - module_base, // module name + relative address
				source_file, source_file_line // source file + source line
			);

		}

		// ========== END CORE DUMP ==========

		// free symbol
		SymCleanup(process);
	}

	static void UExceptionErrorLog(const yycc_u8string& u8_filename, LPEXCEPTION_POINTERS info) {
		// open file stream if we have file name
		std::FILE* fs = nullptr;
		if (!u8_filename.empty()) {
			fs = IOHelper::UTF8FOpen(u8_filename.c_str(), YYCC_U8("wb"));
		}

		// record exception type first
		PEXCEPTION_RECORD rec = info->ExceptionRecord;
		UExceptionErrLogFormatLine(fs, YYCC_U8("Unhandled exception occured at 0x%" PRI_XPTR_LEFT_PADDING PRIXPTR ": %s (%" PRIu32 ")."),
			rec->ExceptionAddress,
			UExceptionGetCodeName(rec->ExceptionCode),
			rec->ExceptionCode
		);

		// special proc for 2 exceptions
		if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || rec->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
			if (rec->NumberParameters >= 2) {
				const yycc_char8_t* op =
					rec->ExceptionInformation[0] == 0 ? YYCC_U8("read") :
					rec->ExceptionInformation[0] == 1 ? YYCC_U8("written") : YYCC_U8("executed");
				UExceptionErrLogFormatLine(fs, YYCC_U8("The data at memory address 0x%" PRI_XPTR_LEFT_PADDING PRIxPTR " could not be %s."),
					rec->ExceptionInformation[1], op);
			}
		}

		// output stacktrace
		UExceptionBacktrace(fs, info->ContextRecord, 1024);

		// close file if necessary
		if (fs != nullptr) {
			std::fclose(fs);
		}
	}

	static void UExceptionCoreDump(const yycc_u8string& u8_filename, LPEXCEPTION_POINTERS info) {
		// convert file encoding
		std::wstring filename;
		if (u8_filename.empty())
			return; // if no given file name, return
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_filename, filename))
			return; // if convertion failed, return

		// open file and write
		HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			MINIDUMP_EXCEPTION_INFORMATION exception_info;
			exception_info.ThreadId = GetCurrentThreadId();
			exception_info.ExceptionPointers = info;
			exception_info.ClientPointers = TRUE;
			MiniDumpWriteDump(
				GetCurrentProcess(), GetCurrentProcessId(), hFile,
				MiniDumpNormal,
				&exception_info,
				NULL, NULL
			);
			CloseHandle(hFile);
		}
	}

	static bool UExceptionFetchRecordPath(yycc_u8string& log_path, yycc_u8string& coredump_path) {
		// build two file names like: "module.dll.1234.log" and "module.dll.1234.dmp".
		// "module.dll" is the name of current module. "1234" is current process id.
		// get self module name
		yycc_u8string u8_self_module_name;
		{
			// get module handle
			HMODULE hSelfModule = YYCC::WinFctHelper::GetCurrentModule();
			if (hSelfModule == nullptr)
				return false;
			// get full path of self module
			yycc_u8string u8_self_module_path;
			if (!YYCC::WinFctHelper::GetModuleFileName(hSelfModule, u8_self_module_path))
				return false;
			// extract file name from full path by std::filesystem::path
			std::filesystem::path self_module_path(FsPathPatch::FromUTF8Path(u8_self_module_path.c_str()));
			u8_self_module_name = FsPathPatch::ToUTF8Path(self_module_path.filename());
		}
		// then get process id
		DWORD process_id = GetCurrentProcessId();
		// conbine them as a file name prefix
		yycc_u8string u8_filename_prefix;
		if (!YYCC::StringHelper::Printf(u8_filename_prefix, YYCC_U8("%s.%" PRIu32), u8_self_module_name.c_str(), process_id))
			return false;
		// then get file name for log and minidump
		yycc_u8string u8_log_filename = u8_filename_prefix + YYCC_U8(".log");
		yycc_u8string u8_coredump_filename = u8_filename_prefix + YYCC_U8(".dmp");

		// fetch crash report path
		// get local appdata folder
		yycc_u8string u8_localappdata_path;
		if (!WinFctHelper::GetLocalAppData(u8_localappdata_path))
			return false;
		// convert to std::filesystem::path
		std::filesystem::path crash_report_path(FsPathPatch::FromUTF8Path(u8_localappdata_path.c_str()));
		// slash into crash report folder
		crash_report_path /= FsPathPatch::FromUTF8Path(YYCC_U8("CrashDumps"));
		// use create function to make sure it is existing
		std::filesystem::create_directories(crash_report_path);

		// build log path and coredump path
		// build std::filesystem::path first
		std::filesystem::path log_filepath = crash_report_path / FsPathPatch::FromUTF8Path(u8_log_filename.c_str());
		std::filesystem::path coredump_filepath = crash_report_path / FsPathPatch::FromUTF8Path(u8_coredump_filename.c_str());
		// output to result
		log_path = FsPathPatch::ToUTF8Path(log_filepath);
		coredump_path = FsPathPatch::ToUTF8Path(coredump_filepath);

		return true;
	}

	static LONG WINAPI UExceptionImpl(LPEXCEPTION_POINTERS info) {
		// detect loop calling
		if (g_IsProcessing) goto end_proc;
		// start process
		g_IsProcessing = true;

		// core implementation
		{
			// fetch error report path first
			yycc_u8string log_path, coredump_path;
			if (!UExceptionFetchRecordPath(log_path, coredump_path)) {
				// fail to fetch path, clear them.
				// we still can handle crash without them
				log_path.clear();
				coredump_path.clear();
				// and tell user we can not output file
				ConsoleHelper::ErrWriteLine(YYCC_U8("Crash occurs, but we can not create crash log and coredump!"));
			} else {
				// okey. output file path to tell user the path where you can find.
				ConsoleHelper::ErrFormatLine(YYCC_U8("Crash Log: %s"), log_path.c_str());
				ConsoleHelper::ErrFormatLine(YYCC_U8("Crash Coredump: %s"), coredump_path.c_str());
			}

			// write crash log
			UExceptionErrorLog(log_path, info);
			// write crash coredump
			UExceptionCoreDump(coredump_path, info);

		}

		// end process
	failed:
		g_IsProcessing = false;
		// if backup proc can be run, run it
		// otherwise directly return.
	end_proc:
		if (g_ProcBackup != nullptr) {
			return g_ProcBackup(info);
		} else {
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}

#pragma endregion

	void Register() {
		if (g_IsRegistered) return;
		g_ProcBackup = SetUnhandledExceptionFilter(UExceptionImpl);
		g_IsRegistered = true;
	}

	void Unregister() {
		if (!g_IsRegistered) return;
		SetUnhandledExceptionFilter(g_ProcBackup);
		g_IsRegistered = false;
	}

}

#endif
