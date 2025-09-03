#pragma once

#include <string>
#include <vector>
#include <tchar.h>

class Util
{
public:
    static std::vector<std::vector<std::wstring>>
        ReadFromCsv(const std::wstring& filename, const bool decrypt);

    static void WriteToCsv(const std::wstring& filename,
                           const std::vector<std::vector<std::wstring>>& vvs,
                           const bool encrypt);

    static std::vector<std::wstring> split(const std::wstring& s, wchar_t delim = ':');

    static bool HitByBoundingBox(const float x1, const float y1, const float z1,
                                 const float x2, const float y2, const float z2,
                                 const float length);

    static bool HitByBoundingBoxWithoutY(const float x1, const float z1,
                                         const float x2, const float z2,
                                         const float length);

    template <typename T>
    static void EraseRemove(std::vector<T>& vec, const T& value)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
    }

    template <typename T, typename Predicate>
    static void EraseRemoveIf(std::vector<T>& vec, Predicate pred)
    {
        vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
    }
};

