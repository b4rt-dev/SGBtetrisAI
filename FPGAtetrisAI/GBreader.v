module GBreader(
    input  clk, //4MHz
    
    input hs, vs, d0, d1,
	 
	 output [14:0] addr,
	 output [1:0] data,
	 output we
);

/*
NOTES:
- The first line is sent while vsync is high
- There is only one clk pulse during the entire hsync pulse
- Data is set on posedge clk, so should be read (and write to framebuffer) on negedge clk
- A single frame is 160x144@2bit (46.080 bits)
*/

reg [7:0] hcount, pcount;

assign data = {d0, d1};
assign addr = (hcount*160) + pcount;
assign we = 1'b1;

always @(negedge clk)
begin
	
	if (hs)
	begin
		pcount <= 8'd1;
		if (vs)
		begin
			hcount <= 8'd0;
		end
		else
		begin
			hcount <= hcount + 1'b1;
		end
	end
	else
	begin
		pcount <= pcount + 1'b1;
	end
end


initial
begin
	hcount <= 8'd0;
	pcount <= 8'd0;
end






endmodule
