#include "pch.hpp"

#include "Physics/JoltWrapper.hpp"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>

#include <Jolt/Physics/PhysicsSettings.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char* _FMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, _FMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), _FMT, list);
	va_end(list);

	// Print to the TTY
	DEBUG_LOG(buffer);
}

#ifdef JPH_ENABLE_ASSERTS
// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char* _expression, const char* _message, const char* _file, unsigned _line)
{
	// Print to the TTY
	DEBUG_ERROR("%s: %s: (%s) %s", _file, _line, _expression, (_message != nullptr ? _message : ""));

	// Breakpoint
	return true;
};
#endif // JPH_ENABLE_ASSERTS

const bool Physics::JoltWrapper::Init()
{
	using namespace JPH;

	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	RegisterDefaultAllocator();

	Trace = TraceImpl;	// Install trace and assert callbacks
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl);

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	Factory::sInstance = new Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	RegisterTypes();

	// We need a temp allocator for temporary allocations during the physics update. We're
	// pre-allocating 10 MB to avoid having to do allocations during the physics update.
	// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to malloc / free.
	p_tempAllocator_ = new TempAllocatorImpl(10 * 1024 * 1024);

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	p_jobSystem_ = new JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	const uint maxBodies = 65536;

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const uint numBodyMutexes = 0;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	const uint maxBodyPairs = 65536;

	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	const uint maxContactConstraints = 10240;

	// Now we can create the actual physics system.
	p_physicsSystem_ = new PhysicsSystem();
	p_physicsSystem_->Init(
		maxBodies, numBodyMutexes,
		maxBodyPairs, maxContactConstraints,
		p_broadPhaseLayerInterface_,
		p_objectVsBroadphaseLayerFilter_,
		p_objectVsObjectLayerFilter_
	);

	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	p_physicsSystem_->SetBodyActivationListener(&bodyActivationListener_);

	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	p_physicsSystem_->SetContactListener(&contactListener_);

	return p_physicsSystem_;
}

void* Physics::JoltWrapper::GetBodyInterface() { return &p_physicsSystem_->GetBodyInterface(); }

void Physics::JoltWrapper::Destroy()
{
	using namespace JPH;

	UnregisterTypes();

	delete Factory::sInstance;
	Factory::sInstance = nullptr;

	delete p_physicsSystem_;
	delete p_jobSystem_;
	delete p_tempAllocator_;
}

void Physics::JoltWrapper::Update(float _deltaTime)
{
	using namespace JPH;

	{
		int nbBodiesCurrent = p_physicsSystem_->GetNumBodies();
		int nbBodiesDifference = nbBodiesCurrent - nbBodiesLastCheck;
		if (nbBodiesDifference >= 3  // if a lot of bodies have been added,
			|| nbBodiesDifference < -5) // or if a lot of bodies have been removed,
		{
			// we optimize the broad phase
			// as per the Jolt doc:
			// This improves collision detection performance. You should definitely not call this every frame.
			p_physicsSystem_->OptimizeBroadPhase();
			// Only update Last Check when we optimize as to avoid incremental decrease not being detected as a big decrease
			nbBodiesLastCheck = nbBodiesCurrent;
		}
	}

	if (_deltaTime >= 0.01f)
	{
		// as per the Jolt doc:
		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable.
		// Do 1 collision step per 1 / 60th of a second (round up).
		const int collisionSteps = (int)(_deltaTime * 60.f);
		if (collisionSteps)
			p_physicsSystem_->Update(_deltaTime, collisionSteps, p_tempAllocator_, p_jobSystem_);
	}
	else
		DEBUG_WARNING("PHYSIC SYSTEM UPDATE SKIPPED ----------------------- JOLT PHYSIC ENGINE");
}