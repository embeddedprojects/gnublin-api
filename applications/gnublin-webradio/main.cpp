#include "radio-functions.h"

#define START_VOLUME 	15
#define LCD_ADDR	0x20
#define MPG_LOG		"mpg.log"
#define MPG_PIPE	"mpg_pipe"
#define MPG_ERR		"mpg_err.log"
#define PLAYLIST	"playlist.m3u"
#define SCROLL_SPEED	3

void *buttons(void*);
void *print_display(void*);
Id3 id3(MPG_LOG);
Display lcd(LCD_ADDR);
Player player(MPG_PIPE, MPG_LOG, MPG_ERR);

pthread_mutex_t mutex;


int main(){

    //run byebye function on "CTRL+c"
	signal (SIGINT,byebye);

    pthread_mutex_init (&mutex, NULL);
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, buttons, NULL);
	pthread_create(&thread2, NULL, print_display, NULL);
	pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

	return 0;
}

void *print_display(void*){
    pthread_mutex_lock (&mutex);

    lcd.writeTime();
    lcdWriteMenu(player.getVolume(), lcd);

    pthread_mutex_unlock (&mutex);
     while (1){
         pthread_mutex_lock (&mutex);
        lcd.writeStation(id3, SCROLL_SPEED);
        lcd.writeTitle(id3, SCROLL_SPEED);
        lcd.writeTime();
        lcdWriteMenu(player.getVolume(), lcd);
        lcd.clearLine(3);

        pthread_mutex_unlock (&mutex);
        sleep(1);
     }
     pthread_exit(0);
}

void *buttons(void*){
    vector<string> stations;
    int channel = 0;
    pthread_mutex_lock (&mutex);
    gnublin_module_pca9555 pca;
   	pthread_mutex_unlock (&mutex);

    stations = readPlaylist(PLAYLIST);

    player.setVolume(START_VOLUME);
    player.loadStation(stations[channel], id3);

    pthread_mutex_lock (&mutex);
    pca.pinMode(11, INPUT);
	pca.pinMode(12, INPUT);
	pca.pinMode(13, INPUT);
	pca.pinMode(14, INPUT);
	pca.pinMode(15, INPUT);
   	pthread_mutex_unlock (&mutex);



	pthread_mutex_unlock (&mutex);
	while(1){
	    pthread_mutex_lock (&mutex);
            //Station down
            if (pca.digitalRead(11) == 0){
                lcdWriteStation(string("Loading ..."), lcd, id3, SCROLL_SPEED);
                lcdWriteTitle(string(" "), lcd, id3, SCROLL_SPEED);
                pthread_mutex_unlock (&mutex);
                if (channel == 0){
                    channel = stations.size();
                }
                channel -= 1;

                #ifdef DEBUG
                cout << stations[channel] << endl;
                #endif

                player.loadStation(stations[channel], id3);
                usleep(500000);
            }
            //Station up
            else if (pca.digitalRead(12) == 0) {
                lcdWriteStation(string("Loading ..."), lcd, id3, SCROLL_SPEED);
                lcdWriteTitle(string(" "), lcd, id3, SCROLL_SPEED);

                pthread_mutex_unlock (&mutex);
                if (channel == stations.size() - 1){
                    channel = -1;
                }
                channel += 1;

                #ifdef DEBUG
                cout << stations[channel] << endl;
                #endif

                player.loadStation(stations[channel], id3);
                usleep(500000);
            }
            //Volume down
            else if (pca.digitalRead(14) == 0) {

                #ifdef DEBUG
                cout << "Volume -5" << endl;
                #endif
                lcd.writeMessage(string("Volume -5"));
                pthread_mutex_unlock (&mutex);


                player.changeVolume(-5);
            }
            //Volume up
            else if (pca.digitalRead(15) == 0) {

                #ifdef DEBUG
                cout << "Volume +5" << endl;
                #endif
                lcd.writeMessage(string("Volume +5"));
                lcdWriteMenu(player.getVolume(), lcd);
                pthread_mutex_unlock (&mutex);
                player.changeVolume(5);
            }
            else {
                pthread_mutex_unlock (&mutex);
            }
        usleep(50000);
	}
    pthread_exit(0);
}
