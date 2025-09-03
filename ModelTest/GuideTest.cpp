#include "CppUnitTest.h"
#include "../Model/Guide.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(GuideTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            Guide* obj = Guide::GetObj();
            Assert::AreEqual(obj != nullptr, true);
        }

        TEST_METHOD(TestMethod02)
        {
            Guide* obj = Guide::GetObj();
            obj->Init(_T("..\\ModelTest\\guideDef.csv"), _T("..\\ModelTest\\guideOrigin.csv"));
            Assert::AreEqual((int)obj->GetCategoryList().size() != 0, true);
        }

        TEST_METHOD(TestMethod03)
        {
            Guide* obj = Guide::GetObj();
            obj->Init(_T("..\\ModelTest\\guideDef.csv"), _T("..\\ModelTest\\guideOrigin.csv"));
            std::wstring text = obj->GetText(_T("ステータス"), _T("ステータスについて"));
            Assert::AreEqual(text.size() != 0, true);
            std::wstring text2 = _T("\"このゲームには以下の１０のステータスが存在する。\n\n");
            text2 += _T("「スタミナ」、「脳のスタミナ」、「瞬発力」\n");
            text2 += _T("「水分」、「身体の疲労・損傷」\n");
            text2 += _T("「糖質」、「タンパク質」、「脂質」、「ビタミン」、「ミネラル」\"");
            Assert::AreEqual(text.find(_T("「ミネラル」")) != std::wstring::npos, true);
            Assert::AreEqual(text.empty() == false, true);
        }

        TEST_METHOD(TestMethod04)
        {
            {
                Guide* obj = Guide::GetObj();
                obj->Init(_T("..\\ModelTest\\guideDef.csv"), _T("..\\ModelTest\\guideOrigin.csv"));
                obj->SetVisible(_T("ステータス"), _T("「脳のスタミナ」について"));
                obj->Save(_T("..\\ModelTest\\guideSave.csv"));
                Guide::Destroy();
            }
            {
                Guide* obj = Guide::GetObj();
                obj->Init(_T("..\\ModelTest\\guideDef.csv"), _T("..\\ModelTest\\guideSave.csv"));
                bool result = obj->GetVisible(_T("ステータス"), _T("「脳のスタミナ」について"));
                Assert::AreEqual(result, true);
            }
        }
    };
}
