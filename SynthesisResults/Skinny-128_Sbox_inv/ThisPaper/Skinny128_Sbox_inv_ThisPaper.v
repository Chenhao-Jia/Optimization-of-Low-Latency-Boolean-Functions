module anf_from_sp(
  input  wire x0,
  input  wire x1,
  input  wire x2,
  input  wire x3,
  input  wire x4,
  input  wire x5,
  input  wire x6,
  input  wire x7,
  output wire f3,
  output wire f1,
  output wire f7,
  output wire f6,
  output wire f0,
  output wire f2,
  output wire f4,
  output wire f5
);

  wire w_f3_nb0 = ~x2;
  wire w_f3_nb1 = ~x6;
  wire w_f3_and2 = x3 & x4;
  wire w_f3_and3 = x0 & w_f3_nb0;
  wire w_f3_and4 = w_f3_and2 & w_f3_and3;
  wire w_f3_and5 = w_f3_and4 & w_f3_nb1;
  wire sp_f3_1055 = w_f3_and5;
  wire w_f3_nb6 = ~x0;
  wire w_f3_nb7 = ~x5;
  wire w_f3_and8 = x6 & w_f3_nb6;
  wire w_f3_and9 = x1 & w_f3_nb0;
  wire w_f3_and10 = w_f3_and8 & w_f3_and9;
  wire w_f3_and11 = w_f3_and10 & w_f3_nb7;
  wire sp_f3_1359 = w_f3_and11;
  wire w_f3_and12 = x6 & w_f3_nb6;
  wire w_f3_and13 = x3 & w_f3_nb0;
  wire w_f3_and14 = w_f3_and12 & w_f3_and13;
  wire w_f3_and15 = w_f3_and14 & w_f3_nb7;
  wire sp_f3_1670 = w_f3_and15;
  wire w_f3_and16 = x5 & x6;
  wire w_f3_and17 = x4 & w_f3_nb6;
  wire w_f3_and18 = w_f3_and16 & w_f3_and17;
  wire sp_f3_1867 = w_f3_and18;
  wire w_f3_nand19 = ~(x1 & x4);
  wire w_f3_and20 = x0 & x5;
  wire w_f3_and21 = w_f3_and20 & w_f3_nand19;
  wire w_f3_and22 = w_f3_and21 & w_f3_nb1;
  wire sp_f3_2039 = w_f3_and22;
  wire w_f3_and23 = x4 & x5;
  wire w_f3_and24 = x1 & w_f3_nb0;
  wire w_f3_and25 = w_f3_and23 & w_f3_and24;
  wire w_f3_and26 = w_f3_and25 & w_f3_nb1;
  wire sp_f3_2225 = w_f3_and26;
  wire w_f3_nand27 = ~(x1 & x5);
  wire w_f3_and28 = x2 & x4;
  wire w_f3_and29 = w_f3_and28 & w_f3_nand27;
  wire w_f3_and30 = x0 & w_f3_nb1;
  wire w_f3_and31 = w_f3_and29 & w_f3_and30;
  wire sp_f3_2735 = w_f3_and31;
  wire w_f3_and32 = x5 & w_f3_nb6;
  wire w_f3_and33 = x4 & w_f3_nb0;
  wire w_f3_and34 = w_f3_and32 & w_f3_and33;
  wire w_f3_and35 = x3 & w_f3_nb1;
  wire w_f3_and36 = w_f3_and34 & w_f3_and35;
  wire sp_f3_3911 = w_f3_and36;
  wire w_f3_nb37 = ~x7;
  wire w_f3_and38 = x0 & x4;
  wire w_f3_and39 = w_f3_and38 & w_f3_nb37;
  wire sp_f3_4663 = w_f3_and39;
  wire w_f3_and40 = x5 & x7;
  wire w_f3_and41 = x1 & w_f3_nb6;
  wire w_f3_and42 = w_f3_and40 & w_f3_and41;
  wire w_f3_and43 = w_f3_and42 & w_f3_nb0;
  wire sp_f3_4812 = w_f3_and43;
  wire w_f3_and44 = x5 & x7;
  wire w_f3_and45 = x3 & w_f3_nb0;
  wire w_f3_and46 = w_f3_and44 & w_f3_and45;
  wire sp_f3_5074 = w_f3_and46;
  wire w_f3_and47 = x3 & x7;
  wire w_f3_and48 = x0 & w_f3_nb0;
  wire w_f3_and49 = w_f3_and47 & w_f3_and48;
  wire w_f3_and50 = w_f3_and49 & w_f3_nb7;
  wire sp_f3_5142 = w_f3_and50;
  wire w_f3_nb51 = ~x4;
  wire w_f3_and52 = x7 & w_f3_nb6;
  wire w_f3_and53 = x5 & w_f3_nb51;
  wire w_f3_and54 = w_f3_and52 & w_f3_and53;
  wire sp_f3_5343 = w_f3_and54;
  wire w_f3_nb55 = ~x1;
  wire w_f3_and56 = x7 & w_f3_nb6;
  wire w_f3_and57 = x6 & w_f3_nb55;
  wire w_f3_and58 = w_f3_and56 & w_f3_and57;
  wire sp_f3_5426 = w_f3_and58;
  wire w_f3_nand59 = ~(x2 & x7);
  wire w_f3_and60 = x0 & w_f3_nb1;
  wire w_f3_and61 = w_f3_nand59 & w_f3_and60;
  wire sp_f3_5510 = w_f3_and61;
  wire w_f3_and62 = x2 & x6;
  wire w_f3_nand63 = ~(w_f3_and62 & x7);
  wire w_f3_and64 = x1 & w_f3_nand63;
  wire sp_f3_5560 = w_f3_and64;
  wire w_f3_and65 = x6 & x7;
  wire w_f3_and66 = x3 & w_f3_nb0;
  wire w_f3_and67 = w_f3_and65 & w_f3_and66;
  wire sp_f3_5793 = w_f3_and67;
  wire w_f3_and68 = x4 & x6;
  wire w_f3_and69 = w_f3_and68 & w_f3_nb37;
  wire sp_f3_5848 = w_f3_and69;
  wire w_f3_m70 = x0 & x1;
  wire w_f3_m71 = x6 & x7;
  wire w_f3_m72 = x2 & w_f3_m71;
  wire w_f3_m73 = w_f3_m70 & w_f3_m72;
  wire rm_f3_199 = w_f3_m73;

  wire w_f3_x74 = sp_f3_1055 ^ sp_f3_1359;
  wire w_f3_x75 = sp_f3_1670 ^ sp_f3_1867;
  wire w_f3_x76 = sp_f3_2039 ^ sp_f3_2225;
  wire w_f3_x77 = sp_f3_2735 ^ sp_f3_3911;
  wire w_f3_x78 = sp_f3_4663 ^ sp_f3_4812;
  wire w_f3_x79 = sp_f3_5074 ^ sp_f3_5142;
  wire w_f3_x80 = sp_f3_5343 ^ sp_f3_5426;
  wire w_f3_x81 = sp_f3_5510 ^ sp_f3_5560;
  wire w_f3_x82 = sp_f3_5793 ^ sp_f3_5848;
  wire w_f3_x83 = w_f3_x74 ^ w_f3_x75;
  wire w_f3_x84 = w_f3_x76 ^ w_f3_x77;
  wire w_f3_x85 = w_f3_x78 ^ w_f3_x79;
  wire w_f3_x86 = w_f3_x80 ^ w_f3_x81;
  wire w_f3_x87 = w_f3_x82 ^ rm_f3_199;
  wire w_f3_x88 = w_f3_x83 ^ w_f3_x84;
  wire w_f3_x89 = w_f3_x85 ^ w_f3_x86;
  wire w_f3_x90 = w_f3_x88 ^ w_f3_x89;
  wire w_f3_x91 = w_f3_x90 ^ w_f3_x87;
  assign f3 = w_f3_x91;

  wire w_f1_nb0 = ~x0;
  wire w_f1_nb1 = ~x1;
  wire w_f1_nb2 = ~x2;
  wire w_f1_nb3 = ~x4;
  wire w_f1_nb4 = ~x6;
  wire w_f1_and5 = w_f1_nb1 & w_f1_nb2;
  wire w_f1_and6 = w_f1_nb0 & w_f1_nb3;
  wire w_f1_and7 = w_f1_and5 & w_f1_and6;
  wire w_f1_and8 = w_f1_and7 & w_f1_nb4;
  wire sp_f1_575 = w_f1_and8;
  wire w_f1_and9 = x3 & w_f1_nb3;
  wire w_f1_and10 = w_f1_and9 & w_f1_nb4;
  wire sp_f1_634 = w_f1_and10;
  wire w_f1_nb11 = ~x3;
  wire w_f1_and12 = w_f1_nb11 & w_f1_nb3;
  wire w_f1_and13 = x2 & w_f1_nb4;
  wire w_f1_and14 = w_f1_and12 & w_f1_and13;
  wire sp_f1_672 = w_f1_and14;
  wire w_f1_nb15 = ~x7;
  wire w_f1_and16 = w_f1_nb1 & w_f1_nb2;
  wire w_f1_and17 = w_f1_nb0 & w_f1_nb15;
  wire w_f1_and18 = w_f1_and16 & w_f1_and17;
  wire sp_f1_779 = w_f1_and18;
  wire w_f1_and19 = x3 & w_f1_nb15;
  wire sp_f1_795 = w_f1_and19;
  wire w_f1_and20 = x2 & w_f1_nb11;
  wire w_f1_and21 = w_f1_and20 & w_f1_nb15;
  wire sp_f1_805 = w_f1_and21;
  wire w_f1_and22 = x4 & w_f1_nb15;
  wire sp_f1_813 = w_f1_and22;
  wire rm_f1_32 = x5;

  wire w_f1_x23 = sp_f1_575 ^ sp_f1_634;
  wire w_f1_x24 = sp_f1_672 ^ sp_f1_779;
  wire w_f1_x25 = sp_f1_795 ^ sp_f1_805;
  wire w_f1_x26 = sp_f1_813 ^ rm_f1_32;
  wire w_f1_x27 = w_f1_x23 ^ w_f1_x24;
  wire w_f1_x28 = w_f1_x25 ^ w_f1_x26;
  wire w_f1_x29 = w_f1_x27 ^ w_f1_x28;
  assign f1 = w_f1_x29;

  wire w_f7_nb0 = ~x0;
  wire w_f7_nb1 = ~x1;
  wire w_f7_nb2 = ~x4;
  wire w_f7_nb3 = ~x6;
  wire w_f7_and4 = w_f7_nb1 & w_f7_nb2;
  wire w_f7_and5 = w_f7_nb0 & w_f7_nb3;
  wire w_f7_and6 = w_f7_and4 & w_f7_and5;
  wire sp_f7_117 = w_f7_and6;
  wire w_f7_nb7 = ~x3;
  wire w_f7_and8 = w_f7_nb7 & w_f7_nb2;
  wire w_f7_and9 = w_f7_and8 & w_f7_nb3;
  wire sp_f7_141 = w_f7_and9;
  wire w_f7_nb10 = ~x7;
  wire w_f7_and11 = w_f7_nb0 & w_f7_nb1;
  wire w_f7_and12 = w_f7_and11 & w_f7_nb10;
  wire sp_f7_163 = w_f7_and12;
  wire w_f7_and13 = w_f7_nb7 & w_f7_nb10;
  wire sp_f7_170 = w_f7_and13;
  wire rm_f7_4 = x2;

  wire w_f7_x14 = sp_f7_117 ^ sp_f7_141;
  wire w_f7_x15 = sp_f7_163 ^ sp_f7_170;
  wire w_f7_x16 = w_f7_x14 ^ w_f7_x15;
  wire w_f7_x17 = w_f7_x16 ^ rm_f7_4;
  assign f7 = w_f7_x17;

  wire w_f6_nb0 = ~x0;
  wire w_f6_nb1 = ~x1;
  wire w_f6_nb2 = ~x2;
  wire w_f6_and3 = w_f6_nb0 & w_f6_nb1;
  wire w_f6_and4 = w_f6_and3 & w_f6_nb2;
  wire sp_f6_23 = w_f6_and4;
  wire w_f6_nb5 = ~x3;
  wire w_f6_and6 = w_f6_nb2 & w_f6_nb5;
  wire sp_f6_31 = w_f6_and6;
  wire rm_f6_16 = x4;

  wire w_f6_x7 = sp_f6_23 ^ sp_f6_31;
  wire w_f6_x8 = w_f6_x7 ^ rm_f6_16;
  assign f6 = w_f6_x8;

  wire w_f0_nb0 = ~x0;
  wire w_f0_nb1 = ~x5;
  wire w_f0_and2 = w_f0_nb0 & w_f0_nb1;
  wire sp_f0_4 = w_f0_and2;
  wire rm_f0_64 = x6;

  wire w_f0_x3 = sp_f0_4 ^ rm_f0_64;
  assign f0 = w_f0_x3;

  wire w_f2_nb0 = ~x1;
  wire w_f2_nb1 = ~x2;
  wire w_f2_and2 = w_f2_nb0 & w_f2_nb1;
  wire sp_f2_5 = w_f2_and2;
  wire rm_f2_1 = x0;

  wire w_f2_x3 = sp_f2_5 ^ rm_f2_1;
  assign f2 = w_f2_x3;

  wire w_f4_nb0 = ~x0;
  wire w_f4_nb1 = ~x1;
  wire w_f4_and2 = w_f4_nb0 & w_f4_nb1;
  wire sp_f4_4 = w_f4_and2;
  wire rm_f4_8 = x3;

  wire w_f4_x3 = sp_f4_4 ^ rm_f4_8;
  assign f4 = w_f4_x3;

  wire w_f5_nb0 = ~x4;
  wire w_f5_nb1 = ~x6;
  wire w_f5_and2 = w_f5_nb0 & w_f5_nb1;
  wire sp_f5_4 = w_f5_and2;
  wire rm_f5_128 = x7;

  wire w_f5_x3 = sp_f5_4 ^ rm_f5_128;
  assign f5 = w_f5_x3;

endmodule

