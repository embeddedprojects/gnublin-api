#include "radio-functions.h"

void byebye(int s){
    string cmd;

    #ifdef DEBUG
    cout << "Terminate Programm" << endl;
    #endif
    cmd = "./player.sh stop";
    system(cmd.c_str());

    exit(0);
}


//check if give file already exist
bool fileExists(const string& filename)
{
    #ifdef DEBUG
    cout << "Ckeck if file: " << filename << " exist" << endl;
    #endif
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}


//Read Playlist in vector
vector<string> readPlaylist(string playlist){
    #ifdef DEBUG
    cout << "Read Playlist" << endl;
    #endif
	ifstream file;
	string fileline;
	vector<string> stations;

	file.open(playlist.c_str(), ifstream::in);

	//Read playlist
	while (!file.eof()){
		getline(file, fileline);
		stations.push_back(fileline);
	}
	stations.erase(stations.end()-1);	//Delete last field, whisch is always empty

	#ifdef DEBUG
	//Show station-Vektor
	for (int i=0; i<stations.size(); i++) {
		cout << i << ": " << stations[i] << endl;
	}
	#endif

	return stations;
}

//Replaces german Umlauts in string
int replaceUmlauts(string& value){

	int pos;
	do {
		pos = value.find("ö");
		if (pos != string::npos){
			value.replace(pos, 2, "oe");
		}
	} while (pos != string::npos);
	do {
		pos = value.find("Ö");
		if (pos != string::npos){
			value.replace(pos, 2, "Oe");
		}
	} while (pos != string::npos);
	do {
		pos = value.find("ü");
		if (pos != string::npos){
			value.replace(pos, 2, "ue");
		}
	} while (pos != string::npos);
	do {
		pos = value.find("Ü");
		if (pos != string::npos){
			value.replace(pos, 2, "Ue");
		}
	} while (pos != string::npos);
	do {
		pos = value.find("ä");
		if (pos != string::npos){
			value.replace(pos, 2, "ae");
		}
	} while (pos != string::npos);
	do {
		pos = value.find("Ä");
		if (pos != string::npos){
			value.replace(pos, 2, "Ae");
		}
	} while (pos != string::npos);

	return 0;
}


//Fill String up to spezific length with spaces
int fillStr(string& line, int len) {
	int size = line.size();

	for (size; size < len; size++){
		line.push_back(' ');
	}

	return 0;
}

//Write Volume in Menu
int lcdWriteMenu(int vol, Display& lcd){
    string vol_str;
    vol_str = numberToString(vol);
    fillStr(vol_str, 3);
    lcd.writeMenu(vol_str);

    return 0;
}

int lcdWriteStation(string station_str, Display& lcd, Id3& id3, int speed){
    id3.setStation(station_str);
    lcd.writeStation(id3, speed);

    return 0;
}

int lcdWriteTitle(string title_str, Display& lcd, Id3& id3, int speed){
    id3.setTitle(title_str);
    lcd.writeTitle(id3, speed);

    return 0;
}



/*###############################################################
			Player Class
#################################################################*/

//Start Player
Player::Player(string m_pipe, string mpg_log, string mpg_err){
	string cmd;
	volume = 0;

    mpg_pipe = m_pipe;

	//remove previous fifo-file
	#ifdef DEBUG
	cout << "Check fifo-file" << endl;
	#endif
	if (fileExists(mpg_pipe)){
		cmd = "rm " + string(mpg_pipe);
		system(cmd.c_str());
		#ifdef DEBUG
		cout << "pipe removed" <<endl;
		#endif
	}

	#ifdef DEBUG
	cout << "load player" << endl;
	#endif
	//Start mpg123
	//cmd = "mpg123 --fifo " + mpg_pipe + " -R > " + mpg_log + " 2> " + mpg_err + " &";
	if (fileExists("mpg.pid")){
        cmd = "./player.sh stop";
        system(cmd.c_str());
        #ifdef DEBUG
		cout << "Player Stoped" <<endl;
		#endif
	}
	cmd = "./player.sh start";
	system(cmd.c_str());
	sleep(1);

	pipe.open(mpg_pipe.c_str(), ofstream::out);	//Open Fifo-Pipe
}


int Player::loadStation(string& station, Id3& id3){
   	string cmd;

   	pipe.close();
    if (fileExists("mpg.pid")){
        cmd = "./player.sh stop";
        system(cmd.c_str());
        #ifdef DEBUG
		cout << "Player Stoped" <<endl;
		#endif
	}

	#ifdef DEBUG
	cout << "Get station" << endl;
	cout << "LOAD " << station << endl;
	#endif
	cmd = "./player.sh start";
	system(cmd.c_str());
	sleep(2);
	pipe.open("mpg_pipe", ofstream::out);   //Open Fifo-Pipe
	setVolume(volume);
	pipe << "LOAD " << station << endl;

	return 0;
}

int Player::setVolume(int vol){
    #ifdef DEBUG
    cout << "Set Volume to " << vol << endl;
    #endif
	volume = vol;
	pipe << "V " << volume << endl;

	return 0;
}

int Player::getVolume() const{
	return volume;
}

int Player::changeVolume(int vol){
	if ((volume + vol) > 100 || (volume + vol) < 0){
		return -1;
	}
	setVolume(volume + vol);

	return 0;
}


/*########################################################################
		ID3 Class
#########################################################################*/

Id3::Id3(string mpg_log){
    logFile = mpg_log;
	station = "GNUBLIN";
	title = "Webradio V0.1";
}

string Id3::getStation() const{
    return station;
}

int Id3::readStation(){
    string tmp, line;
    log.open(logFile.c_str(), ifstream::in);
    if (log.fail()){
        cout << "Error open File: " << logFile << endl;
        return -1;
    }
    while(!log.eof()){
        getline(log, line);
		if (line.compare(0, 11,"@I ICY-NAME") == 0){
			tmp = line.substr(13);
		}
    }
    replaceUmlauts(tmp);
    if (tmp == " ")
        return 0;
    fillStr(tmp, 14);
    #ifdef DEBUG
    cout << "Read Station: " << tmp << endl;
    #endif
    if (station != tmp){
		station = tmp;
    }
    log.close();
    return 0;
}


int Id3::setStation(string string){
    replaceUmlauts(string);
    fillStr(string, 14);
    station = string;
}

string Id3::getTitle() const{
    return title;
}

int Id3::readTitle(){
    string line, tmp;
    int len;

    log.open(logFile.c_str(), ifstream::in);
    if (log.fail()){
        cout << "Error open File: " << logFile << endl;
        return -1;
    }
    if (log.fail()){
        cout << "Error open File: " << logFile << endl;
        return -1;
    }
    while(!log.eof()){
		getline(log, line);
		if (line.compare(0,24, "@I ICY-META: StreamTitle") == 0){
			len = line.find("';")-26;
			tmp = line.substr(26,len);
		}
	}
    replaceUmlauts(tmp);
    fillStr(tmp, 20);

    #ifdef DEBUG
    cout << "Read Title" << tmp << endl;
    #endif
    if (title != tmp){
		title = tmp;
    }

    log.close();

    return 0;
}


int Id3::setTitle(string string){
    replaceUmlauts(string);
    fillStr(string, 20);
    title = string;
}

/*#####################################################################
            Display Class
#######################################################################*/

Display::Display(int addr){
	setAddress(addr);
	init();
	posStation = 0;
	posTitle = 0;
	iS = 0;
	iT = 0;
}

//clear a full display line
int Display::clearLine(int line){
    setcursor(line, 0);
	for (int i=0; i < 20; i++){
		string(" ");
	}
	setcursor(line, 0);

	return 0;
}

//write Button-menu in 4. line
int Display::writeMenu(std::string vol){
    setcursor(4,0);
	string("<");
	setcursor(4,4);
	string(">");
	setcursor(4,13);
	string("-");
	setcursor(4,15);
	string(vol.c_str());
	setcursor(4,19);
	string("+");

	return 0;
}

//Print Station-Title in 1. Line
int Display::writeStation(Id3& id3, int speed){
    #ifdef DEBUG
    cout << "Write Station to Display" << endl;
    #endif
    std::string tmp;
    if (iS == 3){
        id3.readStation();
        iS = 0;
    }
	setcursor(1,0);

	if (id3.getStation().size() > 14) {
		if (posStation >= id3.getStation().size()){
			posStation = 0;
		}
		tmp = id3.getStation().substr(posStation,14);
		fillStr(tmp, 14);
		posStation += speed;
		string(tmp.c_str());
	}
	else{
		string(id3.getStation().c_str());
	}
    iS++;

	return 0;
}

//Print Track-Title on 2. Line
int Display::writeTitle(Id3& id3, int speed){
    #ifdef DEBUG
    cout << "Write Title to Display" << endl;
    #endif
    std::string tmp;
    if (iT == 3){
        id3.readTitle();
        iT = 0;
    }
	setcursor(2,0);
	if (id3.getTitle().size() > 20) {
		if (posTitle >= id3.getTitle().size()){
			posTitle = 0;
		}
		tmp = id3.getTitle().substr(posTitle,20);
		fillStr(tmp, 20);
		posTitle += speed;
		string(tmp.c_str());
	}
	else{
		string(id3.getTitle().c_str());
	}
	iT++;
}

//write time in upper left corner
int Display::writeTime(){
    time_t t;
	tm *ts;
	stringstream ss;
	char tmp[6];

	t = time(NULL);
	ts = localtime(&t);

	strftime(tmp,6,"%H:%M", ts);

	setcursor(1,15);
	string(tmp);
}

//write Message in 3. Line
int Display::writeMessage(std::string msg){
    int n;
	int len = msg.size();

	if (msg.size() > 20){
	    msg = "Msg too long";
	}
	n = (20-msg.size()) / 2;        //set msg in center
    msg.insert(msg.begin(), n, ' ');
	fillStr(msg, 20);

	setcursor(3, 0);
	string(msg.c_str());

}
