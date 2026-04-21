module anf_from_sp(
  input  wire x0,
  input  wire x1,
  input  wire x2,
  input  wire x3,
  input  wire x4,
  input  wire x5,
  input  wire x6,
  input  wire x7,
  output wire f7,
  output wire f6,
  output wire f0,
  output wire f3,
  output wire f4,
  output wire f1,
  output wire f2,
  output wire f5
);

  wire w_f7_nand0 = ~(x0 & x5);
  wire w_f7_nb1 = ~x4;
  wire w_f7_and2 = x1 & w_f7_nb1;
  wire w_f7_and3 = w_f7_nand0 & w_f7_and2;
  wire sp_f7_76 = w_f7_and3;
  wire w_f7_nb4 = ~x1;
  wire w_f7_and5 = x5 & w_f7_nb4;
  wire w_f7_and6 = x2 & w_f7_nb1;
  wire w_f7_and7 = w_f7_and5 & w_f7_and6;
  wire sp_f7_116 = w_f7_and7;
  wire w_f7_nand8 = ~(x3 & x4);
  wire w_f7_and9 = x5 & w_f7_nand8;
  wire sp_f7_165 = w_f7_and9;
  wire w_f7_nb10 = ~x6;
  wire w_f7_and11 = x2 & w_f7_nb4;
  wire w_f7_and12 = w_f7_and11 & w_f7_nb10;
  wire sp_f7_190 = w_f7_and12;
  wire w_f7_nb13 = ~x3;
  wire w_f7_nb14 = ~x5;
  wire w_f7_and15 = x6 & w_f7_nb13;
  wire w_f7_and16 = x1 & w_f7_nb1;
  wire w_f7_and17 = w_f7_and15 & w_f7_and16;
  wire w_f7_and18 = w_f7_and17 & w_f7_nb14;
  wire sp_f7_494 = w_f7_and18;
  wire w_f7_and19 = x7 & w_f7_nb4;
  wire w_f7_and20 = x2 & w_f7_nb1;
  wire w_f7_and21 = w_f7_and19 & w_f7_and20;
  wire sp_f7_711 = w_f7_and21;
  wire w_f7_nb22 = ~x7;
  wire w_f7_and23 = x3 & x4;
  wire w_f7_and24 = x1 & w_f7_nb22;
  wire w_f7_and25 = w_f7_and23 & w_f7_and24;
  wire sp_f7_771 = w_f7_and25;
  wire w_f7_and26 = x7 & w_f7_nb4;
  wire w_f7_and27 = x0 & w_f7_nb10;
  wire w_f7_and28 = w_f7_and26 & w_f7_and27;
  wire sp_f7_1271 = w_f7_and28;
  wire w_f7_and29 = x6 & w_f7_nb4;
  wire w_f7_and30 = x3 & w_f7_nb22;
  wire w_f7_and31 = w_f7_and29 & w_f7_and30;
  wire sp_f7_1358 = w_f7_and31;
  wire w_f7_nb32 = ~x0;
  wire w_f7_nand33 = ~(x4 & x6);
  wire w_f7_and34 = x1 & x7;
  wire w_f7_and35 = w_f7_and34 & w_f7_nb32;
  wire w_f7_and36 = w_f7_and35 & w_f7_nand33;
  wire sp_f7_1537 = w_f7_and36;
  wire w_f7_nb37 = ~x2;
  wire w_f7_and38 = w_f7_nb14 & w_f7_nb10;
  wire w_f7_and39 = w_f7_nb37 & w_f7_nb22;
  wire w_f7_and40 = w_f7_and38 & w_f7_and39;
  wire sp_f7_1864 = w_f7_and40;
  wire w_f7_nand41 = ~(x4 & x7);
  wire w_f7_and42 = x0 & w_f7_nb14;
  wire w_f7_and43 = w_f7_nand41 & w_f7_and42;
  wire w_f7_and44 = w_f7_and43 & w_f7_nb10;
  wire sp_f7_2078 = w_f7_and44;
  wire w_f7_and45 = x5 & w_f7_nb32;
  wire w_f7_and46 = w_f7_and45 & w_f7_nand41;
  wire w_f7_and47 = x1 & w_f7_nb10;
  wire w_f7_and48 = w_f7_and46 & w_f7_and47;
  wire sp_f7_2723 = w_f7_and48;
  wire w_f7_and49 = x7 & w_f7_nb37;
  wire w_f7_and50 = x4 & w_f7_nb14;
  wire w_f7_and51 = w_f7_and49 & w_f7_and50;
  wire w_f7_and52 = w_f7_and51 & w_f7_nb10;
  wire sp_f7_3236 = w_f7_and52;
  wire w_f7_and53 = x3 & x6;
  wire w_f7_and54 = w_f7_and53 & w_f7_nand41;
  wire w_f7_and55 = w_f7_and54 & w_f7_nb14;
  wire sp_f7_3484 = w_f7_and55;
  wire w_f7_m56 = x3 & x4;
  wire w_f7_m57 = x5 & x7;
  wire w_f7_m58 = w_f7_m56 & w_f7_m57;
  wire rm_f7_184 = w_f7_m58;

  wire w_f7_x59 = sp_f7_76 ^ sp_f7_116;
  wire w_f7_x60 = sp_f7_165 ^ sp_f7_190;
  wire w_f7_x61 = sp_f7_494 ^ sp_f7_711;
  wire w_f7_x62 = sp_f7_771 ^ sp_f7_1271;
  wire w_f7_x63 = sp_f7_1358 ^ sp_f7_1537;
  wire w_f7_x64 = sp_f7_1864 ^ sp_f7_2078;
  wire w_f7_x65 = sp_f7_2723 ^ sp_f7_3236;
  wire w_f7_x66 = sp_f7_3484 ^ rm_f7_184;
  wire w_f7_x67 = w_f7_x59 ^ w_f7_x60;
  wire w_f7_x68 = w_f7_x61 ^ w_f7_x62;
  wire w_f7_x69 = w_f7_x63 ^ w_f7_x64;
  wire w_f7_x70 = w_f7_x65 ^ w_f7_x66;
  wire w_f7_x71 = w_f7_x67 ^ w_f7_x68;
  wire w_f7_x72 = w_f7_x69 ^ w_f7_x70;
  wire w_f7_x73 = w_f7_x71 ^ w_f7_x72;
  assign f7 = w_f7_x73;

  wire w_f6_nb0 = ~x2;
  wire w_f6_and1 = x1 & w_f6_nb0;
  wire sp_f6_3 = w_f6_and1;
  wire w_f6_and2 = x5 & w_f6_nb0;
  wire sp_f6_43 = w_f6_and2;
  wire w_f6_nb3 = ~x1;
  wire w_f6_and4 = x3 & x5;
  wire w_f6_and5 = w_f6_and4 & w_f6_nb3;
  wire sp_f6_51 = w_f6_and5;
  wire w_f6_nb6 = ~x5;
  wire w_f6_and7 = x3 & x4;
  wire w_f6_and8 = x1 & w_f6_nb6;
  wire w_f6_and9 = w_f6_and7 & w_f6_and8;
  wire sp_f6_93 = w_f6_and9;
  wire w_f6_and10 = x2 & x6;
  wire w_f6_and11 = w_f6_and10 & w_f6_nb6;
  wire sp_f6_233 = w_f6_and11;
  wire w_f6_nb12 = ~x6;
  wire w_f6_and13 = x4 & w_f6_nb6;
  wire w_f6_and14 = x1 & w_f6_nb12;
  wire w_f6_and15 = w_f6_and13 & w_f6_and14;
  wire sp_f6_321 = w_f6_and15;
  wire w_f6_and16 = x4 & x6;
  wire w_f6_and17 = x0 & w_f6_nb3;
  wire w_f6_and18 = w_f6_and16 & w_f6_and17;
  wire w_f6_and19 = w_f6_and18 & w_f6_nb6;
  wire sp_f6_365 = w_f6_and19;
  wire w_f6_nand20 = ~(x3 & x4);
  wire w_f6_and21 = x6 & w_f6_nb6;
  wire w_f6_and22 = w_f6_nand20 & w_f6_and21;
  wire sp_f6_561 = w_f6_and22;
  wire w_f6_nb23 = ~x7;
  wire w_f6_and24 = x5 & w_f6_nb3;
  wire w_f6_and25 = x0 & w_f6_nb23;
  wire w_f6_and26 = w_f6_and24 & w_f6_and25;
  wire sp_f6_664 = w_f6_and26;
  wire w_f6_and27 = x7 & w_f6_nb6;
  wire w_f6_and28 = x1 & w_f6_nb12;
  wire w_f6_and29 = w_f6_and27 & w_f6_and28;
  wire sp_f6_863 = w_f6_and29;
  wire w_f6_and30 = x6 & x7;
  wire w_f6_and31 = x0 & w_f6_nb3;
  wire w_f6_and32 = w_f6_and30 & w_f6_and31;
  wire w_f6_and33 = w_f6_and32 & w_f6_nb6;
  wire sp_f6_914 = w_f6_and33;
  wire w_f6_and34 = x5 & x7;
  wire w_f6_and35 = x3 & w_f6_nb12;
  wire w_f6_and36 = w_f6_and34 & w_f6_and35;
  wire sp_f6_1113 = w_f6_and36;
  wire rm_f6_1 = x0;
  wire w_f6_m37 = x3 & x7;
  wire w_f6_m38 = x1 & w_f6_m37;
  wire rm_f6_138 = w_f6_m38;
  wire w_f6_m39 = x6 & x7;
  wire w_f6_m40 = x3 & w_f6_m39;
  wire rm_f6_200 = w_f6_m40;

  wire w_f6_x41 = sp_f6_3 ^ sp_f6_43;
  wire w_f6_x42 = sp_f6_51 ^ sp_f6_93;
  wire w_f6_x43 = sp_f6_233 ^ sp_f6_321;
  wire w_f6_x44 = sp_f6_365 ^ sp_f6_561;
  wire w_f6_x45 = sp_f6_664 ^ sp_f6_863;
  wire w_f6_x46 = sp_f6_914 ^ sp_f6_1113;
  wire w_f6_x47 = rm_f6_1 ^ rm_f6_138;
  wire w_f6_x48 = w_f6_x41 ^ w_f6_x42;
  wire w_f6_x49 = w_f6_x43 ^ w_f6_x44;
  wire w_f6_x50 = w_f6_x45 ^ w_f6_x46;
  wire w_f6_x51 = w_f6_x47 ^ rm_f6_200;
  wire w_f6_x52 = w_f6_x48 ^ w_f6_x49;
  wire w_f6_x53 = w_f6_x50 ^ w_f6_x51;
  wire w_f6_x54 = w_f6_x52 ^ w_f6_x53;
  assign f6 = w_f6_x54;

  wire w_f0_nb0 = ~x0;
  wire w_f0_and1 = x1 & x5;
  wire w_f0_and2 = w_f0_and1 & w_f0_nb0;
  wire sp_f0_35 = w_f0_and2;
  wire w_f0_nb3 = ~x4;
  wire w_f0_and4 = x0 & x5;
  wire w_f0_and5 = w_f0_and4 & w_f0_nb3;
  wire sp_f0_54 = w_f0_and5;
  wire w_f0_nb6 = ~x5;
  wire w_f0_and7 = x4 & w_f0_nb0;
  wire w_f0_and8 = x1 & w_f0_nb6;
  wire w_f0_and9 = w_f0_and7 & w_f0_and8;
  wire sp_f0_96 = w_f0_and9;
  wire w_f0_and10 = x3 & x5;
  wire w_f0_and11 = w_f0_and10 & w_f0_nb3;
  wire sp_f0_133 = w_f0_and11;
  wire w_f0_and12 = x1 & x7;
  wire w_f0_and13 = w_f0_and12 & w_f0_nb0;
  wire sp_f0_154 = w_f0_and13;
  wire rm_f0_4 = x2;
  wire w_f0_m14 = x0 & x4;
  wire rm_f0_17 = w_f0_m14;
  wire w_f0_m15 = x3 & x4;
  wire rm_f0_24 = w_f0_m15;
  wire w_f0_m16 = x0 & x7;
  wire rm_f0_129 = w_f0_m16;
  wire w_f0_m17 = x3 & x7;
  wire rm_f0_136 = w_f0_m17;

  wire w_f0_x18 = sp_f0_35 ^ sp_f0_54;
  wire w_f0_x19 = sp_f0_96 ^ sp_f0_133;
  wire w_f0_x20 = sp_f0_154 ^ rm_f0_4;
  wire w_f0_x21 = rm_f0_17 ^ rm_f0_24;
  wire w_f0_x22 = rm_f0_129 ^ rm_f0_136;
  wire w_f0_x23 = w_f0_x18 ^ w_f0_x19;
  wire w_f0_x24 = w_f0_x20 ^ w_f0_x21;
  wire w_f0_x25 = w_f0_x23 ^ w_f0_x24;
  wire w_f0_x26 = w_f0_x25 ^ w_f0_x22;
  assign f0 = w_f0_x26;

  wire w_f3_nb0 = ~x1;
  wire w_f3_and1 = x0 & x2;
  wire w_f3_and2 = w_f3_and1 & w_f3_nb0;
  wire sp_f3_15 = w_f3_and2;
  wire w_f3_nb3 = ~x2;
  wire w_f3_and4 = x3 & w_f3_nb3;
  wire sp_f3_29 = w_f3_and4;
  wire w_f3_nb5 = ~x3;
  wire w_f3_and6 = x4 & w_f3_nb5;
  wire sp_f3_56 = w_f3_and6;
  wire w_f3_nb7 = ~x4;
  wire w_f3_nb8 = ~x5;
  wire w_f3_and9 = x1 & w_f3_nb7;
  wire w_f3_and10 = w_f3_and9 & w_f3_nb8;
  wire sp_f3_107 = w_f3_and10;
  wire w_f3_and11 = w_f3_nb0 & w_f3_nb7;
  wire w_f3_and12 = x0 & w_f3_nb8;
  wire w_f3_and13 = w_f3_and11 & w_f3_and12;
  wire sp_f3_145 = w_f3_and13;
  wire w_f3_and14 = x3 & x5;
  wire w_f3_and15 = w_f3_and14 & w_f3_nb7;
  wire sp_f3_166 = w_f3_and15;
  wire w_f3_and16 = x0 & x7;
  wire w_f3_and17 = w_f3_and16 & w_f3_nb0;
  wire sp_f3_188 = w_f3_and17;
  wire w_f3_m18 = x1 & x2;
  wire rm_f3_6 = w_f3_m18;
  wire w_f3_m19 = x1 & x7;
  wire rm_f3_130 = w_f3_m19;
  wire w_f3_m20 = x3 & x7;
  wire rm_f3_136 = w_f3_m20;

  wire w_f3_x21 = sp_f3_15 ^ sp_f3_29;
  wire w_f3_x22 = sp_f3_56 ^ sp_f3_107;
  wire w_f3_x23 = sp_f3_145 ^ sp_f3_166;
  wire w_f3_x24 = sp_f3_188 ^ rm_f3_6;
  wire w_f3_x25 = rm_f3_130 ^ rm_f3_136;
  wire w_f3_x26 = w_f3_x21 ^ w_f3_x22;
  wire w_f3_x27 = w_f3_x23 ^ w_f3_x24;
  wire w_f3_x28 = w_f3_x26 ^ w_f3_x27;
  wire w_f3_x29 = w_f3_x28 ^ w_f3_x25;
  assign f3 = w_f3_x29;

  wire w_f4_nb0 = ~x4;
  wire w_f4_and1 = x5 & w_f4_nb0;
  wire sp_f4_1 = w_f4_and1;
  wire w_f4_and2 = x7 & w_f4_nb0;
  wire sp_f4_7 = w_f4_and2;
  wire rm_f4_64 = x6;

  wire w_f4_x3 = sp_f4_1 ^ sp_f4_7;
  wire w_f4_x4 = w_f4_x3 ^ rm_f4_64;
  assign f4 = w_f4_x4;

  wire w_f1_nb0 = ~x0;
  wire w_f1_nb1 = ~x1;
  wire w_f1_and2 = w_f1_nb0 & w_f1_nb1;
  wire sp_f1_4 = w_f1_and2;
  wire rm_f1_8 = x3;

  wire w_f1_x3 = sp_f1_4 ^ rm_f1_8;
  assign f1 = w_f1_x3;

  wire w_f2_nb0 = ~x4;
  wire w_f2_nb1 = ~x5;
  wire w_f2_and2 = w_f2_nb0 & w_f2_nb1;
  wire sp_f2_4 = w_f2_and2;
  wire rm_f2_128 = x7;

  wire w_f2_x3 = sp_f2_4 ^ rm_f2_128;
  assign f2 = w_f2_x3;

  wire w_f5_nb0 = ~x5;
  wire w_f5_nb1 = ~x6;
  wire w_f5_and2 = w_f5_nb0 & w_f5_nb1;
  wire sp_f5_5 = w_f5_and2;
  wire rm_f5_2 = x1;

  wire w_f5_x3 = sp_f5_5 ^ rm_f5_2;
  assign f5 = w_f5_x3;

endmodule

