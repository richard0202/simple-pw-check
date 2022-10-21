#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void output(char string[102]) {     // funkce pro zaslani vysledneho hesla
    printf("%s", string);
}

bool isInArray(char x, char charArray[129], int nchars) {   // funkce pro kontrolu zda je dany znak
    int i = 0;                                              // obsazen v danem poli - (pro funkcnost
    if (nchars > 0) {                                       // zapocitavani ruznych znaku)
        while (charArray[i] != '\0') {
            if (x == charArray[i]) {
                return true;
            }
            i++;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {
    if (argc > 4) {                                 // Kontrola zda je obsazen spravny pocet argumentu
        fprintf(stderr, "Moc argumentu!\n");
        return 1;
    }

    if (argc < 3) {                                 // Kontrola zda je obsazen spravny pocet argumentu
        fprintf(stderr, "Malo argumentu!\n");
        return 1;
    }

    for (int i = 0; argv[1][i] != '\0' ; ++i) {     // Kontrola zda argument level obsahuje pouze cisla
        if ((argv[1][i] >= 0 && argv[1][i] <= 47) || (argv[1][i] >= 58 && argv[1][i] <= 126)) {
            fprintf(stderr, "Level argument obsahuje pismena!\n");
            return 1;
        }
    }

    for (int i = 0; argv[2][i] != '\0' ; ++i) {     // Kontrola zda argument parametr obsahuje pouze cisla
        if ((argv[2][i] >= 0 && argv[2][i] <= 47) || (argv[2][i] >= 58 && argv[2][i] <= 126)) {
            fprintf(stderr, "Parametr obsahuje pismena!\n");
            return 1;
        }
    }

    int level = strtol(argv[1], argv, 0);       // Prevedeni vstupu argumentu level na int
    long int param = strtol(argv[2], argv, 0);  // Prevedeni vstupu argumentu parametr na int

    if (level < 1 || level > 4) {                     // Kontrola zda je level se spravnem intervalu
        fprintf(stderr, "Level musi byt v intervalu od 1 do 4!\n");
        return 1;
    }
    if (param < 1 ) {                                 // Kontrola zda je parametr ve spravnem intervalu
        fprintf(stderr, "Parametr musi byt vetsi nez 1!\n");
        return 1;
    }

    if (argc == 4) {                           // Pokud se program spousti se 4 argumeny, tak kontrola zda
        char *vstupStats = argv[3];            //    je posledni parametr ve formatu --stats
        char stats[] = "--stats";
        for (int j = 0; vstupStats[j] != '\0' || stats[j] != '\0'; j++) {
            if (vstupStats[j] != stats[j]) {
                fprintf(stderr, "Zadan spatny parametr --stats!\n");
                return 1;
            }
        }
    }

    char buffer[102];
    int minLength = 100;
    int charCount = 0;
    int passwordCount = 0;
    int nchars = 0;
    char ncharsArray[129];

    while (fgets(buffer, 102, stdin) != NULL) {
        passwordCount++;

        bool haveUppercase = false;
        bool haveLowercase = false;
        bool haveNumber = false;
        bool haveSpecialCharacters = false;

        bool isLevel1 = false;
        bool isLevel2 = false;
        bool isLevel3 = false;

        bool isSame = false;
        int level2Condition = 0;
        int level3Condition = 1;
        int level4Condition = 0;
        int shodnost = 0;
        int znak2 = 0;
        int znak1 = 0;

        int i = 0;
        if (buffer[0] == '\n') {
            minLength = 1;
        }

        while (buffer[i] != '\n') {

            if(buffer[i+1] != '\n' && i > 100) {       // Kontrola zda je heslo delsi nez 100 znaku
                fprintf(stderr, "Ve vstupu je heslo delsi nez 100 znaku!\n");
                return 1;
            }

            if (haveUppercase == false) {       // zjisteni zda heslo obsahuje velke pismeno:
                if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                    haveUppercase = true;
                    level2Condition++;           // pro level 2 - zaznamenani, ze je tato skupina splnena
                }
            }

            if (haveLowercase == false) {        // zjisteni zda heslo obsahuje male pismeno:
                if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                    haveLowercase = true;
                    level2Condition++;           // pro level 2 - zaznamenani, ze je tato skupina splnena
                }
            }

            if (level >= 2) {
                if (haveNumber == false) {       // zjisteni zda heslo obsahuje cislo:
                    if (buffer[i] >= '0' && buffer[i] <= '9') {
                        haveNumber = true;
                        level2Condition++;       // pro level 2 - zaznamenani, ze je tato skupina splnena
                    }
                }

                if (haveSpecialCharacters == false) {   // zjisteni zda heslo obsahuje specialni charakter:
                    if ((buffer[i] >= 32 && buffer[i] <= 47) || (buffer[i] >= 58 && buffer[i] <= 64)
                        || (buffer[i] >= 91 && buffer[i] <= 96) || (buffer[i] >= 123 && buffer[i] <= 126)) {
                        haveSpecialCharacters = true;
                        level2Condition++;              // pro level 2 - zaznamenani, ze je tato skupina splnena
                    }
                }
            }

            if (level >= 3) {
                if (buffer[i] == buffer[i + 1]) {       // zjisteni, zda heslo ma stejnou sekvenci znaku dane delky:
                    level3Condition++;                  // zaznamenani poctu stejnych znaku
                } else if (level3Condition < param) {
                    level3Condition = 1;                // resetovani pri preruseni sekvence znaku
                }
            }

            if (level >= 4) {
                shodnost = 0;
                znak2 = 0;
                isSame = false;
                for (int j = i + 1; buffer[znak1] != '\n' &&
                    buffer[j] != '\n'; j++) {   // zjistovani zda heslo obsahuje 2 stejne podretezce delky parametru
                    if (buffer[znak1] == buffer[j]) {
                        if (shodnost < param) {
                            if (shodnost == 0) {
                                znak2 = j;
                            }
                            shodnost++;
                            znak1++;
                            isSame = true;
                            if (shodnost == param) {
                                level4Condition++;
                            }
                        }
                    } else {
                        znak1 = i;
                        shodnost = 0;
                        if (isSame) {
                            j = znak2;
                            isSame = false;
                        }
                    }
                }

            }
            charCount++;

            if (!isInArray(buffer[i], ncharsArray, nchars)) {   // zaznamenavani poctu ruznych znaku:
                ncharsArray[nchars] = buffer[i];
                nchars++;
            }

            i++;
            znak1 = i;

            if ((buffer[i + 1] == '\n' && minLength > i)) {     // zaznamenavani minimalni delky:
                minLength = i + 1;
            }

        }

        if (passwordCount == 1) {       // zaznamenani zakladni minimalni delky hesla pri prvnim hesle
            minLength = charCount;
        }

        if (level >= 1) {                           // zaznamenani, ze je splneny level 1
            if (haveUppercase && haveLowercase) {
                isLevel1 = true;
            }
        }
        if (level >= 2 && isLevel1 && (level2Condition >= param || level2Condition == 4)) {   // zaznamenani, ze je splneny level 2
            isLevel2 = true;
        }

        if (level >= 3 && isLevel2 && level3Condition < param) {    // zaznamenani, ze je splneny level 3
            isLevel3 = true;
        }

        // Zaslani hesla, ktere proslo dle pozadavku:
        if (level == 1 && isLevel1) {
            output(buffer);
        }

        if (level == 2 && isLevel2) {
            output(buffer);
        }

        if (level == 3 && isLevel3) {
            output(buffer);
        }

        if (level == 4 && level4Condition == 0 && isLevel3) {
            output(buffer);
        }
    }

    if (passwordCount == 0) {       // Kontrola zda je zadan nejaky vstup
        fprintf(stderr, "Zadny vstup!\n");
        return 1;
    }

    if (argc == 4) {                // Zasilani statistik:
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", nchars);
        printf("Minimalni delka: %d\n", minLength);
        printf("Prumerna delka: %.1f\n", (double) charCount / (double) passwordCount);
    }

    return 0;
}

