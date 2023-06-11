#include <xc.h>

#define SENS_MD LATC0
#define PWM_MD LATC1
#define EN_MD LATC2

#define SENS_MG LATC7
#define PWM_MG LATC6
#define EN_MG LATC3


// Timer necessaire pour la fonction PWM Module des CCP1 et CCP2
void init_Timer2(void) {

    T2CLKCON = 0x01;  //choix de l'horloge source pour le Timer2, =Fosc/4 soit 8Mhz
    T2CON = 0x80; // 0b10000000 / 1: active | prescaler de 1 | postcaler de 1
    T2PR = 100; // frequence de 20kHz car (1/20e3)/(1*(1/8e6)*4*1) = 100
}
 

// Configuration PWM moteur Gauche
void init_CCP1(void) {


    //configuration du module CCP1 en PWM 
    CCP1CON =  0x9c; // 0b10011100 | valeurs des bits : (1 => ON) / X / X / 1 / 1100 => PWM
    CCPR1H = 50; // nombre impulsion etat haut, soit duty cycle de 50%
    RC6PPS = 0x09; // assigne le CCP1 a la patte C6
}


// Configuration PWM moteur Droite
void init_CCP2(void) {

    //configuration du module CCP2 en PWM 
    CCP2CON =  0x9C; // 0b10011100 | valeurs des bits : (1 => ON) / X / X / 1 / 1100 => PWM
    CCPR2H = 50; // nombre impulsion etat haut, soit duty cycle de 50%
    RC1PPS = 0x0A; // assigne le CCP2 a la patte C1
}


// Fonction pilotant le moteur Gauche
void moteurGauche(short int enable, short int duty_cycle, short int sens) {

    EN_MG = enable;
    SENS_MG = sens;
    CCPR1H = duty_cycle;
}

// Fonction pilotant le moteur Droit
void moteurDroite(short int enable, short int duty_cycle, short int sens) {

    EN_MD = enable;
    SENS_MD = sens;
    CCPR2H = duty_cycle;
}
