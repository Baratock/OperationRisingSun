//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  RealisticDamage  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2

public func Initialize()
{
for(clnk in FindObjects(Find_Func("IsRealHurtable")))
	{
	if(GetEffectCount("AnatomicMaster",clnk)==0)AddEffect("AnatomicMaster",clnk,2,1,clnk);
	}
return _inherited();
}

public func IsChooseable() { return 1; }

public func Destruction()
{
for(clnk in FindObjects(Find_Func("IsRealHurtable")))
	{
	if(GetEffectCount("AnatomicMaster",clnk)>0)RemoveEffect("AnatomicMaster",clnk);
	}
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}
