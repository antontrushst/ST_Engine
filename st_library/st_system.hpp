#ifndef ST_SYSTEM_HPP
#define ST_SYSTEM_HPP

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <shlobj.h>
#include <objbase.h>
#include <optional>
#include <initializer_list>

namespace st
{
    // MESSAGES ////////////////////////////////////////////////////////////////
    inline void msg_ok(std::string_view text)
    {
        #if defined(_WIN32) || defined(_WIN64)
        
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO cbinfo;
            GetConsoleScreenBufferInfo(hConsole, &cbinfo);
            int originalColor = cbinfo.wAttributes;
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[32m" << text << "\033[0m" << std::endl;
    }

    inline void msg_err(std::string_view text,
        const std::source_location location = std::source_location::current())
    {
        #if defined(_WIN32) || defined(_WIN64)
        
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO cbinfo;
            GetConsoleScreenBufferInfo(hConsole, &cbinfo);
            int originalColor = cbinfo.wAttributes;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << "ERROR in file " << location.file_name() <<
            " in function " << location.function_name() <<
            " on line " << location.line() << "!\n";
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[31m" << "ERROR in file " << location.file_name() <<
            " in function " << location.function_name() <<
            " on line " << location.line() << "!\n" <<
            text << "\033[0m" << std::endl;
    }

    inline void msg_warn(std::string_view text,
        const std::source_location location = std::source_location::current())
    {
        #if defined(_WIN32) || defined(_WIN64)
        
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO cbinfo;
            GetConsoleScreenBufferInfo(hConsole, &cbinfo);
            int originalColor = cbinfo.wAttributes;
            SetConsoleTextAttribute(hConsole, 6);
            std::cout << "Warning in file " << location.file_name() <<
            " in function " << location.function_name() <<
            " on line " << location.line() << "!\n";
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[33m" << "Warning in file " << location.file_name() <<
            " in function " << location.function_name() <<
            " on line " << location.line() << "!\n" <<
            text << "\033[0m" << std::endl;
    }
    // CONVERTER STD::STRING TO STD::WSTRING ///////////////////////////////////
    inline std::wstring convertToWString(const std::string &utf8_str)
    {
        if(utf8_str.empty())
            return L"";

        int bufferSize{MultiByteToWideChar(CP_UTF8, 0, &utf8_str[0],
            (int)utf8_str.size(), NULL, 0)};
        std::wstring result(bufferSize, 0);
        MultiByteToWideChar(CP_UTF8, 0, &utf8_str[0], (int)utf8_str.size(),
            &result[0], bufferSize);
        return result;
    }
    // GET CURRENT PROGRAM LOCATION ////////////////////////////////////////////
    inline std::filesystem::path getThisProgramLocation()
    {
        #if defined(_WIN32) || defined(_WIN64)
            char C_path[MAX_PATH];
            GetModuleFileNameA(NULL, C_path, MAX_PATH);
            std::string path = C_path;
            size_t preNameChar = path.rfind("\\");
            path.erase(preNameChar + 1);
            return std::filesystem::path{path};
        #endif
    }
    // OPEN FOLDER /////////////////////////////////////////////////////////////
    inline std::optional<std::filesystem::path> getFolder(
        const std::string &title = "")
    {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED
            | COINIT_DISABLE_OLE1DDE);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Handle not initialized!");
            return std::nullopt;
        }

        IFileOpenDialog *pFileOpen;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFileOpen));
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Handle instance not created!");
            return std::nullopt;
        }

        DWORD dwFlags;
        hr = pFileOpen->GetOptions(&dwFlags);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Couldn't get handle options!");
            return std::nullopt;
        }

        hr = pFileOpen->SetOptions(dwFlags | FOS_FORCEFILESYSTEM |
            FOS_PICKFOLDERS);

        // set custom window title if provided
        if(!title.empty())
        {
            pFileOpen->SetTitle(convertToWString(title).c_str());
        }
        // ...................................

        hr = pFileOpen->Show(NULL);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder was canceled!");
            return std::nullopt;
        }

        IShellItem *pItem;
        hr = pFileOpen->GetResult(&pItem);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Couldn't get results!");
            return std::nullopt;
        }

        PWSTR pszFilePath;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Couldn't get display name!");
            return std::nullopt;
        }

        int bufferSize = WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, NULL,
            0, NULL, NULL);
        std::vector<char> ansiBuffer(bufferSize);
        WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, ansiBuffer.data(),
            bufferSize, NULL, NULL);

        CoTaskMemFree(pszFilePath);
        pItem->Release();
        pFileOpen->Release();
        CoUninitialize();

        return std::filesystem::path{ansiBuffer.data()};
    }

    // GET FILE /////////////////////////////////////////////////////////////
    inline std::optional<std::filesystem::path> getFile(
        const std::string &title = "",
        std::initializer_list<std::string> extensions = {},
        std::initializer_list<std::string> ext_names = {})
    {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED
            | COINIT_DISABLE_OLE1DDE);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Handle not initialized!");
            return std::nullopt;
        }

        IFileOpenDialog *pFileOpen;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFileOpen));
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Handle instance not created!");
            return std::nullopt;
        }

        if(extensions.size() == 0)
        {
            msg_err("No extensions supplied!");
            exit(-1);
        }
        
        if(extensions.size() != ext_names.size())
        {
            msg_err("Extensions mismatch extension names!");
            exit(-1);
        }
        
        COMDLG_FILTERSPEC fileTypes[extensions.size()];
        for(int i{0}; i < extensions.size(); ++i)
        {
            fileTypes[i].pszName =
                convertToWString(ext_names.begin()[i]).c_str();
            fileTypes[i].pszSpec =
                convertToWString(extensions.begin()[i]).c_str();
        }

        pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

        DWORD dwFlags;
        hr = pFileOpen->GetOptions(&dwFlags);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Couldn't get handle options!");
            return std::nullopt;
        }

        hr = pFileOpen->SetOptions(dwFlags | FOS_FORCEFILESYSTEM |
            FOS_STRICTFILETYPES);

        // set custom window title if provided
        if(!title.empty())
        {
            pFileOpen->SetTitle(convertToWString(title).c_str());
        }
        // ...................................

        hr = pFileOpen->Show(NULL);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder was canceled!");
            return std::nullopt;
        }

        IShellItem *pItem;
        hr = pFileOpen->GetResult(&pItem);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Couldn't get results!");
            return std::nullopt;
        }

        PWSTR pszFilePath;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        if(!SUCCEEDED(hr))
        {
            msg_warn("Warning: GetFolder Failed! Couldn't get display name!");
            return std::nullopt;
        }

        int bufferSize = WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, NULL,
            0, NULL, NULL);
        std::vector<char> ansiBuffer(bufferSize);
        WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, ansiBuffer.data(),
            bufferSize, NULL, NULL);

        CoTaskMemFree(pszFilePath);
        pItem->Release();
        pFileOpen->Release();
        CoUninitialize();

        return std::filesystem::path{ansiBuffer.data()};
    }
}

#endif
