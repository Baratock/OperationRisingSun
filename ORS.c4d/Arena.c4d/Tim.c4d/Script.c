/*-- TIM --*/

#strict 2

local spawn;

public func ContainedLeft(pBy) 	 	{ return Spawn(pBy); }
public func ContainedRight(pBy) 	{ return Spawn(pBy); }
public func ContainedDown(pBy) 		{ return Spawn(pBy); }
public func ContainedUp(pBy) 		{ return Spawn(pBy); }
public func ContainedDig(pBy) 		{ return Spawn(pBy); }
public func ContainedThrow(pBy) 	{ return Spawn(pBy); }

public func Check()
{
	var p = Contents(0,this);
	if(!p)
		return false;
	if(FindObject(GLMS))
		Spawn(p);
	return true;
}

public func Spawn(pBy)
{
	if(!GetEffect("Respawn",pBy))
	{
		pBy->Exit();
		RemoveObject(this,true);
	}
	var iEffect = GetEffect("Respawn",pBy);
	if(iEffect && !EffectVar(2,pBy,iEffect))
	{
		RemoveEffect(0,pBy,iEffect);
		Exit(pBy,0,10);
		RemoveObject(this,true);
	}
	return true;
}


/*--Effecte für den RespawnTimmer--*/


global func RespawnTimer(object pClonk,int iRespawnTime)
{
	if(!pClonk)
		pClonk = this;
	if(!pClonk)
		return false;
	var iTime = 5*36;
	if(iRespawnTime)
		iTime = iRespawnTime;
	AddEffect("Respawn",pClonk,1,1,0,0,iTime);
	return true;
}

/*
EffectVars
1 Respawnzeit
2 Blocker
*/

global func FxRespawnStart(object pTarget, int iEffectNumber, int iTemp, int iRespawnTime)
{
	EffectVar(1,pTarget,iEffectNumber) = iRespawnTime;
	EffectVar(2,pTarget,iEffectNumber) = true;
	//wenn sich der Clonk nirgens Drin befindet bitte ändern
	if(!Contained(pTarget))
	{
		var tim = CreateObject(TIM3,GetX(pTarget),GetY(pTarget),GetOwner(pTarget));
		SetCategory(GetCategory(tim) | C4D_Foreground,tim);
		SetGraphics(0,tim,GetID(pTarget),1,5,0,1,pTarget);
		SetVisibility(VIS_Owner,tim);
		Enter(tim, pTarget);
	}
}

global func FxRespawnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(!(((EffectVar(1,pTarget,iEffectNumber)-iEffectTime)%36)))
	{
		var i = (EffectVar(1,pTarget,iEffectNumber)-iEffectTime)/36;
		PlayerMessage(GetOwner(pTarget),Format("$RespawnTime$",i),pTarget);
		Log("%d",i);
	}
	if(!Contained(pTarget) && EffectVar(2,pTarget,iEffectNumber))
	{
		var tim = CreateObject(TIM3,GetX(pTarget),GetY(pTarget),GetOwner(pTarget));
		Enter(tim, pTarget);
		SetCategory(GetCategory(tim) | C4D_Foreground,tim);
		SetGraphics(0,tim,GetID(pTarget),1,5,0,1,pTarget);
		SetVisibility(VIS_Owner,tim);
	}
	if(EffectVar(1,pTarget,iEffectNumber) <= iEffectTime)
	{
		EffectVar(2,pTarget,iEffectNumber) = false;
		return -1;
	}
	return true;
}
