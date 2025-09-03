#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "ItemManager.h"

namespace NSStarmanLib
{
// クラフト用の素材クラス
class CraftMaterial
{
public:

    void SetId(const std::wstring& arg);
    std::wstring GetId() const;

    void SetName(const std::wstring& arg);
    std::wstring GetName() const;

    void SetNumber(const int arg);
    int GetNumber() const;

    void SetLevel(const int arg);
    int GetLevel() const;

private:

    std::wstring m_id;

    // 素材の名前
    std::wstring m_name;
    
    // 素材の必要数
    int m_number = 0;
    
    // 素材の強化レベル
    int m_level = 0;

};

// 完成品クラス
class CraftOutput
{
public:

    void SetItemId(const std::wstring& id);
    std::wstring GetItemId() const;

    void SetNumber(const int arg);
    int GetNumber() const;

    void SetLevel(const int arg);
    int GetLevel() const;

private:
    
    // 完成品の出力数
    int m_number = 0;
    
    // 完成品の強化レベル
    int m_level = 0;

    // ItemID
    std::wstring m_id;
};

// クラフトの情報クラス
class CraftInfo
{
public:

    void SetOutput(const CraftOutput& arg);
    CraftOutput GetOutput() const;

    // 必要素材の定義情報
    void SetCraftMaterialDef(const int index, const CraftMaterial& arg);
    CraftMaterial GetCraftMaterialDef(const int index);
    std::vector<CraftMaterial> GetCraftMaterialDef();

    // 預けたアイテム
    // クラフト開始前にキャンセルしたら素材が返ってくるので、IDとSubIDを保存しておく必要がある。
    void SetCraftMaterial(std::vector<ItemInfo> items);
    std::vector<ItemInfo> GetCraftMaterial();

private:

    CraftOutput m_output;

    std::unordered_map<int, CraftMaterial> m_materialDefList;

    std::vector<ItemInfo> m_materialList;
};

// クラフト情報の管理クラス
// イカダが完成品となるレシピは11種類ある。
// 強化値が＋１のイカダ、強化値が＋２のイカダ・・・
// レシピを特定するには、完成品の名前だけではなく、名前と個数と強化値が必要
class CraftInfoManager
{

public:

    static CraftInfoManager* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfileDefinition,
              const bool decrypt = false);
    bool Inited() const;

    std::vector<CraftOutput> GetCraftItemList();

    CraftInfo GetCraftInfo(const CraftOutput& output);
    CraftInfo GetCraftInfo(const std::wstring& itemId, const int num, const int level);

    CraftInfo GetCraftInfoByID(const std::wstring& itemId);

private:

    // シングルトンオブジェクト
    static CraftInfoManager* obj;

    std::vector<CraftInfo> m_craftInfoList;
    bool m_init = false;;
};
}

