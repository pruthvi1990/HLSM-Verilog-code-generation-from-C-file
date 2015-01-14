INPUTS  : a b c five
OUTPUTS : z x
REGS    : d e f g xrin zrin cond

d = a + b
cond = d > five

while ( cond ) {

e = a + c
g = d > e
zrin = g ? d : e
f = a * c
xrin = f - d 
cond = d > five

}

x = xrin
z = zrin
