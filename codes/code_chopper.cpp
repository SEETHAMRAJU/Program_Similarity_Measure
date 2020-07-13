#include<bits/stdc++.h>
using namespace std;
/*
This is a basi implementation of the codechopper. 
There have to be many api required to actually implement one such code chopper to extract the syntax tree. 

*/


const int minStatements = 2;
int f = 0;



bool check(vector<string> chops){
	int count=0;
	for(auto x:chops){
		for(int i=0;i<x.size();i++){
			if(x[i]=='{')
				count++;
			else if(x[i]=='}')
				count--;
		}
	}
	if(count!=0)
		return false;
	return true;
}


bool fullof(string s,string p){
	int n = s.size();
	for(int i=0;i<n;i++){
		if(s[i]!=p[0])
			return false;
	}
	return true;
}

char extension[10];

void ext(char *x,int len){
	bool flag = false;int j=0;
	for(int i=0;i<len;i++){
		if(flag)
			extension[j]=x[i],j++;
		if(x[i]=='.')
			flag = true;
	}

}

void write_to_file(vector<string> x){
	char s[15];
	sprintf(s,"fragment_%d.%s",f++,extension);
	FILE *fp = fopen(s,"w");
	for(auto y:x){
		string temp = y+"\n";
		fputs(temp.c_str(),fp);
	}
	fclose(fp);
}
int char_to_num(char *g,int len){
	int r = 0;
	for(int i=0;i<len;i++)
		r= r*10+(g[i]-'0');
	return r;
}
int main (int argc, char* argv[]){
	if(argc<3){
		cout << "Usage\n./a.out <filename> <number of lines>"<<endl;
		return 0;
	}	
	ext(argv[1],strlen(argv[1]));
	int lines = char_to_num(argv[2],strlen(argv[2]));
	vector<string>chops;
	string s;
	ifstream file(argv[1]);
	int c=0;
	while(c<lines){
		getline(file,s);
		if(!fullof(s,"\t") and !fullof(s," "))
			chops.emplace_back(s);
		++c;
	}

	vector<vector<string>> fragments;
	for(int i=0;!chops.empty() || chops.size()>minStatements;i++){
		if(check(chops))
			fragments.push_back(chops);
		chops.erase(chops.begin());
	}
	cout << "Number of valid subsequences for the current implementation : "<<fragments.size()<<endl;
	int i=0;
	for(auto x:fragments){
		write_to_file(x);
		cout << "Saved fragment_"<<i++<<'.'<<extension<<endl;
	}
	return 0;


}