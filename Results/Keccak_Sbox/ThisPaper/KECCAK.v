// Basic logic gate modules in Verilog


/*
module NANDGate (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = ~(a & b);
endmodule

module NORGate (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = ~(a | b);
endmodule

module ANDGate (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = a & b;
endmodule

module ORGate (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = a | b;
endmodule

module NOTGate (
    input  wire a,
    output wire y
);
    assign y = ~a;
endmodule

module XORGate (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = a ^ b;
endmodule

module XNORGate (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = ~(a ^ b);
endmodule


module sbox_KECCAK (
    input  [4:0] in,  // 5-bit input
    output [4:0] out  // 5-bit output
);

	
    
endmodule
*/


module sbox_KECCAK(
  input  wire x0,
  input  wire x1,
  input  wire x2,
  input  wire x3,
  input  wire x4,
  output wire f0,
  output wire f1,
  output wire f2,
  output wire f3,
  output wire f4
);

  wire w_f0_nb0 = ~x4;
  wire w_f0_and1 = x3 & w_f0_nb0;
  wire sp_f0_3 = w_f0_and1;
  wire rm_f0_1 = x0;

  wire w_f0_x2 = sp_f0_3 ^ rm_f0_1;
  assign f0 = w_f0_x2;

  wire w_f1_nb0 = ~x0;
  wire w_f1_and1 = x4 & w_f1_nb0;
  wire sp_f1_2 = w_f1_and1;
  wire rm_f1_2 = x1;

  wire w_f1_x2 = sp_f1_2 ^ rm_f1_2;
  assign f1 = w_f1_x2;

  wire w_f2_nb0 = ~x1;
  wire w_f2_and1 = x0 & w_f2_nb0;
  wire sp_f2_2 = w_f2_and1;
  wire rm_f2_4 = x2;

  wire w_f2_x2 = sp_f2_2 ^ rm_f2_4;
  assign f2 = w_f2_x2;

  wire w_f3_nb0 = ~x2;
  wire w_f3_and1 = x1 & w_f3_nb0;
  wire sp_f3_2 = w_f3_and1;
  wire rm_f3_8 = x3;

  wire w_f3_x2 = sp_f3_2 ^ rm_f3_8;
  assign f3 = w_f3_x2;

  wire w_f4_nb0 = ~x3;
  wire w_f4_and1 = x2 & w_f4_nb0;
  wire sp_f4_2 = w_f4_and1;
  wire rm_f4_16 = x4;

  wire w_f4_x2 = sp_f4_2 ^ rm_f4_16;
  assign f4 = w_f4_x2;

endmodule

