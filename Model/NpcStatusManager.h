#pragma once
#include <string>
#include <unordered_map>
#include "ItemManager.h"

namespace NSStarmanLib
{
// 糖質、タンパク質、脂質、ビタミン、ミネラルだけを状態に持つ
// プレイヤーは五大栄養素がすべて0にならないようにする必要がある。
// 糖質が0になったら餓死、それ以外が0になったら病死する。
// 倉庫にワードブレスがあればライネンの契約をし復活する。

// NPCはクラフト、パッチテスト、お手伝い機能を持つ。
// クラフトとパッチテストができるNPCがいる
enum class eNpcFeature
{
    CRAFTMAN,
    PATCH_TEST,
    CRAFTMAN_AND_PATCH_TEST,
    HELP,
    NONE,
};

class NpcStatus
{
public:

    void SetName(const std::wstring& arg);
    std::wstring GetName();

    void SetCarbo(const float arg);
    float GetCarbo() const;

    void SetProtein(const float arg);
    float GetProtein() const;

    void SetLipid(const float arg);
    float GetLipid() const;

    void SetVitamin(const float arg);
    float GetVitamin() const;

    void SetMineral(const float arg);
    float GetMineral() const;

    void SetWater(const float arg);
    float GetWater() const;

    void SetRynenContract();
    float GetRynenContract() const;

    void SetDrinkWordbress(const bool arg);
    bool GetDrinkWordbress() const;

    void SetDead();
    bool GetDead() const;

    void SetX(const float arg);
    float GetX() const;

    void SetY(const float arg);
    float GetY() const;

    void SetZ(const float arg);
    float GetZ() const;

    void SetRotY(const float arg);
    float GetRotY() const;

    void SetHasTalk(const bool arg);
    bool GetHasTalk() const;

    void SetTalkCsv(const std::wstring& arg);
    std::wstring GetTalkCsv() const;

    void SetFeatureEnable(const bool arg);
    bool GetFeatureEnable() const;

    void SetNpcFeature(const eNpcFeature arg);
    eNpcFeature GetNpcFeature() const;

    void SetRedMan(const bool arg);
    bool IsRedMan() const;

    void SetRedManDay(const int year, const int month, const int day);
    void GetRedManDay(int* year, int* month, int* day);

private:

    // 名前
    std::wstring m_name;

    // 糖質
    float m_carbo { 0.f };

    // タンパク質
    float m_protein { 0.f };

    // 脂質
    float m_lipid { 0.f };

    // ビタミン
    float m_vitamin { 0.f };

    // ミネラル
    float m_mineral { 0.f };

    // 水分
    float m_water { 0.f };

    // ライネンの契約をしているか否か
    bool m_rynenContract { false };

    // ワードブレスを飲んでいるか否か
    bool m_bDrinkWordbress { false };

    // 死んでいるか否か
    bool m_dead { false };

    float m_x = 0.f;

    float m_y = 0.f;

    float m_z = 0.f;

    float m_rotY = 0.f;

    // 会話すべき文言があるか？
    // ・NPCの会話は荷物番や鍛冶屋の機能を表示する前に表示する
    bool m_bHasTalk = false;

    // 会話すべき文言
    // m_talkCsvの内容を使って利用者に会話してもらう。
    std::wstring m_talkCsv;

    // 荷物番や鍛冶屋の機能が解禁されているか。
    bool m_bFeatureEnable = false;

    eNpcFeature m_eNpcFeature = eNpcFeature::NONE;

    // レッドマンになっているか。
    bool m_bRedman = false;

    // レッドマンになった日
    int m_redManYear = 0;
    int m_redManMonth = 0;
    int m_redManDay = 0;
};



class NpcStatusManager
{
public:

    static NpcStatusManager* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfile,
              const bool decrypt = false);

    void Save(const std::wstring& csvfile,
              const bool encrypt = false);

    void Update();

    // 時間を経過させる
    void AdvanceTime(const int hour, const int minute);

    NpcStatus GetNpcStatus(const std::wstring& name);
    void SetNpcStatus(const std::wstring& key, const NpcStatus& value);

    std::vector<std::wstring> GetNameList();

    // シカクマンがレッドマンになってから1週間が経過したか。
    bool OneWeekAfterRedman();

private:

    // シングルトンオブジェクト
    static NpcStatusManager* obj;

    std::unordered_map<std::wstring, NpcStatus> m_NpcStatusMap;

    void Clamp();

    void Eat(const std::wstring npcKey, const ItemDef& itemDef);
};
}

