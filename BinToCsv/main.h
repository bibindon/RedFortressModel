#pragma once

#include <iostream>
#include <fstream>
#include "../Model/MapObjManager.h"
#include "../Model/HeaderOnlyCsv.hpp"
#include <tchar.h>

static void WriteCsv(const std::wstring& binFile)
{
    std::vector<NSModel::stMapObj> stMapObjList;

    std::wifstream inFile(binFile, std::ios::binary);
    if (inFile.is_open() == false)
    {
        throw std::exception();
    }

    size_t size = 0;

    // ベクターサイズを読み込む
    inFile.read(reinterpret_cast<wchar_t*>(&size), sizeof(size));

    // ベクターサイズを設定
    stMapObjList.resize(size);

    // データ本体を読み込む
    inFile.read(reinterpret_cast<wchar_t*>(stMapObjList.data()),
                static_cast<std::streamsize>(size) * sizeof(NSModel::stMapObj));

    inFile.close();

    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;

    vs.push_back(_T("ID"));
    vs.push_back(_T("モデルID"));
    vs.push_back(_T("X"));
    vs.push_back(_T("Y"));
    vs.push_back(_T("Z"));
    vs.push_back(_T("RotY"));
    vs.push_back(_T("Scale"));
    vs.push_back(_T("表示"));
    vvs.push_back(vs);

    for (const auto& item : stMapObjList)
    {
        vs.clear();
        vs.push_back(std::to_wstring(item.m_id));
        vs.push_back(std::to_wstring(item.m_modelId));
        vs.push_back(std::to_wstring((int)item.m_x));
        vs.push_back(std::to_wstring((int)item.m_y));
        vs.push_back(std::to_wstring((int)item.m_z));
        vs.push_back(std::to_wstring(item.m_yRot));
        vs.push_back(std::to_wstring(item.m_scale));

        if (item.m_visible)
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        vvs.push_back(vs);
    }

    std::wstring csvFile = binFile;
    csvFile.replace(csvFile.size() - 4, 4, _T(".csv"));

    csv::Write(csvFile, vvs);
}
