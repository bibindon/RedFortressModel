#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "PatchTestManager.h"
#include "PowereggDateTime.h"
#include "Util.h"
#include "StatusManager.h"
#include <cassert>

using namespace NSStarmanLib;

PatchTestManager* PatchTestManager::m_obj = nullptr;

PatchTestManager* NSStarmanLib::PatchTestManager::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = new PatchTestManager();
    }

    return m_obj;
}

void NSStarmanLib::PatchTestManager::Destroy()
{
    delete m_obj;
    m_obj = nullptr;
}

void NSStarmanLib::PatchTestManager::Init(const std::wstring& originFile,
                                          const std::wstring& saveFileInfo,
                                          const std::wstring& saveFileQue)
{
    m_keyList.clear();
    m_infoMap.clear();
    m_PatchTestQue.clear();

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(originFile, false);
        for (size_t i = 1; i < vvs.size(); ++i)
        {
            m_keyList.push_back(vvs.at(i).at(0));
        }
    }

    //-------------------------------------------------------------
    // 毒、毒ではない、という情報はあらかじめ決まっているわけではない。
    // ゲーム開始時にランダムで決まる
    //-------------------------------------------------------------

    // saveFileInfoが空ならゲーム開始時。
    // 毒、毒ではない情報を作成する
    if (saveFileInfo.empty())
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(originFile, false);

        // 現在時刻の情報で初期化
        srand((unsigned int)time(NULL));

        // UnknownPlantNだったらどれか一つだけ毒
        // それ以外は毒ではない、とする

        for (size_t i = 1; i < vvs.size(); ++i)
        {
            auto& id = vvs.at(i).at(0);

            m_infoMap[id].SetItemId(id);

            auto name = ItemManager::GetObj()->GetItemDef(id).GetName();

            m_infoMap[id].SetName(name);
            m_infoMap[id].SetPoison(false);

            // 最初は正解率60％
            m_infoMap[id].SetAccurate(0.6f);
        }

        auto rnd = rand() % 6;
        if (rnd == 0)
        {
            m_infoMap[L"unknownPlant1"].SetPoison(true);
        }
        else if (rnd == 1)
        {
            m_infoMap[L"unknownPlant2"].SetPoison(true);
        }
        else if (rnd == 2)
        {
            m_infoMap[L"unknownPlant3"].SetPoison(true);
        }
        else if (rnd == 3)
        {
            m_infoMap[L"unknownPlant4"].SetPoison(true);
        }
        else if (rnd == 4)
        {
            m_infoMap[L"unknownPlant5"].SetPoison(true);
        }
        else if (rnd == 5)
        {
            m_infoMap[L"unknownPlant6"].SetPoison(true);
        }
    }
    else
    {
        {
            std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(saveFileInfo, false);

            for (size_t i = 1; i < vvs.size(); ++i)
            {
                auto& id = vvs.at(i).at(0);
                m_infoMap[id].SetItemId(id);
                auto name = ItemManager::GetObj()->GetItemDef(id).GetName();
                m_infoMap[id].SetName(name);

                if (vvs.at(i).at(1) == _T("y"))
                {
                    m_infoMap[id].SetPoison(true);
                }
                else if (vvs.at(i).at(1) == _T("n"))
                {
                    m_infoMap[id].SetPoison(false);
                }
                else
                {
                    throw std::exception();
                }

                int tryNum = std::stoi(vvs.at(i).at(2));
                m_infoMap[id].SetTryNum(tryNum);

                float accurate = std::stof(vvs.at(i).at(3));
                m_infoMap[id].SetAccurate(accurate);
            }
        }

        {
            std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(saveFileQue, false);

            for (size_t i = 1; i < vvs.size(); ++i)
            {
                PatchTest patchTest;

                auto& id = vvs.at(i).at(0);
                patchTest.SetItemId(id);

                auto name = ItemManager::GetObj()->GetItemDef(id).GetName();

                patchTest.SetItemName(name);

                if (vvs.at(i).at(1) == _T("NOT_START"))
                {
                    patchTest.SetState(PatchTest::eState::NOT_START);
                }
                else if (vvs.at(i).at(1) == _T("STARTED"))
                {
                    patchTest.SetState(PatchTest::eState::STARTED);
                }
                else if (vvs.at(i).at(1) == _T("FINISHED"))
                {
                    patchTest.SetState(PatchTest::eState::FINISHED);
                }
                else
                {
                    throw std::exception();
                }

                {
                    if (vvs.at(i).at(2).empty())
                    {
                        throw std::exception();
                    }

                    auto vs = Util::split(vvs.at(i).at(2));

                    patchTest.SetDateTimeReq(std::stoi(vs.at(0)),
                                             std::stoi(vs.at(1)),
                                             std::stoi(vs.at(2)),
                                             std::stoi(vs.at(3)),
                                             std::stoi(vs.at(4)),
                                             std::stoi(vs.at(5)));
                }

                {
                    if (!vvs.at(i).at(3).empty())
                    {
                        auto vs = Util::split(vvs.at(i).at(3));

                        patchTest.SetDateTimeStart(std::stoi(vs.at(0)),
                                                   std::stoi(vs.at(1)),
                                                   std::stoi(vs.at(2)),
                                                   std::stoi(vs.at(3)),
                                                   std::stoi(vs.at(4)),
                                                   std::stoi(vs.at(5)));
                    }
                }

                {
                    if (vvs.at(i).size() >= 5 && !vvs.at(i).at(4).empty())
                    {
                        auto vs = Util::split(vvs.at(i).at(4));

                        patchTest.SetDateTimeEnd(std::stoi(vs.at(0)),
                                                 std::stoi(vs.at(1)),
                                                 std::stoi(vs.at(2)),
                                                 std::stoi(vs.at(3)),
                                                 std::stoi(vs.at(4)),
                                                 std::stoi(vs.at(5)));
                    }
                }

                {
                    if (vvs.at(i).size() <= 5 || vvs.at(i).at(5).empty())
                    {
                        patchTest.SetResult(PatchTest::eResult::NOT_YET);
                    }
                    else if (vvs.at(i).at(5) == _T("毒"))
                    {
                        patchTest.SetResult(PatchTest::eResult::POISON);
                    }
                    else if (vvs.at(i).at(5) == _T("毒ではない"))
                    {
                        patchTest.SetResult(PatchTest::eResult::NOT_POISON);
                    }
                    else
                    {
                        throw std::exception();
                    }
                }

                m_PatchTestQue.push_back(patchTest);
            }
        }
    }
}

void NSStarmanLib::PatchTestManager::Save(const std::wstring& csvfileInfo,
                                          const std::wstring& csvfileQue)
{
    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;

        vs.push_back(_T("Item ID"));
        vs.push_back(_T("毒性"));
        vs.push_back(_T("パッチテスト回数"));
        vs.push_back(_T("正解率"));
        vvs.push_back(vs);
        vs.clear();

        assert(m_infoMap.size() != 0);
        assert(m_infoMap.size() != 1);

        for (auto it = m_infoMap.begin(); it != m_infoMap.end(); ++it)
        {
            vs.clear();

            assert(!it->second.GetItemId().empty());

            vs.push_back(it->second.GetItemId());

            if (it->second.GetPoison())
            {
                vs.push_back(_T("y"));
            }
            else
            {
                vs.push_back(_T("n"));
            }
            
            vs.push_back(std::to_wstring(it->second.GetTryNum()));
            vs.push_back(std::to_wstring(it->second.GetAccurate()));
            vvs.push_back(vs);
        }

        Util::WriteToCsv(csvfileInfo, vvs, false);
    }

    {
        std::vector<std::vector<std::wstring>> vvs;
        std::vector<std::wstring> vs;

        vs.push_back(_T("Item ID"));
        vs.push_back(_T("状態"));
        vs.push_back(_T("予約日時"));
        vs.push_back(_T("開始日時"));
        vs.push_back(_T("完了日時"));
        vs.push_back(_T("テスト結果"));
        vvs.push_back(vs);
        vs.clear();

        for (auto it = m_PatchTestQue.begin(); it != m_PatchTestQue.end(); ++it)
        {
            vs.clear();

            vs.push_back(it->GetItemId());

            if (it->GetState() == PatchTest::eState::NOT_START)
            {
                vs.push_back(_T("NOT_START"));
            }
            else if (it->GetState() == PatchTest::eState::STARTED)
            {
                vs.push_back(_T("STARTED"));
            }
            else if (it->GetState() == PatchTest::eState::FINISHED)
            {
                vs.push_back(_T("FINISHED"));
            }
            else
            {
                throw std::exception();
            }
            
            {
                std::wstring work;
                int y, M, d, h, m, s;
                it->GetDateTimeReq(&y, &M, &d, &h, &m, &s);
                work += std::to_wstring(y) + _T(":");
                work += std::to_wstring(M) + _T(":");
                work += std::to_wstring(d) + _T(":");
                work += std::to_wstring(h) + _T(":");
                work += std::to_wstring(m) + _T(":");
                work += std::to_wstring(s);

                vs.push_back(work);
            }

            {
                std::wstring work;
                int y, M, d, h, m, s;
                it->GetDateTimeStart(&y, &M, &d, &h, &m, &s);
                work += std::to_wstring(y) + _T(":");
                work += std::to_wstring(M) + _T(":");
                work += std::to_wstring(d) + _T(":");
                work += std::to_wstring(h) + _T(":");
                work += std::to_wstring(m) + _T(":");
                work += std::to_wstring(s);

                vs.push_back(work);
            }

            {
                std::wstring work;
                int y, M, d, h, m, s;
                it->GetDateTimeEnd(&y, &M, &d, &h, &m, &s);
                work += std::to_wstring(y) + _T(":");
                work += std::to_wstring(M) + _T(":");
                work += std::to_wstring(d) + _T(":");
                work += std::to_wstring(h) + _T(":");
                work += std::to_wstring(m) + _T(":");
                work += std::to_wstring(s);

                vs.push_back(work);
            }

            if (it->GetResult() == PatchTest::eResult::NOT_YET)
            {
                // do nothing
            }
            else if (it->GetResult() == PatchTest::eResult::POISON)
            {
                vs.push_back(_T("毒"));
            }
            else if (it->GetResult() == PatchTest::eResult::NOT_POISON)
            {
                vs.push_back(_T("毒ではない"));
            }

            vvs.push_back(vs);
        }

        Util::WriteToCsv(csvfileQue, vvs, false);
    }
}

void NSStarmanLib::PatchTestManager::Update()
{
    if (PowereggDateTime::GetObj() == nullptr)
    {
        return;
    }

    auto dateTime = PowereggDateTime::GetObj();

    // すべてFINISHEDならやることなし
    bool allFinish =
        std::all_of(m_PatchTestQue.begin(), m_PatchTestQue.end(),
                    [](PatchTest patchTest)
                    {
                        return patchTest.GetState() == PatchTest::eState::FINISHED;
                    });

    if (allFinish)
    {
        return;
    }

    // 実行中のテストがあるか
    bool existStarted =
        std::any_of(m_PatchTestQue.begin(), m_PatchTestQue.end(),
                    [](PatchTest patchTest)
                    {
                        return patchTest.GetState() == PatchTest::eState::STARTED;
                    });

    // 実行中のテストがあるならば、完了時刻を過ぎていないか判定を行う
    if (existStarted)
    {
        auto it = std::find_if(m_PatchTestQue.begin(), m_PatchTestQue.end(),
                               [](PatchTest patchTest)
                               {
                                   return patchTest.GetState() == PatchTest::eState::STARTED;
                               });

        int y, M, d, h, m, s;
        it->GetDateTimeEnd(&y, &M, &d, &h, &m, &s);

        auto finished = PowereggDateTime::FromPastToFuture(y, M, d, h, m, s,
                                                           dateTime->GetYear(),
                                                           dateTime->GetMonth(),
                                                           dateTime->GetDay(),
                                                           dateTime->GetHour(),
                                                           dateTime->GetMinute(),
                                                           dateTime->GetSecond());

        if (finished)
        {
            // 完了処理
            it->SetState(PatchTest::eState::FINISHED);

            it->SetDateTimeEnd(dateTime->GetYear(),
                               dateTime->GetMonth(),
                               dateTime->GetDay(),
                               dateTime->GetHour(),
                               dateTime->GetMinute(),
                               dateTime->GetSecond());

            auto id = it->GetItemId();
            auto accurate = m_infoMap[id].GetAccurate();

            // 正解率による結果判定

            srand((unsigned int)time(NULL));

            // 0 ~ 99の乱数
            int rnd = rand() % 100;

            int nAccurate = (int)accurate * 100;

            // 正解
            if (rnd < nAccurate)
            {
                auto poison = m_infoMap[id].GetPoison();
                if (poison)
                {
                    it->SetResult(PatchTest::eResult::POISON);
                }
                else
                {
                    it->SetResult(PatchTest::eResult::NOT_POISON);
                }
            }
            // 不正解
            else
            {
                auto poison = m_infoMap[id].GetPoison();
                if (poison)
                {
                    it->SetResult(PatchTest::eResult::NOT_POISON);
                }
                else
                {
                    it->SetResult(PatchTest::eResult::POISON);
                }
            }

            // パッチテスト回数を更新
            auto tryNum = m_infoMap[id].GetTryNum();
            ++tryNum;
            m_infoMap[id].SetTryNum(tryNum);

            // 正解率を更新
            accurate += 0.1f;
            if (accurate >= 0.9f)
            {
                accurate = 0.9f;
            }

            m_infoMap[id].SetAccurate(accurate);

            // 開始処理
            ++it;
            if (it != m_PatchTestQue.end())
            {
                it->SetState(PatchTest::eState::STARTED);
                it->SetDateTimeStart(dateTime->GetYear(),
                                     dateTime->GetMonth(),
                                     dateTime->GetDay(),
                                     dateTime->GetHour(),
                                     dateTime->GetMinute(),
                                     dateTime->GetSecond());

                it->SetDateTimeEnd(dateTime->GetYear(),
                                   dateTime->GetMonth(),
                                   dateTime->GetDay(),
                                   dateTime->GetHour() + 1,
                                   dateTime->GetMinute(),
                                   dateTime->GetSecond());
            }
        }
    }
    // 実行中のテストがないならば、一番古い依頼を開始する
    else
    {
        // 依頼は末尾に追加するので先頭ほど古い依頼
        for (size_t i = 0; i < m_PatchTestQue.size(); ++i)
        {
            auto& patchTest = m_PatchTestQue.at(i);

            // 未完了を見つけたら開始する
            if (patchTest.GetState() == PatchTest::eState::NOT_START)
            {
                patchTest.SetState(PatchTest::eState::STARTED);
                patchTest.SetDateTimeStart(dateTime->GetYear(),
                                           dateTime->GetMonth(),
                                           dateTime->GetDay(),
                                           dateTime->GetHour(),
                                           dateTime->GetMinute(),
                                           dateTime->GetSecond());

                patchTest.SetDateTimeEnd(dateTime->GetYear(),
                                         dateTime->GetMonth(),
                                         dateTime->GetDay(),
                                         dateTime->GetHour() + 1,
                                         dateTime->GetMinute(),
                                         dateTime->GetSecond());

                break;
            }
        }
    }
}

bool NSStarmanLib::PatchTestManager::QueuePatchTest(const std::wstring& id)
{
    auto it = m_infoMap.find(id);
    if (it == m_infoMap.end())
    {
        throw std::exception();
    }

    // 一度に依頼できるのは３回まで
    auto notFinishCount =
        std::count_if(m_PatchTestQue.begin(), m_PatchTestQue.end(),
                      [](PatchTest patchTest)
                      {
                          return patchTest.GetState() != PatchTest::eState::FINISHED;
                      });

    if (notFinishCount >= 4)
    {
        return false;
    }

    PatchTest patchTest;

    patchTest.SetItemId(id);
    auto name = ItemManager::GetObj()->GetItemDef(id).GetName();
    patchTest.SetItemName(name);

    auto dateTime = PowereggDateTime::GetObj();

    patchTest.SetDateTimeReq(dateTime->GetYear(),
                             dateTime->GetMonth(),
                             dateTime->GetDay(),
                             dateTime->GetHour(),
                             dateTime->GetMinute(),
                             dateTime->GetSecond()
                             );

    m_PatchTestQue.push_back(patchTest);

    // 会話したら脳のスタミナを消費
    auto brainStamina = StatusManager::GetObj()->GetBrainStaminaCurrent();
    StatusManager::GetObj()->SetBrainStaminaCurrent(brainStamina - 1.f);

    return true;
}

std::vector<PatchTest> NSStarmanLib::PatchTestManager::GetQueue()
{
    return m_PatchTestQue;
}

// 先頭が一番新しく、末尾に向かうほど古いテスト結果
std::vector<PatchTest> NSStarmanLib::PatchTestManager::GetResultList(const std::wstring& id)
{
    std::vector<PatchTest> resultList;

    for (auto it = m_PatchTestQue.rbegin(); it != m_PatchTestQue.rend(); ++it)
    {
        if (it->GetItemId() == id)
        {
            if (it->GetState() == PatchTest::eState::FINISHED)
            {
                resultList.push_back(*it);
            }
        }
    }

    return resultList;
}

std::vector<std::wstring> NSStarmanLib::PatchTestManager::GetKeyList()
{
    return m_keyList;
}

std::wstring NSStarmanLib::PatchTestManager::GetPoisonPlant()
{
    std::wstring poisonPlant;
    for (auto it = m_infoMap.begin(); it != m_infoMap.end(); ++it)
    {
        if (it->second.GetPoison())
        {
            poisonPlant = it->first;
            break;
        }
    }

    return poisonPlant;
}

void NSStarmanLib::PatchTest::SetItemName(const std::wstring& arg)
{
    m_itemName = arg;
}

std::wstring NSStarmanLib::PatchTest::GetItemName() const
{
    return m_itemName;
}

void NSStarmanLib::PatchTest::SetItemId(const std::wstring& arg)
{
    m_itemId = arg;
}

std::wstring NSStarmanLib::PatchTest::GetItemId() const
{
    return m_itemId;
}

void NSStarmanLib::PatchTest::SetDateTimeReq(const int y, const int M, const int d,
                                             const int h, const int m, const int s)
{
    m_yearReq = y;
    m_monthReq = M;
    m_dayReq = d;

    m_hourReq = h;
    m_minuteReq = m;
    m_secondReq = s;
}

void NSStarmanLib::PatchTest::GetDateTimeReq(int* y, int* M, int* d, int* h, int* m, int* s)
{
    *y = m_yearReq;
    *M = m_monthReq;
    *d = m_dayReq;

    *h = m_hourReq;
    *m = m_minuteReq;
    *s = m_secondReq;
}

void NSStarmanLib::PatchTest::SetDateTimeStart(int y, int M, int d, int h, int m, int s)
{
    m_yearStart = y;
    m_monthStart = M;
    m_dayStart = d;

    m_hourStart = h;
    m_minuteStart = m;
    m_secondStart = s;
}

void NSStarmanLib::PatchTest::GetDateTimeStart(int* y, int* M, int* d, int* h, int* m, int* s)
{
    *y = m_yearStart;
    *M = m_monthStart;
    *d = m_dayStart;

    *h = m_hourStart;
    *m = m_minuteStart;
    *s = m_secondStart;
}

void NSStarmanLib::PatchTest::SetDateTimeEnd(int y, int M, int d, int h, int m, int s)
{
    m_yearEnd = y;
    m_monthEnd = M;
    m_dayEnd = d;

    m_hourEnd = h;
    m_minuteEnd = m;
    m_secondEnd = s;
}

void NSStarmanLib::PatchTest::GetDateTimeEnd(int* y, int* M, int* d, int* h, int* m, int* s)
{
    *y = m_yearEnd;
    *M = m_monthEnd;
    *d = m_dayEnd;

    *h = m_hourEnd;
    *m = m_minuteEnd;
    *s = m_secondEnd;
}

void NSStarmanLib::PatchTest::SetState(const eState arg)
{
    m_eState = arg;
}

PatchTest::eState NSStarmanLib::PatchTest::GetState() const
{
    return m_eState;
}

void NSStarmanLib::PatchTest::SetResult(const eResult result)
{
    m_eResult = result;
}

PatchTest::eResult NSStarmanLib::PatchTest::GetResult() const
{
    return m_eResult;
}

void NSStarmanLib::PatchItemInfo::SetName(const std::wstring arg)
{
    m_name = arg;
}

std::wstring NSStarmanLib::PatchItemInfo::GetName() const
{
    return m_name;
}

void NSStarmanLib::PatchItemInfo::SetItemId(const std::wstring& arg)
{
    m_itemId = arg;
}

std::wstring NSStarmanLib::PatchItemInfo::GetItemId() const
{
    return m_itemId;
}

void NSStarmanLib::PatchItemInfo::SetPoison(const bool arg)
{
    m_poison = arg;
}

bool NSStarmanLib::PatchItemInfo::GetPoison() const
{
    return m_poison;
}

void NSStarmanLib::PatchItemInfo::SetTryNum(const int arg)
{
    m_tryNum = arg;
}

int NSStarmanLib::PatchItemInfo::GetTryNum()
{
    return m_tryNum;
}

void NSStarmanLib::PatchItemInfo::SetAccurate(const float arg)
{
    m_accurate = arg;
}

float NSStarmanLib::PatchItemInfo::GetAccurate() const
{
    return m_accurate;
}
