/**
 * \file potok_nazwany.c
 * \author Gorka Mateusz
 * \brief Komunikacja procesów przez potok nazwany
 *
 * Program przesyła pliki tekstowe podane w argumentach wywołania
 * poprzez potok nazwany, zwany kolejką,
 * który możemy odczytać uruchamiając w drugim terminalu
 * polecenie:
 * ```
 *	cat /tmp/kolejka
 * ```
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main( int argc, char **argv ){

	int  i = 1;				//< Licznik argumentow
	char *nazwa_kolejki = "/tmp/kolejka";	//< Nazwa kolejki
	int  plik, kolejka;		//< Wskazniki plikow
	int  n;					//< Wielkosc buforu
	char bufor[256];		//< Bufor

	if( argc == 1 ){
		fprintf(stderr, "Oczekiwano nazw pliku przy wywolaniu progrmau\n");
		return 5;
	}

	/// Tworzenie potoku nazwanego
	if( mkfifo(nazwa_kolejki, 0666) < 0 ){
		fprintf(stderr, "Blad stworzenia kolejki, prawdopodobnie kolejka juz istnieje\n");
		// return 1;
	}

	/// Otwiernie potoku nazwanego
	if( (kolejka = open(nazwa_kolejki, O_WRONLY)) < 1 ){
		fprintf(stderr, "Blat otwarcia kolejki\n");
		return 2;
	}

	while( i < argc ){

		/// Czytanie pliku
		if( (plik = open(argv[i], O_RDONLY)) < 0 ){
			fprintf(stderr, "Blad otwierania pliku %s\n", argv[i]);
			return 3;
		}

		/// Zapis pliku do potoku nazwanego
		while( (n = read(plik, &bufor, 256)) > 0 )
			if( write(kolejka, bufor, n) < 0 ){
				fprintf(stderr, "Blad zapisu do potok\n");
				return 4;
			}

		++i;
		sleep( 2 );
	}

	close( kolejka );
	return 0;
}