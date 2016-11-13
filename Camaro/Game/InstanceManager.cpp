#include "InstanceManager.h"

#include "Instances/Solid.h"
#include "Instances/Dynamic.h"
#include "Instances/Bunny.h"
#include "Instances/TestObject.h"

CInstanceManager::CInstanceManager()
{
	m_solids.clear();
	m_dynamics.clear();
}

template<typename T> void CInstanceManager::AddSolid(char* name)
{
	CSolid* solid = new CSolid(name);
	m_solids.insert(std::make_pair(solid->GetName(), solid));
}

template<typename T> void CInstanceManager::AddDynamic(char* name)
{
	T* dynamic = new T(name);
	m_dynamics.insert(std::make_pair(dynamic->GetName(), dynamic));
}

bool CInstanceManager::InitSolids()
{
	TSolidsMap::iterator solidIt;

	for (solidIt = m_solids.begin(); solidIt != m_solids.end();)
	{
		if (!solidIt->second->Init())
		{
			solidIt = m_solids.erase(solidIt);
			LogWarning("Solid could not be initialized and has been removed from map");
		}
		else
			++solidIt;
	}

	return true;
}

bool CInstanceManager::InitDynamics()
{
	TDynamicsMap::iterator dynamicIt;

	for (dynamicIt = m_dynamics.begin(); dynamicIt != m_dynamics.end();)
	{
		if (!dynamicIt->second->Init())
		{
			dynamicIt = m_dynamics.erase(dynamicIt);
			LogWarning("Dynamic could not be initialized and has been removed from map");
		}
		else
			++dynamicIt;
	}

	return true;
}

bool CInstanceManager::Init()
{
	// TODO: HERE WE SHOULD JUST GO THROUGH AN XML

	AddSolid<CSolid>("FirstSolid");
	AddSolid<CSolid>("SecondSolid");

	AddDynamic<CDynamic>("FirstDynamic");
	AddDynamic<CBunny>("BunnyDynamic");
	AddDynamic<CTestObject>("TestDynamic");

	if (!InitSolids() || !InitDynamics())
	{
		LogWarning("Not all Solids and Dynamics could be initialized");
		return false;
	}

	return true;
}

#ifdef USE_BOX2D
bool CInstanceManager::InitPhysics()
{
	// TODO: THIS INFO SHOULD ALSO BE GRABBED FROM THE XML FILE
	// GO THROUGH THE MAPS AND INITIALISE PHYSICS FOR EACH ELEMENT

	SSolidProperties props;
	props.type = e_Rectangle;
	props.boxWidth = 50.0f;
	props.boxHeight = 10.0f;
	props.posX = 0.0f;
	props.posY = -10.0f;

	CSolid* solid = NULL;

	TSolidsMap::iterator solidIt = m_solids.find("FirstSolid");
	if (solidIt != m_solids.end())
	{
		solid = solidIt->second;
		if (solid && solid->PreInitializePhysics((void*)&props))
		{
			if (!solid->InitPhysics())
			{
				LogError("Could not initialize Physics of Solid");
				return false;
			}
		}
		else
		{
			LogError("Could not pre-initialize Physics of Solid");
			return false;
		}
	}


	props.type = e_Circle;
	props.posX = 10.0f;
	props.posY = 0.0f;
	props.radius = 5.0f;

	solidIt = m_solids.find("SecondSolid");
	if (solidIt != m_solids.end())
	{
		solid = solidIt->second;
		if (solid && solid->PreInitializePhysics((void*)&props))
		{
			if (!solid->InitPhysics())
			{
				LogError("Could not initialize Physics of Solid");
				return false;
			}
		}
		else
		{
			LogError("Could not pre-initialize Physics of Solid");
			return false;
		}
	}


	SDynamicProperties dynProps;
	dynProps.type = e_Circle;
	dynProps.posX = -1.0f;
	dynProps.posY = 10.0f;
	dynProps.radius = 1.0f;

	CDynamic* dynamic = NULL;

	TDynamicsMap::iterator dynamicIt = m_dynamics.find("FirstDynamic");
	if (dynamicIt != m_dynamics.end())
	{
		dynamic = dynamicIt->second;
		if (dynamic && dynamic->PreInitializePhysics((void*)&dynProps))
		{
			if (!dynamic->InitPhysics())
			{
				LogError("Could not initialize Physics of Dynamic");
				return false;
			}
		}
		else
		{
			LogError("Could not pre-initialize Physics of Dynamic");
			return false;
		}
	}

	dynamicIt = m_dynamics.find("BunnyDynamic");
	if (dynamicIt != m_dynamics.end())
	{
		dynamic = dynamicIt->second;
		if (dynamic && dynamic->PreInitializePhysics(NULL))
		{
			if (!dynamic->InitPhysics())
				return false;
		}
		else
			return false;
	}

	dynamicIt = m_dynamics.find("TestDynamic");
	if (dynamicIt != m_dynamics.end())
	{
		dynamic = dynamicIt->second;
		if (dynamic && dynamic->PreInitializePhysics(NULL))
		{
			if (!dynamic->InitPhysics())
			{
				LogError("Could not initialize Physics of Dynamic");
				return false;
			}
		}
		else
		{
			LogError("Could not pre-initialize Physics of Dynamic");
			return false;
		}
	}

	return true;
}
#endif

void CInstanceManager::Update()
{
	TSolidsMap::iterator solidIt;
	TDynamicsMap::iterator dynamicIt;

	for (solidIt = m_solids.begin(); solidIt != m_solids.end(); ++solidIt)
		solidIt->second->Update();

	for (dynamicIt = m_dynamics.begin(); dynamicIt != m_dynamics.end(); ++dynamicIt)
		dynamicIt->second->Update();
}

#ifdef USE_BOX2D
CPhysicsProxy* CInstanceManager::GetDynamicPhysicsProxy(char* name)
{
	TDynamicsMap::iterator dynamicIt = m_dynamics.find(name);
	if (dynamicIt != m_dynamics.end())
		return dynamicIt->second->GetPhysicsProxy();
	else
		return NULL;
}
#endif

CInstanceManager::~CInstanceManager()
{
	TSolidsMap::iterator solidIt;
	TDynamicsMap::iterator dynamicIt;

	for (solidIt = m_solids.begin(); solidIt != m_solids.end(); ++solidIt)
		SAFE_DELETE(solidIt->second);

	for (dynamicIt = m_dynamics.begin(); dynamicIt != m_dynamics.end(); ++dynamicIt)
		SAFE_DELETE(dynamicIt->second);

	m_solids.clear();
	m_dynamics.clear();
}