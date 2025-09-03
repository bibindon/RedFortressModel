#include "CppUnitTest.h"
#include "../Model/ItemManager.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSModel;

namespace StarmanLibTest
{

    TEST_CLASS(ItemManagerTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            ItemManager* obj = ItemManager::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            ItemManager* obj = ItemManager::GetObj();
            obj->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            ItemManager* obj = ItemManager::GetObj();
            obj->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));
            ItemDef itemDef = obj->GetItemDef(L"sumaho");
            Assert::AreEqual(itemDef.GetImagePath() == _T("res\\\\image\\\\sumaho.png"), true);
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod04)
        {
            ItemManager* obj = ItemManager::GetObj();
            obj->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));
            ItemDef itemDef = obj->GetItemDef(L"mu-rugai");
            Assert::AreEqual(itemDef.GetImagePath() == _T("res\\\\image\\\\mu-rugai.png"), true);
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod05)
        {
            ItemManager* obj = ItemManager::GetObj();
            obj->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));
            ItemDef itemDef = obj->GetItemDef(L"mu-rugai");
            Assert::AreEqual(itemDef.GetType() == ItemDef::ItemType::FOOD, true);
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod06)
        {
            ItemManager* obj = ItemManager::GetObj();
            obj->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));
            ItemDef itemDef = obj->GetItemDef(L"mu-rugai");
            Assert::AreEqual(itemDef.GetWater(), 0.f);
            ItemManager::Destroy();
        }
    };
}
