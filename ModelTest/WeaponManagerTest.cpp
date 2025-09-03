#include "CppUnitTest.h"
#include "../Model/WeaponManager.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include "../Model/ItemManager.h"
#include "../Model/Inventory.h"
#include "../Model/Storehouse.h"
#include "../Model/CraftInfoManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSModel;

namespace StarmanLibTest
{

    TEST_CLASS(WeaponManagerTest)
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

            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
            craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));
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
            WeaponManager* wm = WeaponManager::GetObj();
            WeaponManager::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            WeaponManager* wm = WeaponManager::GetObj();
            wm->Init(_T("..\\ModelTest\\weapon.csv"),
                     _T("..\\ModelTest\\weaponSave.csv"));

            WeaponManager::Destroy();
        }

        // Init関数による読み込みテスト
        TEST_METHOD(TestMethod03)
        {
            WeaponManager* wm = WeaponManager::GetObj();
            wm->Init(_T("..\\ModelTest\\weapon.csv"),
                     _T("..\\ModelTest\\weaponSave.csv"));

            std::wstring work;
            //------------------------------------------------
            work = wm->GetDetail2(L"w1");
            Assert::AreNotEqual(std::wstring::npos, work.find(_T("投げたり")));

            work = wm->GetDetail2(L"w10");
            Assert::AreNotEqual(std::wstring::npos, work.find(_T("そのことに気が付く。")));

            //------------------------------------------------
            work = wm->GetXfilename2(_T("w1"));
            Assert::AreEqual(_T("stone.x"), work.c_str());

            work = wm->GetXfilename2(_T("w10"));
            Assert::AreEqual(_T("spearForAtlatl.x"), work.c_str());

            //------------------------------------------------
            work = wm->GetImageName2(_T("w1"));
            Assert::AreEqual(_T("stone.png"), work.c_str());

            work = wm->GetImageName2(_T("w10"));
            Assert::AreEqual(_T("spearForAtlatl.png"), work.c_str());

            //------------------------------------------------
            work = wm->GetImageName2(_T("w1"));
            Assert::AreEqual(_T("stone.png"), work.c_str());

            work = wm->GetImageName2(_T("w10"));
            Assert::AreEqual(_T("spearForAtlatl.png"), work.c_str());

            double work_d = 0.0;
            //------------------------------------------------
            work_d = wm->GetWeight2(_T("w1"));
            Assert::AreEqual(0.5, work_d, 1e-10);

            work_d = wm->GetWeight2(_T("w10"));
            Assert::AreEqual(0.6, work_d, 1e-10);

            int work_i = 0;
            //------------------------------------------------
            work_i = wm->GetVolume2(_T("w1"));
            Assert::AreEqual(500, work_i);

            work_i = wm->GetVolume2(_T("w10"));
            Assert::AreEqual(2000, work_i);

            //------------------------------------------------
            work_i = wm->GetReinforceMax2(_T("w1"));
            Assert::AreEqual(0, work_i);

            work_i = wm->GetReinforceMax2(_T("w10"));
            Assert::AreEqual(5, work_i);

            //------------------------------------------------
            work_i = wm->GetReinforceMax2(_T("w1"));
            Assert::AreEqual(0, work_i);

            work_i = wm->GetReinforceMax2(_T("w10"));
            Assert::AreEqual(5, work_i);

            //------------------------------------------------
            work_i = wm->GetOwnDamage2(_T("w1"));
            Assert::AreEqual(10, work_i);

            work_i = wm->GetOwnDamage2(_T("w10"));
            Assert::AreEqual(5, work_i);

            bool work_b = false;

            //------------------------------------------------
            work_b = wm->GetIsShow2(_T("w1"));
            Assert::AreEqual(true, work_b);

            work_b = wm->GetIsShow2(_T("w10"));
            Assert::AreEqual(false, work_b);

            WeaponManager::Destroy();
        }

        // Init関数による読み込みテスト
        // 強化値を指定
        TEST_METHOD(TestMethod04)
        {
            WeaponManager* wm = WeaponManager::GetObj();
            wm->Init(_T("..\\ModelTest\\weapon.csv"),
                     _T("..\\ModelTest\\weaponSave.csv"));

            double work_d = 0.0;

            // 石は強化できないので強化値＋２の石は存在しない。
            // 取得しようとしたら例外が発生する
            Assert::ExpectException<std::exception>(
                [&]
                {
                    work_d = wm->GetAttackRate2(_T("w1"), 2);
                });

            //-------------------------------------------------
            work_d = wm->GetAttackRate2(_T("w1"), -1);
            Assert::AreEqual(3.0, work_d, 1e-10);

            work_d = wm->GetAttackRate2(_T("w10"), 3);
            Assert::AreEqual(2.0, work_d, 1e-10);

            //-------------------------------------------------
            work_d = wm->GetFlightDistance2(_T("w1"), -1);
            Assert::AreEqual(30.0, work_d, 1e-10);

            work_d = wm->GetFlightDistance2(_T("w4"), 3);
            Assert::AreEqual(6.0, work_d, 1e-10);

            work_d = wm->GetFlightDistance2(_T("w10"), 3);
            Assert::AreEqual(40.0, work_d, 1e-10);

            //-------------------------------------------------
            int work_i = 0;
            work_i = wm->GetDurabilityMax2(_T("w1"), -1);
            Assert::AreEqual(999, work_i);

            work_i = wm->GetDurabilityMax2(_T("w10"), 3);
            Assert::AreEqual(40, work_i);

            WeaponManager::Destroy();
        }
    };
}

