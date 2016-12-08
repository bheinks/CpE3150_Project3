#include <reg932.h>

#define OSC_FREQ    7372800
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

// enum for program state control
enum MODES {
    debug,
    tunes,
    keyboard,
    mike,
    brett,
    bret,
    NUM_MODES
};
enum MODES mode;

code int period[] = {
    0, B6, A6, G6, C7, 0, F6, C6, D6, E6, B4, E4, A4, G5, D4, D5, C5, 
    C4, G4, E5, 0, 0, 0, 0, 0, 0, 0, 0, Ab3, G3, B3, F5, D7, 
    Bb6, Ab6, Gb6, Bb5, 0, A5, B5, Db6, Eb6, Bb4, Eb4, Ab4, Ab5, Db4, Db5, F4, 
    Bb3, Gb4, Eb5, 0, 0, 0, 0, 0, 0, 0, 0, F3, E3, A3, Gb5
};

code char notes[26][20] = { // ODE TO JOY
                            19,19,31,13,13,31,19,15,16,16,15,19,19,15,15,0,0,0,0,0,
                            19,19,31,13,13,31,19,15,16,16,15,19,15,16,16,0,0,0,0,0,
                            15,15,19,16,15,19,31,19,16,15,19,31,19,15,16,15,18,0,0,0,
                            19,19,31,13,13,31,19,15,16,16,15,19,15,16,16,0,0,0,0,0,
                            // AMERICA THE BEAUTIFUL
                            42,42,18,18,42,42,48,48,18,44,42,16,15,42,0,0,0,0,0,0,
                            42,42,18,18,42,42,48,48,31,19,31,13,16,31,0,0,0,0,0,0,
                            42,13,13,31,51,51,15,15,51,31,15,16,42,51,0,0,0,0,0,0,
                            51,51,16,16,51,51,42,42,42,16,51,42,31,51,0,0,0,0,0,0,
                            // MY COUNTRY TIS OF THEE
                            16,16,15,10,16,15,19,19,31,19,15,16,15,16,10,16,0,0,0,0,
                            13,13,13,13,31,19,31,31,31,31,19,15,0,0,0,0,0,0,0,0,
                            19,31,19,15,16,19,31,13,38,31,19,15,16,0,0,0,0,0,0,0,
                            // THIS OLD MAN
                            13,19,13,13,19,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            38,13,31,19,15,19,31,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            19,31,13,16,16,16,16,16,15,19,31,13,0,0,0,0,0,0,0,0,
                            13,15,15,31,19,15,16,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            // MEXICAN HAT DANCE
                            7,39,7,38,45,38,31,19,31,16,0,0,0,0,0,0,0,0,0,0,
                            12,42,16,15,19,31,13,38,36,13,0,0,0,0,0,0,0,0,0,0,
                            36,38,36,13,63,13,19,51,19,16,0,0,0,0,0,0,0,0,0,0,
                            7,39,7,8,7,36,38,13,31,0,0,0,0,0,0,0,0,0,0,0,
                            // TAKE ME OUT TO THE BALLGAME
                            17,0,16,12,18,11,18,14,17,0,16,12,18,11,18,0,0,0,0,0,
                            0,12,44,12,11,48,18,12,48,14,12,12,12,10,16,15,10,12,18,0,
                            11,14,17,0,16,12,18,11,18,14,14,17,14,11,48,18,12,0,0,0,
                            12,10,16,0,0,16,0,0,16,10,12,18,50,18,12,10,16,0,0,0,
                            //TWINKLE, TWINKLE, LITTLE STAR
                            17,17,18,18,12,12,18,48,48,11,11,14,14,17,0,0,0,0,0,0,
                            18,18,48,48,11,11,14,18,18,48,48,11,11,14,0,0,0,0,0,0,
                            17,17,18,18,12,12,18,48,48,11,11,14,14,17,0,0,0,0,0,0};

code char dur[26][20] = {   // ODE TO JOY
                            16,16,16,16,16,16,16,16,16,16,16,16,24,8,32,0,0,0,0,0,
                            16,16,16,16,16,16,16,16,16,16,16,16,24,8,32,0,0,0,0,0, 
                            16,16,16,16,16,8,8,16,16,16,8,8,16,16,16,16,32,0,0,0,
                            16,16,16,16,16,16,16,16,16,16,16,16,24,8,32,0,0,0,0,0,
                            // AMERICA THE BEAUTIFUL
                            16,24,8,16,16,24,8,16,16,16,16,16,16,48,0,0,0,0,0,0,
                            16,24,8,16,16,24,8,16,16,16,16,16,16,48,0,0,0,0,0,0,
                            16,24,8,16,16,24,8,16,16,16,16,16,16,48,0,0,0,0,0,0,
                            16,24,8,16,16,24,8,16,16,16,16,16,16,48,0,0,0,0,0,0,
                            // MY COUNTRY TIS OF THEE
                            16,16,16,24,8,16,16,16,16,24,8,16,16,16,16,48,0,0,0,0,
                            16,16,16,24,8,16,16,16,16,24,8,16,0,0,0,0,0,0,0,0,
                            16,8,8,8,8,24,8,16,8,8,16,16,48,0,0,0,0,0,0,0,
                            // THIS OLD MAN
                            8,8,16,8,8,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            4,4,8,8,4,4,8,4,4,4,4,16,0,0,0,0,0,0,0,0,
                            8,8,8,8,8,8,16,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            // MEXICAN HAT DANCE
                            4,4,4,4,4,4,4,4,4,12,0,0,0,0,0,0,0,0,0,0,
                            4,4,4,4,4,4,4,4,4,12,0,0,0,0,0,0,0,0,0,0,
                            4,4,4,4,4,4,4,4,4,12,0,0,0,0,0,0,0,0,0,0,
                            4,4,4,4,4,4,4,4,12,0,0,0,0,0,0,0,0,0,0,0,
                            // TAKE ME OUT TO THE BALLGAME
                            8,8,8,8,8,8,24,24,8,8,8,8,8,8,40,0,0,0,0,0,
                            8,8,8,8,8,8,8,16,8,24,16,8,8,8,8,8,8,8,8,0,
                            8,8,8,8,8,8,8,8,24,16,8,16,8,8,8,8,32,0,0,0,
                            8,8,8,8,8,8,8,8,8,8,8,8,8,8,24,24,40,0,0,0,
                            //TWINKLE, TWINKLE, LITTLE STAR
                            16,16,16,16,16,16,32,16,16,16,16,16,16,32,0,0,0,0,0,0,
                            16,16,16,16,16,16,32,16,16,16,16,16,16,32,0,0,0,0,0,0,
                            16,16,16,16,16,16,32,16,16,16,16,16,16,32,0,0,0,0,0,0};

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
void play(char note, char duration) {
    int dur_time = TEMPO * duration; // TEMPO defines speed of all songs
    unsigned char t = 20; // t = 20 puts a small delay between notes
    p_per = &period[note];

    if(*p_per != 0) {
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
void music(char begin, char end) {
    unsigned char note, duration;
    unsigned char i, j;
    
    i = 0;
    j = begin;
    do {
        note = notes[j][i];  // read next note and duration
        duration = dur[j][i];
                
        do {
            // return if interrupted
            if(mode != tunes)
                return;
            play(note, duration);  // play note
            i++;
            note = notes[j][i];  // repeat until dur=0 (indicates end of row (phrase))
            duration = dur[j][i];
        } while (duration != 0);

    i = 0;
    j++;
    } while (j < end);
}

void mute(void) interrupt 0 {
    // some code that mutes 
}

void modechange(void) interrupt 2 {
    mode = (mode + 1) % NUM_MODES;
}

void serialMessage(char inputMessage[])  {
		unsigned char i;
		unsigned char msg[] = {inputMessage}
		int length = sizeof(msg) / sizeof(int);
		
		SCON = 0x40;
		
		EA = 1;
		
		uart_init();
		
		for( i = 0; i < length; i++)
			uart_transmi(msg[i]);
		
		EA = 0;
}

void main(void) {
    char begin = 0;
    char end = 4;
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
        // music mode
        while(mode == tunes)
            music(begin, end);
    }
}