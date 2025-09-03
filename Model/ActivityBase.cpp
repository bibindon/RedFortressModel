#include "ActivityBase.h"
#include <vector>
#include "Util.h"
#include "Voyage.h"

NSStarmanLib::ActivityBase* NSStarmanLib::ActivityBase::m_obj = nullptr;

NSStarmanLib::ActivityBase* NSStarmanLib::ActivityBase::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = new ActivityBase();
    }

    return m_obj;
}

NSStarmanLib::ActivityBase::ActivityBase()
{
}

void NSStarmanLib::ActivityBase::Init(const std::wstring& csvfile)
{
    std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, false);

    if (vvs.at(0).at(1) == _T("Precision"))
    {
        m_eBaseType = eBaseType::Precision;
    }
    else if (vvs.at(0).at(1) == _T("DirectNex"))
    {
        m_eBaseType = eBaseType::DirectNex;
    }

    // 墓に花輪を飾った
    if (vvs.at(1).at(1) == _T("y"))
    {
        m_bHanawa = true;
    }
    else if (vvs.at(1).at(1) == _T("n"))
    {
        m_bHanawa = false;
    }

}

void NSStarmanLib::ActivityBase::Save(const std::wstring& csvfile)
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    vs.push_back(_T("現在の活動拠点"));

    if (m_eBaseType == eBaseType::Precision)
    {
        vs.push_back(_T("Precision"));
    }
    else if (m_eBaseType == eBaseType::DirectNex)
    {
        vs.push_back(_T("DirectNex"));
    }
    vvs.push_back(vs);

    vs.clear();
    vs.push_back(_T("墓に花輪を飾った"));

    if (m_bHanawa)
    {
        vs.push_back(_T("y"));
    }
    else
    {
        vs.push_back(_T("n"));
    }

    vvs.push_back(vs);

    Util::WriteToCsv(csvfile, vvs, false);
}

void NSStarmanLib::ActivityBase::SetBaseType(const eBaseType arg)
{
        m_eBaseType = arg;
}

NSStarmanLib::eBaseType NSStarmanLib::ActivityBase::GetBaseType() const
{
    return m_eBaseType;
}

void NSStarmanLib::ActivityBase::Finalize()
{
    delete ActivityBase::m_obj;
    ActivityBase::m_obj = nullptr;
}

void NSStarmanLib::ActivityBase::GetPierPos(const eBaseType baseType, float* x, float* y, float* z)
{
    if (baseType == eBaseType::Precision)
    {
        *x = -4.5f;
        *y = 458.7f;
        *z = -711.f;
    }
    else if (baseType == eBaseType::DirectNex)
    {
        *x = 1178.5f;
        *y = 11.f;
        *z = 401.0f;
    }
}

bool NSStarmanLib::ActivityBase::CheckRaftNearPier(const float x, const float y, const float z)
{
    float x2, y2, z2;
    GetPierPos(m_eBaseType, &x2, &y2, &z2);

    return Util::HitByBoundingBox(x, y, z, x2, y2, z2, 5.f);
}

bool NSStarmanLib::ActivityBase::CheckRaftExist()
{
    auto raftList = Voyage::Get()->GetRaftList();

    bool bExist = false;

    for (auto& raft : raftList)
    {
        float x, y, z;
        raft.GetXYZ(&x, &y, &z);

        bExist = CheckRaftNearPier(x, y, z);
        if (bExist)
        {
            break;
        }
    }

    return bExist;
}

