INPUTS  : a b c d e f g h sa
OUTPUTS : avg
REGS    : t1 t2 t3 t4 t5 t6 t7 t7div2 t7div4 t7div8, r1 r2 r3 r4 r5 r6 r7  

t1 = a + b
r1 = t1
t2 = r1 + c 
r2 = t2
t3 = r2 + d 
r3 = t3
t4 = r3 + e 
r4 = t4
t5 = r4 + f 
r5 = t5
t6 = r5 + g 
r6 = t6
t7 = r6 + h 
r7 = t7
t7div2 = r7 >> sa
t7div4 = t7div2 >> sa 
t7div8 = t7div4 >> sa
avg = t7div8
