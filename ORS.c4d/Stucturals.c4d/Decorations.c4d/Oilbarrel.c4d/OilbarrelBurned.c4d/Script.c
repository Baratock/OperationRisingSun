/*-- Verbranntes Fass --*/
#strict 2

func Initialize()
{
 AddEffect("FadeOut",this,1,2,this,0,255);
 return true;
}

protected func FxFadeOutStart(object pTarget, int iEffectNumber, int iTemp, int iAlpha)
{
 EffectVar(1,pTarget,iEffectNumber) = iAlpha;
 return true;
}

protected func FxFadeOutTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 SetClrModulation(RGBa(255,255,255,255-EffectVar(1,pTarget,iEffectNumber)--),pTarget);
 if(!EffectVar(1,pTarget,iEffectNumber))
  return RemoveObject(pTarget,true);
 return true;
}
