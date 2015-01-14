INPUTS  : a b c d e f g h sa
OUTPUTS : avg
REGS    : t1 t2 t3 t4 t5 t6 t7 t7div2 t7div4 t7div8

t1 = a + b
t2 = t1 + c 
t3 = t2 + d 
t4 = t3 + e 
t5 = t4 + f 
t6 = t5 + g 
t7 = t6 + h 
t7div2 = t7 >> sa
t7div4 = t7div2 >> sa 
t7div8 = t7div4 >> sa
avg = t7div8
