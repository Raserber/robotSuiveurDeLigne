#include "configuration_bits.h"
#include "PWMFunctions.h"
#include "actionsFunctions.h"

void init_Timer0(void);

#define capteur_G RB7
#define capteur_M RB5
#define capteur_D RB6


int main(void) {

	// permet de configurer les E\S via RxxPPS
	PPSLOCK = 0x00;

	// configuration de selection des timers pour les CCP1, 2
	CCPTMRS0 = 0x05; // 0b00000101


	// E\S TOR
	ANSELA = 0x00;
	ANSELB = 0x00;
	ANSELC = 0x00;

	// Par defaut les E\S non utilises sont parametres en entrees
	TRISA = 0x10; //     X     |     X     |     X     |  Bouton   |     X     |     X     |     X     | *lanceur  |
	TRISB = 0xE0; // capteur_G | capteur_D | capteur_M |  *Pivot   |     X     |     X     |     X     |     X     |
	TRISC = 0x00; // SENS_MG   |  PWM_MG   |     X     |     X     |   EN_MG   |   EN_MD   |   PWM_MD  |  SENS_MD  |


	PORTA = 0x00; // mise a GND de
	PORTB = 0x00; // toutes
	PORTC = 0x00; // les sorties


	// Weak Pull Up : pull up actives
	WPUB7 = 1; // capteur_G
	WPUB5 = 1; // capteur_M
	WPUB6 = 1; // capteur_D


	init_Timer2(); // Timer utilise pour CCP1 ET CCP2
    

	init_CCP1(); // PWM moteur gauche
	init_CCP2(); // PWM moteur droit
    
        
	short int capteur_G_old, capteur_D_old;

	// compteur de ligne d'arret avec detecteur ligne
	// evitant de compter plusieurs fois la meme ligne
	short int compteur, sur_pointArret, actionsActivees;

	compteur = 0;
	sur_pointArret = 0;

	// Si true, configuration qualifications, si false, configuration phase finale
	actionsActivees = 0;

	// ne demarre pas tant que pas appuyer (et relacher) sur RA4
	while(!RA4);
	while(RA4);

  
	while(1) {
	
	// arret si appuyer sur RA4 ou que le compteur de ligne a atteint 5
	// soit la derniere ligne
	if (RA4 || compteur == 5) {
	
		// arret du moteur
	  moteurGauche(0, 0, 0);
	  moteurDroite(0, 0, 0);

		// remise a zero du compteur
	  compteur = 0;
	  
		// ne redemarre pas tant que pas appuyer (et relacher) sur RA4
	  while(RA4);
	  while(!RA4);
	  while(RA4);
	}
	
        
        // comptage de ligne d'arret
        if (capteur_G && capteur_M && capteur_D && !sur_pointArret) {
            
            compteur++;
            

						// Actions a realiser
						// Ligne numero 1 : lancee de balle
						// Ligne numero 4 : tourner servomoteur pour bascule pivot
						if (actionsActivees) {
							switch (compteur) {

								case 1 :

									moteurGauche(1, 50, 1);
									moteurDroite(1, 50, 1);
									
									__delay_ms(100);
									
									moteurGauche(1, 0, 0);
									moteurDroite(1, 0, 0);
									
									lancerBalle();

									__delay_ms(300);
									
									moteurGauche(1, 50, 0);
									moteurDroite(1, 50, 0);
									
									__delay_ms(100);

									break;

								case 4 :

									moteurGauche(1, 50, 1);
									moteurDroite(1, 50, 1);
									
									__delay_ms(100);
									
									moteurGauche(1, 0, 0);
									moteurDroite(1, 0, 0);
									
									basculePivot();
									break;
							}
						}

						// redemarrage tout droit (evitement de conflit
						// ligne horizontale et verticale)
						moteurGauche(1, 50, 0);
						moteurDroite(1, 50, 0);
							
						__delay_ms(100);

            sur_pointArret = 1;
        }
        
        // si ligne d'arret quittee
        else if (!capteur_G || !capteur_M || !capteur_D) {
            
            sur_pointArret = 0;
        }
        
        // si gros ecart G
        if (capteur_G && capteur_M) {
            
            moteurGauche(1, 10, 0);
            moteurDroite(1, 100, 0);
        }
        
        // si gros ecart D
        else if (capteur_M && capteur_D) {
            
            moteurGauche(1, 100, 0);
            moteurDroite(1, 10, 0);
        }
        
        else if (capteur_G) {
            
            moteurGauche(1, 40, 0);
            moteurDroite(1, 100, 0);
        }
        
        else if (capteur_M) {
            
            moteurGauche(1, 100, 0);
            moteurDroite(1, 100, 0);
        }
        
        else if (capteur_D) {
            
            moteurGauche(1, 100, 0);
            moteurDroite(1, 30, 0);
        }
        
        // si ligne totalement quittee et valeur prec non nulle
        else if (capteur_G_old || capteur_D_old) {
            
            if (capteur_G_old) {
                
                moteurGauche(1, 35, 0);
                moteurDroite(1, 100, 0);
            }
            
            else if (capteur_D_old) {
                
                moteurGauche(1, 100, 0);
                moteurDroite(1, 30, 0);
            }
        }

        
        // ..._old -> valeur actuelle capt OU valeur ancienne
        // si pas de detection de l'autre cote
        capteur_G_old = capteur_G || (capteur_G_old && !capteur_D);
        capteur_D_old = capteur_D || (capteur_D_old && !capteur_G);
	}
}
