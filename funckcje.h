#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include <ctype.h>
#include <TIME.H>
#include <windows.h>
#include <conio.h>

#define MAX_ROZ_SL 10 //górny zakres bazy danych
#define MIN_ROZ_SL 3 //dolny zakres bazy dancyh
#define MAX_LINE 1000 //maksymalny rozmiar pobieranej linijki tekstu
#define DLUGOSC_MENU 80 //definuje szerokosc menu
#define WYSOKOSC_MENU 25 //definuje wysokosc menu
#define EL_NA_STR 10 //ilosc elementow wyswielanych jednoczesnie na jednej stronie

typedef struct slowo{ /* Struktura przechowuj¹ca informacje o jednej pozycji w s³owniku */
	char * wyraz;
	char * opis;
	struct slowo * poprzedni, * nastepny;
}slowo;

typedef struct zbior{ /* Struktura przechowuj¹ca infomacjê zbiorze s³ów jednej d³ugoœci */
	int liczba_slow;
	slowo * glowa;
	char * nazwa_pliku;
	FILE * plik;
}zbior;

/*------------------UZYTKOWNIK.C------------------*/
int klawisz(int * klawisz, int * i, FILE * p);
void dodaj_slowo(struct zbior ** zbior);
void edytuj_opis(char * opis);
void edytuj_slowo(slowo * element, int dl_slowa);
void edytuj_wyraz(char * wyraz, int dl_slowa);
void enter();
slowo * laduj_znalezione(zbior ** tab, char * szukane, int * licznik);
slowo * laduj_znalezione_litery(zbior * blok, char * szukane, int * ile_znalezionych, int rozmiar);
void licz_znalezione(zbior ** tab, char * szukane, int * licznik);
char * male_litery(char * napis);
zbior ** obsluga(zbior ** tab);
int pobierz_tekst (char * tekst);
int pobierz_liczbe (int * liczba);
void przeglad (zbior ** tab);
void przeglad_zbioru (zbior * blok, int dl_slowa);
slowo * usun_slowo(slowo * element);
slowo * wybierz_slowo(slowo * slowo, int dl_slowa);
void wyswietlanie_czesci_zbioru(slowo * element, int stala, int i);
int wyswietlanie_zbioru_zarz(int * i, int * strona);
void wyszukiwanie(zbior ** tab);
void wyszukiwanie_fraza(zbior ** tab);
void wyszukiwanie_litery(zbior * blok);
void wyszukiwanie_litery_wpr(zbior * blok, int rozmiar);
void wyszukiwanie_przeglad (int ile_znalezionych, slowo * wynik);
/*------------------------------------------------*/
/*------------------PLIKI.C------------------*/
zbior ** otwarcie_plikow(zbior ** tab, char * tryb);
void otworz_pliki(FILE * plik, zbior * tabz);
zbior ** wczytaj_z_pliku(zbior ** tab);
zbior ** wczytywanie(void);
void zamykanie(zbior ** tab);
void zapis_do_plikow(zbior ** tab);
/*-------------------------------------------*/
/*------------------EKRAN.C------------------*/
char * duze_litery(char * napis);
void info (void);
void pisz_aktualne_menu(char * napis);
int polozenie_srodka_napisu(char * napis);
void wiersz_hash(void);
void wiersz_napis(char * napis, int opcja);
void wyswietlanie_opisu(char * napis, int opcja);
/*-------------------------------------------*/
/*------------------PAMIEC.C------------------*/
zbior ** alokacja_pamieci_lista(zbior ** tab);
slowo * dodaj_do_listy (slowo * sl, slowo * glowa, int * opcja); 
slowo ** dodaj_do_tablicy(slowo * glowa, slowo ** tabS);
int licz_el(struct slowo * element);
void sortowanie_tab(slowo ** tab, int ilosc_slow);
slowo * tworz_wyraz (char * wyraz, char * opis);
slowo * usun_element(slowo * element);
slowo * usun_liste (slowo * glowa);
zbior * zwalnianie_jednego_bloku (zbior * blok);
zbior ** zwalnianie_pamieci(zbior ** tab);
/*--------------------------------------------*/