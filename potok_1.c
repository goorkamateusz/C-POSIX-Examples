/**
 * \file potok_1.c
 * \author Górka Mateusz
 * \brief Komunikacja procesów poprzez potok systemowy
 *
 * Proces rodzica programu przesyła zawartość pliku tekstowego
 * poprzez potok do procesu dziecka, które odczytuje go
 * i wyświetla kolejno odczytane kawałki buforu pomiędzy znakami "#".
 *
 * @copyright Copyright (c) 2020
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	pid_t 	pid_procesu;		//< PID procesu
	int		potok[2];			//< Tablica wskaznikow na potok
	int 	plik;				//< Wskaznik na plik
	int 	n;					//< Wielkosc pobranych danych
	char 	bufor[256];			//< Bufor
	char	nazwa_pliku[64];	//< Nazwa pliku

	/// Tworzymy potok
	if( pipe(potok) < 0 ){
		fprintf(stderr, "Blad otwarcia potoku.\n");
		return 1;
	}

	/// Tworzymy proces dziecka
	if( (pid_procesu = fork()) < 0 ){
		fprintf(stderr, "Blad otwarcia watku.\n");
		return 2;
	}

	if( pid_procesu == 0 ){
		/// tutaj wykonuje sie proces dziecka
		close( potok[1] );

		n = 256;
		while( (n = read(potok[0], &bufor, n)) > 0 )
			printf("#%s#\n", bufor);

		close( potok[0] );
	}
	else {
		/// tutaj wykonuje sie proces rodzica
		close( potok[0] );
		printf("Wprowadz sciezke do pliku\n: ");
		scanf("%s", nazwa_pliku);

		/// Otwieramy plik
		if( (plik = open(nazwa_pliku, O_RDONLY)) < 0 ){
			fprintf(stderr, "Blad otwierania pliku");
			return 3;
		}

		/// Czytamy plik i wpisujemy do potoku
		n = 256;
		while( (n = read(plik, &bufor, n)) > 0 ){

			if( write(potok[1], &bufor, n) < 0 ){
				fprintf(stderr, "Blad wpisywania do potoku\n");
				return 4;
			}
		}

		close( plik );
		close( potok[1] );
	}
	return 0;
}