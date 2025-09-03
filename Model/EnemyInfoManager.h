#pragma once

#include <string>
#include <unordered_map>

namespace NSModel
{

// 敵の定義情報
// 敵一種類につき、1インスタンス
class EnemyDef
{
public:

    void SetID(const std::wstring& arg);
    std::wstring GetID() const;

    void SetName(const std::wstring& arg);
    std::wstring GetName() const;

    void SetDetail(const std::wstring& arg);
    std::wstring GetDetail() const;

    void SetImagePath(const std::wstring& arg);
    std::wstring GetImagePath();

    void SetVisible(const bool arg);
    bool GetVisible() const;

private:

    // 敵一種類ごとに割り振られるID
    std::wstring m_id;

    // 説明文
    std::wstring m_name;

    // 説明文
    std::wstring m_detail;

    // ファイルパス
    std::wstring m_imagePath;

    // メニューでの表示・非表示
    bool m_visible = false;
};

// 敵管理クラス。
// 敵1体につき、1インスタンス
// CSVファイルに記載された敵の座標やHPなどを提供するのみである。
// 敵の行動パターンだとか衝突判定、ジャンプの軌道など詳細なことはここではやらない。
// （ライブラリの使用者がゲーム内で独自の実装を行うべき）
//
// バイナリ形式で読み込みを行うため構造体を使用する。
struct stEnemyInfo
{
    // 敵一体ごとに割り振られる番号
    int m_SerialNumber = 0;

    // 敵一種類ごとに割り振られるID
    wchar_t m_id[64];

    float m_x = 0.f;
    float m_y = 0.f;
    float m_z = 0.f;

    float m_rotX = 0.f;
    float m_rotY = 0.f;
    float m_rotZ = 0.f;

    // 残りHP
    int m_HP = 0;
    bool m_bDefeated = false;

};

class EnemyInfoManager
{
public:

    static EnemyInfoManager* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvEnemyDef,
              const std::wstring& csvEnemyInfo,
              const std::wstring& csvEnemyVisible,
              const bool decrypt = false,
              const bool loadFromBinary = false);

    void Save(const std::wstring& csvEnemyInfo,
              const std::wstring& csvEnemyVisible,
              const bool encrypt = false,
              const bool saveWithBinary = false);

    // 引数で示す座標と半径の内側にいる敵を取得
    std::vector<stEnemyInfo> GetEnemyInfo(const float x, const float y, const float z, const float r);

    void UpdateEnemyInfo(const int serialNumber, const stEnemyInfo& enemyInfo);

    std::vector<std::wstring> GetEnemyIdList();

    EnemyDef GetEnemyDef(const std::wstring& id);
    void SetEnemyVisible(const std::wstring& id, const bool visible);

    stEnemyInfo GetEnemyInfo(const int serialNumber);
    void SetDefeat(const int serialNumber);

private:

    // シングルトンオブジェクト
    static EnemyInfoManager* obj;

    // キーは半角英数字のID
    std::unordered_map<std::wstring, EnemyDef> m_enemyDefMap;

    // キーはシリアルナンバー
    std::unordered_map<int, stEnemyInfo> m_enemyInfoMap;

};
}

