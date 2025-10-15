// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

FAbility::FAbility()
{
}

FAbility::~FAbility()
{
}

FAbility& FAbility::operator+(FAbility& ps)
{
	for (EAbility type : TEnumRange<EAbility>())
	{
		Value[type] += ps.Value[type];
	}

	return *this;
}

FAbility& FAbility::operator+=(FAbility& ps)
{
	for (EAbility type : TEnumRange<EAbility>())
	{
		Value[type] += ps.Value[type];
	}

	return *this;
}

FAbility& FAbility::operator-(FAbility& ps)
{
	for (EAbility type : TEnumRange<EAbility>())
	{
		Value[type] -= ps.Value[type];
	}

	return *this;
}

FAbility& FAbility::operator-=(FAbility& ps)
{
	for (EAbility type : TEnumRange<EAbility>())
	{
		Value[type] -= ps.Value[type];
	}

	return *this;
}

bool FAbility::operator<(FAbility& ps)
{
	for (EAbility type : TEnumRange<EAbility>())
	{
		if (Value[type] > ps.Value[type])
		{
			return false;
		}
	}

	return true;
}

bool FAbility::operator>(FAbility& ps)
{
	for (EAbility type : TEnumRange<EAbility>())
	{
		if (Value[type] < ps.Value[type])
		{
			return false;
		}
	}

	return true;
}
