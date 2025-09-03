#pragma once
#include <string>
#include <vector>
#include <map>

namespace NSModel
{

class PatchTest
{
public:

    enum class eState
    {
        NOT_START,
        STARTED,
        FINISHED,
    };

    enum class eResult
    {
        NOT_YET,
        POISON,
        NOT_POISON,
    };

    void SetItemName(const std::wstring& arg);
    std::wstring GetItemName() const;

    void SetItemId(const std::wstring& arg);
    std::wstring GetItemId() const;

    // 依頼日時
    void SetDateTimeReq(const int y, const int M, const int d,
                        const int h, const int m, const int s);

    void GetDateTimeReq(int* y, int* M, int* d, int* h, int* m, int* s);

    // 開始日時
    void SetDateTimeStart(const int y, const int M, const int d,
                          const int h, const int m, const int s);

    void GetDateTimeStart(int* y, int* M, int* d, int* h, int* m, int* s);

    // 完了日時
    void SetDateTimeEnd(const int y, const int M, const int d,
                        const int h, const int m, const int s);

    void GetDateTimeEnd(int* y, int* M, int* d, int* h, int* m, int* s);

    void SetState(const eState arg);
    eState GetState() const;

    void SetResult(const eResult result);
    eResult GetResult() const;

private:

    std::wstring m_itemName;
    std::wstring m_itemId;
    eState m_eState = eState::NOT_START;
    eResult m_eResult = eResult::NOT_YET;

    //--------------------------------------------
    // 依頼日時
    //--------------------------------------------
    int m_yearReq = 0;

    // 0月は存在しないことに注意すること
    int m_monthReq = 0;

    // 0日は存在しないことに注意すること
    int m_dayReq = 0;

    int m_hourReq = 0;
    int m_minuteReq = 0;
    int m_secondReq = 0;

    //--------------------------------------------
    // 開始日時
    //--------------------------------------------
    int m_yearStart = 0;

    // 0月は存在しないことに注意すること
    int m_monthStart = 0;

    // 0日は存在しないことに注意すること
    int m_dayStart = 0;

    int m_hourStart = 0;
    int m_minuteStart = 0;
    int m_secondStart = 0;

    //--------------------------------------------
    // 完了日時
    //--------------------------------------------
    int m_yearEnd = 0;

    // 0月は存在しないことに注意すること
    int m_monthEnd = 0;

    // 0日は存在しないことに注意すること
    int m_dayEnd = 0;

    int m_hourEnd = 0;
    int m_minuteEnd = 0;
    int m_secondEnd = 0;

};

class PatchItemInfo
{
public:

    void SetName(const std::wstring arg);
    std::wstring GetName() const;

    void SetItemId(const std::wstring& arg);
    std::wstring GetItemId() const;

    void SetPoison(const bool arg);
    bool GetPoison() const;

    void SetTryNum(const int arg);
    int GetTryNum();

    void SetAccurate(const float arg);
    float GetAccurate() const;

private:

    std::wstring m_name;
    bool m_poison = false;

    // パッチテストされた回数
    int m_tryNum = 0;

    // 正解率
    float m_accurate = 0.f;

    std::wstring m_itemId;

};

class PatchTestManager
{
public:

    static PatchTestManager* Get();
    static void Destroy();
    void Init(const std::wstring& originFile,
              const std::wstring& saveFileInfo,
              const std::wstring& saveFileQue);

    void Save(const std::wstring& csvfileInfo,
              const std::wstring& csvfileQue);

    // 現実世界で1秒に一回呼ばれる想定
    void Update();

    bool QueuePatchTest(const std::wstring& patchTest);
    std::vector<PatchTest> GetQueue();

    // 直近の3回のテスト結果を取得できると便利かもしれない。
    // GUIで表示したい。
    std::vector<PatchTest> GetResultList(const std::wstring& id);

    std::vector<std::wstring> GetKeyList();

    std::wstring GetPoisonPlant();

private:

    static PatchTestManager* m_obj;

    // Queueと言いつつstd::vectorを使う。
    // std::queueを使うと先頭と末尾しかアクセスできないので普通に困る。
    std::vector<PatchTest> m_PatchTestQue;

    // アイテム情報リスト
    // 毒、毒じゃない、はあらかじめ決まっていない。
    // ゲーム開始時にランダムで決まる
    std::map<std::wstring, PatchItemInfo> m_infoMap;

    std::vector<std::wstring> m_keyList;
};
}

