//-------------------------------------  Splittergranate  -----------------------------------------
#strict 2
#include RSGR

public func Explosion()
{
for(var i=0;i<100;i++)
	{
	//var pSplitter = CreateObject(SPLT);
	var angle = RandomX(0,3600);
	var ammo = CreateObject(SPL2, Sin(angle, 5, 10), Cos(angle, 5, 10), GetOwner(pThrower));//Besitzer noch einfügen
	ammo->Launch(RandomX(0,3600),RandomX(100,150),200,1,0,25,0,0,10);
	}
	if(Contained()) DoDmg(200,DMG_Projectile,Contained(),10,GetController(Contained()));
	Explode(10,0,0,0,1,Format("handgrenade%d",RandomX(1,2)));
}
