module analyzer(
	input  clk,
	input  [1:0] fbdat,
	input [14:0] fbrdaddress,
	output reg [187:0] boardString
);

parameter BITS_AMOUNT  = 188;

initial
begin
	boardString <= 188'd0;
end


integer i;

always @(negedge clk)
begin

	for (i = 0; i < 18; i = i + 1)
		begin
			//row i
			if (fbrdaddress == ((i*8*160) + 16))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 0)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 24))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 1)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 32))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 2)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 40))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 3)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 48))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 4)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 56))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 5)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 64))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 6)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 72))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 7)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 80))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 8)] <= (fbdat != 2'b00);
			if (fbrdaddress == ((i*8*160) + 88))
				boardString[(BITS_AMOUNT-1) - ((i*10) + 9)] <= (fbdat != 2'b00);
		end

	if (fbrdaddress == ((112*160) + 120))
			boardString[7] <= (fbdat != 2'b00);
	if (fbrdaddress == ((112*160) + 128))
			boardString[6] <= (fbdat != 2'b00);
	if (fbrdaddress == ((112*160) + 136))
			boardString[5] <= (fbdat != 2'b00);
	if (fbrdaddress == ((112*160) + 144))
			boardString[4] <= (fbdat != 2'b00);
			
	if (fbrdaddress == ((120*160) + 120))
			boardString[3] <= (fbdat != 2'b00);
	if (fbrdaddress == ((120*160) + 128))
			boardString[2] <= (fbdat != 2'b00);
	if (fbrdaddress == ((120*160) + 136))
			boardString[1] <= (fbdat != 2'b00);
	if (fbrdaddress == ((120*160) + 144))
			boardString[0] <= (fbdat != 2'b00);


end
    
endmodule
