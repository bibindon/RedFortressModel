#pragma once
#include <string>
#include <vector>

namespace NSModel
{
class GuideItem
{
public:
    void SetCategory(const std::wstring& category);
    std::wstring GetCategory();

    void SetSubCategory(const std::wstring& subCategory);
    std::wstring GetSubCategory();

    void SetText(const std::wstring& text);
    std::wstring GetText();

    void SetVisible(const bool visible);
    bool GetVisible();

    void SetId(const int id) { m_id = id; }
    int GetId() const { return m_id; }

private:

    // 大分類
    std::wstring m_category;

    // 小分類
    std::wstring m_subCategory;

    // 説明文
    std::wstring m_text;

    // 表示済みか否か
    bool m_bVisible { false };

    int m_id = -1;
};

// 操作説明
class Guide
{
public:

    static Guide* GetObj();

    void Init(const std::wstring& csvfile, const std::wstring& savefile, const bool decrypt = false);

    void Save(const std::wstring& csvfile, const bool encrypt = false);

    static void Destroy();

    std::vector<std::wstring> GetCategoryList();
    std::vector<std::wstring> GetSubCategoryList(const std::wstring& category);
    std::wstring GetText(const std::wstring& category, const std::wstring& subCategory);

    bool GetVisible(const std::wstring& category, const std::wstring& subCategory);
    void SetVisible(const std::wstring& category, const std::wstring& subCategory);
    void SetVisible(const int id);

private:

    static Guide* obj;
    std::vector<GuideItem> m_guideList;
};
}

