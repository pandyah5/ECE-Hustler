volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
int PS2_data, RVALID;
unsigned char keyPress;

int main(){
	*(PS2_ptr + 1) = 0x1;
	while(1) {
	  PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
	  keyPress = PS2_data & 0xFF;
	  RVALID = PS2_data & 0x8000;
	  if(RVALID){
		  if (keyPress == 'u') { // if right is pressed
			  printf("Jump ");
			  *(PS2_ptr + 1) = 0x1;
		  }
		  else if (keyPress == 'r') { // if left is pressed
			  printf("Duck ");
			  *(PS2_ptr + 1) = 0x1;
		  }
	  }
	}
}
