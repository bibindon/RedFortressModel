#pragma once

namespace NSStarmanLib
{

//------------------------------------------------------
// 雨のアルゴリズム
//
// 雨が降っていないとき
//      1％の確率で1時間後に雨が降る。
//      その1時間後も1％の確率で雨が降る。
//      そんな感じで24時間後には22％の確率で雨が降る。
//      1 - (0.99) ^ 24
//
// 雨が降ったら1時間は少なくとも降る
//
// 雨が降ったら
//      1時間後に雨が止む確率は10％。
//      その1時間後に雨が止む確率も10％、さらにその1時間後に雨が止む確率も10％。
//      24時間後も雨が降っている確率は0.9の24乗で8％になる。
//      とにかくそんな感じで計算する。
//------------------------------------------------------

class RainModel
{
public:
    
    static RainModel* Get();
    void Destroy();
    void Update();
    bool IsRain() const;

private:

    static RainModel* m_rain;
    bool m_bRain = false;

    int m_previousYear = 0;
    int m_previousMonth = 0;
    int m_previousDay = 0;
    int m_previousHour = 0;
};
}

