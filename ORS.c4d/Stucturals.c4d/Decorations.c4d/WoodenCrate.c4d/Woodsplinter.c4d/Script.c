/*-- Holzsplitter --*/

#strict 2

protected func Initialize() {
 //Effekt für Ausfaden des Splitters erstellen
  AddEffect("SpFadeOut",this,101,3,0,WOSP);
}
//Die Effektintialisierung
protected func FxSpFadeOutStart(object pTarget, int iEffectNumber, int iTemp) {
 EffectVar(1,pTarget,iEffectNumber) = 0;
}

//DER Timer
protected func FxSpFadeOutTimer(object pTarget, int iEffectNumber, int iEffectTime) {
 if(EffectVar(1,pTarget,iEffectNumber) >= 220)
  return -1;
 SetClrModulation(RGBa(255,255,255,EffectVar(1,pTarget,iEffectNumber)), pTarget);
 EffectVar(1,pTarget,iEffectNumber) += 3;
 return true;
}

protected func FxSpFadeOutStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
 return RemoveObject(pTarget);
}
