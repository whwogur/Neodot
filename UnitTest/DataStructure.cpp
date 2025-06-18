#include "NeodotCppUnitTest.h"
#include <Core/src/DS/Spatial/Dimensions.h>
#include <Core/src/DS/Spatial/Vec2.h>
#include <Core/src/DS/Spatial/Rect.h>
#include <Core/src/DS/Concurrency/TaskQueue.h>
#include <string>
#include <sstream>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<> inline std::wstring __cdecl
    ToString<Neodot::DS::Vec2I>(const Neodot::DS::Vec2I& vec)
    {
        std::wstringstream stream;
        stream << L"Vec2(" << vec.x << L", " << vec.y << L")";
        return stream.str();
    }

    template<> inline std::wstring __cdecl
    ToString<Neodot::DS::DimensionsI>(const Neodot::DS::DimensionsI& dims)
    {
        std::wstringstream stream;
        stream << L"Dimensions(" << dims.width << L", " << dims.height << L")";
        return stream.str();
    }
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace Neodot::DS;

namespace Infrastructure::DS
{
	namespace Spatial
	{
		TEST_CLASS(DimensionsTests)
		{
		public:
			TEST_METHOD(GetArea)
			{
				DimensionsT<int> dims{ 4, 30 };

				int result = dims.GetArea();

				Assert::AreEqual(120, result);
			}
		};

		TEST_CLASS(Vec2Tests)
		{
		public:
			TEST_METHOD(Addition)
			{
				Vec2T<int> a{ 7, 1 };
				Vec2T<int> b{ -3, 9 };

				Vec2T<int> result = a + b;

				Assert::AreEqual(Vec2T<int>(4, 10), result);
			}

			TEST_METHOD(Subtraction)
			{
				Vec2T<int> a{ 8, 6 };
				Vec2T<int> b{ 2, 1 };

				Vec2T<int> result = a - b;

				Assert::AreEqual(Vec2T<int>(6, 5), result);
			}

			TEST_METHOD(AdditionAssignment)
			{
				Vec2T<int> a{ -5, 5 };
				Vec2T<int> b{ 10, -10 };

				a += b;

				Assert::AreEqual(Vec2T<int>(5, -5), a);
			}

			TEST_METHOD(SubtractionAssignment)
			{
				Vec2T<int> a{ 3, 3 };
				Vec2T<int> b{ 6, 9 };

				a -= b;

				Assert::AreEqual(Vec2T<int>(-3, -6), a);
			}
		};
	}


    namespace Concurrency
    {
		TEST_CLASS(TaskQueueTests)
		{
		public:
			TEST_METHOD(PushAndPopExecute)
			{
				TaskQueue taskQueue;

				// Push a task to add 1 to x
				int x = 0;
				auto future1 = taskQueue.Push([&x]() { return x += 1; });

				// Push a task to add 2 to x
				auto future2 = taskQueue.Push([&x]() { return x += 2; });

				// Pop and execute tasks
				taskQueue.PopExecute();
				taskQueue.PopExecute();

				// Check result of futures
				Assert::AreEqual(1, future1.get());
				Assert::AreEqual(3, future2.get());
				Assert::AreEqual(3, x);
			}

			TEST_METHOD(MultipleThreadsPushAndPopExecute)
			{
				TaskQueue taskQueue;

				// Push tasks from multiple threads
				int x = 0;
				std::vector<std::future<void>> futures;
				for (int i = 0; i < 10; i++)
				{
					auto future = std::async(std::launch::async, [&taskQueue, &x]() {
						taskQueue.Push([&x]() { x += 1; });
					});
					futures.push_back(std::move(future));
				}

				// Pop and execute tasks from multiple threads
				std::vector<std::future<void>> popFutures;
				for (int i = 0; i < 10; i++)
				{
					auto popFuture = std::async(std::launch::async, [&taskQueue]() {
						taskQueue.PopExecute();
					});
					popFutures.push_back(std::move(popFuture));
				}

				// Wait for all futures to complete
				for (auto& future : futures)
				{
					future.wait();
				}
				for (auto& popFuture : popFutures)
				{
					popFuture.wait();
				}

				// Check result of x
				Assert::AreEqual(10, x);
			}
		};
    }
}