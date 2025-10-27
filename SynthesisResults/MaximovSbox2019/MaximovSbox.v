module complex_circuit(
    input [7:0] U,      
    output [7:0] R      
);


wire Z18, L28, Q0, Z96, Q1, Z160, Q2, Q11, L6, Q3, Q16, Q4, Q5, Z10;
wire Q6, Q7, Z36, Q8, L19, Q9, Q10, Q12, Q13, L10, Q14, Q15, L8, L12;
wire L16, L15, L31, L5, L13, L17, L29, L14, L26, L30, L7, L11, L27, Q17;


wire L0, L1, L2, L3, L4, L5_w, L6_w, L7_w, L8_w, L9, L10_w, L11_w, L12_w;
wire L13_w, L14_w, L15_w, L16_w, L17_w, L18, L19_w, L20, L21, L22, L23;
wire L24, L25, L26_w, L27_w, L28_w, L29_w, L30_w, L31_w;


wire T20, T21, T22, T10, T11, T12, T13, X0, X1, X2, X3;
wire T0, T1, T2, Y0, Y1, Y2, Y3, T3, T4;


wire K0, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15;
wire K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30, K31;


assign Z18 = U[1] ^ U[4];
assign L28 = Z18 ^ U[6];
assign Q0 = U[2] ^ L28;
assign Z96 = U[5] ^ U[6];
assign Q1 = U[0] ^ Z96;
assign Z160 = U[5] ^ U[7];
assign Q2 = U[6] ^ Z160;
assign Q11 = U[2] ^ U[3];
assign L6 = U[4] ^ Z96;
assign Q3 = Q11 ^ L6;
assign Q16 = U[0] ^ Q11;
assign Q4 = Q16 ^ U[4];
assign Q5 = Z18 ^ Z160;
assign Z10 = U[1] ^ U[3];
assign Q6 = Z10 ^ Q2;
assign Q7 = U[0] ^ U[7];
assign Z36 = U[2] ^ U[5];
assign Q8 = Z36 ^ Q5;
assign L19 = U[2] ^ Z96;
assign Q9 = Z18 ^ L19;
assign Q10 = Z10 ^ Q1;
assign Q12 = U[3] ^ L28;
assign Q13 = U[3] ^ Q2;
assign L10 = Z36 ^ Q7;
assign Q14 = U[6] ^ L10;
assign Q15 = U[0] ^ Q5;
assign L8 = U[3] ^ Q5;
assign L12 = Q16 ^ Q2;
assign L16 = U[2] ^ Q4;
assign L15 = U[1] ^ Z96;
assign L31 = Q16 ^ L15;
assign L5 = Q12 ^ L31;
assign L13 = U[3] ^ Q8;
assign L17 = U[4] ^ L10;
assign L29 = Z96 ^ L10;
assign L14 = Q11 ^ L10;
assign L26 = Q11 ^ Q5;
assign L30 = Q11 ^ U[6];
assign L7 = Q12 ^ Q1;
assign L11 = Q12 ^ L15;
assign L27 = L30 ^ L10;
assign Q17 = U[0];


assign L0 = Q10;
assign L1 = Q6;
assign L2 = Q9;
assign L3 = Q8;
assign L4 = U[6];
assign L5_w = L5;
assign L6_w = L6;
assign L7_w = L7;
assign L8_w = L8;
assign L9 = U[5];
assign L10_w = L10;
assign L11_w = L11;
assign L12_w = L12;
assign L13_w = L13;
assign L14_w = L14;
assign L15_w = L15;
assign L16_w = L16;
assign L17_w = L17;
assign L18 = U[1];
assign L19_w = L19;
assign L20 = Q0;
assign L21 = Q11;
assign L22 = Q15;
assign L23 = U[0];
assign L24 = Q16;
assign L25 = Q13;
assign L26_w = L26;
assign L27_w = L27;
assign L28_w = L28;
assign L29_w = L29;
assign L30_w = L30;
assign L31_w = L31;


assign T20 = ~(Q6 & Q12);
assign T21 = ~(Q3 & Q14);
assign T22 = ~(Q1 & Q16);

assign T10 = (~(Q3 | Q14)) ^ (~(Q0 & Q7));
assign T11 = (~(Q4 | Q13)) ^ (~(Q10 & Q11));
assign T12 = (~(Q2 | Q17)) ^ (~(Q5 & Q9));
assign T13 = (~(Q8 | Q15)) ^ (~(Q2 & Q17));

assign X0 = T10 ^ (T20 ^ T22);
assign X1 = T11 ^ (T21 ^ T20);
assign X2 = T12 ^ (T21 ^ T22);
assign X3 = T13 ^ (T21 ^ (~(Q4 & Q13)));


assign T0 = ~(X0 & X2);
assign T1 = ~(X1 | X3);
assign T2 = ~(T0 ^ T1);  // XNOR


assign Y0 = X3 ? T2 : X2;
assign Y2 = X1 ? T2 : X0;
assign T3 = 1'b1 ? X2 : X1;  // sel=1
assign Y1 = X3 ? T3 : T2;
assign T4 = 1'b1 ? X0 : X3;  // sel=1
assign Y3 = X1 ? T4 : T2;


assign K4 = Y0 & L4;
assign K8 = Y0 & L8_w;
assign K24 = Y0 & L24;
assign K28 = Y0 & L28_w;

assign K0 = ~(Y0 & L0);
assign K12 = ~(Y0 & L12_w);
assign K16 = ~(Y0 & L16_w);
assign K20 = ~(Y0 & L20);

assign K1 = ~(Y1 & L1);
assign K5 = ~(Y1 & L5_w);
assign K9 = ~(Y1 & L9);
assign K13 = ~(Y1 & L13_w);
assign K17 = ~(Y1 & L17_w);
assign K21 = ~(Y1 & L21);
assign K25 = ~(Y1 & L25);
assign K29 = ~(Y1 & L29_w);

assign K2 = ~(Y2 & L2);
assign K6 = ~(Y2 & L6_w);
assign K10 = ~(Y2 & L10_w);
assign K14 = ~(Y2 & L14_w);
assign K18 = ~(Y2 & L18);
assign K22 = ~(Y2 & L22);
assign K26 = ~(Y2 & L26_w);
assign K30 = ~(Y2 & L30_w);

assign K3 = ~(Y3 & L3);
assign K7 = ~(Y3 & L7_w);
assign K11 = ~(Y3 & L11_w);
assign K15 = ~(Y3 & L15_w);
assign K19 = ~(Y3 & L19_w);
assign K23 = ~(Y3 & L23);
assign K27 = ~(Y3 & L27_w);
assign K31 = ~(Y3 & L31_w);


assign R[0] = (K0 ^ K1) ^ (K2 ^ K3);
assign R[1] = (K4 ^ K5) ^ (K6 ^ K7);
assign R[2] = (K8 ^ K9) ^ (K10 ^ K11);
assign R[3] = (K12 ^ K13) ^ (K14 ^ K15);
assign R[4] = (K16 ^ K17) ^ (K18 ^ K19);
assign R[5] = (K20 ^ K21) ^ (K22 ^ K23);
assign R[6] = (K24 ^ K25) ^ (K26 ^ K27);
assign R[7] = (K28 ^ K29) ^ (K30 ^ K31);

endmodule
