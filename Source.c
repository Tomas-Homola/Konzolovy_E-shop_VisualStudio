#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pocet_produktov, pocet_kupenych_produktov = 0; // celkovy pocet produktov
float minute_peniaze = 0.0;

typedef struct
{
	int ID;
	char nazov[20];
	char vyrobca[20];
	int pocet_kusov;
	float cena;
} PRODUKT;

PRODUKT* produkty; // smernik, ktory bude ukazovat na miesto, kde budu jednotlive produkty ulozene

typedef struct
{
	char meno[20];
	char priezvisko[20];
	float rozpocet;
	PRODUKT kupene_produkty[50];
} ZAKAZNIK;

ZAKAZNIK* zakaznik;

PRODUKT* produkty_nacitaj_zo_suboru(char* filename) // osetrene
{
	PRODUKT* produkty;
	FILE* subor;
	int i;

	subor = fopen(filename, "r"); // kontrola otvorenia suboru
	if (subor == NULL)
		return NULL;

	fscanf(subor, "%d", &pocet_produktov); // nacitanie poctu produktov

	produkty = malloc(sizeof(PRODUKT) * pocet_produktov); // alokacia pamate

	for (i = 0; i < pocet_produktov; i++)
	{
		fscanf(subor, "%d %s %s %d %f", &produkty[i].ID, &produkty[i].nazov, &produkty[i].vyrobca, &produkty[i].pocet_kusov, &produkty[i].cena);
	}

	fclose(subor);
	return produkty;
}

ZAKAZNIK* registruj_zakaznika() // osetrene
{
	ZAKAZNIK* zakaznik;

	zakaznik = malloc(sizeof(ZAKAZNIK));

	printf("Zadaj meno zakaznika:\n");
	scanf("%s", &zakaznik->meno);

	printf("Zadaj priezvisko zakaznika:\n");
	scanf("%s", &zakaznik->priezvisko);

	printf("Zadaj rozpocet zakaznika:\n");
	scanf("%f", &zakaznik->rozpocet);

	return zakaznik;
}

void main_page();

void produkt_vyber_podla_ID(int vybrane_ID) // osetrene
{
	int kupit;

	if (produkty[vybrane_ID - 1].pocet_kusov < 1) // produkt NIE JE na skladke
	{
		printf("Vybrany produkt '%s' momentalne nie je na sklade\n", produkty[vybrane_ID - 1].nazov);
		main_page();
	}
	else // produkt JE na sklade
	{
		printf("Vybrany produkt '%s' je na sklade, cena: %.2f\n", produkty[vybrane_ID - 1].nazov, produkty[vybrane_ID - 1].cena);
		printf("\nZadajte:\n1 - kupit vybrany produkt\n0 - nekupit vybrany produkt\n");
		scanf("%d", &kupit);

		if (kupit == 1)
		{
			if (zakaznik->rozpocet > produkty[vybrane_ID - 1].cena)
			{
				puts("Kupene");

				zakaznik->rozpocet -= produkty[vybrane_ID - 1].cena;
				strcpy(zakaznik->kupene_produkty[pocet_kupenych_produktov].nazov, produkty[vybrane_ID - 1].nazov); // zapisanie nazvu
				strcpy(zakaznik->kupene_produkty[pocet_kupenych_produktov].vyrobca, produkty[vybrane_ID - 1].vyrobca); // zapisanie vyrobcu
				zakaznik->kupene_produkty[pocet_kupenych_produktov].cena = produkty[vybrane_ID - 1].cena;

				pocet_kupenych_produktov++; // index pre pole kupenych produktov
				minute_peniaze += produkty[vybrane_ID - 1].cena; // celkova minuta suma penazi
				produkty[vybrane_ID - 1].pocet_kusov--;

				printf("Zostavajuci rozpocet: %.2f EUR\n", zakaznik->rozpocet);

				if (produkty[vybrane_ID - 1].pocet_kusov < 0) // kontrola, aby nebol zaporny pocet kusov
					produkty[vybrane_ID - 1].pocet_kusov = 0;
			}
			else
				puts("Nedostatok penazi");
		}
		else if (kupit == 0)
			puts("Nekupene");
		else
			puts("Zle zadana volba");
	}

	printf("\n");
	main_page();
}

void produkty_vypis_podla_nazvu(char* hladany_vyraz) // osetrene
{
	int i, najdene_produkty = 0, vybrane_ID;

	printf("\nVysledky pre hladany vyraz '%s':\n", hladany_vyraz);

	for (i = 0; i < pocet_produktov; i++)
	{
		if (strstr(produkty[i].nazov, _strlwr(hladany_vyraz)) != NULL)
		{
			printf("Vyrobca: %s, Nazov produktu: %s, ID: %d\n", produkty[i].vyrobca, produkty[i].nazov, produkty[i].ID);
			najdene_produkty++;
		}
	}

	if (najdene_produkty > 0)
	{
		printf("\nVyberte si produkt podla jeho ID\n");
		scanf("%d", &vybrane_ID);

		if (vybrane_ID > 0 && vybrane_ID <= pocet_produktov)
			produkt_vyber_podla_ID(vybrane_ID);
		else
		{
			puts("Zle zadane ID produktu");
			printf("\n");
			main_page();
		}
	}
	else
	{
		puts("Pod hladanym nazvom sa nic nenaslo");
		printf("\n");
		main_page();
	}
}

void produkty_vypis_podla_vyrobcu(char* hladany_vyraz) // osetrene
{
	int i, najdene_produkty = 0, vybrane_ID;

	printf("\nVysledky pre hladanie podla vyrobcu '%s':\n", hladany_vyraz);

	for (i = 0; i < pocet_produktov; i++)
	{
		if (strstr(produkty[i].vyrobca, _strupr(hladany_vyraz)) != NULL)
		{
			printf("Vyrobca: %s, Nazov produktu: %s, ID: %d\n", produkty[i].vyrobca, produkty[i].nazov, produkty[i].ID);
			najdene_produkty++;
		}
	}

	if (najdene_produkty > 0)
	{
		printf("\nVyberte si produkt podla jeho ID\n");
		scanf("%d", &vybrane_ID);

		if (vybrane_ID > 0 && vybrane_ID <= pocet_produktov)
			produkt_vyber_podla_ID(vybrane_ID);
		else
		{
			puts("Zle zadane ID produktu");
			printf("\n");
			main_page();
		}
	}
	else
	{
		puts("Pod hladanym nazvom sa nic nenaslo");
		printf("\n");
		main_page();
	}
}

void main_page()
{
	FILE* blocik;
	int volba = 0, i;
	char hladany_vyraz[20] = "";

	do
	{
		printf("\nVyberte moznost:\n1 -> pre hladanie podla nazvu produktu\n2 -> hladanie podla vyrobcu\n3 -> pre ukoncenie nakupu\n");
		scanf("%d", &volba);
		if (volba > 3 || volba < 1)
			printf("Zle zadana moznost\n");

	} while (volba > 3 || volba < 1);

	if (volba == 1)
	{
		puts("Hladat podla:");
		scanf("%s", hladany_vyraz);

		produkty_vypis_podla_nazvu(hladany_vyraz);
	}
	else if (volba == 2)
	{
		puts("Hladat podla:");
		scanf("%s", hladany_vyraz);

		produkty_vypis_podla_vyrobcu(hladany_vyraz);
	}
	else if (volba == 3)
	{
		printf("\nKoniec nakupu\n");

		printf("Minute peniaze: %.2f EUR\n", minute_peniaze);

		blocik = fopen("blocik_od_nakupu.txt", "w"); // vytvorenie "blociku" od nakupu, este sa pohrat s jeho formatom

		fprintf(blocik, "E-shop Obchodik\nUlica, Mesto, Krajina\nDatum a cas nakupu: 30/09/20 11:58\n");
		fprintf(blocik, "******************************\n");
		fprintf(blocik, "Zakaznik si zakupil nasledujuce polozky:\n");

		for (i = 0; i < pocet_kupenych_produktov; i++)
		{
			fprintf(blocik, "1x%s od %s ..... %.2f EUR\n", zakaznik->kupene_produkty[i].nazov, zakaznik->kupene_produkty[i].vyrobca, zakaznik->kupene_produkty[i].cena);
		}

		fprintf(blocik, "______________________________\n");
		fprintf(blocik, "Suma celkovo: %.2f EUR\n\nDakujeme za Vas nakup", minute_peniaze);

		fclose(blocik);

		free(produkty);
		free(zakaznik);

		exit(0);
	}
}

int main()
{
	int i;

	produkty = produkty_nacitaj_zo_suboru("produkty.txt");

	for (i = 0; i < pocet_produktov; i++)
		printf("%d %s %s %d %.2f\n", produkty[i].ID, produkty[i].nazov, produkty[i].vyrobca, produkty[i].pocet_kusov, produkty[i].cena);

	zakaznik = registruj_zakaznika();

	printf("\nZakaznik:\n-meno: %s\n-priezvisko: %s\n-rozpocet: %.2f EUR\n", zakaznik->meno, zakaznik->priezvisko, zakaznik->rozpocet);

	main_page();

	return 0;
}