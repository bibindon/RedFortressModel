#pragma once

#include <string>
#include "ItemManager.h"

// 一般的に、「攻撃を受けたとき、最終的に負うダメージがいくつになるか」は
// それなりに複雑なコードになる。
// ゲーム本体にその複雑さを持ち込まなくてよいようにしたい。

// 武器を装備していない場合、アイテムIDに-1が入っている。

namespace NSStarmanLib
{
enum class eMagicType
{
    None,
    Fire,
    Ice,
    Dark,
};

enum class eBagPos
{
    None,
    Back1,
    Back2,
    Front,
    Left,
    Right,
};

// PlayerのステータスとNPCのステータスは別のものとして用意する。
// ここで扱うステータスはプレイヤーのStatus。

class Status
{
public:

    float GetBodyStaminaCurrent() const;
    void SetBodyStaminaCurrent(float mbodyStaminaCurrent);

    float GetBodyStaminaMax() const;
    void SetBodyStaminaMax(float mbodyStaminaMax);

    float GetBodyStaminaMaxSub() const;
    void SetBodyStaminaMaxSub(float mbodyStaminaMaxSub);

    float GetBrainStaminaCurrent() const;
    void SetBrainStaminaCurrent(float mbrainStaminaCurrent);

    float GetBrainStaminaMax() const;
    void SetBrainStaminaMax(float mbrainStaminaMax);

    float GetBrainStaminaMaxSub() const;
    void SetBrainStaminaMaxSub(float mbrainStaminaMaxSub);

    float GetExplosivePower() const;
    void SetExplosivePower(float mexplosivePower);

    float GetMuscleCurrent() const;
    void SetMuscleCurrent(float mmuscleCurrent);

    float GetMuscleMax() const;
    void SetMuscleMax(float mmuscleMax);

    float GetCarboCurrent() const;
    void SetCarboCurrent(float mcarbo);

    float GetCarboMax() const;
    void SetCarboMax(float mcarbo);

    float GetProteinCurrent() const;
    void SetProteinCurrent(float mprotein);

    float GetProteinMax() const;
    void SetProteinMax(float mprotein);

    float GetLipidCurrent() const;
    void SetLipidCurrent(float mlipid);

    float GetLipidMax() const;
    void SetLipidMax(float mlipid);

    float GetVitaminCurrent() const;
    void SetVitaminCurrent(float mvitamin);

    float GetVitaminMax() const;
    void SetVitaminMax(float mvitamin);

    float GetMineralCurrent() const;
    void SetMineralCurrent(float mmineral);

    float GetMineralMax() const;
    void SetMineralMax(float mmineral);

    float GetWaterCurrent() const;
    void SetWaterCurrent(float mwater);

    float GetWaterMax() const;
    void SetWaterMax(float mwater);

    bool GetFractureArm() const;
    void SetFractureArm(bool mfractureArm);

    bool GetFractureLeg() const;
    void SetFractureLeg(bool mfractureLeg);

    bool GetHeadache() const;
    void SetHeadache(bool mheadache);

    bool GetCold() const;
    void SetCold(bool mcold);

    bool GetStomachache() const;
    void SetStomachache(bool mstomachache);

    bool GetSleep() const;
    void SetSleep(bool msleep);

    bool GetDehydration() const;
    void SetDehydration(bool mdehydration);

    bool GetLackOfSleep() const;
    void SetLackOfSleep(bool mlackOfSleep);

    bool GetDead() const;
    void SetDead(const bool arg);

    float GetSatiety() const;
    void SetSatiety(const float arg);

    float GetX() const;
    void SetX(const float arg);

    float GetY() const;
    void SetY(const float arg);

    float GetZ() const;
    void SetZ(const float arg);

private:

    // 体のスタミナ（現在値）
    float m_bodyStaminaCurrent { 0.f };

    // 体のスタミナ（最大値）
    float m_bodyStaminaMax { 0.f };

    // 体のスタミナ（回復可能値）
    // 現実世界では走った後、座るとある程度体力を回復できるが
    // どれだけ座り続けても全快するわけではない。
    // このときの、回復できる最大を表す値。
    float m_bodyStaminaMaxSub { 0.f };

    // 脳のスタミナ（現在値）
    float m_brainStaminaCurrent { 0.f };

    // 脳のスタミナ（最大値）
    float m_brainStaminaMax { 0.f };

    // 脳のスタミナ（回復可能値）
    // 現実世界では頭を使った後、ボーっとすればある程度、脳の体力を回復できるが
    // どれだけ休み続けても全快するわけではない。
    // このときの、回復できる最大を表す値。
    float m_brainStaminaMaxSub { 0.f };

    // 瞬発力
    // 速筋の太さ
    float m_explosivePower { 0.f };

    // 肉体の修復度（現在値）
    // 損傷・疲労によって低下する
    // 数値が大きいほど、損傷・疲労していない
    float m_muscleCurrent { 0.f };

    // 肉体の修復度（最大値）
    // 損傷・疲労によって低下する。
    float m_muscleMax { 0.f };

    // 糖質（現在値）
    float m_carboCurrent { 0.f };

    // 糖質（最大値）
    float m_carboMax { 0.f };

    // タンパク質（現在値）
    float m_proteinCurrent { 0.f };

    // タンパク質（最大値）
    float m_proteinMax { 0.f };

    // 脂質（現在値）
    float m_lipidCurrent { 0.f };

    // 脂質（最大値）
    float m_lipidMax { 0.f };

    // ビタミン（現在値）
    float m_vitaminCurrent { 0.f };

    // ビタミン（最大値）
    float m_vitaminMax { 0.f };

    // ミネラル（現在値）
    float m_mineralCurrent { 0.f };

    // ミネラル（最大値）
    float m_mineralMax { 0.f };

    // 水分（現在値）
    float m_waterCurrent { 0.f };

    // 水分（最大値）
    float m_waterMax { 0.f };

    // 腕骨折
    bool m_fractureArm { false };

    // 足骨折
    bool m_fractureLeg { false };

    // 頭痛
    bool m_headache { false };

    // 風邪
    bool m_cold { false };

    // 腹痛
    bool m_stomachache { false };

    // 睡眠
    bool m_sleep { false };

    // 脱水症状
    bool m_dehydration { false };

    // 睡眠不足
    bool m_lackOfSleep { false };

    // 死亡
    bool m_dead = false;

    // 満腹度
    float m_satiety = 0.f;

    float m_x = 0.f;
    float m_y = 0.f;
    float m_z = 0.f;

};

enum class eDeadReason
{
    NONE,

    // 肉体の修復度が0になった。全身打撲的な
    MUSCLE_ZERO,

    // 体内の水分が90％以下になった
    WATER_90,

    // 糖質と脂質がゼロになった。餓死
    STARVATION,

    // 体のスタミナがゼロになった。
    KAROSHI,

    // ライネンの契約で1年経過した。
    RYNEN_1_YEAR,

    // 溺死。水中で睡眠した。
    DROWNING,

    // 睡眠中に攻撃された
    ATTACK_ON_SLEEP,
};

class StatusManager
{
public:

    enum class PlayerState
    {
        // 立ち状態と歩き状態は同じくらい疲れるべき
        // 立ち状態
        STAND,

        // 歩き状態
        WALK,

        // 座り状態
        SIT,

        // 寝そべり
        LYING_DOWN,

        // ジョギング
        JOGGING,

        // 全力疾走
        SPRINTING,

        // 水中で静止
        IDLE_WATER,

        // 泳ぎ状態
        SWIM,
    };

    static StatusManager* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfile,
              const bool decrypt = false);

    // 1秒に一度呼ばれる想定
    void Update();

    void Save(const std::wstring& csvfile,
              const float player_x,
              const float player_y,
              const float player_z,
              const bool encrypt = false);

    // 歩行速度。重量や健康度、筋力などありとあらゆるパラメーターが影響する
    float GetWalkSpeed();

    // 攻撃力。重量や健康度、筋力などありとあらゆるパラメーターが影響する
    float GetAttackPower();

    // 攻撃したときにスタミナや武器の耐久度などを減らす
    void ConsumeAttackCost();

    // 防御力。重量や健康度、筋力などありとあらゆるパラメーターが影響する
    float GetDefensePower();

    bool IsUnderwater() const;

    // 食材を食べる
    bool Eat(const ItemDef& food);

    // 寝る
    bool Sleep();

    // 3時間休憩
    bool Rest3Hours();

    // 会話する
    // 会話したら脳の体力が減る。
    void Talk();

    // 魔法を使う
    // 魔法を使ったら脳の体力が減る
    void UseMagic();

    void DrinkWordBress(const float playerX,
                        const float playerY,
                        const float playerZ);

    // 木の伐採
    void CutTree(const std::wstring& itemId, const int level);

    // 植物採取
    void PickPlant();

    // オールを漕ぐ
    void PullOar();

    // 左右のオールを漕ぐ
    void PullOarBoth();

    // 3時間、自動航海を行う
    void Voyage3Hours();

    //-------------------------------------------------------
    // Statusクラスの同名の関数を呼ぶ
    //-------------------------------------------------------

    float GetBodyStaminaCurrent() const;
    void SetBodyStaminaCurrent(float mbodyStaminaCurrent);

    float GetBodyStaminaMax() const;
    void SetBodyStaminaMax(float mbodyStaminaMax);

    float GetBodyStaminaMaxSub() const;
    void SetBodyStaminaMaxSub(float mbodyStaminaMaxSub);

    float GetBrainStaminaCurrent() const;
    void SetBrainStaminaCurrent(float mbrainStaminaCurrent);

    float GetBrainStaminaMax() const;
    void SetBrainStaminaMax(float mbrainStaminaMax);

    float GetBrainStaminaMaxSub() const;
    void SetBrainStaminaMaxSub(float mbrainStaminaMaxSub);

    float GetExplosivePower() const;
    void SetExplosivePower(float mexplosivePower);

    float GetMuscleCurrent() const;
    void SetMuscleCurrent(float mmuscleCurrent);

    float GetMuscleMax() const;
    void SetMuscleMax(float mmuscleMax);

    float GetCarboCurrent() const;
    void SetCarboCurrent(float mcarbo);

    float GetCarboMax() const;
    void SetCarboMax(float mcarbo);

    float GetProteinCurrent() const;
    void SetProteinCurrent(float mprotein);

    float GetProteinMax() const;
    void SetProteinMax(float mprotein);

    float GetLipidCurrent() const;
    void SetLipidCurrent(float mlipid);

    float GetLipidMax() const;
    void SetLipidMax(float mlipid);

    float GetVitaminCurrent() const;
    void SetVitaminCurrent(float mvitamin);

    float GetVitaminMax() const;
    void SetVitaminMax(float mvitamin);

    float GetMineralCurrent() const;
    void SetMineralCurrent(float mmineral);

    float GetMineralMax() const;
    void SetMineralMax(float mmineral);

    float GetWaterCurrent() const;
    void SetWaterCurrent(float mwater);

    float GetWaterMax() const;
    void SetWaterMax(float mwater);

    bool GetFractureArm() const;
    void SetFractureArm(bool mfractureArm);

    int GetFractureArmCureRemain() const;

    bool GetFractureLeg() const;
    void SetFractureLeg(bool mfractureLeg);

    int GetFractureLegCureRemain() const;

    bool GetHeadache() const;
    void SetHeadache(bool mheadache);

    int GetHeadacheCureRemain() const;

    bool GetCold() const;
    void SetCold(bool mcold);

    int GetColdCureRemain() const;

    bool GetStomachache() const;
    void SetStomachache(bool mstomachache);

    int GetStomachacheCureRemain() const;

    bool GetSleep() const;
    void SetSleep(bool msleep);

    bool GetDehydration() const;
    void SetDehydration(bool mdehydration);

    int GetDehydrationCureRemain() const;

    bool GetLackOfSleep() const;
    void SetLackOfSleep(bool mlackOfSleep);

    void SetPlayerAction(const PlayerState arg);
    PlayerState GetPlayerAction() const;

    bool GetDead() const;
    void SetDead(const bool arg);

    // 装備武器
    ItemInfo GetEquipWeapon() const;
    void SetEquipWeapon(const ItemInfo& arg);

    // 袋を装備。戻り値としてどの個所に装着されたかが返される。
    eBagPos EquipBag(const ItemInfo& bag);

    // 袋を外す。
    void UnequipBag(const eBagPos bagPos);
    void UnequipBag(const std::wstring& id, const int subId);

    // 袋を取得
    ItemInfo GetBag(const eBagPos bagPos) const;
    eBagPos GetBag(const std::wstring& id, const int subId) const;
    std::vector<ItemInfo> GetAllBag();

    // 袋の装着状況
    std::vector<eBagPos> GetBagState();

    void UpdateBagDurability();

    // 強い精神ストレス
    void SetSuperStress();

    void GetXYZ(float* x, float* y, float* z);

    void SetMagicType(const eMagicType magicType);
    eMagicType GetMagicType() const;

    bool GetLevelUpFire();
    bool GetLevelUpIce();
    bool GetLevelUpDark();

    bool GetLevelDownFire();
    bool GetLevelDownIce();
    bool GetLevelDownDark();

    int GetLevelFire() const;
    int GetLevelIce() const;
    int GetLevelDark() const;

    void SetDeadReason(const eDeadReason reason);
    eDeadReason GetDeadReason() const;

    void ConsumeJumpCost();

    void SetSugegasa(bool isEquip);
    bool GetSugegasa() const;

    void SetInCave(bool arg) { m_bInCave = arg; }
    bool GetInCave() const { return m_bInCave; }

    bool IsStretcherMode() const;
    void SetStretcherMode(const bool arg);

private:

    void Clamp();

    eDeadReason m_eDeadReason = eDeadReason::NONE;

    // シングルトンオブジェクト
    static StatusManager* obj;

    Status m_status;

    PlayerState m_playerState { PlayerState::STAND };

    // 体のスタミナが70％以下になったことが一日に1度でもあった
    bool m_training70 = false;

    // 体のスタミナが30％以下になったことが一日に1度でもあった
    bool m_training30 = false;

    // 前回日時をチェックした時の日
    int m_previousDay = 0;

    // 前回日時をチェックした時の時刻
    int m_previousHour = 0;

    // 朝8時をまたいだ
    bool m_over8clock = false;

    // 装備武器
    ItemInfo m_EquipWeapon;

    // 袋
    std::unordered_map<eBagPos, ItemInfo> m_BagMap;

    // 風邪の完治にかかる秒数
    // 初期値は5日。身体を動かすと悪化する
    int m_remainColdCure = 0;

    // 腕骨折の完治にかかる秒数
    // 初期値は90日。五大栄養素が半分以下の時は回復が止まる
    int m_remainArmFracCure = 0;

    // 足骨折の完治にかかる秒数
    // 初期値は90日。五大栄養素が半分以下の時は回復が止まる
    int m_remainLegFracCure = 0;

    // 頭痛の完治にかかる秒数
    // 1日（＝24時間）。水を飲むと悪化する
    int m_remainHeadacheCure = 0;

    // 腹痛の完治にかかる秒数
    // 1日（＝24時間）。食べると悪化する
    int m_remainStomachacheCure = 0;

    // 脱水症状の完治にかかる秒数
    // 1日（＝24時間）。五大栄養素・水が不足すると回復が止まる
    int m_remainDehydration = 0;

    // 装備中の魔法
    eMagicType m_eMagicType = eMagicType::None;

    // 魔法の熟練度
    // ・0~10までの11段階
    // ・100回使ったら魔法のレベルが上がる。
    // ・魔法のレベルは100回使った直後ではなく100回使った後に寝るとレベルアップする
    // ・経験値は毎日-10される。つまり、10日魔法を使わなかったらレベルが下がるということ。
    int m_levelFire = 0;
    int m_levelIce = 0;
    int m_levelDark = 0;
    int m_experienceFire = 0;
    int m_experienceIce = 0;
    int m_experienceDark = 0;

    bool m_levelUpFire = false;
    bool m_levelUpIce = false;
    bool m_levelUpDark = false;

    bool m_levelDownFire = false;
    bool m_levelDownIce = false;
    bool m_levelDownDark = false;

    bool m_bEquipSugegasa = false;

    bool m_bInCave = false;

    // 担架モード
    bool m_bStretcher = false;
};
}

