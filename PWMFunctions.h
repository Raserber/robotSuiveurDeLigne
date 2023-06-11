/* 
 * File:   PWMFunctions.h
 * Author: raserber
 *
 * Created on 29 mars 2023, 19:44
 */

void init_Timer2(void);

void init_CCP1(void);

void init_CCP2(void);

void moteurGauche(short int enable, short int duty_cycle, short int sens);

void moteurDroite(short int enable, short int duty_cycle, short int sens);
