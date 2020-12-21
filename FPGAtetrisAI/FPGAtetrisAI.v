module FPGAtetrisAI
(
	input  clock, pclk, //50MHz, 4MHz
	
	input  [1:0]switch,
	input  [7:0] piano,
	output wire [11:0] leds,

	input  DMG_vsync, //60Hz
	input  DMG_hsync, //9KHz
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


//VGA debug
VGA DebugDisplay (
.clk(VGA_clk), //25MHz
.VGA_r(VGA_r),
.VGA_g(VGA_g),
.VGA_b(VGA_b),
.VGA_hs(VGA_hs),
.VGA_vs(VGA_vs),
.fb_addr(fb_vga_addr),
.fb_q(fb_vga_q)
);


//GB ROM (to test vga debug before dual port vram)
ROM gbROM (
.clk(VGA_clk), //25MHz
.address(fb_vga_addr),
.q(fb_vga_q)
);


endmodule