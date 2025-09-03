#include "main.h"

// ファイル名に"enemy"をいう文字があったら敵情報として扱う。
// ファイル名に"enemy"をいう文字がなかったらマップオブジェクト情報として扱う。
int wmain(int argc, wchar_t* argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::wstring fileName = argv[1];

    if (fileName.find(L"enemy") != std::wstring::npos)
    {
        // 敵情報のCSVファイルをバイナリに変換
        WriteBinaryFromEnemy(fileName);
    }
    else
    {
        // マップオブジェクトのCSVファイルをバイナリに変換
        WriteBinaryFromMapObj(fileName);
    }

    return 0;
}

void WriteBinaryFromMapObj(const std::wstring& csvFile)
{
    auto vvs = csv::Read(csvFile);

    std::vector<NSStarmanLib::stMapObj> stMapObjList;
    for (std::size_t i = 1; i < vvs.size(); ++i)
    {
        NSStarmanLib::stMapObj work;
        float work_f = 0.f;

        work.m_id = std::stoi(vvs.at(i).at(0));

        work.m_modelId = std::stoi(vvs.at(i).at(1));

        work_f = std::stof(vvs.at(i).at(2));
        work.m_x = work_f;
        work.m_frameX = (int)work_f/100;

        work.m_y = std::stof(vvs.at(i).at(3));

        work_f = std::stof(vvs.at(i).at(4));
        work.m_z = work_f;
        work.m_frameZ = (int)work_f/100;

        work.m_yRot = std::stof(vvs.at(i).at(5));

        work.m_scale = std::stof(vvs.at(i).at(6));

        if (vvs.at(i).at(7) == _T("y"))
        {
            work.m_visible = true;
        }
        else
        {
            work.m_visible = false;
        }
        stMapObjList.emplace_back(work);
    }

    std::wstring binFile = csvFile;
    binFile.replace(binFile.size() - 4, 4, _T(".bin"));

    std::ofstream outFile(binFile, std::ios::binary);
    if (outFile.is_open())
    {
        size_t size = stMapObjList.size();

        // ベクターサイズを書き込む
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // データ本体を書き込む
        outFile.write(reinterpret_cast<const char*>(stMapObjList.data()),
                      static_cast<std::streamsize>(size) * sizeof(NSStarmanLib::stMapObj));

        outFile.close();
    }
    else
    {
        throw std::exception();
    }
}

void WriteBinaryFromEnemy(const std::wstring& csvFile)
{
    auto vvs = csv::Read(csvFile);

    std::vector<NSStarmanLib::stEnemyInfo> stEnemyList;
    for (std::size_t i = 1; i < vvs.size(); ++i)
    {
        NSStarmanLib::stEnemyInfo work;
        float work_f = 0.f;

        work.m_SerialNumber = std::stoi(vvs.at(i).at(0));
        wcsncpy_s(work.m_id, vvs.at(i).at(1).c_str(), _TRUNCATE);
        work.m_x = std::stof(vvs.at(i).at(2));
        work.m_y = std::stof(vvs.at(i).at(3));
        work.m_z = std::stof(vvs.at(i).at(4));

        work.m_rotX = std::stof(vvs.at(i).at(5));
        work.m_rotY = std::stof(vvs.at(i).at(6));
        work.m_rotZ = std::stof(vvs.at(i).at(7));

        work.m_HP = std::stoi(vvs.at(i).at(8));
        if (vvs.at(i).at(9) == L"y")
        {
            work.m_bDefeated = true;
        }
        else
        {
            work.m_bDefeated = false;
        }

        stEnemyList.emplace_back(work);
    }

    std::wstring binFile = csvFile;
    binFile.replace(binFile.size() - 4, 4, _T(".bin"));

    std::ofstream outFile(binFile, std::ios::binary);
    if (outFile.is_open())
    {
        size_t size = stEnemyList.size();

        // ベクターサイズを書き込む
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // データ本体を書き込む
        outFile.write(reinterpret_cast<const char*>(stEnemyList.data()),
                      static_cast<std::streamsize>(size) * sizeof(NSStarmanLib::stEnemyInfo));

        outFile.close();
    }
    else
    {
        throw std::exception();
    }
}

