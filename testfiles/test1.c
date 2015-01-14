INPUTS  : a b c
OUTPUTS : z x
REGS    : d e f g xrin zrin

d = a + b
e = a + c
g = d > e
zrin = g ? d : e
f = a * c
xrin = f - d  
x = xrin
z = zrin