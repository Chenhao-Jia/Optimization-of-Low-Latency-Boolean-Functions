// SKINNY-128 S-box (structural)
// Only NOR2 and XOR2 are used in the datapath (no operators inside sbox module)

module NOR2 (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = ~(a | b);
endmodule

module XOR2 (
    input  wire a,
    input  wire b,
    output wire y
);
    assign y = a ^ b;
endmodule


module sbox_skinny128 (
    input  wire [7:0] X,
    output wire [7:0] Y
);
    wire t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;

    // t0 = ~(X[7] | X[6])
    NOR2 u_nor0  (.a(X[7]), .b(X[6]), .y(t0));
    // t1 = X[4] ^ t0
    XOR2 u_xor1  (.a(X[4]), .b(t0),   .y(t1));

    // t2 = ~(X[3] | X[2])
    NOR2 u_nor2  (.a(X[3]), .b(X[2]), .y(t2));
    // t3 = X[0] ^ t2
    XOR2 u_xor3  (.a(X[0]), .b(t2),   .y(t3));

    // t4 = ~(X[2] | X[1])
    NOR2 u_nor4  (.a(X[2]), .b(X[1]), .y(t4));
    // t5 = X[6] ^ t4
    XOR2 u_xor5  (.a(X[6]), .b(t4),   .y(t5));

    // t6 = ~(t1 | t3)
    NOR2 u_nor6  (.a(t1),   .b(t3),   .y(t6));
    // t7 = X[5] ^ t6
    XOR2 u_xor7  (.a(X[5]), .b(t6),   .y(t7));

    // t8 = ~(X[3] | t3)
    NOR2 u_nor8  (.a(X[3]), .b(t3),   .y(t8));
    // t9 = X[1] ^ t8
    XOR2 u_xor9  (.a(X[1]), .b(t8),   .y(t9));

    // t10 = ~(X[7] | X[6])
    NOR2 u_nor10 (.a(t5), .b(t7), .y(t10));
    // t11 = X[7] ^ t10
    XOR2 u_xor11 (.a(X[7]), .b(t10),  .y(t11));

    // t12 = ~(t1 | t7)
    NOR2 u_nor12 (.a(t1),   .b(t7),   .y(t12));
    // t13 = X[3] ^ t12
    XOR2 u_xor13 (.a(X[3]), .b(t12),  .y(t13));

    // t14 = ~(t9 | t11)
    NOR2 u_nor14 (.a(t9),   .b(t11),  .y(t14));
    // t15 = X[2] ^ t14
    XOR2 u_xor15 (.a(X[2]), .b(t14),  .y(t15));

    assign Y = {t7, t1, t3, t13, t9, t5, t11, t15};

endmodule

