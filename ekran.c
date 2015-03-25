#include "funckcje.h"

void pisz_aktualne_menu(char * napis) 
{ /* Funkcja wyœwietla aktualne menu. Przyjmuje wskaŸnik na napis maj¹cy znaleŸæ siê w charakterystycznej ramce. */
	system("cls");
	wiersz_hash(); /* Wiersz z³o¿ony z znaków # */
	wiersz_napis(NULL,1); /* Przerwa w menu */
	wiersz_napis(napis,1); /* Linijka z napisem */
	wiersz_napis(NULL,1);
	wiersz_hash();
	putchar('\n'); 
}

void wiersz_hash(void) 
{ /* Funkcja wypisuje w oknie liniê z³o¿on¹ z # */
	int i;

	for(i=0; i<DLUGOSC_MENU-1; i++){
		putchar('\#');
		if((i+2)==DLUGOSC_MENU)
			putchar('\n');
	}
}

char * duze_litery(char * napis) 
{ /* Zamiana wszystkich ma³ych liter w napisie na du¿e. Funkcja przymuje wskaŸnik na napis do przekszta³cenia.
     Zwraca przekszta³ocony napis. Je¿eli przys³ano pusty wskaŸnik funkcja zwraca EOF */
	int i=0;
	if (napis!=NULL){
		while (napis[i]!='\0'){
			napis[i]=toupper(napis[i]);	/* Zmieñ ma³¹ literê na du¿¹ */	
			i++;
		}
		return napis;
	}
	else return '\0';
}

char * male_litery(char * napis) /*zamiana wszystkich amlych liter w napisie na duze*/
{ /* Zamiana wszystkich duzych liter w napisie na ma³e. Funkcja przymuje wskaŸnik na napis do przekszta³cenia.
     Zwraca przekszta³ocony napis. Je¿eli przys³ano pusty wskaŸnik funkcja zwraca EOF */
	int i=0;
	if (napis!=NULL){
		while (napis[i]!='\0'){
			napis[i]=tolower(napis[i]);			
			i++;
		}
		return napis;
	}
	else return '\0';
}

void wiersz_napis(char * napis, int opcja) 
{ /* Funkcja wyœwietla liniê z przys³anym napisem. Jezeli opcja=1, na krañcach d³ugoœci menu wyœwietlane s¹ #, dla innej opcji
     nie s¹ wyœwietlane. Je¿eli wskaŸnik na napis jest NULL, nie jest wyœwietlany ¿aden napis */
	int i=1, j=0, srodek=0;

	srodek=polozenie_srodka_napisu(napis); /* Obliczanie œrodka dla napisu */
	while (i<DLUGOSC_MENU){
		if (napis!=NULL && srodek!=0 && i==srodek ){
			while (napis[j]){
				putchar(toupper(napis[j]));
				j++;
			}
			i+=strlen(napis);
		}
		if(i==1){ /* Je¿eli pierwszy znak w lini */
			if(opcja==1)
				putchar('#');
		}
		else if (i==DLUGOSC_MENU-1){ /* Je¿eli ostatni znak w lini */
			if(opcja==1)
				printf("#\n");
		}
		else putchar('\ ');
		i++;
	}
}

int polozenie_srodka_napisu(char * napis) 
{ /* Obliczanie po³o¿enia œrodka napisu. Zwracana obliczona wartoœæ. */
	int odleglosc;

	if (napis!=NULL) 
		return odleglosc=(DLUGOSC_MENU/2 - strlen(napis)/2);
	else return 0;
}

void info (void)
{ /* Funkcja wyswietlajaca informacje dla uzytkownika */
	pisz_aktualne_menu("INFO");
	wyswietlanie_opisu("Witaj w moim programie ulatwiajacym rozwiazywanie krzyzowek! Do sterowania programem potrzebujesz klawiatury oraz dodatkowo myszki w razie potrzeby przewijania ekranu konsoli. W programie mozesz przegladac, wyszukiwac i edytowac baze danych programu. W kazdej sytuacji mozesz uzyc esc w razie potrzeby cofniecia sie w menu. Powodzenia! (Nacisnij ENTER)", 1);
	enter();
}

void wyswietlanie_opisu(char * napis, int opcja)
{ /* Funkcja odpowiedzialana za podzia³ d³ugiego opisu na czêœci i wyœwietlenie je na ekranie. 
	 Przyjmuje wskaŸnik na tekst który ma zostac wyœwietlony oraz wartoœæ int, która jeœli ró¿na
	 od zera to nie wyswietlany jest tekst "Opis: ". */
	char str[]="Opis: ", ostatni[DLUGOSC_MENU - 24];
	int dlugosc=strlen(napis), stala=(DLUGOSC_MENU - 24), i=0,j=0, licznik, reszta, z=0;
	
	i=dlugosc/stala;
	if (reszta=dlugosc%stala){
		j=i*stala;
		while (ostatni[z]=napis[j]){
			z++;
			j++;
		}
		i++;
	}
	if (opcja)
		printf("\t      ");
	else
		printf("\t%s", str); /* Wyœwietlenie "Opis: " */
	for(licznik=0;licznik<i;licznik++){
		j=licznik*stala;
		while((j<(licznik+1)*stala) && putchar(napis[j]))
			j++;
		printf("\n\t      ");
	}
}

		
	


