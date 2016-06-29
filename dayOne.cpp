#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAPHEIGHT = 10;
const int MAPWIDTH = 10;

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

int sight(int bearing, char shape, int distance, char map[MAPHEIGHT][MAPWIDTH], char vision[9][9]) {
  int x, y;
  int speed = 0;
  findPos(&x,&y,map);
  char arc = 'a';
  char stairs = 's';
  char lobe = 'l';
  for(int a=0;a<((2*distance)+1);a++)
    for(int b=0;b<((2*distance)+1);b++)
      vision[a][b] = '?';
  int i=distance;
  int j=distance;
  //char position = static_cast<char>(bearing);
  vision[i][j] = 'o';
  switch(shape){
    case 'a':
    break;
    case 's':
    break;
    case 'l':
    //cout << "line" << endl;
    switch(bearing) {
      case 0:
      for(int k=1;k<=distance;k++) {
        if((x-k)<0)
          break;
        vision[i-k][j] = map[x-k][y];
        if (map[x-k][y] == 'X')
          break;
        speed++;
      }
      break;
      case 1:
      for(int k=1;k<=distance;k++) {
        if(((x-k)<0)||((y+k)>(MAPWIDTH-1)))
          break;
        vision[i-k][j+k] = map[x-k][y+k];
        if (map[x-k][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 2:
      for(int k=1;k<=distance;k++) {
        if((y+k)>(MAPWIDTH-1))
          break;
        vision[i][j+k] = map[x][y+k];
        if (map[x][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 3:
      for(int k=1;k<=distance;k++) {
        if(((x+k)>(MAPHEIGHT-1))||((y+k)>(MAPWIDTH-1)))
          break;
        vision[i+k][j+k] = map[x+k][y+k];
        if (map[x+k][y+k] == 'X')
          break;
        speed++;
      }
      break;
      case 4:
      for(int k=1;k<=distance;k++) {
        if((x+k)>(MAPHEIGHT-1))
          break;
        vision[i+k][j] = map[x+k][y];
        if (map[x+k][y] == 'X')
          break;
        speed++;
      }
      break;
      case 5:
      for(int k=1;k<=distance;k++) {
        if(((x+k)>(MAPHEIGHT))||((y-k)<0))
          break;
        vision[i+k][j-k] = map[x+k][y-k];
        if (map[x+k][y-k] == 'X')
          break;
        speed++;
      }
      break;
      case 6:
      for(int k=1;k<=distance;k++) {
        if((y-k)<0)
          break;
        vision[i][j-k] = map[x][y-k];
        if (map[x][y-k] == 'X')
          break;
        speed++;
      }
      break;
      case 7:
      for(int k=1;k<=distance;k++) {
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

int aim(char map[MAPHEIGHT][MAPWIDTH]) {
  int x, y, fx, fy;
  findPos(&x,&y,map);
  findFin(&fx,&fy,map);
  cout << "Current Position: (" << x << "," << y << ")" << endl;
  cout << "Destination: (" << fx << "," << fy << ")" << endl;
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

void move(int speed, char map[MAPHEIGHT][MAPWIDTH]) {
  
}

int main() {
  const int depth = 4;
  const int width = 1;
  const char shape = 'l';
  char vision[2*depth+1][2*depth+1];
  char map[MAPHEIGHT][MAPWIDTH] = 
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
    {'C',' ','X',' ',' ',' ',' ',' ',' ',' '},
  };
  int course = aim(map);
  int speed = sight(course,shape,depth,map,vision);
  for(int i=0;i<(2*depth)+1;i++) {
    for(int j=0;j<(2*depth)+1;j++) {
      cout << vision[i][j] << flush;
    }
    cout << "" << endl;
  }
  cout << speed << endl;
  cout << "Map Dimensions: " << MAPWIDTH << "x" << MAPHEIGHT << endl;
  return 0;
}
