//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Sichtfeld  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2

public func Initialize()
{
for(clnk in FindObjects(Find_Func("HasView")))
	{
	if(!LocalN("pView",clnk))LocalN("pView",clnk)=clnk->CreateObject(VIEW,0,0,GetController(clnk));
	if(GetEffectCount("ViewRange1",clnk)==0)AddEffect("ViewRange1",clnk,1,1,clnk);
	}
return _inherited();
}

public func IsChooseable() { return 1; }

public func Destruction()
{
for(clnk in FindObjects(Find_Func("HasView")))
	{
	if(LocalN("pView",clnk))RemoveObject(LocalN("pView",clnk));
	if(GetEffectCount("ViewRange1",clnk)>0)RemoveEffect("ViewRange1",clnk);
	}
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}
