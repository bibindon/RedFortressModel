#include "NpcStatusManager.h"
#include "Storehouse.h"
#include "Rynen.h"
#include "Util.h"
#include "PowereggDateTime.h"
#include "StatusManager.h"

using namespace NSModel;

NpcStatusManager* NpcStatusManager::obj { nullptr };

void NpcStatus::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring NpcStatus::GetName()
{
    return m_name;
}

void NpcStatus::SetCarbo(const float arg)
{
    m_carbo = arg;
}

float NpcStatus::GetCarbo() const
{
    return m_carbo;
}

void NpcStatus::SetProtein(const float arg)
{
    m_protein = arg;
}

float NpcStatus::GetProtein() const
{
    return m_protein;
}

void NpcStatus::SetLipid(const float arg)
{
    m_lipid = arg;
}

float NpcStatus::GetLipid() const
{
    return m_lipid;
}

void NpcStatus::SetVitamin(const float arg)
{
    m_vitamin = arg;
}

float NpcStatus::GetVitamin() const
{
    return m_vitamin;
}

void NpcStatus::SetMineral(const float arg)
{
    m_mineral = arg;
}

float NpcStatus::GetMineral() const
{
    return m_mineral;
}

void NpcStatus::SetWater(const float arg)
{
    m_water = arg;
}

float NpcStatus::GetWater() const
{
    return m_water;
}

void NpcStatus::SetRynenContract()
{
    m_rynenContract = true;
}

float NpcStatus::GetRynenContract() const
{
    return m_rynenContract;
}

void NpcStatus::SetDrinkWordbress(const bool arg)
{
    m_bDrinkWordbress = arg;
}

bool NpcStatus::GetDrinkWordbress() const
{
    return m_bDrinkWordbress;
}

void NpcStatus::SetDead()
{
    m_dead = true;
}

bool NpcStatus::GetDead() const
{
    return m_dead;
}

void NSModel::NpcStatus::SetX(const float arg)
{
    m_x = arg;
}

float NSModel::NpcStatus::GetX() const
{
    return m_x;
}

void NSModel::NpcStatus::SetY(const float arg)
{
    m_y = arg;
}

float NSModel::NpcStatus::GetY() const
{
    return m_y;
}

void NSModel::NpcStatus::SetZ(const float arg)
{
    m_z = arg;
}

float NSModel::NpcStatus::GetZ() const
{
    return m_z;
}

void NSModel::NpcStatus::SetRotY(const float arg)
{
    m_rotY = arg;
}

float NSModel::NpcStatus::GetRotY() const
{
    return m_rotY;
}

void NSModel::NpcStatus::SetHasTalk(const bool arg)
{
    m_bHasTalk = arg;
}

bool NSModel::NpcStatus::GetHasTalk() const
{
    return m_bHasTalk;
}

void NSModel::NpcStatus::SetTalkCsv(const std::wstring& arg)
{
    m_talkCsv = arg;
}

std::wstring NSModel::NpcStatus::GetTalkCsv() const
{
    return m_talkCsv;
}

void NSModel::NpcStatus::SetFeatureEnable(const bool arg)
{
    m_bFeatureEnable = arg;
}

bool NSModel::NpcStatus::GetFeatureEnable() const
{
    return m_bFeatureEnable;
}

void NSModel::NpcStatus::SetNpcFeature(const eNpcFeature arg)
{
    m_eNpcFeature = arg;
}

eNpcFeature NSModel::NpcStatus::GetNpcFeature() const
{
    return m_eNpcFeature;
}

void NSModel::NpcStatus::SetRedMan(const bool arg)
{
    m_bRedman = arg;
}

bool NSModel::NpcStatus::IsRedMan() const
{
    return m_bRedman;
}

void NSModel::NpcStatus::SetRedManDay(const int year, const int month, const int day)
{
    m_redManYear = year;
    m_redManMonth = month;
    m_redManDay = day;
}

void NSModel::NpcStatus::GetRedManDay(int* year, int* month, int* day)
{
    *year = m_redManYear;
    *month = m_redManMonth;
    *day = m_redManDay;
}

NpcStatusManager* NpcStatusManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new NpcStatusManager();
    }
    return obj;
}

void NpcStatusManager::Destroy()
{
    delete NpcStatusManager::obj;
    NpcStatusManager::obj = nullptr;
}

void NpcStatusManager::Init(const std::wstring& csvfile,
                            const bool decrypt)
{
    srand((unsigned int)time(NULL));
    std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, decrypt);

    for (std::size_t i = 1; i < vvs.size(); ++i)
    {
        float work_f = 0.f;

        NpcStatus npcStatus;

        npcStatus.SetName(vvs.at(i).at(0));

        work_f = std::stof(vvs.at(i).at(1));
        npcStatus.SetCarbo(work_f);

        work_f = std::stof(vvs.at(i).at(2));
        npcStatus.SetProtein(work_f);

        work_f = std::stof(vvs.at(i).at(3));
        npcStatus.SetLipid(work_f);

        work_f = std::stof(vvs.at(i).at(4));
        npcStatus.SetVitamin(work_f);

        work_f = std::stof(vvs.at(i).at(5));
        npcStatus.SetMineral(work_f);

        work_f = std::stof(vvs.at(i).at(6));
        npcStatus.SetWater(work_f);

        if (vvs.at(i).at(7) == _T("y"))
        {
            npcStatus.SetRynenContract();
        }

        if (vvs.at(i).at(8) == _T("y"))
        {
            npcStatus.SetDrinkWordbress(true);
        }
        else
        {
            npcStatus.SetDrinkWordbress(false);
        }

        if (vvs.at(i).at(9) == _T("y"))
        {
            npcStatus.SetDead();
        }

        work_f = std::stof(vvs.at(i).at(10));
        npcStatus.SetX(work_f);

        work_f = std::stof(vvs.at(i).at(11));
        npcStatus.SetY(work_f);

        work_f = std::stof(vvs.at(i).at(12));
        npcStatus.SetZ(work_f);

        work_f = std::stof(vvs.at(i).at(13));
        npcStatus.SetRotY(work_f);

        if (vvs.at(i).at(14) == _T("y"))
        {
            npcStatus.SetHasTalk(true);
        }
        else
        {
            npcStatus.SetHasTalk(false);
        }

        npcStatus.SetTalkCsv(vvs.at(i).at(15));

        if (vvs.at(i).at(16) == _T("y"))
        {
            npcStatus.SetFeatureEnable(true);
        }
        else
        {
            npcStatus.SetFeatureEnable(false);
        }

        if (vvs.at(i).at(17) == _T("CRAFTMAN"))
        {
            npcStatus.SetNpcFeature(eNpcFeature::CRAFTMAN);
        }
        else if (vvs.at(i).at(17) == _T("PATCH_TEST"))
        {
            npcStatus.SetNpcFeature(eNpcFeature::PATCH_TEST);
        }
        else if (vvs.at(i).at(17) == _T("CRAFTMAN_AND_PATCH_TEST"))
        {
            npcStatus.SetNpcFeature(eNpcFeature::CRAFTMAN_AND_PATCH_TEST);
        }
        else if (vvs.at(i).at(17) == _T("HELP"))
        {
            npcStatus.SetNpcFeature(eNpcFeature::HELP);
        }
        else
        {
            npcStatus.SetNpcFeature(eNpcFeature::NONE);
        }

        if (vvs.at(i).at(18) == _T("y"))
        {
            npcStatus.SetRedMan(true);
        }
        else
        {
            npcStatus.SetRedMan(false);
        }

        {
            int work1 = 0;
            int work2 = 0;
            int work3 = 0;

            if (!vvs.at(i).at(19).empty())
            {
                work1 = std::stoi(vvs.at(i).at(19));
            }

            if (!vvs.at(i).at(20).empty())
            {
                work2 = std::stoi(vvs.at(i).at(20));
            }

            if (!vvs.at(i).at(21).empty())
            {
                work3 = std::stoi(vvs.at(i).at(21));
            }

            npcStatus.SetRedManDay(work1, work2, work3);
        }

        m_NpcStatusMap[npcStatus.GetName()] = npcStatus;
    }
}

void NpcStatusManager::Save(const std::wstring& csvfile,
                            const bool encrypt)
{
    std::vector<std::vector<std::wstring> > vvs;
    std::vector<std::wstring> vs;
    std::wstring work;

    vs.push_back(_T("名前"));
    vs.push_back(_T("糖質"));
    vs.push_back(_T("タンパク質"));
    vs.push_back(_T("脂質"));
    vs.push_back(_T("ビタミン"));
    vs.push_back(_T("ミネラル"));
    vs.push_back(_T("水分"));
    vs.push_back(_T("ライネンの契約をしている"));
    vs.push_back(_T("ワードブレスを飲んでいる"));
    vs.push_back(_T("死んでいる"));
    vs.push_back(_T("x"));
    vs.push_back(_T("y"));
    vs.push_back(_T("z"));
    vs.push_back(_T("yRot"));
    vs.push_back(_T("会話可能"));
    vs.push_back(_T("会話スクリプト"));
    vs.push_back(_T("機能解禁"));
    vs.push_back(_T("機能種別"));
    vs.push_back(_T("レッドマン"));
    vs.push_back(_T("レッドマンになった年"));
    vs.push_back(_T("レッドマンになった月"));
    vs.push_back(_T("レッドマンになった日"));
    vvs.push_back(vs);
    vs.clear();

    for (auto it = m_NpcStatusMap.begin(); it != m_NpcStatusMap.end(); ++it)
    {
        std::wstring work;
        vs.push_back(it->first);

        work = std::to_wstring(it->second.GetCarbo());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetProtein());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetLipid());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetVitamin());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetMineral());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetWater());
        vs.push_back(work);

        if (it->second.GetRynenContract())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        if (it->second.GetDrinkWordbress())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        if (it->second.GetDead())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        work = std::to_wstring(it->second.GetX());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetY());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetZ());
        vs.push_back(work);

        work = std::to_wstring(it->second.GetRotY());
        vs.push_back(work);

        if (it->second.GetHasTalk())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        vs.push_back(it->second.GetTalkCsv());

        if (it->second.GetFeatureEnable())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        if (it->second.GetNpcFeature() == eNpcFeature::CRAFTMAN)
        {
            vs.push_back(_T("CRAFTMAN"));
        }
        else if (it->second.GetNpcFeature() == eNpcFeature::PATCH_TEST)
        {
            vs.push_back(_T("PATCH_TEST"));
        }
        else if (it->second.GetNpcFeature() == eNpcFeature::CRAFTMAN_AND_PATCH_TEST)
        {
            vs.push_back(_T("CRAFTMAN_AND_PATCH_TEST"));
        }
        else if (it->second.GetNpcFeature() == eNpcFeature::HELP)
        {
            vs.push_back(_T("HELP"));
        }
        else
        {
            vs.push_back(_T(""));
        }

        if (it->second.IsRedMan())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        {
            int year = 0;
            int month = 0;
            int day = 0;
            it->second.GetRedManDay(&year, &month, &day);

            vs.push_back(std::to_wstring(year));
            vs.push_back(std::to_wstring(month));
            vs.push_back(std::to_wstring(day));
        }

        vvs.push_back(vs);
        vs.clear();
    }

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

void NSModel::NpcStatusManager::Update()
{
    // １秒に１回呼ばれる想定
    {
        // 暫定的に、糖質、脂質、タンパク質、ミネラル、ビタミン、水分はどれも20日で0になることとする。
        // １秒での消費量は100/20/24/60/60
        // 水分は90で死亡なので、5日で10減るようにする
        float work_f = 100.f;
        float waterCost = 0.f;
        work_f /= 20;
        work_f /= 24;
        work_f /= 60;
        work_f /= 60;

        // 1秒で、ゲーム内では12秒経過する
        work_f *= 12;
        waterCost = work_f;

        // 担架モード中で水中だったら7200倍する。
        // 20秒（ゲーム内で240秒）で死亡する計算
        if (StatusManager::GetObj()->IsStretcherMode() && StatusManager::GetObj()->IsUnderwater())
        {
            work_f *= 7200;
        }

        for (auto& npc : m_NpcStatusMap)
        {
            // 体力を消費するのは3人だけ
            if (npc.first == _T("daikeiman") ||
                npc.first == _T("sankakuman") ||
                npc.first == _T("shikakuman"))
            {
                if (npc.second.GetDead())
                {
                    continue;
                }

                float work_f2 = 0.f;
                work_f2 = npc.second.GetCarbo();
                npc.second.SetCarbo(work_f2 - work_f);

                work_f2 = npc.second.GetProtein();
                npc.second.SetProtein(work_f2 - work_f);

                work_f2 = npc.second.GetLipid();
                npc.second.SetLipid(work_f2 - (work_f));

                work_f2 = npc.second.GetMineral();
                npc.second.SetMineral(work_f2 - work_f);

                work_f2 = npc.second.GetVitamin();
                npc.second.SetVitamin(work_f2 - work_f);

                // 水分は減少量を0.5にする。
                work_f2 = npc.second.GetWater();
                npc.second.SetWater(work_f2 - (waterCost * 0.5f));
            }
        }
    }

    // ステータスが低い場合は倉庫から食料を減らす
    {
        for (auto& npc : m_NpcStatusMap)
        {
            // 体力を消費するのは3人だけ
            if (npc.first == _T("daikeiman") ||
                npc.first == _T("sankakuman") ||
                npc.first == _T("shikakuman"))
            {
                if (npc.second.GetDead())
                {
                    continue;
                }

                // 水分が足りないとき
                if (npc.second.GetWater() <= 95.f)
                {
                    auto storageManager = StorehouseManager::Get();
                    auto storage = storageManager->GetCurrentActiveStorehouse();
                    auto allItem = storage->GetAllItem();

                    // ランダムで一つ消費
                    // 赤い実は食べない。
                    // キノコ、ニラ・スイセン、大きいどんぐり、は糖質が10のときまで候補にならない。
                    std::vector<ItemInfo> foodList;

                    for (auto& item : allItem)
                    {
                        if (item.GetItemDef().GetType() == ItemDef::ItemType::FOOD)
                        {
                            // 糖質が10以下ならソテツ以外は何でも食べる
                            if (npc.second.GetCarbo() <= 10.f)
                            {
                                if (item.GetId() != L"sotetsu")
                                {
                                    foodList.push_back(item);
                                }
                            }
                            else
                            {
                                if (
                                    item.GetId() == L"unknownPlant1" ||
                                    item.GetId() == L"unknownPlant2" ||
                                    item.GetId() == L"unknownPlant4" ||
                                    item.GetId() == L"unknownPlant5" ||
                                    item.GetId() == L"unknownPlant6" ||
                                    item.GetId() == L"mango" ||
                                    item.GetId() == L"papaiya" ||
                                    item.GetId() == L"banana" ||
                                    item.GetId() == L"coconut" ||
                                    item.GetId() == L"coconutRipe"
                                    )
                                {
                                    foodList.push_back(item);
                                }
                            }
                        }
                    }

                    auto foodSize = foodList.size();

                    if (foodSize != 0)
                    {
                        int rand_ = rand();
                        int pickup = rand_ % foodSize;

                        storage->RemoveItem(foodList.at(pickup).GetId(), foodList.at(pickup).GetSubId());

                        // 体力の回復
                        Eat(npc.first, foodList.at(pickup).GetItemDef());
                    }
                }

                if (npc.second.GetCarbo() <= 50.f)
                {
                    auto storageManager = StorehouseManager::Get();
                    auto storage = storageManager->GetCurrentActiveStorehouse();
                    auto allItem = storage->GetAllItem();

                    // ランダムで一つ消費
                    // 赤い実は食べない。
                    // キノコ、ニラ・スイセン、大きいどんぐり、は糖質が10のときまで候補にならない。
                    std::vector<ItemInfo> foodList;

                    for (auto& item : allItem)
                    {
                        if (item.GetItemDef().GetType() == ItemDef::ItemType::FOOD)
                        {
                            // 糖質が10以下ならソテツ以外は何でも食べる
                            if (npc.second.GetCarbo() <= 10.f)
                            {
                                if (item.GetId() != L"sotetsu")
                                {
                                    foodList.push_back(item);
                                }
                            }
                            else
                            {
                                if (
                                    item.GetId() == L"sotetsuDetox" ||
                                    item.GetId() == L"coconut" ||
                                    item.GetId() == L"coconutRipe" ||
                                    item.GetId() == L"donguri1" ||
                                    item.GetId() == L"donguri2" ||
                                    item.GetId() == L"donguriDetox" ||
                                    item.GetId() == L"tsukushi" ||
                                    item.GetId() == L"unknownPlant1" ||
                                    item.GetId() == L"unknownPlant2" ||
                                    item.GetId() == L"unknownPlant3" ||
                                    item.GetId() == L"unknownPlant4" ||
                                    item.GetId() == L"unknownPlant5" ||
                                    item.GetId() == L"unknownPlant6" ||
                                    item.GetId() == L"tanpopo" ||
                                    item.GetId() == L"yakizakana" ||
                                    item.GetId() == L"papaiya" ||
                                    item.GetId() == L"mango" ||
                                    item.GetId() == L"banana" ||
                                    item.GetId() == L"mu-rugai"
                                    )
                                {
                                    foodList.push_back(item);
                                }
                            }
                        }
                    }

                    auto foodSize = foodList.size();

                    if (foodSize != 0)
                    {
                        int rand_ = rand();
                        int pickup = rand_ % foodSize;

                        storage->RemoveItem(foodList.at(pickup).GetId(), foodList.at(pickup).GetSubId());

                        // 体力の回復
                        Eat(npc.first, foodList.at(pickup).GetItemDef());
                    }
                }

                // ビタミンが足りないときは草を食べる
                if (npc.second.GetVitamin() <= 50.f)
                {
                    auto storageManager = StorehouseManager::Get();
                    auto storage = storageManager->GetCurrentActiveStorehouse();
                    auto allItem = storage->GetAllItem();

                    // ランダムで一つ消費
                    // 赤い実は食べない。
                    // キノコ、ニラ・スイセン、大きいどんぐり、は糖質が10のときまで候補にならない。
                    std::vector<ItemInfo> foodList;

                    for (auto& item : allItem)
                    {
                        if (item.GetItemDef().GetType() == ItemDef::ItemType::FOOD)
                        {
                            if (
                                item.GetId() == L"unknownPlant1" ||
                                item.GetId() == L"unknownPlant2" ||
                                item.GetId() == L"unknownPlant3" ||
                                item.GetId() == L"unknownPlant4" ||
                                item.GetId() == L"unknownPlant5" ||
                                item.GetId() == L"unknownPlant6" ||
                                item.GetId() == L"tsukushi" ||
                                item.GetId() == L"tanpopo" ||
                                item.GetId() == L"yakizakana" ||
                                item.GetId() == L"papaiya" ||
                                item.GetId() == L"banana" ||
                                item.GetId() == L"mu-rugai"
                                )
                            {
                                foodList.push_back(item);
                            }
                        }
                    }

                    auto foodSize = foodList.size();

                    if (foodSize != 0)
                    {
                        int rand_ = rand();
                        int pickup = rand_ % foodSize;

                        storage->RemoveItem(foodList.at(pickup).GetId(), foodList.at(pickup).GetSubId());

                        // 体力の回復
                        Eat(npc.first, foodList.at(pickup).GetItemDef());
                    }
                }
            }
        }
    }

    // 瀕死だったらワードブレスを飲む
    {
        // 主人公が契約した後、NPCも使用可能になる。たまたまだけど・・・。
        if (Rynen::GetObj()->GetContracted())
        {
            for (auto& npc : m_NpcStatusMap)
            {
                if (npc.first == _T("daikeiman") ||
                    npc.first == _T("sankakuman") ||
                    npc.first == _T("shikakuman"))
                {
                    if (npc.second.GetDead())
                    {
                        continue;
                    }

                    if (npc.second.GetCarbo() <= 5.f ||
                        npc.second.GetProtein() <= 5.f ||
                        npc.second.GetLipid() <= 5.f ||
                        npc.second.GetVitamin() <= 5.f ||
                        npc.second.GetMineral() <= 5.f ||
                        npc.second.GetWater() <= 92.f)
                    {
                        // ワードブレスがあれば消費
                        auto storageManager = StorehouseManager::Get();
                        auto storage = storageManager->GetCurrentActiveStorehouse();

                        auto subidlist = storage->GetSubIdList(L"wordbress");

                        if (!subidlist.empty())
                        {
                            storage->RemoveItem(L"wordbress", subidlist.at(0));

                            npc.second.SetRynenContract();
                            npc.second.SetDrinkWordbress(true);
                        }
                    }
                }
            }
        }
    }

    // 死亡判定
    {
        for (auto& npc : m_NpcStatusMap)
        {
            if (npc.first == _T("daikeiman") ||
                npc.first == _T("sankakuman") ||
                npc.first == _T("shikakuman"))
            {
                if (npc.second.GetDead())
                {
                    continue;
                }

                // 水分は90％、それ以外は0％になったら死亡。
                // 仮実装
                if (npc.second.GetCarbo() <= 0.f ||
                    npc.second.GetProtein() <= 0.f ||
                    npc.second.GetLipid() <= 0.f ||
                    npc.second.GetVitamin() <= 0.f ||
                    npc.second.GetMineral() <= 0.f ||
                    npc.second.GetWater() <= 90.f)
                {
                    if (npc.second.GetDrinkWordbress())
                    {
                        // 体力が全快する
                        npc.second.SetDrinkWordbress(false);
                        npc.second.SetCarbo(100.f);
                        npc.second.SetProtein(100.f);
                        npc.second.SetLipid(100.f);
                        npc.second.SetMineral(100.f);
                        npc.second.SetVitamin(100.f);
                        npc.second.SetWater(100.f);
                    }
                    else
                    {
                        npc.second.SetDead();
                    }
                }
            }
        }
    }

    // ステータスがマイナスや１００以上にならないように丸め処理をする
    Clamp();
}

void NSModel::NpcStatusManager::AdvanceTime(const int hour, const int minute)
{
    // 糖質、脂質、タンパク質、ミネラル、ビタミンはどれも20日で0になることとする。
    // １秒での消費量は100/20/24/60/60
    // 水分は90で死亡なので、5日で10減るようにする
    float work_f = 100.f;
    work_f /= 20;
    work_f /= 24;
    work_f /= 60;
    work_f /= 60;

    int second = 0;
    second = minute * 60;
    second += hour * 60 * 60;

    work_f *= second;

    // 担架モードの時は逆に回復する。救済処置
    if (StatusManager::GetObj()->IsStretcherMode())
    {
        work_f *= -1;
    }

    for (auto& npc : m_NpcStatusMap)
    {
        // 体力を消費するのは3人だけ
        if (npc.first == _T("daikeiman") || npc.first == _T("sankakuman") || npc.first == _T("shikakuman"))
        {
            if (npc.second.GetDead())
            {
                continue;
            }

            float work_f2 = 0.f;
            work_f2 = npc.second.GetCarbo();
            npc.second.SetCarbo(work_f2 - work_f);

            work_f2 = npc.second.GetProtein();
            npc.second.SetProtein(work_f2 - work_f);

            work_f2 = npc.second.GetLipid();
            npc.second.SetLipid(work_f2 - (work_f));

            work_f2 = npc.second.GetMineral();
            npc.second.SetMineral(work_f2 - work_f);

            work_f2 = npc.second.GetVitamin();
            npc.second.SetVitamin(work_f2 - work_f);

            // 水分は減少量を0.5倍にする。
            work_f2 = npc.second.GetWater();
            npc.second.SetWater(work_f2 - (work_f * 0.5f));
        }
    }

    Clamp();
}

NpcStatus NpcStatusManager::GetNpcStatus(const std::wstring& name)
{
    return m_NpcStatusMap.at(name);
}

void NpcStatusManager::SetNpcStatus(const std::wstring& key, const NpcStatus& value)
{
    m_NpcStatusMap.at(key) = value;
}

std::vector<std::wstring> NSModel::NpcStatusManager::GetNameList()
{
    std::vector<std::wstring> vs;
    for (auto it = m_NpcStatusMap.begin(); it != m_NpcStatusMap.end(); ++it)
    {
        vs.push_back(it->first);
    }
    return vs;
}

bool NSModel::NpcStatusManager::OneWeekAfterRedman()
{
    bool result = false;

    auto datetime = PowereggDateTime::GetObj();

    int year = 0;
    int month = 0;
    int day = 0;
    m_NpcStatusMap.at(_T("shikakuman")).GetRedManDay(&year, &month, &day);

    // レッドマンになってからの1週間後とは何月何日であるか。
    int limitYear = 0;
    int limitMonth = 0;
    int limitDay = 0;

    limitDay = day + 7;
    limitMonth = month;
    limitYear = year;

    int dayOfMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (limitDay > dayOfMonth[limitMonth])
    {
        limitDay = limitDay - dayOfMonth[limitMonth];

        limitMonth++;
        if (limitMonth == 13)
        {
            limitMonth = 1;
            limitYear++;
        }
    }

    if (datetime->GetYear() > limitYear)
    {
        result = true;
    }
    else if (datetime->GetYear() < limitYear)
    {
        result = false;
    }
    else
    {
        if (datetime->GetMonth() > limitMonth)
        {
            result = true;
        }
        else if (datetime->GetMonth() < limitMonth)
        {
            result = false;
        }
        else
        {
            if (datetime->GetDay() > limitDay)
            {
                result = true;
            }
            else if (datetime->GetDay() < limitDay)
            {
                result = false;
            }
            else
            {
                result = false;
            }
        }
    }

    return false;
}

void NSModel::NpcStatusManager::Clamp()
{
    for (auto& npc : m_NpcStatusMap)
    {
        if (npc.first == _T("daikeiman") ||
            npc.first == _T("sankakuman") ||
            npc.first == _T("shikakuman"))
        {
            float work_f = 0.f;

            work_f = npc.second.GetCarbo();
            if (work_f < 0.f)
            {
                npc.second.SetCarbo(0.f);
            }
            else if (work_f > 100.f)
            {
                npc.second.SetCarbo(100.f);
            }

            work_f = npc.second.GetProtein();
            if (work_f < 0.f)
            {
                npc.second.SetProtein(0.f);
            }
            else if (work_f > 100.f)
            {
                npc.second.SetProtein(100.f);
            }

            work_f = npc.second.GetLipid();
            if (work_f < 0.f)
            {
                npc.second.SetLipid(0.f);
            }
            else if (work_f > 100.f)
            {
                npc.second.SetLipid(100.f);
            }

            work_f = npc.second.GetVitamin();
            if (work_f < 0.f)
            {
                npc.second.SetVitamin(0.f);
            }
            else if (work_f > 100.f)
            {
                npc.second.SetVitamin(100.f);
            }

            work_f = npc.second.GetMineral();
            if (work_f < 0.f)
            {
                npc.second.SetMineral(0.f);
            }
            else if (work_f > 100.f)
            {
                npc.second.SetMineral(100.f);
            }

            work_f = npc.second.GetWater();
            if (work_f < 0.f)
            {
                npc.second.SetWater(0.f);
            }
            else if (work_f > 100.f)
            {
                npc.second.SetWater(100.f);
            }
        }
    }
}

void NSModel::NpcStatusManager::Eat(const std::wstring npcKey, const ItemDef& itemDef)
{
    auto& _status = m_NpcStatusMap.at(npcKey);
    if (itemDef.GetType() != ItemDef::ItemType::FOOD)
    {
        throw std::exception();
    }

    bool bPoison = false;

    int rnd = rand();

    // 赤い実
    if (itemDef.GetId() == L"sotetsu")
    {
        bPoison = true;
    }
    // 大きいどんぐり
    else if (itemDef.GetId() == L"donguri3")
    {
        bPoison = true;
    }
    // ニラ or スイセン
    else if (itemDef.GetId() == L"nira")
    {
        if (rnd % 2 == 0)
        {
            bPoison = true;
        }
        else
        {
            bPoison = false;
        }
    }
    // キノコ
    else if (itemDef.GetId() == L"kinoko")
    {
        if (rnd % 10 == 0)
        {
            bPoison = false;
        }
        else
        {
            bPoison = true;
        }
    }

    float work_f = 0.f;
    work_f += itemDef.GetCarbo();

    work_f = _status.GetCarbo();
    work_f += itemDef.GetCarbo();
    _status.SetCarbo(work_f);

    work_f = _status.GetProtein();
    work_f += itemDef.GetProtein();
    _status.SetProtein(work_f);

    work_f = _status.GetLipid();
    work_f += itemDef.GetLipid();
    _status.SetLipid(work_f);

    work_f = _status.GetVitamin();
    work_f += itemDef.GetVitamin();
    _status.SetVitamin(work_f);

    work_f = _status.GetMineral();
    work_f += itemDef.GetMineral();
    _status.SetMineral(work_f);

    work_f = _status.GetWater();
    work_f += itemDef.GetWater();
    _status.SetWater(work_f);

    if (bPoison)
    {
        work_f = _status.GetCarbo();
        work_f -= itemDef.GetBodyStaminaDebuff();
        _status.SetCarbo(work_f);

        work_f = _status.GetProtein();
        work_f -= itemDef.GetBodyStaminaDebuff();
        _status.SetProtein(work_f);

        work_f = _status.GetLipid();
        work_f -= itemDef.GetBodyStaminaDebuff();
        _status.SetLipid(work_f);

        work_f = _status.GetVitamin();
        work_f -= itemDef.GetBodyStaminaDebuff();
        _status.SetVitamin(work_f);

        work_f = _status.GetMineral();
        work_f -= itemDef.GetBodyStaminaDebuff();
        _status.SetMineral(work_f);

        work_f = _status.GetWater();
        work_f -= (itemDef.GetBodyStaminaDebuff() * 0.02f);
        _status.SetWater(work_f);
    }
}
