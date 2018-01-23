#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
//#include <string>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

const int NUM_BALLS = 5;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

//a global variable for number of readings in potentiometer smoothing array
const int NUMREADINGS = 10;

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();
void ready321();
void level_up();


class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

void printX(Color color);

const Color BLACK(0, 0, 0);
const Color RED(7, 0, 0);
const Color ORANGE(7, 4, 0);
const Color YELLOW(7, 7, 0);
const Color GREEN(0, 7, 0);
const Color BLUE(0, 0, 7);
const Color PURPLE(7, 0, 7);
const Color WHITE(7, 7, 7);
const Color LIME(4, 7, 0);
const Color AQUA(0, 7, 7);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 0;
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }

    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
      erase();
      y++;
      draw();
    }

    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength > 0) {
        Color bodyColor;
        //assigns the Invader a color depending on strength
        switch (strength) {
          case 1:
            bodyColor = RED;
            break;
          case 2:
            bodyColor = ORANGE;
            break;
          case 3:
            bodyColor = YELLOW;
            break;
          case 4:
            bodyColor = GREEN;
            break;
          case 5:
            bodyColor = BLUE;
            break;
          case 6:
            bodyColor = PURPLE;
            break;
          case 7:
            bodyColor = WHITE;
            break;
        }
        draw_with_rgb(bodyColor, BLUE);
      }
    }

    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }

    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength --;
      if (strength == 0) {
        erase();
      }
      else {
        draw();
      }
    }

  private:
    int x;
    int y;
    int strength;

    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      int numColor = body_color.to_333();
      int numEyeColor = eye_color.to_333();

      matrix.drawPixel((x + 1), y, numColor);
      matrix.drawPixel((x + 2), y, numColor);
      matrix.drawPixel(x, (y + 1), numColor);
      matrix.drawPixel((x + 1), (y + 1), numEyeColor);
      matrix.drawPixel((x + 2), (y + 1), numEyeColor);
      matrix.drawPixel((x + 3), (y + 1), numColor);
      matrix.drawPixel(x, (y + 2), numColor);
      matrix.drawPixel((x + 1), (y + 2), numColor);
      matrix.drawPixel((x + 2), (y + 2), numColor);
      matrix.drawPixel((x + 3), (y + 2), numColor);
      matrix.drawPixel(x, (y + 3), numColor);
      matrix.drawPixel((x + 3), (y + 3), numColor);

    }

};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 0;
      fired = false;
    }

    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
    }

    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }

    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      fired = true;
    }

    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      if (y > 0) {
        erase();
        y--;
        draw();
      //  delay(5);
      }
      else if (y <= 0) {
        erase();
      }
    }

    // resets private data members to initial values
    void hit() {
      erase();
      y = 0;
      x = 0;
      fired = false;
    }

    // draws the Cannonball, if it is fired
    void draw() {
      int orangenum = ORANGE.to_333();
      matrix.drawPixel(x, y, orangenum);
      matrix.drawPixel(x, (y + 1), orangenum);
    }

    // draws black where the Cannonball used to be
    void erase() {
      matrix.drawPixel(x, y, 0);
      matrix.drawPixel(x, (y + 1), 0);
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 1;
      y = 14;
      lives = 3;
    }

    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
    }

    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }

    // Modifies: lives
    void die() {
      lives--;
    }

    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }

    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      matrix.drawPixel(x, y, 0);
      matrix.drawPixel(x, (y + 1), 0);
      matrix.drawPixel((x - 1), (y + 1), 0);
      matrix.drawPixel((x + 1), (y + 1), 0);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }

    // draws the player
    void draw_with_rgb(Color color) {
      int inColor = color.to_333();

      matrix.drawPixel(x, y, inColor);
      matrix.drawPixel(x, (y + 1), inColor);
      matrix.drawPixel((x - 1), (y + 1), inColor);
      matrix.drawPixel((x + 1), (y + 1), inColor);
    }
};


class Game {
  public:
    Game() {
      level = 0;
      time = 0;
    }

    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    // see http://arduino.cc/en/Reference/Setup
    void setup() {
      //initializes potentiometer array to 0
      for (int thisReading = 0; thisReading < NUMREADINGS; thisReading++) {
        readings[thisReading] = 0;
      }

    }

    // displays and runs Space Invaders!
    // see spec for details of game
    // Modifies: global variable matrix
    void loop(int potentiometer_value, bool button_pressed) {
      if (level_cleared()) {
        if(millis() > 20){
          level_up();
        }
        reset_level();
      }
      readPotentiometer(potentiometer_value);
      player.erase();
      player.set_x(position_in);
      player.draw();
      if (button_pressed) {
        ball.erase();
        ball.fire(player.get_x(), 14);
        cannon_timer = millis();
      }
      if (ball.has_been_fired() && (millis() > cannon_timer + 10)) {
        cannon_timer = millis();
        ball.move();
        for (int j = 0; j < NUM_ENEMIES; j++) {
          if (enemies[j].get_strength() > 0) {
            int inv_x = enemies[j].get_x();
            int inv_y = enemies[j].get_y();
            int ball_x = ball.get_x();
            int ball_y = ball.get_y();
            if (((inv_x == ball_x) || ((inv_x + 3) == ball_x)) && ((inv_y + 3) == ball_y)) {
              ball.hit();
              enemies[j].hit();
            }
            else if (((((inv_x + 1) == ball_x) || (inv_x + 2) == ball_x)) && ((inv_y + 2) == ball_y)) {
              ball.hit();
              enemies[j].hit();
            }
          }
        }
      }
      if (millis() > invader_timer + 1250) {
        invader_timer = millis();
        if (line_two_exists()) {
          for (int m = 8; m < 16; m++) {
            enemies[m].move();
            if (enemies[m].get_y() == 11 && enemies[m].get_strength() != 0) {
              if (enemies[m].get_x() == player.get_x() || (enemies[m].get_x() + 3 == player.get_x())) {
                player.die();
                if (player.get_lives() > 0) {
                  printX(LIME);
                  reset_level();
                  break;
                }
                else {
                  game_over();
                }
              }
            } else if (enemies[m].get_y() == 12 && enemies[m].get_strength() != 0) {
              player.die();
              if (player.get_lives() > 0) {
                printX(LIME);
                reset_level();
                break;
              } else {
                game_over();
              }
            }
          }
        } else {
          for (int n = 0; n < 8; n++) {
            enemies[n].move();
            if (enemies[n].get_y() == 11 && enemies[n].get_strength() != 0) {
              if (enemies[n].get_x() == player.get_x() || enemies[n].get_x() + 3 == player.get_x()) {
                player.die();
                if (player.get_lives() > 0) {
                  printX(LIME);
                  reset_level();
                  break;
                }
                else {
                  game_over();
                }
              }
              } else if (enemies[n].get_y() == 12 && enemies[n].get_strength() != 0) {
                player.die();
                if (player.get_lives() > 0) {
                  printX(LIME);
                  reset_level();
                  break;
                } else {
                  game_over();
                }
              }
          }
        }
      }

      if (player.get_lives() <= 0) {
        player.erase();
        game_over();
      }
    }

  private:
    int level = 1;
    int position_in;
    int readings[NUMREADINGS];
    int readIndex = 0;
    unsigned long time;
    unsigned long cannon_timer = 0;
    unsigned long invader_timer;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];
    double total = 0.0;
    double average = 0.0;



    void readPotentiometer(int potentiometer_val) {
      // subtract the last reading:
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = potentiometer_val;
      // add the reading to the total:
      total = total + readings[readIndex];
      // advance to the next position in the array:
      readIndex = readIndex + 1;
      // if we're at the end of the array...
      if (readIndex >= NUMREADINGS) {
        // ...wrap around to the beginning:
        readIndex = 0;
      }
      // calculate the average:
      average = total / NUMREADINGS;
      //calculates x position on board
      position_in = floor(average / 32.0);
      if (position_in < 0) {
        position_in = 0;
      }
      else if (position_in > 31) {
        position_in = 31;
      }
    }

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() != 0) {
          return false;
        }
      }
      level ++;
      return true;
    }

    bool line_two_exists() {
      for (int i = 8; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() != 0) {
          return true;
        }
      }
      return false;
    }

    // set up a level
    void reset_level() {
      int x;
      int y;
      int i;
      int k;
      int strength;

      print_lives(player.get_lives());
      print_level(level);
      ready321();

      switch (level) {
        case 1:
          for (int i = 0; i < 8; i++) {
            y = 0;
            x = i * 4;
            enemies[i].initialize(x, y, 1);
            enemies[i].draw();
          }
          break;
        case 2:
          for (i = 0; i < 16; i++) {
            if (i < 8) {
              y = 0;
              x = i * 4;
              if (i % 2 == 0) {
                strength = 1;
              } else {
                strength = 2;
              }
            } else {
              y = 4;
              x = (i * 4) - 32;
              if (i % 2 == 0) {
                strength = 2;
              } else {
                strength = 1;
              }
            }
            enemies[i] = Invader(x, y, strength);
            enemies[i].draw();
          }
          break;

        case 3:
          strength = 1;
          for (i = 0; i < 16; i++) {
            if (i < 8) {
              y = 0;
              x = i * 4;
            } else {
              y = 4;
              x = (i * 4) - 32;
            }
            if (i == 5 || i == 10 || i == 15) {
              strength = 1;
            }
            enemies[i] = Invader(x, y, strength);
            enemies[i].draw();
            strength++;
          }
          break;

        case 4:
          for (i = 0; i < 16; i++) {
            if (i < 8) {
              y = 0;
              x = i * 4;
              if (i % 2 == 0) {
                strength = 5;
              } else {
                strength = 4;
              }

            } else {
              y = 4;
              x = (i * 4) - 32;
              if (i % 2 == 0) {
                strength = 2;
              } else {
                strength = 3;
              }
            }
            enemies[i] = Invader(x, y, strength);
            enemies[i].draw();
          }
          break;

        default:
          for (i = 0; i < NUM_ENEMIES; i++) {
            if (i < 8) {
              x = i * 4;
              y = 0;
            } else {
              x = (i * 4) - 32;
              y = 4;
            }
            strength = random(1, 8);
            enemies[i] = Invader(x, y, strength);
            enemies[i].draw();
          }

          break;
      }
      invader_timer = millis();
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see http://arduino.cc/en/Reference/Setup
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  pinMode(POTENTIOMETER_PIN_NUMBER, INPUT);
  matrix.begin();
  game.setup();
}

// see http://arduino.cc/en/Reference/Loop
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  game.loop(potentiometer_value, button_pressed);
  //  ready321();

}

void printX(Color color) {
  matrix.fillScreen(color.to_333());
  matrix.setTextColor(BLACK.to_333());
  matrix.setTextSize(2);
  matrix.setCursor(12, 1);
  matrix.print('X');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());

}

// displays Level
void print_level(int level) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(1, 1);
  matrix.print('L');
  delay(100);
  matrix.setCursor(7, 1);
  matrix.print('E');
  delay(100);
  matrix.setCursor(13, 1);
  matrix.print('V');
  delay(100);
  matrix.setCursor(19, 1);
  matrix.print('E');
  delay(100);
  matrix.setCursor(25, 1);
  matrix.print('L');
  delay(100);
  matrix.setCursor(28, 1);
  matrix.print(':');
  delay(400);
  matrix.setTextColor(GREEN.to_333());
  matrix.setCursor(1, 9);
  matrix.print('=');
  matrix.setCursor(26, 9);
  matrix.print('=');
  delay(300);
  matrix.setCursor(7, 9);
  matrix.print('-');
  matrix.setCursor(20, 9);
  matrix.print('-');
  delay(300);
  matrix.setTextColor(PURPLE.to_333());
  matrix.setCursor(14, 9);
  matrix.print(level);
  delay(1500);
  matrix.fillScreen(BLACK.to_333());
}

// displays number of lives
void print_lives(int lives) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(GREEN.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(2, 1);
  matrix.print('L');
  delay(100);
  matrix.setCursor(7, 1);
  matrix.print('I');
  delay(100);
  matrix.setCursor(12, 1);
  matrix.print('V');
  delay(100);
  matrix.setCursor(18, 1);
  matrix.print('E');
  delay(100);
  matrix.setCursor(24, 1);
  matrix.print('S');
  delay(100);
  matrix.setCursor(28, 1);
  matrix.print(':');
  delay(400);
  matrix.setTextColor(RED.to_333());
  matrix.setCursor(1, 9);
  matrix.print('>');
  matrix.setCursor(26, 9);
  matrix.print('<');
  delay(300);
  matrix.setCursor(7, 9);
  matrix.print('>');
  matrix.setCursor(20, 9);
  matrix.print('<');
  delay(300);
  delay(400);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(14, 9);
  matrix.print(lives);
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
}

// displays "game over"
void game_over() {
  printX(RED);
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(4, 1);
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.print('G');
  delay(100);
  matrix.setCursor(10, 1);
  matrix.print('A');
  delay(100);
  matrix.setCursor(16, 1);
  matrix.print('M');
  delay(100);
  matrix.setCursor(22, 1);
  matrix.print('E');
  delay(100);
  matrix.setCursor(4, 9);
  matrix.print('O');
  delay(100);
  matrix.setCursor(10, 9);
  matrix.print('V');
  delay(100);
  matrix.setCursor(16, 9);
  matrix.print('E');
  delay(100);
  matrix.setCursor(22, 9);
  matrix.print('R');
  delay(5000);
  exit(0);
}

void ready321() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(WHITE.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(1, 4);
  matrix.print('R');
  matrix.setCursor(7, 4);
  matrix.print('E');
  matrix.setCursor(13, 4);
  matrix.print('A');
  matrix.setCursor(19, 4);
  matrix.print('D');
  matrix.setCursor(25, 4);
  matrix.print('Y');
  delay(1000);
  matrix.setTextSize(2);
  matrix.fillScreen(BLACK.to_333());
  delay(300);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(13, 1);
  matrix.print('3');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(50);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(13, 1);
  matrix.print('2');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(50);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(13, 1);
  matrix.print('1');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(50);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(2, 1);
  matrix.print('G');
  matrix.setCursor(14, 1);
  matrix.print('0');
  matrix.setCursor(22, 1);
  matrix.print('!');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
}

void level_up(){
   matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(1, 1);
  matrix.print('L');
  delay(100);
  matrix.setCursor(7, 1);
  matrix.print('E');
  delay(100);
  matrix.setCursor(13, 1);
  matrix.print('V');
  delay(100);
  matrix.setCursor(19, 1);
  matrix.print('E');
  delay(100);
  matrix.setCursor(25, 1);
  matrix.print('L');
  delay(100);
  matrix.setCursor(11, 8);
  matrix.print('U');
  matrix.setCursor(17, 8);
  matrix.print('P');
  delay(1000);
}

