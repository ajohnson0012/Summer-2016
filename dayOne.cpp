#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAPHEIGHT = 10;
const int MAPWIDTH = 10;
const int SENSORDEPTH = 3;
const int SENSORWIDTH = 1;
const char SENSORSHAPE = 'l';

/*char** createMap(char map[10][10]) {
  map = 
  {
    {' ',' ','X',' ',' ',' ',' ',' ','X','F'},
    {' ',' ',' ',' ',' ',' ',' ',' ','X',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
    {'0',' ','X',' ',' ',' ',' ',' ',' ',' '},
  };
  return map;
}*/

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

int aim(int x, int y, int fx, int fy) {
  //cout << "Current Position: (" << x << "," << y << ")" << endl;
  //cout << "Destination: (" << fx << "," << fy << ")" << endl;
  int bearing = -1;
  if (x<fx) {
    if (y<fy) bearing = 3;
    else if (y==fy) bearing = 4;
    else if (y>fy) bearing = 5;
  }
  if (x==fx) {
    if (y<fy) bearing = 2;
    else if (y>fy) bearing = 6;
  }
  else if (x>fx) {
    if (y<fy) bearing = 1;
    else if (y==fy) bearing = 0;
    else if (y>fy) bearing = 7;
  }
  return bearing;
}

int howFar(int x, int y, int fx, int fy, int speed, int bearing) {
  int min = speed;
  switch(bearing) {
    case 0:
    if ((x-fx)<min)
      min = (x-fx);
    break;
    case 1:
    if ((fy-y)<min)
      min = (fy-y);
    if ((x-fx)<min)
      min = (x-fx);
    break;
    case 2:
    if ((fy-y)<min)
      min = (fy-y);
    break;
    case 3:
    if ((fy-y)<min)
      min = (fy-y);
    if ((fx-x)<min)
      min = (fx-x);
    break;
    case 4:
    if ((fx-x)<min)
      min = (fx-x);
    break;
    case 5:
    if ((y-fy)<min)
      min = (y-fy);
    if ((fx-x)<min)
      min = (fx-x);
    break;
    case 6:
    if ((y-fy)<min)
      min = (y-fy);
    break;
    case 7:
    if ((y-fy)<min)
      min = (y-fy);
    if ((x-fx)<min)
      min = (x-fx);
    break;
  }
  return min;
}

void move(int x, int y, int bearing, int distance, char map[MAPHEIGHT][MAPWIDTH]) {
  map[x][y] = ' ';
  switch(bearing) {
    case 0:
    map[x-distance][y] = 'C';
    break;
    case 1:
    map[x-distance][y+distance] = 'C';
    break;
    case 2:
    map[x][y+distance] = 'C';
    break;
    case 3:
    map[x+distance][y+distance] = 'C';
    break;
    case 4:
    map[x+distance][y] = 'C';
    break;
    case 5:
    map[x+distance][y-distance] = 'C';
    break;
    case 6:
    map[x][y-distance] = 'C';
    break;
    case 7:
    map[x-distance][y-distance] = 'C';
    break;
  }
}

void printMap(char map[MAPHEIGHT][MAPWIDTH]) {
  for (int i=0;i<MAPWIDTH;i++)
    cout << '-' << flush;
  cout << "" << endl;
  for(int i=0;i<MAPHEIGHT;i++) {
    for(int j=0;j<MAPWIDTH;j++) {
      cout << map[i][j] << flush;
    }
    cout << "" << endl;
  }
}

void process(char map[MAPHEIGHT][MAPWIDTH]) {
  int x,y,fx,fy;
  findPos(&x,&y,map);
  findFin(&fx,&fy,map);
  cout << x << fx << endl;
  cout << y << fy << endl;
  while((x!=fx)||(y!=fy)) {
    int course = aim(x,y,fx,fy);
    char vision[2*SENSORDEPTH+1][2*SENSORDEPTH+1];
    int speed = sight(x,y,course,map,vision);
    int length = howFar(x,y,fx,fy,speed,course);
    move(x,y,course,length,map);
    printMap(map);
    findPos(&x,&y,map);
    findFin(&fx,&fy,map);
  }
}

int main() {
  //char vision[2*SENSORDEPTH+1][2*SENSORDEPTH+1];
  char map[MAPHEIGHT][MAPWIDTH] = 
  {
    {' ',' ','X',' ',' ',' ',' ',' ','X',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ','X',' '},
    {' ','F',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ','X',' ',' ',' ',' ',' ',' ',' '},
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
  process(map);
  
  return 0;
}
