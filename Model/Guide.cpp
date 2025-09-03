#include "Guide.h"
#include "Util.h"
#include <algorithm>

using namespace NSModel;

Guide* Guide::obj { nullptr };

void GuideItem::SetCategory(const std::wstring& category)
{
    m_category = category;
}

std::wstring GuideItem::GetCategory()
{
    return m_category;
}

void GuideItem::SetSubCategory(const std::wstring& subCategory)
{
    m_subCategory = subCategory;
}

std::wstring GuideItem::GetSubCategory()
{
    return m_subCategory;
}

void GuideItem::SetText(const std::wstring& text)
{
    m_text = text;
    m_text.erase(std::remove(m_text.begin(), m_text.end(), '"'), m_text.end());
}

std::wstring GuideItem::GetText()
{
    return m_text;
}

void GuideItem::SetVisible(const bool visible)
{
    m_bVisible = visible;
}

bool GuideItem::GetVisible()
{
    return m_bVisible;
}

Guide* Guide::GetObj()
{
    if (obj == nullptr)
    {
        obj = new Guide();
    }
    return obj;
}

void Guide::Init(const std::wstring& csvfile,
                 const std::wstring& savefile,
                 const bool decrypt)
{
    m_guideList.clear();

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            GuideItem guideItem;
            guideItem.SetId(std::stoi(vvs.at(i).at(0)));
            guideItem.SetCategory(vvs.at(i).at(1));
            guideItem.SetSubCategory(vvs.at(i).at(2));
            guideItem.SetText(vvs.at(i).at(3));
            m_guideList.push_back(guideItem);
        }
    }

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(savefile, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            auto id = std::stoi(vvs.at(i).at(0));
            auto visible = vvs.at(i).at(1);
            if (visible == L"y")
            {
                SetVisible(id);
            }
        }
    }
}

void Guide::Save(const std::wstring& csvfile, const bool encrypt)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;

    vs.push_back(_T("ID"));
    vs.push_back(_T("Unlocked"));
    vvs.push_back(vs);
    vs.clear();

    for (std::size_t i = 0; i < m_guideList.size(); ++i)
    {
        vs.push_back(std::to_wstring(m_guideList.at(i).GetId()));

        if (m_guideList.at(i).GetVisible())
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T(""));
        }
        vvs.push_back(vs);
        vs.clear();
    }

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

void Guide::Destroy()
{
    delete Guide::obj;
    Guide::obj = nullptr;
}

std::vector<std::wstring> Guide::GetCategoryList()
{
    std::vector<std::wstring> vs;
    for (std::size_t i = 0; i < m_guideList.size(); ++i)
    {
        auto result = std::find(vs.begin(), vs.end(), m_guideList.at(i).GetCategory());

        if (result == vs.end())
        {
            vs.push_back(m_guideList.at(i).GetCategory());
        }
    }

    return vs;
}

std::vector<std::wstring> Guide::GetSubCategoryList(const std::wstring& category)
{
    std::vector<std::wstring> vs;
    for (std::size_t i = 0; i < m_guideList.size(); ++i)
    {
        if (m_guideList.at(i).GetCategory() == category)
        {
            vs.push_back(m_guideList.at(i).GetSubCategory());
        }
    }
    return vs;
}

std::wstring Guide::GetText(const std::wstring& category, const std::wstring& subCategory)
{
    std::wstring result;
    for (std::size_t i = 0; i < m_guideList.size(); ++i)
    {
        if (m_guideList.at(i).GetCategory() == category)
        {
            if (m_guideList.at(i).GetSubCategory() == subCategory)
            {
                result = m_guideList.at(i).GetText();
                break;
            }
        }
    }
    return result;
}

bool Guide::GetVisible(const std::wstring& category, const std::wstring& subCategory)
{
    bool result = false;
    for (std::size_t i = 0; i < m_guideList.size(); ++i)
    {
        if (m_guideList.at(i).GetCategory() == category)
        {
            if (m_guideList.at(i).GetSubCategory() == subCategory)
            {
                result = m_guideList.at(i).GetVisible();
                break;
            }
        }
    }
    return result;
}

void Guide::SetVisible(const std::wstring& category, const std::wstring& subCategory)
{
    for (std::size_t i = 0; i < m_guideList.size(); ++i)
    {
        if (m_guideList.at(i).GetCategory() == category)
        {
            if (m_guideList.at(i).GetSubCategory() == subCategory)
            {
                m_guideList.at(i).SetVisible(true);
                break;
            }
        }
    }
}

void NSModel::Guide::SetVisible(const int id)
{
    auto it = std::find_if(m_guideList.begin(), m_guideList.end(),
                           [&](GuideItem& x)
                           {
                               return x.GetId() == id;
                           });
    it->SetVisible(true);
}

