/*RauchGranate*/

#strict 2
#include RSGR

public func Explosion() {
Sound("explode_smoke");
Smoking(40);
RemoveObject();
}

public func Smoking(int iLevel) 
{
  var x = AbsX(GetX()),
  	  y = AbsY(GetY());
  var i=0, count = 3+iLevel/8, angle = Random(360);
  while(count > 0 && ++i < count*10) {
    angle += RandomX(40,80);
    var smokex = +Sin(angle,RandomX(iLevel/4,iLevel/2));
    var smokey = -Cos(angle,RandomX(iLevel/4,iLevel/2));
    if(GBackSolid(x+smokex,y+smokey))
      continue;
     var level = iLevel + Random(iLevel/5);
    CreateWhiteSmoke(level,angle,smokex,smokey,this);
    count--;
  }
}

private func CreateWhiteSmoke(int iStrength, int iAngle, int iX, int iY, object pObj) {
//  Log("I...");
    iX += GetX(pObj);
    iY += GetY(pObj);
  AddEffect("WhiteSmoke", 0, 300, 1, 0, 0, iStrength, iAngle, iX, iY);
}

global func FxWhiteSmokeStart(object pTarget, int iEffectNumber, int iTemp, iStrength, iAngle, iX, iY) {

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

global func FxWhiteSmokeTimer(object pTarget, int iEffectNumber, int iEffectTime) {
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
  
  for(var i=6;i<=18;i+=3)
  {
  	Schedule(Format("WhiteSmoke(%d,%d,%d,%d,%d)",iX,iY,iAStr,b,i),i,0);
  }
  
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

global func WhiteSmoke(int iX,int iY,int iAStr,int b,int iMulti)
{CreateParticle("WhiteSmoke",iX,iY-iMulti,RandomX(-4-iMulti/3,4+iMulti/3),RandomX(-1-iMulti/5,-4-iMulti/5),20+iAStr*(8+iMulti),b);}

public func TypeColor() {return RGB(255, 255, 255);}
