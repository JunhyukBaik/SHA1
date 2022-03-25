`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/16 13:03:59
// Design Name: 
// Module Name: SHA-1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module SHA1(
    input CLK, nRST, START,
    input [511:0] SHA1IN,
    output reg DONE,
    output reg [159:0] SHA1OUT
    );

reg [31:0] H0, H1, H2, H3, H4;

reg [31:0] W[0:79]; // word sequence

parameter K0 = 32'h5A827999;
parameter K1 = 32'h6ED9EBA1;
parameter K2 = 32'h8F1BBCDC;
parameter K3 = 32'hCA62C1D6;

// State Definition
parameter S0 = 3'd0;   // 
parameter S1 = 3'd1;   // round 1
parameter S2 = 3'd2;   // round 2
parameter S3 = 3'd3;   // round 3
parameter S4 = 3'd4;   // round 4
parameter S5 = 3'd5;   // round 5

// Internal state variables
reg [2:0] state;
reg [2:0] next_state;

reg [31:0] temp; // temporary word value
reg [31:0] A, B, C, D, E; // 32 bits word buffers

integer t;
integer cnt;

always @(posedge CLK or negedge nRST or posedge START)   // state changes at positive edge clock or negative edge reset
if ((!nRST) || START)
    state <= S0;
else
    state <= next_state;

always @(state or posedge CLK)
begin
    case (state)
        S0: begin   // No change, use default
                cnt = 0;
                H0 = 32'h67452301;  // Default
                H1 = 32'hEFCDAB89;
                H2 = 32'h98BADCFE;
                H3 = 32'h10325476;
                H4 = 32'hC3D2E1F0;
                DONE = 0;
                SHA1OUT = 0;
                for (t = 0; t < 16; t = t + 1)
                begin
                    W[t] = SHA1IN[(t * 4) * 8 +: 8] << 24;  // make 8-bits into 32-bits
                    W[t] = W[t] | ((SHA1IN[(t * 4 + 1) * 8 +: 8] << 16));
                    W[t] = W[t] | ((SHA1IN[(t * 4 + 2) * 8 +: 8] << 8));
                    W[t] = W[t] | ((SHA1IN[(t * 4 + 3) * 8 +: 8]));
                end
                
                for (t = 16; t < 80; t = t + 1)
                begin
                    W[t] = CircularShift((W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]), 1);
                end
                  
                A = H0;
                B = H1;
                C = H2;
                D = H3;
                E = H4;
            end 
        
        S1:    // round 1
            begin
                temp = CircularShift(A, 5) + (((B & C) | ((~B) & D)) + E + W[cnt] + K0);
                E = D;
                D = C;
                C = CircularShift(B, 30);
                B = A;
                A = temp;
                cnt = cnt + 1;
            end
            
        S2:    // round 2
            begin
                temp = CircularShift(A, 5);
                temp = temp + ((B ^ C ^ D) + E + W[cnt] + K1);
                E = D;
                D = C;
                C = CircularShift(B, 30);
                B = A;
                A = temp;
                cnt = cnt + 1;
            end          
        
        S3:    // round 3
            begin
                temp = CircularShift(A, 5);
                temp = temp + (((B & C) | (B & D) | (C & D)) + E + W[cnt] + K2);
                E = D;
                D = C;
                C = CircularShift(B, 30);
                B = A;
                A = temp;
                cnt = cnt + 1;
            end
        
        S4:     // round 4
            begin
                temp = CircularShift(A, 5);
                temp = temp + ((B ^ C ^ D) + E + W[cnt] + K3);
                E = D;
                D = C;
                C = CircularShift(B, 30);
                B = A;
                A = temp;
                cnt = cnt + 1;           
            end
            
        S5:     // 마지막
            begin
                H0 = H0 + A;
                H1 = H1 + B;
                H2 = H2 + C;
                H3 = H3 + D;
                H4 = H4 + E;
                DONE = 1;
                SHA1OUT = (H0 << 128) + (H1 << 96) + (H2 << 64) + (H3 << 32) + H4;
                @(posedge CLK);
                DONE = 0;
                SHA1OUT = 0;
            end
    endcase
end

always @(state or cnt or posedge START)
begin
    case (state)
        S0: begin
            if (START)
            next_state = S1;
            end
        S1: begin
            if (cnt == 19)
                next_state = S2;
            end
        S2: begin
            if (cnt == 40)
                next_state = S3;
            end
        S3: begin
            if (cnt == 60 )
                next_state = S4;
            end
        S4: begin
            if (cnt == 80)
                next_state = S5;
            end
        S5: begin
                next_state = S0;
            end    
        default: next_state = S0;
    endcase
end


function [31:0] CircularShift;  // CircularShift
input [31:0] word;
input integer n;
begin
    CircularShift = (word << n) | (word >> (32 - n));
end
endfunction

endmodule
