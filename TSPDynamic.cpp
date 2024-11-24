// TSPDynamic.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

using namespace std;
using namespace std::chrono;
const int MAXN = 3000;
bool odw[MAXN + 5];
//vector<vector<unsigned long long>> DistanceMatrix; //tablica [i][j] - macierz odleglosci miedzy miastami
#define para pair<unsigned long long,unsigned long long>

unsigned long long INF = ULLONG_MAX;//bardzo duza liczba

vector <para> ListGraph[MAXN]; // <nr_wierz, waga_krawedzi>
priority_queue <para> kolejka; // <-d[i],i>

//deklaracje pozostałych funkcji i zmiennych
int Cities; //Cities - ilosc miast
unsigned long long Distance; //Dlugosc drogi miedzy.
int TestNumber, seed;
int Check;//zmienna na sprawdzanie poprawnosci
int ErrorVar;
unsigned long long Result;

void ResetTimeTable(unsigned long long Time[]) {
    for (int i = 0; i < 2; i++) {
        Time[i] = 0;
    }
}
void InitializeMatrix(int Cities, vector<vector<unsigned long long>>& DistanceMatrix) {
    DistanceMatrix.resize(Cities, vector< unsigned long long>(Cities));
    for (int i = 0; i < Cities; i++)
    {
        for (int j = 0; j < Cities; j++)
        {
            if (i == j) {
                DistanceMatrix[i][j] = 0; //bo dla wierzcholka dorga do siebie jest 0
            }
            else {
                DistanceMatrix[i][j] = INF; //a do kazdego innego roboczo INF
            }
        }
    }
}


void PrintMatrix(vector<vector<unsigned long long>>& DistanceMatrix) {
    cout << "Macierz odleglosci miedzy miastami: " << endl;
    int n = DistanceMatrix.size();
    //wypisanie macierzy
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
                cout << left << setw(8) << setfill(' ') << DistanceMatrix[i][j];
        }
        cout << endl;
    }
}



void GenerateProblem(int Cities, int seed, vector<vector<unsigned long long>>& DistanceMatrix) {
    InitializeMatrix(Cities, DistanceMatrix);
    //generacja skierowanego
    srand(seed);
    for (int i = 0; i < Cities; i++) {
        for (int j = 0; j < Cities; j++) {
            if (i == j) {
                DistanceMatrix[i][j] = 0;
            }
            else {
                DistanceMatrix[i][j] = rand() % 100 + 1;
            }
        }
    }
}


bool CheckIfCorrect() {
        if (Result != Check) {
            return false;
        }
    return true;
}
int tsp(const vector<vector<unsigned long long>>& DistanceMatrix, int Cities, int s)
{
    vector<int> Visited;
    Visited.push_back(s); //wpychamy na wektor Visited startowy
    unsigned long long min_cost = INF;
    unsigned long long result = 0;
    while (Visited.size() < Cities) {
        min_cost = INF; //reinicjalizacja dla kazdego przejscia
        int savedCity = -1; //inicjalizacja zmiennej zachowujacej miasto
        for (int i = 0; i < Cities; i++) {
            if (i == s || find(Visited.begin(), Visited.end(), i) != Visited.end()) {
                //sprawdzamy 2 warunki: czy nie jestesmy w tym miescie juz; czy nie odwiedzilismy juz sprawdzanego miasta)
                continue;
            }
            unsigned long long current_value = DistanceMatrix[s][i];
            if (current_value < min_cost) {
                min_cost = current_value;
                savedCity = i;
            }
        }
        if (savedCity != -1) {
            Visited.push_back(savedCity);
            result += min_cost;
            s = savedCity;
        }
        else {
            break;
        }
    }
    //przejdziemy all miast to wracamy do startowego
    unsigned long long startpoint = DistanceMatrix[s][0];
    result += startpoint;
    return result;
}
    //to dziala ponizej ale to brute force raczej
    //vector<int> toVisit;
    //for (int i = 0; i < Cities; i++) {
    //    if (i != s) {
    //        toVisit.push_back(i);
    //    }
    //}

    //unsigned long long min_cost = INF;
    ////lecimy po permutacjach
    //do {
    //    unsigned long long current_cost = 0;
    //    int j = s;

    //    // sprawdz koszt sprawdzanej permutacji
    //    for (int i = 0; i < toVisit.size(); i++) {
    //        current_cost += DistanceMatrix[j][toVisit[i]];
    //        j = toVisit[i];
    //    }
    //    current_cost += DistanceMatrix[j][s]; // dolicz koszt powrotu do miasta startowego

    //    //Jezeli koszt tej permutacji jest nizszy niz poprzedniej to podmien
    //    min_cost = min(min_cost, current_cost);
    //} while (next_permutation(toVisit.begin(), toVisit.end())); // przelec przez wszystkie permutacje
    //return min_cost;
int main()
{
    vector<vector<unsigned long long>> DistanceMatrix;
    unsigned long long Time[2] = { 0 }; //tablica czasow, wyzerowana na start
    int Options = 0;
    while (Options != 7) {
        switch (Options) {
        case 1: //done wczytaj z pliku
        {
            string NameOfFile;
            cout << "Podaj nazwe pliku. Przykladowa nazawa obslugiwanego pliku: nazwa.txt" << endl;
            cin >> NameOfFile;
            ifstream GraphData(NameOfFile);
            if (!GraphData.is_open()) {
                cout << "Nie udalo sie otworzyc pliku. Sprobuj ponownie." << endl;
            }
            else {
                GraphData >> Cities >> Check;
                if (Cities > 3000) {
                    cout << "Niestety w podanym pliku wystepuje za duzo miast i nie jestem w stanie go obsluzyc." << endl;
                    cout << "Maksymalna liczba obslugiwanych miast to 3000" << endl;
                    GraphData.close();
                }
                else {
                    cout << "Liczba miast: " << Cities << " Wynik poprawny: " << Check << endl; //testowe wypisanie elementów
                    InitializeMatrix(Cities, DistanceMatrix);
                    for (int i = 0; i < Cities; i++) {
                            for (int j = 0; j < Cities; j++) {
                                GraphData >> Distance;
                                DistanceMatrix[i][j] = Distance;
                                if (i == j) {
                                    if (DistanceMatrix[i][j] != 0) {
                                        ErrorVar = 1;
                                        DistanceMatrix[i][j] = 0; //upewnienie sie, ze na przekatnych beda zawsze 0
                                    }
                                }
                            }
                    }
                        if (ErrorVar == 1) {
                            cout << "Wykryto blad w pliku testowym - odleglosc miasta wzgledem siebie =/=0" << endl;
                            cout << "Dokonano automatycznej naprawy" << endl;
                            ErrorVar = 0;
                        }
                    cout << "Dokonano poprawnego odczytu z pliku " << NameOfFile << endl;
                    GraphData.close();
                }
            }
           Options = 0;
           break;
        }
        case 2: //generuje tylko zmienic wartosci
        {
            cout << "Podaj liczbe miaste, ktora ma posiadac wygenerowany problem TSP  (Max 3000): " << endl;
            cin >> Cities;
            if (Cities > 3000 || Cities < 2) {
                cout << "Podaj liczbe z zakresu 2-3000" << endl;
            }
            else {
                GenerateProblem(Cities, time(NULL), DistanceMatrix);
            }
            Options = 0;
            break;
        }
        case 3://wypiszemacierz
            PrintMatrix(DistanceMatrix);
            Options = 0;
            break;
        case 4: //do one test
        {
            int s = 0;
            Result = tsp(DistanceMatrix, Cities, s);
            cout << Result << endl;
            Options = 0;
            break;
        }
        case 5: //sprawdzanie poprawnosci done
        {
            bool CheckVariable;
            CheckVariable = CheckIfCorrect(); //testowanie poprawnosci
            if (CheckVariable == false) {
                cout << "Algorytm zostal zle wykonany. Sprawdz poprawnosc implementacji algorytmu." << endl;
            }
            else {
                cout << "Potwierdzono poprawne dzialanie zaimplementowanego algorytmu TSP dla zadanych danych." << endl;
            }
            Options = 0;
            break;
        }
        case 6:
        {
            cout << "Witaj w czesci programu, ktora pozwoli na zautomatyzowanie testowania efektywnosci algorytmu Helda Carpa dla problemu TSP w zaleznosci od rozmiaru instancji" << endl;
            cout << "Za kazda iteracja petli testowej program wylosuje nowy zestaw miast do problemu TSP" << endl;
            cout << "Podaj liczbe miast problemu TSP (Max 3000)" << endl;
            cin >> Cities;
            if (Cities > 3000 || Cities < 2) {
                cout << "Podaj liczbe z zakresu 2-3000" << endl;
            }

            else {
                cout << "Podaj liczbe testow" << endl;
                cin >> TestNumber;
                if (TestNumber <= 0) {
                    cout << "Podaj prawidlowa liczbe testw - wieksza od 0" << endl;
                }
                else {
                    //do tests
                }
            }

            Options = 0;
            break;
        }
        default: //dziala i wychodzi
            cout << endl << "Witaj w programie badajacym efektywnosc algorytmow grafowych" << endl;
            cout << "w zaleznosci od rozmiaru instancji i sposobu pamietania grafu." << endl;
            cout << "Wybierz jedna z dostępnych opcji: " << endl;
            cout << "1. Pobierz graf z pliku." << endl;
            cout << "2. Wygeneruj losowy graf." << endl;
            cout << "3. Wyswietl graf (listowo/macierzowo)." << endl;
            cout << "4. Wykonaj pojedyncze dzialanie algorytmu Djikstry." << endl;
            cout << "5. Przetestuj poprawnosc wykonanego algorytmu." << endl;
            cout << "6. Przeprowadz dowolna liczbe testow algorytmu Djikstry na losowo wygenerowanym grafie." << endl;
            cout << "7. Zakoncz dzialanie programu" << endl;
            cin >> Options;
            if (Options < 1 || Options > 7) {
                cout << "Wybierz poprawna opcje" << endl;
            }
        }
    }
}
