/* Copyright, 2003 Melting Pot
 *
 * This file is part of MTP Target.
 * MTP Target is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * MTP Target is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with MTP Target; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */


//
// Includes
//

#include "stdpch.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <nel/misc/file.h>
#include <nel/misc/path.h>

#include <nel/3d/u_instance.h>

#include "level.h"
#include "3d_task.h"
#include "sky_task.h"
#include "hud_task.h"
#include "mtp_target.h"
#include "water_task.h"
#include "editor_task.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "lens_flare_task.h"
#include "config_file_task.h"
#include "resource_manager.h"
#include "../../common/lua_nel.h"
#include "../../common/lua_utility.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Types
//

//
// Declarations
//

//
// Variables
//


//
// Functions
//

CLevel::CLevel(const string &filename)
{
	nlinfo("Trying to load level '%s'", filename.c_str());

	Valid = false;
	_changed = false;

	DisplayStartPositions = CConfigFileTask::instance().configFile().getVar("DisplayStartPositions").asInt() == 1;
	DisplayLevel  = CConfigFileTask::instance().configFile().getVar("DisplayLevel").asInt() == 1;

	LuaState = luaOpenAndLoad(filename);

	luaGetGlobalVariable(LuaState, Name);
	nlinfo("level name '%s'", Name.c_str());
	
	luaGetGlobalVariable(LuaState, Author);
	nlinfo("author name '%s'", Author.c_str());
	
	vector<CLuaVector> Cameras;
	luaGetGlobalVector(LuaState, Cameras);
	
	for(uint i = 0; i < Cameras.size(); i++)
	{
		nlinfo("camera %g %g %g", Cameras[i].x, Cameras[i].y, Cameras[i].z);

		uint8 eid = CMtpTarget::instance().controler().getControledEntity();
		if (i == 0 || eid != 255 && CEntityManager::instance()[eid].rank() == (uint8)i)
		{
			CMtpTarget::instance().controler().Camera.setInitialPosition(Cameras[i]);
		}
	}

	vector<CLuaVector> luaStartPoints;
	luaGetGlobalVectorWithName(LuaState, luaStartPoints, "StartPoints");
	uint8 startPositionId = 0;
	for(uint i = 0; i < luaStartPoints.size(); i++)
	{
		nlinfo("%g %g %g", luaStartPoints[i].x, luaStartPoints[i].y, luaStartPoints[i].z);
		CAngleAxis Rotation(CVector(1,0,0),0);
		CStartPoint *startPoint = new CStartPoint();
		startPoint->init("start pos",startPositionId,luaStartPoints[i],Rotation);
		if (!DisplayStartPositions)
			startPoint->hide();
		StartPoints.push_back(startPoint);
		
			/*
		string res = CResourceManager::instance().get("col_box.shape");
		UInstance *inst = C3DTask::instance().scene().createInstance (res);
		if (!inst)
		{
			nlwarning ("Can't load 'col_box.shape'");
			return;
		}

		inst->setTransformMode(UTransformable::RotQuat);
		inst->setPos(StartPoints[i]);

		if (!DisplayStartPositions)
			inst->hide();

		StartPositions.push_back(inst);
		*/
		startPositionId++;
	}


	// Load modules
	lua_getglobal(LuaState, "Modules");
	lua_pushnil(LuaState);
	uint8 moduleId = 0;
	while(lua_next(LuaState, -2) != 0)
	{
		// `key' is at index -2 and `value' at index -1

		// Get module position
		CLuaVector Position;
		lua_pushstring(LuaState,"Position");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Position);		
		nlinfo("pos %g %g %g", Position.x, Position.y, Position.z);
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module rotation
		CLuaAngleAxis Rotation;
		lua_pushstring(LuaState,"Rotation");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Rotation);		
		nlinfo("rot %g %g %g %g", Rotation.Axis.x , Rotation.Axis.y, Rotation.Axis.z, Rotation.Angle);
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module name
		string Name;
		lua_pushstring(LuaState,"Name");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Name);		
		nlinfo("name %s", Name.c_str());
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration

		CModule *module = new CModule();
		module->init(Name,moduleId,Position,Rotation);
		moduleId++;
		if(!DisplayLevel)
			module->hide();
	
		Modules.push_back(module);

	}
	lua_pop(LuaState, 1);  // removes `key'

	FileName = filename;

	DisplayStartPositions = false;
	DisplayLevel = true;

	Valid = true;
	if(C3DTask::instance().levelParticle()!=0)
		C3DTask::instance().levelParticle()->show();


	CLuaRGBA sunAmbientColor(82, 100, 133, 255);
	luaGetGlobalVariable(LuaState, sunAmbientColor);
	nlinfo("sunAmbientColor = %d %d %d %d", sunAmbientColor.R, sunAmbientColor.G, sunAmbientColor.B, sunAmbientColor.A);
	C3DTask::instance().driver().setAmbientColor(sunAmbientColor);
	C3DTask::instance().scene().setSunAmbient(sunAmbientColor);
	
	CLuaRGBA sunDiffuseColor(255,255,255,255);
	luaGetGlobalVariable(LuaState, sunDiffuseColor);
	C3DTask::instance().scene().setSunDiffuse(sunDiffuseColor);
	
	CLuaRGBA sunSpecularColor(255,255,255,255);
	luaGetGlobalVariable(LuaState, sunSpecularColor);
	C3DTask::instance().scene().setSunDiffuse(sunSpecularColor);
	
	CLuaVector sunDirection(-1,0,-1);
	luaGetGlobalVariable(LuaState, sunDirection);
	C3DTask::instance().scene().setSunDirection(sunDirection);
	
	
	string skyShapeFileName;
	luaGetGlobalVariable(LuaState, skyShapeFileName);
	nlinfo("skyShapeFileName '%s'", skyShapeFileName.c_str());
	CSkyTask::instance().shapeName(skyShapeFileName);
	
	CTaskManager::instance().add(CSkyTask::instance(), 100);
	


	string skyEnvMap0Name;
	luaGetGlobalVariable(LuaState, skyEnvMap0Name);
	nlinfo("skyEnvMap0Name '%s'", skyEnvMap0Name.c_str());
	CWaterTask::instance().envMap0Name(skyEnvMap0Name);
	string skyEnvMap1Name;
	luaGetGlobalVariable(LuaState, skyEnvMap1Name);
	nlinfo("skyEnvMap1Name '%s'", skyEnvMap1Name.c_str());
	CWaterTask::instance().envMap1Name(skyEnvMap1Name);
	string skyHeightMap0Name;
	luaGetGlobalVariable(LuaState, skyHeightMap0Name);
	nlinfo("skyHeightMap0Name '%s'", skyHeightMap0Name.c_str());
	CWaterTask::instance().heightMap0Name(skyHeightMap0Name);
	string skyHeightMap1Name;
	luaGetGlobalVariable(LuaState, skyHeightMap1Name);
	nlinfo("skyHeightMap1Name '%s'", skyHeightMap1Name.c_str());
	CWaterTask::instance().heightMap1Name(skyHeightMap1Name);
	CTaskManager::instance().add(CWaterTask::instance(), 101);
	

	CTaskManager::instance().add(CLensFlareTask::instance(), 140);
	
}


CLevel::~CLevel()
{
	CTaskManager::instance().remove(CSkyTask::instance());
	CTaskManager::instance().remove(CLensFlareTask::instance());

	if(C3DTask::instance().levelParticle()!=0)
		C3DTask::instance().levelParticle()->hide();

	if(changed())
		CResourceManager::instance().refresh(FileName);

	CEditorTask::instance().reset();

	if(LuaState)
		luaClose(LuaState);

	// removing all stuffs


	for(uint i = 0; i < Modules.size(); i++)
	{
		delete Modules[i];
	}
	Modules.clear();

	for(uint j = 0; j < StartPoints.size(); j++)
	{
		delete StartPoints[j];
		//C3DTask::instance().scene().deleteInstance(StartPositions[j]);
	}
	StartPoints.clear();
}

CVector CLevel::startPosition(uint32 id)
{
	return StartPoints[id]->position();
}

void CLevel::reset()
{
	nlinfo("--level reset--");
	luaClose(LuaState);

	LuaState = luaOpenAndLoad(FileName);
	
	luaGetGlobalVariable(LuaState, Name);
	nlinfo("level name '%s'", Name.c_str());
	
	luaGetGlobalVariable(LuaState, Author);
	nlinfo("author name '%s'", Author.c_str());
	
	vector<CLuaVector> Cameras;
	luaGetGlobalVector(LuaState, Cameras);
	
	for(uint i = 0; i < Cameras.size(); i++)
	{
		nlinfo("camera %g %g %g", Cameras[i].x, Cameras[i].y, Cameras[i].z);
		uint8 eid = CMtpTarget::instance().controler().getControledEntity();
		if (i == 0 || eid != 255 && CEntityManager::instance()[eid].rank() == (uint8)i)
		{
			CMtpTarget::instance().controler().Camera.setInitialPosition(Cameras[i]);
		}
	}
	
/*


	vector<CLuaVector> StartPoints;
	luaGetGlobalVector(LuaState, StartPoints);
	for(uint i = 0; i < StartPoints.size(); i++)
	{
		nlinfo("%g %g %g", StartPoints[i].x, StartPoints[i].y, StartPoints[i].z);
		string res = CResourceManager::instance().get("col_box.shape");
		UInstance *inst = StartPositions[i];
		
		inst->setTransformMode(UTransformable::RotQuat);
		inst->setPos(StartPoints[i]);
		
		if (!DisplayStartPositions)
			inst->hide();
		
		StartPositions.push_back(inst);
	}
	
	
	// Load modules
	lua_getglobal(LuaState, "Modules");
	lua_pushnil(LuaState);
	int j = 0;
	while(lua_next(LuaState, -2) != 0)
	{
		// `key' is at index -2 and `value' at index -1
		
		// Get module position
		CLuaVector Position;
		lua_pushstring(LuaState,"Position");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Position);		
		nlinfo("pos %g %g %g", Position.x, Position.y, Position.z);
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module name
		string Name;
		lua_pushstring(LuaState,"Name");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Name);		
		nlinfo("name %s", Name.c_str());
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		string res = CResourceManager::instance().get(Name+".shape");
		UInstance *inst = Meshes[j++];
		if (inst == 0)
		{
			nlwarning ("Can't load '%s.shape'", Name.c_str());
		}
		else
		{
			inst->setTransformMode(UTransformable::RotQuat);
			inst->setPos(Position);
			
			if (!DisplayLevel)
				inst->hide();
		}
	}
	lua_pop(LuaState, 1);  // removes `key'
	*/
}


void CLevel::display(CLog *log) const
{
	log->displayNL("LevelName = %s", name().c_str());
	log->displayNL("There's %d starting points", StartPoints.size());
	log->displayNL("There's %d Modules", Modules.size());
}

void CLevel::displayStartPositions(bool b)
{
	DisplayStartPositions = b;
	
	if(DisplayStartPositions)
	{
		for(uint j = 0; j < StartPoints.size(); j++)
		{
			StartPoints[j]->show();
		}
	}
	else
	{
		for(uint j = 0; j < StartPoints.size(); j++)
		{
			StartPoints[j]->hide();
		}
	}
}

void CLevel::displayLevel(bool b)
{
	DisplayLevel = b;
	
	if(DisplayLevel)
	{
		for(uint i = 0; i < Modules.size(); i++)
		{
			Modules[i]->mesh()->show ();
		}
	}
	else
	{
		for(uint i = 0; i < Modules.size(); i++)
		{
			Modules[i]->mesh()->hide ();
		}
	}
}


CModule *CLevel::getModule(uint32 index)
{
	nlassert(index<getModuleCount());
	return Modules[index];
}

uint32 CLevel::getModuleCount()
{
	nlassert(Modules.size()<255);
	return Modules.size();
}

void CLevel::updateModule(uint32 id,CVector pos,CVector rot,uint32 selectedBy)
{
	//TODO selectedBy
	getModule(id)->update(pos,rot);//,selectedBy);
}

CStartPoint *CLevel::getStartPoint(uint32 index)
{
	nlassert(index<getStartPointCount());
	return StartPoints[index];
}

uint32 CLevel::getStartPointCount()
{
	nlassert(StartPoints.size()<255);
	return StartPoints.size();
}

void CLevel::updateStartPoint(uint32 id,CVector pos,CVector rot,uint32 selectedBy)
{
	//TODO selectedBy
	getStartPoint(id)->update(pos,rot);//,selectedBy);
}

