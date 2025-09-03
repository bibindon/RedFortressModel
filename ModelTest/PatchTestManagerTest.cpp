#include "CppUnitTest.h"

#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>
#include "../Model/PatchTestManager.h"
#include "../Model/PowereggDateTime.h"
#include "../Model/ItemManager.h"
#include "../Model/Inventory.h"
#include "../Model/Storehouse.h"
#include "../Model/CraftInfoManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(PatchTestManagerTest)
    {
    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            ItemManager* itemManager = ItemManager::GetObj();
            itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

            Inventory* inventory = Inventory::GetObj();
            inventory->Init(_T("..\\StarmanLibTest\\inventory.csv"));

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            storehouseManager->Init(_T("..\\StarmanLibTest\\storehouseListOrigin.csv"));

            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
            craftInfoManager->Init(_T("..\\StarmanLibTest\\craftDef.csv"));
        }

        TEST_METHOD_CLEANUP(CleanUp)
        {
            CraftInfoManager::Destroy();
            StorehouseManager::Destroy();
            Inventory::Destroy();
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod01)
        {
            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            Assert::AreEqual(true, obj != nullptr);
        }

        TEST_METHOD(TestMethod02)
        {
            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));

            auto que = obj->GetQueue();
            Assert::AreEqual(true, que.empty());

            auto resultList = obj->GetResultList(L"unknownPlant1");
            Assert::AreEqual(true, resultList.empty());
        }

        TEST_METHOD(TestMethod03)
        {
            {
                PowereggDateTime* obj = PowereggDateTime::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\datetime.csv"));
            }

            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));

            obj->QueuePatchTest(L"unknownPlant1");

            auto que = obj->GetQueue();
            Assert::AreEqual(1, (int)que.size());

            auto resultList = obj->GetResultList(L"unknownPlant1");
            Assert::AreEqual(true, resultList.empty());
        }

        TEST_METHOD(TestMethod04)
        {
            PowereggDateTime* datetime = PowereggDateTime::GetObj();
            datetime->Init(_T("..\\StarmanLibTest\\datetime.csv"));

            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));

            obj->QueuePatchTest(L"unknownPlant1");
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 2, 0, 0);
            obj->Update();

            auto que = obj->GetQueue();
            Assert::AreEqual(1, (int)que.size());

            Assert::AreEqual(true, que.at(0).GetState() == PatchTest::eState::FINISHED);

            auto resultList = obj->GetResultList(L"unknownPlant1");
            Assert::AreEqual(1, (int)resultList.size());
        }

        TEST_METHOD(TestMethod05)
        {
            PowereggDateTime* datetime = PowereggDateTime::GetObj();
            datetime->Init(_T("..\\StarmanLibTest\\datetime.csv"));

            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));

            obj->QueuePatchTest(L"unknownPlant1");
            obj->QueuePatchTest(L"unknownPlant1");
            obj->QueuePatchTest(L"unknownPlant2");
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            obj->QueuePatchTest(L"unknownPlant3");
            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            auto que = obj->GetQueue();
            Assert::AreEqual(4, (int)que.size());

            Assert::AreEqual(true, que.at(0).GetState() == PatchTest::eState::FINISHED);
            Assert::AreEqual(true, que.at(1).GetState() == PatchTest::eState::FINISHED);
            Assert::AreEqual(true, que.at(2).GetState() == PatchTest::eState::STARTED);
            Assert::AreEqual(true, que.at(3).GetState() == PatchTest::eState::NOT_START);

            auto resultList1 = obj->GetResultList(L"unknownPlant1");
            Assert::AreEqual(2, (int)resultList1.size());

            auto resultList2 = obj->GetResultList(L"unknownPlant2");
            Assert::AreEqual(0, (int)resultList2.size());

            auto resultList3 = obj->GetResultList(L"unknownPlant3");
            Assert::AreEqual(0, (int)resultList3.size());
        }

        // Init関数で落ちないことを確認するテスト
        TEST_METHOD(TestMethod05_2)
        {
            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"),
                      _T("..\\StarmanLibTest\\patchTestInfoSave.csv"),
                      _T("..\\StarmanLibTest\\patchTestQueSave.csv"));
        }

        // Init関数でデータが読み込まれていることを確認する
        TEST_METHOD(TestMethod05_3)
        {
            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"),
                      _T("..\\StarmanLibTest\\patchTestInfoSave.csv"),
                      _T("..\\StarmanLibTest\\patchTestQueSave.csv"));

            auto que = obj->GetQueue();
            Assert::AreEqual(true, que.size() != 0);
        }

        // Save関数で落ちないことを確認するテスト
        TEST_METHOD(TestMethod06)
        {
            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));
            obj->Save(_T("..\\StarmanLibTest\\patchTestInfoSave2.csv"),
                      _T("..\\StarmanLibTest\\patchTestQueSave2.csv"));
        }

        // パッチテストを行ってからSaveするテスト
        TEST_METHOD(TestMethod07)
        {
            PowereggDateTime* datetime = PowereggDateTime::GetObj();
            datetime->Init(_T("..\\StarmanLibTest\\datetime.csv"));

            PatchTestManager::Destroy();
            PatchTestManager* obj = PatchTestManager::Get();
            obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));

            obj->QueuePatchTest(L"unknownPlant1");
            obj->QueuePatchTest(L"unknownPlant1");
            obj->QueuePatchTest(L"unknownPlant2");
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            obj->QueuePatchTest(L"unknownPlant3");
            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            datetime->IncreaseDateTime(0, 0, 0, 31, 0);
            obj->Update();

            obj->Save(_T("..\\StarmanLibTest\\patchTestInfoSave2.csv"),
                      _T("..\\StarmanLibTest\\patchTestQueSave2.csv"));
        }

        // Saveしたデータを読み取るテスト
        TEST_METHOD(TestMethod08)
        {
            {
                PowereggDateTime* datetime = PowereggDateTime::GetObj();
                datetime->Init(_T("..\\StarmanLibTest\\datetime.csv"));

                PatchTestManager::Destroy();
                PatchTestManager* obj = PatchTestManager::Get();
                obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"), _T(""), _T(""));

                obj->QueuePatchTest(L"unknownPlant1");
                obj->QueuePatchTest(L"unknownPlant1");
                obj->QueuePatchTest(L"unknownPlant2");
                obj->Update();

                datetime->IncreaseDateTime(0, 0, 0, 31, 0);
                obj->Update();

                datetime->IncreaseDateTime(0, 0, 0, 31, 0);
                obj->Update();

                obj->QueuePatchTest(L"unknownPlant3");
                datetime->IncreaseDateTime(0, 0, 0, 31, 0);
                obj->Update();

                datetime->IncreaseDateTime(0, 0, 0, 31, 0);
                obj->Update();

                obj->Save(_T("..\\StarmanLibTest\\patchTestInfoSave2.csv"),
                          _T("..\\StarmanLibTest\\patchTestQueSave2.csv"));
            }

            {
                PatchTestManager* obj = PatchTestManager::Get();
                obj->Init(_T("..\\StarmanLibTest\\patchTestOrigin.csv"),
                          _T("..\\StarmanLibTest\\patchTestInfoSave2.csv"),
                          _T("..\\StarmanLibTest\\patchTestQueSave2.csv"));

                auto que = obj->GetQueue();
                Assert::AreEqual(4, (int)que.size());
            }
        }
    };
}
