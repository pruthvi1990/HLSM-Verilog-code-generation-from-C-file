INPUTS  : a b c
OUTPUTS : z x
REGS    : d e f g h dLTe dEQe xrin zrin

d = a + b
e = a + c
f = a - b  
dEQe = d == e
dLTe = d < e
g = dLTe ? d : e 
h = dEQe ? g : f 
xrin = h << dLTe
zrin = h >> dEQe
x = xrin
z = zrin