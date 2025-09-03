#pragma once

#include <vector>
#include <map>

#include "ItemManager.h"

//-------------------------------------------------
// お手伝い機能
//
// 子供たちが拾ってきたアイテムを受け取れる。
// 「話しかけたら受け取れる」であることに注意。
// 依頼や指示はできない。
// 児童労働法に触れるため。
// クラフト中は受け取れない。
// 16時以降に話しかけたら受け取れる。
//-------------------------------------------------
namespace NSStarmanLib
{
class Help
{
public:
    static Help* Get();
    static void Destroy();

    void Init(const std::wstring& filepath);
    void Update();
    void Save(const std::wstring& filepath);

    // 16時以降に話しかけたら受け取れる。
    // 受け取ったら消失。
    // 受け取らずに翌日の16時になったら新しいアイテムになり、古いものは消失
    // 10個まで
    std::vector<ItemDef> ReceiveItems(const std::wstring& npcName);

    bool Received(const std::wstring& npcName);
    bool CanReceive(const std::wstring& npcName);

private:

    static Help* m_obj;

    std::map<std::wstring, std::vector<ItemDef>> m_presentMap;

    // 16時を超えたら翌日の日付を保存し、現在の日付とする
    int m_previousYear = 0;
    int m_previousMonth = 0;
    int m_previousDay = 0;
    int m_previousHour = 0;

    // アイテムを渡し済みか
    std::map<std::wstring, bool> m_presented;

    std::vector<ItemDef> GetRandomItem(const std::wstring& npcName);

    bool CrossOver16() const;

    // 過去にさかのぼっている？
    bool BackTime() const;

    // 拾うことが可能なアイテムのIDのリスト
    // 拾えるはずのないアイテムを拾えないようにするため。
    std::vector<std::wstring> m_enableItemIdList;
    bool m_bLoaded = false;

    bool IsCraftingRaft();
};
}

