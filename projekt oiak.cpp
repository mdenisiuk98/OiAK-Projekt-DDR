// projekt oiak.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>


using namespace std;


void flushCache(){
	const	int bigger_than_cachesize = 96000;
		static long b[bigger_than_cachesize][600];
	 
			for(int i=1;i<600;i++){
				for (int j = bigger_than_cachesize-1; j >= 0; j--) {
					b[j][i] = b[i][j];
			}
		}
}

int main(){
	bool running = true;
// alokacja //////////////////////////////////////////////
static long long int t[24*524288][16];	


for(int i=0;i<(24*524288);i++){
	for(int j=0;j<16;j++){
		t[i][j]=4532;
	}
}
// glowna petla programu - przygotowanie do pomiarow //////////////////////////////////////////////////////

	while (running){ 

	int choice = 1;
	cout << "\nWybierz tryb testowania czasów dostępu (1-oba w sekwencji, 2- jeden w sekwencji, 0- wyjscie): ";
	cin >> choice;
	if (choice==0){
		running=false;
		break;
	}
	
	ofstream outputFile,meanOutput;
	string dir = "./result ";
	time_t pre = chrono::system_clock::to_time_t(chrono::system_clock::now());
	string fileSuffix = std::ctime(&pre);
	
	int columnBegin,rowBegin,columnEnd,rowEnd,rowCompare,columnCompare;

	outputFile.open(dir + fileSuffix, ofstream::out);
	meanOutput.open(dir  + fileSuffix+ "SREDNIE ", ofstream::out);
	cout << "\nTestowanie czasow dostepu do pamieci DDR\nPodaj rzad z ktorym wykonane bedzie porownanie: ";
	cin >> rowCompare;

	cout << "\nPodaj kolumne z ktora wykonane bedzie porownanie: ";
	cin >> columnCompare;

	cout << "\nPodaj rzad poczatku zakresu pomiaru: ";
	
	cin >> rowBegin;
	cout << "\nPodaj kolumne poczatku zakresu pomiaru: ";
	cin >> columnBegin;
	columnBegin = columnBegin%16;


	cout << "\nPodaj rzad konca zakresu pomiaru: ";
	cin >> rowEnd;

		cout << "\nPodaj kolumne konca zakresu pomiaru: ";
	cin >> columnEnd;
	columnEnd = columnEnd%16;

	int krok;
	cout << "\nPodaj krok z jakim wykonywac pomiary: ";
	cin >> krok;

	
	int testNum=0;
	cout << "\nPodaj z ilu testow zebrac srednie wyniki: ";
	cin >> testNum;

	long long times[testNum];
	long long results[testNum];
	long long measurement=0;
	
	long long timeMean=0;
	long long max = times[0];
	long long min = times [0];
	int blad=0;
	long long a=0;
	long long *end=&t[rowEnd][columnEnd];
	long long *pierwszy=&t[rowCompare][columnCompare]+a;		
	long long *drugi=&t[rowBegin][columnBegin]+a;
	long long baseOffset = drugi-pierwszy;
		
		while(drugi<=end){
				
				outputFile << "--- Test z pary " << pierwszy << " ([" << (pierwszy-&t[0][0])/16 << "][" <<(pierwszy-&t[0][0])%16 << "]) - " << drugi << " ([" << (drugi-&t[0][0])/16 << "][" <<(drugi-&t[0][0])%16 << "]) - przesuniecie " << drugi-pierwszy-baseOffset << " + bazowe " << baseOffset << endl;
				cout <<  "--- Test z pary " << pierwszy << " ([" << (pierwszy-&t[0][0])/16 << "][" <<(pierwszy-&t[0][0])%16 << "]) - " << drugi << " ([" << (drugi-&t[0][0])/16 << "][" <<(drugi-&t[0][0])%16 << "]) - przesuniecie " << drugi-pierwszy -baseOffset << " + bazowe " << baseOffset << endl;
					
					// przygotowanie pomiaru odczytu ///////////////////////

				timeMean=0;
				min = 0;
				max = 0;
				outputFile << "ODCZYT\n\n";
				cout << "\n--- ODCZYT ---\n";
				for (int i=0;i<testNum;i++){
					measurement=0;
					times[i]=0;
					flushCache();
				// ponowny dostep do wskaznikow //////////////////////////
					pierwszy=pierwszy;
					drugi=drugi;
				// testowana sekwencja ///////////////////////////////////////////////
					if (choice==2){
						asm volatile(
						
							"mov %%rax,%%r12\n"			//wskaznik na drugi element	
							"mov %%rdx,%%r13\n"			//wskaznik na pierwszy element	
							"mov 0(%%r13),%%r14\n"		// dostep do pierwszej wartosci przed mierzona sekwencja
							"rdtscp\n"
							"mov %%rax, %%r14\n"				// przechowanie wynikow rdtsc
							"mov %%rdx, %%r15\n"
							"movq    0(%%r12),%%rax\n"			//operacja odczytu jednego adresu	
							"rdtscp\n"							//koniec pomiaru
							"shl $32,%%r15\n"					//składanie wyników i wyliczenie rónicy
							"or %%r14,%%r15\n"
							"shl $32,%%rdx\n"
							"or %%rax,%%rdx\n"
							"sub %%r15,%%rdx\n"
							"mov %%rdx,%%rax\n"					//przekazanie wyników pomiaru do %rax
							: "=a" (measurement)				//wynik zwracany poprzez %rax
							: "r" (drugi),						//argumenty wejsciowe bloku kodu asm
							"r" (pierwszy)
							:"r12",								//clobberowanie rejestrow i pamieci
							"r13",
							"r14",
							"r15",
							"memory"
						);
					}	
					if (choice==1){
						asm volatile(
						
							"mov %%rax,%%r12\n"			//wskaznik na drugi element	
							"mov %%rdx,%%r13\n"			//wskaznik na pierwszy element	
							"rdtscp\n"
							"mov %%rax, %%r14\n"				// przechowanie wynikow rdtsc
							"mov %%rdx, %%r15\n"
							"movq 0(%%r12),%%rdx\n"
							"movq 0(%%r13),%%rax\n"			//mierzymy odczyty obu adresów
							"rdtscp\n"							//koniec pomiaru
							"shl $32,%%r15\n"					//składanie wyników i wyliczenie rónicy
							"or %%r14,%%r15\n"
							"shl $32,%%rdx\n"
							"or %%rax,%%rdx\n"
							"sub %%r15,%%rdx\n"
							"mov %%rdx,%%rax\n"					//przekazanie wyników pomiaru do %rax
							: "=a" (measurement)				
							: "r" (drugi),
							"r" (pierwszy)
							:"r12",
							"r13",
							"r14",
							"r15",
							"memory"
						);
					}	
				// zapisanie wyniku pomiaru czasu /////////////////////////////////////
			
				times[i]=measurement;
				}
			

				timeMean=0;
				max = times[0];
				min = times [0];
				blad=0;
				
				// wyliczenie średniej i wykrycie błędów grubych
				for (int i=0;i<testNum;i++)
				{
					results[i]=times[i];
					if (times[i]>max){
						max=times[i];
					}
					if (times[i]<min){
						min=times[i];
					}
					timeMean+=times[i];
				}
				timeMean=timeMean/testNum;
				for (int i=0;i<testNum;i++)
				{
					if (times[i]>1.6*timeMean){
						timeMean=timeMean*(testNum-blad);
						timeMean-=times[i];
						blad++;
						timeMean=timeMean/(testNum-blad);
						times[i]=0;
					}
				}
				if (blad>0){
					max=0;
					for (int i=0;i<testNum;i++){
						if(times[i]>max){
							max=times[i];
						}
					}
				}

// wypisanie wynikow na wyjsciu standardowym i do plikow wyjsciowych

				for (int i=0;i<testNum;i++){

					cout << results[i] << " CK";
					outputFile << "["<< i << "] " << results[i];

					if(times[i]==0){
						cout << ", mozliwy blad gruby!";
						outputFile << " - mozliwy blad gruby!";
					}
					cout << endl;
					outputFile << endl;
				}

				cout << "\nCzas sredni: " << timeMean << " CK, najwyzszy czas: " << max << " CK, najnizszy czas: " << min << " CK\n\n";
				outputFile << "\nCzas sredni: " << timeMean << " CK, najwyzszy czas: " << max << " CK, najnizszy czas: " << min << " CK\n\n";
			
				cout << endl <<endl;
				meanOutput << drugi-pierwszy << "\t" << timeMean << "\t" << min << endl;

				drugi+=krok;
				}
		}
	
	cout << "Wcisnij dowolny klawisz aby opuscic program.";
	cin.ignore();
	cin.get();
    return 0;
}
