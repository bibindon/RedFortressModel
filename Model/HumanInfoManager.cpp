#include "HumanInfoManager.h"
#include "ItemManager.h"
#include "Util.h"

using namespace NSModel;

HumanInfoManager* HumanInfoManager::obj { nullptr };

HumanInfoManager* HumanInfoManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new HumanInfoManager();
    }
    return obj;
}

void HumanInfoManager::Destroy()
{
    delete HumanInfoManager::obj;
    HumanInfoManager::obj = nullptr;
}

void HumanInfoManager::Init(const std::wstring& csvfileBase, const std::wstring& csvfileSaved,
                            const bool decrypt)
{
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfileBase, decrypt);

        HumanInfo humanInfo;
        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            auto id = vvs.at(i).at(0);

            humanInfo.SetId(id);
            humanInfo.SetName(vvs.at(i).at(1));

            // 「"」記号があれば削除
            std::wstring work = vvs.at(i).at(2);
            work.erase(std::remove(work.begin(), work.end(), '"'), work.end());

            humanInfo.SetDetail(work);
            humanInfo.SetImagePath(vvs.at(i).at(3));
            m_humanInfoMap[id] = humanInfo;
        }
    }
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfileSaved, decrypt);

        if (vvs.size() == 0)
        {
            return;
        }

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            auto id = vvs.at(i).at(0);
            if (vvs.at(i).at(1) == _T("y"))
            {
                m_humanInfoMap.at(id).SetVisible(true);
            }
            else
            {
                m_humanInfoMap.at(id).SetVisible(false);
            }
        }
    }
}

void HumanInfoManager::Save(const std::wstring& csvfile,
                            const bool encrypt)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    vs.push_back(_T("ID"));
    vs.push_back(_T("名前"));
    vs.push_back(_T("表示"));
    vvs.push_back(vs);
    vs.clear();

    for (auto it = m_humanInfoMap.begin(); it != m_humanInfoMap.end(); ++it)
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

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

HumanInfo HumanInfoManager::GetHumanInfo(const std::wstring& id)
{
    HumanInfo result;
    for (auto it = m_humanInfoMap.begin(); it != m_humanInfoMap.end(); ++it)
    {
        if (it->second.GetId() == id)
        {
            result = it->second;
        }
    }

    if (result.GetId().empty())
    {
        throw std::exception("Failed to get HumanInfo");
    }

    return result;
}

std::vector<std::wstring> HumanInfoManager::GetHumanIdList()
{
    std::vector<std::wstring> result;
    for (auto it = m_humanInfoMap.begin(); it != m_humanInfoMap.end(); ++it)
    {
        result.push_back(it->second.GetId());
    }
    return result;
}

void HumanInfoManager::SetHumanVisible(const std::wstring& id)
{
    for (auto it = m_humanInfoMap.begin(); it != m_humanInfoMap.end(); ++it)
    {
        if (it->second.GetId() == id)
        {
            it->second.SetVisible(true);
        }
    }
}

void HumanInfo::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring HumanInfo::GetName()
{
    return m_name;
}

void HumanInfo::SetDetail(const std::wstring& arg)
{
    m_detail = arg;
}

std::wstring HumanInfo::GetDetail()
{
    return m_detail;
}

void HumanInfo::SetImagePath(const std::wstring& arg)
{
    m_imagePath = arg;
}

std::wstring HumanInfo::GetImagePath()
{
    return m_imagePath;
}

void HumanInfo::SetVisible(const bool arg)
{
    m_bVisible = arg;
}

bool HumanInfo::GetVisible()
{
    return m_bVisible;
}
