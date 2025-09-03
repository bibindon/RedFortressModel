#include "CppUnitTest.h"
#include "../Model/EnemyInfoManager.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSModel;

namespace StarmanLibTest
{

    TEST_CLASS(EnemyManagerTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            EnemyInfoManager* obj = EnemyInfoManager::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            EnemyInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            EnemyInfoManager* obj = EnemyInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                      _T("..\\ModelTest\\enemyOrigin.csv"),
                      _T("..\\ModelTest\\enemyVisible.csv"));

            Assert::AreEqual(obj != nullptr, true);
            EnemyInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            EnemyInfoManager* obj = EnemyInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                      _T("..\\ModelTest\\enemyOrigin.csv"),
                      _T("..\\ModelTest\\enemyVisible.csv"));
            std::vector<stEnemyInfo> enemyInfoList = obj->GetEnemyInfo(10.f, 0.f, 10.f, 1.f);
            Assert::AreEqual((int)enemyInfoList.size() == 1, true);
            EnemyInfoManager::Destroy();
        }

        // テストしたいこと
        // 座標と範囲を指定して敵の情報が取得できること
        TEST_METHOD(TestMethod04)
        {
            EnemyInfoManager* obj = EnemyInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                      _T("..\\ModelTest\\enemyOrigin.csv"),
                      _T("..\\ModelTest\\enemyVisible.csv"));
            std::vector<stEnemyInfo> enemyInfoList = obj->GetEnemyInfo(5.f, 5.f, 5.f, 6.f);
            Assert::AreEqual(1, (int)enemyInfoList.size());
            Assert::AreEqual(enemyInfoList.at(0).m_SerialNumber == 1, true);
            Assert::AreEqual(std::wstring(enemyInfoList.at(0).m_id) == _T("cube"), true);
            Assert::AreEqual(enemyInfoList.at(0).m_x, 10.f);
            Assert::AreEqual(enemyInfoList.at(0).m_bDefeated, true);
            EnemyInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod05)
        {
            EnemyInfoManager* obj = EnemyInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                      _T("..\\ModelTest\\enemyOrigin.csv"),
                      _T("..\\ModelTest\\enemyVisible.csv"));
            std::vector<stEnemyInfo> enemyInfoList = obj->GetEnemyInfo(96.f, 0.f, 97.f, 2.f);
            Assert::AreEqual((int)enemyInfoList.size() == 1, true);
            Assert::AreEqual(enemyInfoList.at(0).m_SerialNumber == 30, true);
            Assert::AreEqual(std::wstring(enemyInfoList.at(0).m_id) == _T("cube"), true);
            Assert::AreEqual(enemyInfoList.at(0).m_x, 97.f);
            Assert::AreEqual(enemyInfoList.at(0).m_bDefeated, false);
            EnemyInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod06)
        {
            {
                EnemyInfoManager* obj = EnemyInfoManager::GetObj();
                obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                          _T("..\\ModelTest\\enemyOrigin.csv"),
                          _T("..\\ModelTest\\enemyVisible.csv"));
                stEnemyInfo enemyInfo;
                enemyInfo.m_x = 111.f;
                enemyInfo.m_y = 222.f;
                enemyInfo.m_z = 333.f;
                enemyInfo.m_HP = 444;
                enemyInfo.m_bDefeated = true;
                obj->UpdateEnemyInfo(5, enemyInfo);
                obj->Save(_T("..\\ModelTest\\enemySave.csv"),
                          _T("..\\ModelTest\\enemyVisibleSave.csv"));
                EnemyInfoManager::Destroy();
            }
        }

        TEST_METHOD(TestMethod07)
        {
            {
                EnemyInfoManager* obj = EnemyInfoManager::GetObj();
                obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                          _T("..\\ModelTest\\enemyOrigin.csv"),
                          _T("..\\ModelTest\\enemyVisible.csv"));
                stEnemyInfo enemyInfo;
                enemyInfo.m_SerialNumber = 20;
                wcsncpy_s(enemyInfo.m_id, _T("enban"), _TRUNCATE);
                enemyInfo.m_x = 111.f;
                enemyInfo.m_y = 222.f;
                enemyInfo.m_z = 333.f;
                enemyInfo.m_HP = 444;
                enemyInfo.m_bDefeated = true;
                obj->UpdateEnemyInfo(20, enemyInfo);
                obj->SetEnemyVisible(_T("enban"), true);
                obj->Save(_T("..\\ModelTest\\enemySave.csv"),
                          _T("..\\ModelTest\\enemyVisibleSave.csv"));
                EnemyInfoManager::Destroy();
            }
            {
                EnemyInfoManager* obj = EnemyInfoManager::GetObj();
                obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                          _T("..\\ModelTest\\enemySave.csv"),
                          _T("..\\ModelTest\\enemyVisibleSave.csv"));
                std::vector<stEnemyInfo> enemyInfoList = obj->GetEnemyInfo(111.f, 222.f, 333.f, 1.f);
                Assert::AreEqual(true, (int)enemyInfoList.size() == 1);
                Assert::AreEqual(true, enemyInfoList.at(0).m_SerialNumber == 20);
                Assert::AreEqual(true, std::wstring(enemyInfoList.at(0).m_id) == L"enban");
                Assert::AreEqual(444, enemyInfoList.at(0).m_HP);
                Assert::AreEqual(true, enemyInfoList.at(0).m_bDefeated);

                bool work = obj->GetEnemyDef(_T("enban")).GetVisible();
                Assert::AreEqual(true, work);
                EnemyInfoManager::Destroy();
            }
        }

        // バイナリ形式で読み込みができることを確認するテスト
        TEST_METHOD(TestMethod08)
        {
            EnemyInfoManager* obj = EnemyInfoManager::GetObj();
            obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                      _T("..\\ModelTest\\enemyOrigin.bin"),
                      _T("..\\ModelTest\\enemyVisible.csv"),
                      false,
                      true);

            std::vector<stEnemyInfo> enemyInfoList = obj->GetEnemyInfo(96.f, 0.f, 97.f, 2.f);
            Assert::AreEqual((int)enemyInfoList.size() == 1, true);
            Assert::AreEqual(enemyInfoList.at(0).m_SerialNumber == 30, true);
            Assert::AreEqual(std::wstring(enemyInfoList.at(0).m_id) == _T("cube"), true);
            Assert::AreEqual(enemyInfoList.at(0).m_x, 97.f);
            Assert::AreEqual(enemyInfoList.at(0).m_bDefeated, false);
            EnemyInfoManager::Destroy();
        }

        // バイナリ形式で保存し再度読み込めることを確認するテスト
        TEST_METHOD(TestMethod09)
        {
            {
                EnemyInfoManager* obj = EnemyInfoManager::GetObj();
                obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                          _T("..\\ModelTest\\enemyOrigin.bin"),
                          _T("..\\ModelTest\\enemyVisible.csv"),
                          false,
                          true);

                stEnemyInfo enemyInfo;
                enemyInfo.m_SerialNumber = 20;
                wcsncpy_s(enemyInfo.m_id, _T("enban"), _TRUNCATE);
                enemyInfo.m_x = 111.f;
                enemyInfo.m_y = 222.f;
                enemyInfo.m_z = 333.f;
                enemyInfo.m_HP = 444;
                enemyInfo.m_bDefeated = true;
                obj->UpdateEnemyInfo(20, enemyInfo);
                obj->SetEnemyVisible(_T("enban"), true);
                obj->Save(_T("..\\ModelTest\\enemySave.bin"),
                          _T("..\\ModelTest\\enemyVisibleSave.csv"),
                          false,
                          true);
                EnemyInfoManager::Destroy();
            }

            {
                EnemyInfoManager* obj = EnemyInfoManager::GetObj();
                obj->Init(_T("..\\ModelTest\\enemyDef.csv"),
                          _T("..\\ModelTest\\enemySave.bin"),
                          _T("..\\ModelTest\\enemyVisibleSave.csv"),
                          false,
                          true);
                std::vector<stEnemyInfo> enemyInfoList = obj->GetEnemyInfo(111.f, 222.f, 333.f, 1.f);
                Assert::AreEqual(true, (int)enemyInfoList.size() == 1);
                Assert::AreEqual(true, enemyInfoList.at(0).m_SerialNumber == 20);
                Assert::AreEqual(true, std::wstring(enemyInfoList.at(0).m_id) == L"enban");
                Assert::AreEqual(444, enemyInfoList.at(0).m_HP);
                Assert::AreEqual(true, enemyInfoList.at(0).m_bDefeated);

                bool work = obj->GetEnemyDef(_T("enban")).GetVisible();
                Assert::AreEqual(true, work);
                EnemyInfoManager::Destroy();
            }
        }
    };
}
