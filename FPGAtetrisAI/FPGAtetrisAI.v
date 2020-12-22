module FPGAtetrisAI
(
	input  clock, pclk, //50MHz, 4MHz
	
	input  [1:0]switch,
	input  [7:0] piano,
	output wire [11:0] leds,

	input  DMG_vsync, //60Hz
	input  DMG_hsync, //8KHz
	input  DMG_data0,
	input  DMG_data1,
	
	output VGA_r, VGA_g, VGA_b,
	output VGA_hs, VGA_vs,
	
	output tx

);

assign leds = {4'b1111, piano[7:0]};


// VGA 25MHz clock
wire VGA_clk;

PLL pll(
.inclk0(clock),
.c0(VGA_clk)
);


wire [187:0] boardString;

wire [14:0] fb_vga_addr;
wire [1:0] fb_vga_q;

wire [14:0] fb_gb_addr;
wire [1:0] fb_gb_data;
wire fb_gb_we;

//GB display reader
GBreader gbreader (
.clk(pclk),

.addr(fb_gb_addr),
.data(fb_gb_data),
.we(fb_gb_we),

.hs(DMG_hsync),
.vs(DMG_vsync),
.d0(DMG_data0),
.d1(DMG_data1)
);


//VGA debug
VGA DebugDisplay (
.clk(VGA_clk), //25MHz
.VGA_r(VGA_r),
.VGA_g(VGA_g),
.VGA_b(VGA_b),
.VGA_hs(VGA_hs),
.VGA_vs(VGA_vs),
.fb_addr(fb_vga_addr),
.fb_q(fb_vga_q),
.boardString(boardString)
);

//Framebuffer
FB framebufferDebug (
//WRITE
.wrclock(~pclk), // we want to write on negedge clk
.wraddress(fb_gb_addr),
.wren(fb_gb_we),
.data(fb_gb_data),

//READ
.rdclock(VGA_clk),
.rdaddress(fb_vga_addr),
.q(fb_vga_q)
);




//Pixel analyzer
analyzer Analyzer (
.clk(pclk),
.fbdat(fb_gb_data),
.fbrdaddress(fb_gb_addr),
.boardString(boardString)
);

reg [2:0] sendFrame;

always @(posedge VGA_clk)
begin
	if (piano[0])
		sendFrame <= 3'd0;
	else
	begin
		sendFrame[0] <= (fb_gb_addr == 23000);
		sendFrame[1] <= sendFrame[0];
		sendFrame[2] <= sendFrame[1];
	end
end

UARTtx uarttx(
.i_Clock(VGA_clk), //25MHz
.i_Tx_DV(sendFrame[2]),
.i_Tx_Byte(boardString),
.o_Tx_Active(),
.o_Tx_Serial(tx),
.o_Tx_Done()
);

endmodule
