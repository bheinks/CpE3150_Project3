#include "reg932.h"
#include "uart.h"
#include "uart.c"

#define TEMPO       (OSC_FREQ/204800)
#define N_PAUSE     250 //(OSC_FREQ/29491)

#define E3          ((OSC_FREQ/4)/164.81)
#define F3          ((OSC_FREQ/4)/174.61)
#define G3          ((OSC_FREQ/4)/196.00)
#define Ab3         ((OSC_FREQ/4)/207.65)
#define A3          ((OSC_FREQ/4)/220.00)
#define Bb3         ((OSC_FREQ/4)/233.08)
#define B3          ((OSC_FREQ/4)/246.94)
#define C4          ((OSC_FREQ/4)/261.63)
#define Db4         ((OSC_FREQ/4)/277.18)
#define D4          ((OSC_FREQ/4)/293.67)
#define Eb4         ((OSC_FREQ/4)/311.13)
#define E4          ((OSC_FREQ/4)/329.63)
#define F4          ((OSC_FREQ/4)/349.23)
#define Gb4         ((OSC_FREQ/4)/369.99)
#define G4          ((OSC_FREQ/4)/392.00)
#define Ab4         ((OSC_FREQ/4)/415.30)
#define A4          ((OSC_FREQ/4)/440.00)
#define Bb4         ((OSC_FREQ/4)/466.16)
#define B4          ((OSC_FREQ/4)/493.88)
#define C5          ((OSC_FREQ/4)/523.25)
#define Db5         ((OSC_FREQ/4)/554.37)
#define D5          ((OSC_FREQ/4)/587.33)
#define Eb5         ((OSC_FREQ/4)/622.25)
#define E5          ((OSC_FREQ/4)/659.26)
#define F5          ((OSC_FREQ/4)/698.46)
#define Gb5         ((OSC_FREQ/4)/739.99)
#define G5          ((OSC_FREQ/4)/783.99)
#define Ab5         ((OSC_FREQ/4)/830.61)
#define A5          ((OSC_FREQ/4)/880.00)
#define Bb5         ((OSC_FREQ/4)/932.33)
#define B5          ((OSC_FREQ/4)/987.77)
#define C6          ((OSC_FREQ/4)/1046.5)
#define Db6         ((OSC_FREQ/4)/1108.7)
#define D6          ((OSC_FREQ/4)/1174.7)
#define Eb6         ((OSC_FREQ/4)/1244.5)
#define E6          ((OSC_FREQ/4)/1318.5)
#define F6          ((OSC_FREQ/4)/1396.9)
#define Gb6         ((OSC_FREQ/4)/1480.0)
#define G6          ((OSC_FREQ/4)/1568.0)
#define Ab6         ((OSC_FREQ/4)/1661.2)
#define A6          ((OSC_FREQ/4)/1760.0)
#define Bb6         ((OSC_FREQ/4)/1864.7)
#define B6          ((OSC_FREQ/4)/1975.5)
#define C7          ((OSC_FREQ/4)/2093.0)
#define D7          ((OSC_FREQ/4)/2349.3)

// define SFRs
sbit SPKR = P1^7;
sbit LED1 = P2^4;
sbit LED2 = P0^5;
sbit LED3 = P2^7;
sbit LED4 = P0^6;
sbit LED5 = P1^6;
sbit LED6 = P0^4;
sbit LED7 = P2^5;
sbit LED8 = P0^7;
sbit LED9 = P2^6;

sbit SW4 = P2^1;
sbit SW5 = P0^3;
sbit SW6 = P2^2;

bit MUTED = 0;

// enum for program state control
enum MODES {
    debug,
    tune1,
    tune2,
    keyboard,
    mike,
    brett,
    bret,
    NUM_MODES
};
enum MODES mode;

typedef struct Song {
    unsigned char* title;
    unsigned char length;
    unsigned char* notes;
    unsigned char* durations;
} Song;

code int period[] = {
    0,      // 0
    E3,     // 1
    F3,     // 2
    G3,     // 3
    Ab3,    // 4
    A3,     // 5
    Bb3,    // 6
    B3,     // 7
    C4,     // 8
    Db4,    // 9
    D4,     // 10
    Eb4,    // 11
    E4,     // 12
    F4,     // 13
    Gb4,    // 14
    G4,     // 15
    Ab4,    // 16
    A4,     // 17
    Bb4,    // 18
    B4,     // 19
    C5,     // 20
    Db5,    // 21
    D5,     // 22
    Eb5,    // 23
    E5,     // 24
    F5,     // 25
    Gb5,    // 26
    G5,     // 27
    Ab5,    // 28
    A5,     // 29
    Bb5,    // 30
    B5,     // 31
    C6,     // 32
    Db6,    // 33
    D6,     // 34
    Eb6,    // 35
    E6,     // 36
    F6,     // 37
    Gb6,    // 38
    G6,     // 39
    Ab6,    // 40
    A6,     // 41
    Bb6,    // 42
    B6,     // 43
    C7,     // 44
    D7      // 45
};

code unsigned char Super_Mario[][154] = {
    {
        //E5,E5,E5,C5,E5,G5,G4,C5,G4,E4,A4,B4,Bb4,A4,G4,E5,G5,A5,
        //F5,G5,E5,C5,D5,B4,C5,G4,E4,A4,B4,Bb4,A4,G4,E5,G5,A5,F5,G5,
        //E5,C5,D5,B4,G5,Gg5,F5,D5,E5,Aa5,A4,C5,A4,C5,D5,G5,Gg5,F5,D5,E5,
        //C6,C6,C6,G5,Gg5,F5,D5,E5,Aa5,A4,C5,A4,C5,D5,Ee5,D5,C5,C5,C5,C5,
        //C5,D5,E5,C5,A4,G4,C5,C5,C5,C5,D5,E5,C5,C5,C5,C5,D5,E5,C5,
        //A4,G4,E5,E5,E5,C5,E5,G5,G4,E5,C5,G,Aa5,A4,F5,F5,A4,B4,A5,A5,A5,
        //G5,F5,E5,C5,A4,G4,E5,C5,G4,Aa5,A4,F5,F5,A4,B4,F5,F5,F5,E5,D5,C5,C5,
        //G4,E4,A4,B4,A4,Aa5,Bb4,Aa5,E4,D4,E4
        24,24,24,20,24,27,0,15,0,20,15,12,17,19,18,17,15,24,27,29,//20
        25,27,24,20,22,19,20,15,12,17,19,18,17,15,24,27,29,25,27,//39
        24,20,22,19,27,26,25,22,24,16,17,20,17,20,22,27,26,25,22,24,
        32,32,32,0,27,26,25,22,24,16,17,20,17,20,22,23,22,20,0,20,20,20,
        20,22,24,20,17,15,20,20,20,20,22,24,20,20,20,20,22,24,20,
        17,15,24,24,24,20,24,27,15,24,20,27,28,17,25,25,17,19,29,29,29,
        27,25,24,20,17,15,24,20,15,28,17,25,25,17,19,25,25,25,24,22,20,20,
        15,12,17,19,17,28,18,28,12,10,12
    },
    {
        8,8,16,8,16,16,16,16,16,24,24,24,8,16,8,24,8,16,8,16,//20
        8,16,16,8,8,24,24,24,24,8,16,8,24,8,16,8,16,8,16,//39
        16,8,8,24,8,8,8,16,16,8,8,16,8,8,24,8,8,8,16,16,//59
        16,8,16,16,8,8,8,16,16,8,8,16,8,8,24,24,24,24,24//77
    }
};

code unsigned char old_macdonald[][62] = {
	{
        20,20,20,15,17,17,15,24,24,22,22,20,0,15,20,20,20,15,
        17,17,15,24,24,22,22,20,0,15,15,20,20,20,15,15,20,20,
        20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,15,
        17,17,15,24,24,22,22,20,0
	},
	{
        16,16,16,16,16,16,32,16,16,16,16,32,16,16,16,16,16,16,
        16,16,32,16,16,16,16,32,16,8,8,16,16,16,8,8,16,16,32,8,
        8,16,8,8,16,8,8,8,8,16,16,16,16,16,16,16,16,32,16,16,16,16,
        64,16
	}
	
};

code unsigned char gerudo_valley[][73] = {
    {
        0,21,26,28,29,21,26,28,29,0,22,26,28,29,22,26,28,29,
        0,19,24,26,28,19,24,26,28,0,26,28,26,25,0,21,29,28,26,21,
        24,24,26,24,22,0,19,28,26,24,22,21,22,24,22,21,0,21,29,28,
        26,21,24,24,26,24,22,22,19,26,24,22,21,0
    },
    {
        4,4,4,4,16,4,4,4,24,4,4,4,4,16,4,4,4,24,4,4,4,4,16,4,4,
        4,24,4,4,4,4,48,8,12,12,12,12,8,16,4,4,4,38,8,12,12,
        12,12,8,16,4,4,4,38,8,12,12,12,12,8,12,4,4,4,28,8,
        24,8,24,8,64,16
    }
};

code unsigned char camptown_races[][48] = {
    {
        27, 27, 24, 27, 29, 27, 24, 24, 22, 24, 22, 27, 27, 24, 27, 29, 27, 24, //18
        22, 24, 25, 24, 22, 20, 20, 20, 24, 27, 32, 29, 29, 32, 29, 27,            //34
        27, 27, 24, 27, 29, 27, 24, 22, 24, 25, 24, 22, 20, 0
    },
    {
        16, 16, 16, 16, 16, 16, 32, 16, 32, 16, 32, 16, 16, 16, 16, 16, 16, 32,
        16, 8, 8, 16, 16, 64, 16, 16, 16, 16, 64, 16, 16, 16, 16, 64,
        16, 16, 16, 16, 16, 16, 32, 16, 8, 8, 16, 16, 64, 16
    }
};

code unsigned char great_fairy_fountain[][129] = {
    {
        //A6, D6, Bb5, G5, G6, D6, Bb5, G5, Gb6, D6, Bb5, G5, G6, D6, Bb5, G5
        41, 34, 30, 27, 39, 34, 30, 27, 38, 34, 30, 27, 39, 34, 30, 27,
        //G6, C6, A5, F5, F6, C6, A5, F5, E6, C6, A5, F5, F6, C6, A5, F5
        39, 32, 29, 25, 37, 32, 29, 25, 36, 32, 29, 25, 37, 32, 29, 25,
        //F6, Bb5, G5, E5, E6, Bb5, G5, E5, Eb6, Bb5, G5, E5, E6, Bb5, G5, E5
        37, 30, 27, 24, 36, 30, 27, 24, 35, 30, 27, 24, 36, 30, 27, 24,
        //E6, A5, F5, D5, D6, A5, F5, D5, Db6, A5, F5, D5, D6, A5, F5, D5
        36, 29, 25, 22, 34, 29, 25, 22, 33, 29, 25, 22, 34, 29, 25, 22,
        //A6, D6, Bb5, G5, G6, D6, Bb5, G5, Gb6, D6, Bb5, G5, G6, D6, Bb5, G5
        41, 34, 30, 27, 39, 34, 30, 27, 38, 34, 30, 27, 39, 34, 30, 27,
        //Bb6, Eb6, C6, A5, A6, Eb6, C6, A5, Ab6, Eb6, C6, A5, A6, Eb6, C6, A5
        42, 35, 32, 29, 41, 35, 32, 29, 40, 35, 32, 29, 41, 35, 32, 29,
        //C7, D6, Bb5, G5, Bb6, D6, Bb5, G5, A6, D6, Bb5, G5, Bb6, D6, Bb5, G5
        44, 34, 30, 27, 42, 34, 30, 27, 41, 34, 30, 27, 42, 34, 30, 27,
        //A6, Bb5, G5, E5, G6, Bb5, G5, E5, F6, Bb5, G5, E5, E6, Bb5, G5, E5
        41, 30, 27, 24, 39, 30, 27, 24, 37, 30, 27, 24, 36, 30, 27, 24, 0
    },
    {
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 16
    }
};

int* p_per;

// Interrupt Handler for Timer 1 (Generates sound)
void clk(void) interrupt 3 using 1 {
    TH1 = -(*p_per) >> 8;
    TL1 = -(*p_per) & 0x0ff;
    SPKR = ~SPKR;
}

void delay(int time) {
    unsigned char i;

    while(time-- > 0) {
        for(i = 0; i < N_PAUSE; i++);
    }
}

// play individual note
void play(unsigned char note, unsigned char duration) {
    int dur_time = TEMPO * duration; // TEMPO defines speed of all songs
    unsigned char t = 20; // t = 20 puts a small delay between notes
    p_per = &period[note];

    if(!MUTED && *p_per != 0) {
        // Enable timer
        TH1 = -(*p_per) >> 8;
        TL1 = -(*p_per) & 0x0ff;
        TR1 = 1;
        ET1 = 1;
    }

    delay(dur_time);

    // Disable timer
    ET1 = 0;
    TR1 = 0;

    delay(t);
}

// play music
void music(Song song) {
    unsigned char note, duration;
    unsigned char i;
    enum MODES start_mode = mode; 
    
    for(i = 0; i < song.length; i++) {
        note = song.notes[i];
        duration = song.durations[i];
        
        if(mode != start_mode)
            return;
        
        play(note, duration);
    }
}

void mute(void) interrupt 0 {
	LED9 = ~LED9;
	MUTED = ~MUTED;
}

void modechange(void) interrupt 2 {
    mode = (mode + 1) % NUM_MODES;
}

void serialMessage(unsigned char *msg)  {
    unsigned char i;

    SCON = 0x40;
    EA = 1;
    
    uart_init();
    
    for(i = 0; msg[i] != '\0'; i++)
        uart_transmit(msg[i]);
    
    uart_transmit('\n');
    uart_transmit('\r');
    
    ES = 0;
}

void main(void) {
    Song song1, song2, mikeSong, brettSong, bretSong;
    TMOD = 0x10;
    
    // setting ports bi-directional
    P0M1 = 0;
    P1M1 = 0;
    P2M1 = 0;
    
    // initialize external hardware interrupts (one for the button mode, one for the mute switch)
    IT0 = 1;
    EX0 = 1;
    IT1 = 1;
    EX1 = 1;
    EA = 1;
    
    song1.title = "Camptown Races";
    song1.length = 48;
    song1.notes = camptown_races[0];
    song1.durations = camptown_races[1];
    
    song2.title = "Old MacDonald";
    song2.length = 62;
    song2. notes = old_macdonald[0];
    song2.durations = old_macdonald[1];
    
    mikeSong.title = "Super Mario";
    mikeSong.length = 78;
    mikeSong.notes = Super_Mario[0];
    mikeSong.durations = Super_Mario[1];
    
    brettSong.title = "Great Fairy Fountain";
    brettSong.length = 129;
    brettSong.notes = great_fairy_fountain[0];
    brettSong.durations = great_fairy_fountain[1];
    
    bretSong.title = "Gerudo Valley";
    bretSong.length = 73;
    bretSong.notes = gerudo_valley[0];
    bretSong.durations = gerudo_valley[1];
    
    while(1) {
        //debugging mode
        while(mode == debug) {
            unsigned int i;
            
            for(i=0;i<10;i++) {
                unsigned int j;

                if(mode != debug)
                    break;
                LED5 = ~LED5;
                for(j = 0; j < 60000; j++);
            }
        }
        // tune1 mode
        while(mode == tune1) {            
            serialMessage(song1.title);
            music(song1);
        }
        // tune2 mode
        while(mode == tune2) {
            serialMessage(song2.title);
            music(song2);
        }
        // keyboard/piano mode
        while(mode == keyboard) {
            if(SW4 == 0)
                play(A5,16);
            if(SW5 == 0)
                play(B5,16);
            if(SW6 == 0)
                play(E6,16);	
        }
        // mike's song
        while(mode == mike)
        {
            serialMessage(mikeSong.title);
            music(mikeSong);
        }
        // brett's song
        while(mode == brett)
        {
            serialMessage(brettSong.title);
            music(brettSong);
        }
        // bret's song
        while(mode == bret)
        {
            serialMessage(bretSong.title);
            music(bretSong);
        }
    }
}