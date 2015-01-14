INPUTS  : a b c seven nine
OUTPUTS : z x
REGS    : d e f g h dLTe dEQe xrin zrin greg hreg

g = seven
h = nine
d = a + b
e = a + c
f = a - b  
dEQe = d == e
dLTe = d < e
greg = g
hreg = h

if ( dLTe ) {
 
}


if ( dEQe ) {
hreg = h + a
g = e
h = f
}


xrin = hreg << dLTe
zrin = greg >> dEQe
x = xrin 
z = zrin 
