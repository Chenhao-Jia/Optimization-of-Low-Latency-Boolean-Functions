// SPEEDY S-box (structural)
// - Each OR4 is implemented as a 2-level OR2 tree
// - Each AND3 is implemented as a 2-level AND2 tree
// - All logic uses gate modules (INV/AND2/OR2) via instantiation

module INV(input  wire a, output wire y);
    assign y = ~a;
endmodule

module AND2(input wire a, input wire b, output wire y);
    assign y = a & b;
endmodule

module OR2(input wire a, input wire b, output wire y);
    assign y = a | b;
endmodule

module sbox_speedy (
    input  wire [5:0] X,   // X[0]=x0, ..., X[5]=x5
    output wire [5:0] Y    // Y[0]=y0, ..., Y[5]=y5
);
    // split inputs
    wire x0 = X[0];
    wire x1 = X[1];
    wire x2 = X[2];
    wire x3 = X[3];
    wire x4 = X[4];
    wire x5 = X[5];

    // inverted inputs (reused)
    wire nx0, nx1, nx2, nx3, nx4, nx5;
    INV u_inv0(.a(x0), .y(nx0));
    INV u_inv1(.a(x1), .y(nx1));
    INV u_inv2(.a(x2), .y(nx2));
    INV u_inv3(.a(x3), .y(nx3));
    INV u_inv4(.a(x4), .y(nx4));
    INV u_inv5(.a(x5), .y(nx5));

    // =========================
    // y0 = ( x3 & ~x5 ) | ( x3 & x4 & x2 ) | ( ~x3 & x1 & x0 ) | ( x5 & x4 & x1 )
    // =========================
    wire y0_t0, y0_t1, y0_t2, y0_t3;
    wire y0_a1, y0_a2, y0_a3;
    AND2 u_y0_t0(.a(x3),  .b(nx5), .y(y0_t0));

    AND2 u_y0_a1(.a(x3),  .b(x4),  .y(y0_a1));
    AND2 u_y0_t1(.a(y0_a1), .b(x2), .y(y0_t1));

    AND2 u_y0_a2(.a(nx3), .b(x1),  .y(y0_a2));
    AND2 u_y0_t2(.a(y0_a2), .b(x0), .y(y0_t2));

    AND2 u_y0_a3(.a(x5),  .b(x4),  .y(y0_a3));
    AND2 u_y0_t3(.a(y0_a3), .b(x1), .y(y0_t3));

    wire y0_or01, y0_or23;
    OR2  u_y0_or01(.a(y0_t0), .b(y0_t1), .y(y0_or01));
    OR2  u_y0_or23(.a(y0_t2), .b(y0_t3), .y(y0_or23));
    OR2  u_y0_out (.a(y0_or01), .b(y0_or23), .y(Y[0]));

    // =========================
    // y1 = ( x5 & x3 & ~x2 ) | ( ~x5 & x3 & ~x4 ) | ( x5 & x2 & x0 ) | ( ~x3 & ~x0 & x1 )
    // =========================
    wire y1_t0, y1_t1, y1_t2, y1_t3;
    wire y1_a0, y1_a1, y1_a2, y1_a3;

    AND2 u_y1_a0(.a(x5), .b(x3), .y(y1_a0));
    AND2 u_y1_t0(.a(y1_a0), .b(nx2), .y(y1_t0));

    AND2 u_y1_a1(.a(nx5), .b(x3), .y(y1_a1));
    AND2 u_y1_t1(.a(y1_a1), .b(nx4), .y(y1_t1));

    AND2 u_y1_a2(.a(x5), .b(x2), .y(y1_a2));
    AND2 u_y1_t2(.a(y1_a2), .b(x0), .y(y1_t2));

    AND2 u_y1_a3(.a(nx3), .b(nx0), .y(y1_a3));
    AND2 u_y1_t3(.a(y1_a3), .b(x1), .y(y1_t3));

    wire y1_or01, y1_or23;
    OR2  u_y1_or01(.a(y1_t0), .b(y1_t1), .y(y1_or01));
    OR2  u_y1_or23(.a(y1_t2), .b(y1_t3), .y(y1_or23));
    OR2  u_y1_out (.a(y1_or01), .b(y1_or23), .y(Y[1]));

    // =========================
    // y2 = ( ~x3 & x0 & x4 ) | ( x3 & x0 & x1 ) | ( ~x3 & ~x4 & x2 ) | ( ~x0 & ~x2 & ~x5 )
    // =========================
    wire y2_t0, y2_t1, y2_t2, y2_t3;
    wire y2_a0, y2_a1, y2_a2, y2_a3;

    AND2 u_y2_a0(.a(nx3), .b(x0), .y(y2_a0));
    AND2 u_y2_t0(.a(y2_a0), .b(x4), .y(y2_t0));

    AND2 u_y2_a1(.a(x3),  .b(x0), .y(y2_a1));
    AND2 u_y2_t1(.a(y2_a1), .b(x1), .y(y2_t1));

    AND2 u_y2_a2(.a(nx3), .b(nx4), .y(y2_a2));
    AND2 u_y2_t2(.a(y2_a2), .b(x2), .y(y2_t2));

    AND2 u_y2_a3(.a(nx0), .b(nx2), .y(y2_a3));
    AND2 u_y2_t3(.a(y2_a3), .b(nx5), .y(y2_t3));

    wire y2_or01, y2_or23;
    OR2  u_y2_or01(.a(y2_t0), .b(y2_t1), .y(y2_or01));
    OR2  u_y2_or23(.a(y2_t2), .b(y2_t3), .y(y2_or23));
    OR2  u_y2_out (.a(y2_or01), .b(y2_or23), .y(Y[2]));

    // =========================
    // y3 = ( ~x0 & x2 & ~x3 ) | ( x0 & x2 & x4 ) | ( x0 & ~x2 & x5 ) | ( ~x0 & x3 & x1 )
    // =========================
    wire y3_t0, y3_t1, y3_t2, y3_t3;
    wire y3_a0, y3_a1, y3_a2, y3_a3;

    AND2 u_y3_a0(.a(nx0), .b(x2), .y(y3_a0));
    AND2 u_y3_t0(.a(y3_a0), .b(nx3), .y(y3_t0));

    AND2 u_y3_a1(.a(x0), .b(x2), .y(y3_a1));
    AND2 u_y3_t1(.a(y3_a1), .b(x4), .y(y3_t1));

    AND2 u_y3_a2(.a(x0), .b(nx2), .y(y3_a2));
    AND2 u_y3_t2(.a(y3_a2), .b(x5), .y(y3_t2));

    AND2 u_y3_a3(.a(nx0), .b(x3), .y(y3_a3));
    AND2 u_y3_t3(.a(y3_a3), .b(x1), .y(y3_t3));

    wire y3_or01, y3_or23;
    OR2  u_y3_or01(.a(y3_t0), .b(y3_t1), .y(y3_or01));
    OR2  u_y3_or23(.a(y3_t2), .b(y3_t3), .y(y3_or23));
    OR2  u_y3_out (.a(y3_or01), .b(y3_or23), .y(Y[3]));

    // =========================
    // y4 = ( x0 & ~x3 ) | ( x0 & ~x4 & ~x2 ) | ( ~x0 & x4 & x5 ) | ( ~x4 & ~x2 & x1 )
    // =========================
    wire y4_t0, y4_t1, y4_t2, y4_t3;
    wire y4_a1, y4_a2, y4_a3;

    AND2 u_y4_t0(.a(x0),  .b(nx3), .y(y4_t0));

    AND2 u_y4_a1(.a(x0),  .b(nx4), .y(y4_a1));
    AND2 u_y4_t1(.a(y4_a1), .b(nx2), .y(y4_t1));

    AND2 u_y4_a2(.a(nx0), .b(x4),  .y(y4_a2));
    AND2 u_y4_t2(.a(y4_a2), .b(x5), .y(y4_t2));

    AND2 u_y4_a3(.a(nx4), .b(nx2), .y(y4_a3));
    AND2 u_y4_t3(.a(y4_a3), .b(x1), .y(y4_t3));

    wire y4_or01, y4_or23;
    OR2  u_y4_or01(.a(y4_t0), .b(y4_t1), .y(y4_or01));
    OR2  u_y4_or23(.a(y4_t2), .b(y4_t3), .y(y4_or23));
    OR2  u_y4_out (.a(y4_or01), .b(y4_or23), .y(Y[4]));

    // =========================
    // y5 = ( x2 & x5 ) | ( ~x2 & ~x1 & x4 ) | ( x2 & x1 & x0 ) | ( ~x1 & x0 & x3 )
    // =========================
    wire y5_t0, y5_t1, y5_t2, y5_t3;
    wire y5_a1, y5_a2, y5_a3;

    AND2 u_y5_t0(.a(x2),  .b(x5),  .y(y5_t0));

    AND2 u_y5_a1(.a(nx2), .b(nx1), .y(y5_a1));
    AND2 u_y5_t1(.a(y5_a1), .b(x4), .y(y5_t1));

    AND2 u_y5_a2(.a(x2),  .b(x1),  .y(y5_a2));
    AND2 u_y5_t2(.a(y5_a2), .b(x0), .y(y5_t2));

    AND2 u_y5_a3(.a(nx1), .b(x0),  .y(y5_a3));
    AND2 u_y5_t3(.a(y5_a3), .b(x3), .y(y5_t3));

    wire y5_or01, y5_or23;
    OR2  u_y5_or01(.a(y5_t0), .b(y5_t1), .y(y5_or01));
    OR2  u_y5_or23(.a(y5_t2), .b(y5_t3), .y(y5_or23));
    OR2  u_y5_out (.a(y5_or01), .b(y5_or23), .y(Y[5]));

endmodule

