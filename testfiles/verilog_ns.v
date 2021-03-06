`timescale 1ns / 1ps

// ------------------------------------------------------------------------------------------------- 
 // Copyright (c) 2013 Pruthvi & Mukunda MJ , University of Arizona
 // Permission to copy is granted provided that this header remains
 // intact. This software is provided with no warranties
 // -------------------------------------------------------------------------------------------------
module HLSM (Clk,Rst,Start,Done,a,b,c,z,x);

input Clk, Rst, Start;
output reg Done;
input[31:0] a,b,c;
output reg [31:0]  z,x;
reg[31:0] d,e,f,g,xrin,zrin;

reg [2:0] state ;
parameter start_wait = 0;
parameter state_1 = 1 ;
parameter state_2 = 2 ;
parameter state_3 = 3 ;
parameter state_4 = 4 ;
parameter Final = 5 ;


always@ (posedge Clk) begin
if ( Rst ==1 ) begin
z<=0
x<=0;
d<=0
e<=0;
f<=0;
g<=0;
xrin<=0;
zrin<=0;
Done <=0;
state <= start_wait ;
end

else begin
case( state )
start_wait : begin
if(start == 1) begin 
state <= state_1 ;
Done <= 0;
end
else begin
state <= start_wait ;
end
end
state_1 : begin
d <= a + b;
e <= a + c;
f <= a * c;
state = state_2;
end
state_2 : begin
g <= d > e;
state = state_3;
end
state_3 : begin
zrin <= g ? d : e;
xrin <= f - d;
state = state_4;
end
state_4 : begin
x = xrin;
z = zrin;
state = Final;
end
Final: begin
Done <= 1
state = start_wait
end
endcase
end
end
endmodule