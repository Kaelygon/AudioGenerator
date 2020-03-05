#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 
using namespace std; 

int samplehz = 64000;
int channels = 2;
int loamp = -48; //lowest amplitude ascii character
int hiamp = 207; //highest amplitude 
int steps = abs(hiamp)+abs(loamp); //total amplitude range

void rst(){ //reset the file
	ofstream araw;
	araw.open ("./audio.raw");
	araw << "";
	araw.close();
}

void playAudio(void){ //ffplay
	string cmd ="ffplay -f u8 -ar ";
	cmd += to_string(samplehz);
	cmd +=" -ac ";
	cmd += to_string(channels);
	cmd += " -volume 40 -autoexit -i ./audio.raw";

	system(cmd.c_str());
}

void convAudio(void){ //ffmpeg raw to ogg
	string cmd ="ffmpeg -y -f u8 -ar ";
	cmd += to_string(samplehz);
	cmd +=" -ac ";
	cmd += to_string(channels);
	cmd += " -i ./audio.raw -filter:a volume=0.4 audio.ogg ";

	system(cmd.c_str());
}

void silence(double length, int noise = 0){
	ofstream araw;
	araw.open ("./audio.raw", ios_base::app);

	int loop = length*samplehz/1000*channels;

	for(int ii=0; ii<loop; ii++){

		int on;
		if(noise >= rand()%100+1){
			on = rand()%255;
		}else{
			on = loamp+steps/2;
		}

		char oa = '0'+on;
		araw << oa;
	}

	araw.close();
} 

void tone(int amp, double length, int noise = 0){ //Repeats one amplitude

	ofstream araw;
	araw.open ("./audio.raw", ios_base::app);

	if (length < 0){
		length=1/(channels*samplehz/1000);
	}

	for(int i=0; i<=length*(channels*samplehz/1000); i++){ 

		int on;
		if(noise >= rand()%100+1){
			on = rand()%(amp+loamp);
		}else{
			on = amp+loamp;
		}

		char oa = '0'+on;
		araw << oa; 
	}

	araw.close();
}

void square(int amp, double freq, double length, int noise = 0){ //square wave
	ofstream araw;
	araw.open ("./audio.raw", ios_base::app);

	if(amp<1){amp=1;}//amplitude can not be zero or less

	int ampt = (amp-steps)/2;//offset to highest and lowest amplitude

	int loop = length*(freq/1000); //loops wave according to length

	int lo = loamp-ampt; //lowest amplitude

	int hi = lo+amp; //highest amplitude

	double inc = (abs(hi)+abs(lo))*(freq/samplehz)/(channels)*2; // 1 sample length

	double limit = (abs(hi)+abs(lo))/inc;// limit/inc = 1 wave length

	for(int ii=0; ii<loop; ii++){ //generate wave
		for(int i=0; i<limit; i+=1 ){ //high

			int on;
			if(noise >= rand()%100+1){ //add randomness
				on = rand()%(abs(hi)+abs(lo)) + lo;
			}else{
				on = hi;
			}

			char oa = '0'+on;
			araw << oa;
		}
		for(int i=limit; i>0; i-=1 ){ //low

			int on;
			if(noise >= rand()%100+1){
				on = rand()%(abs(hi)+abs(lo)) + lo;
			}else{
				on = lo;
			}

			char oa = '0'+on;
			araw << oa;

		}
	}

	araw.close();
} 

void triangle(int amp, double freq, double length, int noise = 0){ //triangle wave
	ofstream araw;
	araw.open ("./audio.raw", ios_base::app);

	if(amp<1){amp=1;}

	bool end = 0;//for fade out

	int ampt = (steps-amp)/2+2;

	int loop = length*(freq/1000)+1;

	int lo = loamp+ampt;

	int hi = hiamp-ampt;

	double inc = amp*((freq/samplehz)/(channels))*2;

	for(int ii=0; ii<loop; ii++){

		int on;
		for(double i=lo; i<hi+1; i+=inc){
			if(noise >= rand()%100+1){
				on = rand()%(abs(hi)+abs(lo)) + lo;
			}else{
				on = i;
			}
			char oa = '0'+on;
			araw << oa;
		}

		if(end==0){
			for(double i=hi+1; i>lo; i-=inc){
				if(noise >= rand()%100+1){
					on = rand()%(abs(hi)+abs(lo)) + lo;
				}else{
					on = i;
				}
				char oa = '0'+on;
				araw << oa;
			}
		}
	}
 
	araw.close();
} 

void saw(int amp, double freq, double length,int noise = 0){ //triangle wave
	ofstream araw;
	araw.open ("./audio.raw", ios_base::app);

	if(amp<1){amp=1;}

	int ampt = (steps-amp)/2;

	int loop = length*(freq/1000)-1;

	int lo = loamp+ampt;

	int hi = hiamp-ampt;

	double inc = amp*((freq/samplehz)/(channels));

	for(int ii=0; ii<loop; ii++){
		for(double i=lo; i<hi+1; i+=inc){

			int on;
			if(noise >= rand()%100+1){
				on = rand()%(abs(hi)+abs(lo)) + lo;
			}else{
				on = i;				
			}			

			char oa = '0'+on;
			araw << oa;
		}
	}
 
	araw.close();
} 

void sine(int amp, double freq, double length, int noise = 0){ //sine wave
	ofstream araw;
	araw.open ("./audio.raw", ios_base::app);

	if(amp<1){amp=1;}

	int ampt = (steps-amp)/2;

	int loop = length*(freq/1000)+1;

	int lo = loamp+ampt;

	int hi = hiamp-ampt;

	double inc = amp*((freq/samplehz)/(channels))/1.10;

	for(int ii=0; ii<loop; ii++){
		for(double i=0;i<hi+1;i+=inc){

			int on;
			if(noise >= rand()%100+1){
				on = rand()%(abs(hi)+abs(lo)) + lo;
			}else{
				on = ((hi/2)*(sin(6.283*i/hi))+steps/2+loamp); //some mathy stuff
			}

			char oa = '0'+on;
			araw << oa;
		}
	}

	araw.close();
} 

int main(int argc, char const *argv[])
{
	//set random and reset the file
	srand(time(NULL));
	rst();

	//Test
	// [wave type]([amplitude 1-255],);
	//square 200hz 0.5 second
	square(200,200,500);
	//saw 100hz 1 second
	saw(200,100,1000);
	//triangle with 5% noise 1 second
	triangle(200,100,1000,5);
	//sine 440hz 1 second
	sine(200,440,1000);
	//Single amplitude value 1 second
	tone(50-loamp,1000);
	//silence with 5% noise
	silence(1000,5);

	//ffplay
	playAudio();

	//ffmpeg
	convAudio();

	return 0;
}


/* bass
	for(int b=0;b<8;b++){
		int f=60;
		int l=64;
		for(int a=0;a<=l;a++){
			f=120;

			if(a>l/2){
				saw(100,f,1/(f*0.001));
				tone(rand()%100,1/(f*0.001)/2);
			}else{
				square(100,f,1/(f*0.001));
				tone(rand()%100,1/(f*0.001)/1.5);
			}
			triangle(100,f,1/(f*0.001));

			if(a==l/2){
				silence(40);
			}
			if(a==48){
				silence(40);
			}
		}
	}
*/

