#include "DynamicLibrary.h"
#if PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER) 
#define NOMINMAX
#endif
#include <windows.h>
#endif


std::wstring stringToWString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstrTo[0], size_needed);
    return wstrTo;
}

DynamicLibrary::DynamicLibrary(const std::string& name) 
: instance(nullptr) 
{ 
    #if PLATFORM == PLATFORM_WIN32
    std::wstring wname = stringToWString(name);
    instance = (void*)LoadLibraryW(wname.c_str()); // Заміна на LoadLibraryW
    #endif
}

DynamicLibrary::~DynamicLibrary()
{ 
    #if PLATFORM == PLATFORM_WIN32
    FreeLibrary((HMODULE)instance);
    #endif
} 

void* DynamicLibrary::getSymbol(const std::string& symbolName) const
{ 
    #if PLATFORM == PLATFORM_WIN32
    return (void*)GetProcAddress((HMODULE)instance, symbolName.c_str());
    #endif
    return nullptr; 
}
