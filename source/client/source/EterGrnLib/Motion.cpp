#include "StdAfx.h"
#include "Motion.h"

CGrannyMotion::CGrannyMotion()
{
	Initialize();
}

CGrannyMotion::~CGrannyMotion()
{
	Destroy();
}

bool CGrannyMotion::IsEmpty()
{
	return m_pgrnAni ? false : true;
}

void CGrannyMotion::Destroy()
{
	Initialize();
}

void CGrannyMotion::Initialize()
{
	m_pgrnAni = NULL;
}

bool CGrannyMotion::BindGrannyAnimation(granny_animation * pgrnAni)
{
	assert(IsEmpty());

	m_pgrnAni = pgrnAni;
	return true;
}

granny_animation* CGrannyMotion::GetGrannyAnimationPointer() const
{
	return m_pgrnAni;
}

const char * CGrannyMotion::GetName() const
{
	return m_pgrnAni->Name;
}

float CGrannyMotion::GetDuration() const
{
	return m_pgrnAni->Duration;
}
