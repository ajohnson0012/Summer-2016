/**
 *  Summer Research 2016
 *  Author: Alex Johnson
 *  - with assistance/guidance from Professor Kapralos
 *
 *  The goal of this research is to develop a program, that given the
 *  geographical coordinates of the current position and a final destination,
 *  and data taken from sensors describing the surroundings, can guide a robot
 *  to the final destination if possible. Currently the program is simulating
 *  this process by hard-coding a map and simplifying the sensor shape.
**/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAPHEIGHT = 10;
const int MAPWIDTH = 10;
const int SENSORDEPTH = 3;
const int SENSORWIDTH = 1;
const char SENSORSHAPE = 'l';

/* Find the current position */
void findPos(int* x, int* y, char map[MAPHEIGHT][MAPWIDTH]) {
  for(int i=0;i<10;i++){
    for(int j=0;j<10;j++){
      if(map[i][j]=='C'){
        *x=i;
        *y=j;
      }
    }
  }
}

/* Find the final destination */
void findFin(int* x, int* y, char map[MAPHEIGHT][MAPWIDTH]) {
  for(int i=0;i<MAPHEIGHT;i++){
    for(int j=0;j<MAPWIDTH;j++){
      if(map[i][j]=='F'){
        *x=i;
        *y=j;
      }
    }
  }
}

/* Create an array that shows the line of sight */
int sight(int x, int y, int bearing, char map[MAPHEIGHT][MAPWIDTH], char vision[SENSORDEPTH*2+1][SENSORDEPTH*2+1]) {
  int speed = 0;
  char arc = 'a';
  char stairs = 's';
  char lobe = 'l';
  for(int a=0;a<((2*SENSORDEPTH)+1);a++)
    for(int b=0;b<((2*SENSORDEPTH)+1);b++)
      vision[a][b] = '?';
  int i=SENSORDEPTH;
  int j=SENSORDEPTH;
  vision[i][j] = 'C';
  switch(SENSORSHAPE){
    case 'a':
    break;
    case 's':
    break;
    case 'l':
    switch(bearing) {
      case 0:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((x-k)<0)
          break;
        vision[i-k][j] = map[x-k][y];
        if (map[x-k][y] == 'X')
          break;
        speed++;
      }
      break;
      case 1:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x-k)<0)||((y+k)>(MAPWIDTH-1)))
          break;
        vision[i-k][j+k] = map[x-k][y+k];
        if (map[x-k][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 2:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((y+k)>(MAPWIDTH-1))
          break;
        vision[i][j+k] = map[x][y+k];
        if (map[x][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 3:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x+k)>(MAPHEIGHT-1))||((y+k)>(MAPWIDTH-1)))
          break;
        vision[i+k][j+k] = map[x+k][y+k];
        if (map[x+k][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 4:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((x+k)>(MAPHEIGHT-1))
          break;
        vision[i+k][j] = map[x+k][y];
        if (map[x+k][y] == 'X')
          break;
        speed++;
      }
      break;
      case 5:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x+k)>(MAPHEIGHT))||((y-k)<0))
          break;
        vision[i+k][j-k] = map[x+k][y-k];
        if (map[x+k][y-k] == 'X')
          break;
        speed++;
      }
      break;
      case 6:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((y-k)<0)
          break;
        vision[i][j-k] = map[x][y-k];
        if (map[x][y-k] == 'X')
          break;
        speed++;
      }
      break;
      case 7:
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x-k)<0)||((y-k)<0))
          break;
        vision[i-k][j-k] = map[x-k][y-k];
        if (map[x-k][y-k] == 'X')
          break;
        speed++;
      }
      break;
    }
    break;
  }
  return speed;
}

/* Determine which way to aim based relationship between current and final positions */
int aim(int x, int y, int fx, int fy) {
  //cout << "Current Position: (" << x << "," << y << ")" << endl;
  //cout << "Destination: (" << fx << "," << fy << ")" << endl;
  int bearing = -1;
  if(x<fx) {
    if(y<fy) bearing = 3;
    else if(y==fy) bearing = 4;
    else if(y>fy) bearing = 5;
  }
  if(x==fx) {
    if(y<fy) bearing = 2;
    else if(y>fy) bearing = 6;
  }
  else if(x>fx) {
    if(y<fy) bearing = 1;
    else if(y==fy) bearing = 0;
    else if(y>fy) bearing = 7;
  }
  return bearing;
}

/* Determine the desired distance to travel */
int howFar(int x, int y, int fx, int fy, int speed, int bearing) {
  int min = speed;
  switch(bearing) {
    case 0:
    if((x-fx)<min)
      min = (x-fx);
    break;
    case 1:
    if((fy-y)<min)
      min = (fy-y);
    if((x-fx)<min)
      min = (x-fx);
    break;
    case 2:
    if((fy-y)<min)
      min = (fy-y);
    break;
    case 3:
    if((fy-y)<min)
      min = (fy-y);
    if((fx-x)<min)
      min = (fx-x);
    break;
    case 4:
    if((fx-x)<min)
      min = (fx-x);
    break;
    case 5:
    if((y-fy)<min)
      min = (y-fy);
    if((fx-x)<min)
      min = (fx-x);
    break;
    case 6:
    if((y-fy)<min)
      min = (y-fy);
    break;
    case 7:
    if((y-fy)<min)
      min = (y-fy);
    if((x-fx)<min)
      min = (x-fx);
    break;
  }
  return min;
}

/* Move the desired distance and update the map */
void move(int* x, int* y, int bearing, int distance, char map[MAPHEIGHT][MAPWIDTH]) {
  map[*x][*y] = ' ';
  switch(bearing) {
    case 0:
    *x-=distance;
    map[*x][*y] = 'C';
    break;
    case 1:
    *x-=distance;
    *y+=distance;
    map[*x][*y] = 'C';
    break;
    case 2:
    *y+=distance;
    map[*x][*y] = 'C';
    break;
    case 3:
    *x+=distance;
    *y+=distance;
    map[*x][*y] = 'C';
    break;
    case 4:
    *x+=distance;
    map[*x][*y] = 'C';
    break;
    case 5:
    *x+=distance;
    *y-=distance;
    map[*x][*y] = 'C';
    break;
    case 6:
    *y-=distance;
    map[*x][*y] = 'C';
    break;
    case 7:
    *x-=distance;
    *y-=distance;
    map[*x][*y] = 'C';
    break;
  }
}

/* Print out current state of the map */
void printMap(char map[MAPHEIGHT][MAPWIDTH]) {
  for(int i=0;i<MAPWIDTH;i++)
    cout << '-' << flush;
  cout << "" << endl;
  for(int i=0;i<MAPHEIGHT;i++) {
    for(int j=0;j<MAPWIDTH;j++) {
      cout << map[i][j] << flush;
    }
    cout << "" << endl;
  }
}

int redirect(int x, int y, int fx, int fy, int* bearing) {
  int turn;
  switch(*bearing) {
    case 0:
    if(y>=fy) {
      *bearing = 7;
      turn = 0;
    }
    else {
      *bearing = 1;
      turn = 1;
    }
    break;
    case 1:
    if((x-fx)>=(fy-y)) {
      *bearing = 0;
      turn = 0;
    }
    else {
      *bearing = 2;
      turn = 1;
    }
    break;
    case 2:
    if(x>=fx) {
      *bearing = 1;
      turn = 0;
    }
    else {
      *bearing = 3;
      turn = 1;
    }
    break;
    case 3:
    if((fy-y)>=(fx-x)) {
      *bearing = 2;
      turn = 0;
    }
    else {
      *bearing = 4;
      turn = 1;
    }
    break;
    case 4:
    if(fy>=y) {
      *bearing = 3;
      turn = 0;
    }
    else {
      *bearing = 5;
      turn = 1;
    }
    break;
    case 5:
    if((fx-x)>=(y-fy)) {
      *bearing = 4;
      turn = 0;
    }
    else {
      *bearing = 6;
      turn = 1;
    }
    break;
    case 6:
    if(fx>=x) {
      *bearing = 5;
      turn = 0;
    }
    else {
      *bearing = 7;
      turn = 1;
    }
    break;
    case 7:
    if((y-fy)>=(x-fx)) {
      *bearing = 6;
      turn = 0;
    }
    else {
      *bearing = 0;
      turn = 1;
    }
    break;
  }
  return turn;
}

/* Uses functions above to move to the final destination (with no obstacles) */
void process(char map[MAPHEIGHT][MAPWIDTH]) {
  int x,y,fx,fy;
  findPos(&x,&y,map);
  findFin(&fx,&fy,map);
  //cout << x << fx << endl;
  //cout << y << fy << endl;
  char vision[2*SENSORDEPTH+1][2*SENSORDEPTH+1];
  while((x!=fx)||(y!=fy)) {
    int course = aim(x,y,fx,fy);
    int speed = sight(x,y,course,map,vision);
    int length = howFar(x,y,fx,fy,speed,course);
    //cout << "Old Direction: " << course << endl;
    if(length==0) {
      int oldCourse = course;
      int oldX = x;
      int oldY = y;
      int turn = redirect(x,y,fx,fy,&course);
      //cout << "New Direction: " << course << endl;
      while(course!=oldCourse) {
        if(turn==0) {
          cout << "Turn Left" << endl;
          course = (course+1)%8;
          int sp = sight(x,y,course,map,vision);
          while(sp>0) {
            course = (course+1)%8;
            sp = sight(x,y,course,map,vision);
          }
          course = (course+7)%8;
          sp = sight(x,y,course,map,vision);
          cout << course << endl;
          while(sp==0) {
            course = (course+7)%8;
            sp = sight(x,y,course,map,vision);
          }
          move(&x,&y,course,1,map);
          printMap(map);
        }
        else if(turn==1) {
          cout << "Turn Right" << endl;
          course = (course+7)%8;
          int sp = sight(x,y,course,map,vision);
          while(sp>0) {
            course = (course+7)%8;
            sp = sight(x,y,course,map,vision);
          }
          course = (course+1)%8;
          sp = sight(x,y,course,map,vision);
          while(sp==0) {
            course = (course+1)%8;
            sp = sight(x,y,course,map,vision);
          }
          move(&x,&y,course,1,map);
          printMap(map);
        }
      }
    }
    move(&x,&y,course,length,map);
    printMap(map);
    //findPos(&x,&y,map);
    //findFin(&fx,&fy,map);
  }
}

int main() {
  //char vision[2*SENSORDEPTH+1][2*SENSORDEPTH+1];
  char map[MAPHEIGHT][MAPWIDTH] = 
  {
    {' ',' ','X',' ',' ',' ',' ','F','X',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ','X',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ','X','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {'C',' ','X',' ',' ',' ',' ',' ',' ',' '},
  };
  /*int x,y,fx,fy;
  findPos(&x,&y,map);
  findFin(&fx,&fy,map);
  int course = aim(x,y,fx,fy);
  int speed = sight(x,y,course,map,vision);
  for(int i=0;i<(2*SENSORDEPTH)+1;i++) {
    for(int j=0;j<(2*SENSORDEPTH)+1;j++) {
      cout << vision[i][j] << flush;
    }
    cout << "" << endl;
  }
  cout << "Open: " << speed << endl;
  int length = howFar(x,y,fx,fy,speed,course);
  cout << "Travel: " << length << endl;
  move(x,y,course,length,map);
  printMap(map);*/
  printMap(map);
  process(map);
  
  return 0;
}
