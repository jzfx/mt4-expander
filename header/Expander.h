#pragma once

#include "common.h"
#include "stdafx.h"
#include "shared/defines.h"                                                      // shared between DLL and MQL
#include "shared/errors.h"                                                       // ...
#include <vector>


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                          // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)  // export unmangled names without DEF file
#else
   #define EXPANDER_API extern "C" __declspec(dllimport)
#endif


#define MAX_CHART_DESCRIPTION_LENGTH   ((MAX_SYMBOL_LENGTH) + 1 + 7)             // "SYMBOL,Monthly"


#pragma pack(push, 1)


// MQL-Programmtypen
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL-Modultypen                            // als Flag implementiert
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,      // 1
   MT_EXPERT    = MODULETYPE_EXPERT,         // 2
   MT_SCRIPT    = MODULETYPE_SCRIPT,         // 4
   MT_LIBRARY   = MODULETYPE_LIBRARY         // 8
};


// Launchtypen eines MQL-Programms: via Template, via iCustom() oder von Hand
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,
   LT_MANUAL    = LAUNCHTYPE_MANUAL
};


// MQL-Rootfunktionen
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// InitializeReasons
enum InitializeReason {
   IR_USER              = INITREASON_USER,
   IR_TEMPLATE          = INITREASON_TEMPLATE,
   IR_PROGRAM           = INITREASON_PROGRAM,
   IR_PROGRAM_AFTERTEST = INITREASON_PROGRAM_AFTERTEST,
   IR_PARAMETERS        = INITREASON_PARAMETERS,
   IR_TIMEFRAMECHANGE   = INITREASON_TIMEFRAMECHANGE,
   IR_SYMBOLCHANGE      = INITREASON_SYMBOLCHANGE,
   IR_RECOMPILE         = INITREASON_RECOMPILE
};


// MQL-UninitializeReasons
enum UninitializeReason {
   UR_UNDEFINED         = UNINITREASON_UNDEFINED,
   UR_REMOVE            = UNINITREASON_REMOVE,
   UR_RECOMPILE         = UNINITREASON_RECOMPILE,
   UR_CHARTCHANGE       = UNINITREASON_CHARTCHANGE,
   UR_CHARTCLOSE        = UNINITREASON_CHARTCLOSE,
   UR_PARAMETERS        = UNINITREASON_PARAMETERS,
   UR_ACCOUNT           = UNINITREASON_ACCOUNT,
   UR_TEMPLATE          = UNINITREASON_TEMPLATE,       // since terminal build > 509
   UR_INITFAILED        = UNINITREASON_INITFAILED,     // ...
   UR_CLOSE             = UNINITREASON_CLOSE           // ...
};


#include "mql/structs/mt4/FxtHeader.h"
#include "mql/structs/mt4/HistoryBar400.h"
#include "mql/structs/mt4/HistoryBar401.h"
#include "mql/structs/mt4/HistoryHeader.h"
#include "mql/structs/mt4/MqlStr.h"
#include "mql/structs/mt4/Symbol.h"
#include "mql/structs/mt4/SymbolGroup.h"
#include "mql/structs/mt4/SymbolSelected.h"
#include "mql/structs/mt4/Tick.h"

//#include "mql/structs/myfx/LogMessage.h"
#include "mql/structs/myfx/ExecutionContext.h"

#include "mql/structs/win32/FileTime.h"
#include "mql/structs/win32/ProcessInformation.h"
#include "mql/structs/win32/SecurityAttributes.h"
#include "mql/structs/win32/StartupInfo.h"
#include "mql/structs/win32/SystemTime.h"
#include "mql/structs/win32/TimeZoneInformation.h"
#include "mql/structs/win32/Win32FindData.h"


// Deklaration Thread- und EXECUTION_CONTEXT-Verwaltung (Initialisierung in Expander.cpp)
extern std::vector<pec_vector> contextChains;                        // alle Context-Chains (Index = ProgramID)
extern std::vector<DWORD>      threads;                              // ID's aller bekannten Threads
extern std::vector<uint>       threadsPrograms;                      // ID's des vom Thread zuletzt ausgef�hrten MQL-Programms
extern uint                    lastUIThreadProgram;                  // ID des vom UI-Thread zuletzt ausgef�hrten MQL-Programm
extern CRITICAL_SECTION        terminalLock;                         // Terminal-weites Lock


// Funktionsdeklarationen
#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define  warn(...)    _warn(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define error(...)   _error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)


int   _debug(const char* fileName, const char* funcName, int line, const char*        format, ...);
int   _debug(const char* fileName, const char* funcName, int line, const std::string &format, ...);
void __debug(const char* fileName, const char* funcName, int line, const char*        format, const va_list &args);

int   _warn (const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, ...);
int   _warn (const char* fileName, const char* funcName, int line, int code, const std::string &msgFormat, ...);
void __warn (const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, const va_list &args);

int   _error(const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, ...);
int   _error(const char* fileName, const char* funcName, int line, int code, const std::string &msgFormat, ...);
void __error(const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, const va_list &args);


BOOL              WINAPI onProcessAttach();
BOOL              WINAPI onProcessDetach();

const char*       WINAPI BoolToStr                (BOOL value);
const char*       WINAPI DeinitFlagsToStr         (uint flags);
const char*       WINAPI DoubleQuoteStr           (const char* value);
const char*       WINAPI ErrorToStr               (int error);
HWND              WINAPI FindCurrentChart         (HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, BOOL isTesting, BOOL isVisualMode, const char* symbol, uint timeframe);
int               WINAPI FindFirstIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason);
HWND              WINAPI GetApplicationWindow();
uint              WINAPI GetBoolsAddress          (const BOOL values[]);
uint              WINAPI GetChartDescription      (const char* symbol, uint timeframe, char* buffer, uint bufferSize);
uint              WINAPI GetDoublesAddress        (const double values[]);
uint              WINAPI GetGmtTime();
uint              WINAPI GetIntsAddress           (const int values[]);
int               WINAPI GetLastWin32Error();
uint              WINAPI GetLocalTime();
const char*       WINAPI GetString                (const char* value);
uint              WINAPI GetStringAddress         (const char* value);
uint              WINAPI GetStringsAddress        (const MqlStr values[]);
uint              WINAPI GetTerminalBuild();
const char*       WINAPI GetTerminalVersion();
BOOL              WINAPI GetTerminalVersions      (uint* major, uint* minor, uint* hotfix, uint* build);
DWORD             WINAPI GetUIThreadId();
HANDLE            WINAPI GetWindowProperty        (HWND hWnd, const char* lpName);
const char*       WINAPI InitFlagsToStr           (uint flags);
InitializeReason  WINAPI InitReason               (const char* programName, ProgramType programType, UninitializeReason uninitializeReason, const char* symbol, EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec, BOOL testing, BOOL visualMode, HWND hChart, int subChartDropped);
const char*       WINAPI InitReasonToStr          (InitializeReason reason);
const char*       WINAPI InitializeReasonToStr    (InitializeReason reason);          // Alias
const char*       WINAPI IntToHexStr              (int value);
const char*       WINAPI IntToHexStr              (const void* value);
BOOL              WINAPI IsCustomTimeframe        (int timeframe);
BOOL              WINAPI IsStdTimeframe           (int timeframe);
BOOL              WINAPI IsUIThread();
const char*       WINAPI ModuleTypeDescription    (ModuleType type);
const char*       WINAPI ModuleTypeToStr          (ModuleType type);
uint              WINAPI MT4InternalMsg();
const char*       WINAPI PeriodDescription        (uint period);
const char*       WINAPI PeriodToStr              (uint period);
BOOL              WINAPI ProgramIsLogging         (const EXECUTION_CONTEXT* ec);
BOOL              WINAPI ProgramIsOptimization    (const EXECUTION_CONTEXT* ec, BOOL isOptimization);
BOOL              WINAPI ProgramIsTesting         (const EXECUTION_CONTEXT* ec, BOOL isTesting);
BOOL              WINAPI ProgramIsVisualMode      (const EXECUTION_CONTEXT* ec, BOOL isVisualMode);
const char*       WINAPI ProgramCustomLogFile     (const EXECUTION_CONTEXT* ec);
const char*       WINAPI ProgramTypeDescription   (ProgramType type);
const char*       WINAPI ProgramTypeToStr         (ProgramType type);
BOOL              WINAPI RemoveTickTimer          (int timerId);
void              WINAPI RemoveTickTimers();
HANDLE            WINAPI RemoveWindowProperty     (HWND hWnd, const char* lpName);
const char*       WINAPI RootFunctionDescription  (RootFunction id);
const char*       WINAPI RootFunctionToStr        (RootFunction id);
uint              WINAPI SetupTickTimer           (HWND hWnd, int millis, DWORD flags=NULL);
BOOL              WINAPI SetWindowProperty        (HWND hWnd, const char* lpName, HANDLE value);
BOOL              WINAPI ShiftIndicatorBuffer     (double buffer[], int bufferSize, int bars, double emptyValue);
const char*       WINAPI ShowWindowCmdToStr       (int cmdShow);
BOOL              WINAPI StringCompare            (const char* s1, const char* s2);
BOOL              WINAPI StringEndsWith           (const char* str, const char* suffix);
BOOL              WINAPI StringIsNull             (const char* value);
const char*       WINAPI StringToStr              (const char* value);
const char*       WINAPI TimeframeDescription     (uint timeframe);                   // Alias
const char*       WINAPI TimeframeToStr           (uint timeframe);                   // Alias
VOID            CALLBACK TimerCallback            (HWND hWnd, UINT msg, UINT_PTR timerId, DWORD time);
const char*       WINAPI UninitReasonToStr        (UninitializeReason reason);
const char*       WINAPI UninitializeReasonToStr  (UninitializeReason reason);        // Alias
char*             WINAPI WCharsToStr              (const WCHAR* wcstr, size_t count);


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zur�ckzugeben.
 * Alle Parameter werden ignoriert.
 */
int  WINAPI _CLR_NONE    (...);
int  WINAPI _EMPTY       (...);
HWND WINAPI _INVALID_HWND(...);
int  WINAPI _NULL        (...);
bool WINAPI _true        (...);
bool WINAPI _false       (...);
BOOL WINAPI _TRUE        (...);
BOOL WINAPI _FALSE       (...);


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zur�ckzugeben.
 * Au�er dem ersten werden alle �bergebenen Parameter ignoriert.
 */
bool   WINAPI _bool  (bool   value, ...);
char   WINAPI _char  (char   value, ...);
int    WINAPI _int   (int    value, ...);
float  WINAPI _float (float  value, ...);
double WINAPI _double(double value, ...);
BOOL   WINAPI _BOOL  (BOOL   value, ...);

