//---------------------------------- Allgemeine Funtionssammlung  ---------------------------------

#strict 2

global func AttachTo(object pBy) 
{
	SetAction("Attach",pBy);
}


//gibt keine fehlermeldung aus wenn das zielobjekt nicht existiert
global func _RemoveObject(object pTarget)
{
if(pTarget)RemoveObject(pTarget);
}
