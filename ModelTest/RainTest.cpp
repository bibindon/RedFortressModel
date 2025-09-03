#include "CppUnitTest.h"
#include "../Model/RainModel.h"

#include "../Model/PowereggDateTime.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace NSModel;

namespace StarmanLibTest
{

    TEST_CLASS(RainTest)
    {
    public:

        // 単純にPublic関数を呼んで落ちないことを確認するテスト
        TEST_METHOD(TestMethod01)
        {
            // Target
            RainModel* obj = RainModel::Get();

            Assert::AreEqual(true, obj != nullptr);
            obj->Destroy();
        }

        // 単純にPublic関数を呼んで落ちないことを確認するテスト
        TEST_METHOD(TestMethod02)
        {
            RainModel* obj = RainModel::Get();

            // Target
            obj->Update();

            obj->Destroy();
        }

        // 単純にPublic関数を呼んで落ちないことを確認するテスト
        TEST_METHOD(TestMethod03)
        {
            RainModel* obj = RainModel::Get();
            obj->Update();

            // Target
            obj->IsRain();

            obj->Destroy();
        }

        // 単純にPublic関数を呼んで落ちないことを確認するテスト
        TEST_METHOD(TestMethod04)
        {
            RainModel* obj = RainModel::Get();

            // Target
            obj->Destroy();
        }

        // 晴れているとき、1時間後に雨が降るか、を10000回確認して
        // 雨が降る確率が5％程度であることを確認する
        TEST_METHOD(TestMethod05)
        {
            auto datetime = PowereggDateTime::GetObj();
            RainModel* obj = RainModel::Get();

            // 試行回数
            int tryCount = 0;

            // 雨が降った回数
            int rainStartCount = 0;

            for (int i = 0; i < 10000; ++i)
            {
                bool isRain1 = obj->IsRain();

                // 晴れだったら雨が降るか確認するチャンス
                if (!isRain1)
                {
                    ++tryCount;
                }

                datetime->IncreaseDateTime(0, 0, 1, 0, 0);
                obj->Update();

                bool isRain2 = obj->IsRain();

                if (!isRain1 && isRain2)
                {
                    rainStartCount++;
                }
            }

            double rainRate = (double)rainStartCount / tryCount;
            Assert::AreEqual(true, rainRate <= 0.1);
            Assert::AreEqual(true, rainRate != 0.0);

            obj->Destroy();
            datetime->Destroy();
        }

        // 雨が降っているとき、1時間後に雨が止むか、を10000回確認して
        // 雨がやむ確率が20％程度であることを確認する
        TEST_METHOD(TestMethod06)
        {
            auto datetime = PowereggDateTime::GetObj();
            RainModel* obj = RainModel::Get();

            // 試行回数
            int tryCount = 0;

            // 雨が止んだ回数
            int rainStopCount = 0;

            for (int i = 0; i < 10000; ++i)
            {
                bool isRain1 = obj->IsRain();

                // 雨だったら雨が止むか確認するチャンス
                if (isRain1)
                {
                    ++tryCount;
                }

                datetime->IncreaseDateTime(0, 0, 1, 0, 0);
                obj->Update();

                bool isRain2 = obj->IsRain();

                if (isRain1 && !isRain2)
                {
                    rainStopCount++;
                }
            }

            double stopRainRate = (double)rainStopCount / tryCount;
            Assert::AreEqual(true, stopRainRate <= 0.25);
            Assert::AreEqual(true, stopRainRate >= 0.15);
            Assert::AreEqual(true, stopRainRate != 0.0);

            obj->Destroy();
            datetime->Destroy();
        }
    };
}
