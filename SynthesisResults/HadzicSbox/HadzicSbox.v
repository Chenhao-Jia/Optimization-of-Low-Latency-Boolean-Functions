// -----------------------------------------------------------------------------
// 8-bit S-box — 139-line gate-level implementation (no LUT)
//
// Ports: x0..x7 -> y0..y7
// Every assign corresponds 1:1 to the user's 139 equations.
// -----------------------------------------------------------------------------
module hadzic_sbox (
    input  wire x0, input wire x1, input wire x2, input wire x3,
    input  wire x4, input wire x5, input wire x6, input wire x7,
    output wire y0, output wire y1, output wire y2, output wire y3,
    output wire y4, output wire y5, output wire y6, output wire y7
);
    // ---------------- temps ----------------
    // p
    wire p0, p1, p2, p3;
    // a
    wire a0, a1, a2, a3, a4, a5, a6, a7, a8;
    // b
    wire b1, b2, b3, b4, b5, b6, b7, b8;
    // c
    wire c0, c1, c2, c3;
    // d
    wire d0, d1, d2, d3, d4, d5, d6, d7, d8;
    // q
    wire q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14;
    // e
    wire e0, e1, e2, e3, e4, e5, e6, e7, e8;
    // f, g, h
    wire f0, f1, f2, f3, f4, f5, f6, f7, f8;
    wire g0, g1, g2, g3, g4, g5, g6, g7, g8;
    wire h0, h1, h2;
    // r / i / s / j
    wire r0, r1, r2, r3, r4, r5;
    wire i0, i1, i2, i3, i4, i5;
    wire s0, s1, s2, s3, s4, s5;
    wire j0, j1, j2, j3, j4, j5;
    // t / k / l / m / u
    wire t0, t1;
    wire k0, k1, k2;
    wire l0, l1, l2, l3, l4, l5;
    wire m0, m1, m2, m3, m4, m5;
    wire u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;

    // ---------------- equations (1..139) ----------------
    // L0 / precompute
    assign p0 = x1 ^ x3;                     // 1
    assign p1 = x5 ^ x6;                     // 2
    assign b4 = x0 ^ p1;                     // 3
    assign p2 = x2 ^ x5;                     // 4
    assign a7 = x2 ^ x7;                     // 5
    assign a0 = x4 ^ b4;                     // 6
    assign a5 = x1 ^ x7;                     // 7
    assign a6 = x4 ^ x7;                     // 8
    assign a8 = x2 ^ x4;                     // 9
    assign b2 = p0 ^ a6;                     //10
    assign b5 = p0 ^ p2;                     //11
    assign c0 = x7 ^ b5;                     //12
    assign p3 = x2 ^ a5;                     //13
    assign b7 = p1 ^ b2;                     //14
    assign a4 = x1 ^ b4;                     //15
    assign b8 = p2 ^ a6;                     //16
    assign a3 = x7 ^ b4;                     //17
    assign c1 = x1 ^ c0;                     //18
    assign b6 = p1 ^ b5;                     //19
    assign a2 = x4 ^ p3;                     //20
    assign b1 = x0 ^ b2;                     //21
    assign b3 = b4 ^ b5;                     //22
    assign c2 = a7 ^ b7;                     //23
    assign c3 = a6 ^ b6;                     //24
    assign a1 = b4 ^ p3;                     //25

    // N0 layer
    assign d0 = a0 & x0;                     //26
    assign d1 = a1 & b1;                     //27
    assign d2 = a2 & b2;                     //28
    assign d3 = a3 & b3;                     //29
    assign d4 = a4 & b4;                     //30
    assign d5 = a5 & b5;                     //31
    assign d6 = a6 & b6;                     //32
    assign d7 = a7 & b7;                     //33
    assign d8 = a8 & b8;                     //34

    // L1 layer
    assign q0  = d6 ^ d2;                    //35
    assign q1  = d7 ^ d3;                    //36
    assign q2  = q0 ^ q1;                    //37
    assign q3  = d1 ^ c3;                    //38
    assign q4  = d8 ^ q0;                    //39
    assign e4  = q3 ^ q4;                    //40
    assign q5  = d5 ^ d2;                    //41
    assign q6  = c0 ^ q5;                    //42
    assign q7  = d4 ^ c1;                    //43
    assign q8  = d3 ^ c0;                    //44
    assign e0  = q2 ^ q6;                    //45
    assign q9  = q5 ^ q7;                    //46
    assign q10 = d0 ^ c2;                    //47
    assign q11 = d7 ^ d8;                    //48
    assign e1  = q4 ^ q9;                    //49
    assign q12 = d3 ^ q10;                   //50
    assign e7  = q3 ^ q9;                    //51
    assign e3  = q2 ^ q12;                   //52
    assign e6  = q6 ^ q12;                   //53
    assign q13 = q3 ^ q10;                   //54
    assign e5  = q11 ^ q13;                  //55
    assign q14 = q7 ^ q8;                    //56
    assign e8  = q13 ^ q14;                  //57
    assign e2  = q11 ^ q14;                  //58

    // N1 layer
    assign f0 = a0 & e0;                     //59
    assign f1 = a1 & e1;                     //60
    assign f2 = a2 & e2;                     //61
    assign f3 = a3 & e3;                     //62
    assign f4 = a4 & e4;                     //63
    assign f5 = a5 & e5;                     //64
    assign f6 = a6 & e6;                     //65
    assign f7 = a7 & e7;                     //66
    assign f8 = a8 & e8;                     //67
    assign g0 = x0 & e0;                     //68
    assign g1 = b1 & e1;                     //69
    assign g2 = b2 & e2;                     //70
    assign g3 = b3 & e3;                     //71
    assign g4 = b4 & e4;                     //72
    assign g5 = b5 & e5;                     //73
    assign g6 = b6 & e6;                     //74
    assign g7 = b7 & e7;                     //75
    assign g8 = b8 & e8;                     //76
    assign h0 = e0 & e3;                     //77
    assign h1 = e1 & e4;                     //78
    assign h2 = e2 & e5;                     //79

    // L^S_2 layer (from f*)
    assign r0 = f1 ^ f8;                     //80
    assign r1 = f0 ^ f7;                     //81
    assign r2 = f2 ^ f6;                     //82
    assign i0 = r1 ^ r2;                     //83
    assign r3 = f4 ^ f8;                     //84
    assign i1 = r0 ^ r2;                     //85
    assign r4 = f3 ^ f7;                     //86
    assign i5 = r3 ^ r4;                     //87
    assign i2 = r0 ^ r1;                     //88
    assign r5 = f5 ^ f6;                     //89
    assign i4 = r3 ^ r5;                     //90
    assign i3 = r4 ^ r5;                     //91

    // L^S_2 layer (from g*)
    assign s0 = g1 ^ g8;                     //92
    assign s1 = g0 ^ g7;                     //93
    assign s2 = g2 ^ g6;                     //94
    assign j0 = s1 ^ s2;                     //95
    assign s3 = g4 ^ g8;                     //96
    assign j1 = s0 ^ s2;                     //97
    assign s4 = g3 ^ g7;                     //98
    assign j5 = s3 ^ s4;                     //99
    assign j2 = s0 ^ s1;                     //100
    assign s5 = g5 ^ g6;                     //101
    assign j4 = s3 ^ s5;                     //102
    assign j3 = s4 ^ s5;                     //103

    // LM^2 layer
    assign t0 = e7 ^ h1;                     //104
    assign k0 = h2 ^ t0;                     //105
    assign t1 = e8 ^ h0;                     //106
    assign k2 = t0 ^ t1;                     //107
    assign k1 = h2 ^ t1;                     //108

    // N2 layer
    assign l0 = k0 & i0;                     //109
    assign l1 = k1 & i1;                     //110
    assign l2 = k2 & i2;                     //111
    assign l3 = k0 & i3;                     //112
    assign l4 = k1 & i4;                     //113
    assign l5 = k2 & i5;                     //114
    assign m0 = k0 & j0;                     //115
    assign m1 = k1 & j1;                     //116
    assign m2 = k2 & j2;                     //117
    assign m3 = k0 & j3;                     //118
    assign m4 = k1 & j4;                     //119
    assign m5 = k2 & j5;                     //120

    // L3 layer & outputs
    assign u0  = l0 ^ l1;                    //121
    assign u1  = m0 ^ m2;                    //122
    assign u2  = m4 ^ m5;                    //123
    assign u3  = l3 ^ l5;                    //124
    assign u4  = l1 ^ l2;                    //125
    assign u5  = m1 ^ m2;                    //126
    assign u6  = m3 ^ m4;                    //127
    assign u9  = l4 ^ l5;                    //128
    assign u7  = u0 ^ u3;                    //129
    assign u8  = u1 ^ u2;                    //130
    assign u10 = u1 ^ u6;                    //131
    assign y7  = u2 ^ u4;                    //132
    assign y6  = u4 ^ u5;                    //133
    assign y5  = u1 ^ u3;                    //134
    assign y4  = u5 ^ y7;                    //135
    assign y3  = y6 ^ u10;                   //136
    assign y2  = u7 ^ u8;                    //137
    assign y1  = u6 ^ u9;                    //138
    assign y0  = u8 ^ y1;                    //139
endmodule
