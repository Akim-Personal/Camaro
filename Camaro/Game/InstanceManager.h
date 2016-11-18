#pragma once

#include <System/God.h>

class CSolid;
class CDynamic;

#ifdef USE_BOX2D
class CPhysicsProxy;
#endif

class CInstanceManager
{
private:
	typedef std::map<std::string, CSolid*> TSolidsMap;
	typedef std::map<std::string, CDynamic*> TDynamicsMap;

public:
	CInstanceManager();
	~CInstanceManager();

#ifdef USE_BOX2D
	CPhysicsProxy* GetDynamicPhysicsProxy(char* name);
#endif

	bool Init();
	bool InitPhysics();
	void Update();

private:
	template<typename T> void AddSolid(char* name);
	template<typename T> void AddDynamic(char* name);
	bool InitSolids();
	bool InitDynamics();

	TSolidsMap m_solids;
	TDynamicsMap m_dynamics;
};