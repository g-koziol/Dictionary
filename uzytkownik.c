#include "funckcje.h"

zbior ** obsluga(zbior ** tab) 
{ /* Funkcja odpowiedzialna za menu g³ówne. Przyjmuje tablicê wskaŸników na struktury zbior.
	 Zwraca przys³an¹ tablicê. */
	int klawisz=0;

	while(klawisz!='5'){
		pisz_aktualne_menu("menu");
		printf("\t(1) Przeglad hasel\n\t(2) Wyszukiwanie hasel\n\t(3) Dodaj haslo\n\t\(4) Info\n\t(5) Wyjscie\n\n\tDla calego programu wciskaj ESC aby powrocic.\n\tNacisnij cyfre odpowiadajaca danej opcji: ");
		fflush(stdin);
		switch (klawisz=getch()){
		case '1':
			przeglad(tab);
			break;
		case '2':
			wyszukiwanie(tab);
			break;
		case '3':
			dodaj_slowo(tab);
			break;
		case '4':
			info();
			break;
		case '5':
			break;
		}
	}
	return tab;
}

void przeglad (zbior ** tab) 
{ /* Funkcja odpowiedzialana za sterowanie przegl¹daniem bazy danych. Przyjmuje tablice wskaŸników na struktutry zbior.
     Pobiera od u¿ytkownika ¿¹dany rozmiar i przechodzi do wyœwietlania odpowiedniego zbioru. Sprawdza poprawnoœæ
	 wprowadzonych danych oraz informuje o braku danych. Dzia³a a¿ do wciœniêcia klawisza ESC*/
	int rozmiar=0, klawisz=0;
	
	while(1){
		pisz_aktualne_menu("przeglad slow");
		printf("\tPodaj rozmiar slowa [%d - %d] i zatwierdz wciskajac ENTER: ", MIN_ROZ_SL, MAX_ROZ_SL);
		fflush(stdin);
		if(pobierz_liczbe(&rozmiar)==1) /* Pobierz liczbê i sprawdŸ czy nie naciœniêto klawisza ESC */
			break;	
		if (rozmiar>=MIN_ROZ_SL && rozmiar<=MAX_ROZ_SL){ /* Je¿eli podano w³aœciwy rozmiar */
			if (tab[rozmiar-MIN_ROZ_SL]->glowa!=NULL){ /* Je¿eli zbiór istnieje */
				przeglad_zbioru(tab[rozmiar-MIN_ROZ_SL], rozmiar); /* Przejœcie do przegl¹du danych */
				if (tab[rozmiar-MIN_ROZ_SL]->glowa==NULL){ /* Sprawdzenie istnienia danych w razie ich usuniêcia */
					pisz_aktualne_menu("przeglad slow");
					printf("\n\tBrak danych! (Nacisnij ENTER)");
					enter();
					continue;
				}
				else continue;
			}
			else{
				printf("\n\tBrak danych! (Nacisnij ENTER)");			
				enter();
				continue;
			}
		}
		else{
			printf("\n\tPodano zly rozmiar! (Nacisnij ENTER)");
			enter();
			continue;	
		}
		break;
	}
}

void przeglad_zbioru (zbior * blok, int dl_slowa) 
{ /* Przeglad wybranego zbioru. Przyjmuje wskaŸnik na strukture zbior i wartoœæ d³ugoœci wyrazów wystêpuj¹cych w zbiorze.
     Kontroluje przegl¹danie kolejnych strn i prze³¹czanie miêdzy nimi. W razie potrzeby alokuje rozmiar tablicy na nowo.
	 Sortuje ca³¹ tablicê pod wzglêdem wyrazów. Zmienione dane przypisuje jako now¹ listê.*/
	int i=0, strona=1, klawisz;	
	char nr_strony[10]; /* Zmienna dla przechowywania numeru strony */
	slowo ** tab; /* Tablica wskaŸników na struktry slowo, do której zostanie skopiowana lista */

	tab=malloc(blok->liczba_slow * sizeof(slowo * ));
	tab=dodaj_do_tablicy(blok->glowa, tab);	 /* £adownie listy do tablicy */
	while(1){
		blok->liczba_slow=licz_el(blok->glowa); /* Licz elementy listy */
		klawisz=-1;
		if (((blok->liczba_slow-1)-(strona-1)*10)<0){ /* Sprawdzanie poprawnoœci wybranej strony */
			i-=10;
			strona-=1;
		}
		if(strona <= 0){
			strona=1;
			i=0;
		}
		fflush(stdin);
		pisz_aktualne_menu("przeglad slow");
		sprintf(nr_strony,"Strona: %d", strona); /* Przypisanie numeru strony jako tekst */
		wiersz_napis(nr_strony,0);
		wyswietlanie_czesci_zbioru(tab[i],EL_NA_STR,i); /* Przejœcie do wyœwietlenia jednej strny z danymi */
		if((klawisz=wyswietlanie_zbioru_zarz(&i,&strona))==0)	/* Sprawdzenie naciœniêtego klawisza */
			break;
		else if (klawisz>=48 && klawisz<=57) /* Je¿eli naciœniêto odpowiedn¹ cyfrê - przejœcie do podgl¹du danego wyrazu */
			if (blok->liczba_slow-(strona-1)*10-1 >= klawisz-48)
				tab[((strona-1)*10+(klawisz-48))]=wybierz_slowo(tab[((strona-1)*10+(klawisz-48))], dl_slowa);	/* Przejœcie do funkcji wyœwietlaj¹cej wyraz */
		if (tab[((strona-1)*10+(klawisz-48))]==NULL) /* Sprawdzenie czy lista istnieje */
			break;		
		tab=realloc(tab, (blok->liczba_slow=licz_el(tab[0])) * sizeof(slowo *)); /* Zmiana rozmiaru zaalokowanej pamiêci */
		sortowanie_tab(tab, blok->liczba_slow); /* sortowanie zmienionej tablicy */
	}
	blok->glowa=usun_liste(blok->glowa); /* usuwanie starych danych */
	blok->glowa=tab[0];	/* Przypisanie nowych danych do zbioru */
	free(tab);
}

int wyswietlanie_zbioru_zarz(int * i, int * strona)
{ /* Funkcja steruj¹ca przebiegiem funkcji przeglad_zbioru. Przyjmuje wskaŸniki na aktualny element który
	 ma byæ wyœwietlony na ekranie oraz wskaŸnik na numer aktualnej strony. Funkcja sprawdza czy nie naciœniêto klawisza 
	 odpowiadaj¹cemu danej funkcji. Zwraca naciœniêty klawisz */
	int klawisz=-1;
	
	fflush(stdin);
	if ((klawisz=getch())==27) /* jezeli esc */
		return 0;
	else if (klawisz=='p'){ /* Je¿eli naciœniêto p - poprzednia strona */
		*i-=10;
		*strona-=1;
	}
	else if(klawisz=='n'){ /* Je¿eli naciœniêto n - nastêpna strona */
		*i+=10;
		*strona+=1;
	}
	else if (klawisz>=48 && klawisz<=57) /* Je¿eli naciœniêto cyfrê */
		return klawisz;
	return 1; /* Je¿eli naciœnieo klawisz nie przypisany do ¿adnej funkcji */
}

slowo * wybierz_slowo(slowo * slowo, int dl_slowa)
{ /* Funkcja odpowiedzilana za wyœwietlenie informacji o konkretnym s³owie i mo¿liwoœæ jego edycji/usuniêcia.
     Funkcja przyjmuje wskaŸnik  na wyœwietlane s³owo i d³ugoœæ pola wyraz w przys³anej strukturze.
	 Je¿eli za dl_slowa przys³ano 0, funkcja nie pozwoli na edytowanie/usuwanie struktury.
	 Zwraca wskaŸnik na zedytowan¹ strukturê */
	char klawisz=-1;
	
	while(1){
		pisz_aktualne_menu("WYBRANE SLOWO");
		printf("\n\tSlowo: %s\n", slowo->wyraz);
		wyswietlanie_opisu(slowo->opis, 0); /* Wyœwietlanie du¿ego opisu */
		if (dl_slowa!=0) 
			printf("\n\tNacisnij: E - edytuj, U - usun");
		printf("\n\n\tAby powrocic nacisnij ESC");
		fflush(stdin);
		klawisz=getch();
		if (klawisz==10 || klawisz==27) /* Je¿eli nacœniêto klawisz ESC lub backspace */
			return slowo;
		else if(dl_slowa!=0){
			if (klawisz=='E' || klawisz=='e')
				edytuj_slowo(slowo, dl_slowa); /* PrzejdŸ do edycji s³owa */
			else if((klawisz=='U') || (klawisz=='u')){
				return usun_slowo(slowo); /* PrzejdŸ do usuwnaia s³owa */
			}
		}
	}
}
 
void wyswietlanie_czesci_zbioru(slowo * element, int stala, int i) 
{ /* Wyœwietlanie jednej strony z elemenatami zbioru. Funckja przyjmuje wskaŸnik na pierwszy element do wyœwietlenia, wartoœæ okreœlaj¹c¹ 
     iloœæ elementów na stronie oraz wartoœæ indeksu przypisanego do przys³anego wskaŸnika. Funkcja edytuje opis s³owa tak, aby zmieœci³ siê na 
	 ekranie a przys³oniêt¹ czêœæ przys³ania "..." */
	int licz_porz=1, koniec_wiersza=(DLUGOSC_MENU - 18), j;
	char tmp[MAX_LINE]="", linia[MAX_LINE]="";

	wiersz_napis(NULL,0);
	i=i%stala; /* Dzielenie modulo dla poprawnego sprawdzenia czy mo¿na jeszcze wyœwietliæ dane na stronie */ 
	while (i<stala && element!=NULL){ /* Dopóki zakres jest nie przekroczony i istniej¹ dane */
		strcpy(linia, element->wyraz); /* Tworzenie jedenj lini z wyrazu i opisu */
		strcat(linia, " - ");
		strncat(linia, element->opis, (MAX_LINE - strlen(linia) - 3));
		if(strlen(linia)>=koniec_wiersza){ /* Je¿eli utworzona linia jest wiêksza niz dostene miejsce */
			strncpy(&tmp, linia, koniec_wiersza-3);
			strcpy(&linia, tmp);
			for(j=0;j<3;j++)
				linia[strlen(linia)-3+j]='.';
		}
		printf("\n\t (%d) %s", i, linia); /* Wyswietlenie lini na ekranie */
		element=element->nastepny;
		i++;
	}
	printf("\n\n\tCyfra - przejscie dalej, n - nastepna strona, p- poprzednia strona, \n\tESC - powrot.");
}

void wyszukiwanie(zbior ** tab)
{ /* Funkcja odpowiedzialna za menu wyszukiwania. Urochoamia kolejne funckje. Przyjmuje wskaznik na tablice wskaznikow 
	 do struktur zbior. */
	int i=0;

	while(i!=27){ /* Dopóki nie naciœniêto kalwisza ESC */
		pisz_aktualne_menu("wyszukiwanie");
		printf("\t(1) Wyszukiwanie po frazie\n\t(2) Wyszukiwanie po znanych literach");
		fflush(stdin);
		switch(i=getch()){
		case '1':
			wyszukiwanie_fraza(tab);
			break;
		case '2':
			wyszukiwanie_litery(tab);
			break;
		}
	}
}
	
void wyszukiwanie_fraza(zbior ** tab) 
{ /* Funkcja ospowiedizlana za wyszukiwanie danych poprzez szukanie podanej frazy od u¿ytkownika.
	 Przyjmuje tablicê na wskaŸnik do struktur zbior. Pobiera od u¿ytkownika szukan¹ frazê i 
	 wyszukuje wed³ug niej pasuj¹ce wyrazy i opisy. Wyniki ³aduje do listy dynamicznej i wyswietla na 
	 ekranie. */
	int ile_znalezionych=0;
	slowo * wynik=NULL;
	char szukane[MAX_LINE];
	
	while(1){
		pisz_aktualne_menu("Wyszukiwanie");
		printf("\tPodaj slowo, ktorego chcesz szukasz i wscinij ENTER\n\n\t: ");
		if(pobierz_tekst(&szukane)==1) /* Pobiera szukan¹ frazê i sprawdza czy nie naciœniêto ESC */
			break;
		wynik=laduj_znalezione(tab,szukane,&ile_znalezionych); /* £aduj znalezione elementy do listy */
		pisz_aktualne_menu(szukane);
		if (ile_znalezionych!=0){			
			wyszukiwanie_przeglad(ile_znalezionych, wynik); /* Przejœcie do wyœietlania wyników */
			continue;
		}
		else{  /* Je¿eli nic nie znaleziono */
			printf("\n\tNic nie znaleziono! (nacisnij jakis klawisz...)");
			fflush(stdin);
			getch();
			continue;
		}
	}
}

void wyszukiwanie_litery(zbior ** tab)
{ /* Wyszukiwanie zbioru pod k¹tem znanych u¿ytkownikowi liter i dlugoœci szukanego s³owa. 
     Przymuje tablicê wskaŸników na struktry zbior. Funkcja pobiera od uzytkownika liczbe 
	 z rozmiarem szukanego wyrazu i uruchamia funkcjê pobierania kolejnych liter. Sprawdza 
	 poprawnoœæ podanych danych i czy dane istnieja. */
	int rozmiar=0;

	while(1){
		pisz_aktualne_menu("Wyszukiwanie po znanych literach");
		printf("\tPodaj rozmiar slowa [%d - %d] i potwierdz wciskajac ENTER: ", MIN_ROZ_SL, MAX_ROZ_SL);
		fflush(stdin);
		if(pobierz_liczbe(&rozmiar)==1) /* Pobierz rozmiar suzkanego wyrazu */
			break;	
		if (rozmiar>=MIN_ROZ_SL && rozmiar<=MAX_ROZ_SL){ /* Je¿eli podano poprawny rozmiar */
			if (tab[rozmiar-MIN_ROZ_SL]->glowa!=NULL){
				wyszukiwanie_litery_wpr(tab[rozmiar-MIN_ROZ_SL], rozmiar); /* Przejœcie do pobrania liter */
				if (tab[rozmiar-MIN_ROZ_SL]->glowa==NULL){
					pisz_aktualne_menu("Wyszukiwanie po znanych literach");
					printf("\n\tBrak danych! (Nacisnij ENTER)");
					enter();
					continue;
				}
				else continue;
			}
			else{ /* Je¿eli zbior o wybranej dlugosci wyrazu jest pusty */
				printf("\n\tBrak danych! (Nacisnij ENTER)");			
				enter();
				continue;
			}
		}
		else{ /* Je¿eli podano z³y rozmiar */
			printf("\n\tPodano zly rozmiar! (Nacisnij ENTER)");
			enter();
			continue;	
		}
		break;
	}
}

void wyszukiwanie_litery_wpr(zbior * blok, int rozmiar)
{ /* Funkcja pobiera od uzytkownika kolejne litery. Przyjmuje wskaŸnik na 
	 strukture zbior o wybranej dlugosci wyrazów i wartoœæ ich d³ugoœci.
	 Je¿eli u¿ytkownik nie zna litery naciska ENTER i zostawiane jest puste 
	 miejsce dla znaku. Wywo³uje funkcje wyszukuj¹c¹ i wyœwietlaj¹c¹ wyniki. */
	
	int i=0, ile_znalezionych=0;
	char wyraz[MAX_ROZ_SL];
	slowo * wynik;

	while(1){
		for(i=0;i<rozmiar;i++){
			pisz_aktualne_menu("Wyszukiwanie po znanych literach");
			printf("\tPodaj %d litere. Jesli nie znasz nacisnij ENTER: ", i+1); /* Pobieranie kolejnej litery */
			fflush(stdin);
			if((wyraz[i]=getch())==27) /* Je¿eli naciœniêto ESC */
				return;
			else if(wyraz[i]<65 || (wyraz[i]>90 && wyraz[i]<97) || wyraz[i]>122) /* Je¿eli naciœniêto klawisz litery */
				wyraz[i]=32;
		}
		wynik=laduj_znalezione_litery(blok, wyraz, &ile_znalezionych, rozmiar); /* Przeszukanie zbioru i za³dowanie do listy */
		if (ile_znalezionych!=0){			
			wyszukiwanie_przeglad(ile_znalezionych, wynik); /* Podgl¹d wyszukaych hase³ */
			continue;
		}
		else{
			printf("\n\tNic nie znaleziono! (nacisnij jaks klawisz...)");
			fflush(stdin);
			getch();
		
		break;
		}
	}
}

void wyszukiwanie_przeglad (int ile_znalezionych, slowo * wynik)
{ /* Funkcja odpowiedzialna za stworzenie struktury blok, dla funkji przegla_zbioru.
     Dziêki temu mo¿na wykorzystac to funkcjê równie¿ przy wyszukiwaniu. Przyjmuje 
	 wartoœæ znalezionych elementów i wskaŸnik na pierwszy element listy 
	 z wynikami. Alokuje pamiêæ i usuwa znalezione dane po skoczeniu ich przegl¹dania. */
	zbior * blok=NULL;

	blok=malloc(sizeof(zbior));
	blok->liczba_slow=ile_znalezionych;
	blok->glowa=wynik;
	przeglad_zbioru(blok,0); /* Wyswietl znalezione elementy */
	usun_liste(blok->glowa);
	free(blok);
	blok=NULL;
}

void enter() 
{ /* Czekaj na enter */
	char klawisz=0;

	fflush(stdin);
	while((klawisz=getchar())!=10)
		;
}

int klawisz (int * klawisz, int * i, FILE * p) 
{ /* Sprawdzenie nacisniêtego klawisza przyjmuje wskaŸnik na wartoœæ naciœniêtego
     klawisza, pozycje kolejnego wcisnietego kalwisza oraz wskznik na plik, który
	 zostanie uzyty jako bufor. Je¿eli naciœniêto Backspace, funkcja usuwa znak na 
	 ekranie i buforze. Sprawdzanie kolejnych liter koñczy siê z naciœniêciem kalwisza
	 ENTER. Zwraca 0 jezeli ENTER, 1 je¿eli inny klawisz. */
	fpos_t pos;

	fflush(stdin);	
	if ((*klawisz=getch())==13) /* jezeli enter */
		return 0;
	else if ((*klawisz==8) && (*i!=0) ){ /* jezeli backspace usun jeden klawisz z buforu i ekranu */
		fgetpos(p,&pos);
		pos-=1;
		fsetpos(p, &pos);
		putc('Í',p);
		fsetpos(p, &pos);
		putchar(8);
		putchar(32);
		putchar(8);
		(*i)--;
	}
	else if (*klawisz!=8) {
		fgetpos(p,&pos);
		tolower(&klawisz);
		putc(*klawisz,p); /* jezeli inne daj na ekran i do bufora */
		putchar(*klawisz);
		(*i)++;
	}
	return 1;	
}

slowo * laduj_znalezione(zbior ** tab, char * szukane, int * licznik) 
{ /* Za³aduj znalezione wyniki po frazie i za³aduj do listy. Funkcja przyjmuje
     tablic wskaŸników na struktury zbior, wskaŸnik na szukan¹ frazê i 
	 wskaŸnik na liczbê znalezionych elementów. Zwraca wskaŸnik na pocz¹tek listy */
	int i,j, opcja=0;
	slowo * tmp=NULL, *tmpz=NULL;
	char a[MAX_LINE], b[MAX_LINE];

	for(j=0;j<2;j++){
		for(i=0;i<(MAX_ROZ_SL-MIN_ROZ_SL+1);i++){
			tmpz=tab[i]->glowa;
			while(tmpz){
				strncpy(a, tmpz->wyraz, MAX_LINE);
				strncpy(b, tmpz->opis, MAX_LINE);
				if(strstr(a,szukane) || strstr(b,szukane)){					
					tmp=dodaj_do_listy(tworz_wyraz(a,b),tmp,&opcja); /* jezeli znalezione tworz nowy element do listy znalezionych */	
				}
				tmpz=tmpz->nastepny;
			}
		}
		szukane=duze_litery(szukane); /* Przekszta³cenie poszukiwanej frazy na du¿e litery ze wzglêdu na znaczenie wielkoœci liter */
	}
	szukane=male_litery(szukane);
	*licznik=licz_el(tmp); /* Zlicz znalezione */
	return tmp;
}

slowo * laduj_znalezione_litery(zbior * blok, char * szukane, int * ile_znalezionych, int rozmiar)
{ /* Za³aduj znalezione wyniki po znanych i za³aduj do listy. Funkcja przyjmuje
     wskaŸnik na strukturê zbior, wskaŸnik na szukany klucz, wskaŸnik na liczbê 
	 znalezionych elementów i wartoœæ d³ugoœci wyrazu. Zwraca wskaŸnik na pocz¹tek listy 
	 z wynikami */
	int i=0, znaleziono=0, opcja=0;
	char * tmp;
	slowo * tmp_glowa=blok->glowa, * wynik=NULL;

	szukane=duze_litery(szukane);
	while(tmp_glowa!=NULL){
		znaleziono=0;
		for(i=0;i<rozmiar;i++){ /* Sprawdzenie ka¿dej litery wyrazu wzglêdem podanego klucza */
			if(szukane[i]>=65 && szukane[i]<=90){
				if(tmp_glowa->wyraz[i]==szukane[i]){ /* Je¿eli litery zgadzaj¹ siê na denj pozycji */
					znaleziono=1;
					continue;
				}
				else{ /* Je¿eli litery nie zgadzaj¹ siê na denj pozycji */
					znaleziono=0;
					break;
				}
			}
			else if(szukane[i]=32) /* Je¿eli puste pole klucza - automatyczna zgodnosc */
				znaleziono=1;
		}
		if(znaleziono==1) /* Je¿eli wyraz pasuje do klucza, dodaj do listy z wynikami */
			wynik=dodaj_do_listy(tworz_wyraz(tmp_glowa->wyraz,tmp_glowa->opis), wynik, &opcja);
		tmp_glowa=tmp_glowa->nastepny;
	}
	*ile_znalezionych=licz_el(wynik); /* Licz ile znaleziono */
	return wynik;
}

int pobierz_tekst (char * tekst) 
{ /* Pobieranie tekstu od u¿ytkownika z mo¿liwoœci¹ wycofania siê z wprowadzania 
     Zwraca 1 jezlei naciœniêto kalwisz ESC, je¿li pobra³ tekst zwraca 0.*/
	int kl=0,i=0;
	fpos_t position;
	FILE * p;

	p=fopen("bufor","w+");
	fgetpos(p,&position);
	while(klawisz(&kl,&i,p))		
		if(kl==27){
			fclose(p);
			return 1;
		}
	fsetpos(p,&position);		
	fgets(tekst,MAX_LINE,p);
	fclose(p);
	return 0;
}

int pobierz_liczbe (int * liczba) 
{ /* Pobieranie liczby od u¿ytkownika z mo¿liwoœci¹ wycofania siê z wprowadzania 
     Zwraca 1 jezlei naciœniêto kalwisz ESC, je¿li pobra³ tekst zwraca 0.*/
	int kl=0, i=0;
	fpos_t position;
	FILE * p;

	p=fopen("bufor","w+");
	fgetpos(p,&position);
	while(klawisz(&kl,&i,p))		
		if(kl==27){
			fclose(p);
			return 1;
		}
	fsetpos(p,&position);		
	fscanf(p,"%d",liczba);
	fclose(p);
	return 0;
}

void dodaj_slowo (zbior ** zbior) 
{ /* Funkcja odpowiedzialna za dodanie nowego s³owa do bazy danych
     programu. Przyjmuje tablicê wskaŸników na struktury zbior.
	 Funkcja pobiera od uzytkownika rozmiar wyrazu, wyraz i opis,
	 tworzy strukturê i dodaje do obecnej bazy danych. */
	int rozmiar=0, klawisz=0, j, opcja;
	char wyraz[MAX_LINE], opis[MAX_LINE];

	while(1){
		opcja=1;
		pisz_aktualne_menu("DODAWANIE SLOWA");
		printf("\tPodaj rozmiar slowa ktore chcesz dodac [%d - %d] i wcisnij ENTER: ", MIN_ROZ_SL, MAX_ROZ_SL);
		fflush(stdin);
		if(pobierz_liczbe(&rozmiar)==1) /* Pobranie rozmiaru wyrazu */
			break;
		if (!(rozmiar>=MIN_ROZ_SL && rozmiar<=MAX_ROZ_SL)){
			printf("\n\tPodano zly rozmiar! (Nacisnij ENTER)");
			enter();
			continue;
		}
		printf("\n\tPodaj nowe slowo i wcisnij ENTER: ");
		if (pobierz_tekst(wyraz)==1) /* Pobranie wyrazu */
			break;
		else if(strlen(wyraz)!=rozmiar){
			printf("\n\tNiepoprawna dlugosc slowa! (nacisnij jakis klawisz...)");
			getch();
			continue;
		}
		printf("\n\tPodaj opis do nowego slowa i wcisnij ENTER: ");
		pobierz_tekst(opis);
		for(j=0;j<strlen(wyraz);j++)
			wyraz[j]=toupper(wyraz[j]);
		zbior[rozmiar-3]->glowa=dodaj_do_listy(tworz_wyraz(wyraz,opis),zbior[rozmiar-3]->glowa,&opcja); /* Dodaj do bazy danych */
		if (opcja>=0){ /* Je¿eli funkcja dodaj¹ca listê nie zmieni zmiennej opcja na -1 oznacza to ¿e s³owo ju¿ istnieje */
			zbior[rozmiar-3]->liczba_slow+=1;
			printf("\n\tDodano nowe slowo do bazy! (nacisnij jakis klawisz...)");
			getch();
		}
	}
}

void edytuj_slowo(slowo * element, int dl_slowa)
{ /* Funkcja odpowiedzialna za menu edycji struktury slowo.
     Przyjmuje wskaŸnik na strukture przeznaczon¹ do edycji 
	 oraz dlugosc wyrazu w strukturze. Uruchamia kolejne funkcje
	 odpwiedzialne za edycjê. */
	char klawisz;
	while(1){
		pisz_aktualne_menu("edycja");
		printf("\tCo chcesz poprawic? Wcisnij W(wyraz), O(opis) lub ESC aby powrocic.");
		fflush(stdin);
		klawisz=getch();
		if (klawisz==10 || klawisz==27)
			break;
		else if(klawisz=='w' || klawisz=='W')
			edytuj_wyraz(element->wyraz, dl_slowa); /* Przejdz do edycji wyrazu */
		else if(klawisz=='o' || klawisz=='O')
			edytuj_opis(element->opis); /* przejdz do edycji opisu*/
	}
}

void edytuj_wyraz(char * wyraz, int dl_slowa)
{ /* Funckja odpowiedzialna za edycje wyrazu. Przyjmuje 
	 wskaŸnik na wyraz do edycji i wartoœæ jego d³ugoœci.
	 Pobiera od u¿ytkownika nowy wyraz, sprawdza jego poprawnoœæ
	 i przypisuje do przys³anego wskaŸnika. */
	char * str=malloc(MAX_LINE);
	
	while(1){
		pisz_aktualne_menu("edytuj wyraz");
		printf("\tPodaj nowy %d-literowy poprawiony wyraz i wcisnij ENTER: ", dl_slowa);
		if (pobierz_tekst(str))
			break;
		if (strlen(str)!=dl_slowa){
			fflush(stdin);
			getch();
			printf("\n\tNiepoprawna dlugosc edytowanego slowa! (nacisnij jakis klawisz)");
			getch();
			continue;
		}
		strcpy(wyraz,str);
		printf("\n\tDane zmienione! (nacisnij jakis klawisz...)");
		fflush(stdin);
		getch();
		free(str);
		duze_litery(wyraz); /* zmiana liter wyrazu na duze */
		break;
	}
}

void edytuj_opis(char * opis)
{ /* Funckja odpowiedzialna za edycje opisu. Przyjmuje 
	 wskaŸnik na wyraz do edycji. Pobiera od u¿ytkownika 
	 nowy opis i przypisuje do przys³anego wskaŸnika. */
	char * str=malloc(MAX_LINE);

	while(1){
		pisz_aktualne_menu("edytuj opis");
		printf("Podaj nowy poprawiony opis i wcisnij ENTER:\n");
		if(pobierz_tekst(str))
			break;
		strncpy(opis,str,MAX_LINE);
		printf("\n\tDane zmienione! (nacisnij jakis klawisz...)");
		fflush(stdin);
		getch();
		free(str);
		break;
	}
}

slowo * usun_slowo(slowo * element)
{ /* Funkcja odpowiedzialna za usniêcie struktury slowo z bazy danych.
     Przyjmuje wskaŸnik naa strukture do usuniêcia. Pobiera potwierdzenie
	 od u¿ytkownika i ustawia wskaŸniki i zwalnia pamiêæ */
	char klawisz;

	while(1){
		printf("\n\tCzy jestes pewien [T/N]?: ");
		fflush(stdin);
		klawisz=getch();
		if((klawisz=='t') || (klawisz||'T')){
			if(element->nastepny!=NULL){ /* Jezeli istnieje nastepny element */
				if(element->poprzedni!=NULL)
					element->poprzedni->nastepny=element->nastepny;
				element->nastepny->poprzedni=element->poprzedni;
			}
			if(element->poprzedni!=NULL){ /* Jezeli istnieje poprzedni element */
				if(element->nastepny!=NULL)
					element->nastepny->poprzedni=element->poprzedni;
				element->poprzedni->nastepny=element->nastepny;
			}
			element=usun_element(element);
			return element=NULL;
		}
		else if ((klawisz=='n') || (klawisz=='N')) /* Je¿eli zrezygnowano z usuwania */
			return element;
	}
}