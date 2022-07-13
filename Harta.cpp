#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>

#define MAX 100
#define infinity INT_MAX

using namespace std;

string message;


struct node
{
	int prev;
	int next;
	int vizitat;

};

int adj[MAX][MAX];
int n;
string jud[100];
string resedinta[100];

int findpath(int s, int d, int path[MAX], int* minDist) {
	node judet[MAX];
	int i, min, count = 0, current, distanta;
	*minDist = 0;
	//Se viziteaza toate nodurile din graf
	for (i = 1; i <= n; i++) {
		judet[i].prev = 0; //predecesorul nodului devine 0
		judet[i].next = infinity; //"pointerul" catre urmatorul element primeste valoarea maxima
		judet[i].vizitat = 0; //nodul se marcheaza ca nevizitat
	}

	//Nodul sursa se marcheaza ca vizitat
	judet[s].prev = 0;
	judet[s].next = 0;
	judet[s].vizitat = 1;

	//Se incepe cautarea de la nodul sursa pana se gaseste nodul destinatie
	current = s;
	while (current != d) {
		for (i = 1; i <= n; i++) //Se parcurg toate nodurile
		{
			//Se verifica daca exista o valoare in matricea de adiacenta intre nodul curent si nodul 'i'
			//si se verifica daca nodul 'i' a fost vizitat
			if (adj[current][i] > 0 && judet[i].vizitat == 0) {
				//Fiecarui nod i se creaza legatura cu urmatorul nod
				distanta = judet[current].next + adj[current][i];
				if (distanta < judet[i].next) {
					judet[i].prev = current;
					judet[i].next = distanta;
				}
			}
		}

		//Se cauta nodul cel mai apropiat ca sa devina noul nod curent
		min = infinity;
		current = 0;
		for (i = 1; i <= n; i++) {
			if (judet[i].vizitat == 0 && judet[i].next < min) {
				min = judet[i].next;
				current = i;
			}
		}

		if (current == 0) //Daca nu este niciun nod curent nou inseamna ca nodul sursa este izolat
			return 0;

		judet[current].vizitat = 1; //Nodul curent se marcheaza ca vizitat
	}

	//Se obtine drumul de la destinatie catre sursa
	while (current != 0) {
		count++;
		path[count] = current;
		current = judet[current].prev;
	}

	//Se calculeaza distanta parcursa (se aduna valorile din matricea de adiacenta)
	int u, v;
	for (i = count; i > 1; i--) {
		u = path[i];
		v = path[i - 1];
		*minDist += adj[u][v];
	}
	return (count); //Se returneaza numarul de "judete" din traseu
}

int case_insensitive_comparision(string s1, string s2) {
	//Se transforma string.urile primite ca parametrii in litere mici
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	if (s1.compare(s2) == 0)
		return 1; //Sirurile se aseamana
	return 0; //Nu se aseamana
}

void minimalPathDisplay(string n1, string n2) {
    message = "";
	int i, ok1 = 0, ok2 = 0;
	int source, dest;
	int path[MAX];
	int minDist, count;

	//Se cauta cele doua nume in lista de judete, respectiv resedinte, pentru a putea incepe cautarea de la pozitiile acestora
	for (i = 1; i <= n; i++)
		if (case_insensitive_comparision(n1, jud[i]) || case_insensitive_comparision(n1, resedinta[i])) { ok1 = 1; source = i; break; }

	for (i = 1; i <= n; i++)
		if (case_insensitive_comparision(n2, jud[i]) || case_insensitive_comparision(n2, resedinta[i])) { ok2 = 1; dest = i; break; }

	if (ok1 == 0 || ok2 == 0) //Se verifica daca cele doua siruri au fost introduse corect
		message += "Nu s-a introdus un judet corect! \n";
	else {
		//Se apeleaza functia findpath si se afiseaza lungimea traseului si
		//judetele prin care s-a trecut, in cazul in care distanta parcursa nu este nula
		count = findpath(source, dest, path, &minDist);
		if (minDist != 0) {
			message += "Traseul cel mai scurt este de ";
            message += std::to_string(minDist);
            message += " km\n";

			for (i = count; i > 1; i--)
				message += jud[path[i]] + "(" + resedinta[path[i]] + ")->";
			message += jud[path[i]] + "(" + resedinta[path[i]] + ")\n";

		}
	}

}

int main() {
	int i, j;
	string n1, n2;

	//Se citesc elementele din fisierele text
	fstream newfile;
	i = 0;
	newfile.open("Judete.txt", ios::in);
	if (newfile.is_open()) {   //Se verifica daca fisierul este deschis
		string s;
		while (getline(newfile, s)) {
			jud[i++] = s;
		}
		newfile.close();
	}

	i = 0;
	newfile.open("Resedinte.txt", ios::in);
	if (newfile.is_open()) {
		string s;
		while (getline(newfile, s)) {
			resedinta[i++] = s;
		}
		newfile.close();
	}

	ifstream f("Distante.txt");
	f >> n;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			f >> adj[i][j];


	//Se introduc locatiile sursa si destinatie pana programul este oprit de catre utilizator
	while (1) {
		cout << "Introduceti locatia de start : ";
		getline(cin, n1);
		cout << "Introduceti locatia destinatie : ";
		getline(cin, n2);

		minimalPathDisplay(n1, n2);
		cout<<message;
	}
}
