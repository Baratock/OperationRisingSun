/*-- Fadenkreuz --*/

#strict

protected func Initialize()
{
  SetVisibility(VIS_Owner());
}

protected func Check()
{
  if(GetAction(GetActionTarget()) != "GrenadeAim") return RemoveObject();
}
