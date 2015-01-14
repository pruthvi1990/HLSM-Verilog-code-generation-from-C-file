INPUTS  : a b c d e f g h sa
OUTPUTS : avg
REGS    : l00 l01 l02 l03 l10 l11 l2 l2div2 l2div4 l2div8

l00 = a + b
l01 = c + d
l02 = e + f
l03 = g + h
l10 = l00 + l01
l11 = l02 + l03
l2 = l10 + l11
l2div2 = l2 >> sa
l2div4 = l2div2 >> sa 
l2div8 = l2div4 >> sa
avg = l2div8
