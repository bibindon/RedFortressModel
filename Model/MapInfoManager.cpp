#include "MapInfoManager.h"

#include "Util.h"

using namespace NSStarmanLib;

MapInfoManager* MapInfoManager::obj { nullptr };

void MapInfo::SetId(const std::wstring& id)
{
    m_id = id;
}

std::wstring MapInfo::GetId() const
{
    return m_id;
}

void MapInfo::SetName(const std::wstring& name)
{
    m_name = name;
}

std::wstring MapInfo::GetName()
{
    return m_name;
}

void MapInfo::SetDetail(const std::wstring& detail)
{
    m_detail = detail;

    // 「"」記号は取り除く
    m_detail.erase(std::remove(m_detail.begin(), m_detail.end(), '"'), m_detail.end());
}

std::wstring MapInfo::GetDetail()
{
    return m_detail;
}

void MapInfo::SetPos(const int x, const int y)
{
    m_x = x;
    m_y = y;
}

void MapInfo::GetPos(int* x, int* y)
{
    *x = m_x;
    *y = m_y;
}

void NSStarmanLib::MapInfo::SetImagePath(const std::wstring& imagePath)
{
    m_imagePath = imagePath;
}

std::wstring NSStarmanLib::MapInfo::GetImagePath()
{
    return m_imagePath;
}

void MapInfo::SetDiscovered(const bool discovered)
{
    m_isDiscovered = discovered;
}

bool MapInfo::IsDiscovered()
{
    return m_isDiscovered;
}

MapInfoManager* MapInfoManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new MapInfoManager();
    }
    return obj;
}

void MapInfoManager::Init(const std::wstring& csvfile, const std::wstring& saveFile, const bool decrypt)
{
    m_mapInfoList.clear();

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            MapInfo mapInfo;
            mapInfo.SetId(vvs.at(i).at(0));
            mapInfo.SetName(vvs.at(i).at(1));
            mapInfo.SetDetail(vvs.at(i).at(2));
            int x = std::stoi(vvs.at(i).at(3));
            int y = std::stoi(vvs.at(i).at(4));
            mapInfo.SetPos(x, y);

            mapInfo.SetImagePath(vvs.at(i).at(5));
            m_mapInfoList.push_back(mapInfo);
        }
    }

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(saveFile, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            auto id = vvs.at(i).at(0);
            auto it = std::find_if(m_mapInfoList.begin(), m_mapInfoList.end(),
                                   [&] (MapInfo& x)
                                   {
                                       return x.GetId() == id;
                                   });

            if (vvs.at(i).at(1) == L"y")
            {
                it->SetDiscovered(true);
            }
        }
    }
}

void MapInfoManager::Save(const std::wstring& csvfile, const bool encrypt)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    vs.push_back(_T("ID"));
    vs.push_back(_T("Unclocked"));
    vvs.push_back(vs);
    vs.clear();

    for (std::size_t i = 0; i < m_mapInfoList.size(); ++i)
    {
        vs.push_back(m_mapInfoList.at(i).GetId());

        if (m_mapInfoList.at(i).IsDiscovered())
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

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

void MapInfoManager::Destroy()
{
    delete MapInfoManager::obj;
    MapInfoManager::obj = nullptr;
}

std::vector<std::wstring> MapInfoManager::GetIdList()
{
    std::vector<std::wstring> vs;
    for (std::size_t i = 0; i < m_mapInfoList.size(); ++i)
    {
        vs.push_back(m_mapInfoList.at(i).GetId());
    }
    return vs;
}

bool MapInfoManager::IsDiscovered(const std::wstring& id)
{
    auto it = std::find_if(m_mapInfoList.begin(),
                           m_mapInfoList.end(),
                           [&](MapInfo mi) { return mi.GetId() == id; });

    return it->IsDiscovered();
}

void MapInfoManager::SetDiscovered(const std::wstring& id)
{
    auto it = std::find_if(m_mapInfoList.begin(),
                           m_mapInfoList.end(),
                           [&] (MapInfo mi) { return mi.GetId() == id; });

    it->SetDiscovered(true);
}

std::wstring NSStarmanLib::MapInfoManager::GetName(const std::wstring& id)
{
    auto it = std::find_if(m_mapInfoList.begin(),
                           m_mapInfoList.end(),
                           [&] (MapInfo mi) { return mi.GetId() == id; });

    return it->GetName();
}

std::wstring MapInfoManager::GetDetail(const std::wstring& id)
{
    auto it = std::find_if(m_mapInfoList.begin(),
                           m_mapInfoList.end(),
                           [&](MapInfo mi) { return mi.GetId() == id; });

    return it->GetDetail();
}

void MapInfoManager::GetPos(const std::wstring& id, int* x, int* y)
{
    auto it = std::find_if(m_mapInfoList.begin(),
                           m_mapInfoList.end(),
                           [&] (MapInfo mi) { return mi.GetId() == id; });

    it->GetPos(x, y);
}

std::wstring NSStarmanLib::MapInfoManager::GetImagePath(const std::wstring& id)
{
    auto it = std::find_if(m_mapInfoList.begin(),
                           m_mapInfoList.end(),
                           [&](MapInfo mi) { return mi.GetId() == id; });

    return it->GetImagePath();
}

