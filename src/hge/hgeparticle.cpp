/*
 * Thanks to Dr.Watson JGE++!
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeParticleSystem helper class implementation
*/

#include "hgeparticle.h"
#include "../nge_io_file.h"
#include "../nge_timer.h"
#include "../nge_graphics.h"
#include "HGEExport.h"

hgeParticleSystem::hgeParticleSystem(const char *filename, sprite_p sprite)
{
	int handle = io_fopen(filename,IO_RDONLY);
	if(handle<=0)
		return ;
	io_fread(&info,1,sizeof(hgeParticleSystemInfo),handle);
	io_fclose(handle);
//	hge->Random_Seed(0);
	info.sprite=sprite;
	if(info.sprite->sprite_src_blend == 0)
		info.sprite->sprite_src_blend = BLEND_SRC_ALPHA;
	if(info.sprite->sprite_des_blend == 0)
		info.sprite->sprite_des_blend = BLEND_ONE;
	
	vecLocation.x=vecPrevLocation.x=0.0f;
	vecLocation.y=vecPrevLocation.y=0.0f;
	fTx=fTy=0;

	fEmissionResidue=0.0f;
	nParticlesAlive=0;
	fAge=-2.0;

	rectBoundingBox.Clear();
	bUpdateBoundingBox=false;
}

hgeParticleSystem::hgeParticleSystem(hgeParticleSystemInfo *psi)
{
	memcpy(&info, psi, sizeof(hgeParticleSystemInfo));

	vecLocation.x=vecPrevLocation.x=0.0f;
	vecLocation.y=vecPrevLocation.y=0.0f;
	fTx=fTy=0;

	fEmissionResidue=0.0f;
	nParticlesAlive=0;
	fAge=-2.0;

	rectBoundingBox.Clear();
	bUpdateBoundingBox=false;
}

hgeParticleSystem::hgeParticleSystem(const hgeParticleSystem &ps)
{
	memcpy(this, &ps, sizeof(hgeParticleSystem));
}

void hgeParticleSystem::Update(float fDeltaTime)
{
	int i;
	float ang;
	hgeParticle *par;
	hgeVector vecAccel, vecAccel2;
	if(fAge >= 0)
	{
		fAge += fDeltaTime;
		if(fAge >= info.fLifetime) fAge = -2.0f;
	}

	mTimer += fDeltaTime;
	if (mTimer < 0.01f)
		return;

	fDeltaTime = mTimer;
	mTimer = 0.0f;


	// update all alive particles

	if(bUpdateBoundingBox) rectBoundingBox.Clear();
	par=particles;

	for(i=0; i<nParticlesAlive; i++)
	{
		par->fAge += fDeltaTime;
		if(par->fAge >= par->fTerminalAge)
		{
			nParticlesAlive--;
			memcpy(par, &particles[nParticlesAlive], sizeof(hgeParticle));
			i--;
			continue;
		}

		vecAccel = par->vecLocation-vecLocation;
		vecAccel.Normalize();
		vecAccel2 = vecAccel;
		vecAccel *= par->fRadialAccel;

		// vecAccel2.Rotate(M_PI_2);
		// the following is faster
		ang = vecAccel2.x;
		vecAccel2.x = -vecAccel2.y;
		vecAccel2.y = ang;

		vecAccel2 *= par->fTangentialAccel;
		par->vecVelocity += (vecAccel+vecAccel2)*fDeltaTime;
		par->vecVelocity.y += par->fGravity*fDeltaTime;

		//par->vecVelocity.y = 0.1f;
		par->vecLocation += par->vecVelocity;

		par->fSpin += par->fSpinDelta*fDeltaTime;
		par->fSize += par->fSizeDelta*fDeltaTime;
		par->colColor += par->colColorDelta*fDeltaTime;

		if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

		par++;
	}

	// generate new particles

	if(fAge != -2.0f)
	{
		float fParticlesNeeded = info.nEmission*fDeltaTime + fEmissionResidue;
		int nParticlesCreated = (unsigned int)fParticlesNeeded;
		fEmissionResidue=fParticlesNeeded-nParticlesCreated;

		par=&particles[nParticlesAlive];

		for(i=0; i<nParticlesCreated; i++)
		{
			if(nParticlesAlive>=MAX_PARTICLES) break;

			par->fAge = 0.0f;
			par->fTerminalAge = hge->Random_Float(info.fParticleLifeMin, info.fParticleLifeMax, true);

			par->vecLocation = vecPrevLocation+(vecLocation-vecPrevLocation)*hge->Random_Float(0.0f, 1.0f, true);
			par->vecLocation.x += hge->Random_Float(-2.0f, 2.0f, true);
			par->vecLocation.y += hge->Random_Float(-2.0f, 2.0f, true);

			ang=info.fDirection-M_PI_2+hge->Random_Float(0, info.fSpread, true)-info.fSpread/2.0f;
			if(info.bRelative) ang += (vecPrevLocation-vecLocation).Angle()+M_PI_2;
			par->vecVelocity.x = cosf(ang);
			par->vecVelocity.y = sinf(ang);
			par->vecVelocity *= hge->Random_Float(info.fSpeedMin, info.fSpeedMax, true);

			par->fGravity = hge->Random_Float(info.fGravityMin, info.fGravityMax, true);
			par->fRadialAccel = hge->Random_Float(info.fRadialAccelMin, info.fRadialAccelMax, true);
			par->fTangentialAccel = hge->Random_Float(info.fTangentialAccelMin, info.fTangentialAccelMax, true);

			par->fSize = hge->Random_Float(info.fSizeStart, info.fSizeStart+(info.fSizeEnd-info.fSizeStart)*info.fSizeVar, true);
			par->fSizeDelta = (info.fSizeEnd-par->fSize) / par->fTerminalAge;

			par->fSpin = hge->Random_Float(info.fSpinStart, info.fSpinStart+(info.fSpinEnd-info.fSpinStart)*info.fSpinVar, true);
			par->fSpinDelta = (info.fSpinEnd-par->fSpin) / par->fTerminalAge;

			par->colColor.r = hge->Random_Float(info.colColorStart.r, info.colColorStart.r+(info.colColorEnd.r-info.colColorStart.r)*info.fColorVar, true);
			par->colColor.g = hge->Random_Float(info.colColorStart.g, info.colColorStart.g+(info.colColorEnd.g-info.colColorStart.g)*info.fColorVar, true);
			par->colColor.b = hge->Random_Float(info.colColorStart.b, info.colColorStart.b+(info.colColorEnd.b-info.colColorStart.b)*info.fColorVar, true);
			par->colColor.a = hge->Random_Float(info.colColorStart.a, info.colColorStart.a+(info.colColorEnd.a-info.colColorStart.a)*info.fAlphaVar, true);

			par->colColorDelta.r = (info.colColorEnd.r-par->colColor.r) / par->fTerminalAge;
			par->colColorDelta.g = (info.colColorEnd.g-par->colColor.g) / par->fTerminalAge;
			par->colColorDelta.b = (info.colColorEnd.b-par->colColor.b) / par->fTerminalAge;
			par->colColorDelta.a = (info.colColorEnd.a-par->colColor.a) / par->fTerminalAge;

			if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

			nParticlesAlive++;
			par++;
		}
	}

	vecPrevLocation=vecLocation;
}

void hgeParticleSystem::MoveTo(float x, float y, BOOLu8 bMoveParticles)
{
	int i;
	float dx,dy;
	
	if(bMoveParticles)
	{
		dx=x-vecLocation.x;
		dy=y-vecLocation.y;

		for(i=0;i<nParticlesAlive;i++)
		{
			particles[i].vecLocation.x += dx;
			particles[i].vecLocation.y += dy;
		}

		vecPrevLocation.x=vecPrevLocation.x + dx;
		vecPrevLocation.y=vecPrevLocation.y + dy;
	}
	else
	{
		if(fAge==-2.0) { vecPrevLocation.x=x; vecPrevLocation.y=y; }
		else { vecPrevLocation.x=vecLocation.x;	vecPrevLocation.y=vecLocation.y; }
	}

	vecLocation.x=x;
	vecLocation.y=y;
}

void hgeParticleSystem::FireAt(float x, float y)
{
	Stop();
	MoveTo(x,y,false);
	Fire();
}

void hgeParticleSystem::Fire()
{
	mTimer = 0.0f;

	if(info.fLifetime==-1.0f) fAge=-1.0f;
	else fAge=0.0f;
}

void hgeParticleSystem::Stop(BOOLu8 bKillParticles)
{
	fAge=-2.0f;
	if(bKillParticles) 
	{
		nParticlesAlive=0;
		rectBoundingBox.Clear();
	}
}
#include "../nge_debug_log.h"
void hgeParticleSystem::Render()
{
	int i;
 	hgeParticle *par=particles;
	rectf *prectf = &info.sprite->sprite_clip;
	info.sprite->sprite_image->rcentrex = info.sprite->sprite_center.x;
	info.sprite->sprite_image->rcentrey = info.sprite->sprite_center.y;
	SetTexBlend(info.sprite->sprite_src_blend,info.sprite->sprite_des_blend);
	for(i=0; i<nParticlesAlive; i++)
	{
		RenderQuad(info.sprite->sprite_image,
			       (float)prectf->left,(float)prectf->top,(float)prectf->right,(float)prectf->bottom,//子图4元组
			       (float)(par->vecLocation.x+fTx), (float)(par->vecLocation.y+fTy), //屏幕坐标dx,dy
				   (float)par->fSize, (float)par->fSize,//size scale
				   (float)(par->fSpin*par->fAge*360),//angel rot
				   (int)(par->colColor.GetHWColor())//color mask
				   );
		par++;
	}
	ResetTexBlend();
}
