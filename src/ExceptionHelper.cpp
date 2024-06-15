#include "ExceptionHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "WinFctHelper.hpp"
#include "ConsoleHelper.hpp"
#include "StringHelper.hpp"
#include "IOHelper.hpp"
#include "EncodingHelper.hpp"
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
	static const char* UExceptionGetCodeName(DWORD code) {
		switch (code) {
			case EXCEPTION_ACCESS_VIOLATION:
				return "access violation";
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				return "array index out of bound";
			case EXCEPTION_BREAKPOINT:
				return "breakpoint reached";
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				return "misaligned data access";
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				return "operand had denormal value";
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				return "floating-point division by zero";
			case EXCEPTION_FLT_INEXACT_RESULT:
				return "no decimal fraction representation for value";
			case EXCEPTION_FLT_INVALID_OPERATION:
				return "invalid floating-point operation";
			case EXCEPTION_FLT_OVERFLOW:
				return "floating-point overflow";
			case EXCEPTION_FLT_STACK_CHECK:
				return "floating-point stack corruption";
			case EXCEPTION_FLT_UNDERFLOW:
				return "floating-point underflow";
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				return "illegal instruction";
			case EXCEPTION_IN_PAGE_ERROR:
				return "inaccessible page";
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				return "integer division by zero";
			case EXCEPTION_INT_OVERFLOW:
				return "integer overflow";
			case EXCEPTION_INVALID_DISPOSITION:
				return "documentation says this should never happen";
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				return "can't continue after a noncontinuable exception";
			case EXCEPTION_PRIV_INSTRUCTION:
				return "attempted to execute a privileged instruction";
			case EXCEPTION_SINGLE_STEP:
				return "one instruction has been executed";
			case EXCEPTION_STACK_OVERFLOW:
				return "stack overflow";
			default:
				return "unknown exception";
		}
	}

	/**
	 * @brief Backtrace used output function with format feature
	 * @details
	 * This function will format message first.
	 * And write them into given file stream and stderr.
	 * @param[in] fs 
	 * The file stream where we write.
	 * If it is nullptr, function will skip writing for file stream.
	 * @param[in] fmt The format string.
	 * @param[in] ... The argument to be formatted.
	*/
	static void UExceptionBacktraceFormatLine(std::FILE* fs, const char* fmt, ...) {
		// write to file
		if (fs != nullptr) {
			va_list arg1;
			va_start(arg1, fmt);
			std::vfprintf(fs, fmt, arg1);
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
	 * @brief Backtrace used output function
	 * @details
	 * This function will write given string into given file stream and stderr.
	 * @param[in] fs 
	 * The file stream where we write.
	 * If it is nullptr, function will skip writing for file stream.
	 * @param[in] strl The string to be written.
	*/
	static void UExceptionBacktraceWriteLine(std::FILE* fs, const char* strl) {
		// write to file
		if (fs != nullptr) {
			std::fputs(strl, fs);
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
			UExceptionBacktraceWriteLine(fs, "Fail to initialize symbol handle for process!");
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
				UExceptionBacktraceWriteLine(fs, "...\n");		// indicate there are some frames not listed
				break;
			}

			// get module name
			const char* module_name = "<unknown module>";
			std::string module_name_raw;
			DWORD64 module_base;
			if (module_base = SymGetModuleBase64(process, frame.AddrPC.Offset)) {
				if (WinFctHelper::GetModuleName((HINSTANCE)module_base, module_name_raw)) {
					module_name = module_name_raw.c_str();
				}
			}

			// get source file and line
			const char* source_file = "<unknown source>";
			DWORD64 source_file_line = 0;
			DWORD dwDisplacement;
			IMAGEHLP_LINE64 winline;
			winline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &dwDisplacement, &winline)) {
				source_file = winline.FileName;
				source_file_line = winline.LineNumber;
			}

			// write to file
			UExceptionBacktraceFormatLine(fs, "0x%" PRI_XPTR_LEFT_PADDING PRIXPTR "[%s+0x%" PRI_XPTR_LEFT_PADDING PRIXPTR "]\t%s#L%" PRIu64 "\n",
				frame.AddrPC.Offset, // memory adress
				module_name, frame.AddrPC.Offset - module_base, // module name + relative address
				source_file, source_file_line // source file + source line
			);

		}

		// ========== END CORE DUMP ==========

		// free symbol
		SymCleanup(process);
	}

	static void UExceptionErrorLog(const std::string& u8_filename, LPEXCEPTION_POINTERS info) {

	}

	static void UExceptionCoreDump(const std::string& u8_filename, LPEXCEPTION_POINTERS info) {
		// convert file encoding
		std::wstring filename;
		if (u8_filename.empty()) 
			return; // if no given file name, return
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_filename.c_str(), filename))
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

	static void UExceptionFetchRecordPath(std::wstring& log_path, std::wstring& coredump_path) {
		// get self name first
		std::string self_name;

		std::filesystem::path ironpad_path;
		WCHAR module_path[MAX_PATH];
		std::memset(module_path, 0, sizeof(module_path));
		if (GetModuleFileNameW(WinFctHelper::GetCurrentModule(), module_path, MAX_PATH) == 0) {
			//goto failed;
		}
		ironpad_path = module_path;
		ironpad_path = ironpad_path.parent_path();

		// create 2 filename
		auto logfilename = ironpad_path / "IronPad.log";
		auto dmpfilename = ironpad_path / "IronPad.dmp";
		ConsoleHelper::ErrWriteLine("");
		ConsoleHelper::ErrFormatLine("Exception Log: %s\n", logfilename.string().c_str());
		ConsoleHelper::ErrFormatLine("Exception Coredump: %s\n", dmpfilename.string().c_str());

	}

	static LONG WINAPI UExceptionImpl(LPEXCEPTION_POINTERS info) {
		// detect loop calling
		if (g_IsProcessing) goto end_proc;
		// start process
		g_IsProcessing = true;

		{
			// get main folder first
			std::filesystem::path ironpad_path;
			WCHAR module_path[MAX_PATH];
			std::memset(module_path, 0, sizeof(module_path));
			if (GetModuleFileNameW(WinFctHelper::GetCurrentModule(), module_path, MAX_PATH) == 0) {
				goto failed;
			}
			ironpad_path = module_path;
			ironpad_path = ironpad_path.parent_path();

			// create 2 filename
			auto logfilename = ironpad_path / "IronPad.log";
			auto dmpfilename = ironpad_path / "IronPad.dmp";
			std::fputc('\n', stdout);
			std::fprintf(stdout, "Exception Log: %s\n", logfilename.string().c_str());
			std::fprintf(stdout, "Exception Coredump: %s\n", dmpfilename.string().c_str());

			// output log file
			{
				std::FILE* fs = _wfopen(logfilename.wstring().c_str(), L"w");
				if (fs == nullptr) {
					goto failed;
				}

				// record exception type first
				PEXCEPTION_RECORD rec = info->ExceptionRecord;
				fprintf(fs, "Unhandled exception occured at 0x%08p: %s (%lu).\n",
					rec->ExceptionAddress,
					UExceptionGetCodeName(rec->ExceptionCode),
					rec->ExceptionCode
				);

				// special proc for 2 exceptions
				if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || rec->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
					if (rec->NumberParameters >= 2) {
						const char* op =
							rec->ExceptionInformation[0] == 0 ? "read" :
							rec->ExceptionInformation[0] == 1 ? "written" : "executed";
						fprintf(fs, "The data at memory address 0x%016" PRIxPTR " could not be %s.\n",
							rec->ExceptionInformation[1], op);
					}
				}

				// output stacktrace
				UExceptionBacktrace(fs, info->ContextRecord, 1024);

				std::fclose(fs);
			}

			// output minidump
			{
				//UExceptionCoreDump(dmpfilename.wstring().c_str(), info);
			}

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
