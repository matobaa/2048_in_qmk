// gameboard

#define MAX_X (4)
#define MAX_Y (4)

char matrix[MAX_X][MAX_Y];
char matrix_backup[MAX_X][MAX_Y];
int score = 0;

void backup(void) {
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      matrix_backup[x][y] = matrix[x][y];
    }
  }
}

void recovery(void) {
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      matrix[x][y] = matrix_backup[x][y];
    }
  }
}

bool canmove(void) {
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      if(matrix[x][y]==0) return true;
    }
  }
  int last = 0;
  for (int x = 0; x < MAX_Y; x++) {
    last = matrix[x][0];
    for (int y = 1; y < MAX_X; y++) {
      if(last == matrix[x][y]) return true;
      last = matrix[x][y];
    }
  }
  for (int y = 0; y < MAX_X; y++) {
    last = matrix[0][y];
    for (int x = 1; x < MAX_Y; x++) {
      if(last == matrix[x][y]) return true;
      last = matrix[x][y];
    }
  }
  return false;
}

bool newtile(void) {
  int qtyempty = 0;
  // count
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      if(matrix[x][y] == 0) {
	qtyempty++;
      }
    }
  }
  if(qtyempty==0)
    return false;
  int newindex = rand() % qtyempty;
  // place a new tile
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      if(matrix[x][y] == 0) {
	if(newindex == 0) {
	  int newvalue = (rand() % 5) == 0 ? 2 : 1;
	  matrix[x][y] = newvalue;
	  return true;
	}
	newindex--;
      }
    }
  }
  return false;
}
  
void reset_board(void) {
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      matrix[x][y] = 0;
    }
  }
  newtile();
  newtile();

  score = 0;
}

void dump_board(void) {
  for (int x = 0; x < MAX_Y; x++) {
    for (int y = 0; y < MAX_X; y++) {
      send_char(matrix[x][y] < 10 ? ('0'+ matrix[x][y]) : ('A' + matrix[x][y] - 10));
    }
    send_char('\n');
  }
  send_char('\n');
}

// { LEFT, DOWN, UP, RIGHT };
bool move(int direction) {
  bool result = false;
  if(direction == 0) {  // left
    for(int x=0; x<MAX_X; x++) {
      for(int y=0; y<MAX_Y-1; y++) {
	if(matrix[x][y] == 0) {
	  for(int dy=y+1; dy<MAX_Y; dy++) {
	    if(matrix[x][dy] != 0) {
	      matrix[x][y] = matrix[x][dy];
	      matrix[x][dy] = 0;
	      result = true;
	      break;
	    }
	  }
	}
	if(matrix[x][y] != 0) {
	  for(int dy=y+1; dy<MAX_Y; dy++) {
	    if(matrix[x][dy] == matrix[x][y]) {
	      matrix[x][y] += 1;
	      matrix[x][dy] = 0;
	      result = true;
	      break;
	    }
	    if(matrix[x][dy] != 0) break;
	  }
	}
      }
    }
  }
  if(direction == 2) {  // down
    for(int y=0; y<MAX_Y; y++) {
      for(int x=0; x<MAX_X-1; x++) {
	if(matrix[x][y] == 0) {
	  for(int dx=x+1; dx<MAX_X; dx++) {
	    if(matrix[dx][y] != 0) {
	      matrix[x][y] = matrix[dx][y];
	      matrix[dx][y] = 0;
	      result = true;
	      break;
	    }
	  }
	}
	if(matrix[x][y] != 0) {
	  for(int dx=x+1; dx<MAX_X; dx++) {
	    if(matrix[dx][y] == matrix[x][y]) {
	      matrix[x][y] += 1;
	      matrix[dx][y] = 0;
	      result = true;
	      break;
	    }
	    if(matrix[dx][y] != 0) break;
	  }
	}
      }
    }
  }
  if(direction == 3) {  // right
    for(int x=0; x<MAX_X; x++) {
      for(int y=MAX_Y-1; 0<y; y--) {
	if(matrix[x][y] == 0) {
	  for(int dy=y-1; 0<=dy; dy--) {
	    if(matrix[x][dy] != 0) {
	      matrix[x][y] = matrix[x][dy];
	      matrix[x][dy] = 0;
	      result = true;
	      break;
	    }
	  }
	}
	if(matrix[x][y] != 0) {
	  for(int dy=y-1; 0<=dy; dy--) {
	    if(matrix[x][dy] == matrix[x][y]) {
	      matrix[x][y] += 1;
	      matrix[x][dy] = 0;
	      result = true;
	      break;
	    }
	    if(matrix[x][dy] != 0) break;
	  }
	}
      }
    }
  }
  if(direction == 1) {  // up
    for(int y=0; y<MAX_Y; y++) {
      for(int x=MAX_X-1; 0<x; x--) {
	if(matrix[x][y] == 0) {
	  for(int dx=x-1; 0<=dx; dx--) {
	    if(matrix[dx][y] != 0) {
	      matrix[x][y] = matrix[dx][y];
	      matrix[dx][y] = 0;
	      result = true;
	      break;
	    }
	  }
	}
	if(matrix[x][y] != 0) {
	  for(int dx=x-1; 0<=dx; dx--) {
	    if(matrix[dx][y] == matrix[x][y]) {
	      matrix[x][y] += 1;
	      matrix[dx][y] = 0;
	      result = true;
	      break;
	    }
	    if(matrix[dx][y] != 0) break;
	  }
	}
      }
    }
  }
  return result;
}
