// FOR LIST R - WITH LATENCY 4
// NESTED CONTROL STATEMENTS.
// DUMMY STATE CHECK - FOLLOWING ONE CONTROL STRUCTURE BEFORE ANOTHER.
INPUTS  : a b c
OUTPUTS : z x
REGS    : d e f g h dLTe dEQe xrin zrin r i

d = a + b
e = a + c
f = a - b
dEQe = d == e

if ( dEQe ) {
	i = a * b
	dLTe = d < e
	r = i * a
	while ( dLTe ) {
		g = dLTe ? d : e
		h = dEQe ? g : f
	}
	xrin = h << dLTe
	zrin = h >> dEQe
	i = a * b
}
x = xrin
z = zrin
