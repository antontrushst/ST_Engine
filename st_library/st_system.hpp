#ifndef ST_SYSTEM_HPP
#define ST_SYSTEM_HPP

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <shlobj.h>
#include <objbase.h>

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
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[31m" << text << "\033[0m" << std::endl;
    }

    inline void msg_warn(const std::string text)
    {
        #if defined(_WIN32) || defined(_WIN64)
        
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO cbinfo;
            GetConsoleScreenBufferInfo(hConsole, &cbinfo);
            int originalColor = cbinfo.wAttributes;
            SetConsoleTextAttribute(hConsole, 6);
            std::cout << text << std::endl;
            SetConsoleTextAttribute(hConsole, originalColor);
            return;

        #endif

        std::cout << "\033[33m" << text << "\033[0m" << std::endl;
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
    inline std::string getFolder()
    {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED
            | COINIT_DISABLE_OLE1DDE);
        if(!SUCCEEDED(hr))
        {
            msg_err("ERROR: GetFolder Failed! Handle not initialized!");
            exit(-1);
        }

        IFileOpenDialog *pFileOpen;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFileOpen));
        if(!SUCCEEDED(hr))
        {
            msg_err("ERROR: GetFolder Failed! Handle instance not created!");
            exit(-1);
        }

        DWORD dwFlags;
        hr = pFileOpen->GetOptions(&dwFlags);
        if(!SUCCEEDED(hr))
        {
            msg_err("ERROR: GetFolder Failed! Couldn't get handle options!");
            exit(-1);
        }

        hr = pFileOpen->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_PICKFOLDERS);
        hr = pFileOpen->Show(NULL);
        if(!SUCCEEDED(hr))
        {
            msg_err("ERROR: GetFolder Failed! Couldn't show dialog!");
            exit(-1);
        }

        IShellItem *pItem;
        hr = pFileOpen->GetResult(&pItem);
        if(!SUCCEEDED(hr))
        {
            msg_err("ERROR: GetFolder Failed! Couldn't get results!");
            exit(-1);
        }

        PWSTR pszFilePath;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        if(!SUCCEEDED(hr))
        {
            msg_err("ERROR: GetFolder Failed! Couldn't get display name!");
            exit(-1);
        }

        int bufferSize = WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, NULL, 0,
            NULL, NULL);
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
