//latency = 3 ;
// Checking for dummy state generation and conditional stmts.
INPUTS  : a b c
OUTPUTS : z x
REGS    : d e f g xrin zrin

d = a + b
e = a * c
g = d * e
if ( g ) {
f = a * c
zrin = g
}
xrin = f - d
x = xrin
z = zrin
d = xrin * b
