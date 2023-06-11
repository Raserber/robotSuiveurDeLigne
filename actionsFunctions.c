#include "configuration_bits.h"
#include <xc.h>

// active le lanceur de balle
void lancerBalle(void) {
 
	LATA1 = 1;

	__delay_ms(150);

	LATA1 = 0;
}

// fait tourner le servomoteur en sens horaire
void basculePivot(void) {

	for (short int i = 0; i<=30; i++) {
		LATB4 = 1;

		__delay_ms(1);

		LATB4 = 0;

		__delay_ms(19);
	}
}
