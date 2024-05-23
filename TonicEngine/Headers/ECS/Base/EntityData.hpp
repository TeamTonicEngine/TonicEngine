#pragma once
// Forward Declarations
namespace ECS::Components
{
	struct CameraComponent;
	struct TransformComponent;
}


namespace ECS
{
	class EntityManager;

	class  EntityData
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		EntityManager* p_mgr_;
		EntityID id_ = INVALID_ENTITY_ID;
		EntityID parent_ = ROOT_ENTITY_ID;
		std::vector<EntityID> children_;
		EntitySignature components_;
		EntitySignature systems_; /* not used for now*/
		std::string name_;

	public:
		__declspec(property(get = GetID))
			EntityID id;
		__declspec(property(get = GetChildren))
			std::vector<EntityID> children;
		__declspec(property(get = GetParent))
			EntityID parent;
		__declspec(property(get = GetName, put = SetName))
			std::string name;
		/**********************************************
				FUNCTIONS BLOC
		**********************************************/
		TONIC_ENGINE_API EntityData(const EntityID _id, EntityManager* _p_manager);
		TONIC_ENGINE_API EntityData(const EntityID _id, EntityManager* _p_manager, EntityID _parent);
		TONIC_ENGINE_API ~EntityData() = default;
		void TONIC_ENGINE_API Destroy();

		const TONIC_ENGINE_API EntityID GetID() const;

#pragma region Components
		template<typename T, typename ... Args>
		void AddComponent(Args&& ... _args);

		template<typename T>
		void AddComponent(T& _component);

		template<typename T>
		T& GetComponent();

		template<typename T>
		void RemoveComponent();

		template<typename T>
		const bool HasComponent();

		inline EntitySignature TONIC_ENGINE_API GetComponents() { return components_; };
#pragma endregion //Components

#pragma region Hierarchy

		void TONIC_ENGINE_API AddChild(EntityID _entity);
		void TONIC_ENGINE_API RemoveChild(EntityID _entity);
		const bool TONIC_ENGINE_API HasChildren() const { return !children_.empty(); };

		std::vector<EntityID> TONIC_ENGINE_API GetChildren() const { return children_; };
		EntityID TONIC_ENGINE_API GetChild(int _index) const { return children_.at(_index); };
		EntityID TONIC_ENGINE_API GetParent() const { return parent_; };
		std::string TONIC_ENGINE_API GetName() const { return name_; };
		void TONIC_ENGINE_API SetName(std::string _name) { name_ = _name; };
#pragma endregion //Hierarchy

		friend class EntityManager;
	};
}

