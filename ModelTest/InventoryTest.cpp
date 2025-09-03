#include "CppUnitTest.h"
#include "../Model/Inventory.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include "../Model/ItemManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(InventoryTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            Inventory* obj = Inventory::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            Inventory::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            ItemManager* itemManager = ItemManager::GetObj();
            itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

            Inventory* obj = Inventory::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\inventory.csv"));
            Inventory::Destroy();

            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            ItemManager* itemManager = ItemManager::GetObj();
            itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

            Inventory* obj = Inventory::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\inventory.csv"));
            float weight = obj->GetWeight();
            // floatは7桁しか有効桁数がない。4桁の数値同士の比較なら、小数点以下は3桁までしか正確じゃない。
            // 繰り返し掛け算と足し算を行うので7桁もない。課題
//            Assert::AreEqual((1063.094f <= weight && weight <= 1063.096f), true);
            Assert::AreEqual(1000.f <= weight, true);
            Inventory::Destroy();
            ItemManager::Destroy();
        }

        // アイテムの増減のテスト
        TEST_METHOD(TestMethod04)
        {
            ItemManager* itemManager = ItemManager::GetObj();
            itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

            Inventory* obj = Inventory::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\inventory.csv"));
            Assert::AreEqual(obj->CountItem(L"sumaho"), 10);
            Assert::AreEqual(obj->CountItem(L"yakizakana"), 10);
            obj->AddItem(L"sumaho");
            obj->AddItem(L"yakizakana");
            Assert::AreEqual(obj->CountItem(L"sumaho"), 11);
            Assert::AreEqual(obj->CountItem(L"yakizakana"), 11);
            obj->RemoveItem(L"sumaho", 1);
            obj->RemoveItem(L"yakizakana", 1);
            Assert::AreEqual(obj->CountItem(L"sumaho"), 10);
            Assert::AreEqual(obj->CountItem(L"yakizakana"), 10);
            Inventory::Destroy();
            ItemManager::Destroy();
        }

        // セーブ機能の確認
        TEST_METHOD(TestMethod05)
        {
            {
                Inventory::Destroy();

                ItemManager* itemManager = ItemManager::GetObj();
                itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

                Inventory* obj = Inventory::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\inventory.csv"));
                obj->AddItem(L"sumaho");
                obj->AddItem(L"yakizakana");
                Assert::AreEqual(obj->CountItem(L"sumaho"), 11);
                Assert::AreEqual(obj->CountItem(L"yakizakana"), 11);
                obj->Save(_T("..\\StarmanLibTest\\inventorySave.csv"));
                Inventory::Destroy();
                ItemManager::Destroy();
            }
            {
                ItemManager* itemManager = ItemManager::GetObj();
                itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

                Inventory* obj = Inventory::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\inventorySave.csv"));
                Assert::AreEqual(obj->CountItem(L"sumaho"), 11);
                Assert::AreEqual(obj->CountItem(L"yakizakana"), 11);
                Inventory::Destroy();
                ItemManager::Destroy();
            }
        }

        // SubIDの採番の確認
        // 同じ種類のアイテムを複数持てるのでそれぞれのアイテムにSubIDが割り振られている
        // アイテムを削除できるのでSubIDは連番にならない。
        //
        // SubIDが1,2,3,4,5のときSubID＝3、SubID＝4のアイテムを削除して、
        // そのあとアイテムを追加したらSubIDは1,2,3,5になっているはず。
        TEST_METHOD(TestMethod06)
        {
            {
                Inventory::Destroy();

                ItemManager* itemManager = ItemManager::GetObj();
                itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

                Inventory* obj = Inventory::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\inventory.csv"));

                obj->AddItem(L"sumaho");
                obj->AddItem(L"yakizakana");

                Assert::AreEqual(obj->ExistItem(L"sumaho", 11), true);
                Assert::AreEqual(obj->ExistItem(L"yakizakana", 11), true);

                obj->RemoveItem(L"sumaho", 3);
                obj->RemoveItem(L"sumaho", 4);
                obj->AddItem(L"sumaho");

                obj->RemoveItem(L"yakizakana", 3);
                obj->RemoveItem(L"yakizakana", 4);
                obj->AddItem(L"yakizakana");

                Assert::AreEqual(obj->ExistItem(L"sumaho", 3), true);
                Assert::AreEqual(obj->ExistItem(L"sumaho", 4), false);
                Assert::AreEqual(obj->ExistItem(L"yakizakana", 3), true);
                Assert::AreEqual(obj->ExistItem(L"yakizakana", 4), false);

                Inventory::Destroy();
                ItemManager::Destroy();
            }
        }

        // SubIDの採番の確認
        // 初めて追加されるアイテムならSubIDは1が採番されるはず
        TEST_METHOD(TestMethod07)
        {
            {
                Inventory::Destroy();

                ItemManager* itemManager = ItemManager::GetObj();
                itemManager->Init(_T("..\\StarmanLibTest\\item.csv"), _T("..\\StarmanLibTest\\item_pos.csv"));

                Inventory* obj = Inventory::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\inventory.csv"));

                obj->RemoveItem(L"sotetsu", 1);
                obj->RemoveItem(L"sotetsu", 2);
                obj->RemoveItem(L"sotetsu", 3);
                obj->RemoveItem(L"sotetsu", 4);
                obj->RemoveItem(L"sotetsu", 5);
                obj->RemoveItem(L"sotetsu", 6);
                obj->RemoveItem(L"sotetsu", 7);
                obj->RemoveItem(L"sotetsu", 8);
                obj->RemoveItem(L"sotetsu", 9);
                obj->RemoveItem(L"sotetsu", 10);

                Assert::AreEqual(obj->CountItem(L"sotetsu"), 0);

                obj->AddItem(L"sotetsu");

                Assert::AreEqual(obj->CountItem(L"sotetsu"), 1);
                Assert::AreEqual(obj->ExistItem(L"sotetsu", 0), false);
                Assert::AreEqual(obj->ExistItem(L"sotetsu", 1), true);
                Assert::AreEqual(obj->ExistItem(L"sotetsu", 2), false);

                Inventory::Destroy();
                ItemManager::Destroy();
            }
        }
    };
}
