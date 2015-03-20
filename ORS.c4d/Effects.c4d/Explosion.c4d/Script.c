/* Explosion */
#strict 2

global func Explode(int iLevel, object pObj, id idEffect, string szEffect, bool bNoShots, string sSound) 
{
  if(!pObj)
  if(!(pObj=this))return 0;
  var x = AbsX(pObj->GetX()),
  	  y = AbsY(pObj->GetY());
  var i=0, count = 3+iLevel/8, angle = Random(360);
  while(count > 0 && ++i < count*10) {
    angle += RandomX(40,80);
    if(!bNoShots)
      if(GetName(0,SHT1)) { //Gibt es das Objekt? (HAX)
        var ammo = CreateObject(SHT1,x,y,GetController(pObj));
        ammo->Launch(angle,100+iLevel*3,Max(1,RandomX(30,iLevel*8)),4,50, 3, 0,1);
      }
    var smokex = +Sin(angle,RandomX(iLevel/4,iLevel/2));
    var smokey = -Cos(angle,RandomX(iLevel/4,iLevel/2));
    if(GBackSolid(x+smokex,y+smokey))
      continue;
     var level = iLevel + Random(iLevel/5);
    CreateSmokeTrail(level,angle,smokex,smokey,pObj);
    count--;
  }

  CreateBurnMark(x,y,iLevel,20+iLevel/2);

  if(IsDark()) {
    var iSize = iLevel*100;
    if(iLevel < 20) {
      iSize /= 2;
    }
    AddLightFlash(iSize/3, x, y, RGBa(255,220,64,15));
  }
  ShakeViewPort(iLevel, 0, GetX(pObj), GetY(pObj));
  // Sound muss vor dem Löschen des Objektes erzeugt werden, damit die Position stimmt
  var grade = BoundBy((iLevel/10)-1,1,3);
  if(!sSound)Sound(Format("Blast%d", grade), false, pObj);
  else Sound(sSound,0,pObj);
  if (!pObj) if (!(pObj = this())) return false;
  var x=GetX(pObj), y=GetY(pObj);
  var cause_plr = GetController(pObj);
  var container = Contained(pObj);
  var exploding_id = GetID(pObj);
  var layer = GetObjectLayer(pObj);
  RemoveObject(pObj);
  exploding_id->DoExplosion(x, y, iLevel, container, cause_plr, idEffect, szEffect, layer);
}

global func CreateSmokeTrail(int iStrength, int iAngle, int iX, int iY, object pObj) {
//  Log("I...");
    iX += GetX(pObj);
    iY += GetY(pObj);
  AddEffect("SmokeTrail", 0, 300, 1, 0, 0, iStrength, iAngle, iX, iY);
}

global func FxSmokeTrailStart(object pTarget, int iEffectNumber, int iTemp, iStrength, iAngle, iX, iY) {

  if(iTemp)
    return 0;
  
  if(iAngle%90 == 1) iAngle += 1;
  iStrength = Max(iStrength,5);

  EffectVar(0, pTarget, iEffectNumber) = iStrength;
  EffectVar(1, pTarget, iEffectNumber) = iStrength;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(4, pTarget, iEffectNumber) = +Sin(iAngle,iStrength*40);
  EffectVar(5, pTarget, iEffectNumber) = -Cos(iAngle,iStrength*40);
}

global func FxSmokeTrailTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var iStrength = EffectVar(0, pTarget, iEffectNumber);
  var iAStr = EffectVar(1, pTarget, iEffectNumber);
  var iX = EffectVar(2, pTarget, iEffectNumber);
  var iY = EffectVar(3, pTarget, iEffectNumber);
  var iXDir = EffectVar(4, pTarget, iEffectNumber);
  var iYDir = EffectVar(5, pTarget, iEffectNumber);
  iAStr = Max(1,iAStr-iAStr/10+Random(2));
  iAStr--;
  iYDir += GetGravity()*2/3;
  var xdir = iXDir*iAStr/iStrength;
  var ydir = iYDir*iAStr/iStrength;
  iX += RandomX(-3,3);
  iY += RandomX(-3,3);
  var b = RGBa(75,65,55,55+35*iAStr/iStrength);
  
  CreateParticle("Smoke3",iX,iY,RandomX(-2,2),RandomX(-2,4),20+iAStr*8,b);
  for(var i=6;i<=18;i+=3)
  {
  	Schedule(Format("ExplosionSmoke(%d,%d,%d,%d,%d)",iX,iY,iAStr,b,i),i,0);
  }
  CreateParticle("Blast",iX,iY,0,0,10+iAStr*8,RGBa(250,100+Random(100),100,160));
  
  iX += xdir/100;
  iY += ydir/100;
  
  if(GBackSemiSolid(iX,iY))
    return(-1);
  if(iAStr <= 1)
    return(-1);
    
  EffectVar(1, pTarget, iEffectNumber) = iAStr;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(5, pTarget, iEffectNumber) = iYDir;
}
global func CreateBurnMark(int iX, int iY, int iLevel, int Count) 
{
  var boom;
  if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
  else boom = FindObject(BOOM);
  
  var angle=Random(360/Count); //variablen für die überprüfung
  var type;
  for(var z; z < Count; z++) {

    angle += Max(1,360/Count);

    // Check: Sky or Solid/Liquid
    var x = iX+Cos(angle,iLevel);
    var y = iY+Sin(angle,iLevel);
    if(GetMaterialVal("Density","Material",GetMaterial(x,y)) != 0
       || GetMaterial(x,y) == -1)
      continue;

    type = 1+Random(3);
    var size = BurnMarkCheck(angle,RandomX(iLevel/2,iLevel*2),iX,iY);
    var sin = Sin(angle,(size-iLevel)/2+iLevel+Random(3));
    var cos = Cos(angle,(size-iLevel)/2+iLevel+Random(3));

    CreateParticle("BurnMark",iX+cos,iY+sin,Cos(angle+RandomX(-5,5),50),Sin(angle+RandomX(-5,5),50),size*5+Random(25),RGBa(0,0,0,64),boom,1); 
  }

}

global func BurnMarkCheck(int angle,int size, int iX, int iY) {
  var sin = Sin(angle,size);
  var cos = Cos(angle,size);
  var x = cos, y = sin, i=100;
  while(GetMaterial(iX+x,iY+y) == -1 || GetMaterialVal("Density","Material",GetMaterial(iX+x,iY+y)) != 0) {
    x = cos*i/100;
    y = sin*i/100;
    if(i <= 0) return(0);
    i-=10;
  }
  return(size);
}

global func ExplosionSmoke(int iX,int iY,int iAStr,int b,int iMulti)
{CreateParticle("Smoke10",iX,iY-iMulti,RandomX(-4-iMulti/3,4+iMulti/3),RandomX(-1-iMulti/5,-4-iMulti/5),20+iAStr*(8+iMulti),b);}
