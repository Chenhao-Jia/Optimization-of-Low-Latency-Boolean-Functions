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


module anf_from_sp(
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

  wire w_f0_nb0 = ~x2;
  wire w_f0_nb1 = ~x4;
  wire w_f0_and2 = x1 & w_f0_nb0;
  wire w_f0_and3 = w_f0_and2 & w_f0_nb1;
  wire sp_f0_18 = w_f0_and3;
  wire w_f0_and4 = x3 & w_f0_nb1;
  wire sp_f0_26 = w_f0_and4;
  wire rm_f0_1 = x0;

  wire w_f0_x5 = sp_f0_18 ^ sp_f0_26;
  wire w_f0_x6 = w_f0_x5 ^ rm_f0_1;
  assign f0 = w_f0_x6;

  wire w_f1_nb0 = ~x0;
  wire w_f1_nb1 = ~x3;
  wire w_f1_and2 = x2 & w_f1_nb0;
  wire w_f1_and3 = w_f1_and2 & w_f1_nb1;
  wire sp_f1_15 = w_f1_and3;
  wire w_f1_and4 = x4 & w_f1_nb0;
  wire sp_f1_25 = w_f1_and4;
  wire rm_f1_2 = x1;

  wire w_f1_x5 = sp_f1_15 ^ sp_f1_25;
  wire w_f1_x6 = w_f1_x5 ^ rm_f1_2;
  assign f1 = w_f1_x6;

  wire w_f2_nb0 = ~x1;
  wire w_f2_and1 = x0 & w_f2_nb0;
  wire sp_f2_2 = w_f2_and1;
  wire w_f2_nb2 = ~x4;
  wire w_f2_and3 = x3 & w_f2_nb0;
  wire w_f2_and4 = w_f2_and3 & w_f2_nb2;
  wire sp_f2_20 = w_f2_and4;
  wire rm_f2_4 = x2;

  wire w_f2_x5 = sp_f2_2 ^ sp_f2_20;
  wire w_f2_x6 = w_f2_x5 ^ rm_f2_4;
  assign f2 = w_f2_x6;

  wire w_f3_nb0 = ~x2;
  wire w_f3_and1 = x1 & w_f3_nb0;
  wire sp_f3_2 = w_f3_and1;
  wire w_f3_nb2 = ~x0;
  wire w_f3_and3 = x4 & w_f3_nb2;
  wire w_f3_and4 = w_f3_and3 & w_f3_nb0;
  wire sp_f3_17 = w_f3_and4;
  wire rm_f3_8 = x3;

  wire w_f3_x5 = sp_f3_2 ^ sp_f3_17;
  wire w_f3_x6 = w_f3_x5 ^ rm_f3_8;
  assign f3 = w_f3_x6;

  wire w_f4_nb0 = ~x1;
  wire w_f4_nb1 = ~x3;
  wire w_f4_and2 = x0 & w_f4_nb0;
  wire w_f4_and3 = w_f4_and2 & w_f4_nb1;
  wire sp_f4_17 = w_f4_and3;
  wire w_f4_and4 = x2 & w_f4_nb1;
  wire sp_f4_25 = w_f4_and4;
  wire rm_f4_16 = x4;

  wire w_f4_x5 = sp_f4_17 ^ sp_f4_25;
  wire w_f4_x6 = w_f4_x5 ^ rm_f4_16;
  assign f4 = w_f4_x6;

endmodule

