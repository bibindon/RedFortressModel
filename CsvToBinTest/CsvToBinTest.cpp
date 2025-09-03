#include "pch.h"
#include "CppUnitTest.h"
#include "../CsvToBin/main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CsvToBinTest
{
    TEST_CLASS(CsvToBinTest)
    {
    public:
        
        TEST_METHOD(TestMethod1)
        {
            WriteBinaryFromMapObj(_T("..\\CsvToBinTest\\map_obj.csv"));

            BOOL result = PathFileExists(_T("..\\CsvToBinTest\\map_obj.bin"));
            Assert::AreEqual(TRUE, result);
        }

        TEST_METHOD(TestMethod2)
        {
            WriteBinaryFromEnemy(_T("..\\CsvToBinTest\\enemyOrigin.csv"));

            BOOL result = PathFileExists(_T("..\\CsvToBinTest\\enemyOrigin.bin"));
            Assert::AreEqual(TRUE, result);
        }
    };
}
