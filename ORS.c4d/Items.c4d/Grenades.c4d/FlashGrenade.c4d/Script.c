//------------------------------------------ BlendGranate -----------------------------------------
#strict 2
#include RSGR

local ziele, flash;

public func Explosion()
{
    ziele = FindObjects(Find_OCF(OCF_Alive),Find_Distance(400));
    flash = CreateArray(GetLength(ziele));
    for(var i = 0;i < GetLength(ziele); i++)
    {
    	if(!Contained(ziele[i]) && PathFree(GetX(this),GetY(this),GetX(ziele[i]),GetY(ziele[i])+GetDefCoreVal("Height",0,GetID(ziele[i]))/3 + GetDefCoreVal("Offset",0,GetID(ziele[i]),1)))Blenden(RGBa(255,255,255,0),GetController(ziele[i]), ziele[i]);
    }
    Explode(10,0,0,0,1,Format("handgrenade%d",RandomX(1,2)));
}

public func Blenden(iColor, iPlr, pTarget) 
{
  flash[iPlr] = CreateObject(FLAS,0,0,iPlr);
  SetClrModulation(iColor, flash[iPlr]);
  SetVisibility(VIS_Owner, flash[iPlr]);
  LocalN("pFlashedClonk",flash[iPlr])=pTarget;
  flash[iPlr]->AttachTo(pTarget);
  //abhängigkeit von distanz und richtung einbauen
  var mod=10;
  if((GetX(this)<GetX(pTarget)&&GetDir(pTarget)==DIR_Right) || (GetX(this)>GetX(pTarget)&&GetDir(pTarget)==DIR_Left) )mod*=2;
  mod+=mod*Distance(GetX(this),GetY(this),GetX(pTarget),GetY(pTarget))/400;
  flash[iPlr]->SetModifier(mod);
}
