#include "Inventory.h"
#include "ItemManager.h"
#include "Storehouse.h"

#include <algorithm>
#include "Util.h"

using namespace NSStarmanLib;

Inventory* Inventory::obj { nullptr };

Inventory* Inventory::GetObj()
{
    if (obj == nullptr)
    {
        obj = new Inventory();
    }
    return obj;
}

void Inventory::Destroy()
{
    delete Inventory::obj;
    Inventory::obj = nullptr;
}

void Inventory::Init(const std::wstring& csvfile,
                     const bool decrypt)
{
    // ItemManagerのInit関数が先に呼ばれている必要がある。
    {
        if (ItemManager::GetObj()->Inited() == false)
        {
            throw std::exception();
        }
    }

    std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, decrypt);

    for (std::size_t i = 1; i < vvs.size(); ++i)
    {
        ItemInfo itemInfo;
        int id = 0;
        int subId = 0;
        int durability = 0;

        itemInfo.SetId(vvs.at(i).at(0));

        subId = std::stoi(vvs.at(i).at(2));
        itemInfo.SetSubId(subId);

        if (vvs.at(i).at(3).empty())
        {
            durability = -1;
        }
        else
        {
            durability = std::stoi(vvs.at(i).at(3));
        }

        itemInfo.SetDurabilityCurrent(durability);

        m_itemInfoList.push_back(itemInfo);
    }

    m_weight = CalcWeight();
    m_volumeCurrent = CalcVolume();
    m_inited = true;
}

bool NSStarmanLib::Inventory::Inited()
{
    return m_inited;
}

void Inventory::Save(const std::wstring& csvfile,
                     const bool encrypt)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    vs.push_back(_T("ID"));
    vs.push_back(_T("アイテム名"));
    vs.push_back(_T("SubID"));
    vs.push_back(_T("耐久度"));
    vvs.push_back(vs);
    vs.clear();

    ItemManager* itemManager = ItemManager::GetObj();

    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        vs.push_back(it->GetId());

        ItemDef itemDef = itemManager->GetItemDef(it->GetId());
        vs.push_back(itemDef.GetName());

        vs.push_back(std::to_wstring(it->GetSubId()));
        vs.push_back(std::to_wstring(it->GetDurabilityCurrent()));
        vvs.push_back(vs);
        vs.clear();
    }

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

int Inventory::AddItem(const std::wstring& id, const int durability)
{
    int subId = 0;

    // 新しいSubIDを取得。アイテムは削除できるのでSubIDが連番になるとは限らない。
    // SubIDは 0ではなく1からスタートする
    // SubIDが 1,2,3,5となっていたらSubID＝4のアイテムを追加する
    // SubIDが 1,2,5となっていたらSubID＝3のアイテムを追加する（4に追加するわけではない）
    // SubIDが 1,2,3,4となっていたらSubID＝5のアイテムを追加する
    // アイテムがなかったらSubID＝1のアイテムを追加する
    // 
    // インベントリと倉庫でSubIdが被らないようにする

    // 欠番となっているSubIDを探す

    // subIdのリスト
    std::vector<int> subIdList = GetSubIdList(id);

    // 倉庫のSubID
    // 倉庫はゲーム内に複数存在することに注意
    std::vector<int> subIdList2 = StorehouseManager::Get()->GetSubIdListFromAllStorehouse(id);

    // インベントリと倉庫で被っているSubIDがあるなら異常終了させる
    {
        std::vector<int> intersection;

        // 二つのリストの積集合を作る
        std::set_intersection(subIdList.begin(), subIdList.end(),
                              subIdList2.begin(), subIdList2.end(),
                              std::back_inserter(intersection));

        if (!intersection.empty())
        {
            throw std::exception();
        }
    }

    subIdList.insert(subIdList.end(), subIdList2.begin(), subIdList2.end());
    std::sort(subIdList.begin(), subIdList.end());

    // 被っているサブIDがあったら異常終了させる
    {
        for (size_t i = 1; i < subIdList.size(); ++i)
        {
            if (subIdList.at(i) == subIdList.at(i - 1))
            {
                throw std::exception();
            }
        }
    }

    int work = 1;
    int missingSubId = -1;
    for (auto it = subIdList.begin(); it != subIdList.end(); ++it)
    {
        // workとSubIDが一致するなら次のSubIDを見に行く
        if (work == *it)
        {
            ++work;
        }
        // workとSubIDが異なる＝欠番となっているSubID
        else
        {
            missingSubId = work;
            break;
        }
    }

    int newSubId = -1;

    // 欠番となっているSubIDがあったら新規SubIDとする
    if (missingSubId != -1)
    {
        newSubId = missingSubId;
    }
    // 欠番となっているSubIDがなかった（＝連番だった）なら末尾に追加する
    else
    {
        newSubId = (int)subIdList.size() + 1;
    }

    ItemInfo itemInfo;
    itemInfo.SetId(id);
    itemInfo.SetSubId(newSubId);
    itemInfo.SetDurabilityCurrent(durability);
    m_itemInfoList.push_back(itemInfo);
    Sort();

    m_weight = CalcWeight();
    m_volumeCurrent = CalcVolume();

    auto brainStamina = StatusManager::GetObj()->GetBrainStaminaCurrent();
    StatusManager::GetObj()->SetBrainStaminaCurrent(brainStamina - 1.f);

    return newSubId;
}

void NSStarmanLib::Inventory::AddItemWithSubID(const std::wstring& id, const int subId, const int durability)
{
    ItemInfo itemInfo;
    itemInfo.SetId(id);
    itemInfo.SetSubId(subId);
    itemInfo.SetDurabilityCurrent(durability);
    m_itemInfoList.push_back(itemInfo);

    Sort();

    m_weight = CalcWeight();
    m_volumeCurrent = CalcVolume();

    auto brainStamina = StatusManager::GetObj()->GetBrainStaminaCurrent();
    StatusManager::GetObj()->SetBrainStaminaCurrent(brainStamina - 1.f);
}

void Inventory::RemoveItem(const std::wstring& id, const int subId)
{
    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        if (it->GetId() == id && it->GetSubId() == subId)
        {
            m_itemInfoList.erase(it);
            break;
        }
    }

    m_weight = CalcWeight();
    m_volumeCurrent = CalcVolume();

    auto brainStamina = StatusManager::GetObj()->GetBrainStaminaCurrent();
    StatusManager::GetObj()->SetBrainStaminaCurrent(brainStamina - 1.f);
}

void NSStarmanLib::Inventory::SetItemDurability(const std::wstring& id,
                                                const int subId,
                                                const int durability)
{
    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        if (it->GetId() == id && it->GetSubId() == subId)
        {
            it->SetDurabilityCurrent(durability);
            break;
        }
    }
}

bool NSStarmanLib::Inventory::ExistItem(const std::wstring& id, const int subId)
{
    bool result = false;

    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        if (it->GetId() == id && it->GetSubId() == subId)
        {
            result = true;
            break;
        }
    }
    return result;
}

ItemInfo NSStarmanLib::Inventory::GetItemInfo(const std::wstring& id, const int subId)
{
    if (ExistItem(id, subId) == false)
    {
        throw std::exception();
    }

    ItemInfo result;

    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        if (it->GetId() == id && it->GetSubId() == subId)
        {
            result = *it;
            break;
        }
    }
    return result;
}

// SubIDをリストで受け取る
std::vector<int> NSStarmanLib::Inventory::GetSubIdList(const std::wstring& id)
{
    std::vector<int> result;
    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        if (it->GetId() == id)
        {
            result.push_back(it->GetSubId());
        }
    }

    return result;
}


// 耐久度を無視して個数を数える
// したがって、耐久度の下がったアイテムをクラフトの素材として使用出来て良いということにする
int Inventory::CountItem(const std::wstring& id)
{
    int num = 0;
    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        if (it->GetId() == id)
        {
            num++;
        }
    }

    return num;
}

float Inventory::GetWeight()
{
    return m_weight;
}

std::list<ItemInfo> NSStarmanLib::Inventory::GetAllItem()
{
    return m_itemInfoList;
}

float NSStarmanLib::Inventory::CalcVolume()
{
    float result = 0.f;
    ItemManager* itemManager = ItemManager::GetObj();
    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        auto id = it->GetId();
        ItemDef itemDef = itemManager->GetItemDef(id);
        if (itemDef.GetId().empty())
        {
            continue;
        }
        result += itemDef.GetVolume();
    }

    m_volumeCurrent = result;
    return result;
}

float NSStarmanLib::Inventory::GetVolume() const
{
    return m_volumeCurrent;
}

void NSStarmanLib::Inventory::UpdateVolumeMax(const std::vector<ItemInfo>& bagMap)
{
    // 強化値0の袋→積載量3000mL
    // 強化値1の袋→積載量4000mL
    // 強化値2の袋→積載量5000mL
    // 強化値3の袋→積載量6000mL
    // 強化値4の袋→積載量7000mL
    // 強化値5の袋→積載量8000mL

    m_volumeMax = 5000.f; // 袋がなくても5リットルまでは持てる

    for (auto it = bagMap.begin(); it != bagMap.end(); ++it)
    {
        if (it->GetId().empty())
        {
            continue;
        }

        if (it->GetDurabilityCurrent() <= 0)
        {
            continue;
        }

        if (it->GetItemDef().GetLevel() == -1)
        {
            m_volumeMax += 3000;
        }
        else if (it->GetItemDef().GetLevel() == 1)
        {
            m_volumeMax += 4000;
        }
        else if (it->GetItemDef().GetLevel() == 2)
        {
            m_volumeMax += 5000;
        }
        else if (it->GetItemDef().GetLevel() == 3)
        {
            m_volumeMax += 6000;
        }
        else if (it->GetItemDef().GetLevel() == 4)
        {
            m_volumeMax += 7000;
        }
        else if (it->GetItemDef().GetLevel() == 5)
        {
            m_volumeMax += 8000;
        }
    }
}

float NSStarmanLib::Inventory::GetVolumeMax() const
{
    return m_volumeMax;
}

void NSStarmanLib::Inventory::ReduceEquipBagDurability()
{
    auto allBag = StatusManager::GetObj()->GetAllBag();

    for (auto it = allBag.begin(); it != allBag.end(); ++it)
    {
        for (auto it2 = m_itemInfoList.begin(); it2 != m_itemInfoList.end(); ++it2)
        {
            if (it->GetId() == it2->GetId() && it->GetSubId() == it2->GetSubId())
            {
                auto dura = it2->GetDurabilityCurrent();
                if (dura >= 1)
                {
                    it2->SetDurabilityCurrent(dura - 1);
                    it->SetDurabilityCurrent(dura - 1);
                }
                break;
            }
        }
    }
    UpdateVolumeMax(allBag);

    StatusManager::GetObj()->UpdateBagDurability();
}

float Inventory::CalcWeight()
{
    float result = 0.f;
    ItemManager* itemManager = ItemManager::GetObj();
    for (auto it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it)
    {
        auto id = it->GetId();
        ItemDef itemDef = itemManager->GetItemDef(id);
        if (itemDef.GetId().empty())
        {
            continue;
        }
        result += itemDef.GetWeight();
    }
    return result;
}

void NSStarmanLib::Inventory::Sort()
{
    m_itemInfoList.sort([](const ItemInfo& lhs, const ItemInfo& rhs)
                        {
                            if (lhs.GetId() < rhs.GetId())
                            {
                                return true;
                            }
                            else if (lhs.GetId() == rhs.GetId())
                            {
                                if (lhs.GetSubId() < rhs.GetSubId())
                                {
                                    return true;
                                }
                            }
                            return false;
                        });
}

