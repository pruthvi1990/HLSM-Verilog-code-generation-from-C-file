`timescale 1ns / 1ps

// ------------------------------------------------------------------------------------------------- 
 // Copyright (c) 2013 Pruthvi & Mukunda MJ , University of Arizona
 // Permission to copy is granted provided that this header remains
 // intact. This software is provided with no warranties
 // -------------------------------------------------------------------------------------------------
module HLSM (Clk, Rst, Start, Done,a,b,c,seven,nine,z,x);

input Clk, Rst, Start;
output reg Done ;
input[31:0] a,b,c,seven,nine;
output reg [31:0] z,x;
reg[31:0] d,e,f,g,h,dLTe,dEQe,xrin,zrin,greg,hreg;

reg [4:0] state ;
parameter start_wait = 0;
parameter state_1 = 1 ;
parameter state_2 = 2 ;
parameter state_3 = 3 ;
parameter state_4 = 4 ;
parameter state_5 = 5 ;
parameter state_6 = 6 ;
parameter state_7 = 7 ;
parameter state_8 = 8 ;
parameter state_9 = 9 ;
parameter state_10 = 10 ;
parameter state_11 = 11 ;
parameter state_12 = 12 ;
parameter state_13 = 13 ;
parameter state_14 = 14 ;
parameter state_15 = 15 ;
parameter state_16 = 16 ;
parameter state_17 = 17 ;
parameter state_18 = 18 ;
parameter state_19 = 19 ;
parameter state_20 = 20 ;
parameter Final = 21 ;


always@ (posedge Clk) begin
if ( Rst ==1 ) begin
z<=0 ;
x<=0;
d<=0 ;
e<=0;
f<=0;
g<=0;
h<=0;
dLTe<=0;
dEQe<=0;
xrin<=0;
zrin<=0;
greg<=0;
hreg<=0;
Done <=0;
state <= start_wait ;
end

else begin
case( state )
start_wait : begin
if(Start == 1) begin 
state <= state_1 ;
Done <= 0 ;
end
else begin
state <= start_wait ;
end
end
state_1 : begin
g = seven;
state <= state_2;
end
state_2 : begin
h = nine;
state <= state_3;
end
state_3 : begin
d = a + b;
state <= state_4;
end
state_4 : begin
e = a + c;
state <= state_5;
end
state_5 : begin
f = a - b;
state <= state_6;
end
state_6 : begin
dEQe = d == e;
state <= state_7;
end
state_7 : begin
dLTe = d < e;
state <= state_8;
end
state_8 : begin
greg = g;
state <= state_9;
end
state_9 : begin
hreg = h;
state <= state_10;
end
state_10 : begin
if (dLTe) begin 
state <= state_11 ;
end
else begin
state <= state_11 ;
end
end
state_11 : begin
state <= state_12 ;
end
state_12 : begin
if (dEQe) begin 
state <= state_13 ;
end
else begin
state <= state_16 ;
end
end
state_16 : begin
state <= state_17 ;
end
state_13 : begin
hreg = h + a;
state <= state_14;
end
state_14 : begin
g = e;
state <= state_15;
end
state_15 : begin
h = f;
state <= state_16;
end
state_17 : begin
xrin = hreg << dLTe;
state <= state_18;
end
state_18 : begin
zrin = greg >> dEQe;
state <= state_19;
end
state_19 : begin
x = xrin;
state <= state_20;
end
state_20 : begin
z = zrin;
state <= Final ;
end
Final : begin
Done <= 1 ;
state <= start_wait ;
end
endcase
end 
end
endmodule
