#include "NeodotCppUnitTest.h"
#include <Core/src/IOC/Container.h>
#include <memory>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Neodot;

struct Base
{
	virtual int Test() { return 420; }
	virtual ~Base() = default;
};

struct Derived : public Base
{
	int Test() override { return 69; }
};

namespace Infrastructure
{
	TEST_CLASS(IOCTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			pIOC = std::make_unique<IOC::Container>();
		}

		TEST_METHOD(SimpleResolve)
		{
			pIOC->Register<Base>([] {return std::make_shared<Derived>(); });
			Assert::AreEqual(69, pIOC->Resolve<Base>()->Test());
		}

		TEST_METHOD(SimpleResolveFailure)
		{
			Assert::ExpectException<std::runtime_error>([this] {
				pIOC->Resolve<Base>();
			});
		}
	private:
		std::unique_ptr<IOC::Container> pIOC;
	};
}
