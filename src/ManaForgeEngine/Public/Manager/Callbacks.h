#pragma once

template<typename Manager>
void updateManager(Manager& man) {}

template<typename Manager>
void initManager(Manager& man) {}

template<typename Manager>
void beginPlayManager(Manager& man) {}

template<typename Manager>
void exitManager(Manager& man) {}

template<typename Manager>
struct ManagerData {};

struct ManagerBase;

namespace detail
{

	template<typename BaseManagerType, typename ThisManager>
	void getAllMatching(ManagerBase& thisType, std::vector<size_t>& append, const typename BaseManagerType::RuntimeSignature_t& toTest)
	{
		auto& thisTypeDerived = static_cast<ThisManager&>(thisType);

		auto&& sigToTest = thisTypeDerived.template baseRuntimeSignatureToThisRuntimeSignature<BaseManagerType>(toTest);
		for (auto&& elem : thisTypeDerived.getEntityStorage())
		{
			if (elem.matchesSignature(sigToTest)) append.push_back(elem.GUID);
		}
		for (auto&& elem : thisTypeDerived.children)
		{
			elem.first.getAllMatching(*elem.second, append, sigToTest);
		}
	}

	template<typename ThisManager, typename BaseManager>
	struct Update_t
	{
		static void update(ManagerBase& thisManager, BaseManager& baseManager)
		{
			auto&& castedManager = static_cast<ThisManager&>(thisManager);

			updateManager(castedManager);

			for (auto&& child : castedManager.children)
			{
				child.first.update(*child.second, castedManager);
			}
		}
	};


	template<typename ThisManager, typename BaseManager>
	struct BeginPlay_t
	{
		static void beginPlay(ManagerBase& thisManager, BaseManager& baseManager)
		{
			auto&& castedManager = static_cast<ThisManager&>(thisManager);

			::beginPlayManager(castedManager);

			for (auto&& child : castedManager.children)
			{
				child.first.beginPlay(*child.second, castedManager);
			}
		}
	};

}
