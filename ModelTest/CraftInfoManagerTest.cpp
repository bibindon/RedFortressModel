#include "CppUnitTest.h"
#include "../Model/CraftInfoManager.h"
#include "../Model/Inventory.h"
#include "../Model/ItemManager.h"
#include "../Model/Storehouse.h"

#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSModel;

namespace StarmanLibTest
{

    TEST_CLASS(CraftInfoManagerTest)
    {
    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            ItemManager* itemManager = ItemManager::GetObj();
            itemManager->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));

            Inventory* inventory = Inventory::GetObj();
            inventory->Init(_T("..\\ModelTest\\inventory.csv"));

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            storehouseManager->Init(_T("..\\ModelTest\\storehouseListOrigin.csv"));
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
            CraftInfoManager* obj = CraftInfoManager::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            CraftInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            CraftInfoManager* obj = CraftInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\craftDef.csv"));
            Assert::AreEqual(obj != nullptr, true);
            CraftInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            CraftInfoManager* obj = CraftInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\craftDef.csv"));
            std::vector<CraftOutput> work = obj->GetCraftItemList();
            Assert::AreEqual((int)work.size(), 76);
            CraftInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod04)
        {
            CraftInfoManager* obj = CraftInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\craftDef.csv"));
            std::vector<CraftOutput> work = obj->GetCraftItemList();

            {
                std::wstring work_sz = work.at(0).GetItemId();
                Assert::AreEqual(work_sz == _T("raft"), true);

                int work_i = 0;

                work_i = work.at(0).GetNumber();
                Assert::AreEqual(work_i == 1, true);

                work_i = work.at(0).GetLevel();
                Assert::AreEqual(work_i == -1, true);
            }
            {
                std::wstring work_sz = work.at(1).GetItemId();
                Assert::AreEqual(work_sz == _T("raft1"), true);

                int work_i = 0;

                work_i = work.at(1).GetNumber();
                Assert::AreEqual(work_i == 1, true);

                work_i = work.at(1).GetLevel();
                Assert::AreEqual(work_i == 1, true);
            }
            {
                std::wstring work_sz = work.at(56).GetItemId();
                Assert::AreEqual(work_sz.c_str(), _T("spearForAtlatl5"));

                int work_i = 0;

                work_i = work.at(56).GetNumber();
                Assert::AreEqual(work_i, 1);

                work_i = work.at(56).GetLevel();
                Assert::AreEqual(work_i, 5);
            }
            CraftInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod05)
        {
            CraftInfoManager* obj = CraftInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\craftDef.csv"));
            std::vector<CraftOutput> work = obj->GetCraftItemList();

            {
                CraftInfo craftInfo = obj->GetCraftInfo(work.at(0));
                std::wstring work_sz;
                int work_i = 0;

                // 素材１
                work_sz = craftInfo.GetCraftMaterialDef(0).GetName();
                Assert::AreEqual(work_sz == _T("細い木の幹"), true);

                work_i = craftInfo.GetCraftMaterialDef(0).GetNumber();
                Assert::AreEqual(work_i == 25, true);

                work_i = craftInfo.GetCraftMaterialDef(0).GetLevel();
                Assert::AreEqual(work_i == -1, true);

                // 素材２
                work_sz = craftInfo.GetCraftMaterialDef(1).GetName();
                Assert::AreEqual(work_sz == _T("ツタ"), true);

                work_i = craftInfo.GetCraftMaterialDef(1).GetNumber();
                Assert::AreEqual(work_i == 25, true);

                work_i = craftInfo.GetCraftMaterialDef(1).GetLevel();
                Assert::AreEqual(work_i == -1, true);
            }
            {
                CraftInfo craftInfo = obj->GetCraftInfo(work.at(56));
                std::wstring work_sz;
                int work_i = 0;

                // 素材１
                work_sz = craftInfo.GetCraftMaterialDef(0).GetName();
                Assert::AreEqual(work_sz == _T("木の枝"), true);

                work_i = craftInfo.GetCraftMaterialDef(0).GetNumber();
                Assert::AreEqual(work_i == 1, true);

                work_i = craftInfo.GetCraftMaterialDef(0).GetLevel();
                Assert::AreEqual(work_i == -1, true);

                // 素材２
                work_sz = craftInfo.GetCraftMaterialDef(1).GetName();
                Assert::AreEqual(work_sz == _T("いい形の石（槍）"), true);

                work_i = craftInfo.GetCraftMaterialDef(1).GetNumber();
                Assert::AreEqual(work_i == 1, true);

                work_i = craftInfo.GetCraftMaterialDef(1).GetLevel();
                Assert::AreEqual(work_i == -1, true);

                // 素材３
                work_sz = craftInfo.GetCraftMaterialDef(2).GetName();
                Assert::AreEqual(work_sz == _T("ツタ"), true);

                work_i = craftInfo.GetCraftMaterialDef(2).GetNumber();
                Assert::AreEqual(work_i == 2, true);

                work_i = craftInfo.GetCraftMaterialDef(2).GetLevel();
                Assert::AreEqual(work_i == -1, true);
            }
            CraftInfoManager::Destroy();
        }
    };
}
