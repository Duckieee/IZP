/*
 * Iteracni vypocty - 2. projekt pro predmet Zaklady programovani
 * Soubor:	proj2.c
 * Datum:	19.11.2017
 * Autor:	Katerina Cibulcova, xcibul12, xcibul12@stud.fit.vutbr.cz
 * Popis:	Program provadi vypocet vzdalenosti a vysky mereneho objektu s pomoci funkci urcenych k vypoctu tangens.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define POCET 13 //pocet pouzitych koeficientu
#define ITERACE 10 //spravna iterace
#define MAX_UHEL 1.4 //maximalni povoleny uhel 
#define MIN_UHEL 0 //nepovoleny uhel
#define MAX_VYSKA 100 //maximalni povolena vyska
#define MIN_VYSKA 0 //nepovolena vyska
#define VYSKA_PRISTROJE 1.5 //implicitni vyska pristroje


void error_fce(char *zprava) //funkce pro chybovy vystup
{
	fprintf(stderr, "%s\nPro zobrazeni napovedy programu zadejte prikaz --help.\n", zprava);

}


double taylor_tan(double x, unsigned int n) // funkce pro vypocet tangens pomoci Taylorova polynomu
{
    long long pole_citatelu[POCET] = { 1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604 };
    long long pole_jmenovatelu[POCET] = { 1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375 };

    double taylor = 0.0;
    double mocnina = x;

    for (unsigned int i = 0; i < n; i++)
    {
            taylor += pole_citatelu[i] * mocnina / pole_jmenovatelu[i];
            mocnina *= x * x;
    }
    return taylor;
}


double cfrac_tan(double x, unsigned int n) //funkce pro vypocet tangens pomoci zretezenych zlomku
{
	double cfrac = 1.0;
	double b = x * x;
	
	for (int k = n; k; k--)
	{
		double a = (2 * k)  - 1;
		cfrac = a - (b / cfrac);
	}
	return (x / cfrac);
}

double my_abs(double prvni_cislo, double druhe_cislo) //funkce pro vypocet absolutni chyby
{
	if (prvni_cislo >= druhe_cislo)
	{
		return prvni_cislo - druhe_cislo;
	}
	else
	{
		return druhe_cislo - prvni_cislo;
	}
}

int my_comparison(double min_cislo, double x, double max_cislo) //funkce pro overeni, zda je dana hodnota ve spravnem rozsahu
{
	return (min_cislo < x && x <= max_cislo);

}

int main(int argc, char *argv[])
{
	double uhel_a;
	double uhel_b;
	int iterace_n;
	int iterace_m;
	double vyska_pristroje;
	double vzdalenost;
	double vyska;
	char *zbytek;


	if (argc == 2 && strcmp(argv[1], "--help") == 0) //kontrola prikazu --help, ktery zobrazuje napovedu
	{
		printf("Napoveda pouzivani programu:\n Pro napovedu zadejte prikaz --help.\n Pro porovnani presnosti vypoctu tangens daneho uhlu zadejte prikaz ve tvaru: --tan A N M, kde A je hodnota uhlu (v radianech) a hodnoty N a M udavaji, ve kterych iteracich vypoctu ma porovnani probihat (od nejmensi po nejvetsi). Maximalni hodnota uhlu muze byt 1.4 radianu a maximalni iterace muze byt 13. Zadna hodnota nesmi byt rovna nule.\n Pro vypis vzdalenosti mereneho objektu zadejte prikaz -m a hodnotu uhlu alfa(v radianech), pro vypis vysky zadejte i hodnotu uhlu beta (v radianech). Maximalni hodnota uhlu muze byt 1.4 radianu. Vychozi hodnota vysky pristroje je 1.5 metru, pro zmenu zadejte na zacatek prikazu -c a hodnotu vysky pristroje v metrech. Maximalni vyska pristroje muze byt 100 m. Zadna z hodnot se nesmi rovnat nule. \n");
	}
	else if (argc == 5 && strcmp(argv[1], "--tan") == 0) //kontrola prikazu --tan, ktery zobrazuje porovnani presnosti vypoctu
	{
		uhel_a = strtod(argv[2], &zbytek);
		iterace_n = atoi(argv[3]);
		iterace_m = atoi(argv[4]);

		if (strlen(zbytek)) //kontrola, zda jsou hodnoty v ciselnem formatu
		{
			error_fce("Zadavate chybny prikaz! Uhel musi byt v ciselnem formatu!");
			return -1;
		}
		else 
		{
			if (my_comparison(0, iterace_n, iterace_m) && iterace_m < 14) //kontrola, zda jsou zadane iterace ve spravnem rozsahu
			{


				for (int i = 1; i <= iterace_m; i++) //porovnani presnosti vypoctu mezi tan z knihovny a tan z funkci
				{
					if (i >= iterace_n)
					{
						printf("%d %e %e %e %e %e\n", i, tan(uhel_a), taylor_tan(uhel_a, i), my_abs(taylor_tan(uhel_a, i), tan(uhel_a)), cfrac_tan(uhel_a, i), my_abs(cfrac_tan(uhel_a, i), tan(uhel_a)));
					}
				}
			}
			else
       			{
           		 error_fce("Zadavate chybny prikaz!\nHodnoty N a M udavaji, ve kterych iteracich vypoctu ma porovnani probihat (od nejmensi po nejvetsi).\nPorovnani muze probihat minimalne v 1. iteraci a maximalne ve 13. iteraci.");
           		 return -1;
       		 	}
		}
	}	
	else if ((argc == 6 && strcmp(argv[1], "-c") == 0) && (strcmp(argv[3], "-m") == 0)) //kontrola prikazu -c a -m, ktery vypocita vzdalenost a vysku
	{
		vyska_pristroje = strtod(argv[2], &zbytek);
		uhel_a = strtod(argv[4], &zbytek);
		uhel_b = strtod(argv[5], &zbytek);

		if (strlen(zbytek))
		{
			error_fce("Zadavate chybny prikaz! Uhel/vyska musi byt v ciselnem formatu!");
			return -1;
		}
		else
		{
			if (my_comparison(MIN_VYSKA, vyska_pristroje, MAX_VYSKA) && my_comparison(MIN_UHEL, uhel_a, MAX_UHEL) && my_comparison(MIN_UHEL, uhel_b, MAX_UHEL)) //kontrola, zda jsou zadane hodnoty ve spravnem rozsahu
			{
				vzdalenost = vyska_pristroje / cfrac_tan(uhel_a, ITERACE);
				vyska = cfrac_tan(uhel_b, ITERACE) * vzdalenost + vyska_pristroje;

				printf("%.10e\n%.10e\n", vzdalenost, vyska);
			}
			else
			{
				error_fce("Zadavate chybny prikaz!\nHodnota uhlu musi byt vetsi nez 0 radianu a zaroven musi byt mensi nebo rovna hodnote 1.4 radianu.\nVyska mericiho pristroje musi byt vetsi nez 0 m a mensi nez 100 m.");
				return -1;
			}
		}
	}
	else if ((argc == 5 && strcmp(argv[1], "-c") == 0) && (strcmp(argv[3], "-m") == 0)) //kontrola prikazu -c a -m, ktery vypocita vzdalenost
	{
		vyska_pristroje = strtod(argv[2], &zbytek);
		uhel_a = strtod(argv[4], &zbytek);

		if (strlen(zbytek))
		{
			error_fce("Zadavate chybny prikaz! Uhel/vyska musi byt v ciselnem formatu!");
			return -1;
		}
		else
		{
			if (my_comparison(MIN_VYSKA, vyska_pristroje, MAX_VYSKA) && my_comparison(MIN_UHEL, uhel_a, MAX_UHEL)) //kontrola, zda jsou zadane hodnoty ve spravnem rozsahu
			{
				vzdalenost = vyska_pristroje / cfrac_tan(uhel_a, ITERACE);

				printf("%.10e\n", vzdalenost);
			}
			else
			{
				error_fce("Zadavate chybny prikaz!\nHodnota uhlu musi byt vetsi nez 0 radianu a zaroven musi byt mensi nebo rovna hodnote 1.4 radianu.\nVyska mericiho pristroje musi byt vetsi nez 0 m a mensi nez 100 m.");
				return -1;
			}
		}
	}
	else if ((argc == 4 && strcmp(argv[1], "-m") == 0)) //kontrola prikazu -m pro vypocet vzdalenosti a vysky
	{
		vyska_pristroje = VYSKA_PRISTROJE;
		uhel_a = strtod(argv[2], &zbytek);
		uhel_b = strtod(argv[3], &zbytek);

		if (strlen(zbytek))
		{
			error_fce("Zadavate chybny prikaz! Uhel musi byt v ciselnem formatu!");
			return -1;
		}
		else
		{

			if (my_comparison(MIN_UHEL, uhel_a, MAX_UHEL) && my_comparison(MIN_UHEL, uhel_b, MAX_UHEL)) //kontrola, zda jsou zadane hodnoty ve spravnem rozsahu
			{
				vzdalenost = vyska_pristroje / cfrac_tan(uhel_a, ITERACE);
				vyska = cfrac_tan(uhel_b, ITERACE) * vzdalenost + vyska_pristroje;

				printf("%.10e\n%.10e\n", vzdalenost, vyska);
			}
			else
			{
				error_fce("Zadavate chybny prikaz!\nHodnota uhlu musi byt vetsi nez 0 radianu a zaroven musi byt mensi nebo rovna hodnote 1.4 radianu.");
				return -1;
			}
		}
	}
	else if ((argc == 3 && strcmp(argv[1], "-m") == 0)) //kontrola prikazu -m pro vypocet vzdalenosti
	{
		vyska_pristroje = VYSKA_PRISTROJE;
		uhel_a = strtod(argv[2], &zbytek);

		if (strlen(zbytek))
		{
			error_fce("Zadavate chybny prikaz! Uhel  musi byt v ciselnem formatu!");
			return -1;
		}
		else
		{

			if (my_comparison(MIN_UHEL, uhel_a, MAX_UHEL))
			{
				vzdalenost = vyska_pristroje / cfrac_tan(uhel_a, ITERACE);

				printf("%.10e\n", vzdalenost);
			}
			else
			{
				error_fce("Zadavate chybny prikaz!\n Hodnota uhlu musi byt vetsi nez 0 radianu a zaroven musi byt mensi nebo rovna hodnote 1.4 radianu.\n");
				return -1;
			}
		}
    }
	else 
	{
		error_fce("Zadavate chybny prikaz! Nespravny pocet argumentu/nespravne zadane argumenty.");
		return -1;
	}

	return 0;
}
