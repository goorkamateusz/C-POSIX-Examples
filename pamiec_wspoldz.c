/**
 * \file pamiec_wspoldz.c
 * \author Gorka Mateusz
 * \brief Komunikacja przez pamięć współdzieloną
 *
 * Program przesyła pomiędzy wątkami obrazy za pomocą pamięci wspólnej.
 * Wyświetla je przy użyciu programu display.
 *
 * @copyright Copyright (c) 2020
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#define FILE_NAME_SIZE 64       //< Maksymalna dlugosc scierzki do pliku
#define BUFF_SIZE 256           //< Wielkosc buforu

int main(){

    char file_name[FILE_NAME_SIZE];
    int file, file_mem;
    struct stat st;
    char *file_memory;

    int pid = fork();

    /// Tworzy pamiec wpoldzielona
    if( (file_mem = open("pamiec_wspoldz", O_RDWR | O_CREAT, 0666)) == -1 ){
        fprintf(stderr, "Blad otwarcia pamieci wspoldz\n");
        return 2;
    }

    if( pid == 0 ){     // Proces dzecka
        sleep(3);
        /// Aktualizuje obrazem
        execlp("display", "", "-update", "1", "-delay", "2", "pamiec_wspoldz", NULL);
    }
    else
    if( pid == -1 ){    // Blad otwarcia procesu
        fprintf(stderr,"Blad otwarcia procesu\n");
        return 1;
    }
    else {              // Proces rodzica
        while(1){

            /// Wypytanie uzytkownika o nazwe pliku
            printf("Wprowadz nazwe pliku ze zdjeciem:\n");
            scanf("%s", file_name);

            /// Otwiera plik z dysku
            if( (file = open(file_name, O_RDONLY, 0666)) == -1 ){
                fprintf(stderr,"Blad otwarcja pliku %s\n", file_name);
                break;
            }

            /// Czyta dane o pliku
            if( fstat(file, &st) < 0 ){
                fprintf(stderr,"Blad czytania danych o pliku %s\n", file_name);
                break;
            }

            /// Przycinanie pamieci wspoldz do okreslonego wymiaru
            if( truncate("pamiec_wspoldz", st.st_size ) == -1 ){
                fprintf(stderr,"Blad truncate\n");
                break;
            }

            /// Mapowanie pliku do pamieci
            file_memory = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_mem, 0);

            if( file_memory == MAP_FAILED ){
                fprintf(stderr,"Blad mapowania do pamieci");
                break;
            }

            /// Wczytuje plik do pamieci wspoldzielonej
            read(file, file_memory, st.st_size);
            /// Zaktualizuje zmiany w pamieci wymiany
            msync(file_memory, st.st_size, MS_SYNC);
            /// Zwalnia "odmapowuje" zajętą pamięć
            munmap(file_memory, st.st_size);

            close(file);
        }

        /// Zabija proces dziecko
        kill( pid, SIGTERM );
    }

    close(file_mem);

    /// Usuwa segment wsolny
    if( unlink( "pamiec_wspoldz" ) < 0 ){
        fprintf(stderr,"Blad ustuwania pamiedzi wspol\n");
        return 3;
    }

    return 0;
}