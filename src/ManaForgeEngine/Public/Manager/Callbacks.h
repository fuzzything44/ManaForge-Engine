#pragma once

#include <mutex>

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
		for (auto&& elem : thisTypeDerived.entityStorage)
		{
			if (elem.second.matchesSignature(sigToTest)) append.push_back(elem.first);
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

			if (castedManager.tickNumber + 1 == baseManager.tickNumber) // make sure that update isn't called twice per frame
			{
				updateManager(castedManager);

				for (auto&& child : castedManager.children)
				{
					child.first.update(*child.second, castedManager);
				}

				++castedManager.tickNumber;
			}

			
		}
	};


	template<typename ThisManager, typename BaseManager>
	struct BeginPlay_t
	{
		static void beginPlay(ManagerBase& thisManager, BaseManager& baseManager)
		{
			auto&& castedManager = static_cast<ThisManager&>(thisManager);

			if (!castedManager.hasBegunPlay)
			{
				::beginPlayManager(castedManager);

				for (auto&& child : castedManager.children)
				{
					child.first.beginPlay(*child.second, castedManager);
				}
				castedManager.hasBegunPlay = true;
			}

		}
	};

}
