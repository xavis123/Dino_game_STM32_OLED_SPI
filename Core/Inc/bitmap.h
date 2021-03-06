#ifndef BITMAP_H_
#define BITMAP_H_


static const uint8_t bmp_tile[] = {
		0x38,0x44,0x82,0x29,0x11,0x29,0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,
		0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,0x82,0x44,0x38,0x44,0x82,0x29,
		0x11,0x29,0x82,0x44,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
		0x00,0x01,0x01,0x01,0x00,0x00
};

static const uint8_t cactus []  = {
0xC0, 0x00, 0xF0, 0xF0, 0x00, 0x00, 0x03, 0x02, 0xFF, 0xFF, 0x08, 0x0F, 
};

static const uint8_t cactus_dead3x7[]  ={
		0x00, 0x7c, 0x7c
	};

static const uint8_t googledino20x20_dead []  = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xfe, 0xf2, 0xf2, 0x7e, 0x3e,
		0x3e, 0x3e, 0x1e, 0x1c, 0x07, 0x1f, 0x3e, 0x7c, 0xfc, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff,
		0x3f, 0x1f, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0b, 0x09,
		0x00, 0x01, 0x0f, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

static const uint8_t bird_down_14x12 [] = {
		0x08, 0x0c, 0x0e, 0x1f, 0xfe, 0xfc, 0xfc, 0xfc, 0x78, 0x78, 0x38, 0x38, 0x28, 0x28, 0x00, 0x00,
					0x00, 0x00, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t bird_up_14x12 [] = {
		0x00, 0x80, 0xc0, 0xe0, 0xc0, 0x87, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x01, 0x01,
			0x01, 0x03, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x05, 0x05
};

static const uint8_t sword13x13_up []  = {
		0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0xa0, 0x50, 0x28, 0x14, 0x0a, 0x05, 0x03, 0x10, 0x09, 0x07,
		0x05, 0x0e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

static const uint8_t sword19x7_down []  ={
	0x08, 0x08, 0x08, 0x55, 0x2a, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x1c, 0x08
};



static const uint8_t googledino20x20_right [] =  {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xfe, 0xfa, 0xfe, 0x7e, 0x5e,
		0x5e, 0x5e, 0x1e, 0x1c, 0x07, 0x1f, 0x3e, 0x7c, 0xfc, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff,
		0x3f, 0x1f, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0b, 0x09,
		0x00, 0x01, 0x0f, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

static const uint8_t googledino20x20_left [] ={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xfe, 0xfa, 0xfe, 0x7e, 0x5e,
	0x5e, 0x5e, 0x1e, 0x1c, 0x07, 0x1f, 0x3e, 0x7c, 0xfc, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0x7f,
	0x3f, 0x1f, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x05, 0x01,
	0x00, 0x00, 0x0f, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t googledino16x16_clear [] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};


static const uint8_t ground [] = {
0x40, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x20,
0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10,
0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x42, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x04, 0x00, 0x20, 0x20, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04,
0x24, 0x04, 0x00, 0x00,0x40, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x20,
0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10,
0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x42, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x04, 0x00, 0x20, 0x20, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04,
0x24, 0x04, 0x00, 0x00
};



#endif 
