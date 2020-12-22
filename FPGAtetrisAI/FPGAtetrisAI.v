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
	output VGA_hs, VGA_vs

);

assign leds = {4'b1111, piano[7:0]};


// VGA 25MHz clock
wire VGA_clk;

PLL pll(
.inclk0(clock),
.c0(VGA_clk)
);


wire [14:0] fb_vga_addr;
wire [1:0] fb_vga_q;

wire [14:0] fb_gb_addr;
wire [1:0] fb_gb_data;
wire fb_gb_we;


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

wire [187:0] boardString;


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





analyzer Analyzer (
.clk(pclk),
.fbdat(fb_gb_data),
.fbrdaddress(fb_gb_addr),
.boardString(boardString)
);

endmodule
