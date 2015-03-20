/*-- Flagge --*/

#strict 2

local pFlagPole;

protected func Initialize() {
 SetAction("Fly");
 SetOwner(NO_OWNER);
 SetColorDw(RGB(255,255,255),this);
 return true;
}

protected func Flying() {
 if(!pFlagPole)
  RemoveObject();
 if(GetWind() < 0)
  SetDir(DIR_Left);
 if(GetWind() >= 0)
  SetDir(DIR_Right);
 var dir = -1+2*GetDir();
 SetPosition(GetX(pFlagPole)+15*dir,GetY(pFlagPole)+25-LocalN("iProcess",pFlagPole)/2);
 if(LocalN("iFlaggers",pFlagPole))
  SetColorDw(GetTeamColor(LocalN("iFlaggers",pFlagPole)),this);
 else
  SetColorDw(RGB(255,255,255),this);
 return true;
}
