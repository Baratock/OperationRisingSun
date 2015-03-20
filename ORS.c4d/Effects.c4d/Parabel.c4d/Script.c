//------------------------------------------Flugbahn-----------------------------------------------
#strict 2

protected func Initialize()
{
	//man darf es nur selber sehen
  SetVisibility(VIS_Owner);
}

global func RemoveParabel(object pObj)
{
  //Nach sich selbst suchen an diesem Objekt
  var Parabel = FindObject2(Find_ID(PRBL), Find_ActionTarget(pObj));
  //... und entfernen
  if(Parabel) RemoveObject(Parabel);
}

global func AddParabel(object pObj, int xOffset, int yOffset, int iAngle, int iSpeed, int iColor)
{
  // Alte Parabel entfernen
  RemoveParabel(pObj);
  // Neues Hilfsobjekt erzeugen
  var Parabel = CreateObject(PRBL, GetX(pObj)-GetX(), GetY(pObj)-GetY(), GetOwner(pObj));
  Parabel->SetAction("Attach", pObj);
  if(!iColor)iColor=RGB(0,255,0);
	var iXDir =  Sin(iAngle,iSpeed);
	var iYDir = -Cos(iAngle,iSpeed);
  // Startwerte setzen
  var iXOld, iYOld;
  var iFaktor = 100;
  xOffset *= iFaktor; yOffset *= iFaktor;
  iYDir *= 5; iXDir *= 5;
  yOffset -= 4*iFaktor;
  iXOld = xOffset; iYOld = yOffset;
  
  // Flugbahn vorberechnen
  for(var i=0;i<400;i++)
  {
    // Geschwindigkeit und Gravitation einberechnen
    xOffset += iXDir;
    yOffset += iYDir + GetGravity()*i/20;
    if(Distance((iXOld-xOffset)/iFaktor, (iYOld-yOffset)/iFaktor)>=20)
    {
      CreateParticle("Punkt", xOffset/iFaktor-GetX(Parabel), yOffset/iFaktor-GetY(Parabel),
		      iXDir/400, iYDir/400, 10, iColor, Parabel);
      iXOld = xOffset; iYOld = yOffset;
    }
    //Beim erreichen von Material abbrechen
    if(GBackSolid(xOffset/iFaktor-GetX(), yOffset/iFaktor-GetY())) break;
  }
  
  return Parabel;
}

//wenn das Zeil weg ist, löschen
public func AttachTargetLost()
{
  RemoveObject();
}
