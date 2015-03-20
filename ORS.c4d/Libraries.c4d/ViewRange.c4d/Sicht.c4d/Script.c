//-------------------------------------------- View -----------------------------------------------
#strict 2
local pViewer;

public func Construction(object creator)
{
pViewer=creator;
}

public func Initialize() 
{
	AddEffect("Existenz",this,20,1,this);
  return 1;
}

protected func FxExistenzTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!pViewer)return RemoveObject(this);
if(!GetAlive(pViewer))return RemoveObject(this);
}
