#pragma once

#include <string>

namespace NSStarmanLib
{
// ゲーム内の日付・時刻
// パワーエッグ星では24時間で一日が経過し、ゲーム内では2時間で24時間が経過する。

class PowereggDateTime
{
public:

    static PowereggDateTime* GetObj();

    static void Destroy();

    void Init(const std::wstring& csvfile,
              const bool decrypt = false);
    void Save(const std::wstring& csvfile,
              const bool encrypt = false) const;

    // 時を進める。パワーエッグ星の時間軸で指定する。
    // パワーエッグ星での24時間がゲーム内では2時間であるが、
    // ゲーム内時間ではなくパワーエッグ星での時間を指定する。
    void IncreaseDateTime(int month, int day, int hour, int minute, int second);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    int GetHour() const;
    int GetMinute() const;
    int GetSecond() const;
    int DAY_OF_MONTH(const int arg) const;

    // 二つの日時のうち最初の日にの方が古いならtrue
    static bool FromPastToFuture(int year1, int month1, int day1, int hour1, int minute1, int second1,
                                 int year2, int month2, int day2, int hour2, int minute2, int second2);

private:

    // シングルトン
    static PowereggDateTime* obj;

    int m_year { 0 };

    // 0月は存在しないことに注意すること
    int m_month { 0 };

    // 0日は存在しないことに注意すること
    int m_day { 0 };

    int m_hour { 0 };
    int m_minute { 0 };
    int m_second { 0 };

    // DAY_OF_MONTH[1]が1月の日数を表す。
    // DAY_OF_MONTH[0]は無効
    const int DAY_OF_MONTH_[13] { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
};
}

