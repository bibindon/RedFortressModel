#pragma once
#include <string>

namespace NSStarmanLib
{
// ライネンという契約を管理するクラス
// 悪魔と契約すると魔法が使えるようになり、ワードブレスという小瓶で死んでも復活できるようになる。
// ただし、1年後に死ぬ。
// 
// 復活システム
// ワードブレスという小瓶を飲んだら1度だけ生き返られる。
// ・生き返る場所は小瓶を飲んだ場所
// ・何本飲んでも生き返られるのは1度だけ
// ・生き返った後にもう一度飲めばもう一度生き返られる
// ・体力は全快する
// ・小瓶を飲んでいない状態で死んだらゲームはオープニングからスタートになる
class Rynen
{
public:

    static Rynen* GetObj();
    static void Destroy();
    void Init(const std::wstring& csvfile,
              const bool decrypt = false);
    void Save(const std::wstring& csvfile,
              const bool encrypt = false);

    void SetContracted(const bool arg);
    bool GetContracted();

    void SetReviveEnable(const bool arg);
    bool GetReviveEnable();

    void SetRevivePos(const float x, const float y, const float z);
    void GetRevivePos(float* x, float* y, float* z);

    void SetContractDate(const int year, const int month, const int day);
    void GetContractDate(int* year, int* month, int* day);

private:

    // シングルトンオブジェクト
    static Rynen* obj;

    // 契約済みフラグ
    bool m_contracted { false };

    // 復活可能フラグ
    bool m_reviveEnable { false };

    // 復活場所
    float m_x { 0.f };
    float m_y { 0.f };
    float m_z { 0.f };

    // 契約した日
    int m_year { 0 };
    int m_month { 0 };
    int m_day { 0 };
};
}

