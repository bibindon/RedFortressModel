// TODO 最大積載量
#pragma once

#include "HeaderOnlyCsv.hpp"
#include "ItemManager.h"

#include <string>
#include <vector>
#include <unordered_map>
#include "StatusManager.h"

namespace NSModel
{
// プレイヤーの所持品を管理するクラス。
// アイテムの定義はItemManagerが管理している。
// 武器についてはWeaponManagerが所持数まで管理しているので
// Inventoryクラスに出番はない予定

class Inventory
{
public:

    static Inventory* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfile,
              const bool decrypt = false);
    bool Inited();
    void Save(const std::wstring& csvfile,
              const bool encrypt = false);

    // 一個ずつしか増加減出来ない想定
    // 新たにSubIDを割り当てる
    // 新たに割り当てたSubIDを返す
    int AddItem(const std::wstring& id, const int durability = -1);

    // SubIDを指定してアイテムを追加（倉庫からインベントリへアイテムを移動するときに使う）
    // 倉庫でアイテムを削除してから追加する必要がある。
    // 倉庫でアイテムを削除していないと「すでにそのSubIDは使われている」ということでエラーになる
    void AddItemWithSubID(const std::wstring& id, const int subId, const int durability = -1);

    // subId・・・耐久値の50の石斧と耐久値が100の石斧を所持することができる
    // そのため、アイテムにはsubIdが振られる。アイテムを削除するにはsubIdがないとできない
    // アイテムは削除できるのでSubIDが連番になるとは限らない
    void RemoveItem(const std::wstring& id, const int subId);

    void SetItemDurability(const std::wstring& id, const int subId, const int durability);

    bool ExistItem(const std::wstring& id, const int subId);
    ItemInfo GetItemInfo(const std::wstring& id, const int subId);

    // SubIDをリストで受け取る
    std::vector<int> GetSubIdList(const std::wstring& id);

    int CountItem(const std::wstring& id);

    float GetWeight();

    std::list<ItemInfo> GetAllItem();

    float CalcVolume();
    float GetVolume() const;

    void UpdateVolumeMax(const std::vector<ItemInfo>& bagMap);
    float GetVolumeMax() const;

    // 装備中の袋の耐久値を減らす
    void ReduceEquipBagDurability();

private:
    // 以下の4つの関数を宣言・定義するのが正しいが・・・なくても・・・別に・・・
    Inventory() {};
    Inventory(const Inventory&);
    Inventory& operator=(const Inventory&) {}
    ~Inventory() {};

    float CalcWeight();

    void Sort();

    // シングルトンオブジェクト
    static Inventory* obj;

    // 所持品の重量（武器は含めない）
    float m_weight = 0.f;

    // 所持品リスト
    // 同じアイテムを複数所持できることに注意
    std::list<ItemInfo> m_itemInfoList;
    
    bool m_inited = false;

    // 最大積載量
    float m_volumeMax = 0.f;
    float m_volumeCurrent = 0.f;
};
}

