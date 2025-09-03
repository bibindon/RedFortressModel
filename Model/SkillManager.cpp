#include "SkillManager.h"

#include "Util.h"

using namespace NSStarmanLib;

SkillManager* SkillManager::obj { nullptr };

void SkillDefinition::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring SkillDefinition::GetName()
{
    return m_name;
}

void SkillDefinition::SetDetail(const std::wstring& arg)
{
    m_detail = arg;
    m_detail.erase(std::remove(m_detail.begin(), m_detail.end(), '"'), m_detail.end());
}

std::wstring SkillDefinition::GetDetail()
{
    return m_detail;
}

void SkillDefinition::SetDamage(const int arg1, const float& arg2)
{
    m_damageMap[arg1] = arg2;
}

float SkillDefinition::GetDamage(const int arg)
{
    return m_damageMap[arg];
}

void SkillDefinition::SetRange(const int arg1, const float& arg2)
{
    m_rangeMap[arg1] = arg2;
}

float SkillDefinition::GetRange(const int arg)
{
    return m_rangeMap[arg];
}

SkillManager* SkillManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new SkillManager();
    }
    return obj;
}

void SkillManager::Destroy()
{
    delete SkillManager::obj;
    SkillManager::obj = nullptr;
}

void SkillManager::Init(const std::wstring& csvfileDefinition, const std::wstring& csvfilePlayer,
                        const bool decrypt)
{
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfileDefinition, decrypt);

        SkillDefinition skillDefinition;
        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            int id = std::stoi(vvs.at(i).at(0));
            skillDefinition.SetName(vvs.at(i).at(1));
            skillDefinition.SetDetail(vvs.at(i).at(2));
            skillDefinition.SetDamage(0, std::stof(vvs.at(i).at(3)));
            skillDefinition.SetDamage(1, std::stof(vvs.at(i).at(4)));
            skillDefinition.SetDamage(2, std::stof(vvs.at(i).at(5)));
            skillDefinition.SetDamage(3, std::stof(vvs.at(i).at(6)));
            skillDefinition.SetDamage(4, std::stof(vvs.at(i).at(7)));
            skillDefinition.SetDamage(5, std::stof(vvs.at(i).at(8)));
            skillDefinition.SetDamage(6, std::stof(vvs.at(i).at(9)));
            skillDefinition.SetDamage(7, std::stof(vvs.at(i).at(10)));
            skillDefinition.SetDamage(8, std::stof(vvs.at(i).at(11)));
            skillDefinition.SetDamage(9, std::stof(vvs.at(i).at(12)));
            skillDefinition.SetDamage(10, std::stof(vvs.at(i).at(13)));

            skillDefinition.SetRange(0, std::stof(vvs.at(i).at(14)));
            skillDefinition.SetRange(1, std::stof(vvs.at(i).at(15)));
            skillDefinition.SetRange(2, std::stof(vvs.at(i).at(16)));
            skillDefinition.SetRange(3, std::stof(vvs.at(i).at(17)));
            skillDefinition.SetRange(4, std::stof(vvs.at(i).at(18)));
            skillDefinition.SetRange(5, std::stof(vvs.at(i).at(19)));
            skillDefinition.SetRange(6, std::stof(vvs.at(i).at(20)));
            skillDefinition.SetRange(7, std::stof(vvs.at(i).at(21)));
            skillDefinition.SetRange(8, std::stof(vvs.at(i).at(22)));
            skillDefinition.SetRange(9, std::stof(vvs.at(i).at(23)));
            skillDefinition.SetRange(10, std::stof(vvs.at(i).at(24)));
            m_skillLevelMap[id] = skillDefinition;
        }
    }

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfilePlayer, decrypt);

        int work = 0;

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            work = std::stoi(vvs.at(i).at(1));
            m_playerSkillLevelMap[std::stoi(vvs.at(i).at(0))] = work;
        }
    }
}

void SkillManager::Save(const std::wstring& csvfile,
                        const bool encrypt)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    std::wstring work;

    vs.push_back(_T("ID"));
    vs.push_back(_T("レベル"));
    vvs.push_back(vs);
    vs.clear();

    for (auto it = m_playerSkillLevelMap.begin(); it != m_playerSkillLevelMap.end(); ++it)
    {
        vs.push_back(std::to_wstring(it->first));
        vs.push_back(std::to_wstring(it->second));
        vvs.push_back(vs);
        vs.clear();
    }

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

void SkillManager::SetSkillLevel(const std::wstring& skillName, const int skillLevel)
{
    auto it = std::find_if(m_skillLevelMap.begin(), m_skillLevelMap.end(),
                           [&](auto& pair)
                           {
                               return pair.second.GetName() == skillName;
                           });
    int id = it->first;

    if (m_playerSkillLevelMap.find(id) == m_playerSkillLevelMap.end())
    {
        throw std::exception();
    }

    m_playerSkillLevelMap[id] = skillLevel;
}

int SkillManager::GetSkillLevel(const std::wstring& skillName)
{
    auto it = std::find_if(m_skillLevelMap.begin(), m_skillLevelMap.end(),
                           [&](auto& pair)
                           {
                               return pair.second.GetName() == skillName;
                           });
    int id = it->first;

    return m_playerSkillLevelMap.at(id);
}

std::wstring SkillManager::GetDetail(const std::wstring& key)
{
    std::wstring detail;
    for (auto it = m_skillLevelMap.begin(); it != m_skillLevelMap.end(); ++it)
    {
        if (it->second.GetName() == key)
        {
            detail = it->second.GetDetail();
        }
    }

    return detail;
}

float SkillManager::GetDamage(const std::wstring& key, const int level)
{
    float damage;
    for (auto it = m_skillLevelMap.begin(); it != m_skillLevelMap.end(); ++it)
    {
        if (it->second.GetName() == key)
        {
            damage = it->second.GetDamage(level);
        }
    }

    return damage;
}

float SkillManager::GetRange(const std::wstring& key, const int level)
{
    float range;
    for (auto it = m_skillLevelMap.begin(); it != m_skillLevelMap.end(); ++it)
    {
        if (it->second.GetName() == key)
        {
            range = it->second.GetRange(level);
        }
    }

    return range;
}

std::vector<std::wstring> NSStarmanLib::SkillManager::GetNameList()
{
    std::vector<std::wstring> nameList;

    for (auto it = m_skillLevelMap.begin(); it != m_skillLevelMap.end(); ++it)
    {
        nameList.push_back(it->second.GetName());
    }

    return nameList;
}


