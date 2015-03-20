//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Sichtfeld  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2

public func Initialize()
{
for(clnk in FindObjects(Find_Func("HasView")))
	if(GetEffectCount("ViewRange2",clnk)==0)AddEffect("ViewRange2",clnk,2,1,clnk);
return _inherited();
}

public func IsChooseable() { return 1; }

public func Destruction()
{
for(clnk in FindObjects(Find_Func("HasView")))
	if(GetEffectCount("ViewRange2",clnk)>0)RemoveEffect("ViewRange2",clnk);
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}
