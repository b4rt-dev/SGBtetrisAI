module VGA(
    input  clk,
    
    output VGA_r,
    output VGA_g,
    output VGA_b,
    output VGA_hs,
    output VGA_vs,
	 
	 output reg [14:0] fb_addr,
	 input [1:0] fb_q
);

//assign frameDrawn = o_frame;

parameter
//VGA 480pp
    H_RES   = 640,      // horizontal resolution (pixels)
    V_RES   = 480,      // vertical resolution (lines)
    H_FP    = 16,       // horizontal front porch
    H_SYNC  = 96,       // horizontal sync
    H_BP    = 48,       // horizontal back porch
    V_FP    = 10,       // vertical front porch
    V_SYNC  = 2,        // vertical sync
    V_BP    = 33,       // vertical back porch
    H_POL   = 0,        // horizontal sync polarity (0:neg, 1:pos)
    V_POL   = 0;        // vertical sync polarity
	 
    
// Horizontal: sync, active, and pixels
localparam HS_STA = H_FP - 1;           // sync start (first pixel is 0)
localparam HS_END = HS_STA + H_SYNC;    // sync end
localparam HA_STA = HS_END + H_BP;      // active start
localparam HA_END = HA_STA + H_RES;     // active end 
localparam LINE   = HA_END;             // line pixels 

// Vertical: sync, active, and pixels
localparam VS_STA = V_FP - 1;           // sync start (first line is 0)
localparam VS_END = VS_STA + V_SYNC;    // sync end
localparam VA_STA = VS_END + V_BP;      // active start
localparam VA_END = VA_STA + V_RES;     // active end 
localparam FRAME  = VA_END;             // frame lines 

reg [9:0] h_count;  // line position in pixels including blanking 
reg [8:0] v_count;  // frame position in lines including blanking 

wire o_hs, o_vs, o_de, o_h, o_v, o_frame;
//assign crt_sync = !(o_hs^o_vs);
assign VGA_hs = o_hs;
assign VGA_vs = o_vs;

// generate sync signals with correct polarity
assign o_hs = H_POL ? (h_count > HS_STA & h_count <= HS_END)
    : ~(h_count > HS_STA & h_count <= HS_END);
assign VGA_hs = o_hs;
assign o_vs = V_POL ? (v_count > VS_STA & v_count <= VS_END)
    : ~(v_count > VS_STA & v_count <= VS_END);
assign VGA_vs = o_vs;
    
// display enable: high during active period
assign o_de = h_count > HA_STA & h_count <= HA_END
    & v_count > VA_STA & v_count <= VA_END; 

// keep o_h and o_v bound within active pixels
assign o_h = (o_de & h_count > HA_STA & h_count <= HA_END) ? 
                h_count - (HA_STA + 1): 0;
assign o_v = (o_de & v_count > VA_STA & v_count <= VA_END) ? 
                v_count - (VA_STA + 1): 0;

// o_frame: high for some ticks when last frame is drawn
assign o_frame = (v_count == 0 & h_count < 8);


// GB frame position
localparam HBEGIN = 400;
localparam HEND = 560;

localparam VBEGIN = 200;
localparam VEND = 344;
 
always @ (posedge clk)
begin
    if (h_count == LINE)  // end of line
    begin
         h_count <= 0;
         if (v_count == FRAME) // end of frame
         begin      
              v_count <= 0;
         end
         else
              v_count <= v_count + 1;
    end
    else 
         h_count <= h_count + 1;
	
	
	if (v_count == 0)
	begin
		fb_addr <= 15'd0;
	end
	else
	begin
		if (v_count >= VBEGIN && v_count < VEND && h_count >= HBEGIN-2 && h_count < HEND-2)
		begin
			fb_addr <= fb_addr + 1'b1;
		end
	end
	
	
end

initial begin
    h_count = 12'd0;
    v_count = 12'd0;
end

reg data;

assign VGA_r = (v_count >= VBEGIN && v_count < VEND && h_count >= HBEGIN && h_count < HEND) ? fb_q[0] : 1'b0;
assign VGA_g = (v_count >= VBEGIN && v_count < VEND && h_count >= HBEGIN && h_count < HEND) ? data : o_h;
assign VGA_b = (v_count >= VBEGIN && v_count < VEND && h_count >= HBEGIN && h_count < HEND) ? fb_q[1] : 1'b0;




integer i;

always @(*)
begin

	data <= 1'b0;

	//board
	for (i = 0; i < 18; i = i + 1)
	begin
		//row i
		if (fb_addr == ((i*8*160) + 18))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 26))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 34))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 42))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 50))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 58))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 66))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 74))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 82))
			data <= 1'b1;
		if (fb_addr == ((i*8*160) + 90))
			data <= 1'b1;
	end
	
	//next piece
	if (fb_addr == ((112*160) + 122))
			data <= 1'b1;
	if (fb_addr == ((112*160) + 130))
			data <= 1'b1;
	if (fb_addr == ((112*160) + 138))
			data <= 1'b1;
	if (fb_addr == ((112*160) + 146))
			data <= 1'b1;
			
	if (fb_addr == ((120*160) + 122))
			data <= 1'b1;
	if (fb_addr == ((120*160) + 130))
			data <= 1'b1;
	if (fb_addr == ((120*160) + 138))
			data <= 1'b1;
	if (fb_addr == ((120*160) + 146))
			data <= 1'b1;
end

endmodule
