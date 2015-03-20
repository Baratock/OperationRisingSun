//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Unconsciousness  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2

public func IsChooseable() { return 1; }

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}
