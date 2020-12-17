/**
 * \file potok_2.c
 * \author Górka Mateusz
 * \brief Komunikacja procesów poprzez potok systemowy.
 *
 * Proces rodzica programu przesyła zawartość pliku zdjecia
 * poprzez potok oraz wyświetla go w procesie dziecka.
 *
 * @copyright Copyright (c) 2020
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	pid_t 	pid_procesu;		//< PID procesu
	int		potok[2];			//< Tablica id potoków
	int 	plik;				//< Plik
	int 	n;					//< Wielkosc pobranych danych
	char 	bufor[256];			//< Bufor
	char	nazwa_pliku[64];	//< Nazwa pliku

	/// Tworzymy potok
	if( pipe(potok) < 0 ){
		fprintf(stderr, "Blad otwarcia potoku.\n");
		return 1;
	}

	/// Tworzymy proces potomny
	if( (pid_procesu = fork()) < 0 ){
		fprintf(stderr, "Blad otwarcia watku.\n");
		return 4;
	}

	if( pid_procesu == 0 ){
		/// Proces dziecka
		close( potok[1] );
		close( 0 );				// Zamykamy stdin
		dup( potok[0] );		// Przenosimy potok[0] na stdin
		close( potok[0] );

		execlp("display", "display", "-", NULL);
	}
	else {
		/// Proces rodzica
		close( potok[0] );
		printf("Wprowadz scierzke do obrazka\n: ");
		scanf("%s", nazwa_pliku);

		if( (plik = open(nazwa_pliku, O_RDONLY) ) < 0 ){
			fprintf(stderr, "Blad otwierania pliku");
			return 2;
		}

		while( (n = read(plik, &bufor, 256) ) > 0 ){

			if( write(potok[1], &bufor, n) < 0 ){
				fprintf(stderr, "Blad wpisywania do potoku\n");
				return 3;
			}
		}

		close( plik );
	}
	return 0;
}