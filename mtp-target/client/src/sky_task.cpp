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

#include <nel/3d/u_scene.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_cloud_scape.h>
#include <nel/3d/u_text_context.h>

#include "3d/nelu.h"
#include "3d/scene.h"
#include "time_task.h"
#include "3d/scene_user.h"
#include "3d/water_model.h"
#include "3d/water_shape.h"
#include "3d/texture_file.h"
#include "3d/texture_blend.h"
#include "3d/transform_shape.h"
#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "3d_task.h"
#include "sky_task.h"
#include "time_task.h"
#include "game_task.h"
#include "chat_task.h"
#include "score_task.h"
#include "network_task.h"
#include "font_manager.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "background_task.h"
#include "resource_manager.h"
#include "config_file_task.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Variables
//


//
// Functions
//


CSkyTask::CSkyTask():ITask()
{
	shapeName("");

	nelSkyScene   = NULL;
	nelSkyMesh    = NULL;
	nelCloudScape = NULL;
}


void CSkyTask::init()
{
	nelSkyScene = C3DTask::instance().driver().createScene(false);

	nelSkyScene->getCam ()->setPerspective(degToRad(CConfigFileTask::instance().configFile().getVar("Fov").asFloat()), 1.33f, 0.15f, 3000.0f);
	nelSkyScene->getCam ()->setTransformMode (UTransformable::DirectMatrix);

	string res = CResourceManager::instance().get(shapeName());
	nelSkyMesh = nelSkyScene->createInstance(res);
	if (nelSkyMesh)
	{
		nelSkyMesh->setTransformMode (UTransformable::DirectMatrix);
		nelSkyMesh->setMatrix(CMatrix::Identity);
	}

////
	if(CConfigFileTask::instance().configFile().getVar("DisplayClouds").asInt() == 1)
	{
		nelCloudScape = nelSkyScene->createCloudScape();
		SCloudScapeSetup css;	
		css.NbCloud = 50;
		css.CloudSpeed = 8.0f;
		css.WindSpeed = 1.5f;

		css.Ambient = NLMISC::CRGBA (64, 72, 132, 255);
		css.Diffuse = NLMISC::CRGBA (167, 177, 236, 255);

		//css.Ambient = NLMISC::CRGBA (120,150,155,255);
		//css.Diffuse = NLMISC::CRGBA (220,250,255,255);
		nelCloudScape->setNbCloudToUpdateIn80ms (1);
		nelCloudScape->setQuality (160.0);
		//nelCloudScape->setDebugQuad (true);
		nelCloudScape->init (&css);	
	}
}

void CSkyTask::update()
{
	CMatrix skyCameraMatrix;
	skyCameraMatrix.identity();
	skyCameraMatrix = C3DTask::instance().scene().getCam()->getMatrix();
	skyCameraMatrix.setPos(CVector::Null);
	
	nelSkyScene->getCam()->setMatrix(skyCameraMatrix);
	
	nelSkyScene->animate (CTimeTask::instance().time());
	
	if (nelCloudScape)
		nelCloudScape->anim (CTimeTask::instance().deltaTime()); // WARNING this function work with screen
}

void CSkyTask::render()
{
	C3DTask::instance().driver().clearZBuffer();
	nelSkyScene->render ();
	
	// Must clear ZBuffer For incoming rendering.
	//C3DTask::instance().driver().clearZBuffer();
	
	if (nelCloudScape)
		nelCloudScape->render ();
}

void CSkyTask::release()
{
	if(nelSkyMesh)
	{
		nelSkyScene->deleteInstance(nelSkyMesh);
		nelSkyMesh = NULL;
	}

	if(nelCloudScape)
	{
		nelSkyScene->deleteCloudScape(nelCloudScape);
		nelCloudScape = NULL;
	}

	if(nelSkyScene)
	{
		C3DTask::instance().driver().deleteScene(nelSkyScene);
		nelSkyScene = NULL;
	}

	
}


void CSkyTask::shapeName(std::string shapeName)
{
	ShapeName = "sky.shape";
	if(shapeName.empty()) return;
	ShapeName = shapeName;
}

string CSkyTask::shapeName()
{
	return ShapeName;
}

