#pragma once

#include <string>

namespace NSStarmanLib
{

enum class eBaseType
{
    Precision = 0,
    DirectNex,
};

// 活動拠点情報
// 
// Configというクラス名にして、活動拠点以外の様々な設定値を扱えるようにするかもしれない。
class ActivityBase
{
public:

    static ActivityBase* Get();

    void Init(const std::wstring& csvFile);
    void Save(const std::wstring& csvFile);
    void SetBaseType(const eBaseType arg);
    eBaseType GetBaseType() const;
    void Finalize();

    // 船着き場の座標
    // クラフトされたイカダが配置される場所
    void GetPierPos(const eBaseType baseType, float* x, float* y, float* z);

    // イカダが船着き場にあるか。5メートル以内にある場合は、イカダがあると見なされる
    bool CheckRaftNearPier(const float x, const float y, const float z);

    // 現在の活動拠点にイカダがあるか
    bool CheckRaftExist();

    bool GetHanawa() const { return m_bHanawa; }
    void SetHanawa(const bool hanawa) { m_bHanawa = hanawa; }

private:

    ActivityBase();

    static ActivityBase* m_obj;

    eBaseType m_eBaseType = eBaseType::Precision;

    bool m_bHanawa = false;

};
}

