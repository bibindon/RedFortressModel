#include "CraftSystem.h"
#include "PowereggDateTime.h"
#include "ItemManager.h"
#include "Storehouse.h"
#include "Voyage.h"
#include "ActivityBase.h"
#include "Util.h"
#include "StatusManager.h"
#include "WeaponManager.h"

using namespace NSModel;

CraftSystem* CraftSystem::obj { nullptr };

CraftSystem* CraftSystem::GetObj()
{
    if (obj == nullptr)
    {
        obj = new CraftSystem();
    }
    return obj;
}

void CraftSystem::Destroy()
{
    delete CraftSystem::obj;
    CraftSystem::obj = nullptr;
}

void CraftSystem::Init(const std::wstring& csvfileSkill, const std::wstring& csvfileQueue,
                       const bool decrypt)
{
    // CraftSystemのInit関数より先に、CraftInfoManagerのInitが呼ばれている必要がある。
    {
        CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
        if (craftInfoManager->Inited() == false)
        {
            throw std::exception("CraftInfoManager::Init() must be called before CraftSystem::Init() called.");
        }
    }

    {
        std::vector<std::vector<std::wstring> > vvs = Util::ReadFromCsv(csvfileSkill, decrypt);

        if (vvs.empty())
        {
            throw std::exception();
        }

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            CraftSkill craftSkill;

            std::wstring name = vvs.at(i).at(0);
            craftSkill.SetId(name);

            int level = 0;
            if (vvs.at(i).at(1).empty())
            {
                level = -1;
            }
            else
            {
                level = std::stoi(vvs.at(i).at(1));
            }
            craftSkill.SetLevel(level);

            if (vvs.at(i).at(2) == _T("○"))
            {
                craftSkill.SetEnable(true);
            }
            else
            {
                craftSkill.SetEnable(false);
            }

            int work = 0;
            if (vvs.at(i).at(3).empty())
            {
                work = -1;
            }
            else
            {
                work = std::stoi(vvs.at(i).at(3));
            }
            craftSkill.SetLevelUpNecessity(work);

            if (vvs.at(i).at(4).empty())
            {
                work = -1;
            }
            else
            {
                work = std::stoi(vvs.at(i).at(4));
            }
            craftSkill.SetSuccessNum(work);

            m_craftSkillList.push_back(craftSkill);
        }
    }
    {
        std::vector<std::vector<std::wstring> > vvs = Util::ReadFromCsv(csvfileQueue, decrypt);

        if (vvs.empty())
        {
            throw std::exception();
        }

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();

            std::wstring name = vvs.at(i).at(0);

            int number = 0;
            number = std::stoi(vvs.at(i).at(1));

            int level = 0;

            if (vvs.at(i).at(2).empty())
            {
                level = -1;
            }
            else
            {
                level = std::stoi(vvs.at(i).at(2));
            }

            // クラフト情報を取得
            CraftInfo craftInfo = craftInfoManager->GetCraftInfo(name, number, level);

            // クラフト中のアイテム情報
            CraftRequest craftRequest;

            craftRequest.SetCraftInfo(craftInfo);

            if (vvs.at(i).at(3) == _T("○"))
            {
                craftRequest.SetCrafting(true);
            }
            else
            {
                craftRequest.SetCrafting(false);
            }

            if (craftRequest.GetCrafting())
            {
                craftRequest.SetStartYear(std::stoi(vvs.at(i).at(4)));
                craftRequest.SetStartMonth(std::stoi(vvs.at(i).at(5)));
                craftRequest.SetStartDay(std::stoi(vvs.at(i).at(6)));
                craftRequest.SetStartHour(std::stoi(vvs.at(i).at(7)));
                craftRequest.SetStartMinute(std::stoi(vvs.at(i).at(8)));
                craftRequest.SetStartSecond(std::stoi(vvs.at(i).at(9)));

                craftRequest.SetFinishYear(std::stoi(vvs.at(i).at(10)));
                craftRequest.SetFinishMonth(std::stoi(vvs.at(i).at(11)));
                craftRequest.SetFinishDay(std::stoi(vvs.at(i).at(12)));
                craftRequest.SetFinishHour(std::stoi(vvs.at(i).at(13)));
                craftRequest.SetFinishMinute(std::stoi(vvs.at(i).at(14)));
                craftRequest.SetFinishSecond(std::stoi(vvs.at(i).at(15)));
            }
            else
            {
                craftRequest.SetStartYear(-1);
                craftRequest.SetStartMonth(-1);
                craftRequest.SetStartDay(-1);
                craftRequest.SetStartHour(-1);
                craftRequest.SetStartMinute(-1);
                craftRequest.SetStartSecond(-1);

                craftRequest.SetFinishYear(-1);
                craftRequest.SetFinishMonth(-1);
                craftRequest.SetFinishDay(-1);
                craftRequest.SetFinishHour(-1);
                craftRequest.SetFinishMinute(-1);
                craftRequest.SetFinishSecond(-1);
            }

            craftRequest.SetStorehouseId(std::stoi(vvs.at(i).at(16)));

            m_craftRequestList.push_back(craftRequest);
        }
    }
}

void NSModel::CraftSystem::Save(const std::wstring& csvfileSkill,
                                     const std::wstring& csvfileQueue,
                                     const bool encrypt)
{
    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;

        vs.push_back(_T("クラフトアイテムID"));
        vs.push_back(_T("強化値"));
        vs.push_back(_T("クラフト可能"));
        vs.push_back(_T("次のレベルを習得するのに必要なクラフト回数"));
        vs.push_back(_T("現在のクラフト経験回数"));
        vvs.push_back(vs);
        vs.clear();

        for (auto it = m_craftSkillList.begin(); it != m_craftSkillList.end(); ++it)
        {
            vs.push_back(it->GetId());

            if (it->GetLevel() == -1)
            {
                vs.push_back(_T(""));
            }
            else
            {
                vs.push_back(std::to_wstring(it->GetLevel()));
            }

            if (it->GetEnable())
            {
                vs.push_back(_T("○"));
            }
            else
            {
                vs.push_back(_T(""));
            }

            if (it->GetLevelUpNecessity() == -1)
            {
                vs.push_back(_T(""));
            }
            else
            {
                vs.push_back(std::to_wstring(it->GetLevelUpNecessity()));
            }

            if (it->GetSuccessNum() == -1)
            {
                throw std::exception();
            }
            else
            {
                vs.push_back(std::to_wstring(it->GetSuccessNum()));
            }

            vvs.push_back(vs);
            vs.clear();
        }

        Util::WriteToCsv(csvfileSkill, vvs, encrypt);
    }
    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;

        vs.push_back(_T("クラフトアイテムID"));
        vs.push_back(_T("個数"));
        vs.push_back(_T("強化値"));
        vs.push_back(_T("クラフト中"));
        vs.push_back(_T("開始年"));
        vs.push_back(_T("開始月"));
        vs.push_back(_T("開始日"));
        vs.push_back(_T("開始時"));
        vs.push_back(_T("開始分"));
        vs.push_back(_T("開始秒"));
        vs.push_back(_T("完了年"));
        vs.push_back(_T("完了月"));
        vs.push_back(_T("完了日"));
        vs.push_back(_T("完了時"));
        vs.push_back(_T("完了分"));
        vs.push_back(_T("完了秒"));
        vs.push_back(_T("収納先倉庫ID"));
        vvs.push_back(vs);
        vs.clear();

        for (auto it = m_craftRequestList.begin(); it != m_craftRequestList.end(); ++it)
        {
            // ID
            vs.push_back(it->GetId());

            // Number
            vs.push_back(std::to_wstring(it->GetNumber()));

            // Level
            int level = it->GetCraftInfo().GetOutput().GetLevel();
            if (level != -1)
            {
                vs.push_back(std::to_wstring(level));
            }
            else
            {
                vs.push_back(_T(""));
            }

            if (it->GetCrafting())
            {
                vs.push_back(_T("○"));
            }
            else
            {
                vs.push_back(_T(""));
            }

            if (it->GetCrafting())
            {
                vs.push_back(std::to_wstring(it->GetStartYear()));
                vs.push_back(std::to_wstring(it->GetStartMonth()));
                vs.push_back(std::to_wstring(it->GetStartDay()));
                vs.push_back(std::to_wstring(it->GetStartHour()));
                vs.push_back(std::to_wstring(it->GetStartMinute()));
                vs.push_back(std::to_wstring(it->GetStartSecond()));

                vs.push_back(std::to_wstring(it->GetFinishYear()));
                vs.push_back(std::to_wstring(it->GetFinishMonth()));
                vs.push_back(std::to_wstring(it->GetFinishDay()));
                vs.push_back(std::to_wstring(it->GetFinishHour()));
                vs.push_back(std::to_wstring(it->GetFinishMinute()));
                vs.push_back(std::to_wstring(it->GetFinishSecond()));
            }
            else
            {
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));

                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
                vs.push_back(_T(""));
            }

            vs.push_back(std::to_wstring(it->GetStorehouseId()));

            vvs.push_back(vs);
            vs.clear();
        }

        Util::WriteToCsv(csvfileQueue, vvs, encrypt);
    }
}

void NSModel::CraftSystem::SetCraftsmanSkill(const std::wstring& unreinforcedId, const int level)
{
    // +3の石斧を作れるようにする場合は、+0, +1, +2の石斧も作れるようにする。
    for (auto it = m_craftSkillList.begin(); it != m_craftSkillList.end(); ++it)
    {
        auto itemDef2 = ItemManager::GetObj()->GetItemDef(it->GetId());
        auto unreinforcedId2 = itemDef2.GetUnreinforcedId();
        // 強化値が同じなら、職人のスキルを有効にする
        if (unreinforcedId == unreinforcedId2 && it->GetLevel() <= level)
        {
            it->SetEnable(true);
        }
    }
}

// アイテムのIDは強化値によってraft, raft1, raft2のように変わる。
int NSModel::CraftSystem::GetCraftsmanSkill(const std::wstring& unreinforcedId)
{
    // ＋１の石斧と＋２の石斧が作れて、＋３の石斧が作れないなら2を返す。
    // （craftItemの作れるアイテムの中で最高レベルの数値を返す。）
    int level = -1;
    for (auto it = m_craftSkillList.begin(); it != m_craftSkillList.end(); ++it)
    {
        auto itemDef = ItemManager::GetObj()->GetItemDef(it->GetId());
        bool find = (itemDef.GetUnreinforcedId() == unreinforcedId);
        if (find && it->GetEnable())
        {
            if (it->GetLevel() > level)
            {
                level = it->GetLevel();
            }
        }
    }
    return level;
}

bool NSModel::CraftSystem::QueueCraftRequest(const std::wstring& itemId,
                                                  std::wstring* errMsg,
                                                  const int storehouseId,
                                                  const int num)
{
    // 会話したら脳のスタミナを消費
    auto brainStamina = StatusManager::GetObj()->GetBrainStaminaCurrent();
    StatusManager::GetObj()->SetBrainStaminaCurrent(brainStamina - 1.f);

    // 予約は5件まで
    if (m_craftRequestList.size() >= 5)
    {
        *errMsg = _T("予約は５件までにしておこう");
        return false;
    }

    CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();

    auto itemDef2 = ItemManager::GetObj()->GetItemDef(itemId);
    auto unreinforcedId = itemDef2.GetUnreinforcedId();

    // 現在、職人が作れるcraftItemのレベルを取得
    // ＋１の石斧と＋２の石斧が作れて、＋３の石斧が作れないなら2を取得。
    int level = GetCraftsmanSkill(unreinforcedId);

    // raft, raft1, raft2...のように、強化値が変わるとアイテムIDも変わる
    // 強化値に対応するIDを取得する。

    std::wstring newItemId;

    {
        auto idList = ItemManager::GetObj()->GetItemIdList();

        for (size_t i = 0; i < idList.size(); ++i)
        {
            auto itemDef2 = ItemManager::GetObj()->GetItemDef(idList.at(i));
            auto unreinforcedId2 = itemDef2.GetUnreinforcedId();

            if (unreinforcedId == unreinforcedId2)
            {
                if (level == itemDef2.GetLevel())
                {
                    newItemId = itemDef2.GetId();
                }
            }
        }
    }

    if (newItemId.empty())
    {
        throw std::exception();
    }

    // クラフト情報
    CraftInfo craftInfo = craftInfoManager->GetCraftInfo(newItemId, num, level);

    // 素材を消費する
    // 素材が足りないときはfalseを返す
    ItemManager* itemManager = ItemManager::GetObj();
    std::vector<CraftMaterial> craftMaterialList = craftInfo.GetCraftMaterialDef();

    auto storehouse = StorehouseManager::Get()->GetCurrentActiveStorehouse();
    bool materialShortage = false;

    // 倉庫内に必要なだけの素材があるかのチェック
    for (std::size_t i = 0; i < craftMaterialList.size(); ++i)
    {
        int materialNum = 0;

        materialNum = craftMaterialList.at(i).GetNumber();

        int materialNumCurrent = storehouse->CountItem(craftMaterialList.at(i).GetId());

        // 素材が足りない
        if (materialNumCurrent < materialNum)
        {
            materialShortage = true;
            break;
        }
    }

    if (materialShortage)
    {
        *errMsg = _T("素材が足りない");
        return false;
    }

    // インベントリ内の素材を削除
    // （クラフト開始前にキャンセルしたら素材が返ってくる）
    std::vector<ItemInfo> items;
    for (std::size_t i = 0; i < craftMaterialList.size(); ++i)
    {
        ItemInfo item;

        int materialNum = 0;

        materialNum = craftMaterialList.at(i).GetNumber();

        auto id = craftMaterialList.at(i).GetId();
        item.SetId(id);

        // 素材の必要数分削除する
        // subIdは数値が若いものから順に使う

        std::vector<int> subIdList = storehouse->GetSubIdList(craftMaterialList.at(i).GetId());
        for (int j = 0; j < materialNum; ++j)
        {
            // subIdは返却時に新規で割り当てる。再利用しない。
            item.SetSubId(-1);
            auto info = storehouse->GetItemInfo(id, subIdList.at(j));
            auto dura = info.GetDurabilityCurrent();
            item.SetDurabilityCurrent(dura);
            items.push_back(item);

            storehouse->RemoveItem(craftMaterialList.at(i).GetId(), subIdList.at(j));
        }
    }
    craftInfo.SetCraftMaterial(items);

    // クラフト中のアイテム情報
    CraftRequest craftRequest;

    // 作りたいものをセット
    // クラフト開始時間とクラフト完了時間はここでは設定しない（クラフト要求はキューイング出来るため）
    craftRequest.SetCraftInfo(craftInfo);
    craftRequest.SetCrafting(false);

    //----------------------------------------------------------
    // 完成した際の格納先の倉庫
    //----------------------------------------------------------

    // イカダをクラフトする場合は倉庫に入らないので-1
    auto itemDef = ItemManager::GetObj()->GetItemDef(newItemId);
    if (itemDef.GetUnreinforcedId() == L"raft")
    {
        craftRequest.SetStorehouseId(-1);
    }
    else
    {
        if (storehouseId != -1)
        {
            craftRequest.SetStorehouseId(storehouseId);
        }
        // -1なら現在の拠点の倉庫
        else
        {
            auto baseType = ActivityBase::Get()->GetBaseType();

            if (baseType == eBaseType::Precision)
            {
                craftRequest.SetStorehouseId(1);
            }
            else if (baseType == eBaseType::DirectNex)
            {
                craftRequest.SetStorehouseId(2);
            }
        }
    }

    m_craftRequestList.push_back(craftRequest);

    return true;
}

bool NSModel::CraftSystem::CancelCraftStart(const int index)
{
    if ((int)m_craftRequestList.size() <= index)
    {
        return false;
    }

    auto storehouse = StorehouseManager::Get()->GetCurrentActiveStorehouse();

    // 開始していないクラフトをキャンセルしたら素材が返ってくる
    if (index != 0)
    {
        auto it = std::next(m_craftRequestList.begin(), index);
        auto craftInfo = it->GetCraftInfo();
        auto craftMaterials = craftInfo.GetCraftMaterial();
        for (auto& material : craftMaterials)
        {
            auto id = material.GetId();
            int dura = material.GetDurabilityCurrent();
            storehouse->AddItem(id, dura);
        }
    }

    m_craftRequestList.erase(std::next(m_craftRequestList.begin(), index));
    return true;
}

void NSModel::CraftSystem::UpdateCraftStatus()
{
    // リクエストがないならやることなし
    if (m_craftRequestList.size() == 0)
    {
        return;
    }

    // クラフトリクエストの先頭がクラフト中でないならば、クラフトを開始する
    if (m_craftRequestList.front().GetCrafting() == false)
    {
        StartCraft();
    }
    // クラフトリクエストの先頭がクラフト中ならばクラフト完了でないか確認する
    else if (m_craftRequestList.front().GetCrafting())
    {

        // 日付同士の比較
        // 1年2月3日4時5分6秒と6年5月4日3時2分1秒のどちらが未来・過去かを判定するには
        // 010203040506 と 060504030201 という12桁の数値と見なして12桁の数値同士の比較を行えばよい
        int y1, M1, d1, h1, m1, s1;
        int y2, M2, d2, h2, m2, s2;

        y1 = m_craftRequestList.front().GetFinishYear();
        M1 = m_craftRequestList.front().GetFinishMonth();
        d1 = m_craftRequestList.front().GetFinishDay();
        h1 = m_craftRequestList.front().GetFinishHour();
        m1 = m_craftRequestList.front().GetFinishMinute();
        s1 = m_craftRequestList.front().GetFinishSecond();

        PowereggDateTime* obj = PowereggDateTime::GetObj();
        y2 = obj->GetYear();
        M2 = obj->GetMonth();
        d2 = obj->GetDay();
        h2 = obj->GetHour();
        m2 = obj->GetMinute();
        s2 = obj->GetSecond();

        bool fromPastToFuture = PowereggDateTime::FromPastToFuture(y1, M1, d1, h1, m1, s1, 
                                                                   y2, M2, d2, h2, m2, s2);

        // 作業完了時刻よりも現在時刻新しい、ことは完成していることを意味する
        if (fromPastToFuture)
        {
            // 完成したアイテムを倉庫に配置する
            CraftOutput output = m_craftRequestList.front().GetCraftInfo().GetOutput();

            int storehouseId = m_craftRequestList.front().GetStorehouseId();

            Storehouse* storehouse = nullptr;

            // イカダの場合は倉庫に入れないため
            // 倉庫IDが-1にセットされている。-1の時は倉庫の取得をしない
            if (storehouseId == -1)
            {
                // Do nothing
            }
            else
            {
                storehouse = StorehouseManager::Get()->GetStorehouse(storehouseId);
            }

            for (int i = 0; i < output.GetNumber(); ++i)
            {
                // イカダの場合は倉庫に入れない。
                auto itemDef = ItemManager::GetObj()->GetItemDef(output.GetItemId());
                if (itemDef.GetUnreinforcedId() == L"raft")
                {
                    auto voyage = Voyage::Get();
                    Raft raft;

                    auto itemManager = ItemManager::GetObj();
                    auto itemDef = itemManager->GetItemDef(output.GetItemId());

                    // 使われていないIDを探してセットする
                    {
                        auto raftList = Voyage::Get()->GetRaftList();
                        int newID = 1;

                        while (true)
                        {
                            auto it = std::find_if(raftList.begin(), raftList.end(),
                                                   [ & ] (const Raft& r)
                                                   {
                                                       return r.GetId() == newID;
                                                   });

                            if (it != raftList.end())
                            {
                                // すでに存在するIDならば、次のIDを探す
                                newID++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        raft.SetId(newID);
                    }

                    raft.SetLevel(itemDef.GetLevel());

                    auto dura = itemDef.GetDurabilityMax();
                    raft.SetDurability(dura);

                    // 現在の活動拠点によってイカダが配置される場所が異なる
                    auto baseType = ActivityBase::Get()->GetBaseType();

                    float x, y, z;
                    ActivityBase::Get()->GetPierPos(baseType, &x, &y, &z);
                    raft.SetXYZ(x, y, z);

                    if (baseType == eBaseType::Precision)
                    {
                         raft.SetPosType(Raft::ePosType::River);
                    }
                    else if (baseType == eBaseType::DirectNex)
                    {
                         raft.SetPosType(Raft::ePosType::Sea);
                    }

                    auto storehouseList = StorehouseManager::Get()->GetStorehouseIdList();
                    StorehouseManager::Get()->AddStorehouse();
                    raft.SetStorehouseId((int)storehouseList.size() + 1);

                    voyage->AddRaft(raft);
                }
                else
                {
                    if (itemDef.GetType() != ItemDef::ItemType::WEAPON)
                    {
                        storehouse->AddItem(output.GetItemId(), itemDef.GetDurabilityMax());
                    }
                    else
                    {
                        auto dura = WeaponManager::GetObj()->GetDurabilityMax2(itemDef.GetWeaponId(), itemDef.GetLevel());
                        storehouse->AddItem(output.GetItemId(), dura);
                    }
                }
            }

            // 職人の熟練度の更新
            // 強化値なしの袋を1回作ったら、次は強化値＋１の袋を作れるようになる。
            // そのため作った回数を記録する
            bool levelup = false;
            for (std::size_t i = 0; i < m_craftSkillList.size(); ++i)
            {
                if (output.GetItemId() == m_craftSkillList.at(i).GetId() &&
                    output.GetLevel() == m_craftSkillList.at(i).GetLevel())
                {
                    int successNum = m_craftSkillList.at(i).GetSuccessNum();
                    ++successNum;
                    m_craftSkillList.at(i).SetSuccessNum(successNum);

                    // レベルアップ判定
                    // 規定回数、クラフトに成功していたら次の強化値のクラフトを可能にする
                    if (m_craftSkillList.at(i).GetLevelUpNecessity() <= successNum)
                    {
                        levelup = true;
                    }
                    break;
                }
            }

            // レベルアップ処理
            // まず、強化値が一つ上のクラフトが存在するかを調べる。
            if (levelup)
            {
                // 強化が+1~+5で可能の場合、強化値は-1, 1, 2, 3, 4, 5の6通りとなる。
                // 0が含まれないことに注意。
                int targetSkill = -1;
                bool targetSkillExist = false;
                if (output.GetLevel() == -1)
                {
                    targetSkill = 1;
                }
                else if (output.GetLevel() >= 1)
                {
                    targetSkill = output.GetLevel() + 1;
                }
                else
                {
                    throw std::exception();
                }

                std::size_t targetSkillIndex = 0;
                auto outputUnreinforcedId = ItemManager::GetObj()->GetItemDef(output.GetItemId()).GetUnreinforcedId();
                for (std::size_t i = 0; i < m_craftSkillList.size(); ++i)
                {
                    auto craftSkillUnreinforcedId = ItemManager::GetObj()->GetItemDef(m_craftSkillList.at(i).GetId()).GetUnreinforcedId();

                    if (outputUnreinforcedId == craftSkillUnreinforcedId &&
                        targetSkill == m_craftSkillList.at(i).GetLevel())
                    {
                        targetSkillExist = true;
                        targetSkillIndex = i;
                        break;
                    }
                }

                // 存在するならクラフト可能に変更する
                if (targetSkillExist)
                {
                    if (m_craftSkillList.at(targetSkillIndex).GetEnable() == false)
                    {
                        m_craftSkillList.at(targetSkillIndex).SetEnable(true);
                    }
                }
            }


            // 先頭の要素をリクエストのリストから削除
            m_craftRequestList.pop_front();

            // 予約リストが０ではないなら、新たに戦闘の要素となったリクエストをクラフト開始
            if (m_craftRequestList.size() >= 1)
            {
                StartCraft();
            }
        }
    }
}

void NSModel::CraftSystem::StartCraft()
{
    //-----------------------------------------------
    // 袋のクラフトを3件予約したとする。
    // 袋は2回作成すると、熟練度が上がり、強化値の高い袋をクラフトできるようになる。
    // そのため、3個目の袋は１，２個目の袋より高い強化値の袋が作られなくてはいけない。
    // これに対応するため、クラフト開始時に熟練度を確認する
    //-----------------------------------------------
    {
        auto& req = m_craftRequestList.front();
        auto output = req.GetCraftInfo().GetOutput();

        auto unreinforcedId = ItemManager::GetObj()->GetItemDef(output.GetItemId()).GetUnreinforcedId();

        int level = GetCraftsmanSkill(unreinforcedId);
        if (level != output.GetLevel())
        {
            output.SetLevel(level);

            // 強化値が変わるとアイテムIDも変わる。
            // bag, bag1, bag2...
            // そのため新しいIDを割り振る
            {
                std::wstring newId;
                output.GetItemId();

                auto idList = ItemManager::GetObj()->GetItemIdList();
                for (size_t i = 0; i < idList.size(); ++i)
                {
                    auto itemDef = ItemManager::GetObj()->GetItemDef(idList.at(i));
                    if (itemDef.GetUnreinforcedId() == unreinforcedId)
                    {
                        if (itemDef.GetLevel() == level)
                        {
                            newId = itemDef.GetId();
                        }
                    }
                }

                if (newId.empty())
                {
                    throw std::exception();
                }

                output.SetItemId(newId);
            }

            auto craftInfo = m_craftRequestList.front().GetCraftInfo();
            craftInfo.SetOutput(output);
            m_craftRequestList.front().SetCraftInfo(craftInfo);
        }
    }

    m_craftRequestList.front().SetCrafting(true);

    // パワーエッグ星での現在時刻とクラフト完了時刻を設定する
    PowereggDateTime* obj = PowereggDateTime::GetObj();
    m_craftRequestList.front().SetStartYear(obj->GetYear());
    m_craftRequestList.front().SetStartMonth(obj->GetMonth());
    m_craftRequestList.front().SetStartDay(obj->GetDay());
    m_craftRequestList.front().SetStartHour(obj->GetHour());
    m_craftRequestList.front().SetStartMinute(obj->GetMinute());
    m_craftRequestList.front().SetStartSecond(obj->GetSecond());

    // 完了時刻は24時間後。
    // 完了時刻をアイテムごとに変えるのは難しくないのでやっても良いような気がする
    // 1月32日や13月は存在しないので、その対応を行うために
    // 秒、分、時、というように細かいほうから設定していく
    m_craftRequestList.front().SetFinishSecond(obj->GetSecond());
    m_craftRequestList.front().SetFinishMinute(obj->GetMinute());
    m_craftRequestList.front().SetFinishHour(obj->GetHour());

    // 1月31日とか2月28日のような月末でないならば日数に＋１する。
    if (obj->GetDay() < obj->DAY_OF_MONTH(obj->GetMonth()))
    {
        m_craftRequestList.front().SetFinishDay(obj->GetDay() + 1);
        m_craftRequestList.front().SetFinishMonth(obj->GetMonth());
        m_craftRequestList.front().SetFinishYear(obj->GetYear());
    }
    // 月末ならば日数を1日にする
    else
    {
        m_craftRequestList.front().SetFinishDay(1);

        // 月数を+1する
        // 13月にならないように注意する
        if (obj->GetMonth() != 12)
        {
            m_craftRequestList.front().SetFinishMonth(obj->GetMonth() + 1);
            m_craftRequestList.front().SetFinishYear(obj->GetYear());
        }
        // 12月ならば、完了日を翌年の1月にする
        else if (obj->GetMonth() == 12)
        {
            m_craftRequestList.front().SetFinishMonth(1);
            m_craftRequestList.front().SetFinishYear(obj->GetYear() + 1);
        }
    }
}

int NSModel::CraftSystem::GetProgress()
{
    if (m_craftRequestList.front().GetCrafting() == false)
    {
        return -1;
    }

    // 24時間で完成なので1時間で4%上昇させる。分・秒は考えない。
    // 経過時間
    int elapsedHour = 0;

    PowereggDateTime* obj = PowereggDateTime::GetObj();
    int currentHour = obj->GetHour();
    int finishHour = m_craftRequestList.front().GetFinishHour();
    if (currentHour > finishHour)
    {
        elapsedHour = currentHour - finishHour;
    }
    // 現在時刻が0時を超えたら24を足してから引く
    else if (currentHour < finishHour)
    {
        elapsedHour = (currentHour + 24) - finishHour;
    }
    else if (currentHour == finishHour)
    {
        if (obj->GetDay() == m_craftRequestList.front().GetStartDay())
        {
            elapsedHour = 0;
        }
        else
        {
            elapsedHour = 24;
        }
    }

    auto progress = elapsedHour * 100 / 24;

    // 100だったら99％にしておく
    if (progress == 100)
    {
        progress = 99;
    }

    return progress;
}

std::list<CraftRequest> NSModel::CraftSystem::GetCraftRequestList()
{
    return m_craftRequestList;
}

std::wstring CraftRequest::GetId() const
{
    return m_craftInfo.GetOutput().GetItemId();
}

int NSModel::CraftRequest::GetNumber() const
{
    return m_craftInfo.GetOutput().GetNumber();
}

int NSModel::CraftRequest::GetLevel() const
{
    return m_craftInfo.GetOutput().GetLevel();
}

int CraftRequest::GetStartYear() const
{
    return m_startYear;
}

void CraftRequest::SetStartYear(int startYear)
{
    m_startYear = startYear;
}

int CraftRequest::GetStartMonth() const
{
    return m_startMonth;
}

void CraftRequest::SetStartMonth(int startMonth)
{
    m_startMonth = startMonth;
}

int CraftRequest::GetStartDay() const
{
    return m_startDay;
}

void CraftRequest::SetStartDay(int startDay)
{
    m_startDay = startDay;
}

int CraftRequest::GetStartHour() const
{
    return m_startHour;
}

void CraftRequest::SetStartHour(int startHour)
{
    m_startHour = startHour;
}

int CraftRequest::GetStartMinute() const
{
    return m_startMinute;
}

void CraftRequest::SetStartMinute(int startMinute)
{
    m_startMinute = startMinute;
}

int CraftRequest::GetStartSecond() const
{
    return m_startSecond;
}

void CraftRequest::SetStartSecond(int startSecond)
{
    m_startSecond = startSecond;
}

int CraftRequest::GetFinishYear() const
{
    return m_finishYear;
}

void CraftRequest::SetFinishYear(int mfinishYear)
{
    m_finishYear = mfinishYear;
}

int CraftRequest::GetFinishMonth() const
{
    return m_finishMonth;
}

void CraftRequest::SetFinishMonth(int mfinishMonth)
{
    m_finishMonth = mfinishMonth;
}

int CraftRequest::GetFinishDay() const
{
    return m_finishDay;
}

void CraftRequest::SetFinishDay(int mfinishDay)
{
    m_finishDay = mfinishDay;
}

int CraftRequest::GetFinishHour() const
{
    return m_finishHour;
}

void CraftRequest::SetFinishHour(int mfinishHour)
{
    m_finishHour = mfinishHour;
}

int CraftRequest::GetFinishMinute() const
{
    return m_finishMinute;
}

void CraftRequest::SetFinishMinute(int mfinishMinute)
{
    m_finishMinute = mfinishMinute;
}

int CraftRequest::GetFinishSecond() const
{
    return m_finishSecond;
}

void CraftRequest::SetFinishSecond(int mfinishSecond)
{
    m_finishSecond = mfinishSecond;
}

int NSModel::CraftRequest::GetStorehouseId() const
{
    return m_storehouseId;
}

void NSModel::CraftRequest::SetStorehouseId(const int id)
{
    m_storehouseId = id;
}

CraftInfo NSModel::CraftRequest::GetCraftInfo() const
{
    return m_craftInfo;
}

void NSModel::CraftRequest::SetCraftInfo(const CraftInfo& arg)
{
    m_craftInfo = arg;
}

bool NSModel::CraftRequest::GetCrafting() const
{
    return m_crafting;
}

void NSModel::CraftRequest::SetCrafting(const bool arg)
{
    m_crafting = arg;
}

void NSModel::CraftSkill::SetId(const std::wstring& arg)
{
    m_id = arg;
}

std::wstring NSModel::CraftSkill::GetId() const
{
    return m_id;
}

void NSModel::CraftSkill::SetLevel(const int arg)
{
    m_level = arg;
}

int NSModel::CraftSkill::GetLevel() const
{
    return m_level;
}

void NSModel::CraftSkill::SetEnable(const bool arg)
{
    m_enable = arg;
}

bool NSModel::CraftSkill::GetEnable() const
{
    return m_enable;
}

void NSModel::CraftSkill::SetLevelUpNecessity(const int arg)
{
    m_levelUpNecessity = arg;
}

int NSModel::CraftSkill::GetLevelUpNecessity() const
{
    return m_levelUpNecessity;
}

void NSModel::CraftSkill::SetSuccessNum(const int arg)
{
    m_successNum = arg;
}

int NSModel::CraftSkill::GetSuccessNum() const
{
    return m_successNum;
}

