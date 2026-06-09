#ifndef ST_SYSTEM_HPP
#define ST_SYSTEM_HPP

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <shlobj.h>
#include <objbase.h>
#include <optional>

namespace st
{
    // MESSAGES ////////////////////////////////////////////////////////////////
    inline void msg_ok(const std::string text)
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

    inline void msg_err(const std::string text)
    {
        #if defined(_WIN32) || defined(_WIN64)
        
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO cbinfo;
            GetConsoleScreenBufferInfo(hConsole, &cbinfo);
            int originalColor = cbinfo.wAttributes;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << "ERROR in file " << __FILE__ <<
            " in function " << __func__ <<
            " on line " << std::to_string(__LINE__) << "!\n";
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[31m" << "ERROR in file " << __FILE__ <<
            " in function " << __func__ <<
            " on line " << std::to_string(__LINE__) << "!\n" <<
            text << "\033[0m" << std::endl;
    }

    inline void msg_warn(const std::string text)
    {
        #if defined(_WIN32) || defined(_WIN64)
        
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO cbinfo;
            GetConsoleScreenBufferInfo(hConsole, &cbinfo);
            int originalColor = cbinfo.wAttributes;
            SetConsoleTextAttribute(hConsole, 6);
            std::cout << "Warning in file " << __FILE__ <<
            " in function " << __func__ <<
            " on line " << std::to_string(__LINE__) << "!\n";
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[33m" << "Warning in file " << __FILE__ <<
            " in function " << __func__ <<
            " on line " << std::to_string(__LINE__) << "!\n" <<
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
    inline std::string getThisProgramLocation()
    {
        #if defined(_WIN32) || defined(_WIN64)
            char C_path[MAX_PATH];
            GetModuleFileNameA(NULL, C_path, MAX_PATH);
            std::string path = C_path;
            size_t preNameChar = path.rfind("\\");
            path.erase(preNameChar + 1);
            return path;
        #endif
    }
    // OPEN FOLDER /////////////////////////////////////////////////////////////
    inline std::optional<std::string> getFolder(const std::string &title = "")
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
            msg_warn("Warning: GetFolder Failed! Couldn't show dialog!");
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

        return std::string(ansiBuffer.data());
    }
}

#endif
