#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// およそ100メートル以内の3Dモデルの情報を返す。
// 1秒に一回くらい呼ばれる想定
namespace NSStarmanLib
{

// 廃止予定
// マップデータをクラスで扱うのは重すぎた。

//class MapObj
//{
//public:
//
//    void SetId(const int id);
//    int GetId() const;
//
//    void SetFrameX(const int x);
//    int GetFrameX() const;
//
//    void SetFrameZ(const int z);
//    int GetFrameZ() const;
//
//    void SetModelId(const int arg);
//    int GetModelId() const;
//
//    void SetX(const float arg);
//    float GetX() const;
//
//    void SetY(const float arg);
//    float GetY() const;
//
//    void SetZ(const float arg);
//    float GetZ() const;
//
//    void SetRotY(const float arg);
//    float GetRotY() const;
//
//    void SetScale(const float arg);
//    float GetScale() const;
//
//    // 100メートル以上離れたら非表示にするが、それとは別に、木を切ったら、切った木を非表示にしたい。
//    // Show/Hide, Visible/Disableで言葉を使い分けることにする
//    void SetShow(const bool arg);
//    bool GetShow() const;
//
//    void SetVisible(const bool arg);
//    bool GetVisible() const;
//private:
//
//    int m_id = 0;
//
//    int m_frameX = 0;
//    int m_frameZ = 0;
//
//    int m_modelId;
//
//    float m_x = 0.f;
//    float m_y = 0.f;
//    float m_z = 0.f;
//
//    float m_yRot = 0.f;
//
//    float m_scale = 0.f;
//
//    bool m_show = false;
//    bool m_visible = false;
//};

// MapObjが大量に必要となるため構造体を使い高速化を計る
struct stMapObj
{
    int m_id = 0;

    int m_frameX = 0;
    int m_frameZ = 0;

    int m_modelId = 0;

    float m_x = 0.f;
    float m_y = 0.f;
    float m_z = 0.f;

    float m_yRot = 0.f;

    float m_scale = 0.f;

    bool m_show = false;
    bool m_visible = false;
};

// 全部調べたら大変負荷が重くなるので100メートルごとの格子を考える。
// マップを100メートルごとの格子で分割し、自分がいる格子とその周りの8つの格子だけを処理する
//
// 100メートル以上離れたら非表示にするが、それとは別に、木を切ったら、切った木を非表示にしたい。
// Show/Hide, Visible/Disableで言葉を使い分けることにする
class MapObjManager
{
public:

    static MapObjManager* GetObj();

    void Init(const std::wstring& csvfile,
              const std::wstring& csvModelId,
              const bool decrypt = false);

    // バイナリファイルで読み書きする
    void InitWithBinary(const std::wstring& binfile,
                        const std::wstring& csvModelId,
                        const bool decrypt = false);

    void Save(const std::wstring& csvfile,
              const bool encrypt = false);

    void SaveWithBinary(const std::wstring& binfile);

    static void Destroy();

    // 表示すべきオブジェクトを返す
    std::vector<stMapObj> GetMapObjList(const float playerX, const float playerZ);

    // 半径r以内のオブジェクトを返す。
    std::vector<stMapObj> GetMapObjListR(const float playerX, const float playerZ, const float r);

    // 表示すべきなのに表示されていないオブジェクトを返す
    void GetMapObjListShow(const float playerX,
                           const float playerZ,
                           std::vector<stMapObj>* needShow);

    // 表示すべきでないのに表示されているオブジェクトを返す
    void GetMapObjListHide(const float playerX,
                           const float playerZ,
                           std::vector<stMapObj>* needHide);

    // 100メートル以上離れたら非表示にするが、それとは別に、木を切ったら、切った木を非表示にしたい。
    // Show/Hide, Visible/Disableで言葉を使い分けることにする
    void SetShow(const int frame_x, const int frame_z, const int id, const bool show);

    void SetVisible(const int frame_x, const int frame_z, const int id, const bool visible);

    std::wstring GetModelName(const int id);
    int GetModelId(const std::wstring& name);

    // Show/Hideをすべてリセット
    void ResetShow();

private:

    static MapObjManager* obj;

    // m_mapObjMap[10][20] これは1000m ~ 1100m, 2000m ~ 2100mの範囲の3Ｄモデルのリストを意味する
//    std::unordered_map<int, std::unordered_map<int, std::vector<MapObj>>> m_mapObjMap;

    std::unordered_map<int, std::unordered_map<int, std::vector<stMapObj>>> m_stMapObjMap;

    // モデルの配置情報は巨大なためファイル名をそのまま扱うととてつもなく巨大になってしまう。
    // tree.x -> 1
    // rock.x -> 2
    // のような識別子を与えるようにする
    std::unordered_map<int, std::wstring> m_XnameMap;

    bool m_bBinData = false;
};

}

