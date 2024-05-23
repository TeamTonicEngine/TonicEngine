#pragma once
#include "pch.hpp"

#include "Types.hpp"

#include "Core/Log.hpp"

namespace Core::Applications
{
	class Engine;
}

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
		std::vector<T> data_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		CompList() = default;
		~CompList() = default;

		inline void Insert(const T& _component)
		{
			auto comp = std::find_if(data_.begin(), data_.end(), [&](const T& _c) { return _c.GetID() == _component.GetID(); });
			if (comp == data_.end())
				data_.push_back(_component);
		}

		inline T& Get(const EntityID _entity)
		{
			auto comp = std::find_if(data_.begin(), data_.end(), [&](const T& _c) { return _c.GetID() == _entity; });
			if (comp == data_.end())
				DEBUG_WARNING("Trying to get non-existing component!");
			return *comp;
		}

		inline void Erase(const EntityID _entity) override final
		{
			auto comp = std::find_if(data_.begin(), data_.end(), [&](const T& _c) { return _c.GetID() == _entity; });
			if (comp != data_.end())
				data_.erase(comp);
		}

		inline std::vector<T> GetData()
		{
			return data_;
		}

		//should be in a scene class
		friend class Core::Applications::Engine;
	};
}