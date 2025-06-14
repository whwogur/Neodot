#include "NeodotCppUnitTest.h"
#include <Core/src/ioc/Container.h>
#include <Core/src/ioc/Singletons.h>
#include <memory>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct Base
{
	virtual int Test() { return 420; }
	virtual ~Base() = default;
};

struct Derived : public Base
{
	int Test() override { return 69; }
};

struct ParameterizedClass
{
	struct IocParams
	{
		std::string s;
	};
	ParameterizedClass(IocParams params) : s{ std::move(params.s) } {}
	std::string s;
};

struct Dependant
{
	std::shared_ptr<Base> pDependency;
	Dependant(std::shared_ptr<Base> pDependency_in) : pDependency{ std::move(pDependency_in) } {}
};

using namespace Neodot;
using namespace std::string_literals;

namespace Infrastructure
{
	TEST_CLASS(IocSingletonTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			pIOC = std::make_unique<IOC::Container>();
			pSing = std::make_unique<IOC::Singletons>();
		}
		// registering a service with a derived implementation, and resolving derived
		// direct creation in singleton container
		TEST_METHOD(PolymorphicResolveDirect)
		{
			pSing->Register<Base>([] {return std::make_shared<Derived>(); });
			Assert::AreEqual(69, pSing->Resolve<Base>()->Test());
		}
		// registering a service with a derived implementation, and resolving derived
		// indirect creation using ioc container
		TEST_METHOD(PolymorphicResolveIndirect)
		{
			pIOC->Register<Base>([] {return std::make_shared<Derived>(); });
			pSing->Register<Base>([this] {return pIOC->Resolve<Base>(); });
			Assert::AreEqual(69, pSing->Resolve<Base>()->Test());
		}
		// trying to resolve a service without registering
		TEST_METHOD(SimpleResolveFailure)
		{
			Assert::ExpectException<std::runtime_error>([this] {
				pSing->Resolve<Base>();
				});
		}
		// dependent resolve, where resolved instances of the same type are the object
		TEST_METHOD(DependentResolve)
		{
			pSing->Register<ParameterizedClass>([] {
				return std::make_shared<ParameterizedClass>(ParameterizedClass::IocParams{ .s = "first"s });
				});
			auto pFirst = pSing->Resolve<ParameterizedClass>();
			auto pSecond = pSing->Resolve<ParameterizedClass>();
			Assert::AreEqual("first"s, pFirst->s);
			Assert::AreEqual("first"s, pSecond->s);
			// changing one should affect the other
			pFirst->s = "pube"s;
			Assert::AreEqual("pube"s, pFirst->s);
			Assert::AreEqual("pube"s, pSecond->s);
		}
	private:
		std::unique_ptr<IOC::Container> pIOC;
		std::unique_ptr<IOC::Singletons> pSing;
	};
}