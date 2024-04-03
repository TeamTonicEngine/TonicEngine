#pragma once
#include "pch.hpp"

#include "Types.hpp"

#include "Core/Log.hpp"

namespace ECS
{
	class TONIC_ENGINE_API ICompList
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		ICompList() = default;
		virtual ~ICompList() = default;

		virtual void Erase(const EntityID _entity) = 0;
	};

	template<typename T>
	class CompList : public ICompList
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		//this could be an array as well, more powerful, but less practical
		std::vector<T> data;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		CompList() = default;
		~CompList() = default;

		inline void Insert(const T& _component)
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& _c) { return _c.GetID() == _component.GetID(); });
			if (comp == data.end())
				data.push_back(_component);
		}

		inline T& Get(const EntityID _entity)
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& _c) { return _c.GetID() == _entity; });
			if (comp == data.end())
				DEBUG_WARNING("Trying to get non-existing component!");
			return *comp;
		}

		inline void Erase(const EntityID _entity) override final
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& _c) { return _c.GetID() == _entity; });
			if (comp != data.end())
				data.erase(comp);
		}
	};
}