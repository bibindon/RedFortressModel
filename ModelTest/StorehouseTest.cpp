#include "CppUnitTest.h"
#include "../Model/Inventory.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include "../Model/Storehouse.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSStarmanLib;

namespace StarmanLibTest
{

TEST_CLASS(StorehouseTest)
{
public:

    TEST_METHOD(TestMethod01)
    {
        auto storehouse = StorehouseManager::Get();

        bool bException = false;
        try
        {
            storehouse->Init(_T("no-exist-file.csv"));
        }
        catch (const std::exception&)
        {
            bException = true;
        }
        Assert::AreEqual(true, bException);
    }
};
}
