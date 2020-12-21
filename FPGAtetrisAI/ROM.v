module ROM(
    input clk,
    input [14:0] address,
    output reg [1:0] q
);

/*
160x144@2bit (46.080 bits)
*/

reg [1:0] rom [0:23039];

always @(negedge clk) 
begin
    q <= rom[address];
end

initial
begin
    $readmemb("rom.list", rom);
end

endmodule