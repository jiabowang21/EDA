#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Temptation


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
void dfs(const vector<vector<char> > &tablero, int i, int j, int &x1, int &y1, int &distance, char c1, char c2, char c3) {
  int n = tablero.size();
  int m = tablero[0].size();
  vector< vector <bool> > vist(n, vector<bool>(m, false));
  queue <pair <pair <int, int>, int> > q;
  q.push(make_pair(make_pair(i, j), 0));
  int x = 0, y = 0, d = 0;
  bool encontrado = false;
  while (!q.empty() and !encontrado) {
    x = (q.front().first).first;
    y = (q.front().first).second;
    d = q.front().second;
    Pos p = Pos(x, y);
    q.pop();
    if ((tablero[x][y] == c1 and cell(p).b_owner == -1) or (tablero[x][y] == c2 and cell(p).b_owner == -1) or (tablero[x][y] == c3 and cell(p).b_owner == -1)) {
      encontrado = true;
    }
    else if (!vist[x][y]) {
      vist[x][y] = true;
      if (x > 0 and tablero[x - 1][y] != 'B' and tablero[x - 1][y] != 'x') q.push(make_pair(make_pair(x - 1, y), d + 1));
      if (x < n - 1 and tablero[x + 1][y] != 'B' and tablero[x + 1][y] != 'x') q.push(make_pair(make_pair(x + 1, y), d + 1));
      if (y > 0 and tablero[x][y - 1] != 'B' and tablero[x][y - 1] != 'x') q.push(make_pair(make_pair(x, y - 1), d + 1));
      if (y < m - 1 and tablero[x][y + 1] != 'B' and tablero[x][y + 1] != 'x') q.push(make_pair(make_pair(x, y + 1), d + 1));
    }
  }
  x1 = x;
  y1 = y;
  if (encontrado) distance = d;
  else distance = 0;
}

void dibujar(vector<vector<char> > &tablero, int n, int m) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (cell(i, j).type == Street) tablero[i][j] = '.'; //carrer
      if (cell(i, j).type == Building) tablero[i][j] = 'B'; //edifici
      if (cell(i, j).bonus == Money) tablero[i][j] = 'M'; //diners
      if (cell(i, j).bonus == Food) tablero[i][j] = 'F'; //menjar
      if (cell(i, j).weapon == Gun) tablero[i][j] = 'g'; //pistola
      if (cell(i, j).weapon == Bazooka) tablero[i][j] = 'b'; //bazuca
      if (cell(i, j).b_owner != -1 and cell(i, j).b_owner != me()) {
        tablero[i][j] = 'x'; //barricades enemigues
      }
      if (cell(i, j).id != -1 and citizen(cell(i, j).id).player == me()) tablero[i][j] = 'e'; //yo
      if (cell(i, j).id != -1 and citizen(cell(i, j).id).player != me()) {
        if (citizen(cell(i, j).id).type == Builder) tablero[i][j] = 'p'; //enemy builder
        else if (citizen(cell(i, j).id).type == Warrior) {
          if (citizen(cell(i, j).id).weapon == Hammer) tablero[i][j] = 'w'; //enemy warrior hammer
          else if (citizen(cell(i, j).id).weapon == Gun) tablero[i][j] = 'G'; //enemy warrior gun
          else if (citizen(cell(i, j).id).weapon == Bazooka) tablero[i][j] = 'P'; //enemy warrior hammer
        }
      }
    }
  }
}
  /**
   * Play method, invoked once per each round.
   */
virtual void play () {
  double st = status(me());
  //If nearly out of time, do nothing.
  if (st >= 0.95 or st < 0) return;
  vector<Dir> dirs = {Up,Down,Left,Right};
  int n = board_rows();
  int m = board_cols();
  vector< vector <char> > tablero(n, vector<char>(m, '.'));
  dibujar(tablero, n, m);
  /* for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cerr << tablero[i][j];
    }
    cerr << endl;
  } */
  vector<int> b = builders(me()); //builders
  vector<int> w = warriors(me()); //warriors
  if (is_day()) { //during the day
    bool construir = false;
    int i;
    if (is_round_night(round() + 7)) {
      construir = true;
      i = 0;
    }
    bool perseguir = false;
    if (is_round_night(round() + 5)) {
      perseguir = true;
    }
    for (int id : b) { //action of all builders
      dibujar(tablero, n, m);
      Pos p = citizen(id).pos;
      if (construir and i < 3) {
        ++i;
        bool libre = false;
        Dir libre_dir = Down;
        for (Dir d : dirs) {
          if (cell(p + d).b_owner == me()) {
            libre = true;
            libre_dir = d;
          }
          else if (pos_ok(p + d) and cell(p + d).type != Building and cell(p + d).bonus == NoBonus and cell(p + d).weapon == NoWeapon and cell(p + d).id == -1 and cell(p + d).resistance == -1) {
            libre = true;
            libre_dir = d;
          }
        }
        if (libre) build(id, libre_dir);
      }
      else {
      // If there is food nearby, move there
        bool food_nearby = false;
        bool money_nearby = false;
        Dir food_dir = Down;
        Dir money_dir = Down;
        for (Dir d : dirs) {
          if (pos_ok(p + d) and cell(p + d).bonus == Food and cell(p + d).type != Building) {
            food_nearby = true;
            food_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).bonus == Money and cell(p + d).type != Building) {
            money_nearby = true;
            money_dir = d;
          }
        }
        if (citizen(id).life < builder_ini_life()) {
          if (food_nearby) move(id,food_dir);
          else if (money_nearby) move(id,money_dir);
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'F', 'F', 'M');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'F', 'F', 'M');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'F', 'F', 'M');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'F', 'F', 'M');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                  move(id,random_dir);
              }
            }
          }
        }
        else if (citizen(id).life == builder_ini_life()){
          if (money_nearby) move(id,money_dir);
          else if (food_nearby) move(id,food_dir);
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'M');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'M');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'M');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'M');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist );
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                 move(id,random_dir);
               }
             }
           }
         }
      }
    }
    for (int id : w) { //action of all warriors
      dibujar(tablero, n, m);
      Pos p = citizen(id).pos;
      if (perseguir and (citizen(id).weapon == Hammer or citizen(id).weapon == Bazooka)) {
        bool builder_nearby = false;
        for (Dir d : dirs) {
        if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).type == Builder) {
            builder_nearby = true;
          }
        }
        if (not builder_nearby) {
          map<int, pair<int, int>> distancias;
          int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
          int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
          int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
          int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
          if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x') {
            dfs(tablero, i0, j0, x0, y0, d0, 'p', 'p', 'p');
            distancias.insert(make_pair(d0, make_pair(x0, y0)));
          }
          if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x') {
            dfs(tablero, i1, j1, x1, y1, d1, 'p', 'p', 'p');
            distancias.insert(make_pair(d1, make_pair(x1, y1)));
          }
          if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x') {
            dfs(tablero, i2, j2, x2, y2, d2, 'p', 'p', 'p');
            distancias.insert(make_pair(d2, make_pair(x2, y2)));
          }
          if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x') {
            dfs(tablero, i3, j3, x3, y3, d3, 'p', 'p', 'p');
            distancias.insert(make_pair(d3, make_pair(x3, y3)));
          }
          auto it = distancias.begin();
          if (it->first == d3 and it->first != 0) {
            Dir dist = Right;
            move(id, dist);
          }
          else if (it->first == d2 and it->first != 0) {
            Dir dist = Left;
            move(id, dist);
          }
          else if (it->first == d1 and it->first != 0) {
            Dir dist = Down;
            move(id, dist);
          }
          else if (it->first == d0 and it->first != 0) {
            Dir dist = Up;
            move(id, dist);
          }
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x') {
              dfs(tablero, i0, j0, x0, y0, d0, 'p', 'p', 'p');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x') {
              dfs(tablero, i1, j1, x1, y1, d1, 'p', 'p', 'p');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x') {
              dfs(tablero, i2, j2, x2, y2, d2, 'p', 'p', 'p');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x') {
              dfs(tablero, i3, j3, x3, y3, d3, 'p', 'p', 'p');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
          }
        }
      }
      else {
      //If there is food nearby, move there
        bool food_nearby = false;
        bool gun_nearby = false;
        bool bazoka_nearby = false;
        bool money_nearby = false;
        Dir food_dir = Up;
        Dir bazoka_dir = Up;
        Dir gun_dir = Up;
        Dir money_dir = Up;
        for (Dir d : dirs) {
          if (pos_ok(p + d) and cell(p + d).bonus == Food and cell(p + d).type != Building) {
            food_nearby = true;
            food_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).bonus == Money and cell(p + d).type != Building) {
            money_nearby = true;
            money_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).weapon == Gun and cell(p + d).type != Building) {
            gun_nearby = true;
            gun_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).weapon == Bazooka and cell(p + d).type != Building) {
            bazoka_nearby = true;
            bazoka_dir = d;
          }
        }
        if (citizen(id).life < warrior_ini_life()) {
          if (food_nearby) move(id,food_dir);
          else if (money_nearby) move(id, money_dir);
          else if (bazoka_nearby and (citizen(id).weapon == Gun or citizen(id).weapon == Hammer)) move(id,bazoka_dir);
          else if (gun_nearby and citizen(id).weapon == Hammer) move(id,gun_dir);
          else if (citizen(id).weapon == Hammer) {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'g', 'F', 'b');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'g', 'F', 'b');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'g', 'F', 'b');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'g', 'F', 'b');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                move(id,random_dir);
              }
            }
          }
          else if (citizen(id).weapon == Gun) {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'b', 'b', 'F');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'b', 'b', 'F');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'b', 'b', 'F');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'b', 'b', 'F');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                move(id,random_dir);
              }
            }
          }
          else if (citizen(id).weapon == Bazooka) {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'F');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'F');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'F');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'F');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                move(id,random_dir);
              }
            }
          }
        }
        else {
          if (money_nearby) move(id, money_dir);
          else if (bazoka_nearby and (citizen(id).weapon == Gun or citizen(id).weapon == Hammer)) move(id,bazoka_dir);
          else if (gun_nearby and citizen(id).weapon == Hammer) move(id,gun_dir);
          else if (citizen(id).weapon == Hammer) {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'g', 'b', 'b');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'g', 'b', 'b');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'g', 'b', 'b');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'g', 'b', 'b');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                move(id,random_dir);
              }
            }
          }
          else if (citizen(id).weapon == Gun) {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'b', 'b', 'b');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'b', 'b', 'b');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'b', 'b', 'b');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'b', 'b', 'b');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                move(id,random_dir);
              }
            }
          }
          else if (citizen(id).weapon == Bazooka) {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'F');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'F');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'F');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'F');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                move(id,random_dir);
              }
            }
          }
        }
      }
    }
  }
  else if (is_night()) {
    int i = 0;
    for (int id : b) { //action of all builder
      dibujar(tablero, n, m);
      Pos p = citizen(id).pos;
      if (i < 3) {
        bool barricada_nearby = false;
        Dir barricada_dir = Down;
        for (Dir d : dirs) {
          if (cell(p + d).b_owner == me()) {
            barricada_nearby = true;
            barricada_dir = d;
          }
        }
        if (barricada_nearby) move(id, barricada_dir);
        ++i;
      }
      else {
        bool money_nearby = false;
        bool food_nearby = false;
        bool builder_nearby = false;
        bool hammer_nearby = false;
        bool gunner_nearby = false;
        bool bazooka_nearby = false;
        Dir food_dir = Up;
        Dir money_dir = Up;
        Dir builder_dir = Up;
        for (Dir d : dirs) {
          if (pos_ok(p + d) and cell(p + d).bonus == Money and cell(p + d).type != Building) {
            money_nearby = true;
            money_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).bonus == Food and cell(p + d).type != Building) {
            food_nearby = true;
            food_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).type == Builder) {
            builder_nearby = true;
            builder_dir = d;
          }
          if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).weapon == Hammer) {
            hammer_nearby = true;
          }
          if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).weapon == Gun) {
            gunner_nearby = true;
          }
          if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).weapon == Bazooka) {
            bazooka_nearby = true;
          }
        }
        if (bazooka_nearby or gunner_nearby or hammer_nearby) {
          bool yes = false;
          Dir yes_direction = Up;
          for (Dir d : dirs) {
            if (pos_ok(p + d) and cell(p + d).id == -1 and cell(p + d).type != Building) {
              yes = true;
              yes_direction = d;
            }
          }
          if (yes) {
            move(id, yes_direction);
          }
        }
        else if (money_nearby) move(id, money_dir);
        else if (builder_nearby and citizen(id).life == warrior_ini_life()) move(id, builder_dir);
        else if (food_nearby) move(id, food_dir);
        else {
          if (money_nearby) move(id,money_dir);
          else if (food_nearby) move(id,food_dir);
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'p' and tablero[p.i - 1][p.j] != 'w' and tablero[p.i - 1][p.j] != 'G' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'M');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'p' and tablero[p.i + 1][p.j] != 'w' and tablero[p.i + 1][p.j] != 'G' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'M');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'p' and tablero[p.i][p.j - 1] != 'w' and tablero[p.i][p.j - 1] != 'G' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'M');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'p' and tablero[p.i][p.j + 1] != 'w' and tablero[p.i][p.j + 1] != 'G' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'M');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
            else {
              Dir random_dir = dirs[random(0,3)];
              if (pos_ok(p + random_dir) and cell(p+random_dir).id == -1 and cell(p+random_dir).type != Building) { // Move there if no citizen or builder
                  move(id,random_dir);
              }
            }
          }
        }
      }
    }
    for (int id : w) { //action of all warriors
      dibujar(tablero, n, m);
      Pos p = citizen(id).pos;
      bool money_nearby = false;
      bool food_nearby = false;
      bool builder_nearby = false;
      bool hammer_nearby = false;
      bool gunner_nearby = false;
      bool bazooka_nearby = false;
      Dir food_dir = Up;
      Dir money_dir = Up;
      Dir builder_dir = Up;
      Dir hammer_dir = Up;
      Dir gunner_dir = Up;
      Dir bazooka_dir = Up;
      for (Dir d : dirs) {
        if (pos_ok(p + d) and cell(p + d).bonus == Money and cell(p + d).type != Building) {
          money_nearby = true;
          money_dir = d;
        }
        if (pos_ok(p + d) and cell(p + d).bonus == Food and cell(p + d).type != Building) {
          food_nearby = true;
          food_dir = d;
        }
        if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).type == Builder and cell(p + d).b_owner == -1) {
          builder_nearby = true;
          builder_dir = d;
        }
        if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).weapon == Hammer) {
          hammer_nearby = true;
          hammer_dir = d;
        }
        if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).weapon == Gun) {
          gunner_nearby = true;
          gunner_dir = d;
        }
        if (pos_ok(p + d) and cell(p + d).id != -1 and citizen(cell(p + d).id).player != me() and citizen(cell(p + d).id).weapon == Bazooka) {
          bazooka_nearby = true;
          bazooka_dir = d;
        }
      }
      if (citizen(id).weapon == Bazooka) {
        if (builder_nearby) {
          move(id, builder_dir);
        }
        else if (hammer_nearby) {
          move(id, hammer_dir);
        }
        else if (gunner_nearby) {
          move(id, hammer_dir);
        }
        else if (bazooka_nearby and citizen(id).life == warrior_ini_life()) {
          move(id, bazooka_dir);
        }
        else if (food_nearby and citizen(id).life == warrior_ini_life()) move(id, food_dir);
        else if (money_nearby) move(id,money_dir);
        else {
          map<int, pair<int, int>> distancias;
          int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
          int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
          int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
          int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
          if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x') {
            dfs(tablero, i0, j0, x0, y0, d0, 'w', 'G', 'p');
            distancias.insert(make_pair(d0, make_pair(x0, y0)));
          }
          if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x') {
            dfs(tablero, i1, j1, x1, y1, d1, 'w', 'G', 'p');
            distancias.insert(make_pair(d1, make_pair(x1, y1)));
          }
          if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x') {
            dfs(tablero, i2, j2, x2, y2, d2, 'w', 'G', 'p');
            distancias.insert(make_pair(d2, make_pair(x2, y2)));
          }
          if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x') {
            dfs(tablero, i3, j3, x3, y3, d3, 'w', 'G', 'p');
            distancias.insert(make_pair(d3, make_pair(x3, y3)));
          }
          auto it = distancias.begin();
          if (it->first == d3 and it->first != 0) {
            Dir dist = Right;
            move(id, dist);
          }
          else if (it->first == d2 and it->first != 0) {
            Dir dist = Left;
            move(id, dist);
          }
          else if (it->first == d1 and it->first != 0) {
            Dir dist = Down;
            move(id, dist);
          }
          else if (it->first == d0 and it->first != 0) {
            Dir dist = Up;
            move(id, dist);
          }
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'F');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'F');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'F');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'F');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
          }
        }
      }
      else if (citizen(id).weapon == Gun) {
        if (builder_nearby) {
          move(id, builder_dir);
        }
        else if (hammer_nearby) {
          move(id, hammer_dir);
        }
        if (bazooka_nearby) {
          bool yes = false;
          Dir yes_direction = Up;
          for (Dir d : dirs) {
            if (pos_ok(p + d) and cell(p + d).id == -1 and cell(p + d).type != Building) {
              yes = true;
              yes_direction = d;
            }
          }
          if (yes) move(id, yes_direction);
        }
        else if (gunner_nearby and citizen(id).life == warrior_ini_life()) {
          move(id, gunner_dir);
        }
        else if (food_nearby and citizen(id).life == warrior_ini_life()) move(id, food_dir);
        else if (money_nearby) move(id,money_dir);
        else {
          map<int, pair<int, int>> distancias;
          int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
          int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
          int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
          int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
          if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'P') {
            dfs(tablero, i0, j0, x0, y0, d0, 'w', 'g', 'p');
            distancias.insert(make_pair(d0, make_pair(x0, y0)));
          }
          if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'P') {
            dfs(tablero, i1, j1, x1, y1, d1, 'w', 'g', 'p');
            distancias.insert(make_pair(d1, make_pair(x1, y1)));
          }
          if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'P') {
            dfs(tablero, i2, j2, x2, y2, d2, 'w', 'g', 'p');
            distancias.insert(make_pair(d2, make_pair(x2, y2)));
          }
          if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'P') {
            dfs(tablero, i3, j3, x3, y3, d3, 'w', 'g', 'p');
            distancias.insert(make_pair(d3, make_pair(x3, y3)));
          }
          auto it = distancias.begin();
          if (it->first == d3 and it->first != 0) {
            Dir dist = Right;
            move(id, dist);
          }
          else if (it->first == d2 and it->first != 0) {
            Dir dist = Left;
            move(id, dist);
          }
          else if (it->first == d1 and it->first != 0) {
            Dir dist = Down;
            move(id, dist);
          }
          else if (it->first == d0 and it->first != 0) {
            Dir dist = Up;
            move(id, dist);
          }
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'P') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'F');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'P') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'F');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'P') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'F');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'P') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'F');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
          }
        }
      }
      else if (citizen(id).weapon == Hammer) {
        if (builder_nearby) {
          move(id, builder_dir);
        }
        else if (bazooka_nearby or gunner_nearby) {
          bool yes = false;
          Dir yes_direction = Up;
          for (Dir d : dirs) {
            if (pos_ok(p + d) and cell(p + d).type != Building and cell(p + d).id == -1) {
              yes = true;
              yes_direction = d;
            }
          }
          if (yes) move(id, yes_direction);
        }
        else if (hammer_nearby and citizen(id).life == warrior_ini_life()) {
          move(id, hammer_dir);
        }
        else if (food_nearby and citizen(id).life == warrior_ini_life()) move(id, food_dir);
        else if (money_nearby) move(id,money_dir);
        else {
          map<int, pair<int, int>> distancias;
          int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
          int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
          int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
          int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
          if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'P' and tablero[p.i - 1][p.j] != 'G') {
            dfs(tablero, i0, j0, x0, y0, d0, 'b', 'g', 'p');
            distancias.insert(make_pair(d0, make_pair(x0, y0)));
          }
          if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'P' and tablero[p.i + 1][p.j] != 'G') {
            dfs(tablero, i1, j1, x1, y1, d1, 'b', 'g', 'p');
            distancias.insert(make_pair(d1, make_pair(x1, y1)));
          }
          if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'P' and tablero[p.i][p.j - 1] != 'G') {
            dfs(tablero, i2, j2, x2, y2, d2, 'b', 'g', 'p');
            distancias.insert(make_pair(d2, make_pair(x2, y2)));
          }
          if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'P' and tablero[p.i][p.j + 1] != 'G') {
            dfs(tablero, i3, j3, x3, y3, d3, 'b', 'g', 'p');
            distancias.insert(make_pair(d3, make_pair(x3, y3)));
          }
          auto it = distancias.begin();
          if (it->first == d3 and it->first != 0) {
            Dir dist = Right;
            move(id, dist);
          }
          else if (it->first == d2 and it->first != 0) {
            Dir dist = Left;
            move(id, dist);
          }
          else if (it->first == d1 and it->first != 0) {
            Dir dist = Down;
            move(id, dist);
          }
          else if (it->first == d0 and it->first != 0) {
            Dir dist = Up;
            move(id, dist);
          }
          else {
            map<int, pair<int, int>> distancias;
            int i0 = p.i - 1, j0 = p.j, x0 = 0, y0 = 0, d0 = 0;
            int i1 = p.i + 1, j1 = p.j, x1 = 0, y1 = 0, d1 = 0;
            int i2 = p.i, j2 = p.j - 1, x2 = 0, y2 = 0, d2 = 0;
            int i3 = p.i, j3 = p.j + 1, x3 = 0, y3 = 0, d3 = 0;
            if (p.i > 0 and tablero[p.i - 1][p.j] != 'B' and tablero[p.i - 1][p.j] != 'e' and tablero[p.i - 1][p.j] != 'x' and tablero[p.i - 1][p.j] != 'P' and tablero[p.i - 1][p.j] != 'G') {
              dfs(tablero, i0, j0, x0, y0, d0, 'M', 'M', 'F');
              distancias.insert(make_pair(d0, make_pair(x0, y0)));
            }
            if (p.i < n - 1 and tablero[p.i + 1][p.j] != 'B' and tablero[p.i + 1][p.j] != 'e' and tablero[p.i + 1][p.j] != 'x' and tablero[p.i + 1][p.j] != 'P' and tablero[p.i + 1][p.j] != 'G') {
              dfs(tablero, i1, j1, x1, y1, d1, 'M', 'M', 'F');
              distancias.insert(make_pair(d1, make_pair(x1, y1)));
            }
            if (p.j > 0 and tablero[p.i][p.j - 1] != 'B' and tablero[p.i][p.j - 1] != 'e' and tablero[p.i][p.j - 1] != 'x' and tablero[p.i][p.j - 1] != 'P' and tablero[p.i][p.j - 1] != 'G') {
              dfs(tablero, i2, j2, x2, y2, d2, 'M', 'M', 'F');
              distancias.insert(make_pair(d2, make_pair(x2, y2)));
            }
            if (p.j < m - 1 and tablero[p.i][p.j + 1] != 'B' and tablero[p.i][p.j + 1] != 'e' and tablero[p.i][p.j + 1] != 'x' and tablero[p.i][p.j + 1] != 'P' and tablero[p.i][p.j + 1] != 'G') {
              dfs(tablero, i3, j3, x3, y3, d3, 'M', 'M', 'F');
              distancias.insert(make_pair(d3, make_pair(x3, y3)));
            }
            auto it = distancias.begin();
            if (it->first == d3 and it->first != 0) {
              Dir dist = Right;
              move(id, dist);
            }
            else if (it->first == d2 and it->first != 0) {
              Dir dist = Left;
              move(id, dist);
            }
            else if (it->first == d1 and it->first != 0) {
              Dir dist = Down;
              move(id, dist);
            }
            else if (it->first == d0 and it->first != 0) {
              Dir dist = Up;
              move(id, dist);
            }
          }
        }
      }
    }
  }
}
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
