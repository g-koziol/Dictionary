#include "funckcje.h"

slowo * tworz_wyraz (char * wyraz, char * opis) 
{ /* Tworzenie nowego slowa w bazie danych programu. 
     Fukncja kopiuje przys³any wyraz i opis dla nowej struktury.
	 Zwracany jest wskaŸnik do utworzonej struktury */
	struct slowo * nowy;

	nowy=malloc(sizeof(struct slowo));	/* Alokacja pamiêci dla struktury 'slowo' */	
	nowy->wyraz=(char*)malloc(MAX_ROZ_SL+1);
	nowy->opis=(char*)malloc(MAX_LINE);
	strcpy(nowy->wyraz,wyraz);
	if(strlen(opis)>MAX_LINE) /* Zabezpieczenie przed przekroczeniem zarezerwowanego miejsca */
		strncpy(nowy->opis,opis,MAX_LINE);
	else
		strcpy(nowy->opis,opis);
	nowy->nastepny=nowy->poprzedni=NULL;
	return nowy;
}

slowo * usun_element(slowo * element)
{ /* Zwalnianie pamieci dla jednego slowa.
     Zwracany pusty wskaŸnik */
	free(element->opis);
	free(element->wyraz);
	free(element);
	return element=NULL;
}

slowo * usun_liste (slowo * glowa)		
{ /* Usuwanie listy struktur slowo. Funkcja przyjmuje wslaŸnik na pocz¹tek listy.
     Zwraca pusty wskaŸnik */
	slowo * tmp;
	
	if(glowa!=NULL){
		while(glowa!=NULL){		/* Dopóki elementy istniej¹ */
			tmp=glowa;
			glowa=glowa->nastepny;
			if (glowa!=NULL)
				glowa->poprzedni=NULL;
			tmp=usun_element(tmp); /* Zwolnij strukturê */
		}
	}
	return NULL;
}

slowo * dodaj_do_listy (slowo * sl, slowo * glowa, int * opcja) 
{ /* Dodawanie struktury slowa do listy. Przyjmuje wskaŸnik na strukture, która ma zostac dodana,
	 wskaŸnik na pocz¹tek listy do której ma zostac dodana oraz wskaŸnik na int steruj¹cy przebiegiem
	 dzia³ania funkcji. Dla *opcja=1 funckja wyswietla monit o poetorzeniu sie wyrazu, dla *opcja=0
	 informacje t¹ pomija. Sortuje sturktry wed³ug pola wyraz malej¹co. zwraca wskaŸnik na przekszta³con¹
	 listê. */
	int por;

	if(glowa!=NULL) 
	{
		slowo * tmp=glowa;
		if((por=strcmp(tmp->wyraz,sl->wyraz))>0){ /* Je¿eli trzbe wstawic na pocz¹tku */
			sl->nastepny=tmp;
			tmp->poprzedni=sl;
			return glowa=sl;
		}
		else while(tmp->nastepny!=NULL){
			if (por=strcmp(tmp->wyraz,sl->wyraz)<0) /* Jezeli strukturê trzeba wstawiæ dalej */
				tmp=tmp->nastepny;
			else if ((por=strcmp(tmp->wyraz,sl->wyraz))==0){ /* Je¿eli natrafiono na takie samo pole wyraz */
				if (*opcja==1){	/* Je¿eli monit ma byc wyœwietlony */
					printf("\n\tSlowo juz istnieje! [Wcisnij ENTER]");
					enter();
					*opcja=-1;
				}
				return glowa; /* Zwróæ niezmienion¹ listê */
			}
			else { /* Wstawienie na odpowiednie miejsce */
				sl->poprzedni=tmp->poprzedni;
				tmp->poprzedni->nastepny=sl;
				sl->nastepny=tmp;
				tmp->poprzedni=sl;
				return glowa;
			}			
		}
		tmp->nastepny=sl;
		sl->poprzedni=tmp;
		return glowa; 
	}
	return sl; 	/* Je¿eli lista jest pusta */
}

zbior ** alokacja_pamieci_lista(zbior ** tab)
{ /* Alokacja pamiêci dla danych programu. Funckja alokuje pamiec dla tablicy wskaŸników na struktury zbior.
     Dla ka¿dej struktury rezerwuje pamiêæ dla odpowiednich pól i zeruje wskaŸniki. Zwraca wskaŸnik na 
	 tablicê z zaalokowan¹ pamiêci¹. */
	int i=0;
	
	tab=malloc((MAX_ROZ_SL-MIN_ROZ_SL+1) * sizeof(struct zbior *)); /* Alokacja pamieci dla calego zbioru*/
	while(i<(MAX_ROZ_SL-MIN_ROZ_SL+1)){ /* Alokacja miejsca dla jednego zbioru i zerowanie wskaŸników*/
		tab[i]=malloc(sizeof(struct zbior));
		tab[i]->glowa=NULL;
		tab[i]->liczba_slow=0;
		tab[i]->nazwa_pliku=(char *) malloc(MAX_ROZ_SL + 4); /* 3.txt, 4.txt itd. */
		tab[i]->plik=NULL;
		i++;
	}
	return tab;
}

slowo ** dodaj_do_tablicy(slowo * glowa, slowo ** tabS)
{ /* Dodanie elementów listy do osobnej tablicy. Funkcja przyjmuje wskaŸnik na pocz¹tek listy struktur, 
	 które maj¹ zostaæ przypisane jako osobne struktury do tablicy. Zwracay jest wskŸnik do tablicy. */

	int i=0;
	slowo * tmp=NULL, * tmp2=NULL;

	while(glowa!=NULL){ /* Dopóki elementy istniej¹ */
		tabS[i]=tworz_wyraz(glowa->wyraz,glowa->opis);	/* Tworzenie kopii struktury w pamiêci */
		tabS[i]->poprzedni=tmp;
		if (tmp!=NULL)
			tmp->nastepny=tabS[i];
		tmp=tabS[i];
		glowa=glowa->nastepny;
		i++;
	}
	return tabS;
}

zbior * zwalnianie_jednego_bloku (zbior * blok) 
{ /* Funkcja zwalnia z pamiêci strukturê zbior przys³anej przez wskaŸnik */
	blok->glowa=usun_liste(blok->glowa); /* Usuwanie przypisanej listy struktur slowo */
	free(blok->nazwa_pliku); /* zwlanianie zaalokowanej pamiêci dla nazwy pliku */
	free(blok);
	return blok=NULL;
}

zbior ** zwalnianie_pamieci(zbior ** tab)
{ /* Zwalnianie pamiêci zajmowanej przez dne programu. Przyjmuje tablicê wskaŸników na 
     struktury zbior. Zwraca pusty wskaŸnik je¿eli nie nast¹pi³ ¿aden b³¹d. */
	int i;

	for(i=0;i<(MAX_ROZ_SL-MIN_ROZ_SL+1);i++){ /* Usuwanie poszczególnych zbiorów */
		tab[i]=zwalnianie_jednego_bloku(tab[i]);	
	}
	free(tab); /* Zwalanianie wskaznikow na zbiory */
	return tab=NULL;
}

void sortowanie_tab(slowo ** tab, int ilosc_slow)
{ /* Funkcja sortujaca tablice wskaznikow na struktury slowo. Przyjmuje t¹ tablicê i iloœæ elemetów
	 w tej tablicy. Funkcja korzysta z sortowania b¹belkowego. */
	int i,j;
	slowo * tmp;
	
	for(i=0;i<ilosc_slow;i++){
		for(j=0;j<(ilosc_slow-1);j++){
			if(strcmp(tab[j]->wyraz,tab[j+1]->wyraz)>0){
				tab[j-1]->nastepny=tab[j+1];
				tab[j+1]->poprzedni=tab[j]->poprzedni;
				tab[j]->poprzedni=tab[j]->nastepny;
				tab[j]->nastepny=tab[j+1]->nastepny;
				tab[j+1]->nastepny=tab[j];
				tmp=tab[j+1];
				tab[j+1]=tab[j];
				tab[j]=tmp;
			}			
		}
	}
}

int licz_el(slowo * element)
{ /* Funkcja licz¹ca iloœæ elemetów w liœcie struktur slowo. Zwraca liczbê elementów. */
	int i=0;

	while(element!=NULL){
		i++;
		element=element->nastepny;
	}
	return i;
}

		