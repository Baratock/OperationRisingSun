/*-- Crosshair Appendto --*/

#strict 2
#appendto HCRH

protected func Check() {
	var user = GetActionTarget();
	var wepn = Contents(0, user);
	if(wepn->~IsWeapon())
	{
		
		if(wepn->~GetFMData(FM_NoCrosshair))
			SetVisibility(VIS_None);
		else
			SetVisibility(VIS_Owner);
	}
	_inherited();
}
