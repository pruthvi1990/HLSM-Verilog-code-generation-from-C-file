INPUTS  : a b c one
OUTPUTS : z x
REGS    : d e f g h dLTe dEQe dLTEe

d = a + b
e = a + c
f = a - b  
dEQe = d == e
dLTe = d > e
dLTEe = dEQe + dLTe

if ( dLTEe ) {

	if ( dLTe ) {
		g = e
		h = f
	}
	g = d + e
	h = f + e
}
x = h 
z = h 
