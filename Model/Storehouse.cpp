#include "ItemManager.h"
#include "Storehouse.h"

#include <algorithm>
#include "Inventory.h"
#include "Voyage.h"
#include "ActivityBase.h"

#include "Util.h"

using namespace NSStarmanLib;

void Storehouse::Init(const std::wstring& csvfile,
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

        durability = std::stoi(vvs.at(i).at(3));
        itemInfo.SetDurabilityCurrent(durability);

        m_itemInfoList.push_back(itemInfo);
    }

    m_weight = CalcWeight();

    // 保存するときのためにファイル名を保存
    size_t pos = csvfile.find_last_of(_T("/\\"));

    if (pos == std::wstring::npos)
    {
        m_csvfilename = csvfile;
    }
    else
    {
        m_csvfilename = csvfile.substr(pos + 1);
    }
}

void Storehouse::Save(const std::wstring& csvfile,
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

void Storehouse::AddItem(const std::wstring& id, const int durability)
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
    Inventory* inventory = Inventory::GetObj();

    std::vector<int> subIdList2 = inventory->GetSubIdList(id);

    // インベントリと倉庫で被っているSubIDがあるなら異常終了させる
    {
        std::vector<int> intersection;

        // 二つのリストの積集合を作る
        std::set_intersection(subIdList.begin(), subIdList.end(),
                              subIdList2.begin(), subIdList2.end(),
                              std::back_inserter(intersection));

        if (intersection.empty() == false)
        {
            throw std::exception();
        }
    }

    subIdList.insert(subIdList.end(), subIdList2.begin(), subIdList2.end());
    std::sort(subIdList.begin(), subIdList.end());

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
}

void NSStarmanLib::Storehouse::AddItemWithSubID(const std::wstring& id, const int subId, const int durability)
{
    ItemInfo itemInfo;
    itemInfo.SetId(id);
    itemInfo.SetSubId(subId);
    itemInfo.SetDurabilityCurrent(durability);
    m_itemInfoList.push_back(itemInfo);

    Sort();
    m_weight = CalcWeight();
}

void Storehouse::RemoveItem(const std::wstring& id, const int subId)
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
}

void NSStarmanLib::Storehouse::SetItemDurability(const std::wstring& id,
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

bool NSStarmanLib::Storehouse::ExistItem(const std::wstring& id, const int subId)
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

ItemInfo NSStarmanLib::Storehouse::GetItemInfo(const std::wstring& id, const int subId)
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
std::vector<int> NSStarmanLib::Storehouse::GetSubIdList(const std::wstring& id)
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
int Storehouse::CountItem(const std::wstring& id)
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

float Storehouse::GetWeight()
{
    return m_weight;
}

std::list<ItemInfo> NSStarmanLib::Storehouse::GetAllItem()
{
    return m_itemInfoList;
}

void NSStarmanLib::Storehouse::SetXYZ(const float x, const float y, const float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void NSStarmanLib::Storehouse::GetXYZ(float* x, float* y, float* z)
{
    *x = m_x;
    *y = m_y;
    *z = m_z;
}

void NSStarmanLib::Storehouse::SetIsRaft(const bool arg)
{
    m_bRaft = arg;
}

bool NSStarmanLib::Storehouse::GetIsRaft() const
{
    return m_bRaft;
}

void NSStarmanLib::Storehouse::SetCsvFileName(const std::wstring arg)
{
    m_csvfilename = arg;
}

std::wstring NSStarmanLib::Storehouse::GetCsvFileName() const
{
    return m_csvfilename;
}

float Storehouse::CalcWeight()
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

void NSStarmanLib::Storehouse::Sort()
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

StorehouseManager* NSStarmanLib::StorehouseManager::m_obj = nullptr;

StorehouseManager* NSStarmanLib::StorehouseManager::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = new StorehouseManager();
    }
    return m_obj;
}

void NSStarmanLib::StorehouseManager::Destroy()
{
    delete StorehouseManager::m_obj;
    StorehouseManager::m_obj = nullptr;
}

void NSStarmanLib::StorehouseManager::Init(const std::wstring& csvfile)
{
    std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, false);

    int work_i = 0;
    float work_f1 = 0;
    float work_f2 = 0;
    float work_f3 = 0;

    for (size_t i = 1; i < vvs.size(); ++i)
    {
        Storehouse storehouse;
        storehouse.Init(vvs.at(i).at(1));

        if (vvs.at(i).at(5) == _T("y"))
        {
            storehouse.SetIsRaft(true);
        }
        else if (vvs.at(i).at(5) == _T("n"))
        {
            storehouse.SetIsRaft(false);

            work_f1 = std::stof(vvs.at(i).at(2));
            work_f2 = std::stof(vvs.at(i).at(3));
            work_f3 = std::stof(vvs.at(i).at(4));

            storehouse.SetXYZ(work_f1, work_f2, work_f3);
        }
        else
        {
            throw std::exception();
        }

        work_i = std::stoi(vvs.at(i).at(0));
        m_StorehouseMap[work_i] = storehouse;
    }

    m_inited = true;
}

bool NSStarmanLib::StorehouseManager::Inited()
{
    return m_inited;
}

void NSStarmanLib::StorehouseManager::Save(const std::wstring& managerFile,
                                           const std::wstring& csvDir)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;

    vs.push_back(_T("ID"));
    vs.push_back(_T("倉庫ファイル"));
    vs.push_back(_T("x"));
    vs.push_back(_T("y"));
    vs.push_back(_T("z"));
    vs.push_back(_T("イカダ"));

    vvs.push_back(vs);
    vs.clear();

    for (auto it = m_StorehouseMap.begin(); it != m_StorehouseMap.end(); ++it)
    {
        vs.clear();

        vs.push_back(std::to_wstring(it->first));

        std::wstring work;
        work = csvDir;
        work += _T("/") + it->second.GetCsvFileName();

        vs.push_back(work);

        float x, y, z;

        it->second.GetXYZ(&x, &y, &z);

        vs.push_back(std::to_wstring(x));
        vs.push_back(std::to_wstring(y));
        vs.push_back(std::to_wstring(z));

        if (it->second.GetIsRaft())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }
        vvs.push_back(vs);
    }

    Util::WriteToCsv(managerFile, vvs, false);

    for (auto it = m_StorehouseMap.begin(); it != m_StorehouseMap.end(); ++it)
    {
        std::wstring work;
        work = csvDir;
        work += _T("/") + it->second.GetCsvFileName();

        it->second.Save(work);
    }
}

std::vector<int> NSStarmanLib::StorehouseManager::GetStorehouseIdList()
{
    std::vector<int> vi;
    for (auto it = m_StorehouseMap.begin(); it != m_StorehouseMap.end(); ++it)
    {
        vi.push_back(it->first);
    }

    return vi;
}

Storehouse* NSStarmanLib::StorehouseManager::GetStorehouse(const int id)
{
    return &m_StorehouseMap.at(id);
}

Storehouse* NSStarmanLib::StorehouseManager::GetNearStorehouse(const float x, const float z)
{
    Storehouse* result = nullptr;
    for (auto it = m_StorehouseMap.begin(); it != m_StorehouseMap.end(); ++it)
    {
        if (!it->second.GetIsRaft())
        {
            float x2, y2, z2;

            it->second.GetXYZ(&x2, &y2, &z2);

            auto hit = Util::HitByBoundingBoxWithoutY(x, z, x2, z2, 3.f);
            if (hit)
            {
                result = &it->second;
                break;
            }
        }
    }

    if (result == nullptr)
    {
        // イカダの袋
        // イカダの袋は移動できるのでこの関数が呼ばれたタイミングで座標を取得する必要がある
        for (auto& raft : Voyage::Get()->GetRaftList())
        {
            float x2, y2, z2;

            raft.GetXYZ(&x2, &y2, &z2);

            auto hit = Util::HitByBoundingBoxWithoutY(x, z, x2, z2, 3.f);
            if (hit)
            {
                auto id = raft.GetStorehouseId();
                result = &m_StorehouseMap.at(id);
                break;
            }
        }
    }

    return result;
}

Storehouse* NSStarmanLib::StorehouseManager::GetCurrentActiveStorehouse()
{
    if (ActivityBase::Get()->GetBaseType() == eBaseType::Precision)
    {
        return &m_StorehouseMap.at(1);
    }
    else
    {
        return &m_StorehouseMap.at(2);
    }
}

std::vector<int> NSStarmanLib::StorehouseManager::GetSubIdListFromAllStorehouse(const std::wstring& id)
{
    std::vector<int> vi;

    for (auto it = m_StorehouseMap.begin(); it != m_StorehouseMap.end(); ++it)
    {
        auto work = it->second.GetSubIdList(id);
        vi.insert(vi.end(), work.begin(), work.end());
    }

    std::sort(vi.begin(), vi.end());

    return vi;
}

void NSStarmanLib::StorehouseManager::AddStorehouse()
{
    Storehouse storehouse;

    storehouse.SetIsRaft(true);

    // _T("storehouseRaft1.csv")のようなファイル名を作る
    std::wstring work;

    int RaftNum = 0;
    for (auto it = m_StorehouseMap.begin(); it != m_StorehouseMap.end(); ++it)
    {
        if (it->second.GetIsRaft())
        {
            ++RaftNum;
        }
    }

    work = _T("storehouseRaft") + std::to_wstring(RaftNum+1) + _T(".csv");

    storehouse.SetCsvFileName(work);

    size_t size_ = m_StorehouseMap.size() + 1;
    m_StorehouseMap[(int)size_] = storehouse;
}

