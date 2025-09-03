#include "MapObjManager.h"
#include <algorithm>
#include "Util.h"
#include <fstream>

NSStarmanLib::MapObjManager* NSStarmanLib::MapObjManager::obj = nullptr;

NSStarmanLib::MapObjManager* NSStarmanLib::MapObjManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new MapObjManager();
    }
    return obj;
}

void NSStarmanLib::MapObjManager::Init(const std::wstring& csvfile,
                                       const std::wstring& csvModelId,
                                       const bool decrypt)
{
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvfile, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            stMapObj work;

            float work_f = 0.f;

            work.m_id = std::stoi(vvs.at(i).at(0));

            work.m_modelId = std::stoi(vvs.at(i).at(1));

            work_f = std::stof(vvs.at(i).at(2));
            work.m_x = work_f;
            work.m_frameX = (int)work_f/100;

            work.m_y = std::stof(vvs.at(i).at(3));

            work_f = std::stof(vvs.at(i).at(4));
            work.m_z = work_f;
            work.m_frameZ = (int)work_f/100;

            work.m_yRot = std::stof(vvs.at(i).at(5));

            work.m_scale = std::stof(vvs.at(i).at(6));

            if (vvs.at(i).at(7) == _T("y"))
            {
                work.m_visible = true;
            }
            else
            {
                work.m_visible = false;
            }

            m_stMapObjMap[work.m_frameX][work.m_frameZ].emplace_back(work);
        }
    }
    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvModelId, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            m_XnameMap[std::stoi(vvs.at(i).at(0))] = vvs.at(i).at(1);
        }
    }
}

void NSStarmanLib::MapObjManager::InitWithBinary(const std::wstring& binFile,
                                                 const std::wstring& csvModelId,
                                                 const bool decrypt)
{
    {
        std::vector<NSStarmanLib::stMapObj> stMapObjList;

        std::ifstream inFile(binFile, std::ios::binary);
        if (!inFile.is_open())
        {
            throw std::exception();
        }

        size_t size = 0;

        // ベクターサイズを読み込む
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

        // ベクターサイズを設定
        stMapObjList.resize(size);

        // データ本体を読み込む
        inFile.read(reinterpret_cast<char*>(stMapObjList.data()),
                    static_cast<std::streamsize>(size) * sizeof(NSStarmanLib::stMapObj));

        inFile.close();

        for (auto it = stMapObjList.begin(); it != stMapObjList.end(); ++it)
        {
            // 表示済みフラグをオフにする
            // セーブしたときは木や草を表示していたかもしれないが、ゲームを再起動したのだから
            // 表示済みではない、ということ。
            it->m_show = false;

            m_stMapObjMap[it->m_frameX][it->m_frameZ].emplace_back(*it);
        }
    }

    {
        std::vector<std::vector<std::wstring>> vvs = Util::ReadFromCsv(csvModelId, decrypt);

        for (std::size_t i = 1; i < vvs.size(); ++i)
        {
            m_XnameMap[std::stoi(vvs.at(i).at(0))] = vvs.at(i).at(1);
        }
    }
}

void NSStarmanLib::MapObjManager::Save(const std::wstring& csvfile,
                                       const bool encrypt)
{
    std::vector<stMapObj> work;

    for (auto itX = m_stMapObjMap.begin(); itX != m_stMapObjMap.end(); ++itX)
    {
        for (auto itZ = itX->second.begin(); itZ != itX->second.end(); ++itZ)
        {
            for (int i = 0; i < (int)itZ->second.size(); ++i)
            {
                work.push_back(itZ->second.at(i));
            }
        }
    }

    std::sort(work.begin(), work.end(),
              [](const stMapObj& x1, const stMapObj& x2)
              {
                  return x1.m_id < x2.m_id;
              });

    std::vector<std::vector<std::wstring> > vvs;
    std::vector<std::wstring> vs;
    std::wstring work_str;

    vs.clear();
    vs.push_back(_T("ID"));
    vs.push_back(_T("モデルID"));
    vs.push_back(_T("X"));
    vs.push_back(_T("Y"));
    vs.push_back(_T("Z"));
    vs.push_back(_T("RotY"));
    vs.push_back(_T("Scale"));
    vs.push_back(_T("表示"));
    vvs.push_back(vs);

    for (int i = 0; i < (int)work.size(); ++i)
    {
        vs.clear();
        vs.push_back(std::to_wstring(work.at(i).m_id));
        vs.push_back(std::to_wstring(work.at(i).m_modelId));
        vs.push_back(std::to_wstring(work.at(i).m_x));
        vs.push_back(std::to_wstring(work.at(i).m_y));
        vs.push_back(std::to_wstring(work.at(i).m_z));
        vs.push_back(std::to_wstring(work.at(i).m_yRot));
        vs.push_back(std::to_wstring(work.at(i).m_scale));
        if (work.at(i).m_visible)
        {
            vs.push_back(_T("y"));
        }
        else
        {
            vs.push_back(_T("n"));
        }

        vvs.push_back(vs);
    }

    Util::WriteToCsv(csvfile, vvs, encrypt);
}

void NSStarmanLib::MapObjManager::SaveWithBinary(const std::wstring& binFile)
{
    std::vector<stMapObj> stMapObjList;

    for (auto itX = m_stMapObjMap.begin(); itX != m_stMapObjMap.end(); ++itX)
    {
        for (auto itZ = itX->second.begin(); itZ != itX->second.end(); ++itZ)
        {
            for (int i = 0; i < (int)itZ->second.size(); ++i)
            {
                stMapObjList.push_back(itZ->second.at(i));
            }
        }
    }

    std::sort(stMapObjList.begin(), stMapObjList.end(),
              [](const stMapObj& x1, const stMapObj& x2)
              {
                  return x1.m_id < x2.m_id;
              });

    std::ofstream outFile(binFile, std::ios::binary);
    if (outFile.is_open())
    {
        size_t size = stMapObjList.size();

        // ベクターサイズを書き込む
        outFile.write(reinterpret_cast<char*>(&size), sizeof(size));

        // データ本体を書き込む
        outFile.write(reinterpret_cast<char*>(stMapObjList.data()),
                      static_cast<std::streamsize>(size) * sizeof(NSStarmanLib::stMapObj));

        outFile.close();
    }
}

void NSStarmanLib::MapObjManager::Destroy()
{
    delete MapObjManager::obj;
    MapObjManager::obj = nullptr;
}

std::vector<NSStarmanLib::stMapObj>
NSStarmanLib::MapObjManager::GetMapObjList(const float playerX,
                                           const float playerZ)
{
    std::vector<stMapObj> result;
    // プレイヤーのX座標が   0 ~  99 なら格子Xは0
    // プレイヤーのX座標が 100 ~ 199 なら格子Xは1

    int frameX = (int)playerX / 100;
    int frameZ = (int)playerZ / 100;

    std::vector<stMapObj> work;
    work = m_stMapObjMap[frameX][frameZ];
    result = work;

    //-----------------------------------------------------------------------------
    // 左上、上、右上、左、右、左下、下、右下の格子についても追加
    //-----------------------------------------------------------------------------

    // 左上
    work = m_stMapObjMap[frameX-1][frameZ+1];
    result.insert(result.end(), work.begin(), work.end());

    // 上
    work = m_stMapObjMap[frameX][frameZ+1];
    result.insert(result.end(), work.begin(), work.end());

    // 右上
    work = m_stMapObjMap[frameX+1][frameZ+1];
    result.insert(result.end(), work.begin(), work.end());

    // 左
    work = m_stMapObjMap[frameX-1][frameZ];
    result.insert(result.end(), work.begin(), work.end());

    // 右
    work = m_stMapObjMap[frameX+1][frameZ];
    result.insert(result.end(), work.begin(), work.end());

    // 左下
    work = m_stMapObjMap[frameX-1][frameZ-1];
    result.insert(result.end(), work.begin(), work.end());

    // 下
    work = m_stMapObjMap[frameX][frameZ-1];
    result.insert(result.end(), work.begin(), work.end());

    // 右下
    work = m_stMapObjMap[frameX+1][frameZ-1];
    result.insert(result.end(), work.begin(), work.end());

    return result;
}

std::vector<NSStarmanLib::stMapObj>
NSStarmanLib::MapObjManager::GetMapObjListR(const float playerX,
                                            const float playerZ,
                                            const float r)
{
    std::vector<stMapObj> result;
    // プレイヤーのX座標が   0 ~  99 なら格子Xは0
    // プレイヤーのX座標が 100 ~ 199 なら格子Xは1

    int frameX = (int)playerX / 100;
    int frameZ = (int)playerZ / 100;

    std::vector<stMapObj> work;
    work = m_stMapObjMap[frameX][frameZ];

    std::vector<stMapObj> work2;
    float r2 = r * r;
    for (int i = 0; i < (int)work.size(); ++i)
    {
        float x3 = work.at(i).m_x;
        float z3 = work.at(i).m_z;
        float r3 = (playerX - x3) * (playerX - x3) + (playerZ - z3) * (playerZ - z3);
        if (r2 > r3)
        {
            work2.push_back(work.at(i));
        }
    }

    return work2;
}

void NSStarmanLib::MapObjManager::GetMapObjListShow(const float playerX,
                                                    const float playerZ,
                                                    std::vector<stMapObj>* needShow)
{
    needShow->clear();

    std::vector<stMapObj> work = GetMapObjList(playerX, playerZ);

    for (int i = 0; i < (int)work.size(); ++i)
    {
        if (work.at(i).m_show == false && work.at(i).m_visible)
        {
            needShow->push_back(work.at(i));
        }
    }
}

void NSStarmanLib::MapObjManager::GetMapObjListHide(const float playerX,
                                                    const float playerZ,
                                                    std::vector<stMapObj>* needHide)
{
    int frameX = (int)playerX / 100;
    int frameZ = (int)playerZ / 100;

    // 描画範囲外オブジェクトについては、プレイヤーのいる格子から二つ離れた格子だけ調べればよい気がする
    // □□□□□□□□□□□□
    // □□□□□□□□□□□□
    // □□□■■■■■□□□□ (1)
    // □□□■□□□■□□□□ (2)
    // □□□■□★□■□□□□ (3)
    // □□□■□□□■□□□□ (4)
    // □□□■■■■■□□□□ (5)
    // □□□□□□□□□□□□
    //
    // 木を切ったとかで木を非表示にしたい場合については、プレイヤーとその周りについて調べる
    // □□□□□□□□□□
    // □□□■■■□□□□ (6)
    // □□□■★■□□□□ (7)
    // □□□■■■□□□□ (8)
    // □□□□□□□□□□

    struct XZ
    {
        int x = 0;
        int z = 0;
    };
    std::vector<XZ> target;
    XZ xz;

    // (1)
    xz.z = frameZ + 2;

    xz.x = frameX - 2;
    target.push_back(xz);

    xz.x = frameX - 1;
    target.push_back(xz);

    xz.x = frameX;
    target.push_back(xz);

    xz.x = frameX + 1;
    target.push_back(xz);

    xz.x = frameX + 2;
    target.push_back(xz);

    // (2)
    xz.z = frameZ + 1;
    xz.x = frameX - 2;
    target.push_back(xz);

    xz.x = frameX + 2;
    target.push_back(xz);

    // (3)
    xz.z = frameZ;
    xz.x = frameX - 2;
    target.push_back(xz);

    xz.x = frameX + 2;
    target.push_back(xz);

    // (4)
    xz.z = frameZ - 1;
    xz.x = frameX - 2;
    target.push_back(xz);

    xz.x = frameX + 2;
    target.push_back(xz);

    // (5)
    xz.z = frameZ - 2;

    xz.x = frameX - 2;
    target.push_back(xz);

    xz.x = frameX - 1;
    target.push_back(xz);

    xz.x = frameX;
    target.push_back(xz);

    xz.x = frameX + 1;
    target.push_back(xz);

    xz.x = frameX + 2;
    target.push_back(xz);

    for (int i = 0; i < (int)target.size(); ++i)
    {
        int x = target.at(i).x;
        int z = target.at(i).z;
        std::vector<stMapObj> list = m_stMapObjMap[x][z];
        for (int j = 0; j < (int)list.size(); ++j)
        {
            if (list.at(j).m_show)
            {
                needHide->push_back(list.at(j));
            }
        }
    }

    target.clear();

    // (6)
    xz.z = frameZ + 1;
    xz.x = frameX - 1;
    target.push_back(xz);

    xz.z = frameZ + 1;
    xz.x = frameX;
    target.push_back(xz);

    xz.z = frameZ + 1;
    xz.x = frameX + 1;
    target.push_back(xz);

    // (7)
    xz.z = frameZ;
    xz.x = frameX - 1;
    target.push_back(xz);

    xz.z = frameZ;
    xz.x = frameX;
    target.push_back(xz);

    xz.z = frameZ;
    xz.x = frameX + 1;
    target.push_back(xz);

    // (8)
    xz.z = frameZ - 1;
    xz.x = frameX - 1;
    target.push_back(xz);

    xz.z = frameZ - 1;
    xz.x = frameX;
    target.push_back(xz);

    xz.z = frameZ - 1;
    xz.x = frameX + 1;
    target.push_back(xz);

    for (int i = 0; i < (int)target.size(); ++i)
    {
        int x = target.at(i).x;
        int z = target.at(i).z;
        std::vector<stMapObj> list = m_stMapObjMap[x][z];
        for (int j = 0; j < (int)list.size(); ++j)
        {
            if (list.at(j).m_show && list.at(j).m_visible == false)
            {
                needHide->push_back(list.at(j));
            }
        }
    }
}

void NSStarmanLib::MapObjManager::SetShow(const int frame_x,
                                          const int frame_z,
                                          const int id,
                                          const bool show)
{
    std::vector<stMapObj>& work = m_stMapObjMap[frame_x][frame_z];
    auto it = std::find_if(work.begin(), work.end(),
                           [&](const stMapObj& item)
                           {
                               return item.m_id == id;
                           });
    if (it == work.end())
    {
        throw std::exception();
    }
    it->m_show = show;
}

void NSStarmanLib::MapObjManager::SetVisible(const int frame_x,
                                             const int frame_z,
                                             const int id,
                                             const bool visible)
{
    std::vector<stMapObj>& work = m_stMapObjMap[frame_x][frame_z];
    auto it = std::find_if(work.begin(), work.end(),
                           [&](const stMapObj& item)
                           {
                               return item.m_id == id;
                           });
    if (it == work.end())
    {
        throw std::exception();
    }
    it->m_visible = visible;
}

std::wstring NSStarmanLib::MapObjManager::GetModelName(const int id)
{
    return m_XnameMap.at(id);
}

int NSStarmanLib::MapObjManager::GetModelId(const std::wstring& name)
{
    int id = -1;
    for (auto it = m_XnameMap.begin(); it != m_XnameMap.end(); ++it)
    {
        if (it->second.find(name) != std::wstring::npos)
        {
            id = it->first;
            break;
        }
    }
    return id;
}

void NSStarmanLib::MapObjManager::ResetShow()
{
    for (auto& line : m_stMapObjMap)
    {
        for (auto& item : line.second)
        {
            for (auto& mapObj : item.second)
            {
                mapObj.m_show = false;
            }
        }
    }
}

