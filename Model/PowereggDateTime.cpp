#include "PowereggDateTime.h"
#include <vector>
#include "Util.h"
#include "NpcStatusManager.h"
#include "Rynen.h"

using namespace NSStarmanLib;

PowereggDateTime* PowereggDateTime::obj = nullptr;

PowereggDateTime* PowereggDateTime::GetObj()
{
    if (obj == nullptr)
    {
        obj = new PowereggDateTime();
    }
    return obj;
}

void PowereggDateTime::Destroy()
{
    delete PowereggDateTime::obj;
    PowereggDateTime::obj = nullptr;
}

void PowereggDateTime::Init(const std::wstring& csvfile,
                            const bool decrypt)
{
    std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, decrypt);

    m_year = std::stoi(vvs.at(0).at(1));
    m_month = std::stoi(vvs.at(1).at(1));
    m_day = std::stoi(vvs.at(2).at(1));
    m_hour = std::stoi(vvs.at(3).at(1));
    m_minute = std::stoi(vvs.at(4).at(1));
    m_second = std::stoi(vvs.at(5).at(1));
}

void PowereggDateTime::Save(const std::wstring& csvfile,
                            const bool encrypt) const
{
    std::vector<std::vector<std::wstring>> vvs;
    std::vector<std::wstring> vs;
    int work = 0;

    vs.push_back(_T("年"));
    vs.push_back(std::to_wstring(m_year));
    vvs.push_back(vs);
    vs.clear();

    vs.push_back(_T("月"));
    vs.push_back(std::to_wstring(m_month));
    vvs.push_back(vs);
    vs.clear();

    vs.push_back(_T("日"));
    vs.push_back(std::to_wstring(m_day));
    vvs.push_back(vs);
    vs.clear();

    vs.push_back(_T("時"));
    vs.push_back(std::to_wstring(m_hour));
    vvs.push_back(vs);
    vs.clear();

    vs.push_back(_T("分"));
    vs.push_back(std::to_wstring(m_minute));
    vvs.push_back(vs);
    vs.clear();

    vs.push_back(_T("秒"));
    vs.push_back(std::to_wstring(m_second));
    vvs.push_back(vs);
    vs.clear();

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

void PowereggDateTime::IncreaseDateTime(int month, int day, int hour, int minute, int second)
{
    m_month += month;

    m_day += day;

    m_hour += hour;

    m_minute += minute;

    m_second += second;

    if (m_second >= 60)
    {
        m_second -= 60;
        m_minute++;
    }

    if (m_minute >= 60)
    {
        m_minute -= 60;
        m_hour++;
    }

    if (m_hour >= 24)
    {
        m_hour -= 24;
        m_day++;
    }

    // 9月31日だったら10月1日にする
    // 9月32日だったら10月2日にする
    if (m_day > DAY_OF_MONTH_[m_month])
    {
        m_day -= DAY_OF_MONTH_[m_month];
        m_month++;
    }

    if (m_month >= 12)
    {
        m_month -= 12;
        m_year++;
    }

    // ライネンの契約をするまではNPCの消費を行わない。
    if (Rynen::GetObj()->GetContracted())
    {
        NpcStatusManager::GetObj()->AdvanceTime(hour, minute);
    }
}

int PowereggDateTime::GetYear() const
{
    return m_year;
}

int PowereggDateTime::GetMonth() const
{
    return m_month;
}

int PowereggDateTime::GetDay() const
{
    return m_day;
}

int PowereggDateTime::GetHour() const
{
    return m_hour;
}

int PowereggDateTime::GetMinute() const
{
    return m_minute;
}

int PowereggDateTime::GetSecond() const
{
    return m_second;
}

int NSStarmanLib::PowereggDateTime::DAY_OF_MONTH(const int arg) const
{
    return DAY_OF_MONTH_[arg];
}

// 日付同士の比較
// 1年2月3日4時5分6秒と6年5月4日3時2分1秒のどちらが未来・過去かを判定するには
// 010203040506 と 060504030201 という12桁の数値と見なして12桁の数値同士の比較を行えばよい
bool NSStarmanLib::PowereggDateTime::FromPastToFuture(int year1, int month1, int day1,
                                                      int hour1, int minute1, int second1,
                                                      int year2, int month2, int day2,
                                                      int hour2, int minute2, int second2)
{
    long long leftDateTime = 0;
    leftDateTime += (long long)year1   * 10000000000;
    leftDateTime += (long long)month1  * 100000000;
    leftDateTime += (long long)day1    * 1000000;
    leftDateTime += (long long)hour1   * 10000;
    leftDateTime += (long long)minute1 * 100;
    leftDateTime += (long long)second1 * 1;

    long long rightDateTime = 0;
    rightDateTime += (long long)year2   * 10000000000;
    rightDateTime += (long long)month2  * 100000000;
    rightDateTime += (long long)day2    * 1000000;
    rightDateTime += (long long)hour2   * 10000;
    rightDateTime += (long long)minute2 * 100;
    rightDateTime += (long long)second2 * 1;

    return leftDateTime < rightDateTime;
}
