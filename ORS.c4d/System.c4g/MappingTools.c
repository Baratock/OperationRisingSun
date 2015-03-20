/*-- Funktionen Zum Zeichnen von Landschaft --*/

#strict 2

global func DrawRamp(string szMatTex ,int iX, int iY, int iLength, int iHeight, bool sub)
{
	return	DrawMaterialQuad(szMatTex, iX, iY, iX + iLength, iY, iX + iLength, iY - iHeight, iX + iLength, iY - iHeight, sub);
}
