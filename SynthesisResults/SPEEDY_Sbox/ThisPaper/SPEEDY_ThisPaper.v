module anf_from_sp(
  input  wire x0,
  input  wire x1,
  input  wire x2,
  input  wire x3,
  input  wire x4,
  input  wire x5,
  output wire f0,
  output wire f1,
  output wire f2,
  output wire f3,
  output wire f4,
  output wire f5
);

  wire w_f0_nb0 = ~x3;
  wire w_f0_nand1 = ~(x4 & x5);
  wire w_f0_and2 = x0 & x1;
  wire w_f0_and3 = w_f0_and2 & w_f0_nb0;
  wire w_f0_and4 = w_f0_and3 & w_f0_nand1;
  wire sp_f0_204 = w_f0_and4;
  wire w_f0_nand5 = ~(x2 & x4);
  wire w_f0_and6 = x3 & x5;
  wire w_f0_and7 = w_f0_and6 & w_f0_nand5;
  wire sp_f0_296 = w_f0_and7;
  wire w_f0_nand8 = ~(x2 & x3);
  wire w_f0_and9 = x1 & x4;
  wire w_f0_and10 = w_f0_and9 & x5;
  wire w_f0_and11 = w_f0_and10 & w_f0_nand8;
  wire sp_f0_349 = w_f0_and11;
  wire rm_f0_8 = x3;

  wire w_f0_x12 = sp_f0_204 ^ sp_f0_296;
  wire w_f0_x13 = sp_f0_349 ^ rm_f0_8;
  wire w_f0_x14 = w_f0_x12 ^ w_f0_x13;
  assign f0 = w_f0_x14;

  wire w_f1_nb0 = ~x5;
  wire w_f1_and1 = x0 & x2;
  wire w_f1_and2 = w_f1_and1 & w_f1_nb0;
  wire sp_f1_32 = w_f1_and2;
  wire w_f1_nb3 = ~x2;
  wire w_f1_nand4 = ~(x3 & x5);
  wire w_f1_and5 = w_f1_nb3 & w_f1_nand4;
  wire sp_f1_52 = w_f1_and5;
  wire w_f1_m6 = x1 & x3;
  wire w_f1_m7 = x0 & w_f1_m6;
  wire rm_f1_11 = w_f1_m7;

  wire w_f1_x8 = sp_f1_32 ^ sp_f1_52;
  wire w_f1_x9 = w_f1_x8 ^ rm_f1_11;
  assign f1 = w_f1_x9;

  wire w_f2_nb0 = ~x2;
  wire w_f2_and1 = x0 & w_f2_nb0;
  wire sp_f2_2 = w_f2_and1;
  wire rm_f2_4 = x2;

  wire w_f2_x2 = sp_f2_2 ^ rm_f2_4;
  assign f2 = w_f2_x2;

  wire w_f3_nb0 = ~x0;
  wire w_f3_and1 = x1 & x3;
  wire w_f3_and2 = w_f3_and1 & w_f3_nb0;
  wire sp_f3_9 = w_f3_and2;
  wire w_f3_nb3 = ~x3;
  wire w_f3_and4 = x2 & w_f3_nb0;
  wire w_f3_and5 = w_f3_and4 & w_f3_nb3;
  wire sp_f3_32 = w_f3_and5;
  wire w_f3_nb6 = ~x2;
  wire w_f3_and7 = x0 & x5;
  wire w_f3_and8 = w_f3_and7 & w_f3_nb6;
  wire sp_f3_60 = w_f3_and8;
  wire w_f3_m9 = x2 & x4;
  wire w_f3_m10 = x0 & w_f3_m9;
  wire rm_f3_21 = w_f3_m10;

  wire w_f3_x11 = sp_f3_9 ^ sp_f3_32;
  wire w_f3_x12 = sp_f3_60 ^ rm_f3_21;
  wire w_f3_x13 = w_f3_x11 ^ w_f3_x12;
  assign f3 = w_f3_x13;

  wire w_f4_nb0 = ~x0;
  wire w_f4_nb1 = ~x2;
  wire w_f4_nb2 = ~x4;
  wire w_f4_and3 = w_f4_nb0 & w_f4_nb1;
  wire w_f4_and4 = x1 & w_f4_nb2;
  wire w_f4_and5 = w_f4_and3 & w_f4_and4;
  wire sp_f4_96 = w_f4_and5;
  wire w_f4_nand6 = ~(x0 & x3);
  wire w_f4_and7 = x4 & w_f4_nand6;
  wire sp_f4_124 = w_f4_and7;
  wire w_f4_nand8 = ~(x2 & x3);
  wire w_f4_and9 = x0 & w_f4_nb2;
  wire w_f4_and10 = w_f4_nand8 & w_f4_and9;
  wire sp_f4_168 = w_f4_and10;
  wire w_f4_nb11 = ~x5;
  wire w_f4_and12 = x4 & w_f4_nb0;
  wire w_f4_and13 = w_f4_and12 & w_f4_nb11;
  wire sp_f4_195 = w_f4_and13;

  wire w_f4_x14 = sp_f4_96 ^ sp_f4_124;
  wire w_f4_x15 = sp_f4_168 ^ sp_f4_195;
  wire w_f4_x16 = w_f4_x14 ^ w_f4_x15;
  assign f4 = w_f4_x16;

  wire w_f5_nand0 = ~(x0 & x3);
  wire w_f5_nb1 = ~x1;
  wire w_f5_nb2 = ~x2;
  wire w_f5_and3 = x4 & w_f5_nb1;
  wire w_f5_and4 = w_f5_nand0 & w_f5_and3;
  wire w_f5_and5 = w_f5_and4 & w_f5_nb2;
  wire sp_f5_220 = w_f5_and5;
  wire w_f5_nb6 = ~x5;
  wire w_f5_and7 = x1 & x2;
  wire w_f5_and8 = x0 & w_f5_nb6;
  wire w_f5_and9 = w_f5_and7 & w_f5_and8;
  wire sp_f5_391 = w_f5_and9;
  wire w_f5_nand10 = ~(x2 & x5);
  wire w_f5_and11 = x0 & x3;
  wire w_f5_and12 = w_f5_and11 & w_f5_nb1;
  wire w_f5_and13 = w_f5_and12 & w_f5_nand10;
  wire sp_f5_548 = w_f5_and13;
  wire w_f5_m14 = x2 & x5;
  wire rm_f5_36 = w_f5_m14;

  wire w_f5_x15 = sp_f5_220 ^ sp_f5_391;
  wire w_f5_x16 = sp_f5_548 ^ rm_f5_36;
  wire w_f5_x17 = w_f5_x15 ^ w_f5_x16;
  assign f5 = w_f5_x17;

endmodule

