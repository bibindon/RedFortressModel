#include "CppUnitTest.h"
#include "../Model/MapObjManager.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(MapObjManagerTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            Assert::AreEqual(true, obj != nullptr);
            MapObjManager::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs = obj->GetMapObjList(-290, 550);
            Assert::AreEqual(true, vs.size() != 0);
            MapObjManager::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs = obj->GetMapObjList(-150, 550);
            Assert::AreEqual(3, (int)vs.size());
            MapObjManager::Destroy();
        }

        TEST_METHOD(TestMethod04)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs = obj->GetMapObjList(-150, 550);
            Assert::AreEqual(3, (int)vs.size());

            vs = obj->GetMapObjList(-150, 550);
            Assert::AreEqual(3, (int)vs.size());
            MapObjManager::Destroy();
        }

        TEST_METHOD(TestMethod05)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs = obj->GetMapObjList(-150, 550);
            obj->SetShow(-2, 5, 1, true);

             vs = obj->GetMapObjList(-150, 550);
            Assert::AreEqual(3, (int)vs.size());
            MapObjManager::Destroy();
        }

        TEST_METHOD(TestMethod06)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs;
            obj->GetMapObjListShow(-150, 550, &vs);
            Assert::AreEqual(3, (int)vs.size());

            obj->SetShow(-2, 5, 1, true);

            obj->GetMapObjListShow(-150, 550, &vs);
            Assert::AreEqual(2, (int)vs.size());
            MapObjManager::Destroy();
        }

        TEST_METHOD(TestMethod07)
        {
            MapObjManager* obj = MapObjManager::GetObj();
            obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs;
            obj->GetMapObjListHide(-150, 550, &vs);
            Assert::AreEqual(0, (int)vs.size());

            obj->SetShow(-2, 5, 1, true);

            obj->GetMapObjListHide(0, 550, &vs);
            Assert::AreEqual(1, (int)vs.size());
            MapObjManager::Destroy();
        }

        // バイナリデータを保存して読み込めるかをテスト
        TEST_METHOD(TestMethod08)
        {
            {
                MapObjManager* obj = MapObjManager::GetObj();
                obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));
                obj->SaveWithBinary(_T("..\\ModelTest\\map_obj2.bin"));
                MapObjManager::Destroy();
            }

            MapObjManager* obj = MapObjManager::GetObj();
            obj->InitWithBinary(_T("..\\ModelTest\\map_obj2.bin"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs;
            auto mapObjList = obj->GetMapObjList(-430, 405);

            Assert::AreEqual<size_t>(27, mapObjList.size());
            MapObjManager::Destroy();
        }

        // マップデータを変更して、バイナリデータを保存して読み込み、マップデータの変更が反映されているかをテスト
        TEST_METHOD(TestMethod09)
        {
            {
                MapObjManager* obj = MapObjManager::GetObj();
                obj->Init(_T("..\\ModelTest\\map_obj.csv"), _T("..\\ModelTest\\map_obj_type.csv"));

                // 1件目だけ書き換える
                obj->SetVisible(-2, 5, 1, false);
                obj->SaveWithBinary(_T("..\\ModelTest\\map_obj3.bin"));
                MapObjManager::Destroy();
            }

            MapObjManager* obj = MapObjManager::GetObj();
            obj->InitWithBinary(_T("..\\ModelTest\\map_obj3.bin"), _T("..\\ModelTest\\map_obj_type.csv"));
            std::vector<stMapObj> vs;
            auto mapObjList = obj->GetMapObjList(-285, 550);

            auto it = std::find_if(mapObjList.begin(), mapObjList.end(), [](stMapObj& item) { return item.m_id == 1; });

            Assert::IsTrue(it != mapObjList.end());
            Assert::IsTrue(it->m_visible == false);

            MapObjManager::Destroy();
        }
    };
}
