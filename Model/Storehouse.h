#pragma once

#include "HeaderOnlyCsv.hpp"
#include "ItemManager.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace NSStarmanLib
{
// プレイヤーの所持品を管理するクラス。
// アイテムの定義はItemManagerが管理している。
// 武器についてはWeaponManagerが所持数まで管理しているので
// Storehouseクラスに出番はない予定

class Storehouse
{
public:

    void Init(const std::wstring& csvfile,
              const bool decrypt = false);

    void Save(const std::wstring& csvfile,
              const bool encrypt = false);

    // 一個ずつしか増加減出来ない想定
    void AddItem(const std::wstring& id, const int durability = -1);

    // SubIDを指定してアイテムを追加（インベントリから倉庫へアイテムを移動するときに使う）
    // インベントリでアイテムを削除してから追加する必要がある。
    // インベントリでアイテムを削除していないと
    // 「すでにそのSubIDは使われている」ということでエラーになる
    void AddItemWithSubID(const std::wstring& id, const int subId, const int durability = -1);

    // subId・・・耐久値の50の石斧と耐久値が100の石斧を所持することができる
    // そのため、アイテムにはsubIdが振られる。アイテムを削除するにはsubIdがないとできない
    // アイテムは削除できるのでSubIDが連番になるとは限らない
    void RemoveItem(const std::wstring& id, const int subId);

    void SetItemDurability(const std::wstring& id, const int subId, const int durability);

    bool ExistItem(const std::wstring& id, const int subId);
    ItemInfo GetItemInfo(const std::wstring& id, const int subId);

    std::vector<int> GetSubIdList(const std::wstring& id);

    int CountItem(const std::wstring& id);

    float GetWeight();

    std::list<ItemInfo> GetAllItem();

    void SetXYZ(const float x, const float y, const float z);
    void GetXYZ(float* x, float* y, float* z);

    void SetIsRaft(const bool arg);
    bool GetIsRaft() const;

    void SetCsvFileName(const std::wstring arg);
    std::wstring GetCsvFileName() const;

private:

    float CalcWeight();

    void Sort();

    // 所持品の重量（武器は含めない）
    float m_weight = 0.f;

    // 所持品リスト
    // 同じアイテムを複数所持できることに注意
    std::list<ItemInfo> m_itemInfoList;

    float m_x = 0.f;
    float m_y = 0.f;
    float m_z = 0.f;

    // イカダの上に配置された倉庫か否か。
    bool m_bRaft = false;

    // ディレクトリを除いた、ファイル名。
    std::wstring m_csvfilename;
};

// 倉庫管理クラス
// 倉庫は複数持つことができる
// イカダを作ると倉庫は増える
class StorehouseManager
{
public:

    static StorehouseManager* Get();
    static void Destroy();

    void Init(const std::wstring& csvfile);
    bool Inited();

    void Save(const std::wstring& managerFile,
              const std::wstring& csvDir);

    std::vector<int> GetStorehouseIdList();

    Storehouse* GetStorehouse(const int id);

    // 3メートル以内の倉庫を取得。
    // なければNULL
    Storehouse* GetNearStorehouse(const float x, const float z);

    // 現在の活動拠点の倉庫
    Storehouse* GetCurrentActiveStorehouse();

    // サブIDをすべての倉庫から取得する
    std::vector<int> GetSubIdListFromAllStorehouse(const std::wstring& id);

    // 倉庫を増やす
    // イカダに乗せる倉庫についてのみ増やすことができる
    void AddStorehouse();

private:

    // シングルトンオブジェクト
    static StorehouseManager* m_obj;

    std::map<int, Storehouse> m_StorehouseMap;

    bool m_inited = false;
};

}

