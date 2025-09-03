#include "Help.h"

#include "Util.h"
#include "PowereggDateTime.h"
#include "CraftSystem.h"
#include "NpcStatusManager.h"

#include <time.h>
#include <cassert>

using namespace NSStarmanLib;

Help* Help::m_obj = nullptr;

Help* NSStarmanLib::Help::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = new Help();
    }
    return m_obj;
}

void NSStarmanLib::Help::Destroy()
{
    delete m_obj;
    m_obj = nullptr;
}

void NSStarmanLib::Help::Init(const std::wstring& filepath)
{
    assert(ItemManager::GetObj()->Inited());

    // 拾ってよいアイテムのリスト
    // 3,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,25,33,34,35,38,39,40,41,42,
    // 54, 55, 56, 57
    m_enableItemIdList.clear();
    m_enableItemIdList.push_back(L"sotetsu");
    m_enableItemIdList.push_back(L"coconut");
    m_enableItemIdList.push_back(L"coconutRipe");
    m_enableItemIdList.push_back(L"donguri1");
    m_enableItemIdList.push_back(L"donguri2");
    m_enableItemIdList.push_back(L"donguri3");
    m_enableItemIdList.push_back(L"flower");
    m_enableItemIdList.push_back(L"kakan");
    m_enableItemIdList.push_back(L"tsukushi");
    m_enableItemIdList.push_back(L"unknownPlant1");
    m_enableItemIdList.push_back(L"unknownPlant2");
    m_enableItemIdList.push_back(L"unknownPlant3");
    m_enableItemIdList.push_back(L"unknownPlant4");
    m_enableItemIdList.push_back(L"unknownPlant5");
    m_enableItemIdList.push_back(L"unknownPlant6");
    m_enableItemIdList.push_back(L"tanpopo");
    m_enableItemIdList.push_back(L"nira");
    m_enableItemIdList.push_back(L"kinoko");
    m_enableItemIdList.push_back(L"copper");
    m_enableItemIdList.push_back(L"bui");
    m_enableItemIdList.push_back(L"stoneForAxe");
    m_enableItemIdList.push_back(L"stoneForSpear");
    m_enableItemIdList.push_back(L"wordbress");
    m_enableItemIdList.push_back(L"yakizakana");
    m_enableItemIdList.push_back(L"papaiya");
    m_enableItemIdList.push_back(L"mango");
    m_enableItemIdList.push_back(L"banana");
    m_enableItemIdList.push_back(L"longstone");
    m_enableItemIdList.push_back(L"mu-rugai");
    m_enableItemIdList.push_back(L"branch");
    m_enableItemIdList.push_back(L"trunk");
    m_enableItemIdList.push_back(L"tsuta");
    m_enableItemIdList.push_back(L"stone");
    m_enableItemIdList.push_back(L"ironPipe");

    m_presentMap[_T("sankakuman")] = std::vector<ItemDef>();
    m_presentMap[_T("shikakuman")] = std::vector<ItemDef>();
    
    m_presented[_T("sankakuman")] = false;
    m_presented[_T("shikakuman")] = false;

    std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(filepath, false);

    auto itemManager = ItemManager::GetObj();

    for (int i = 1; i < vvs.size(); ++i)
    {
        auto& name = vvs.at(i).at(0);
        for (int j = 1; j < vvs.at(i).size(); ++j)
        {
            if (vvs.at(i).at(j).empty())
            {
                break;
            }

            auto& itemId = vvs.at(i).at(j);
            auto itemDef = itemManager->GetItemDef(itemId);
            m_presentMap.at(name).push_back(itemDef);
        }
    }

    m_bLoaded = true;
}

// TODO NPCの健康状態を考慮
void NSStarmanLib::Help::Update()
{
    auto datetime = PowereggDateTime::GetObj();

    // 時が巻き戻っていたら回収アイテムをクリアする
    bool backtime = BackTime();

    if (backtime)
    {
        for (auto& x : m_presented)
        {
            x.second = false;
        }

        for (auto& x : m_presentMap)
        {
            x.second.clear();
        }

        m_previousYear = datetime->GetYear();
        m_previousMonth = datetime->GetMonth();
        m_previousDay = datetime->GetDay();
        m_previousHour = datetime->GetHour();
        return;
    }

    auto crossOver = CrossOver16();

    if (crossOver)
    {
        // イカダをクラフトしているならアイテム収集はナシ。

        if (!IsCraftingRaft())
        {
            srand((unsigned int)time(NULL));

            for (auto& x : m_presentMap)
            {
                // アイテムをランダムに10個以下で選択
                auto items = GetRandomItem(x.first);

                // 受け取らずに翌日の16時になったら新しいアイテムになり、古いものは消失
                x.second = items;

                m_presented.at(x.first) = false;
            }
        }
        else
        {
            for (auto& x : m_presentMap)
            {
                x.second.clear();
                m_presented.at(x.first) = false;
            }
        }
    }

    m_previousYear = datetime->GetYear();
    m_previousMonth = datetime->GetMonth();
    m_previousDay = datetime->GetDay();
    m_previousHour = datetime->GetHour();
}

void NSStarmanLib::Help::Save(const std::wstring& filepath)
{
    // m_presentMapの内容を出力
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    int work = 0;

    vs.push_back(_T("名前"));
    vs.push_back(_T("アイテム1"));
    vs.push_back(_T("アイテム2"));
    vs.push_back(_T("アイテム3"));
    vs.push_back(_T("アイテム4"));
    vs.push_back(_T("アイテム5"));
    vs.push_back(_T("アイテム6"));
    vs.push_back(_T("アイテム7"));
    vs.push_back(_T("アイテム8"));
    vs.push_back(_T("アイテム9"));
    vs.push_back(_T("アイテム10"));
    vvs.push_back(vs);
    vs.clear();

    for (auto& helper : m_presentMap)
    {
        vs.push_back(helper.first);
        for (auto& item : helper.second)
        {
            auto id = item.GetId();
            vs.push_back(id);
        }

        vvs.push_back(vs);
        vs.clear();
    }

    Util::WriteToCsv(filepath, vvs, false);
}

std::vector<ItemDef> NSStarmanLib::Help::ReceiveItems(const std::wstring& npcName)
{
    if (IsCraftingRaft())
    {
        m_presentMap.at(npcName).clear();
    }

    auto present = std::vector<ItemDef>(m_presentMap.at(npcName));

    m_presentMap.at(npcName).clear();
    m_presented.at(npcName) = true;

    return present;
}

bool NSStarmanLib::Help::Received(const std::wstring& npcName)
{
    // 空だったら渡した後。
    auto received = m_presentMap.at(npcName).empty();
    return received;
}

bool NSStarmanLib::Help::CanReceive(const std::wstring& npcName)
{
    if (!m_bLoaded)
    {
        return false;
    }

    // 渡した後、もしくは一つもないなら受け取り出来ない。
    auto empty = m_presentMap.at(npcName).empty();
    if (empty)
    {
        return false;
    }

    auto presented = m_presented.at(npcName);
    if (presented)
    {
        return false;
    }

    if (IsCraftingRaft())
    {
        m_presentMap.at(npcName).clear();
        return false;
    }

    return true;
}

std::vector<ItemDef> NSStarmanLib::Help::GetRandomItem(const std::wstring& npcName)
{
    int rnd = 0;
    int work = 0;

    // 個数をランダムで決める(1~10)
    int itemNum = 1;
    {
        // TODO 一番低い栄養素をもとに最大取得個数を求める
        auto carbo = NpcStatusManager::GetObj()->GetNpcStatus(npcName).GetCarbo();

        int maxNum = (int)(carbo / 10);

        if (maxNum != 0)
        {
            rnd = rand();
            work = rnd % maxNum + 1;
        }
        else
        {
            work = 1;
        }

        itemNum = work;
    }

    // アイテムをランダムで決める
    std::vector<ItemDef> randomItem;
    {
        auto idListNum = m_enableItemIdList.size();

        for (int i = 0; i < itemNum; ++i)
        {
            rnd = rand();
            work = rnd % idListNum;
            auto& id = m_enableItemIdList.at(work);
            auto itemDef = ItemManager::GetObj()->GetItemDef(id);
            randomItem.push_back(itemDef);
        }
    }

    return randomItem;
}

bool NSStarmanLib::Help::CrossOver16() const
{
    auto datetime = PowereggDateTime::GetObj();
    bool crossOver = false;

    if (datetime->GetYear() > m_previousYear)
    {
        crossOver = true;
    }
    else if (datetime->GetYear() == m_previousYear)
    {
        if (datetime->GetMonth() > m_previousMonth)
        {
            crossOver = true;
        }
        else if (datetime->GetMonth() == m_previousMonth)
        {
            if (datetime->GetDay() > m_previousDay)
            {
                crossOver = true;
            }
            else if (datetime->GetDay() == m_previousDay)
            {
                if (datetime->GetHour() >= 16)
                {
                    if (m_previousHour < 16)
                    {
                        crossOver = true;
                    }
                }
            }
        }
    }

    return crossOver;
}

bool NSStarmanLib::Help::BackTime() const
{
    auto datetime = PowereggDateTime::GetObj();
    bool backtime = false;

    if (datetime->GetYear() < m_previousYear)
    {
        backtime = true;
    }
    else if (datetime->GetYear() == m_previousYear)
    {
        if (datetime->GetMonth() < m_previousMonth)
        {
            backtime = true;
        }
        else if (datetime->GetMonth() == m_previousMonth)
        {
            if (datetime->GetDay() < m_previousDay)
            {
                backtime = true;
            }
            else if (datetime->GetDay() == m_previousDay)
            {
                if (datetime->GetHour() < m_previousHour)
                {
                    backtime = true;
                }
            }
        }
    }

    return backtime;
}

bool NSStarmanLib::Help::IsCraftingRaft()
{
    // "raft", "raft1"、という風に、強化値が違うとIDが異なるので注意
    bool raftCrafting = false;

    auto request = CraftSystem::GetObj()->GetCraftRequestList();

    if (!request.empty())
    {
        auto itemDef = ItemManager::GetObj()->GetItemDef(request.front().GetId());
        if (itemDef.GetUnreinforcedId() == L"raft")
        {
            raftCrafting = true;
        }
    }

    return raftCrafting;
}
