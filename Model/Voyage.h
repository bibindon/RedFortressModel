#pragma once
#include <vector>
#include <string>

namespace NSStarmanLib
{
// イカダ
class Raft
{
public:

    enum class ePosType
    {
        Sea,
        River,
    };

    void SetId(const int arg);
    int GetId() const;

    void SetSail(const bool arg);
    bool GetSail() const;

    void SetXYZ(const float x, const float y, const float z);
    void GetXYZ(float* x, float* y, float* z);

    void SetDurability(const int arg);
    int GetDurability() const;

    void SetLevel(const int arg);
    int GetLevel() const;

    void SetPosType(const ePosType arg);
    ePosType GetPosType() const;

    void SetStorehouseId(const int arg);
    int GetStorehouseId() const;

    float GetStorehouseWeight() const;

private:

    int m_id = -1;

    // 帆が展開されているか、畳まれているか。
    bool m_bSail = false;

    float m_x = 0.f;
    float m_y = 0.f;
    float m_z = 0.f;

    // 耐久度
    // イカダは倉庫にもインベントリにも入らないので
    // InventoryクラスやStorehouseクラスのことを考える必要はない。
    int m_durability = 0;

    // 強化値
    int m_level = 0;

    // 川か海か
    ePosType m_ePosType = ePosType::River;

    // 倉庫のID
    int m_storehouseId = -1;
};

//----------------------------------------------------
// 航海機能
//----------------------------------------------------
class Voyage
{
public:

    static Voyage* Get();
    static void Destroy();

    void Init(const std::wstring& csvRaft);
    void Save(const std::wstring& csvRaft);

    // 毎フレーム呼ばれる想定
    // 主人公のXYZ座標を引数として受け取る。
    void Update(const float x, const float y, const float z);

    void SetRaftMode(const bool arg);
    bool GetRaftMode() const;

    void AddRaft(const Raft& arg);
    std::vector<Raft> GetRaftList();

    Raft GetRaft(const int id);

    // 乗船中のイカダ
    Raft GetRaftCurrent();
    int GetRaftCurrentId() const;
    void SetRaftCurrentId(const int arg);

    // 右のオールを引く
    // 衝突判定はライブラリ内ではできないので
    // イカダを進める処理をライブラリ内で行うことはできない。
    void PullRightOar();

    // 左のオールを引く
    // 衝突判定はライブラリ内ではできないので
    // イカダを進める処理をライブラリ内で行うことはできない。
    void PullLeftOar();

    // 両方のオールを引く
    // 衝突判定はライブラリ内ではできないので
    // イカダを進める処理をライブラリ内で行うことはできない。
    void PullBothOar();

    // 衝突判定はライブラリ内ではできないので
    // イカダを進める処理をライブラリ内で行うことはできない。
    // この座標はセーブ・ロード時に使うものであって、
    // リアルタイムに使用するものではない。
    void SetCurrentRaftCoord(const float x, const float y, const float z);
    void GetCurrentRaftCoord(float* x, float* y, float* z);

    // 現在の方向に向かって3時間漕ぐ
    bool Set3HoursAuto();

    // 3時間の自動走行が可能か
    bool Enable3HoursAuto();

    // 現在乗っているイカダの帆の状態を取得
    void SetSailCurrentRaft(const bool arg);
    bool GetSailCurrentRaft() const;

    // 現在乗っているイカダがいるのは川か海か
    void SetPosTypeCurrentRaft(const Raft::ePosType arg);
    Raft::ePosType GetPosTypeCurrentRaft() const;

    // 風の向きと強さ（秒速）
    void GetWindXZ(float* x, float* z);

    // 潮の向きと強さ（秒速）
    void GetTideXZ(float* x, float* z);

    // イカダが2メートル以内にあるか？
    bool CheckNearRaft(const float x, const float y, const float z, int* id = nullptr);

    // イカダに乗ることが可能か？
    // 袋を装備していたらイカダに乗ることはできない。
    bool CheckRaftEnable();

    // イカダが壊れていないか。
    bool CheckRaftBroken();

    void CollideGround();

    // 風の向きと強さ、潮の向きをリセット
    // 3時間休憩や、3時間自動運転、睡眠をした時など
    void ResetWindAndTide();

private:

    // GetRaftCurrentと違い、こちらはポインタを使う。
    // 内部処理用
    Raft* GetRaftCurrentPriv() const;

    static Voyage* m_single;

    bool m_bRaftMode = false;

    int m_currentRaftId = -1;

    std::vector<Raft> m_raftList;

    // 風の向きと強さ（秒速）
    // 1時間おきに変わる
    // 自動走行中は変わらない
    float m_windX = 0.f;
    float m_windZ = 0.f;

    // 潮の向きと強さ（秒速）
    // 1時間おきに変わる
    // 自動走行中は変わらない
    float m_tideX = 0.f;
    float m_tideZ = 0.f;
};
}

