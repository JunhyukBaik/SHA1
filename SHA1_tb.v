`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/24 13:32:41
// Design Name: 
// Module Name: SHA1_tb
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


module SHA1_tb;

reg clk, nrst, start;
reg [511:0] sha1in;
wire [159:0] sha1out;
wire done;

SHA1 shasha(clk, nrst, start, sha1in, done, sha1out);

always
    #5 clk = ~clk;

initial
begin
    clk = 0;
    nrst = 0;
    start = 0;
    sha1in = 0;
end

initial #20 nrst = 1;

initial
begin
    #30 start = 1;
    sha1in = 512'h74657374800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020;
    #10 start = 0;
    sha1in = 512'h0;
end

endmodule
