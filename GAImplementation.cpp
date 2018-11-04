#include <math.h>
#include <time.h>
#include<bits/stdc++.h>
#include<unordered_map>
#include <fstream>
#include <limits.h>
#define ll long long
#define POP 100
#define WEIGHT 60
#define FEA 20
#define TERMINATION 85
#define TOURNAMENT 20 
#define CROSSRATIO 10
#define GENLIMIT 150
#define STEADY 1
#define GEN 0
#define MUTRATIO 10
using namespace std;


// Structures

// item structure
struct item{
	int w,vUB,vLB,vA;
}listPop[20];

// chromosome
struct chromosomes{
	vector<int> chromosome;
	int fitness;	
	int weight;
	int gen;
	chromosomes():chromosome(20,0),fitness(0),weight(0),gen(0){}
};

// roulette structure
class roulette{
	unordered_map <int,int> m;
	unordered_map <int,vector<chromosomes> > mapping;
public:
	void wrapperChromosome(chromosomes &c){
		if(m.find(c.fitness)==m.end()){
			addChromosome(c);
			m[c.fitness]=1;
			return;
		}
		else {
			addChromosome(c);
			incr(c.fitness);
		}
	}
	
	void addChromosome(chromosomes &c){
		mapping[c.fitness].push_back(c);
	}

	void incr(int key){
		m[key]++;
	}


	pair<chromosomes,chromosomes> parents(){
		int size = m.size(), i = 0, flag = 0, f1, f2;
		int random1 = rand()%POP ,random2 = rand()%POP;

		// roulette wheel pointer creation
		unordered_map <int,int> :: iterator prev = m.begin();
		for(unordered_map <int,int> :: iterator it = m.begin(); it!=m.end(); ++it){
			if(flag==2)
				break;
			i+=it->second;
			if(i>=random1 && !flag){
				f1 = prev->first;
				flag++;
			}
			if(i>=random2){
				f2 = prev->first;
				flag++;
			}
			prev = it;
		}

		// roulette wheel selection
		pair<chromosomes,chromosomes> p;
		flag = 0;
		// select parents
		for(unordered_map <int,vector<chromosomes> > :: iterator it = mapping.begin(); it!=mapping.end(); ++it){
			if(flag==2)
				break;
			if(it->first==f1){
				int r = rand()%it->second.size();
				p.first = it->second[r];
				flag++;
			}
			if(it->first==f2){
				int r = rand()%it->second.size();
				p.second = it->second[r];
				flag++;
			}
		}
		return p;
	}

	// to check state of parent and offspring
	void display(){
		for(unordered_map <int,vector<chromosomes> > :: iterator it = mapping.begin(); it!=mapping.end(); ++it){
			// cout << "key: "<<it->first<<" ";
			// for(int i = 0; i < it->second.size(); ++i){
			// 	cout <<" vector: "<<i+1 <<" ";
			// 	for(int j =0; j < it->second[i].chromosome.size(); ++j){
			// 		cout<< it->second[i].chromosome[j]<<" ";
			// 	}
			// 	cout<<endl;
			// }
			cout<<" fitness: "<< it->first<<" quantity: "<<m[it->first]<<endl;
		}
	}

};


// DEVELOPMENT FUNCTIONS

// to display chromosomes and to their state
void display(vector<chromosomes> &c){ 
	for(int i = 0; i < c.size(); ++i){
		for(int j = 0; j < FEA; ++j){
			cout << c[i].chromosome[j]<<" ";
		}		
		cout<<endl<<"f: "<<c[i].fitness<<" w: "<<c[i].weight<<" g: "<<c[i].gen<<endl;
	}
}

// helper function to check if we have duplicate chromosomes
int checkDuplicate(vector<chromosomes> &c){ 
	int ctr=0;
	for(int i = 1; i < POP; ++i){
		int j = 0;
		for(j = 0; j < FEA; ++j){
			if(c[i-1].chromosome[j]!=c[i].chromosome[j])
				break;
		}
		if(j==FEA)
			ctr++;
	}
	return ctr;
}

// testing random generator
void randomGenerator(){ 
	for(int i = 0; i < 1000; ++i)
		cout<< rand() % 10 + 1<<endl;
}

// wrapper function which can test any function using function pointer
void functionTester(void (*testFunction)()){
	testFunction();
}


// HELPER FUNCTIONS

// comparator for sorting on basis of fitness
bool comparatorFitness(chromosomes a, chromosomes b){
	return a.fitness > b.fitness;
}

// comparator for sorting on basis of age
bool comparatorAge(chromosomes a, chromosomes b){
	return a.gen > b.gen;
}

// for randomly flipping a bit
bool coinToss(){
	return rand()%2==1;
}

// provides avg fitness of a generation for plotting avg graph
float plotterAvg(vector<chromosomes> &c){
	float  avg = 0;
	for(int i = 0; i < POP; ++i){
		avg += c[i].fitness;
	}
	avg/=POP;
	return avg;
}

// provides best fitness of a generation for plotting best graph
int plotterBest(vector<chromosomes> &c){
	int max = -1;
	for(int i = 0; i < POP; ++i){
		if(max < c[i].fitness)
			max = c[i].fitness;
	}
	return max;
}

// MAIN FUNCTIONS

// set item value and weight
void setItem(){ 
	listPop[0].w = 8; // 1 company dreams/a++/a+
	listPop[0].vUB = 100;
	listPop[0].vLB = 10;
	listPop[0].vA = 60;

	listPop[1].w = 8; // 2 company a/b
	listPop[1].vUB = 100;
	listPop[1].vLB = 8;
	listPop[1].vA = 60;
	
	listPop[2].w = 16;	// 3 company c/d
	listPop[2].vUB = 90;
	listPop[2].vLB = 10;
	listPop[2].vA = 30;
	 
	listPop[3].w = 16;	// 4 About company
	listPop[3].vUB = 90;
	listPop[3].vLB = 20;
	listPop[3].vA = 40;
	 
	listPop[4].w = 24;	 // 5 Company specific prep
	listPop[4].vUB = 80;
	listPop[4].vLB = 15;
	listPop[4].vA = 50;
	
	listPop[5].w = 8;  // 6 GFG
	listPop[5].vUB = 100;
	listPop[5].vLB = 50;
	listPop[5].vA = 40;
	
	listPop[6].w = 16; // 7 Codechef
	listPop[6].vUB = 80;
	listPop[6].vLB = 30;
	listPop[6].vA = 40;
	
	listPop[7].w = 16; // 8 Codeforces  
	listPop[7].vUB = 80;
	listPop[7].vLB = 25;
	listPop[7].vA = 30;
	 
	listPop[8].w = 16; // 9 Topcoder
	listPop[8].vUB = 75;
	listPop[8].vLB = 20;
	listPop[8].vA = 30;
	 
	listPop[9].w = 16; // 10 Hackerearth
	listPop[9].vUB = 60;
	listPop[9].vLB = 10;
	listPop[9].vA = 35;
	 
	listPop[10].w = 24; // 11 Hackerrank
	listPop[10].vUB = 65;
	listPop[10].vLB = 15;
	listPop[10].vA = 38;
	 
	listPop[11].w = 16; // 12 os
	listPop[11].vUB = 70;
	listPop[11].vLB = 30;
	listPop[11].vA = 40;
	 
	listPop[12].w = 8; // 13 nw
	listPop[12].vUB = 20;
	listPop[12].vLB = 100;
	listPop[12].vA = 40;
	 
	listPop[13].w = 8; // 14 dbms
	listPop[13].vUB = 15;
	listPop[13].vLB = 100;
	listPop[13].vA = 10;
	 
	listPop[14].w = 8; // 15 oops
	listPop[14].vUB = 10;
	listPop[14].vLB = 90;
	listPop[14].vA = 10;
	 
	listPop[15].w = 8; // 16 c/c++
	listPop[15].vUB = 15;
	listPop[15].vLB = 90;
	listPop[15].vA = 10;
	 
	listPop[16].w = 16; // 17 java
	listPop[16].vUB= 10;
	listPop[16].vLB = 85;
	listPop[16].vA = 30;
	
	listPop[17].w = 16; // 18 asked
	listPop[17].vUB = 20;
	listPop[17].vLB = 10;
	listPop[17].vA = 100;
	 
	listPop[18].w = 16; // 19 current
	listPop[18].vUB = 20;
	listPop[18].vLB = 12;
	listPop[18].vA = 100;
	 
	listPop[19].w = 16; // 20 resume
	listPop[19].vUB = 15;
	listPop[19].vLB = 15;
	listPop[19].vA = 95;
	 
}

// make intial population
void make_population(vector<chromosomes> &c){  
	for(int i = 0; i < POP; ++i){
		int w =  0,v=0;
		while(w<=WEIGHT){
			int random = rand()%FEA;
			if(c[i].chromosome[random]==0){
				w+= listPop[random].w;
				c[i].chromosome[random]=1;
				v+=(listPop[random].vUB * listPop[random].vLB * listPop[random].vA);
			}
		}
		c[i].weight = w;
		c[i].fitness = v;
	}
}

 // helper function to calculate fitness
void fitnessFunction(vector<chromosomes> &c){
	int ctr=0;
	for(int i = 0; i < POP; ++i){
		int v = 0, w =0,j =0, in = 0;
		while(j<FEA){
			for(; j < FEA && w<=WEIGHT; ++j){
				if(c[i].chromosome[j]){
					v+=(listPop[j].vUB * listPop[j].vLB * listPop[j].vA);
					w+=listPop[j].w;
					in = j;
				}
			}

			if(w>WEIGHT){
				v-=(listPop[in].vUB * listPop[in].vLB * listPop[in].vA);
				w-=listPop[in].w;
				c[i].chromosome[in]=0;
			}
		}
		c[i].fitness = v;
		c[i].weight = w;
	}
}


// selection operator - roulette
pair<chromosomes,chromosomes> selectionRoulette(vector<chromosomes> &c){ 

	// create roulette
	roulette r;

	// select parents
	int key;
	for(int i = 0; i < POP; ++i){
		r.wrapperChromosome(c[i]);
	}
	r.display();
	return r.parents();
}


// selection operator - tournament
pair<chromosomes,chromosomes> selectionTournament(vector<chromosomes> &c){ 
	int k = TOURNAMENT, random;
	vector<chromosomes> selected(k);
	for(int i = 0; i < k; ++i){
		random = rand()%POP;
		selected[i] = c[random];
	}
	sort(selected.begin(),selected.end(), comparatorFitness);
	pair<chromosomes,chromosomes> p;
	p.first = selected[0];
	p.second = selected[1];
	return p;
}


// crossover operator - one point
pair<chromosomes,chromosomes> crossoverOnePoint(pair<chromosomes,chromosomes> &p, int &gen){ 
	pair<chromosomes,chromosomes> children;
	int random = rand()%FEA;
	for(int i = 0; i < FEA; ++i){
		if(i>=random){
			children.first.chromosome[i]=p.second.chromosome[i];
			children.second.chromosome[i]=p.first.chromosome[i];
		} else {
			children.first.chromosome[i]=p.first.chromosome[i];
			children.second.chromosome[i]=p.second.chromosome[i];
		}
	}
	for(int i = 0; i < FEA; ++i){
		if(children.first.weight>WEIGHT){
			children.first.fitness = -1;
			break;
		}
		if(children.first.chromosome[i]){
			children.first.fitness+=(listPop[i].vUB * listPop[i].vLB * listPop[i].vA);
			children.first.weight+=listPop[i].w;
		}
	}
	if(children.first.weight>WEIGHT){
		children.first.fitness = -1;
	}
	for(int i = 0; i < FEA; ++i){
		if(children.second.weight>WEIGHT){
			children.second.fitness = -1;
			break;
		}
		if(children.second.chromosome[i]){
			children.second.fitness+=(listPop[i].vUB * listPop[i].vLB * listPop[i].vA);
			children.second.weight+=listPop[i].w;
		}
	}
	if(children.second.weight>WEIGHT){
		children.second.fitness = -1;
	}
	children.first.gen = children.second.gen = gen;
	return children;
}

// crossover operator - two point
pair<chromosomes,chromosomes> crossoverTwoPoint(pair<chromosomes,chromosomes> &p, int &gen){ 
	pair<chromosomes,chromosomes> children;
	int random1 = rand()%FEA;
	int random2 = rand()%FEA;
	if(random1>random2){
		int t = random1;
		random1 =random2;
		random2 = t;
	}
	for(int i = 0; i < FEA; ++i){
		if(i>=random1 && i<random2){
			children.first.chromosome[i]=p.second.chromosome[i];
			children.second.chromosome[i]=p.first.chromosome[i];
		} else {
			children.first.chromosome[i]=p.first.chromosome[i];
			children.second.chromosome[i]=p.second.chromosome[i];
		}
	}
	for(int i = 0; i < FEA; ++i){
		if(children.first.weight>WEIGHT){
			children.first.fitness = -1;
			break;
		}
		if(children.first.chromosome[i]){
			children.first.fitness+=(listPop[i].vUB * listPop[i].vLB * listPop[i].vA);
			children.first.weight+=listPop[i].w;
		}
	}
	if(children.first.weight>WEIGHT){
		children.first.fitness = -1;
	}
	for(int i = 0; i < FEA; ++i){
		if(children.second.weight>WEIGHT){
			children.second.fitness = -1;
			break;
		}
		if(children.second.chromosome[i]){
			children.second.fitness+=(listPop[i].vUB * listPop[i].vLB * listPop[i].vA);
			children.second.weight+=listPop[i].w;
		}
	}
	if(children.second.weight>WEIGHT){
		children.second.fitness = -1;
	}
	children.first.gen = children.second.gen = gen;
	return children;
}

// crossover operator - uniform
pair<chromosomes,chromosomes> crossoverUniform(pair<chromosomes,chromosomes> &p, int &gen){ 
	pair<chromosomes,chromosomes> children;
	for(int i = 0; i < FEA; ++i){
		if(coinToss()){
			children.first.chromosome[i]=p.second.chromosome[i];
			children.second.chromosome[i]=p.first.chromosome[i];
		} else {
			children.first.chromosome[i]=p.first.chromosome[i];
			children.second.chromosome[i]=p.second.chromosome[i];
		}
	}
	for(int i = 0; i < FEA; ++i){
		if(children.first.weight>WEIGHT){
			children.first.fitness = -1;
			break;
		}
		if(children.first.chromosome[i]){
			children.first.fitness+=(listPop[i].vUB * listPop[i].vLB * listPop[i].vA);
			children.first.weight+=listPop[i].w;
		}
	}
	if(children.first.weight>WEIGHT){
		children.first.fitness = -1;
	}
	for(int i = 0; i < FEA; ++i){
		if(children.second.weight>WEIGHT){
			children.second.fitness = -1;
			break;
		}
		if(children.second.chromosome[i]){
			children.second.fitness+=(listPop[i].vUB * listPop[i].vLB * listPop[i].vA);
			children.second.weight+=listPop[i].w;
		}
	}
	if(children.second.weight>WEIGHT){
		children.second.fitness = -1;
	}

	children.first.gen = children.second.gen = gen;
	return children;
}

// mutation operator - single bit
void mutateSingleBit(pair<chromosomes,chromosomes> &p){ 
	int random = rand() % 1000 + 1;
	if(random<=MUTRATIO*10){
		random = random%FEA; 
		p.first.chromosome[random]^=1;
		if(p.first.chromosome[random]==1){
			if((p.first.weight+listPop[random].w)>WEIGHT)
				p.first.chromosome[random]=0;	
			else{
				p.first.fitness+=(listPop[random].vUB * listPop[random].vLB * listPop[random].vA);
				p.first.weight+=listPop[random].w;
			}
		} else{
			p.first.fitness-= (listPop[random].vUB * listPop[random].vLB * listPop[random].vA);
			p.first.weight-=listPop[random].w;
		}
		p.second.chromosome[random]^=1;
		if(p.second.chromosome[random]==1){
			if((p.second.weight+listPop[random].w)>WEIGHT)
				p.second.chromosome[random]=0;	
			else{
				p.second.fitness+=(listPop[random].vUB * listPop[random].vLB * listPop[random].vA);
				p.second.weight+=listPop[random].w;
			}
		} else{
			p.second.fitness-= (listPop[random].vUB * listPop[random].vLB * listPop[random].vA);
			p.second.weight-=listPop[random].w;
		}
	}
}

// wrapper function for three steps - selecting parent, cross-over, mutation
vector<chromosomes> scmWrapper(vector<chromosomes> &c, int &gen){
	
	vector<chromosomes> children;

	// choice between steady state and genearation cross over
	int crossover = STEADY ? CROSSRATIO :  POP;

	for(int i = 0; i < crossover/2; ++i){
		//select()
		pair<chromosomes,chromosomes> parent = selectionRoulette(c);
		
		//crossover() 
		pair<chromosomes,chromosomes> child = crossoverTwoPoint(parent,gen);
		
		//mutate()
		mutateSingleBit(child);
		if(child.first.fitness>0)
			children.push_back(child.first);
		if(child.second.fitness>0)
			children.push_back(child.second);
	}
	return children;
}

// survivor selection - age
void survivorSelectionAge(vector<chromosomes> &c, vector<chromosomes> &p){ 

	// sort parent array according to fitness
	sort(c.begin(), c.end(),comparatorAge);

	// replace
	for(int i = POP-p.size(), k=0; i< POP; ++i,k++){ 
		c[i] = p[k];
	}	

}

// survivor selection - fitness
void survivorSelectionFitness(vector<chromosomes> &c, vector<chromosomes> &p){ 
	
	// sort parent array according to fitness
	sort(c.begin(), c.end(),comparatorFitness);

	// sort children array according to fitness
	sort(p.begin(), p.end(),comparatorFitness);

	// replace
	for(int i = POP-p.size(), k=0; i< POP; ++i,k++){ // remove last k of a generation
		if(p[k].fitness<c[i].fitness)
			continue;
		c[i] = p[k];
	}	

}

// terminating condition - generation number or iteration
bool terminateGenLimit(int &gen){
	return gen>GENLIMIT;
}

// terminating condition - population
bool terminatePop(vector<chromosomes> &c){ 
	unordered_map<int,int> m;
	int maxFitness = -1;
	for(int i = 0; i < POP; ++i){
		if(c[i].fitness>maxFitness)
			maxFitness = c[i].fitness;
		if(m.find(c[i].fitness)==m.end())
			m[c[i].fitness]=1;
		else
			m[c[i].fitness]++;
	}
	int max = 0, t;
	for(unordered_map<int,int> :: iterator it = m.begin(); it!=m.end(); ++it){
		if(it->second>max){
			max = it->second;
			t = it->first;
		}
	}
	return maxFitness==t?(max >=TERMINATION?true:false):false;
}

//  solution of given GA
int solutionGA(vector<chromosomes> &c){ 
	int res = 0;
	for(int i = 0; i < POP; ++i){
		if(res<c[i].fitness)
			res = c[i].fitness;
	}
	return res;
}



int main(){

	// declare chromosome 
	vector<chromosomes> c(POP);

	// set item value weight -- later with a db
	setItem();

	// make initial population
	make_population(c);

	// calculate its fitness
	fitnessFunction(c);

	// variable to be used
	int gen = 0;
	int best;
	float avg;

	// files for best and avg graph CSV
	ofstream fa("avg.csv");
	ofstream fb("best.csv");
	
	
	// main loop
	do{
		cout <<"Generation "<<gen+1<<endl<<endl<<endl;

		// remove later
		// sort according to fitness
		//sort(c.begin(), c.end(),comparator);
		
		// select-crossover-mutate wrapper
		vector<chromosomes> children = scmWrapper(c,gen);

		// survivor selection
		survivorSelectionFitness(c,children);

		//to plot average
		avg = plotterAvg(c);
		fa<< gen <<","<< avg <<endl;
		
		// to plot best
		best = plotterBest(c);
		fb<< gen <<","<< best <<endl;
		
		// generation iterator
		gen++;

		// display current generation
		display(c);

		// terminating condition

		// population
		// if(terminatePop(c))
		// 	break;

		// generation limit
		if(terminateGenLimit(gen))
			break;

	}while(true);

	// closing files
	fa.close();
	fb.close();
	
	// solution
	cout << "Solution is "<<solutionGA(c)<<"g: "<<gen-1<<endl;

	
	return 0;
}