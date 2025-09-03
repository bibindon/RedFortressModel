#include "WeaponManager.h"
#include "ItemManager.h"

#include <iomanip>
#include "Inventory.h"
#include "Storehouse.h"
#include "Util.h"
#include "CraftInfoManager.h"

using namespace NSModel;

WeaponManager* WeaponManager::obj;

WeaponManager* WeaponManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new WeaponManager();
    }
    return obj;
}

void WeaponManager::Destroy()
{
    delete WeaponManager::obj;
    WeaponManager::obj = nullptr;
}

void WeaponManager::Init(const std::wstring& csvfilename,
                         const std::wstring& savefilename,
                         const bool decrypt)
{
    // ItemManagerクラスのInit関数が先に呼ばれている必要がある
    {
        if (ItemManager::GetObj()->Inited() == false)
        {
            throw std::exception();
        }
    }

    // 武器データを読む
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfilename, decrypt);

        // 先頭行は無視
        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            WeaponDef weaponDef;
            weaponDef.SetWeaponId(vvs.at(i).at(0));
            weaponDef.SetName(vvs.at(i).at(1));
            weaponDef.SetDetail(vvs.at(i).at(2));
            weaponDef.SetXfileName(vvs.at(i).at(3));
            weaponDef.SetImageName(vvs.at(i).at(4));
            weaponDef.SetWeight(std::stod(vvs.at(i).at(5).c_str()));
            weaponDef.SetVolume(std::stoi(vvs.at(i).at(6).c_str()));
            weaponDef.SetReinforceMax(std::stoi(vvs.at(i).at(7).c_str()));
            weaponDef.SetAttackRate(std::stod(vvs.at(i).at(8).c_str()));
            weaponDef.SetAttackRateUp(std::stod(vvs.at(i).at(9).c_str()));

            weaponDef.SetFlightDistance(std::stod(vvs.at(i).at(10).c_str()));
            weaponDef.SetFlightDistanceUp(std::stod(vvs.at(i).at(11).c_str()));
            weaponDef.SetStaminaDown(std::stod(vvs.at(i).at(12).c_str()));
            weaponDef.SetDurability(std::stoi(vvs.at(i).at(13).c_str()));
            weaponDef.SetDurabilityUp(std::stoi(vvs.at(i).at(14).c_str()));
            weaponDef.SetOwnDamage(std::stoi(vvs.at(i).at(15).c_str()));

            m_weaponDefMap[weaponDef.GetWeaponId()] = weaponDef;
        }
    }
    // 保存された武器の情報を読み込む
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(savefilename, decrypt);

        // 先頭行は無視
        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            std::wstring id = vvs.at(i).at(0);
            std::wstring isShow = vvs.at(i).at(1);

            if (m_weaponDefMap.find(id) == m_weaponDefMap.end())
            {
                throw std::exception();
            }

            if (isShow == _T("true"))
            {
                m_weaponDefMap[id].SetIsShow(true);
            }
            else
            {
                m_weaponDefMap[id].SetIsShow(false);
            }
        }
    }
    // m_weaponDefSubListを作る
    // 強化値ごとに武器情報をもつ配列
    for (auto it = m_weaponDefMap.begin(); it != m_weaponDefMap.end(); ++it)
    {
        WeaponDef weaponDef = it->second;

        // 強化値の数だけループする
        //
        // 最大強化値が-1の時は強化無し。
        // つまりWeaponDefSubは1つ必要になる
        // 
        // 最大強化値が1のときは1度は強化できる。
        // つまりWeaponDefSubは2つ必要になる
        int loopNum = 0;
        if (weaponDef.GetReinforceMax() == -1)
        {
            loopNum = 1;
        }
        else
        {
            loopNum = weaponDef.GetReinforceMax()+1;
        }

        for (int i = 0; i < loopNum; ++i)
        {
            // 強化値は0,1,2,3,4というようにはならず
            // -1,1,2,3,4,5という感じになる
            int level = 0;
            if (i == 0)
            {
                level = -1;
            }
            else
            {
                level = i;
            }

            WeaponDefSub weaponDefSub;
            weaponDefSub.SetWeaponId(weaponDef.GetWeaponId());

            ItemManager* itemManager = ItemManager::GetObj();
            ItemDef itemDef = itemManager->GetItemDefByWeaponId(weaponDef.GetWeaponId(), level);

            weaponDefSub.SetItemId(itemDef.GetId());

            weaponDefSub.SetReinforce(level);

            double temp = 0.0;
            int temp_i = 0;

            // 攻撃力補正値
            temp = weaponDef.GetAttackRate() + (weaponDef.GetAttackRateUp() * level);
            weaponDefSub.SetAttackRate(temp);

            // 飛距離
            temp = weaponDef.GetFlightDistance() + (weaponDef.GetFlightDistanceUp() * level);
            weaponDefSub.SetFlightDistance(temp);

            // 耐久力
            temp_i = weaponDef.GetDurability() + (weaponDef.GetDurabilityUp() * level);
            weaponDefSub.SetDurabilityMax(temp_i);

            m_weaponDefSubList.push_back(weaponDefSub);
        }
    }
}


void WeaponManager::Save(const std::wstring& savefilename,
                         const bool encrypt)
{
    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;
        vs.push_back(_T("ID"));
        vs.push_back(_T("表示・非表示"));
        vvs.push_back(vs);
        vs.clear();
        auto itBegin = m_weaponDefMap.begin();
        auto itEnd = m_weaponDefMap.end();
        for (;itBegin != itEnd; ++itBegin)
        {
            vs.push_back(itBegin->second.GetWeaponId());
            if (itBegin->second.GetIsShow())
            {
                vs.push_back(_T("true"));
            }
            else
            {
                vs.push_back(_T("false"));
            }
            vvs.push_back(vs);
            vs.clear();
        }

        Util::WriteToCsv(savefilename, vvs, encrypt);
    }
}

std::wstring WeaponManager::GetDetail2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });

    return it->second.GetDetail();
}

std::wstring WeaponManager::GetXfilename2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetXfileName();
}

std::wstring WeaponManager::GetImageName2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetImageName();
}

double WeaponManager::GetWeight2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetWeight();
}

int WeaponManager::GetVolume2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetVolume();
}

int WeaponManager::GetReinforceMax2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetReinforceMax();
}

int WeaponManager::GetOwnDamage2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetOwnDamage();
}

bool WeaponManager::GetIsShow2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetIsShow();
}

double WeaponManager::GetAttackRate2(const std::wstring& weaponId, const int level) const
{
    auto it = std::find_if(m_weaponDefSubList.begin(), m_weaponDefSubList.end(),
                           [&](const WeaponDefSub& x)
                           {
                               return x.GetWeaponId() == weaponId && x.GetReinforce() == level;
                           });

    if (it == m_weaponDefSubList.end())
    {
        throw std::exception();
    }

    return it->GetAttackRate();
}

double WeaponManager::GetFlightDistance2(const std::wstring& weaponId, const int level) const
{
    auto it = std::find_if(m_weaponDefSubList.begin(), m_weaponDefSubList.end(),
                           [&](const WeaponDefSub& x)
                           {
                               return x.GetWeaponId() == weaponId && x.GetReinforce() == level;
                           });

    return it->GetFlightDistance();
}

double WeaponManager::GetStaminaDown2(const std::wstring& weaponId) const
{
    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
                           [&](const auto x)
                           {
                               return x.second.GetWeaponId() == weaponId;
                           });
    return it->second.GetStaminaDown();
}

int WeaponManager::GetDurabilityMax2(const std::wstring& weaponId, const int level) const
{
    auto it = std::find_if(m_weaponDefSubList.begin(), m_weaponDefSubList.end(),
                           [&](const WeaponDefSub& x)
                           {
                               return x.GetWeaponId() == weaponId && x.GetReinforce() == level;
                           });

    return it->GetDurabilityMax();
}

std::vector<std::wstring> NSModel::WeaponManager::GetWeaponNameList()
{
    std::vector<std::wstring> result;
    for (auto it = m_weaponDefMap.begin(); it != m_weaponDefMap.end(); ++it)
    {
        result.push_back(it->first);
    }

    return result;
}

bool NSModel::WeaponManager::IsTorchLit()
{
    return m_torchLit;
}

void NSModel::WeaponManager::SetTorchLit(const bool lit)
{
    m_torchLit = lit;
}
//
//std::wstring NSModel::WeaponManager::GetItemName(const std::wstring& weaponName) const
//{
//    auto it = std::find_if(m_weaponDefMap.begin(), m_weaponDefMap.end(),
//                           [&](const auto x)
//                           {
//                               return x.second.GetName() == weaponName;
//                           });
//
//    return it->second.GetWeaponId();
//}

std::wstring WeaponDef::GetWeaponId() const
{
    return m_weaponId;
}

void WeaponDef::SetWeaponId(const std::wstring& id)
{
    m_weaponId = id;
}

std::wstring WeaponDef::GetName() const
{
    return m_name;
}

void WeaponDef::SetName(const std::wstring& name)
{
    m_name = name;
}

std::wstring WeaponDef::GetDetail() const
{
    return m_detail;
}

void WeaponDef::SetDetail(const std::wstring& detail)
{
    m_detail = detail;
}

std::wstring WeaponDef::GetXfileName() const
{
    return m_xfileName;
}

void WeaponDef::SetXfileName(const std::wstring& xfileName)
{
    m_xfileName = xfileName;
}

std::wstring WeaponDef::GetImageName() const
{
    return m_imageName;
}

void WeaponDef::SetImageName(const std::wstring& imageName)
{
    m_imageName = imageName;
}

double WeaponDef::GetWeight() const
{
    return m_weight;
}

void WeaponDef::SetWeight(double weight)
{
    m_weight = weight;
}

int WeaponDef::GetVolume() const
{
    return m_volume;
}

void WeaponDef::SetVolume(int volume)
{
    m_volume = volume;
}

double WeaponDef::GetAttackRate() const
{
    return m_attackRate;
}

void WeaponDef::SetAttackRate(double attackRate)
{
    m_attackRate = attackRate;
}

double WeaponDef::GetAttackRateUp() const
{
    return m_attackRateUp;
}

void WeaponDef::SetAttackRateUp(double attackRateUp)
{
    m_attackRateUp = attackRateUp;
}

int WeaponDef::GetReinforceMax() const
{
    return m_reinforceMax;
}

void WeaponDef::SetReinforceMax(int reinforceMax)
{
    m_reinforceMax = reinforceMax;
}

double WeaponDef::GetFlightDistance() const
{
    return m_flightDistance;
}

void WeaponDef::SetFlightDistance(double flightDistance)
{
    m_flightDistance = flightDistance;
}

double WeaponDef::GetFlightDistanceUp() const
{
    return m_flightDistanceUp;
}

void WeaponDef::SetFlightDistanceUp(double flightDistanceUp)
{
    m_flightDistanceUp = flightDistanceUp;
}

double WeaponDef::GetStaminaDown() const
{
    return m_staminaDown;
}

void WeaponDef::SetStaminaDown(double staminaDown)
{
    m_staminaDown = staminaDown;
}

int WeaponDef::GetDurability() const
{
    return m_durability;
}

void WeaponDef::SetDurability(int durability)
{
    m_durability = durability;
}

int WeaponDef::GetDurabilityUp() const
{
    return m_durabilityUp;
}

void WeaponDef::SetDurabilityUp(int durabilityUp)
{
    m_durabilityUp = durabilityUp;
}

int WeaponDef::GetOwnDamage() const
{
    return m_ownDamage;
}

void WeaponDef::SetOwnDamage(int ownDamage)
{
    m_ownDamage = ownDamage;
}

bool WeaponDef::GetIsShow() const
{
    return m_isShow;
}

void WeaponDef::SetIsShow(bool isShow)
{
    m_isShow = isShow;
}

std::wstring WeaponDefSub::GetWeaponId() const
{
    return m_weaponId;
}

void WeaponDefSub::SetWeaponId(const std::wstring& id)
{
    m_weaponId = id;
}

void NSModel::CraftOutput::SetItemId(const std::wstring& id)
{
    m_id = id;
}

std::wstring NSModel::CraftOutput::GetItemId() const
{
    return m_id;
}

std::wstring WeaponDefSub::GetItemId() const
{
    return m_itemId;
}

void WeaponDefSub::SetItemId(const std::wstring& id)
{
    m_itemId = id;
}

int WeaponDefSub::GetReinforce() const
{
    return m_reinforce;
}

void WeaponDefSub::SetReinforce(int reinforce)
{
    m_reinforce = reinforce;
}

double WeaponDefSub::GetAttackRate() const
{
    return m_attackRate;
}

void WeaponDefSub::SetAttackRate(double attackRate)
{
    m_attackRate = attackRate;
}

double WeaponDefSub::GetFlightDistance() const
{
    return m_flightDistance;
}

void WeaponDefSub::SetFlightDistance(double flightDistance)
{
    m_flightDistance = flightDistance;
}

int WeaponDefSub::GetDurabilityMax() const
{
    return m_durabilityMax;
}

void WeaponDefSub::SetDurabilityMax(int durabilityMax)
{
    m_durabilityMax = durabilityMax;
}

