/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Begin part1.s for Lab 7
void clear_screen();
void draw_line(int x0, int x1, int y0, int y1, short int line_color);
void plot_pixel(int x, int y, short int line_color);
void draw_poly(int num_vertices, int *vertices, short int color);
void wait_for_vsync();
void write_char(int x, int y, char c);
volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();

    draw_line(0, 200, 319, 200, RED);// line of base platform
    for(int x = 0; x < 320; x++){
      for(int y = 200; y < 240; y++){
        plot_pixel(x, y, RED);
      }
    }
    // int num_vertices = 3;
    // int vertices[6] = {100, 150, 150, 190, 200, 100};
    // draw_poly(num_vertices, vertices, MAGENTA);


   char* hw = "Hello, world!";
   int x_char = 15;
   while (*hw) {
     write_char(x_char, 10, *hw);
  	 x++;
  	 hw++;
   }
}

// code not shown for clear_screen() and draw_line() subroutines
void clear_screen(){
  for(int x = 0; x < 320; x++){
    for(int y = 0; y < 240; y++){
      plot_pixel(x, y, 0x0);
    }
  }
}


void draw_line(int x0, int y0, int x1, int y1, short int line_color){
  bool is_steep = ABS(y1 - y0) > ABS(x1 - x0);
  int temp = 0;
  int y_step = 0;
  if (is_steep){
    //swap x0 and y0
    temp = x0;
    x0 = y0;
    y0 = temp;
    //swap x1 and y1
    temp = x1;
    x1 = y1;
    y1 = temp;
  }
  if(x0 > x1){
    //swap x0 and x1
    temp = x0;
    x0 = x1;
    x1 = temp;
    //swap y0 and y1
    temp = y0;
    y0 = y1;
    y1 = temp;
  }

  int deltax = x1 - x0;
  int deltay = ABS(y1 - y0);
  int error = -(deltax / 2);
  int y = y0;

  if (y0 < y1){
    y_step = 1;
  }
  else{
    y_step = -1;
  }

  for(int x = x0; x < x1; x++){
    if (is_steep){
      plot_pixel(y, x, line_color);
    }
    else{
      plot_pixel(x, y, line_color);
    }

    error = error + deltay;
    if (error > 0){
      y = y + y_step;
      error = error - deltax;
    }
  }

}

void draw_poly(int num_vertices, int *vertices, short int color){
  for(int i=0;i<num_vertices-1;i++)
    {
      draw_line(vertices[(i<<1)+0],
           vertices[(i<<1)+1],
           vertices[(i<<1)+2],
           vertices[(i<<1)+3],
           color);
    }
    draw_line(vertices[0],
         vertices[1],
         vertices[(num_vertices<<1)-2],
         vertices[(num_vertices<<1)-1],
         color);

}

void wait_for_vsync(){
  volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
  int status = 0;
  //launch the swap process
  *pixel_ctrl_ptr = 1; //set S bit to 1
  //poll the status bit in the status register
  status = *(pixel_ctrl_ptr + 3);
  while ((status & 0x01) != 0) {
    status = *(pixel_ctrl_ptr + 3);
  }
  //done so exit
}

void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0xC9000000 + (y<<7) + x);
  *character_buffer = c;
}


void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
