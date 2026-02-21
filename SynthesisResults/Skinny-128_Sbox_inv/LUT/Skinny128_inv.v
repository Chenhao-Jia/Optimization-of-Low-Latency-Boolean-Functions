// SKINNY-128 inverse S-box LUT implementation (8-bit -> 8-bit)
module sbox_skinny128_inv (
    input  [7:0] X,   // 8-bit input
    output reg [7:0] Y // 8-bit output
);

always @(*) begin
    case (X)
        8'h00: Y = 8'hAC;
        8'h01: Y = 8'hE8;
        8'h02: Y = 8'h68;
        8'h03: Y = 8'h3C;
        8'h04: Y = 8'h6C;
        8'h05: Y = 8'h38;
        8'h06: Y = 8'hA8;
        8'h07: Y = 8'hEC;
        8'h08: Y = 8'hAA;
        8'h09: Y = 8'hAE;
        8'h0A: Y = 8'h3A;
        8'h0B: Y = 8'h3E;
        8'h0C: Y = 8'h6A;
        8'h0D: Y = 8'h6E;
        8'h0E: Y = 8'hEA;
        8'h0F: Y = 8'hEE;

        8'h10: Y = 8'hA6;
        8'h11: Y = 8'hA3;
        8'h12: Y = 8'h33;
        8'h13: Y = 8'h36;
        8'h14: Y = 8'h66;
        8'h15: Y = 8'h63;
        8'h16: Y = 8'hE3;
        8'h17: Y = 8'hE6;
        8'h18: Y = 8'hE1;
        8'h19: Y = 8'hA4;
        8'h1A: Y = 8'h61;
        8'h1B: Y = 8'h34;
        8'h1C: Y = 8'h31;
        8'h1D: Y = 8'h64;
        8'h1E: Y = 8'hA1;
        8'h1F: Y = 8'hE4;

        8'h20: Y = 8'h8D;
        8'h21: Y = 8'hC9;
        8'h22: Y = 8'h49;
        8'h23: Y = 8'h1D;
        8'h24: Y = 8'h4D;
        8'h25: Y = 8'h19;
        8'h26: Y = 8'h89;
        8'h27: Y = 8'hCD;
        8'h28: Y = 8'h8B;
        8'h29: Y = 8'h8F;
        8'h2A: Y = 8'h1B;
        8'h2B: Y = 8'h1F;
        8'h2C: Y = 8'h4B;
        8'h2D: Y = 8'h4F;
        8'h2E: Y = 8'hCB;
        8'h2F: Y = 8'hCF;

        8'h30: Y = 8'h85;
        8'h31: Y = 8'hC0;
        8'h32: Y = 8'h40;
        8'h33: Y = 8'h15;
        8'h34: Y = 8'h45;
        8'h35: Y = 8'h10;
        8'h36: Y = 8'h80;
        8'h37: Y = 8'hC5;
        8'h38: Y = 8'h82;
        8'h39: Y = 8'h87;
        8'h3A: Y = 8'h12;
        8'h3B: Y = 8'h17;
        8'h3C: Y = 8'h42;
        8'h3D: Y = 8'h47;
        8'h3E: Y = 8'hC2;
        8'h3F: Y = 8'hC7;

        8'h40: Y = 8'h96;
        8'h41: Y = 8'h93;
        8'h42: Y = 8'h03;
        8'h43: Y = 8'h06;
        8'h44: Y = 8'h56;
        8'h45: Y = 8'h53;
        8'h46: Y = 8'hD3;
        8'h47: Y = 8'hD6;
        8'h48: Y = 8'hD1;
        8'h49: Y = 8'h94;
        8'h4A: Y = 8'h51;
        8'h4B: Y = 8'h04;
        8'h4C: Y = 8'h01;
        8'h4D: Y = 8'h54;
        8'h4E: Y = 8'h91;
        8'h4F: Y = 8'hD4;

        8'h50: Y = 8'h9C;
        8'h51: Y = 8'hD8;
        8'h52: Y = 8'h58;
        8'h53: Y = 8'h0C;
        8'h54: Y = 8'h5C;
        8'h55: Y = 8'h08;
        8'h56: Y = 8'h98;
        8'h57: Y = 8'hDC;
        8'h58: Y = 8'h9A;
        8'h59: Y = 8'h9E;
        8'h5A: Y = 8'h0A;
        8'h5B: Y = 8'h0E;
        8'h5C: Y = 8'h5A;
        8'h5D: Y = 8'h5E;
        8'h5E: Y = 8'hDA;
        8'h5F: Y = 8'hDE;

        8'h60: Y = 8'h95;
        8'h61: Y = 8'hD0;
        8'h62: Y = 8'h50;
        8'h63: Y = 8'h05;
        8'h64: Y = 8'h55;
        8'h65: Y = 8'h00;
        8'h66: Y = 8'h90;
        8'h67: Y = 8'hD5;
        8'h68: Y = 8'h92;
        8'h69: Y = 8'h97;
        8'h6A: Y = 8'h02;
        8'h6B: Y = 8'h07;
        8'h6C: Y = 8'h52;
        8'h6D: Y = 8'h57;
        8'h6E: Y = 8'hD2;
        8'h6F: Y = 8'hD7;

        8'h70: Y = 8'h9D;
        8'h71: Y = 8'hD9;
        8'h72: Y = 8'h59;
        8'h73: Y = 8'h0D;
        8'h74: Y = 8'h5D;
        8'h75: Y = 8'h09;
        8'h76: Y = 8'h99;
        8'h77: Y = 8'hDD;
        8'h78: Y = 8'h9B;
        8'h79: Y = 8'h9F;
        8'h7A: Y = 8'h0B;
        8'h7B: Y = 8'h0F;
        8'h7C: Y = 8'h5B;
        8'h7D: Y = 8'h5F;
        8'h7E: Y = 8'hDB;
        8'h7F: Y = 8'hDF;

        8'h80: Y = 8'h16;
        8'h81: Y = 8'h13;
        8'h82: Y = 8'h83;
        8'h83: Y = 8'h86;
        8'h84: Y = 8'h46;
        8'h85: Y = 8'h43;
        8'h86: Y = 8'hC3;
        8'h87: Y = 8'hC6;
        8'h88: Y = 8'h41;
        8'h89: Y = 8'h14;
        8'h8A: Y = 8'hC1;
        8'h8B: Y = 8'h84;
        8'h8C: Y = 8'h11;
        8'h8D: Y = 8'h44;
        8'h8E: Y = 8'h81;
        8'h8F: Y = 8'hC4;

        8'h90: Y = 8'h1C;
        8'h91: Y = 8'h48;
        8'h92: Y = 8'hC8;
        8'h93: Y = 8'h8C;
        8'h94: Y = 8'h4C;
        8'h95: Y = 8'h18;
        8'h96: Y = 8'h88;
        8'h97: Y = 8'hCC;
        8'h98: Y = 8'h1A;
        8'h99: Y = 8'h1E;
        8'h9A: Y = 8'h8A;
        8'h9B: Y = 8'h8E;
        8'h9C: Y = 8'h4A;
        8'h9D: Y = 8'h4E;
        8'h9E: Y = 8'hCA;
        8'h9F: Y = 8'hCE;

        8'hA0: Y = 8'h35;
        8'hA1: Y = 8'h60;
        8'hA2: Y = 8'hE0;
        8'hA3: Y = 8'hA5;
        8'hA4: Y = 8'h65;
        8'hA5: Y = 8'h30;
        8'hA6: Y = 8'hA0;
        8'hA7: Y = 8'hE5;
        8'hA8: Y = 8'h32;
        8'hA9: Y = 8'h37;
        8'hAA: Y = 8'hA2;
        8'hAB: Y = 8'hA7;
        8'hAC: Y = 8'h62;
        8'hAD: Y = 8'h67;
        8'hAE: Y = 8'hE2;
        8'hAF: Y = 8'hE7;

        8'hB0: Y = 8'h3D;
        8'hB1: Y = 8'h69;
        8'hB2: Y = 8'hE9;
        8'hB3: Y = 8'hAD;
        8'hB4: Y = 8'h6D;
        8'hB5: Y = 8'h39;
        8'hB6: Y = 8'hA9;
        8'hB7: Y = 8'hED;
        8'hB8: Y = 8'h3B;
        8'hB9: Y = 8'h3F;
        8'hBA: Y = 8'hAB;
        8'hBB: Y = 8'hAF;
        8'hBC: Y = 8'h6B;
        8'hBD: Y = 8'h6F;
        8'hBE: Y = 8'hEB;
        8'hBF: Y = 8'hEF;

        8'hC0: Y = 8'h26;
        8'hC1: Y = 8'h23;
        8'hC2: Y = 8'hB3;
        8'hC3: Y = 8'hB6;
        8'hC4: Y = 8'h76;
        8'hC5: Y = 8'h73;
        8'hC6: Y = 8'hF3;
        8'hC7: Y = 8'hF6;
        8'hC8: Y = 8'h71;
        8'hC9: Y = 8'h24;
        8'hCA: Y = 8'hF1;
        8'hCB: Y = 8'hB4;
        8'hCC: Y = 8'h21;
        8'hCD: Y = 8'h74;
        8'hCE: Y = 8'hB1;
        8'hCF: Y = 8'hF4;

        8'hD0: Y = 8'h2C;
        8'hD1: Y = 8'h78;
        8'hD2: Y = 8'hF8;
        8'hD3: Y = 8'hBC;
        8'hD4: Y = 8'h7C;
        8'hD5: Y = 8'h28;
        8'hD6: Y = 8'hB8;
        8'hD7: Y = 8'hFC;
        8'hD8: Y = 8'h2A;
        8'hD9: Y = 8'h2E;
        8'hDA: Y = 8'hBA;
        8'hDB: Y = 8'hBE;
        8'hDC: Y = 8'h7A;
        8'hDD: Y = 8'h7E;
        8'hDE: Y = 8'hFA;
        8'hDF: Y = 8'hFE;

        8'hE0: Y = 8'h25;
        8'hE1: Y = 8'h70;
        8'hE2: Y = 8'hF0;
        8'hE3: Y = 8'hB5;
        8'hE4: Y = 8'h75;
        8'hE5: Y = 8'h20;
        8'hE6: Y = 8'hB0;
        8'hE7: Y = 8'hF5;
        8'hE8: Y = 8'h22;
        8'hE9: Y = 8'h27;
        8'hEA: Y = 8'hB2;
        8'hEB: Y = 8'hB7;
        8'hEC: Y = 8'h72;
        8'hED: Y = 8'h77;
        8'hEE: Y = 8'hF2;
        8'hEF: Y = 8'hF7;

        8'hF0: Y = 8'h2D;
        8'hF1: Y = 8'h79;
        8'hF2: Y = 8'hF9;
        8'hF3: Y = 8'hBD;
        8'hF4: Y = 8'h7D;
        8'hF5: Y = 8'h29;
        8'hF6: Y = 8'hB9;
        8'hF7: Y = 8'hFD;
        8'hF8: Y = 8'h2B;
        8'hF9: Y = 8'h2F;
        8'hFA: Y = 8'hBB;
        8'hFB: Y = 8'hBF;
        8'hFC: Y = 8'h7B;
        8'hFD: Y = 8'h7F;
        8'hFE: Y = 8'hFB;
        8'hFF: Y = 8'hFF;

        default: Y = 8'h00;
    endcase
end

endmodule

