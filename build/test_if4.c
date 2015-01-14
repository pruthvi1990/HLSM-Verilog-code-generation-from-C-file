INPUTS  : a b c zero
OUTPUTS : z x
REGS    : e g xrin zrin regd freg d f

regd = a + b
d = regd
xrin = zero
zrin = zero

if ( d ) {
e = a + c
g = d > e
zrin = g ? d : e
freg = a * c
f = freg
xrin = f - d  
}

x = xrin
z = zrin
