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

#include "3d_task.h"
#include "time_task.h"
#include "resource_manager2.h"
#include "gui.h"
#include <nel/3d/u_material.h>

#define MTPT_GUI_DEBUG_DISPLAY 0

//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Variables
//

const char CGuiObject::className[] = "CGuiObject";

Lunar<CGuiObject>::RegType CGuiObject::methods[] = 
{
	/*bind_method(CParticlesProxy, setMetatable),	
	bind_method(CParticlesProxy, getUserData),	
	bind_method(CParticlesProxy, setUserData),	
	*/
	bind_method(CGuiObject, getName),	
	{0,0}
};


int CGuiObject::getName(lua_State *luaSession)
{
	lua_pushstring(luaSession,name().c_str());
	return 1;
}




//
// Functions
//


#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

NLMISC::CVector CVectorMax(const NLMISC::CVector &a, const NLMISC::CVector &b)
{
	NLMISC::CVector res;
	res.x = max(a.x,b.x);
	res.y = max(a.y,b.y);
	res.z = max(a.z,b.z);
	
	return res;
}


void CGuiObjectManager::init()
{
	CGuiScriptManager::instance().init();
	CGuiListViewManager::instance().init();
	CGuiButtonManager::instance().init();
	CGuiSpacerManager::instance().init();
	CGuiBitmapManager::instance().init();
	CGuiFrameManager::instance().init();
	CGuiScaleManager::instance().init();
	CGuiTextManager::instance().init();
	CGuiBoxManager::instance().init();
	CGuiProgressBarManager::instance().init();
	
	//C3DTask::instance().driver().setCapture(true);
	_mouseListener = new CGuiMouseListener();
	_mouseListener->init();
	_mouseListener->addToServer(C3DTask::instance().driver().EventServer);

	_focus = 0;

	
}
	
void CGuiObjectManager::render()
{
	_mouseListener->update();

	float screenWidth = (float)C3DTask::instance().screenWidth();
	float screenHeight = (float)C3DTask::instance().screenHeight();
	C3DTask::instance().driver().setFrustum(CFrustum(0, screenWidth,screenHeight, 0, -1, 1, false));


	//nlinfo("(%f,%f) , (%f,%f)",a.x,a.y,b.x,b.y);

	list<CGuiObject *>::iterator it;
	for(it=objects.begin();it!=objects.end();it++)
	{
		CVector a(0,0,0);
		CVector b(screenWidth,screenHeight,0);
		CGuiObject *obj = *it;
		obj->render(a,b);
	}

	
	CVector a(0,0,0);
	CVector b(screenWidth,screenHeight,0);
	CGuiCustom::instance().render(a,b);
	
}

void CGuiObjectManager::update()
{
	CGuiCustom::instance().update();
}

void CGuiObjectManager::release()
{
	objects.clear();

	//C3DTask::instance().driver().setCapture(false);
	_mouseListener->removeFromServer(C3DTask::instance().driver().EventServer);
	_mouseListener = 0;
}

CGuiMouseListener &CGuiObjectManager::mouseListener()
{
	return *_mouseListener;
}


void CGuiObjectManager::focus(CGuiObject *object)
{
	_focus = object;
}

CGuiObject *CGuiObjectManager::focus()
{
	 return _focus;
}

void CGuiObjectManager::registerClass(const string &className,CreateObjectCB cb)
{
	_createFunctionMap.insert(string2CreateFunction::value_type(className,cb));
}

CGuiObject *CGuiObjectManager::create(const string &className)
{
	string2CreateFunction::iterator it;
	it = _createFunctionMap.find(className);
	if(it==_createFunctionMap.end())
		nlwarning("Class not defined : %s",className.c_str());
	nlassert(it!=_createFunctionMap.end());
	CreateObjectCB cb = (*it).second;
	CGuiObject *res=cb();
	if(res)
		res->setClassName(className);
	return res;
}

//
//
//
CGuiObject::CGuiObject()
{
	alignment( eAlignCenterVertical | eAlignCenterHorizontal );
	_position.set(0,0,0);
	_minSize.set(0,0,0);
	_name = "untitled";
}

CGuiObject::~CGuiObject()
{
	list<CGuiObject *>::iterator it,itDel;
	for(it=CGuiObjectManager::instance().objects.begin();it!=CGuiObjectManager::instance().objects.end();)
	{
		CGuiObject *obj = *it;
		if(obj==this)
		{
			itDel = it;
			it++;
			CGuiObjectManager::instance().objects.erase(itDel);
		}
		else
			it++;
	}
}

void CGuiObject::init()
{

}

void CGuiObject::update()
{
	
}

void CGuiObject::release()
{

}

string CGuiObject::getClassName()
{
	return _className;
}

void CGuiObject::setClassName(const string cname)
{
	_className = cname;
}


CVector CGuiObject::globalPosition(const CVector &pos,const CVector &maxSize)
{
	//CVector res(pos.x + position().x + xOffset(maxSize.x),pos.y + position().y + yOffset(maxSize.y),0);
	return pos + position() + offset(maxSize);
}

CVector CGuiObject::position()
{
	return _position;
}

void CGuiObject::position(const CVector &position)
{
	_position = position;
}


float CGuiObject::ToProportionalX(float x)
{
	return x/C3DTask::instance().screenWidth();
}

float CGuiObject::ToProportionalY(float y)
{
	return y/C3DTask::instance().screenHeight();	
}

CGuiObject::TGuiAlignment CGuiObject::alignment()
{
	return _alignment;
}

void CGuiObject::alignment(int alignment)
{
	_alignment = (TGuiAlignment )alignment;
}


CVector CGuiObject::offset(const CVector &maxSize)
{
	CVector res(0,0,0);

	float xspace = maxSize.x - width();
	float xoffset = 0;
	
	if( (alignment()&CGuiObject::eAlignExpandHorizontal) == 0 )
	{
		if(alignment()&eAlignCenterHorizontal)
			xoffset = xspace / 2;
		if(alignment()&eAlignLeft)
			xoffset = 0;
		if(alignment()&eAlignRight)
			xoffset = xspace;
		
		res.x = max(xoffset,0.0f);
	}
	

	float yspace = maxSize.y - height();
	float yoffset = 0;
	
	if( (alignment()&CGuiObject::eAlignExpandVertical) == 0 )
	{
		if(alignment()&eAlignCenterVertical)
			yoffset = yspace / 2;
		if(alignment()&eAlignUp)
			yoffset = 0;
		if(alignment()&eAlignBottom)
			yoffset = yspace;
		
		res.y = max(yoffset,0.0f);
	}
	
	return res;	
}



//TODO
CVector CGuiObject::expandSize(const CVector &maxSize)
{
	CVector res = size();

	
	if(alignment()&CGuiObject::eAlignExpandHorizontal)
		res.x = max(res.x,maxSize.x);
	if(alignment()&CGuiObject::eAlignExpandVertical)
		res.y = max(res.y,maxSize.y);

	return res;
}

CVector CGuiObject::size()
{
	CVector res(width(),height(),0);
	return res;
}


bool CGuiObject::focused()
{
	return CGuiObjectManager::instance().focus()==this;
}


bool CGuiObject::isIn(const CVector &point,const CVector &startPos,const CVector &size)
{
	
	//CVector globalPos = startPos + position() + offset(size);
	CVector globalPos = globalPosition(startPos,size);
	CVector expSize = expandSize(size);
	
	return globalPos.x<=point.x && point.x<=(globalPos.x+expSize.x) && globalPos.y<=point.y && point.y<=(globalPos.y+expSize.y) ;
	
}

void CGuiObject::_checkFocus(const CVector &pos,const CVector &maxSize)
{
	CVector mousePos = CGuiObjectManager::instance().mouseListener().position();
	if(isIn(mousePos,pos,maxSize))
	{
		CVector mousePressedPos = CGuiObjectManager::instance().mouseListener().pressedPosition();
		if(CGuiObjectManager::instance().mouseListener().Clicked && isIn(mousePressedPos,pos,maxSize))
		{
			CGuiObjectManager::instance().focus(this);
		}
	}
}


void CGuiObject::render(const CVector &pos,CVector &maxSize)
{
	CVector oldMaxSize = maxSize;
	_checkFocus(pos,maxSize);	
	_preRender(pos,maxSize);
	_render(pos,maxSize);
	_postRender(pos,maxSize);
	_renderedSize = maxSize;
	_renderedPos= pos;
#if MTPT_GUI_DEBUG_DISPLAY
	CGuiStretchedQuad quad;
	quad.material(CGuiBoxManager::instance().material());
	quad.size(maxSize);
	quad.position(globalPosition(pos,oldMaxSize));
	quad.render();
#endif
}



void CGuiObject::_preRender(const CVector &pos,CVector &maxSize)
{
}

void CGuiObject::_render(const CVector &pos,CVector &maxSize)
{

}

void CGuiObject::_postRender(const CVector &pos,CVector &maxSize)
{
}


float CGuiObject::minWidth()
{
	return _minSize.x;
}

float CGuiObject::minHeight()
{
	return _minSize.y;
}

void CGuiObject::minWidth(float minWidth)
{
	_minSize.x = minWidth;
}

void CGuiObject::minHeight(float minHeight)
{
	_minSize.y = minHeight;
}


CVector CGuiObject::minSize()
{
	return _minSize;
}

void CGuiObject::minSize(const CVector &minSize)
{
	_minSize = minSize;
}


float CGuiObject::width()
{
	float w = _width()+position().x;
	float mw = minWidth();
	return max(w,mw);
}

float CGuiObject::height()
{
	float h = _height();
	float mh = minHeight();
	return max(h,mh);
}

UMaterial CGuiObject::LoadBitmap(const string &filename)
{
	UTextureFile	*_texture;
	UMaterial _material;
	string res;
	
	res = CResourceManager::instance().get(filename);
	_texture= C3DTask::instance().driver().createTextureFile(res);
	nlassert(_texture);
	
	_material= C3DTask::instance().createMaterial();
	_material.setTexture(_texture);
	_material.setBlend(true);
	_material.setZFunc(UMaterial::always);
	_material.setDoubleSided();
	
	return _material;
}

string CGuiObject::name()
{
	return _name;
}


void CGuiObject::luaPush(lua_State *L)
{
	Lunar<CGuiObject>::push(L, this);
}

CGuiObject *CGuiObject::XmlCreateFromNode(CGuiXml *xml, xmlNodePtr node)
{
	bool isok;
	string className;
	isok = xml->doc.getPropertyString(className,node,"class");
	string name;
	isok = xml->doc.getPropertyString(name,node,"name");
	
	CGuiObject *res = CGuiObjectManager::instance().create(className);
	res->init(xml,node);
	return res;
}

void CGuiObject::init(CGuiXml *xml,xmlNodePtr node)
{
	_xml = xml;
	bool isok;
	string name;
	isok = xml->doc.getPropertyString(_name,node,"name");
	
	CVector v;
	if(xml->getVector(node,"minSize",v))
		minSize(v);
	if(xml->getVector(node,"position",v))
		position(v);

	TGuiAlignment align;
	if(xml->getAlignment(node,"alignment",align))
		alignment(align);

	xmlNodePtr snode = xml->doc.getFirstChildNode(node,"script");
	if(snode)
	{
		string onLoadScript;
		if(xml->getString(snode,"onLoad",onLoadScript))
		{
			lua_dostring(xml->LuaState,onLoadScript.c_str());
		}
	}

	
}

NLMISC::CVector CGuiObject::renderedSize()
{
	return _renderedSize;
}

NLMISC::CVector CGuiObject::renderedPos()
{
	return _renderedPos;
}
