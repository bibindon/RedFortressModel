#include "CppUnitTest.h"
#include "../Model/HumanInfoManager.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(HumanInfoManagerTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            HumanInfoManager* obj = HumanInfoManager::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            HumanInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            HumanInfoManager* obj = HumanInfoManager::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\humanInfo.csv"), _T("..\\StarmanLibTest\\humanInfoSub.csv"));
            HumanInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            HumanInfoManager* obj = HumanInfoManager::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\humanInfo.csv"), _T("..\\StarmanLibTest\\humanInfoSub.csv"));
            std::vector<std::wstring> vs = obj->GetHumanIdList();
            Assert::AreEqual((int)vs.size(), 12);
            HumanInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod04)
        {
            HumanInfoManager* obj = HumanInfoManager::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\humanInfo.csv"), _T("..\\StarmanLibTest\\humanInfoSub.csv"));
            std::vector<std::wstring> vs = obj->GetHumanIdList();
            HumanInfo humanInfo = obj->GetHumanInfo(vs.at(0));
            Assert::AreEqual(humanInfo.GetName() == _T("ホシマン"), true);
            bool find = (humanInfo.GetDetail().find(_T("１７０ｃｍ")) != std::wstring::npos);
            Assert::AreEqual(find, true);
            Assert::AreEqual(humanInfo.GetImagePath() == _T("human_hoshiman.png"), true);
            HumanInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod05)
        {
            HumanInfoManager* obj = HumanInfoManager::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\humanInfo.csv"), _T("..\\StarmanLibTest\\humanInfoSub.csv"));
            std::vector<std::wstring> vs = obj->GetHumanIdList();
            HumanInfo humanInfo = obj->GetHumanInfo(vs.at(11));
            Assert::AreEqual(humanInfo.GetName() == _T("ヒシガタマン"), true);
            bool find = (humanInfo.GetDetail().find(_T("うまくいかない")) != std::wstring::npos);
            Assert::AreEqual(find, true);
            Assert::AreEqual(humanInfo.GetImagePath() == _T("human_xxx.png"), true);
            HumanInfoManager::Destroy();
        }

        TEST_METHOD(TestMethod06)
        {
            {
                HumanInfoManager* obj = HumanInfoManager::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\humanInfo.csv"),
                          _T("..\\StarmanLibTest\\humanInfoSub.csv"));
                HumanInfo humanInfo = obj->GetHumanInfo(_T("noble"));
                Assert::AreEqual(humanInfo.GetVisible(), false);
                obj->SetHumanVisible(_T("noble"));
                humanInfo = obj->GetHumanInfo(_T("noble"));
                Assert::AreEqual(humanInfo.GetVisible(), true);
                obj->Save(_T("..\\StarmanLibTest\\humanInfoSubSave.csv"));
                HumanInfoManager::Destroy();
            }
            {
                HumanInfoManager* obj = HumanInfoManager::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\humanInfo.csv"),
                          _T("..\\StarmanLibTest\\humanInfoSubSave.csv"));
                HumanInfo humanInfo = obj->GetHumanInfo(_T("noble"));
                Assert::AreEqual(humanInfo.GetVisible(), true);
                HumanInfoManager::Destroy();
            }
        }
    };
}
