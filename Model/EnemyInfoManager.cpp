#include "EnemyInfoManager.h"

#include <algorithm>
#include <fstream>

#include "Util.h"

using namespace NSStarmanLib;

EnemyInfoManager* EnemyInfoManager::obj { nullptr };

EnemyInfoManager* EnemyInfoManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new EnemyInfoManager();
    }
    return obj;
}

void EnemyInfoManager::Destroy()
{
    delete EnemyInfoManager::obj;
    EnemyInfoManager::obj = nullptr;
}

void EnemyInfoManager::Init(const std::wstring& csvEnemyDef,
                            const std::wstring& csvEnemyInfo,
                            const std::wstring& csvEnemyVisible,
                            const bool decrypt,
                            const bool loadFromBinary)
{
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvEnemyDef, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            std::wstring id = vvs.at(i).at(0);
            float work_f = 0.f;

            EnemyDef enemyDef;

            enemyDef.SetID(id);

            enemyDef.SetName(vvs.at(i).at(1));

            enemyDef.SetDetail(vvs.at(i).at(2));

            enemyDef.SetImagePath(vvs.at(i).at(3));

            m_enemyDefMap[id] = enemyDef;
        }
    }

    // バイナリファイルから読み込む
    if (loadFromBinary)
    {
        std::vector<stEnemyInfo> stEnemyInfoList;

        std::wstring binFile = csvEnemyInfo;
        binFile.replace(binFile.size() - 4, 4, _T(".bin"));
        std::ifstream inFile(binFile, std::ios::binary);
        if (!inFile.is_open())
        {
            throw std::exception();
        }
        
        size_t size = 0;

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        stEnemyInfoList.resize(size);
        inFile.read(reinterpret_cast<char*>(stEnemyInfoList.data()),
                    static_cast<std::streamsize>(size) * sizeof(stEnemyInfo));
        inFile.close();

        for (const auto& enemyInfo : stEnemyInfoList)
        {
            if (m_enemyDefMap.find(enemyInfo.m_id) == m_enemyDefMap.end())
            {
                throw std::exception();
            }

            m_enemyInfoMap[enemyInfo.m_SerialNumber] = stEnemyInfo(enemyInfo);
        }
    }
    // テキストファイルから読み込む
    else
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvEnemyInfo, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            int serialNumber = 0;
            float work_f = 0.f;

            stEnemyInfo enemyInfo;

            serialNumber = std::stoi(vvs.at(i).at(0));
            enemyInfo.m_SerialNumber = serialNumber;

            std::wstring id = vvs.at(i).at(1);
            wcsncpy_s(enemyInfo.m_id, id.c_str(), _TRUNCATE);

            work_f = std::stof(vvs.at(i).at(2));
            enemyInfo.m_x = work_f;

            work_f = std::stof(vvs.at(i).at(3));
            enemyInfo.m_y = work_f;

            work_f = std::stof(vvs.at(i).at(4));
            enemyInfo.m_z = work_f;

            work_f = std::stof(vvs.at(i).at(5));
            enemyInfo.m_rotX = work_f;

            work_f = std::stof(vvs.at(i).at(6));
            enemyInfo.m_rotY = work_f;

            work_f = std::stof(vvs.at(i).at(7));
            enemyInfo.m_rotZ = work_f;

            int hp = std::stoi(vvs.at(i).at(8));
            enemyInfo.m_HP = hp;

            if (vvs.at(i).at(9) == _T("y"))
            {
                enemyInfo.m_bDefeated = true;
            }
            else
            {
                enemyInfo.m_bDefeated = false;
            }
            m_enemyInfoMap[serialNumber] = enemyInfo;
        }
    }

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvEnemyVisible, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            std::wstring work_str;
            work_str = vvs.at(i).at(0);
            if (m_enemyDefMap.find(work_str) == m_enemyDefMap.end())
            {
                throw std::exception();
            }

            if (vvs.at(i).at(1) == _T("y"))
            {
                m_enemyDefMap[work_str].SetVisible(true);
            }
            else if (vvs.at(i).at(1) == _T(""))
            {
                m_enemyDefMap[work_str].SetVisible(false);
            }
            else
            {
                throw std::exception();
            }
        }
    }
}

void EnemyInfoManager::Save(const std::wstring& csvEnemyInfo,
                            const std::wstring& csvEvemyVisible,
                            const bool encrypt,
                            const bool saveWithBinary)
{
    // テキスト形式で保存する
    if (!saveWithBinary)
    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;
        vs.push_back(_T("SerialNumber"));
        vs.push_back(_T("Name"));
        vs.push_back(_T("PosX"));
        vs.push_back(_T("PosY"));
        vs.push_back(_T("PosZ"));
        vs.push_back(_T("RotX"));
        vs.push_back(_T("RotY"));
        vs.push_back(_T("RotZ"));
        vs.push_back(_T("残りHP"));
        vs.push_back(_T("討伐済み"));
        vvs.push_back(vs);
        vs.clear();

        for (auto it = m_enemyInfoMap.begin(); it != m_enemyInfoMap.end(); ++it)
        {
            vs.push_back(std::to_wstring(it->second.m_SerialNumber));
            vs.push_back(it->second.m_id);
            vs.push_back(std::to_wstring(it->second.m_x));
            vs.push_back(std::to_wstring(it->second.m_y));
            vs.push_back(std::to_wstring(it->second.m_z));
            vs.push_back(std::to_wstring(it->second.m_rotX));
            vs.push_back(std::to_wstring(it->second.m_rotY));
            vs.push_back(std::to_wstring(it->second.m_rotZ));
            vs.push_back(std::to_wstring(it->second.m_HP));

            if (it->second.m_bDefeated)
            {
                vs.push_back(_T("y"));
            }
            else
            {
                vs.push_back(_T(""));
            }

            vvs.push_back(vs);
            vs.clear();
        }

        Util::WriteToCsv(csvEnemyInfo, vvs, encrypt);
    }
    // バイナリ形式で保存する
    else
    {
        std::vector<stEnemyInfo> stEnemyList;

        for (auto it = m_enemyInfoMap.begin(); it != m_enemyInfoMap.end(); ++it)
        {
            stEnemyList.push_back(it->second);
        }

        std::sort(stEnemyList.begin(), stEnemyList.end(),
                  [](const stEnemyInfo& x1, const stEnemyInfo& x2)
                  {
                      return x1.m_SerialNumber < x2.m_SerialNumber;
                  });

        std::ofstream outFile(csvEnemyInfo, std::ios::binary);
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
    }

    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;
        vs.push_back(_T("ID"));
        vs.push_back(_T("表示・非表示"));
        vvs.push_back(vs);
        vs.clear();

        for (auto it = m_enemyDefMap.begin(); it != m_enemyDefMap.end(); ++it)
        {
            vs.push_back(it->first);

            if (it->second.GetVisible())
            {
                vs.push_back(_T("y"));
            }
            else
            {
                vs.push_back(_T(""));
            }
            vvs.push_back(vs);
            vs.clear();
        }

        Util::WriteToCsv(csvEvemyVisible, vvs, encrypt);
    }
}

std::vector<stEnemyInfo> EnemyInfoManager::GetEnemyInfo(const float x,
                                                  const float y,
                                                  const float z,
                                                  const float r)
{
    // 球で判定すると重いので立方体で判定する
    std::vector<stEnemyInfo> enemyInfoList;
    for (auto it = m_enemyInfoMap.begin(); it != m_enemyInfoMap.end(); ++it)
    {
        float dx = it->second.m_x - x;
        if (-r <= dx && dx <= r)
        {
            float dz = it->second.m_z - z;
            if (-r <= dz && dz <= r)
            {
                enemyInfoList.push_back(it->second);
            }
        }
    }
    return enemyInfoList;
}

void EnemyInfoManager::UpdateEnemyInfo(const int serialNumber, const stEnemyInfo& enemyInfo)
{
    m_enemyInfoMap[serialNumber] = enemyInfo;
}

std::vector<std::wstring> NSStarmanLib::EnemyInfoManager::GetEnemyIdList()
{
    std::vector<std::wstring> idList;
    for (auto it = m_enemyDefMap.begin(); it != m_enemyDefMap.end(); ++it)
    {
        idList.push_back(it->second.GetID());
    }

    // IDを辞書順にソート
    std::sort(idList.begin(), idList.end(),
              [&](const std::wstring& x1, const std::wstring& x2)
              {
                  return x1 < x2;
              });

    return idList;
}

EnemyDef NSStarmanLib::EnemyInfoManager::GetEnemyDef(const std::wstring& id)
{
    EnemyDef enemyDef;
    for (auto it = m_enemyDefMap.begin(); it != m_enemyDefMap.end(); ++it)
    {
        if (it->second.GetID() == id)
        {
            enemyDef = it->second;
            break;
        }
    }
    return enemyDef;
}

void NSStarmanLib::EnemyInfoManager::SetEnemyVisible(const std::wstring& id, const bool visible)
{
    for (auto it = m_enemyDefMap.begin(); it != m_enemyDefMap.end(); ++it)
    {
        if (it->second.GetID() == id)
        {
            it->second.SetVisible(visible);
            break;
        }
    }
}

stEnemyInfo NSStarmanLib::EnemyInfoManager::GetEnemyInfo(const int serialNumber)
{
    return m_enemyInfoMap.at(serialNumber);
}

void NSStarmanLib::EnemyInfoManager::SetDefeat(const int serialNumber)
{
    m_enemyInfoMap.at(serialNumber).m_bDefeated = true;
}

void NSStarmanLib::EnemyDef::SetID(const std::wstring& arg)
{
    m_id = arg;
}

std::wstring NSStarmanLib::EnemyDef::GetID() const
{
    return m_id;
}

void NSStarmanLib::EnemyDef::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring NSStarmanLib::EnemyDef::GetName() const
{
    return m_name;
}

void NSStarmanLib::EnemyDef::SetDetail(const std::wstring& arg)
{
    m_detail = arg;
    m_detail.erase(std::remove(m_detail.begin(), m_detail.end(), '"'), m_detail.end());
}

std::wstring NSStarmanLib::EnemyDef::GetDetail() const
{
    return m_detail;
}

void NSStarmanLib::EnemyDef::SetImagePath(const std::wstring& arg)
{
    m_imagePath = arg;
}

std::wstring NSStarmanLib::EnemyDef::GetImagePath()
{
    return m_imagePath;
}

void NSStarmanLib::EnemyDef::SetVisible(const bool arg)
{
    m_visible = arg;
}

bool NSStarmanLib::EnemyDef::GetVisible() const
{
    return m_visible;
}
