#include "CppUnitTest.h"
#include "../Model/Rynen.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(RynenTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            Rynen* obj = Rynen::GetObj();
            Assert::AreEqual(obj != nullptr, true);
            Rynen::Destroy();
        }

        TEST_METHOD(TestMethod02)
        {
            Rynen* obj = Rynen::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\rynen.csv"));
            Assert::AreEqual(obj != nullptr, true);
            Rynen::Destroy();
        }

        TEST_METHOD(TestMethod03)
        {
            Rynen* obj = Rynen::GetObj();
            obj->Init(_T("..\\StarmanLibTest\\rynen.csv"));
            obj->SetContracted(true);
            obj->SetReviveEnable(true);
            obj->SetRevivePos(111.f, 222.f, 333.f);
            Assert::AreEqual(obj->GetContracted(), true);
            Assert::AreEqual(obj->GetReviveEnable(), true);
            float x = 0.f;
            float y = 0.f;
            float z = 0.f;
            obj->GetRevivePos(&x, &y, &z);
            Assert::AreEqual(x, 111.f);
            Assert::AreEqual(y, 222.f);
            Assert::AreEqual(z, 333.f);
            Rynen::Destroy();
        }

        TEST_METHOD(TestMethod04)
        {
            {
                Rynen* obj = Rynen::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\rynen.csv"));
                obj->SetContracted(true);
                obj->SetReviveEnable(true);
                obj->SetRevivePos(444.f, 555.f, 666.f);
                obj->Save(_T("..\\StarmanLibTest\\rynenSave.csv"));
                Rynen::Destroy();
            }
            {
                Rynen* obj = Rynen::GetObj();
                obj->Init(_T("..\\StarmanLibTest\\rynenSave.csv"));
                Assert::AreEqual(obj->GetContracted(), true);
                Assert::AreEqual(obj->GetReviveEnable(), true);
                float x = 0.f;
                float y = 0.f;
                float z = 0.f;
                obj->GetRevivePos(&x, &y, &z);
                Assert::AreEqual(x, 444.f);
                Assert::AreEqual(y, 555.f);
                Assert::AreEqual(z, 666.f);
                Rynen::Destroy();
            }
        }
    };
}
