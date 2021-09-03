#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h>
#include <time.h>
#include <vector> 
#include <math.h>

using namespace std;

sf::RenderWindow window(sf::VideoMode(1250,1250), "Traffic Simulator" /*,sf::Style::Resize*/);
sf::Event event;// check all the window's events that were triggered since the last iteration of the loop

enum tLightState{
	red,
	green
};
//these 4 variables are used in move functions. It is not the best way to do it but could not figure it out how to do it other way.
tLightState GlobalState[18];//holds state of the lights
int vehicle_x[8];//holds coordinates of the vehicles
int vehicle_y[8];
int vehicleindex;//holds index of the vehicles

class roadtile{
		float x; // x coordinates of roadtile class;
		float y;// y coordinates of roadtile class;
		
		sf::Texture texture; //create an object called texture in Texture class of sf
		sf::Sprite sprite;	//create an object called sprite in sprite class of sf
	
public:

	void draw(){	//prints roadtiles 
		window.draw(sprite);
	}
	
	roadtile(const char *file, float a, float b){
	
		x=a;
		y=b;	
		if (!texture.loadFromFile(file)){			//takes images 
			cout << "Image can't be found" << endl;
		}
		sprite.setTexture(texture);					//assign images to sprite
		sprite.setPosition(x,y);					//set images to given x and y coordinates
	}
};//constructor

class waypoint{
	float x;
	float y;
	int next1;
	int next2;
	int next3;
	int dir;
	int glob_idx;
	
	sf::Texture texture;
	sf::Sprite sprite;
	
public:
	waypoint(int _dir, const char *type, int row , int col, int idx, int _next1, int _next2, int _next3, int _glob_idx){
		
		if(_dir==2){//right direction
			if (!texture.loadFromFile("images/waypoints/right.png")){
				cout << "Image can't be found" << endl;
			}

		}	
		
		else if(_dir==1){//up direction
			if (!texture.loadFromFile("images/waypoints/up.png")){
				cout << "Image can't be found" << endl;
			}

		} 
	
		else if(_dir==3){//down direction
			if (!texture.loadFromFile("images/waypoints/down.png")){
				cout << "Image can't be found" << endl;
			}

		}
		
		else if(_dir==0){//left direction
			if (!texture.loadFromFile("images/waypoints/left.png")){
				cout << "Image can't be found" << endl;
			}

		}
		
		if (type=="corner-bottomleft"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+218;
				y=row*239+121;
			}
		}
		
		else if (type=="corner-bottomright"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+20;
			}
		}
	
		else if (type=="corner-topleft"){
			if(idx==0){
				x=col*239+118;
				y=row*239+218;
			}
			
			else if(idx==1){
				x=col*239+218;
				y=row*239+121;
			}
		}
	
		else if (type=="corner-topright"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+218;
			}
		}	
	
		else if (type=="cross"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==2){
				x=col*239+218;
				y=row*239+121;
			}
			else if(idx==3){
				x=col*239+118;
				y=row*239+218;
			}			
		}	
	
		else if (type=="straight-horizontal"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+172;
				y=row*239+121;
			}
		}
	
		else if (type=="straight-vertical"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+172;
			}
		}
	
		else if (type=="t-bottom"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+20;
			}
			else if(idx==2){
				x=col*239+218;
				y=row*239+121;
			}
		}
	
		else if (type=="t-left"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+218;
				y=row*239+121;
			}
			else if(idx==2){
				x=col*239+118;
				y=row*239+218;
			}
		}
	
		else if (type=="t-right"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+20;
				y=row*239+121;
			}
			else if(idx==2){
				x=col*239+118;
				y=row*239+218;
			}
		}
		
		else if (type=="t-top"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+218;
			}
			else if(idx==2){
				x=col*239+218;
				y=row*239+121;
			}
		}

		dir=_dir;
		glob_idx=_glob_idx;

		next1=_next1;		
		next2=_next2;
		next3=_next3;

		
	sprite.setTexture(texture);
	sprite.setPosition(x,y);
}
	
	void getNext(int &);
	void getNextBus(int &, int &, int&);
	void getPosition(float&,float&,int&);

	void draw(){
		window.draw(sprite);
	}
};

void waypoint::getPosition (float &_x , float &_y , int &_dir){
	_x=x;
	_y=y;
	_dir=dir;

}

void waypoint::getNext(int &c){
	int randnum;
	srand(time(NULL));	
	if (next3!=0){
		randnum=rand()%3;
		if(randnum==0){
			c=next1;
		}
		else if(randnum==1){
			c=next2;
		}
		else if(randnum==2){
			c=next3;
		}
	}
	
	else if (next2!=0){
		randnum=rand()%2;
		if(randnum==0){
			c=next1;
		}
		else if(randnum==1){
			c=next2;
		}
	}
	
	else{
		c=next1;
	}
}

void waypoint::getNextBus(int &c,int &c2,int &c3){//for busses
	c = next1;
	c2 = next2;
	c3 = next3;
} 

class TrafficLight {
	float x;
	float y;
	const char* dir;
	tLightState state;
	sf::Texture redTexture;
	sf::Texture greenTexture;
	sf::Sprite sprite;
	TrafficLight* _next;
	
	public:
		friend class TrafficLightGroup;
		TrafficLight (const char* , int , int , int , const char* , tLightState);
		void getPosition(float &, float &);
		void draw();
		tLightState getState();
		void setState(tLightState);
};

void TrafficLight::getPosition(float &_x,float &_y){
	_x=x;
	_y=y;
}

TrafficLight::TrafficLight(const char* type, int row, int col, int idx, const char* _dir, tLightState _state){
 	state=_state;
 	float _angle;
	
			if (type=="corner-bottomleft"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+218;
				y=row*239+121;
			}
		}
		
		else if (type=="corner-bottomright"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+20;
			}
		}
	
		else if (type=="corner-topleft"){
			if(idx==0){
				x=col*239+118;
				y=row*239+218;
			}
			
			else if(idx==1){
				x=col*239+218;
				y=row*239+121;
			}
		}
	
		else if (type=="corner-topright"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+218;
			}
		}	
	
		else if (type=="cross"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==2){
				x=col*239+218;
				y=row*239+121;
			}
			else if(idx==3){
				x=col*239+118;
				y=row*239+218;
			}			
		}	
	
		else if (type=="straight-horizontal"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+172;
				y=row*239+121;
			}
		}
	
		else if (type=="straight-vertical"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+172;
			}
		}
	
		else if (type=="t-bottom"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+20;
			}
			else if(idx==2){
				x=col*239+218;
				y=row*239+121;
			}
		}
	
		else if (type=="t-left"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+218;
				y=row*239+121;
			}
			else if(idx==2){
				x=col*239+118;
				y=row*239+218;
			}
		}
	
		else if (type=="t-right"){
			if(idx==0){
				x=col*239+118;
				y=row*239+20;
			}
			
			else if(idx==1){
				x=col*239+20;
				y=row*239+121;
			}
			else if(idx==2){
				x=col*239+118;
				y=row*239+218;
			}
		}
		
		else if (type=="t-top"){
			if(idx==0){
				x=col*239+20;
				y=row*239+121;
			}
			
			else if(idx==1){
				x=col*239+118;
				y=row*239+218;
			}
			else if(idx==2){
				x=col*239+218;
				y=row*239+121;
			}
		}

if(_dir=="left"){
	y = y - 50;
	_angle=270;
}
else if(_dir=="right"){
	
	y = y + 50;
	_angle=90;

}
else if(_dir=="up"){
	x = x + 50;
	_angle=0;

}
else if(_dir=="down"){
	x = x - 50;
	_angle=180;

}

	sprite.setPosition(x,y);
	sprite.setRotation(_angle);	

}

tLightState TrafficLight::getState(){
	return state;
}

void TrafficLight::setState(tLightState state){
	this->state=state;
}

void TrafficLight::draw(){

if(getState()==red){
	
	if (!redTexture.loadFromFile("images/trafficlights/red.png")){
					cout << "Image can't be found" << endl;
	}
	sprite.setTexture(redTexture);
}

else if(getState()==green){
	
	if (!greenTexture.loadFromFile("images/trafficlights/green.png")){
					cout << "Image can't be found" << endl;
	}
	sprite.setTexture(greenTexture);
}
	window.draw(sprite);
}

class TrafficLightGroup{
	private:
		TrafficLight* head;
		TrafficLight* current;
		TrafficLight* greenLight;
		int time;
		int duration;

	public:
		TrafficLightGroup (int _duration){
			duration=_duration;
		}
		void add(TrafficLight*);
		void simulate(int);
};

void TrafficLightGroup::add(TrafficLight* light){
	if (head){
		current->_next=light;
		current=light;
		light->_next=head;
		}
	else{
		current=light;
		light->_next=NULL;
		head=light;
	}

}

void TrafficLightGroup::simulate(int timestep){
	time = timestep;	
	if(time%duration==0){			
		if(head->state==red){
			head->setState(green);
			head->_next->setState(red);
		}
		
		else if(head->state==green){
			head->setState(red);
			head->_next->setState(green);			
		}	
	}
}

class Vehicle{	
	public:
		virtual void move() = 0;
		
		int curr_wp;
		int next_wp;		
		float _x; // x coordinates of vehicle class;
		float _y;// y coordinates of vehicle class;
		float _angle;// angle of the vehicle
		int _dir;
		int next_dir;
		float increment_angle;
		int dist=0;
		float increment_x;
		float increment_y;
		int i2=0;				// these are for the if statements
		float diff_x;			// takes difference between cars x coordinates and the next waypoints x coordinates and steer the vehicle upon that information
		float diff_y;			// takes difference between cars y coordinates and the next waypoints y coordinates and steer the vehicle upon that information
		float next_x;			// next waypoints x coordinates.
		float next_y;			// next waypoints y coordinates.
		int iter=0 ;
		int i10=0 ;// when else "if (_dir != next_dir)" occurs in the main, after equalizing _x = next_x and _y = next_y, this "if (_x == next_x && _y == next_y)"
		 		  // statement at the end doesn't occurs so i give another value for that if to occurs immidiately. --> "if (_x == next_x && _y == next_y || i10==1 )"
		
		float tx[18];//coordinates of the traffic lights 
		float ty[18];

		waypoint p[68] = {						   	// It is the array objective of waypoint class. The class members are given below from left to right:
			{1,"corner-topleft",0,0,0,   1,0,0    ,0}, 		// First one is the direction of the waypoints. 0 = left; 1 = up ; 2 = right; 3 = down. 
			{2,"corner-topleft",0,0,1,   2,0,0    ,1}, 		// Second is the roadtile type that waypoints belongs to.
												// Third one specifies which row of the roadtile is, it starts 0;
			{2,"t-top"         ,0,1,0,   3,4,0    ,2},		// Fourth one is the column of the roadtile
			{3,"t-top"         ,0,1,1,   16,0,0    ,3},		// Fifth is the index
			{2,"t-top"         ,0,1,2,   5,0,0    ,4},		// sixth , seventh , eighth is next1, next2 and next3 respectively.
												// seventh one specifies the array number in the array object. It is never used on code but it helps finding the waypoint when you are searching it by yourself.
			{2,"straight-horizontal",0,2,0,   6,0,0   ,5},
			{2,"straight-horizontal",0,2,1,   7,0,0   ,6},

			{2,"t-top"         ,0,3,0,   9,0,0    ,7},
			{0,"t-top"         ,0,3,2,   9,0,0    ,8},
			{3,"t-top"         ,0,3,1,   23,0,0    ,9},

			{0,"corner-topright",0,4,0,   8,0,0    ,10},
			{1,"corner-topright",0,4,1,   10,0,0    ,11},
			//--------------------------------------------------------2
			{1,"t-left"         ,1,0,0,   0,0,0    ,12},
			{1,"t-left"         ,1,0,2,   12,0,0    ,13},
			{0,"t-left"         ,1,0,1,   12,0,0    ,14},
		
			{0,"cross"         ,1,1,0,   14,0,0    ,15},
			{3,"cross"         ,1,1,1,   15,17,18    ,16},
			{2,"cross"         ,1,1,2,   19,0,0    ,17},
			{3,"cross"         ,1,1,3,   32,0,0    ,18},

			{2,"t-top"         ,1,2,0,   20,0,0    ,19},
			{3,"t-top"         ,1,2,1,   36,0,0    ,20},
			{2,"t-top"         ,1,2,2,   22,0,0    ,21},
			
			{2,"t-bottom"         ,1,3,0,   24,0,0    ,22},
			{3,"t-bottom"         ,1,3,1,   24,0,0    ,23},
			{2,"t-bottom"         ,1,3,2,   25,0,0    ,24},

			{2,"t-right"         ,1,4,1,   26,27,0    ,25},
			{1,"t-right"         ,1,4,0,   11,0,0    ,26},
			{3,"t-right"         ,1,4,2,   41,0,0    ,27},
			//--------------------------------------------------------3
			{1,"t-left"         ,2,0,0,   13,0,0    ,28},
			{0,"t-left"         ,2,0,1,   28,0,0    ,29},
			{1,"t-left"         ,2,0,2,   28,0,0    ,30},

			{0,"cross"         ,2,1,0,   29,0,0    ,31},
			{3,"cross"         ,2,1,1,   31,33,0    ,32},
			{3,"cross"         ,2,1,3,   45,0,0    ,33},
			{0,"cross"         ,2,1,2,   31,33,0    ,34},

			{0,"t-right"         ,2,2,1,   34,0,0    ,35},
			{3,"t-right"         ,2,2,0,   35,0,0    ,36},
			{1,"t-right"         ,2,2,2,   35,0,0    ,37},

			{0,"corner-topleft"         ,2,3,1,   39,0,0    ,38},
			{3,"corner-topleft"         ,2,3,0,   51,0,0    ,39},

			{0,"t-right"         ,2,4,1,   38,0,0    ,40},
			{3,"t-right"         ,2,4,0,   40,42,0    ,41},
			{3,"t-right"         ,2,4,2,   54,0,0    ,42},
			//--------------------------------------------------------4
			{1,"straight-vertical"         ,3,0,0,   30,0,0    ,43},
			{1,"straight-vertical"         ,3,0,1,   43,0,0    ,44},

			{3,"straight-vertical"         ,3,1,0,   46,0,0    ,45},
			{3,"straight-vertical"         ,3,1,1,   59,0,0    ,46},

			{1,"t-left"         ,3,2,0,   37,0,0    ,47},
			{1,"t-left"         ,3,2,2,   47,0,0    ,48},
			{0,"t-left"         ,3,2,1,   47,0,0    ,49},
	
			{0,"t-bottom"         ,3,3,0,   49,0,0    ,50},
			{3,"t-bottom"         ,3,3,1,   50,52,0    ,51},
			{2,"t-bottom"         ,3,3,2,   53,0,0    ,52},

			{2,"t-right"         ,3,4,1,   55,0,0    ,53},
			{3,"t-right"         ,3,4,0,   55,0,0    ,54},
			{3,"t-right"         ,3,4,2,   67,0,0    ,55},
			//--------------------------------------------------------5
			{1,"corner-bottomleft"         ,4,0,0,   44,0,0    ,56},
			{0,"corner-bottomleft"         ,4,0,1,   56,0,0    ,57},
	
			{0,"t-bottom"         ,4,1,0,   57,0,0    ,58},
			{3,"t-bottom"         ,4,1,1,   58,60,0    ,59},
			{2,"t-bottom"         ,4,1,2,   61,0,0    ,60},

			{2,"t-bottom"         ,4,2,0,   62,0,0    ,61},
			{1,"t-bottom"         ,4,2,1,   48,0,0    ,62},
			{0,"t-bottom"         ,4,2,2,   62,0,0    ,63},
		
			{0,"straight-horizontal"        ,4,3,0,   63,0,0   ,64},
			{0,"straight-horizontal"        ,4,3,1,   64,0,0   ,65},
		
			{0,"corner-bottomright"         ,4,4,0,   65,0,0    ,66},
			{3,"corner-bottomright"         ,4,4,1,   66,0,0    ,67},
		};

};

class Car : public Vehicle , public waypoint {
		
		sf::Texture texture;
		sf::Sprite sprite;
	public:
		
		Car(const char*c, float _x, float _y, int _dir,int next_dir, int curr_wp, int next_wp, float next_x, float next_y,float _tx[18],float _ty[18]) : waypoint( 0, "q", 0 ,0, 0, 0, 0, 0, 0){ 
				
		this->_x=_x;
		this->_y=_y;
		this->_dir=_dir;
		this->next_dir=next_dir;
		this->curr_wp = curr_wp;
		this->next_wp = next_wp;
		this->next_x = next_x;
		this->next_y = next_y;

		for(int i=0;i<18;i++){
			tx[i] = _tx[i];
			ty[i] = _ty[i];
		}
		
		
		
		if (!texture.loadFromFile(c)){
			cout << "Image can't be found" << endl;
		}	
		
		sprite.setTexture(texture);	
		}
		
		void draw(){
			window.draw(sprite);
		}
		
		void move();
		
};

void Car::move(){	
				
if (_dir == next_dir && dist == 0){	 //there are two possibilities for the car; going straight or turning. if _dir equivalent to next_dir, for sure it is going straight.
	
	if (_dir == 0){//left
		increment_x = -1;
		increment_y = 0;
		_angle = 180;
	}
	
	else if (_dir == 1){//up
		increment_x = 0;
		increment_y = -1;
		_angle = 270;
	}
	
	else if (_dir == 2){//right
		increment_x = 1;
		increment_y = 0;
		_angle = 0;
	}
	
	else if (_dir == 3){//down
		increment_x = 0;
		increment_y = 1;
		_angle = 90;
	}

}

else if (_dir != next_dir && dist == 0){ // if _dir and next_dir are not equal, car is turning.
	if(i2 == 0){//diff_x and diff_y has to be calculated once. if not, they are change in every loop.
		diff_x = next_x - _x;
		diff_y = next_y - _y;
		i2 = 1;//i2 = 1 until the if(i==180) statements occurs.
	}
	
	iter++; // car needs to be turn 90 degree so after 180 times of iteration, _dir equalizes the mext_dir thanks to if(i == 180) statement. 
	increment_x = diff_x/180; //after 180 times of cycle, the car will be in the next waypoint
	increment_y = diff_y/180; //after 180 times of cycle, the car will be in the next waypoint
	
	if ( _dir == 0 ){//left
		if ( next_dir == 1){
			increment_angle = + 0.5;
		}
		else if ( next_dir == 3){
			increment_angle = - 0.5;
		}
	}

	else if ( _dir == 2 ){//right
		if ( next_dir == 1){
			increment_angle = - 0.5;
		}
		else if ( next_dir == 3){
			increment_angle = + 0.5;
		}
	}

	else if ( _dir == 3 ){//down
		if ( next_dir == 0){
			increment_angle = + 0.5;
		}
		else if ( next_dir == 2){
			increment_angle = - 0.5;
		}
	}

	else if ( _dir == 1 ){//up
		if ( next_dir == 0){
			increment_angle = - 0.5;
		}
		else if ( next_dir == 2){
			increment_angle = + 0.5;
		}
	}
	
	if (iter>=180){//180th cycle
		_x = next_x;
		_y = next_y;
		iter = 0;
		i2 = 0;
		i10=1;
		increment_angle = 0;
	}

}

for(int k=0;k<18;k++){
	
	if( _x+50==tx[k] && _y==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 270; //car goes to left
		iter = 0;
	}
	
	else if( _x-50==tx[k] && _y==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 90;//car goes to right
		iter = 0;
	}
		
	else if( _x==tx[k] && _y+50==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 0;//car goes to up
		iter = 0;
	}

	else if( _x==tx[k] && _y-50==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 180;//car goes to down
		iter = 0;
	}
}

for(int j=0;j<8;j++){
	
	if(vehicleindex!=j){
	
	if(_dir == 2){ //when car goes right, looks for a car infront
	
		if(next_dir == 2){// if the next direction is right
			if(   (vehicle_x[j] - _x) <= 80 && (vehicle_x[j] - _x) >= 10 && ( vehicle_y[j] - _y ) >= -50 &&  ( vehicle_y[j] - _y ) <= 50    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;//iter variable is used for turning action and it  increases in every loop so as long as the car stops, it needs to be reduced.( but only while turning so it is in the if )
				}
				increment_angle = 0;

			}
		}
		else if(next_dir == 1){// if the next direction is up
			if(   (vehicle_x[j] - _x) <= 80 && (vehicle_x[j] - _x) >= 10 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
	
			}
		}
		else if(next_dir == 3){// if the next direction is down
			if(   (vehicle_x[j] - _x) <= 80 && (vehicle_x[j] - _x) >= 10 && ( vehicle_y[j] - _y ) >= 0 &&  ( vehicle_y[j] - _y ) <= 70    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}
	
	else if(_dir == 0){ //when car goes left, looks for a car infront
	
		if(next_dir == 0){// if the next direction is left
			if(   (vehicle_x[j] - _x) >= -80 && (vehicle_x[j] - _x) <= -10 && ( vehicle_y[j] - _y ) >= -50 &&  ( vehicle_y[j] - _y ) <= 50    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 1){// if the next direction is up
			if(   (vehicle_x[j] - _x) >= -80 && (vehicle_x[j] - _x) <= -10 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 3){// if the next direction is down
			if(   (vehicle_x[j] - _x) >= -80 && (vehicle_x[j] - _x) <= -10 && ( vehicle_y[j] - _y ) >= 0 &&  ( vehicle_y[j] - _y ) <= 70    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}

	else if(_dir == 1){ //when car goes up, looks for a car infront
	
		if(next_dir == 1){// if the next direction is up
			if(   (vehicle_x[j] - _x) <= 50 && (vehicle_x[j] - _x) >= -50 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 0){// if the next direction is left
			if(   (vehicle_x[j] - _x) < -10 && (vehicle_x[j] - _x) > -80 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 2){// if the next direction is right
			if(   (vehicle_x[j] - _x) <= 70 && (vehicle_x[j] - _x) >= 0 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}
	
	else if(_dir == 3){ //when car goes down, looks for a car infront
	
		if(next_dir == 3){// if the next direction is down
			if(   (vehicle_x[j] - _x) <= 50 && (vehicle_x[j] - _x) >= -50 && ( vehicle_y[j] - _y ) <= 80 &&  ( vehicle_y[j] - _y ) >= 10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 0){// if the next direction is left
			if(   (vehicle_x[j] - _x) <= -10 && (vehicle_x[j] - _x) >= -50 && ( vehicle_y[j] - _y ) <= 80 &&  ( vehicle_y[j] - _y ) >= 10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 2){// if the next direction is right
			if(   (vehicle_x[j] - _x) <= 70 && (vehicle_x[j] - _x) >= 0 && ( vehicle_y[j] - _y ) <= 80 &&  ( vehicle_y[j] - _y ) >= 10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}
}
}

		_x += increment_x;
 	   	_y += increment_y;
  		_angle = _angle + increment_angle;
  		
  		
  		vehicle_x[vehicleindex] = _x;//equals cars x coordinates to global value
  		vehicle_y[vehicleindex] = _y;//equals cars y coordinates to global value
		vehicleindex++;
		  			
		sprite.setPosition(_x,_y);
		sprite.setRotation(_angle);	
		
		sf::FloatRect boundingBox = sprite.getGlobalBounds();
		sprite.setOrigin(sf::Vector2f(boundingBox.width / 4, boundingBox.height / 4));
		
if (_x == next_x && _y == next_y || i10==1 ){ //assigning cars current and next waypoints
	curr_wp = next_wp;
	
	p[curr_wp].getPosition(_x,_y,_dir);
	p[curr_wp].getNext(next_wp);
	p[next_wp].getPosition(next_x,next_y,next_dir);
	i10=0; // this i10 for the if(i==180) statement specifyed just above. after equalizing next_ = x and y in it, it doesn't directly go through this if statement as required.
		   // so i found this solution
}

}

class Bus:public Vehicle,public waypoint{
	
	sf::Texture texture;
	sf::Sprite sprite;
		
	vector<int> stops;
//	int currentStop; //current stop index
	int next_wp1;
	int next_wp2;
	int next_wp3;

	//-------//those variables are used for selecting the next waypoint.
	float x1;//holds bus's current waypoint's first possible next waypoint's x coordinates
	float y1;//holds bus's current waypoint's first possible next waypoint's y coordinates
	int dir1;
	float x2;
	float y2;
	int dir2;
	float x3;
	float y3;
	int dir3;
	float xstop;
	float ystop;
	int dirstop;
	int iterstop = 1;//used in vector's index
	//--------------------------------------

	public:
		Bus(const char*c,float _x, float _y, int _dir,int next_dir, int curr_wp, int next_wp, float next_x, float next_y,float _tx[18],float _ty[18]):waypoint(0,"dir",0,0,0,0,0,0,0){
			
		this->_x=_x;
		this->_y=_y;
		this->_dir=_dir;
		this->next_dir=next_dir;
		this->curr_wp = curr_wp;
		this->next_wp = next_wp;
		this->next_x = next_x;
		this->next_y = next_y;

		for(int i=0;i<18;i++){
			tx[i] = _tx[i];
			ty[i] = _ty[i];
		}
	if (!texture.loadFromFile("images/vehicles/bus.png")){
		cout << "Image can't be found" << endl;
	}				
		sprite.setTexture(texture);		
		}
		
		void addStop(int index);
		void move();
		void draw(){
	window.draw(sprite);			
		}
		
};

void Bus::addStop(int index){
	stops.push_back(index);
}

void Bus::move(){

if (_dir == next_dir && dist == 0){	 //there are two possibilities for the bus; going straight or turning. if _dir equivalent to next_dir, for sure it is going straight.
	
	if (_dir == 0){//left
		increment_x = -1;
		increment_y = 0;
		_angle = 180;
	}
	
	else if (_dir == 1){//up
		increment_x = 0;
		increment_y = -1;
		_angle = 270;
	}
	
	else if (_dir == 2){//right
		increment_x = 1;
		increment_y = 0;
		_angle = 0;
	}
	
	else if (_dir == 3){//down
		increment_x = 0;
		increment_y = 1;
		_angle = 90;
	}

}

else if (_dir != next_dir && dist == 0){ // if _dir and next_dir are not equal, bus is turning.
	if(i2 == 0){//diff_x and diff_y has to be calculated once. if not, they change in every loop.
		diff_x = next_x - _x;
		diff_y = next_y - _y;
		i2 = 1;//i2 = 1 until the if(i==180) statements occurs.
	}
	
	iter++; // bus needs to be turned 90 degree so after 180 times of iteration, _dir equalizes the mext_dir thanks to if(i == 180) statement. 
	increment_x = diff_x/180; //after 180 times of cycle, the bus will be in the next waypoint
	increment_y = diff_y/180; //after 180 times of cycle, the bus will be in the next waypoint
	
	if ( _dir == 0 ){//left
		if ( next_dir == 1){
			increment_angle = + 0.5;
		}
		else if ( next_dir == 3){
			increment_angle = - 0.5;
		}
	}

	else if ( _dir == 2 ){//right
		if ( next_dir == 1){
			increment_angle = - 0.5;
		}
		else if ( next_dir == 3){
			increment_angle = + 0.5;
		}
	}

	else if ( _dir == 3 ){//down
		if ( next_dir == 0){
			increment_angle = + 0.5;
		}
		else if ( next_dir == 2){
			increment_angle = - 0.5;
		}
	}

	else if ( _dir == 1 ){//up
		if ( next_dir == 0){
			increment_angle = - 0.5;
		}
		else if ( next_dir == 2){
			increment_angle = + 0.5;
		}
	}
	
	if (iter>=180){//180th cycle
		_x = next_x;
		_y = next_y;
		iter = 0;
		i2 = 0;
		i10=1;
		increment_angle = 0;
	}

}

for(int k=0;k<18;k++){
	
	if( _x+50==tx[k] && _y==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 270; //bus goes to left
		iter = 0;
		//hold = k;
	}
	
	else if( _x-50==tx[k] && _y==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 90;//bus goes to right
		iter = 0;
	//	hold = k;
	}
		
	else if( _x==tx[k] && _y+50==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 0;//bus goes to up
		iter = 0;
	//	hold = k;
	}

	else if( _x==tx[k] && _y-50==ty[k] && GlobalState[k]==red ){
		increment_x = 0;
		increment_y = 0;
		_angle = 180;//bus goes to down
		iter = 0;
	//	hold = k;
	}
}

for(int j=0;j<8;j++){
	
	if(vehicleindex!=j){
	
	if(_dir == 2){ //when bus goes right, looks for a vehicle infront
	
		if(next_dir == 2){// if the next direction is right
			if(   (vehicle_x[j] - _x) <= 80 && (vehicle_x[j] - _x) >= 10 && ( vehicle_y[j] - _y ) >= -50 &&  ( vehicle_y[j] - _y ) <= 50    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;//iter variable is used for turning action and it  increases in every loop so as long as the car stops, it needs to be reduced.	
				}
				increment_angle = 0;

			}
		}
		else if(next_dir == 1){// if the next direction is up
			if(   (vehicle_x[j] - _x) <= 80 && (vehicle_x[j] - _x) >= 10 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;//iter variable is used for turning action and it  increases in every loop so as long as the car stops, it needs to be reduced.	
				}
				increment_angle = 0;
	
			}
		}
		else if(next_dir == 3){// if the next direction is down
			if( (vehicle_x[j] - _x) <= 80 && (vehicle_x[j] - _x) >= 10 && ( vehicle_y[j] - _y ) >= 0 &&  ( vehicle_y[j] - _y ) <= 70    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;//iter variable is used for turning action and it  increases in every loop so as long as the car stops, it needs to be reduced.	
				}
				increment_angle = 0;
			}
		}
	}
	
	else if(_dir == 0){ //when car goes left, looks for a car infront
	
		if(next_dir == 0){// if the next direction is left
			if(   (vehicle_x[j] - _x) >= -80 && (vehicle_x[j] - _x) <= -10 && ( vehicle_y[j] - _y ) >= -50 &&  ( vehicle_y[j] - _y ) <= 50    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;//iter variable is used for turning action and it  increases in every loop so as long as the car stops, it needs to be reduced.	
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 1){// if the next direction is up
			if(   (vehicle_x[j] - _x) >= -80 && (vehicle_x[j] - _x) <= -10 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 3){// if the next direction is down
			if(   (vehicle_x[j] - _x) >= -80 && (vehicle_x[j] - _x) <= -10 && ( vehicle_y[j] - _y ) >= 0 &&  ( vehicle_y[j] - _y ) <= 70    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}

	else if(_dir == 1){ //when car goes up, looks for a car infront
	
		if(next_dir == 1){// if the next direction is up
			if(   (vehicle_x[j] - _x) <= 50 && (vehicle_x[j] - _x) >= -50 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 0){// if the next direction is left
			if(   (vehicle_x[j] - _x) < -10 && (vehicle_x[j] - _x) > -80 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 2){// if the next direction is right
			if(   (vehicle_x[j] - _x) <= 70 && (vehicle_x[j] - _x) >= 0 && ( vehicle_y[j] - _y ) >= -80 &&  ( vehicle_y[j] - _y ) <= -10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}
	
	else if(_dir == 3){ //when car goes down, looks for a car infront
	
		if(next_dir == 3){// if the next direction is down
			if(   (vehicle_x[j] - _x) <= 50 && (vehicle_x[j] - _x) >= -50 && ( vehicle_y[j] - _y ) <= 80 &&  ( vehicle_y[j] - _y ) >= 10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 0){// if the next direction is left
			if(   (vehicle_x[j] - _x) <= -10 && (vehicle_x[j] - _x) >= -50 && ( vehicle_y[j] - _y ) <= 80 &&  ( vehicle_y[j] - _y ) >= 10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
		else if(next_dir == 2){// if the next direction is right
			if(   (vehicle_x[j] - _x) <= 70 && (vehicle_x[j] - _x) >= 0 && ( vehicle_y[j] - _y ) <= 80 &&  ( vehicle_y[j] - _y ) >= 10    ){
				increment_x = 0;
				increment_y = 0;
				if(increment_angle!=0){
					iter--;
				}
				increment_angle = 0;
			}
		}
	}
}
}

		_x += increment_x;
 	   	_y += increment_y;
  		_angle = _angle + increment_angle;
  		
  		vehicle_x[vehicleindex] = _x;//equals cars x coordinates to global value
  		vehicle_y[vehicleindex] = _y;//equals cars y coordinates to global value
		vehicleindex++;
		
		sprite.setPosition(_x,_y);
		sprite.setRotation(_angle);	
		
		sf::FloatRect boundingBox = sprite.getGlobalBounds();
		sprite.setOrigin(sf::Vector2f(boundingBox.width / 4, boundingBox.height / 4));

if (_x == next_x && _y == next_y || i10==1 ){ //assigning bus's current and next waypoints
	curr_wp = next_wp;
	
	p[curr_wp].getPosition(_x,_y,_dir);
	p[curr_wp].getNext(next_wp);

	i10=0; // this i10 for the if(i==180) statement specifyed just above. after equalizing next_ = x and y in it, it doesn't directly go through this if statement as required.

	p[curr_wp].getNextBus(next_wp1,next_wp2,next_wp3);//gets the number of the waypoints
	p[next_wp1].getPosition(x1,y1,dir1);//gets the coordinations and directions of the first waypoint
	p[next_wp2].getPosition(x2,y2,dir2);///for 2nd
	p[next_wp3].getPosition(x3,y3,dir3);//for 3rd
	p[stops[iterstop]].getPosition(xstop,ystop,dirstop);//for stops
	
	if(x1 == xstop && y1== ystop){//----------------//the basic algorithm
		next_wp = next_wp1;
		p[next_wp].getPosition(next_x,next_y,next_dir);
	}
		
	else if(x2 == xstop && y2== ystop){
		next_wp = next_wp2;
		p[next_wp].getPosition(next_x,next_y,next_dir);
	}
	else if(x3 == xstop && y3== ystop){
		next_wp = next_wp3;
		p[next_wp].getPosition(next_x,next_y,next_dir);
	}
	else{
		next_wp = next_wp1;
		p[next_wp].getPosition(next_x,next_y,next_dir);
	}//----------------------------------------------//until here
	
if(_x == xstop && _y == ystop){//counts the bus stops
	iterstop++;
	if(iterstop ==6) iterstop =0;
}
}

if(vehicleindex==8)	vehicleindex =0; //there are 8 vehicles and this one counts them
}

class BusStop{
	float x;
	float y;
	float dir;
	
	sf::Texture texture;
	sf::Sprite sprite;
	
	public:
		BusStop(float x,float y, float dir);
		void getPosition(float &_x, float&_y, float&_dir);
		void draw();
};

BusStop::BusStop(float x,float y, float dir){
	this->x=x;
	this->y=y;
	this->dir=dir;
}

void BusStop::getPosition(float &_x, float&_y, float&_dir){
	_x=x;
	_y=y;
	_dir=dir;
}

void BusStop::draw(){
	if (!texture.loadFromFile("images/busstop/busstop.png")){
		cout << "Image can't be found" << endl;
	}	
	sprite.setTexture(texture);
	sprite.setPosition(x,y);
	sprite.setRotation(dir);
	window.draw(sprite);
}

int main(){

int car_curr_wp[6]= {1,3,5,24,9,33};			// current waypoint.( it is alsa the start point of the car. you have to give numbers to it because the car has to have a starting point.
int car_next_wp[6];			// next waypoint.
float car_x[6];				// cars x coordinates
float car_y[6];				// cars y coordinates
float car_angle[6];
int car_dir[6];				// direction of the  current waypoint --- 0->left // 1->up // 2->right // 3->down
float car_next_x[6];			// next waypoints x coordinates.
float car_next_y[6];			// next waypoints y coordinates.
int car_next_dir[6];			// next waypoints direction.

float tx[18];
float ty[18];

int bus_curr_wp[2]= {56,4};			// current waypoint.( it is alsa the start point of the car. you have to give numbers to it because the car has to have a starting point.
int bus_next_wp[2]={44,5};			// next waypoint.

int bus_next_wp2[2]={44,19};//in waypoints getNextBus method we need to send three variable although we need 1. so for one time use we create bus_next_wp2 & bus_next_wp3.
int bus_next_wp3[2]={44,19};

float bus_x[2];				// cars x coordinates
float bus_y[2];				// cars y coordinates
float bus_angle[2];
int bus_dir[2];				// direction of the  current waypoint --- 0->left // 1->up // 2->right // 3->down
float bus_next_x[2];			// next waypoints x coordinates.
float bus_next_y[2];			// next waypoints y coordinates.
int bus_next_dir[2];			// next waypoints direction.

int timestep=1;// send time information to simulate function of TrafficLightGroup

roadtile r[25] = {
{"images/roadpieces/corner-topleft.png",0.0, 0.0 },
{"images/roadpieces/t-top.png",239.0, 0.0},
{"images/roadpieces/straight-horizontal.png",478.0, 0.0},
{"images/roadpieces/t-top.png",717.0, 0.0},
{"images/roadpieces/corner-topright.png",956.0, 0.0},

{"images/roadpieces/t-left.png",0.0, 239.0},
{"images/roadpieces/cross.png",239.0, 239.0},
{"images/roadpieces/t-top.png",478.0, 239.0},
{"images/roadpieces/t-bottom.png",717.0, 239.0},
{"images/roadpieces/t-right.png",956.0, 239.0},

{"images/roadpieces/t-left.png",0.0, 478.0},
{"images/roadpieces/cross.png",239.0, 478.0},
{"images/roadpieces/t-right.png",478.0, 478.0},
{"images/roadpieces/corner-topleft.png",717.0, 478.0},
{"images/roadpieces/t-right.png",956.0, 478.0},

{"images/roadpieces/straight-vertical.png",0.0, 717.0},
{"images/roadpieces/straight-vertical.png",239.0, 717.0},
{"images/roadpieces/t-left.png",478.0, 717.0},
{"images/roadpieces/t-bottom.png",717.0, 717.0},
{"images/roadpieces/t-right.png",956.0, 717.0},

{"images/roadpieces/corner-bottomleft.png",0.0, 956.0},
{"images/roadpieces/t-bottom.png", 239.0, 956.0},
{"images/roadpieces/t-bottom.png",478.0, 956.0},
{"images/roadpieces/straight-horizontal.png",717.0, 956.0},
{"images/roadpieces/corner-bottomright.png",956.0, 956.0},
};

waypoint p[68] = {						   	// It is the array objective of waypoint class. The class members are given below from left to right:
{1,"corner-topleft",0,0,0,   1,0,0    ,0}, 		// First one is the direction of the waypoints. 0 = left; 1 = up ; 2 = right; 3 = down. 
{2,"corner-topleft",0,0,1,   2,0,0    ,1}, 		// Second is the roadtile type that waypoints belongs to.
												// Third one specifies which row of the roadtile is, it starts 0;
{2,"t-top"         ,0,1,0,   3,4,0    ,2},		// Fourth one is the column of the roadtile
{3,"t-top"         ,0,1,1,   16,0,0    ,3},		// Fifth is the index
{2,"t-top"         ,0,1,2,   5,0,0    ,4},		// sixth , seventh , eighth is next1, next2 and next3 respectively.
												// seventh one specifies the array number in the array object. It is never used on code but it helps finding the waypoint when you are searching it by yourself.
{2,"straight-horizontal",0,2,0,   6,0,0   ,5},
{2,"straight-horizontal",0,2,1,   7,0,0   ,6},

{2,"t-top"         ,0,3,0,   9,0,0    ,7},
{0,"t-top"         ,0,3,2,   9,0,0    ,8},
{3,"t-top"         ,0,3,1,   23,0,0    ,9},

{0,"corner-topright",0,4,0,   8,0,0    ,10},
{1,"corner-topright",0,4,1,   10,0,0    ,11},
//--------------------------------------------------------2
{1,"t-left"         ,1,0,0,   0,0,0    ,12},
{1,"t-left"         ,1,0,2,   12,0,0    ,13},
{0,"t-left"         ,1,0,1,   12,0,0    ,14},

{0,"cross"         ,1,1,0,   14,0,0    ,15},
{3,"cross"         ,1,1,1,   15,17,18    ,16},
{2,"cross"         ,1,1,2,   19,0,0    ,17},
{3,"cross"         ,1,1,3,   32,0,0    ,18},

{2,"t-top"         ,1,2,0,   20,0,0    ,19},
{3,"t-top"         ,1,2,1,   36,0,0    ,20},
{2,"t-top"         ,1,2,2,   22,0,0    ,21},

{2,"t-bottom"         ,1,3,0,   24,0,0    ,22},
{3,"t-bottom"         ,1,3,1,   24,0,0    ,23},
{2,"t-bottom"         ,1,3,2,   25,0,0    ,24},

{2,"t-right"         ,1,4,1,   26,27,0    ,25},
{1,"t-right"         ,1,4,0,   11,0,0    ,26},
{3,"t-right"         ,1,4,2,   41,0,0    ,27},
//--------------------------------------------------------3
{1,"t-left"         ,2,0,0,   13,0,0    ,28},
{0,"t-left"         ,2,0,1,   28,0,0    ,29},
{1,"t-left"         ,2,0,2,   28,0,0    ,30},

{0,"cross"         ,2,1,0,   29,0,0    ,31},
{3,"cross"         ,2,1,1,   31,33,0    ,32},
{3,"cross"         ,2,1,3,   45,0,0    ,33},
{0,"cross"         ,2,1,2,   31,33,0    ,34},

{0,"t-right"         ,2,2,1,   34,0,0    ,35},
{3,"t-right"         ,2,2,0,   35,0,0    ,36},
{1,"t-right"         ,2,2,2,   35,0,0    ,37},

{0,"corner-topleft"         ,2,3,1,   39,0,0    ,38},
{3,"corner-topleft"         ,2,3,0,   51,0,0    ,39},

{0,"t-right"         ,2,4,1,   38,0,0    ,40},
{3,"t-right"         ,2,4,0,   40,42,0    ,41},
{3,"t-right"         ,2,4,2,   54,0,0    ,42},
//--------------------------------------------------------4
{1,"straight-vertical"         ,3,0,0,   30,0,0    ,43},
{1,"straight-vertical"         ,3,0,1,   43,0,0    ,44},

{3,"straight-vertical"         ,3,1,0,   46,0,0    ,45},
{3,"straight-vertical"         ,3,1,1,   59,0,0    ,46},

{1,"t-left"         ,3,2,0,   37,0,0    ,47},
{1,"t-left"         ,3,2,2,   47,0,0    ,48},
{0,"t-left"         ,3,2,1,   47,0,0    ,49},

{0,"t-bottom"         ,3,3,0,   49,0,0    ,50},
{3,"t-bottom"         ,3,3,1,   50,52,0    ,51},
{2,"t-bottom"         ,3,3,2,   53,0,0    ,52},

{2,"t-right"         ,3,4,1,   55,0,0    ,53},
{3,"t-right"         ,3,4,0,   55,0,0    ,54},
{3,"t-right"         ,3,4,2,   67,0,0    ,55},
//--------------------------------------------------------5
{1,"corner-bottomleft"         ,4,0,0,   44,0,0    ,56},
{0,"corner-bottomleft"         ,4,0,1,   56,0,0    ,57},

{0,"t-bottom"         ,4,1,0,   57,0,0    ,58},
{3,"t-bottom"         ,4,1,1,   58,60,0    ,59},
{2,"t-bottom"         ,4,1,2,   61,0,0    ,60},

{2,"t-bottom"         ,4,2,0,   62,0,0    ,61},
{1,"t-bottom"         ,4,2,1,   48,0,0    ,62},
{0,"t-bottom"         ,4,2,2,   62,0,0    ,63},

{0,"straight-horizontal"        ,4,3,0,   63,0,0   ,64},
{0,"straight-horizontal"        ,4,3,1,   64,0,0   ,65},

{0,"corner-bottomright"         ,4,4,0,   65,0,0    ,66},
{3,"corner-bottomright"         ,4,4,1,   66,0,0    ,67},
};

TrafficLight t[18] = {
	{"t-top",  0,3,0,  "right",green}, //group 1
	{"t-top",  0,3,2,  "left", red},
	
	{"t-left", 1,0,2, "up",green},  //group 2
	{"t-left", 1,0,1, "left",red},
	
	{"t-bottom", 1,3,0, "right",green}, //group 3
	{"t-bottom", 1,3,1, "down",red},
	
	{"t-left", 2,0,1, "left",green}, //group 4
	{"t-left", 2,0,2, "up",red},
	
	{"cross",  2,1,1,  "down",green},//grup 5
	{"cross",  2,1,2,  "left",red},
	
	{"t-right",  2,2,0,  "down",green},//grup 6
	{"t-right",  2,2,2,  "up",red},
	
	{"t-left",  3,2,1,  "left",green},//grup 7
	{"t-left",  3,2,2,  "up",red},

	{"t-right",  3,4,0,  "down",green},//grup 8
	{"t-right",  3,4,1,  "right",red},
	
	{"t-bottom", 4,2,0, "right",green}, // group 9
	{"t-bottom", 4,2,2, "left",red},
	
};

TrafficLightGroup t_l_g[9]={
	{1500},{2000},{1000},{2000},{1000},{1500},{2000},{1000},{2500},
};

t_l_g[0].add(&t[0]);t_l_g[0].add(&t[1]);	t_l_g[1].add(&t[2]);t_l_g[1].add(&t[3]);	t_l_g[2].add(&t[4]);t_l_g[2].add(&t[5]);	t_l_g[3].add(&t[6]);t_l_g[3].add(&t[7]);

t_l_g[4].add(&t[8]);t_l_g[4].add(&t[9]);	t_l_g[5].add(&t[10]);t_l_g[5].add(&t[11]);	t_l_g[6].add(&t[12]);t_l_g[6].add(&t[13]);	t_l_g[7].add(&t[14]);t_l_g[7].add(&t[15]);	

t_l_g[8].add(&t[16]);t_l_g[8].add(&t[17]);

for(int i=0;i<6;i++){
	p[car_curr_wp[i]].getPosition(car_x[i],car_y[i],car_dir[i]);				// these three objects are used 1 time.
	p[car_curr_wp[i]].getNext(car_next_wp[i]);									// the are assigning the first iformation based on the given start point of the car.
	p[car_next_wp[i]].getPosition(car_next_x[i],car_next_y[i],car_next_dir[i]);
}

for(int i=0;i<2;i++){
	p[bus_curr_wp[i]].getPosition(bus_x[i],bus_y[i],bus_dir[i]);				// same but for the busses
	p[bus_curr_wp[i]].getNextBus(bus_next_wp[i],bus_next_wp2[i],bus_next_wp3[i]);					
	p[bus_next_wp[i]].getPosition(bus_next_x[i],bus_next_y[i],bus_next_dir[i]);
}

for(int k=0;k<18;k++){
	t[k].getPosition(tx[k],ty[k]);
}

Car car1("images/vehicles/car1.png",car_x[0],car_y[0],car_dir[0],car_next_dir[0],car_curr_wp[0],car_next_wp[0],car_next_x[0],car_next_y[0],tx,ty);
Car car2("images/vehicles/car2.png",car_x[1],car_y[1],car_dir[1],car_next_dir[1],car_curr_wp[1],car_next_wp[1],car_next_x[1],car_next_y[1],tx,ty);
Car car3("images/vehicles/car3.png",car_x[2],car_y[2],car_dir[2],car_next_dir[2],car_curr_wp[2],car_next_wp[2],car_next_x[2],car_next_y[2],tx,ty);
Car car4("images/vehicles/car4.png",car_x[3],car_y[3],car_dir[3],car_next_dir[3],car_curr_wp[3],car_next_wp[3],car_next_x[3],car_next_y[3],tx,ty);
Car car5("images/vehicles/car5.png",car_x[4],car_y[4],car_dir[4],car_next_dir[4],car_curr_wp[4],car_next_wp[4],car_next_x[4],car_next_y[4],tx,ty);
Car car6("images/vehicles/car6.png",car_x[5],car_y[5],car_dir[5],car_next_dir[5],car_curr_wp[5],car_next_wp[5],car_next_x[5],car_next_y[5],tx,ty);

Bus bus1("images/vehicles/bus.png",bus_x[0],bus_y[0],bus_dir[0],bus_next_dir[0],bus_curr_wp[0],bus_next_wp[0],bus_next_x[0],bus_next_y[0],tx,ty);
Bus bus2("images/vehicles/bus.png",bus_x[1],bus_y[1],bus_dir[1],bus_next_dir[1],bus_curr_wp[1],bus_next_wp[1],bus_next_x[1],bus_next_y[1],tx,ty);

vehicle_x[0] = car_x[0] ;//sets the global x values of car1
vehicle_x[1] = car_x[1] ;//car2
vehicle_x[2] = car_x[2] ;//car3
vehicle_x[3] = car_x[3] ;//car4
vehicle_x[4] = car_x[4] ;//car5
vehicle_x[5] = car_x[5] ;//car6
vehicle_x[6] = bus_x[0] ;//bus1
vehicle_x[7] = bus_x[1] ;//bus2


vehicle_y[0] = car_y[0] ;//setting the global y values of car1
vehicle_y[1] = car_y[1] ;//car2
vehicle_y[2] = car_y[2] ;//car3
vehicle_y[3] = car_y[3] ;//car4
vehicle_y[4] = car_y[4] ;//car5
vehicle_y[5] = car_y[5] ;//car6
vehicle_y[6] = bus_y[0] ;//bus1
vehicle_y[7] = bus_y[1] ;//bus2

vehicleindex=0;//sets global vehicle index =0;

bus1.addStop(56);
bus1.addStop(4);
bus1.addStop(27);
bus1.addStop(42);
bus1.addStop(33);
bus1.addStop(58);

bus2.addStop(4);
bus2.addStop(27);
bus2.addStop(42);
bus2.addStop(33);
bus2.addStop(58);
bus2.addStop(56);

BusStop bus_stop[6]={
{0*239+118+15,4*239+20,0}, //cornor bottom left 0-4 road index:0
{1*239+218+15,0*239+121+15,90}, //ttop 1-0 road index:2
{4*239+118-15,1*239+218+15,180}, //t-right 4-1 road index:2
{4*239+118-15,2*239+218+15,180}, //t-right 4-2 road index:2
{1*239+118+45,2*239+218,180}, //cross 1-2 index:3
{1*239+20+15,4*239+121-15,270}, //t-bottom 1-4 road index:0
};


while (window.isOpen()) //This is the main loop, the simulation should take place within this loop
{
	while (window.pollEvent(event)){
		if (event.type == sf::Event::Closed)
		window.close();
	}	
	window.clear(sf::Color::White);
	
for(int i=0; i<25;i++){//prints roadtiles
	r[i].draw();
}

for(int i=0; i<68; i++){//prints waypoints
	p[i].draw();
}

for(int i=0;i<18;i++){//prints trafficlights
	t[i].draw();
}

for(int i=0;i<6;i++){ // prints bus stops
	bus_stop[i].draw();
}

for (int k=0;k<18;k++){//ubdates global states in every loop
	GlobalState[k] = t[k].getState();
}

car1.move();
car2.move();
car3.move();
car4.move();
car5.move();
car6.move();

car1.draw(); //prints cars
car2.draw();
car3.draw();
car4.draw();
car5.draw();
car6.draw();

bus1.move();
bus2.move();

bus1.draw();  //prints buses
bus2.draw();


timestep = timestep + 1;

for(int i=0;i<9;i++){
	t_l_g[i].simulate(timestep);
}

	window.display();
//	sf::sleep(sf::seconds(0.0001));

}
return 0;
}
