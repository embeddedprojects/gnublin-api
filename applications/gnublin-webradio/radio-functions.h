#include "gnublin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <pthread.h>
#include <signal.h>

#define DEBUG

using namespace std;

void byebye(int);

bool fileExists(const string& filename);

vector<string> readPlaylist(string playlist);

int replaceUmlauts(string&);

int fillStr(string& line, int len);





class Id3{
	public:
	Id3(string);
	string getStation() const;
	int readStation();
	int setStation(string string);
	string getTitle() const;
	int readTitle();
	int setTitle(string string);


	private:
	string logFile;
	string station;
	string title;
	ifstream log;
};


class Player{
	public:
	Player(string, string, string);
	int setStation();
	int setVolume(int);
	int getVolume() const;
	int changeVolume(int);
	int loadStation(string&, Id3&);

	private:
	string mpg_pipe;
	int volume;
	ofstream pipe;
};

class Display : public gnublin_module_lcd{
    public:
        Display(int addr);
        int clearLine(int);
        int writeTime();
        int writeStation(Id3&, int);
        int writeTitle(Id3&, int);
        int writeMessage(std::string);
        int writeMenu(std::string);
    private:
        int posStation;
        int posTitle;
        int iT, iS;
};


int lcdWriteMenu(int, Display&);
int lcdWriteStation(string, Display&, Id3&, int);
int lcdWriteTitle(string, Display&, Id3&, int);

