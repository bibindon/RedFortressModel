#pragma once

#include <string>
#include <vector>

namespace NSStarmanLib
{
// TODO セーブデータの暗号化

class MapInfo
{
public:

    void SetId(const std::wstring& id);
    std::wstring GetId() const;

    void SetName(const std::wstring& name);
    std::wstring GetName();

    void SetDetail(const std::wstring& detail);
    std::wstring GetDetail();

    void SetPos(const int x, const int y);
    void GetPos(int* x, int* y);

    void SetImagePath(const std::wstring& imagePath);
    std::wstring GetImagePath();

    void SetDiscovered(const bool);
    bool IsDiscovered();

private:

    std::wstring m_id;
    std::wstring m_name;
    std::wstring m_imagePath;
    std::wstring m_detail;
    int m_x = 0;
    int m_y = 0;
    bool m_isDiscovered = false;
};

class MapInfoManager
{
public:
    static MapInfoManager* GetObj();

    void Init(const std::wstring& csvfile, const std::wstring& savefile, const bool decrypt = false);
    void Save(const std::wstring& csvfile, const bool encrypt = false);
    
    static void Destroy();
    std::vector<std::wstring> GetIdList();
    bool IsDiscovered(const std::wstring& id);
    void SetDiscovered(const std::wstring& id);
    std::wstring GetName(const std::wstring& id);
    std::wstring GetDetail(const std::wstring& id);
    void GetPos(const std::wstring& id, int* x, int* y);
    std::wstring GetImagePath(const std::wstring& id);

private:

    static MapInfoManager* obj;
    std::vector<MapInfo> m_mapInfoList;
};
}

