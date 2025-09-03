#include "CppUnitTest.h"
#include "../Model/PowereggDateTime.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

    TEST_CLASS(PowereggDateTimeTest)
    {
    public:

        TEST_METHOD(TestMethod01)
        {
            PowereggDateTime* obj = PowereggDateTime::GetObj();
            Assert::AreEqual(obj != nullptr, true);
        }
    };
}
