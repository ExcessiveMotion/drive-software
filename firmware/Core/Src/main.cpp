#include "main.h"

device* Device = nullptr;
#include "interrupt_catch.h"

int main(void){

	device dev = device();
	Device = &dev;

	Device->init();
	
	Device->run();

	return 1;	// should never reach this
}

// extern "C" {

// void Parity_Error_Callback(void){
// 	while(1);
// }

// void Framing_Error_Callback(void){
// 	while(1);
// }

// void Noise_Detected_Error_Callback(void){
// 	while(1);
// }

// void Overrun_Error_Callback(void){
// 	while(1);
// }


// }