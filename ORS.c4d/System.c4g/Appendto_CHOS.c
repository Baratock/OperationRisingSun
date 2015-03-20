/*-- Spielregelwähler-Appendto --*/

#strict 2
#appendto CHOS

protected func Initialize()
{
	_inherited();
	SetStdRules([WPCH, RDMG]);
	return true;
}

private func SetStdRules(array aStd)
{
	for(var i=0, idR; idR = GetDefinition(i, Chooser_Cat) ; i++)
	{
		if(DefinitionCall(idR, "IsChooseable"))
		{
			for(a in aStd)
				if(a == idR)
					aRules[i] = true;
		}
		else
			aRules[i] = 0;
	}
	return true;
}
