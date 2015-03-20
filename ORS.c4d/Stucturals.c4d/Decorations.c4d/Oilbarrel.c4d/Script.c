/*-- Ölfass --*/

#strict 2

protected func Initialize()
{
  return true;  
}
protected func MaxDamage()		{ return 55;				}
protected func IsBulletTarget()	{ return true;				}

protected func Hit()	{return Sound("ClonkHit*");	}
protected func Hit3()	{return DoDamage(30); }

protected func Damage(int iChange, int iByPlayer)
{
 if(iChange+GetDamage(this) >= 7)
  Incinerate (this);
 if(GetDamage()+iChange > MaxDamage())
 {
  CreateObject(D_BO,0,-1,-1);
  Explode(36);
 }
 return iChange;
}

