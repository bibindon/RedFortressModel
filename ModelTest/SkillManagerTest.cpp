#include "CppUnitTest.h"
#include "../Model/SkillManager.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

TEST_CLASS(SkillManagerTest)
{
public:

    TEST_METHOD(TestMethod01)
    {
        SkillManager* obj = SkillManager::GetObj();
        Assert::AreEqual(obj != nullptr, true);
        SkillManager::Destroy();
    }

    TEST_METHOD(TestMethod02)
    {
        SkillManager* obj = SkillManager::GetObj();
        obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSub.csv"));
        Assert::AreEqual(obj != nullptr, true);
        SkillManager::Destroy();
    }

    TEST_METHOD(TestMethod03)
    {
        SkillManager* obj = SkillManager::GetObj();
        obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSub.csv"));

        int level = 0;

        level = obj->GetSkillLevel(_T("炎の魔法"));
        Assert::AreEqual(level, 1);

        level = obj->GetSkillLevel(_T("氷の魔法"));
        Assert::AreEqual(level, 2);

        level = obj->GetSkillLevel(_T("闇の魔法"));
        Assert::AreEqual(level, 3);

        SkillManager::Destroy();
    }

    TEST_METHOD(TestMethod04)
    {
        SkillManager* obj = SkillManager::GetObj();
        obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSub.csv"));

        std::wstring temp;

        temp = obj->GetDetail(_T("炎の魔法"));
        Assert::AreEqual(temp.find(_T("負傷してしまう")) != std::wstring::npos, true);

        SkillManager::Destroy();
    }

    TEST_METHOD(TestMethod05)
    {
        SkillManager* obj = SkillManager::GetObj();
        obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSub.csv"));

        float temp = obj->GetDamage(_T("炎の魔法"), 5);
        Assert::AreEqual(temp, 60.f);

        SkillManager::Destroy();
    }

    TEST_METHOD(TestMethod06)
    {
        SkillManager* obj = SkillManager::GetObj();
        obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSub.csv"));

        float temp = obj->GetRange(_T("闇の魔法"), 5);
        Assert::AreEqual(temp, 60.f);

        SkillManager::Destroy();
    }

    TEST_METHOD(TestMethod07)
    {
        {
            SkillManager* obj = SkillManager::GetObj();
            obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSub.csv"));

            obj->SetSkillLevel(_T("闇の魔法"), 5);
            obj->Save(_T("..\\ModelTest\\skillSubSave.csv"));

            SkillManager::Destroy();
        }
        {
            SkillManager* obj = SkillManager::GetObj();
            obj->Init(_T("..\\ModelTest\\skill.csv"), _T("..\\ModelTest\\skillSubSave.csv"));

            int level = obj->GetSkillLevel(_T("闇の魔法"));
            Assert::AreEqual(level, 5);

            SkillManager::Destroy();
        }
    }
};
}
