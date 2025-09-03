#pragma once

#include "HeaderOnlyCsv.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace NSStarmanLib
{
class HumanInfo
{
public:

    void SetId(const std::wstring& arg) { m_id = arg; }
    std::wstring GetId() const { return m_id; }

    void SetName(const std::wstring& arg);
    std::wstring GetName();

    void SetDetail(const std::wstring& arg);
    std::wstring GetDetail();

    void SetImagePath(const std::wstring& arg);
    std::wstring GetImagePath();

    void SetVisible(const bool arg);
    bool GetVisible();

private:
    std::wstring m_id;

    std::wstring m_name;
    std::wstring m_detail;
    std::wstring m_imagePath;

    // 人物情報はメニュー画面で表示され、
    // ゲームの進行度によって表示される人と表示されない人がいる想定
    bool m_bVisible = false;
};

class HumanInfoManager
{

public:

    static HumanInfoManager* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfileBase, const std::wstring& csvfileSaved,
              const bool decrypt = false);

    void Save(const std::wstring& csvfile,
              const bool encrypt = false);

    HumanInfo GetHumanInfo(const std::wstring& id);
    std::vector<std::wstring> GetHumanIdList();
    void SetHumanVisible(const std::wstring& id);

private:

    // シングルトンオブジェクト
    static HumanInfoManager* obj;

    std::unordered_map<std::wstring, HumanInfo> m_humanInfoMap;

};
}

