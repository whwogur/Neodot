#include "NeodotCppUnitTest.h"
#include <Core/src/IOC/Container.h>
#include <memory>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

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

namespace Infrastructure::IOC
{
	TEST_CLASS(IOCTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			pIOC = std::make_unique<Neodot::IOC::Container>();
		}

		// registering a service with a derived implementation, and resolving derived
		TEST_METHOD(PolymorphicResolve)
		{
			pIOC->Register<Base>([] {return std::make_shared<Derived>(); });
			Assert::AreEqual(69, pIOC->Resolve<Base>()->Test());
		}

		// trying to resolve a service without registering
		TEST_METHOD(SimpleResolveFailure)
		{
			Assert::ExpectException<Neodot::IOC::ServiceNotFound>([this] {
				pIOC->Resolve<Base>();
			});
		}

		// parameterized resolveAdd commentMore actions
		TEST_METHOD(ParameterizedResolve)
		{
			pIOC->Register<ParameterizedClass>([](ParameterizedClass::IocParams p) {
				return std::make_shared<ParameterizedClass>(std::move(p));
				});
			Assert::AreEqual("pube"s, pIOC->Resolve<ParameterizedClass>({ .s = "pube"s })->s);
		}
		// cascaded resolve, where factories also use container to resolve dependencies
		TEST_METHOD(CascadedResolve)
		{
			pIOC->Register<Dependant>([this] {
				return std::make_shared<Dependant>(pIOC->Resolve<Base>());
				});
			pIOC->Register<Base>([this] {
				return std::make_shared<Base>();
				});
			Assert::AreEqual(420, pIOC->Resolve<Dependant>()->pDependency->Test());
		}
		// independent resolve, where resolved instances of the same type are separate objects
		TEST_METHOD(IndependentResolve)
		{
			pIOC->Register<ParameterizedClass>([](ParameterizedClass::IocParams p) {
				return std::make_shared<ParameterizedClass>(std::move(p));
				});
			auto pFirst = pIOC->Resolve<ParameterizedClass>({ .s = "first"s });
			auto pSecond = pIOC->Resolve<ParameterizedClass>({ .s = "second"s });
			Assert::AreEqual("first"s, pFirst->s);
			Assert::AreEqual("second"s, pSecond->s);
			// changing one should not affect the other
			pFirst->s = "pube"s;
			Assert::AreEqual("pube"s, pFirst->s);
			Assert::AreNotEqual("pube"s, pSecond->s);
		}
		// replacing existing factory
		TEST_METHOD(ReplacementInjection)
		{
			// first make initial registration and check
			pIOC->Register<Dependant>([this] {
				return std::make_shared<Dependant>(pIOC->Resolve<Base>());
				});
			pIOC->Register<Base>([this] {
				return std::make_shared<Base>();
				});
			Assert::AreEqual(420, pIOC->Resolve<Dependant>()->pDependency->Test());
			// replace nested dependency and verify that Dependant with be affected when resolved
			pIOC->Register<Base>([this] {
				return std::make_shared<Derived>();
				});
			Assert::AreEqual(69, pIOC->Resolve<Dependant>()->pDependency->Test());
		}
	private:
		std::unique_ptr<Neodot::IOC::Container> pIOC;
	};
}
