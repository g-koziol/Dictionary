#include "funckcje.h"

zbior ** wczytywanie(void) 
{ /* Wywyo�anie kolejnych fuknkcji do wczytania danych. Zwraca wska�nik
     na utworzon� tablic� wska�nik�w na zbiory. */
	zbior ** pliki=NULL;

	pliki=alokacja_pamieci_lista(pliki); /* Alokuj pamiec dla zbiorow */
	pliki=otwarcie_plikow(pliki, "r+"); /* Otwrcie plikow do odczytu */
	pliki=wczytaj_z_pliku(pliki); /* Wczytywanie danych z poszczegolnych plikow */
	return pliki;
}

zbior ** otwarcie_plikow(zbior ** tab, char * tryb)
{ /* Funkcja odpowiedzialna za otwarcie plik�w na dysku i zapisanie ich nazwy
  i wska�nika w odpowiednich strukturach zbior. Przyjmuje tablic� wska�nik�w
  na struktury zbior i wska�nik na tekst oznaczaj�cy tryb odczytu. Zwraca
  zmieniona tablice wska�nik�w, kt�ra zosta�a przys�ana. */
	int i=0;
	char liczba[MAX_ROZ_SL + 4];

	while(i<(MAX_ROZ_SL-MIN_ROZ_SL+1)) /* np. od MIN_ROZ_SL do MAX_ROZ_SL */
	{
		sprintf(liczba, "%d.txt", (i+MIN_ROZ_SL));
		strcpy(tab[i]->nazwa_pliku, liczba);
		tab[i]->plik=fopen(tab[i]->nazwa_pliku, tryb);
		if (!tab[i]->plik){ /* Je�eli nie uda�o si� otworzyc pliku utworz nowy*/
			printf("Wystapil blad przy odczycie, tworzenie czystego pliku. (Nacisnij ENTER)\n");
			fflush(stdin);
			getch();
			tab[i]->plik=fopen(tab[i]->nazwa_pliku, "w+");
			fprintf(tab[i]->plik,"0");
			fseek(tab[i]->plik,0,0);
		}
		i++;
	}
	return tab;
}

zbior ** wczytaj_z_pliku(zbior ** tab)
{ /* Wczytywanie danych z poszczeg�lnych plik�w. Funkcja przyjmuje 
     tablic� wska�nik�w na struktury zbi�r i zwraca zmienion� tablic�.
	 Pobiera z pliku MAX_LINE znak�w dla wyrazu i opisu i dopisuje na 
	 koniec EOF */
	int i=0,j, opcja=0;
	char napis[MAX_LINE], opis[MAX_LINE];

	while(i<(MAX_ROZ_SL-MIN_ROZ_SL+1)){
		tab[i]->glowa=NULL;		
		fscanf(tab[i]->plik, "%d\n", &(tab[i]->liczba_slow)); /* informacja o ilo�ci s��w */
		while (feof(tab[i]->plik)==0)
		{			
			fgets(napis,MAX_LINE,tab[i]->plik); /* wczytaj wyraz i opis */
			fgets(opis,MAX_LINE,tab[i]->plik);
			napis[strlen(napis)-1]=0; /* Dopisanie na koniec EOF */
			if (opis!="\n")
				opis[strlen(opis)-1]=32;
			for(j=0;j<strlen(napis);j++)
				napis[j]=toupper(napis[j]);
			tab[i]->glowa=dodaj_do_listy(tworz_wyraz(napis,opis),tab[i]->glowa,&opcja); /* dodaj do listy odczytane dane*/
		}
		i++;
	}
	return tab;
}

void zamykanie(zbior ** tab) 
{ /* Wywo�anie funkcji konczace prace programu. Przyjmuje 
  tablice wska�nik�w na struktury zbior. Zapisuje dane z pami�ci
  do plik�w, zamyka pliki i zwalnia pami��. */
	int j;

	zapis_do_plikow(tab);
	for(j=0; j<(MAX_ROZ_SL-MIN_ROZ_SL+1); j++){ /* zamknij otwarte pliki */
		fclose(tab[j]->plik);
		tab[j]->plik=NULL;
	}
	zwalnianie_pamieci(tab); /* Wyczysc zajmowana pamiec przez dane programu */
}

void zapis_do_plikow(zbior ** tab)
{ /* Funkcja odpowiedzialn za w�a�ciwe zapisanie danych do
  plik�w. przyjmuje tablic� wska�nik�w na struktury zbior. */
	int i=0;
	slowo * tmp;

	while(i<(MAX_ROZ_SL-MIN_ROZ_SL+1)){
		tmp=tab[i]->glowa;
		fclose(tab[i]->plik);
		tab[i]->plik=fopen(tab[i]->nazwa_pliku, "w");
		fprintf(tab[i]->plik, "%d", licz_el(tab[i]->glowa));
		while (tmp){			
			fprintf(tab[i]->plik,"\n%s\n%s", tmp->wyraz, tmp->opis);
			tmp=tmp->nastepny;
		}
		i++;
	}
}