/*-- Holz Kiste --*/

#strict 2

private func MaxDamage() {return 280;}

protected func Initialize() {
 return true;
}

protected func IsBulletTarget() {return true;}

protected func Hit() {
 Sound("WoodHit*");
 return true;
}

protected func Damage(int iChange, int iByPlayer) {
 if(GetDamage()+iChange > MaxDamage())
 {
 CastObjects(WOSP,10,50,0,0);
 RemoveObject(this,true);
 }
}
