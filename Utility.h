#ifndef _UTILITIES_
#define _UTILITIES_

#include <codecvt>
#include <random>
#include <chrono>
#include "Common.h"

#ifdef _WIN32
#include "windows.h"
#endif

namespace SNY
{
static wstring_convert<codecvt_utf8<wchar_t>, wchar_t> s_StrWStrConverter;

inline REAL RandomReal()
{
    //static random_device device;
    //static mt19937 generator(device());
    //static uniform_real_distribution<REAL> distribution(0, 1.0);

    //return distribution(generator);

    return rand() / (RAND_MAX + 1.0);
}

inline string GetCurrentTimeStr()
{
    using chrono::system_clock;
    time_t tt = system_clock::to_time_t(system_clock::now());
    return ctime(&tt);
}

inline bool ReadFileToMemory(const wstring &filePath, vector<char> &buf)
{
#ifdef _WIN32
    ifstream ifs(filePath, ios_base::in | ios_base::ate | ios_base::binary); // Windows has extension for ifstream(wstring, ...)
#else
    ifstream ifs(s_StrWStrConverter.to_bytes(filePath), std::ios_base::in | std::ios_base::ate | std::ios_base::binary);
#endif
    ifs.unsetf(ios::skipws); // Do not skip white space

    if (ifs.is_open())
    {
        long fileSize = ifs.tellg();
        ifs.seekg(0, ios_base::beg);
        ifs.clear();
        char *temp = new char[fileSize];
        ifs.read(temp, fileSize);
        buf.reserve(fileSize);
        buf.assign(temp, temp + fileSize);
        delete[] temp;
        ifs.close();
        return true;
    }
    else
    {
        return false;
    }
}

inline bool ReadFileToMemory(const wstring &fileName, vector<wchar_t> &buf)
{
#ifdef _WIN32
    wifstream wifs(fileName, ios_base::in | ios_base::ate | ios_base::binary); // Windows has extension for ifstream(wstring, ...)
#else
    wifstream wifs(s_StrWStrConverter.to_bytes(fileName), std::ios_base::in | std::ios_base::ate | std::ios_base::binary);
#endif
    wifs.unsetf(ios::skipws); // Do not skip white space

    if (wifs.is_open())
    {
        long fileSize = wifs.tellg();
        wifs.seekg(0, ios_base::beg);
        wifs.clear();
        wchar_t *temp = new wchar_t[fileSize];
        wifs.read(temp, fileSize);
        buf.reserve(fileSize);
        buf.assign(temp, temp + fileSize);
        delete[] temp;
        wifs.close();
        return true;
    }
    else
    {
        return false;
    }
}

inline string Exec(const string &cmd, bool showStderr = false)
{
    string command = cmd + (showStderr ? " 2>&1" : ""); // Option to redirect stderr to stdout
    string rs = "";
#ifdef _WIN32
    shared_ptr<FILE> pipe(_popen(command.c_str(), "r"), _pclose);
#else
    shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
#endif
    if (pipe)
    {
        char buf[1024];
        while (!feof(pipe.get()))
        {
            if (fgets(buf, 1024, pipe.get()) != nullptr)
            {
                rs += buf;
            }
        }
    }
    return rs;
}

inline wstring GetCurrentDirectory()
{
#ifdef _WIN32
    wchar_t szDirectory[MAX_PATH];
    memset(szDirectory, 0, MAX_PATH);
    ::GetModuleFileNameW(NULL, szDirectory, sizeof(szDirectory) / 2);
    wstring path(szDirectory);
    wstring cd = path.substr(0, path.rfind(L"\\") + 1);
#else
    wstring cd = s_StrWStrConverter.from_bytes(Exec("pwd"));
    cd[cd.size() - 1] = L'/'; // remove '/n' and add '/'
#endif
    return cd;
}

static map<string, chrono::system_clock::time_point> namedTimers;

inline void StartTiming(const string &timerName)
{
    cout << timerName << " starts" << endl;
    auto it = namedTimers.find(timerName);
    if (it != namedTimers.end())
    {
        it->second = chrono::system_clock::now();
    }
    else
    {
        namedTimers.insert(pair<string, chrono::system_clock::time_point>(timerName, chrono::system_clock::now()));
    }
}

inline void EndTiming(const string &timerName)
{
    auto it = namedTimers.find(timerName);
    if (it != namedTimers.end())
    {
        auto endTime = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(endTime - namedTimers[timerName]).count();
        cout << timerName << " operation takes " << dt << "us" << endl;
    }
}

inline vector<string> StringSplit(const string &str, const char delim)
{
    vector<string> result;
    istringstream iss(str);
    string s;
    while (getline(iss, s, delim))
    {
        result.push_back(s);
    }
    return result;
}

inline void StringFilter(string &str, const string &filter)
{
    for (size_t i = 0; i < filter.size(); ++i)
    {
        size_t j = 0;
        while (j != string::npos)
        {
            if ((j = str.find(filter[i])) != string::npos)
            {
                str.erase(j, 1);
                ++j;
            }
        }
    }
}

inline void StringTrimBack(string &str, const string &filter)
{
    for (size_t i = 0; i < filter.size(); ++i)
    {
        while (str[str.size() - 1] == filter[i])
        {
            str.erase(str.size() - 1, 1);
        }
    }
}

inline string GetFileName(const string &str)
{
    size_t pos = str.find_last_of(".");
    if (pos != string::npos)
    {
        return str.substr(0, pos);
    }
    return str;
}
} // namespace SNY

#endif
