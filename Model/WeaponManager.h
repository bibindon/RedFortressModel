#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace NSModel
{

// weapon.csvの内容そのまま
class WeaponDef
{

public:

    // ここでのIDはweapon.csvでのID
    std::wstring GetWeaponId() const;
    void SetWeaponId(const std::wstring& id);

    std::wstring GetName() const;
    void SetName(const std::wstring& name);

    std::wstring GetDetail() const;
    void SetDetail(const std::wstring& detail);

    std::wstring GetXfileName() const;
    void SetXfileName(const std::wstring& xfileName);

    std::wstring GetImageName() const;
    void SetImageName(const std::wstring& imageName);

    double GetWeight() const;
    void SetWeight(double weight);

    int GetVolume() const;
    void SetVolume(int volume);

    double GetAttackRate() const;
    void SetAttackRate(double attackRate);

    double GetAttackRateUp() const;
    void SetAttackRateUp(double attackRateUp);

    int GetReinforceMax() const;
    void SetReinforceMax(int reinforceMax);

    double GetFlightDistance() const;
    void SetFlightDistance(double flightDistance);

    double GetFlightDistanceUp() const;
    void SetFlightDistanceUp(double flightDistanceUp);

    double GetStaminaDown() const;
    void SetStaminaDown(double staminaDown);

    int GetDurability() const;
    void SetDurability(int durability);

    int GetDurabilityUp() const;
    void SetDurabilityUp(int durabilityUp);

    int GetOwnDamage() const;
    void SetOwnDamage(int ownDamage);

    // 使われていない？
    bool GetIsShow() const;
    void SetIsShow(bool isShow);

private:
    std::wstring m_weaponId;
    std::wstring m_name;
    std::wstring m_detail;
    std::wstring m_xfileName;
    std::wstring m_imageName;
    double m_weight = 0.f; // 重量
    int m_volume = 0; // 体積
    double m_attackRate = 0.f; // 攻撃力補正
    double m_attackRateUp = 0.f; // 強化時の攻撃力補正の上昇値
    int m_reinforceMax = 0; // 強化可能回数
    double m_flightDistance = 0.f; // 飛距離
    double m_flightDistanceUp = 0.f; // 強化時の飛距離の上昇値
    double m_staminaDown = 0.f; // 消費スタミナ
    int m_durability = 0; // 耐久値（壊れるまでの回数）
    int m_durabilityUp = 0; // 強化時の耐久値の上昇値
    int m_ownDamage = 0; // 使用時の肉体損傷

    // 使われていない？
    bool m_isShow = false; // 表示
};

// WeaponDefの定義では「強化値＋３の石斧の攻撃力」を知りたいとき
// 計算しないと分からない。
// 
// WeaponDefSubでは強化値ごとの攻撃力や飛距離などをあらかじめ計算して保持しておく。
// 
// 最大耐久度は管理するが現在の耐久度はInventoryかStorehouseが管理しているので扱わない。
class WeaponDefSub
{
public:

    std::wstring GetWeaponId() const;
    void SetWeaponId(const std::wstring& id);

    std::wstring GetItemId() const;
    void SetItemId(const std::wstring& id);

    int GetReinforce() const;
    void SetReinforce(int reinforce);

    double GetAttackRate() const;
    void SetAttackRate(double attackRate);

    double GetFlightDistance() const;
    void SetFlightDistance(double flightDistance);

    int GetDurabilityMax() const;
    void SetDurabilityMax(int durabilityMax);

private:

    std::wstring m_weaponId; // 武器ID（weapon.csv）
    std::wstring m_itemId; // アイテムID（item.csv）

    int m_reinforce = 0;
    double m_attackRate = 0.f;
    double m_flightDistance = 0;
    int m_durabilityMax = 0;
};

class WeaponManager
{
public:
    static WeaponManager* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfilename,
              const std::wstring& savefilename,
              const bool decrypt = false);

    void Save(const std::wstring& savefilename,
              const bool encrypt = false);

    // 同じ名前を使うの禁止
//    std::wstring GetDetail(const std::wstring& weaponName) const;
//    std::wstring GetXfilename(const std::wstring& weaponName) const;
//    std::wstring GetImageName(const std::wstring& weaponName) const;
//    double GetWeight(const std::wstring& weaponName) const;
//    int GetVolume(const std::wstring& weaponName) const;
//    int GetReinforceMax(const std::wstring& weaponName) const;
//    double GetStaminaDown(const std::wstring& weaponName) const;
//    int GetOwnDamage(const std::wstring& weaponName) const;
//    bool GetIsShow(const std::wstring& weaponName) const;
//
//    double GetAttackRate(const std::wstring& weaponName, const int level) const;
//    double GetFlightDistance(const std::wstring& weaponName, const int level) const;
//    int GetDurabilityMax(const std::wstring& weaponName, const int level) const;

    std::wstring GetDetail2(const std::wstring& weaponId) const;
    std::wstring GetXfilename2(const std::wstring& weaponId) const;
    std::wstring GetImageName2(const std::wstring& weaponId) const;
    double GetWeight2(const std::wstring& weaponId) const;
    int GetVolume2(const std::wstring& weaponId) const;
    int GetReinforceMax2(const std::wstring& weaponId) const;
    double GetStaminaDown2(const std::wstring& weaponId) const;
    int GetOwnDamage2(const std::wstring& weaponId) const;
    bool GetIsShow2(const std::wstring& weaponId) const;

    double GetAttackRate2(const std::wstring& weaponId, const int level) const;
    double GetFlightDistance2(const std::wstring& weaponId, const int level) const;
    int GetDurabilityMax2(const std::wstring& weaponId, const int level) const;

    std::vector<std::wstring> GetWeaponNameList();

    // 松明に火がついているか否か。火がついている・いないはセーブしない。ゲームを再起動したら消えている。
    // 火が付いた松明は一つしか存在できないのでsubIdは管理不要
    bool IsTorchLit();
    void SetTorchLit(const bool lit);

private:

//    std::wstring GetItemName2(const std::wstring& weaponId) const;

    // シングルトンオブジェクト
    static WeaponManager* obj;

    std::unordered_map<std::wstring, WeaponDef> m_weaponDefMap;
    std::vector<WeaponDefSub> m_weaponDefSubList;

    bool m_torchLit = false;
};

}
