/**
 * \file obsluga_sygnalow.c
 * \author Górka Mateusz
 * \brief  Obsługa sygnałów systemu Linux
 *
 * Program w nieskończoność inkrementuje zmienną `i`,
 * co jest tylko sposobem na pokazanie, jak przechytywać
 * sygnały w systemie Linux.
 *
 * Program można uruchomić w tle, a następnie za pomocą
 * polecenia `kill` wysyłać do niego odpowiednie sygnały.
 * SIGALRM - spowoduje wyswietlenie komunikatu i wyjscie z programu;
 * SIGTERM - jedynie wyswietlenie komunikatu;
 * SIGUSR1 - zatrzymanie obslugi przerwan na okreslony czas;
 * SIGUSR2 - zostanie zignorowane;
 *
 * @copyright Copyright (c) 2020
 * 
 */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int STOP_TM = 0;	//< Global, Stop signal handeler time

/**
 * \brief Obsługuje przechycone sygnały
 * \param signal_code - kod sygnału
 */
void handler( int signal_code ){
	if( STOP_TM == 0 )
		switch( signal_code ){
		case SIGALRM:
			printf("Otrzymano SIGALRM\n");
			exit(0);
			break;
		case SIGTERM:
			printf("Otrzymano SIGTERM\n");
			break;
		case SIGUSR1:
			STOP_TM = 10000;
			break;
		case SIGUSR2: break;
		}
}

int main(){
	signal( SIGALRM, handler );
	signal( SIGTERM, handler );
	signal( SIGUSR1, handler );
	signal( SIGUSR2, handler );

	long long i=0;

	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 100000;

	for(;;){
		++i;
		if( STOP_TM ) --STOP_TM;
		nanosleep( &tim, &tim2 );
	}
}