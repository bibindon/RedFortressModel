#include "CppUnitTest.h"
#include "../Model/CraftSystem.h"
#include "../Model/ItemManager.h"
#include "../Model/Storehouse.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include "../Model/PowereggDateTime.h"
#include "../Model/Voyage.h"
#include "../Model/WeaponManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(CraftSystemTest)
    {
    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            ItemManager* itemManager = ItemManager::GetObj();
            itemManager->Init(_T("..\\ModelTest\\item.csv"), _T("..\\ModelTest\\item_pos.csv"));

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            storehouseManager->Init(_T("..\\ModelTest\\storehouseListOrigin.csv"));

            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
            craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();
            powereggDateTime->Init(_T("..\\ModelTest\\datetime.csv"));

            WeaponManager* wm = WeaponManager::GetObj();
            wm->Init(_T("..\\ModelTest\\weapon.csv"),
                     _T("..\\ModelTest\\weaponSave.csv"));
        }

        TEST_METHOD_CLEANUP(CleanUp)
        {
            WeaponManager::Destroy();
            PowereggDateTime::Destroy();
            CraftInfoManager::Destroy();
            StorehouseManager::Destroy();
            ItemManager::Destroy();
        }

        TEST_METHOD(TestMethod01)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            CraftSystem::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
            craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueue.csv"));
            Assert::AreEqual(obj != nullptr, true);
            CraftSystem::Destroy();
        }

        // 職人のレベルが取得できるかテスト
        TEST_METHOD(TestMethod03)
        {
            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
            craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueue.csv"));
            int result = false;
            result = obj->GetCraftsmanSkill(_T("raft"));
            Assert::AreEqual(result, -1);

            result = obj->GetCraftsmanSkill(_T("stick"));
            Assert::AreEqual(result, 1);

            CraftSystem::Destroy();
        }

        TEST_METHOD(TestMethod04)
        {
            CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
            craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueue.csv"));
            int result = 0;
            obj->SetCraftsmanSkill(_T("raft"), 1);
            result = obj->GetCraftsmanSkill(_T("raft"));
            Assert::AreEqual(1, result);

            obj->SetCraftsmanSkill(_T("stick"), 2);
            result = obj->GetCraftsmanSkill(_T("stick"));
            Assert::AreEqual(2, result);

            CraftSystem::Destroy();
        }

        // 職人のスキルが保存できていることを確認
        TEST_METHOD(TestMethod05)
        {
            {
                CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
                craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));

                CraftSystem* obj = CraftSystem::GetObj();
                obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                          _T("..\\ModelTest\\craftsmanQueue.csv"));

                bool result = false;
                obj->SetCraftsmanSkill(_T("raft"), 1);
                obj->SetCraftsmanSkill(_T("spearForAtlatl"), 1);
                obj->Save(_T("..\\ModelTest\\craftsmanSkillSave.csv"),
                          _T("..\\ModelTest\\craftsmanQueueSave.csv"));

                CraftSystem::Destroy();
                CraftInfoManager::Destroy();
            }
            {
                CraftInfoManager* craftInfoManager = CraftInfoManager::GetObj();
                craftInfoManager->Init(_T("..\\ModelTest\\craftDef.csv"));

                CraftSystem* obj = CraftSystem::GetObj();
                obj->Init(_T("..\\ModelTest\\craftsmanSkillSave.csv"),
                          _T("..\\ModelTest\\craftsmanQueueSave.csv"));

                int result = false;
                result = obj->GetCraftsmanSkill(_T("raft"));
                Assert::AreEqual(result, 1);

                result = obj->GetCraftsmanSkill(_T("spearForAtlatl"));
                Assert::AreEqual(result, 1);

                CraftSystem::Destroy();
                CraftInfoManager::Destroy();
            }
        }

        TEST_METHOD(TestMethod06)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueue.csv"));
            std::wstring work;
            obj->QueueCraftRequest(_T("raft"), &work, 1);

            CraftSystem::Destroy();
        }

        // クラフト用の素材がなくてキューイング失敗
        TEST_METHOD(TestMethod07)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            bool result = obj->QueueCraftRequest(_T("raft"), &work, 1);

            Assert::AreEqual(result, false);

            std::list<CraftRequest> craftRequestList;
            craftRequestList = obj->GetCraftRequestList();

            Assert::AreEqual((int)craftRequestList.size() == 0, true);

            CraftSystem::Destroy();
        }

        // キューイングしてスタート
        TEST_METHOD(TestMethod08)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            obj->QueueCraftRequest(_T("atlatl"), &work, 1);

            std::list<CraftRequest> craftRequestList;
            craftRequestList = obj->GetCraftRequestList();

            Assert::AreEqual((int)craftRequestList.size() == 1, true);
            Assert::AreEqual(craftRequestList.front().GetCrafting(), false);

            obj->UpdateCraftStatus();

            craftRequestList = obj->GetCraftRequestList();

            Assert::AreEqual((int)craftRequestList.size() == 1, true);
            Assert::AreEqual(craftRequestList.front().GetCrafting(), true);

            CraftSystem::Destroy();
        }

        // キューイングした時点でクラフト用素材がインベントリからなくなる
        TEST_METHOD(TestMethod09)
        {
            auto storehouse = StorehouseManager::Get()->GetCurrentActiveStorehouse();
            int temp = 0;

            temp = storehouse->CountItem(L"trunk");
            Assert::AreEqual(temp, 10);

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            std::list<CraftRequest> craftRequestList;
            craftRequestList = obj->GetCraftRequestList();

            Assert::AreEqual((int)craftRequestList.size() == 1, true);
            Assert::AreEqual(craftRequestList.front().GetCrafting(), false);

            temp = storehouse->CountItem(L"trunk");
            Assert::AreEqual(temp, 9);

            temp = storehouse->CountItem(L"stoneForSpear");
            Assert::AreEqual(temp, 9);

            obj->UpdateCraftStatus();

            craftRequestList = obj->GetCraftRequestList();

            Assert::AreEqual((int)craftRequestList.size() == 1, true);
            Assert::AreEqual(craftRequestList.front().GetCrafting(), true);

            temp = storehouse->CountItem(L"trunk");
            Assert::AreEqual(temp, 9);

            temp = storehouse->CountItem(L"stoneForSpear");
            Assert::AreEqual(temp, 9);

            CraftSystem::Destroy();
        }

        // キューイングしてスタートして完了する
        TEST_METHOD(TestMethod10)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));
            
            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            std::list<CraftRequest> craftRequestList;
            craftRequestList = obj->GetCraftRequestList();

            // クラフト要求がなくなっていること
            Assert::AreEqual(true, (int)craftRequestList.size() == 0);

            CraftSystem::Destroy();
        }

        // キューイングしてスタートして完了して倉庫に完成品が追加される
        TEST_METHOD(TestMethod11)
        {
            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            int work = 0;
            work = storehouse->CountItem(L"stoneSpear");

            Assert::AreEqual(work, 10);

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work2;
            obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear");

            Assert::AreEqual(work, 11);

            CraftSystem::Destroy();
        }

        // 複数キューイングしてスタートして完了して倉庫に完成品が現れる
        TEST_METHOD(TestMethod12)
        {
            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            int work = 0;
            work = storehouse->CountItem(L"stoneSpear");

            Assert::AreEqual(work, 10);

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work2;
            obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);
            obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear");

            Assert::AreEqual(work, 11);

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear1");

            Assert::AreEqual(work, 11);

            CraftSystem::Destroy();
        }

        // 複数キューイングしてスタートして完了して倉庫に完成品が現れる(2)
        TEST_METHOD(TestMethod13)
        {
            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            int work = 0;

            work = storehouse->CountItem(L"stoneSpear");
            Assert::AreEqual(10, work);

            work = storehouse->CountItem(L"bow");
            Assert::AreEqual(10, work);

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work2;
            obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);
            obj->QueueCraftRequest(_T("bow"), &work2, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear");
            Assert::AreEqual(work, 11);

            work = storehouse->CountItem(L"bow");
            Assert::AreEqual(work, 10);

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear");
            Assert::AreEqual(work, 11);

            work = storehouse->CountItem(L"bow");
            Assert::AreEqual(work, 11);

            CraftSystem::Destroy();
        }

        // 進捗度が取得できる
        TEST_METHOD(TestMethod14)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 12時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 0, 12, 0, 0);

            obj->UpdateCraftStatus();

            int progress = obj->GetProgress();

            Assert::AreEqual(progress, 50);

            CraftSystem::Destroy();
        }

        // 保存できる
        TEST_METHOD(TestMethod15)
        {
            {
                CraftSystem* obj = CraftSystem::GetObj();
                obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                          _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

                std::wstring work;
                obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

                obj->UpdateCraftStatus();

                PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

                // 12時間、時を進める
                powereggDateTime->IncreaseDateTime(0, 0, 12, 0, 0);

                obj->UpdateCraftStatus();

                int progress = obj->GetProgress();

                Assert::AreEqual(progress, 50);
                obj->Save(_T("..\\ModelTest\\craftsmanSkillSave.csv"),
                          _T("..\\ModelTest\\craftsmanQueueSave.csv"));

                CraftSystem::Destroy();
            }
            {
                CraftSystem* obj = CraftSystem::GetObj();
                obj->Init(_T("..\\ModelTest\\craftsmanSkillSave.csv"),
                          _T("..\\ModelTest\\craftsmanQueueSave.csv"));

                size_t work = 0;

                work = obj->GetCraftRequestList().size();
                Assert::AreEqual<size_t>(work, 1u);

                work = obj->GetProgress();
                Assert::AreEqual<size_t>(work, 50u);

                CraftRequest craftRequest = obj->GetCraftRequestList().front();
                Assert::AreEqual(craftRequest.GetCrafting(), true);

                Assert::AreEqual(craftRequest.GetStartHour(), 33);
                Assert::AreEqual(craftRequest.GetStartMinute(), 44);
                Assert::AreEqual(craftRequest.GetStartSecond(), 55);

                Assert::AreEqual(craftRequest.GetFinishHour(), 33);
                Assert::AreEqual(craftRequest.GetFinishMinute(), 44);
                Assert::AreEqual(craftRequest.GetFinishSecond(), 55);

                CraftSystem::Destroy();
            }
        }

        // キューイングしてキャンセル
        TEST_METHOD(TestMethod16)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 12時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 0, 12, 0, 0);

            obj->UpdateCraftStatus();

            obj->CancelCraftStart(0);

            size_t size = obj->GetCraftRequestList().size();

            Assert::AreEqual<size_t>(size, 0u);

            CraftSystem::Destroy();
        }

        // キューイングしてキャンセル
        // 開始していないクラフトをキャンセルしたら素材が返ってくる
        TEST_METHOD(TestMethod16_2)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));
            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 12時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 0, 12, 0, 0);

            obj->UpdateCraftStatus();

            auto storehouse = StorehouseManager::Get()->GetCurrentActiveStorehouse();
            auto mat1 = storehouse->CountItem(L"trunk");
            auto mat2 = storehouse->CountItem(L"stoneForSpear");

            Assert::AreEqual(8, mat1);
            Assert::AreEqual(8, mat2);

            obj->CancelCraftStart(1);

            mat1 = storehouse->CountItem(L"trunk");
            mat2 = storehouse->CountItem(L"stoneForSpear");
            Assert::AreEqual(9, mat1);
            Assert::AreEqual(9, mat2);

            CraftSystem::Destroy();
        }

        // キューイングしてキャンセル
        // 開始したクラフトをキャンセルしたら素材が返ってこない
        TEST_METHOD(TestMethod16_3)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 12時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 0, 12, 0, 0);

            obj->UpdateCraftStatus();

            auto storehouse = StorehouseManager::Get()->GetCurrentActiveStorehouse();
            auto mat1 = storehouse->CountItem(L"trunk");
            auto mat2 = storehouse->CountItem(L"stoneForSpear");

            Assert::AreEqual(8, mat1);
            Assert::AreEqual(8, mat2);

            obj->CancelCraftStart(0);

            mat1 = storehouse->CountItem(L"trunk");
            mat2 = storehouse->CountItem(L"stoneForSpear");
            Assert::AreEqual(8, mat1);
            Assert::AreEqual(8, mat2);

            CraftSystem::Destroy();
        }

        // 複数キューイングしてキャンセル
        TEST_METHOD(TestMethod17)
        {
            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));
            std::wstring work;
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);
            obj->QueueCraftRequest(_T("stoneSpear"), &work, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 12時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 0, 12, 0, 0);

            obj->UpdateCraftStatus();

            obj->CancelCraftStart(0);
            obj->CancelCraftStart(0);

            size_t size = obj->GetCraftRequestList().size();

            Assert::AreEqual<size_t>(size, 1u);

            CraftSystem::Destroy();
        }

        // 規定回数、クラフトを行うと強化値の高いアイテムがクラフトされるようになる。
        // 現在は、1回のクラフトでレベルが上がるようになっている。
        TEST_METHOD(TestMethod18)
        {
            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            int work = 0;
            bool work_b = false;
            work = storehouse->CountItem(L"stoneSpear");

            Assert::AreEqual(10, work);

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            work = obj->GetCraftsmanSkill(_T("stoneSpear"));
            Assert::AreEqual(-1, work);

            std::wstring work2;
            work_b = obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);
            work_b = obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear");

            Assert::AreEqual(11, work);

            work = obj->GetCraftsmanSkill(_T("stoneSpear"));
            Assert::AreEqual(1, work);

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            work = storehouse->CountItem(L"stoneSpear1");

            Assert::AreEqual(11, work);

            work = obj->GetCraftsmanSkill(_T("stoneSpear"));
            Assert::AreEqual(2, work);

            CraftSystem::Destroy();
        }

        // 職人の熟練度が高い状態でクラフトを依頼すると同じ素材を使っても
        // 強化度の高いアイテムが作られる。
        // ダークソウルのような古い武器を素材に使う、ということはしない。
        TEST_METHOD(TestMethod19)
        {
            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            int work = 0;
            bool work_b = false;

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work2;
            work_b = obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            // 職人に石槍の製造を依頼すると、この時点では強化値なしの石槍を作り始める。
            work = obj->GetCraftRequestList().front().GetCraftInfo().GetOutput().GetLevel();
            Assert::AreEqual(work, -1);

            work_b = obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);
            // ここで職人のレベルアップ。＋１の石槍を作れるようになる

            obj->UpdateCraftStatus();

            // 1日と1時間、時を進める
            // ここで職人のレベルアップ。＋２の石槍を作れるようになる
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            // ここで石槍を予約したら＋２の石槍が作られる
            work_b = obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            // ＋２の石槍が予約されているか？
            work = obj->GetCraftRequestList().front().GetCraftInfo().GetOutput().GetLevel();
            Assert::AreEqual(work, 2);

            CraftSystem::Destroy();
        }

        // 武器を作り、武器管理クラスが更新されるかテスト
        TEST_METHOD(TestMethod20)
        {
            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            int work = 0;
            bool work_b = false;

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work2;
            work_b = obj->QueueCraftRequest(_T("stoneSpear"), &work2, 1);

            // 武器の制作を開始
            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            // 武器の制作が完了
            obj->UpdateCraftStatus();

            CraftSystem::Destroy();
        }
        
        // 素材に武器がある場合。
        //TEST_METHOD(TestMethod21)
        //{
        //    Storehouse* storehouse = Storehouse::GetObj();
        //    int work = 0;
        //    bool work_b = false;

        //    CraftSystem* obj = CraftSystem::GetObj();
        //    obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
        //              _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

        //    work_b = obj->QueueCraftRequest(_T("石槍"));

        //    // 職人に石槍の製造を依頼すると、この時点では強化値なしの石槍を作り始める。
        //    work = obj->GetCraftRequestList().front().GetCraftInfo().GetOutput().GetLevel();
        //    Assert::AreEqual(work, -1);

        //    work_b = obj->QueueCraftRequest(_T("石槍"));

        //    obj->UpdateCraftStatus();

        //    PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

        //    // 1日と1時間、時を進める
        //    powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

        //    obj->UpdateCraftStatus();

        //    // 1日と1時間、時を進める
        //    // ここで職人のレベルアップ。＋１の石槍を作れるようになる
        //    powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

        //    obj->UpdateCraftStatus();

        //    // ここで石槍を予約したら＋１の石槍が作られる
        //    work_b = obj->QueueCraftRequest(_T("石槍"));

        //    // ＋１の石槍が予約されているか？
        //    work = obj->GetCraftRequestList().front().GetCraftInfo().GetOutput().GetLevel();
        //    Assert::AreEqual(work, 1);

        //    CraftSystem::Destroy();
        //}

        // イカダのクラフト
        // 完成しても倉庫に格納されず、拠点の近くに配置される
        TEST_METHOD(TestMethod21)
        {
            auto raftNum1 = (int)Voyage::Get()->GetRaftList().size();

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(L"trunk");
                storehouse->AddItem(L"tsuta");
            }

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            std::wstring work;
            obj->QueueCraftRequest(_T("raft"), &work);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            auto raftCount = storehouse->CountItem(L"raft");

            Assert::AreEqual(0, raftCount);

            auto raftNum2 = (int)Voyage::Get()->GetRaftList().size();

            Assert::AreEqual(1, raftNum2 - raftNum1);

            CraftSystem::Destroy();
        }

        // イカダのクラフト
        // 強化値1のイカダが作れることを確認するテスト
        TEST_METHOD(TestMethod22)
        {
            Voyage::Destroy();

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(L"trunk");
                storehouse->AddItem(L"tsuta");
            }

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            // 職人のスキルを1にする
            obj->SetCraftsmanSkill(L"raft", 1);

            std::wstring work;
            obj->QueueCraftRequest(_T("raft"), &work);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            auto raftList = Voyage::Get()->GetRaftList();

            Assert::AreEqual(1, (int)raftList.size());
            Assert::AreEqual(1, raftList.at(0).GetLevel());
            Assert::AreEqual(200, raftList.at(0).GetDurability());

            CraftSystem::Destroy();
        }

        // イカダのクラフト
        // 強化値3のイカダが作れることを確認するテスト
        // 強化値3から素材にブイが追加される
        TEST_METHOD(TestMethod23)
        {
            Voyage::Destroy();

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(L"trunk");
                storehouse->AddItem(L"tsuta");
                storehouse->AddItem(L"bui");
            }

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            // 職人のスキルを1にする
            obj->SetCraftsmanSkill(L"raft", 3);

            std::wstring work;
            obj->QueueCraftRequest(_T("raft"), &work);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            auto raftList = Voyage::Get()->GetRaftList();

            Assert::AreEqual(1, (int)raftList.size());
            Assert::AreEqual(3, raftList.at(0).GetLevel());
            Assert::AreEqual(400, raftList.at(0).GetDurability());

            CraftSystem::Destroy();
        }

        // イカダのクラフト
        // 職人のレベルが10の時、どうなるか確認するテスト
        // 10が上限なので、レベルアップすると問題
        TEST_METHOD(TestMethod24)
        {
            Voyage::Destroy();

            StorehouseManager* storehouseManager = StorehouseManager::Get();
            Storehouse* storehouse = storehouseManager->GetStorehouse(1);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(L"trunk");
                storehouse->AddItem(L"tsuta");
                storehouse->AddItem(L"bui");
            }

            CraftSystem* obj = CraftSystem::GetObj();
            obj->Init(_T("..\\ModelTest\\craftsmanSkill.csv"),
                      _T("..\\ModelTest\\craftsmanQueueEmpty.csv"));

            // 職人のスキルを1にする
            obj->SetCraftsmanSkill(L"raft", 10);

            std::wstring work;
            obj->QueueCraftRequest(_T("raft"), &work);

            obj->UpdateCraftStatus();

            PowereggDateTime* powereggDateTime = PowereggDateTime::GetObj();

            // 1日と1時間、時を進める
            powereggDateTime->IncreaseDateTime(0, 1, 1, 0, 0);

            obj->UpdateCraftStatus();

            auto raftList = Voyage::Get()->GetRaftList();

            Assert::AreEqual(1, (int)raftList.size());
            Assert::AreEqual(10, raftList.at(0).GetLevel());

            auto skillLevel = obj->GetCraftsmanSkill(L"raft");
            Assert::AreEqual(10, skillLevel);

            CraftSystem::Destroy();
        }
    };
}
