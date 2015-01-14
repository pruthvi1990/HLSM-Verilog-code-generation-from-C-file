INPUTS  : a b c
OUTPUTS : z x
REGS    : d e f g xrin zrin cnt1 dec1 cnt2 dec2 d1 e1 g1 h1 d2 e2 g2 d3 e3 h3 yrin

d = a + b
e = d * c
g = d > e
yrin = d
while ( cnt1 ) {
        d1 = a + b
        e1 = a + c
        h1 = a < b
        g1 = d1 * e1
        if ( cnt2 ) {
           d2 = a + b
           e2 = a + c
           g2 = d2 * e2
           cnt2 = cnt2 - dec2
        }
        cnt1  = cnt1 - dec1
        d3 = a + b
        e3 = a - b
        h3 = e3 * d3
}
zrin = g ? d : e
f = zrin * c
xrin = f - d
z = zrin
