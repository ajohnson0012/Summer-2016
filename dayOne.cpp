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
#include <vector>
#include <random>
using namespace std;

const int MAPHEIGHT = 10;
const int MAPWIDTH = 10;
const int SENSORDEPTH = 3;
const int SENSORWIDTH = 1;
const char SENSORSHAPE = 'l';
char map[MAPHEIGHT][MAPWIDTH];
int x, y;
int fx, fy;

void createMap() {
  int marker;
  random_device rd;
  for(int i=0;i<MAPHEIGHT;i++) {
    for(int j=0;j<MAPWIDTH;j++) {
      marker = rd()%10;
      if(marker<=3) map[i][j]='X';
      else map[i][j]=' ';
    }
  }
  int x,y,fx,fy;
  fx = rd()%MAPHEIGHT;
  fy = rd()%MAPWIDTH;
  do {
    x = rd()%MAPHEIGHT;
    y = rd()%MAPWIDTH;
  } while((x==fx)&&(y==fy));
  map[x][y] = 'C';
  map[fx][fy] = 'F';
}

/* Find the current position */
void findPos() {
  for(int i=0;i<10;i++){
    for(int j=0;j<10;j++){
      if(map[i][j]=='C'){
        x=i;
        y=j;
      }
    }
  }
}

/* Find the final destination */
void findFin() {
  for(int i=0;i<MAPHEIGHT;i++){
    for(int j=0;j<MAPWIDTH;j++){
      if(map[i][j]=='F'){
        fx=i;
        fy=j;
      }
    }
  }
}

/* Create an array that shows the line of sight */
int sight(int bearing, char vision[SENSORDEPTH*2+1][SENSORDEPTH*2+1]) {
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
      if((x-1)<0)
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((x-k)<0) {
          break;
        }
        vision[i-k][j] = map[x-k][y];
        if (map[x-k][y] == 'X')
          break;
        speed++;
      }
      break;
      case 1:
      if(((x-1)<0)||((y+1)>(MAPWIDTH-1)))
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x-k)<0)||((y+k)>(MAPWIDTH-1))) {
          break;
        }
        vision[i-k][j+k] = map[x-k][y+k];
        if (map[x-k][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 2:
      if((y+1)>(MAPWIDTH-1))
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((y+k)>(MAPWIDTH-1)) {
          break;
        }
        vision[i][j+k] = map[x][y+k];
        if (map[x][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 3:
      if(((x+1)>(MAPHEIGHT-1))||((y+1)>(MAPWIDTH-1)))
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x+k)>(MAPHEIGHT-1))||((y+k)>(MAPWIDTH-1))) {
          break;
        }
        vision[i+k][j+k] = map[x+k][y+k];
        if (map[x+k][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 4:
      if((x+1)>(MAPHEIGHT-1))
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((x+k)>(MAPHEIGHT-1)) {
          break;
        }
        vision[i+k][j] = map[x+k][y];
        if (map[x+k][y] == 'X')
          break;
        speed++;
      }
      break;
      case 5:
      if(((x+1)>(MAPHEIGHT-1))||((y-1)<0))
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x+k)>(MAPHEIGHT-1))||((y-k)<0)) {
          break;
        }
        vision[i+k][j-k] = map[x+k][y-k];
        if (map[x+k][y-k] == 'X')
          break;
        speed++;
      }
      break;
      case 6:
      if((y-1)<0)
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if((y-k)<0) {
          break;
        }
        vision[i][j-k] = map[x][y-k];
        if (map[x][y-k] == 'X')
          break;
        speed++;
      }
      break;
      case 7:
      if(((x-1)<0)||((y-1)<0))
        speed = -1;
      for(int k=1;k<=SENSORDEPTH;k++) {
        if(((x-k)<0)||((y-k)<0)) {
          break;
        }
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
int aim() {
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
int howFar(int speed, int bearing) {
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
void move(int bearing, int distance) {
  map[x][y] = ' ';
  switch(bearing) {
    case 0:
    x-=distance;
    map[x][y] = 'C';
    break;
    case 1:
    x-=distance;
    y+=distance;
    map[x][y] = 'C';
    break;
    case 2:
    y+=distance;
    map[x][y] = 'C';
    break;
    case 3:
    x+=distance;
    y+=distance;
    map[x][y] = 'C';
    break;
    case 4:
    x+=distance;
    map[x][y] = 'C';
    break;
    case 5:
    x+=distance;
    y-=distance;
    map[x][y] = 'C';
    break;
    case 6:
    y-=distance;
    map[x][y] = 'C';
    break;
    case 7:
    x-=distance;
    y-=distance;
    map[x][y] = 'C';
    break;
  }
}

/* Print out current state of the map */
void printMap() {
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

/* Changes course when it hits an obstacle based on destination */
int redirect(int* bearing) {
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

/* Navigates around obstacles */
int obstacle(vector<int> oldX, vector<int> oldY, char vision[2*SENSORDEPTH+1][2*SENSORDEPTH+1], int course) {
  int success = 1;
  int goal = course;
  int turn = redirect(&course);
  int startX[] = {x,x,x};
  int startY[] = {y,y,y};
  switch(goal) {
    case 1:
    startX[1]--;
    startY[2]++;
    break;
    case 3:
    startX[1]++;
    startY[2]++;
    break;
    case 5:
    startX[1]++;
    startY[2]--;
    break;
    case 7:
    startX[1]--;
    startY[2]--;
    break;
  }
  int deadEnd = 0;
  int backTrack = 0;
  int numMoves = 0;
  while(((sight(aim(),vision)<=0)||(aim()==((course+4)%8)))||(backTrack==1)) {
    choice:
    if(deadEnd==2) {
      success = 0;
      return success;
      //goto end;
    }
    if(turn==0) {
      course = (course+1)%8;
      int sp = sight(course,vision);
      while(sp>0) {
        course = (course+1)%8;
        sp = sight(course,vision);
      }
      course = (course+7)%8;
      sp = sight(course,vision);
      int spin = 0;
      while(sp==0) {
        course = (course+7)%8;
        sp = sight(course,vision);
        spin++;
        if(spin>8) {
          success = 0;
          return success;
          //goto end;
        }
      }
      if(sp==-1) {
        while(sight(course,vision)!=0)
          course = (course+1)%8;
        turn = 1;
        deadEnd++;
        backTrack = 1;
        goto choice;
      }
      move(course,1);
      numMoves++;
      for(int i=0;i<3;i++) {
        if((x==startX[i])&&(y==startY[i])) backTrack = 0;
      }
      int goal = aim();
      printMap();
      for(int i=0;i<3;i++) {
        //cout << x << " " << startX[i] << endl;
        //cout << y << " " << startY[i] << endl;
        if(((x==startX[i])&&(y==startY[i]))&&(deadEnd==0)&&(numMoves>1)) {
          //cout << "HEY" << endl;
          success = 0;
          return success;
          //goto end;
        }
      }
      if(deadEnd==0) {
        for(int i=0;i<oldX.size();i++) {
          if((x==oldX.at(i))&&(y==oldY.at(i))) {
            course = (course+2)%8;
            turn = 1;
            goto choice;
          }
        }
      }
    }
    else if(turn==1) {
      course = (course+7)%8;
      int sp = sight(course,vision);
      while(sp>0) {
        course = (course+7)%8;
        sp = sight(course,vision);
      }
      course = (course+1)%8;
      sp = sight(course,vision);
      int spin = 0;
      while(sp==0) {
        course = (course+1)%8;
        sp = sight(course,vision);
        if(spin>8) {
          success = 0;
          return success;
          //goto end;
        }
      }
      if(sp==-1) {
        while(sight(course,vision)!=0)
          course = (course+7)%8;
        turn = 0;
        deadEnd++;
        backTrack = 1;
        goto choice;
      }
      move(course,1);
      numMoves++;
      for(int i=0;i<3;i++) {
        if((x==startX[i])&&(y==startY[i])) backTrack = 0;
      }
      printMap();
      for(int i=0;i<3;i++) {
        if(((x==startX[i])&&(y==startY[i]))&&(deadEnd==0)&&(numMoves>1)) {
          success = 0;
          return success;
          //goto end;
        }
      }
      if(deadEnd==0) {
        for(int i=0;i<oldX.size();i++) {
          if((x==oldX.at(i))&&(y==oldY.at(i))) {
            course = (course+6)%8;
            turn = 0;
            goto choice;
          }
        }
      }
    }
  }
  oldX.push_back(x);
  oldY.push_back(y);
  return success; // goto end;
}

/* Uses functions above to move to the final destination (with no obstacles) */
int process() {
  findPos();
  findFin();
  cout << x << " " << y << endl;
  cout << fx << " " << fy << endl;
  char vision[2*SENSORDEPTH+1][2*SENSORDEPTH+1];
  vector<int> oldX;
  vector<int> oldY;
  int success = 1;
  while((x!=fx)||(y!=fy)) {
    int course = aim();
    int speed = sight(course,vision);
    int length = howFar(speed,course);
    if(length==0) {
      success = obstacle(oldX, oldY, vision, course);
      if(success==0) goto end;
    }
    else {
      move(course,length);
      printMap();
    }
  }
  end:
  if(success==0) return 0;
  else return 1;
}

int main() {
  createMap();
  printMap();
  int result = process();
  for(int i=0;i<MAPWIDTH;i++) cout << "-" << flush;
  cout << "" << endl;
  if(result==1) cout << "The object has reached its destination successfully." << endl;
  else cout << "The object was unable to reach the destination from its origin." << endl;
  return 0;
}
