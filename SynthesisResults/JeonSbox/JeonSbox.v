// -----------------------------------------------------------------------------
// AES S-box — "New AES S-box circuit" (Listing 17, D=15, AD=4, #NL=34, #L=100)
// Source: Jeon et al., 2024, Listing 17 (eBPD result).
// Conventions: XOR(^), AND(&), XNOR is expressed as (a ^ b) ^ 1'b1 when needed.
// Purely combinational, inputs x[7:0] -> outputs y[7:0].
// -----------------------------------------------------------------------------
module aes_sbox_bp15 (
  input  wire [7:0] x,
  output wire [7:0] y
);

  // ----- Depth 0 -----
  wire r5  = x[0];
  wire r37 = x[0];

  // ----- Depth 1 -----
  wire t0  = x[1] ^ x[7];
  wire t1  = x[4] ^ x[7];
  wire t2  = x[2] ^ x[4];
  wire t4  = x[2] ^ x[7];
  wire t5  = x[5] ^ x[6];
  wire t10 = x[1] ^ x[3];
  wire t14 = x[2] ^ x[5];
  wire t18 = x[2] ^ x[6];
  wire r6  = t0;
  wire r12 = t1;
  wire r14 = t2;
  wire r16 = t4;
  wire r57 = t0;
  wire r63 = t1;
  wire r65 = t2;
  wire r67 = t4;

  // ----- Depth 2 -----
  wire t3  = t0 ^ t2;
  wire t6  = x[0] ^ t5;
  wire t11 = t1 ^ t10;
  wire t15 = t10 ^ t14;
  wire t17 = t1 ^ t14;
  wire t19 = t10 ^ t18;
  wire t25 = x[1] ^ t4;

  wire r0  = t3;
  wire r1  = t11;
  wire r7  = t15;
  wire r9  = t6;
  wire r13 = t19;
  wire r15 = t17;
  wire r33 = t11;
  wire r39 = t15;
  wire r41 = t6;
  wire r45 = t19;
  wire r47 = t17;
  wire r51 = t3;

  // ----- Depth 3 -----
  wire g0  = (r0 & r1);
  wire g3  = (r6 & r7);
  wire g6  = (r12 & r13);
  wire g7  = (r14 & r15);

  wire t7  = x[4] ^ t6;
  wire t8  = x[1] ^ t6;
  wire t9  = x[7] ^ t6;
  wire t12 = x[0] ^ t11;
  wire t13 = t5 ^ t11;
  wire t16 = t6 ^ t15;
  wire t26 = t6 ^ t25;

  wire r2  = t26;
  wire r3  = t12;
  wire r4  = t7;
  wire r8  = t8;
  wire r10 = t9;
  wire r11 = t16;
  wire r17 = t13;
  wire r35 = t12;
  wire r43 = t16;
  wire r49 = t13;
  wire r53 = t26;
  wire r55 = t7;
  wire r59 = t8;
  wire r61 = t9;

  // ----- Depth 4 -----
  wire g1  = (r2 & r3);
  wire g2  = (r4 & r5);
  wire g4  = (r8 & r9);
  wire g5  = (r10 & r11);
  wire g8  = (r16 & r17);

  wire t22 = t15 ^ g6;
  wire t27 = t14 ^ g7;
  wire t29 = t13 ^ g6;
  wire t36 = t0  ^ g3;
  wire t48 = x[7] ^ g3;

  // ----- Depth 5 -----
  wire t20 = g4 ^ g7;
  wire t21 = g8 ^ g5;
  wire t28 = g1 ^ t27;
  wire t30 = g0 ^ t29;
  wire t32 = t4 ^ g8;
  wire t37 = t22 ^ t36;
  wire t49 = t22 ^ t48;

  // ----- Depth 6 -----
  wire t23 = x[1] ^ t21;
  wire t31 = t28 ^ t30;
  wire t33 = g2  ^ t32;
  wire t38 = t20 ^ t37;
  wire t50 = t21 ^ t49;

  wire r18 = t38;
  wire r19 = t31;
  wire r24 = t31;
  wire r25 = t50;
  wire r29 = t38;

  // ----- Depth 7 -----
  wire g9  = (r18 & r19);
  wire g12 = (r24 & r25);

  wire t24 = t20 ^ t23;
  wire t34 = t28 ^ t33;
  wire t35 = t30 ^ t33;
  wire t50_d = t23 ^ t37; // (annotation only; same as t50 above)
  wire t60 = t31 ^ t38;

  wire r21 = t34;
  wire r23 = t24;
  wire r26 = t34;
  wire r27 = g12;
  wire r28 = t35;
  wire r30 = t24;

  // ----- Depth 8 -----
  wire g13 = (r26 & r27);
  wire g14 = (r28 & r29);

  wire t39 = t35 ^ g9;
  wire t41 = g9 ^ t50;
  wire t45 = t24 ^ g9;
  wire t53 = t24 ^ t34;
  wire t56 = t31 ^ g9;

  wire r20 = t41;
  wire r22 = t39;
  wire r31 = g14;

  // ----- Depth 9 -----
  wire g10 = (r20 & r21);
  wire g11 = (r22 & r23);
  wire g15 = (r30 & r31);

  wire t51 = t39 ^ g13;
  wire t54 = g13 ^ t53;
  wire t57 = g13 ^ t56;
  wire t61 = g13 ^ t60;

  // ----- Depth 10 -----
  wire t42 = g11 ^ t50;
  wire t43 = t35 ^ g10;
  wire t46 = g15 ^ t45;
  wire t52 = t31 ^ t51;
  wire t55 = g15 ^ t54;
  wire t58 = g10 ^ t57;
  wire t59 = g15 ^ g11;
  wire t62 = g10 ^ t61;

  wire r34 = t46;
  wire r36 = t42;
  wire r38 = t58;
  wire r40 = t52;
  wire r42 = t43;
  wire r48 = t55;
  wire r52 = t46;
  wire r54 = t42;
  wire r56 = t58;
  wire r58 = t52;
  wire r60 = t43;
wire r66 = t55;

  // ----- Depth 11 -----
  wire g17 = (r34 & r35);
  wire g18 = (r36 & r37);
  wire g19 = (r38 & r39);
  wire g20 = (r40 & r41);
  wire g21 = (r42 & r43);
  wire g24 = (r48 & r49);
  wire g26 = (r52 & r53);
  wire g27 = (r54 & r55);
  wire g28 = (r56 & r57);
  wire g29 = (r58 & r59);
  wire g30 = (r60 & r61);
  wire g33 = (r66 & r67);

  wire t44 = t42 ^ t43;
  wire t47 = t42 ^ t46;
  wire t63 = t59 ^ t62;

  wire r32 = t47;
  wire r44 = t44;
  wire r46 = t63;
  wire r50 = t47;
  wire r62 = t44;
wire r64 = t63;

  // ----- Depth 12 -----
  wire g16 = (r32 & r33);
  wire g22 = (r44 & r45);
  wire g23 = (r46 & r47);
  wire g25 = (r50 & r51);
  wire g31 = (r62 & r63);
  wire g32 = (r64 & r65);

  wire t69 = g20 ^ g19;
  wire t70 = g21 ^ g20;
  wire t72 = g29 ^ g28;
  wire t89 = g24 ^ g28;
  wire t90 = g18 ^ g30;
  wire t96 = g27 ^ g26;

  // ----- Depth 13 -----
  wire t64 = g31 ^ g32;
  wire t65 = g26 ^ g25;
  wire t67 = g17 ^ g16;
  wire t68 = g22 ^ g23;
  wire t71 = g18 ^ t70;
  wire t82 = g24 ^ g23;
  wire t85 = g16 ^ t69;
  wire t88 = g22 ^ g31;
  wire t91 = t89 ^ t90;
  wire t97 = g23 ^ t96;

  // ----- Depth 14 -----
  wire t66 = t64 ^ t65;
  wire t73 = t64 ^ t72;
  wire t74 = t68 ^ t69;
  wire t76 = g17 ^ t71;
  wire t78 = t67 ^ t69;
  wire t80 = t67 ^ t68;
  wire t83 = t70 ^ t82;
  wire t86 = t71 ^ t85;
  wire t92 = g16 ^ t91;
  wire t93 = g33 ^ t88;
  wire t95 = t85 ^ t91;
  wire t98 = t64 ^ t97;

  // ----- Depth 15 (final) -----
  wire t75 = t66 ^ t74;
  wire t77 = t66 ^ t76;
  wire t79 = t66 ^ t78;
  wire t81 = t66 ^ t80 ^ 1'b1;
  wire t84 = t73 ^ t83 ^ 1'b1;
  wire t87 = t73 ^ t86 ^ 1'b1;
  wire t94 = t92 ^ t93 ^ 1'b1;
  wire t99 = t95 ^ t98;

  // outputs
  assign y[0] = t87;
  assign y[1] = t84;
  assign y[2] = t99;
  assign y[3] = t77;
  assign y[4] = t79;
  assign y[5] = t94;
  assign y[6] = t81;
  assign y[7] = t75;

endmodule
