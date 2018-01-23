#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

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
const int NUM_WALLS = 12;
const int NUM_BUBBLES = 5;
const int NUM_EELS = 4;
const int NUM_PUFFS = 3;
//a global variable for number of readings in potentiometer smoothing array
const int NUMREADINGS = 15;

int scoreCount = 0;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void game_over();
void ready321();
void level_up();
void big_blue();
void instructions();

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

class Eel {
  public:
    Eel() {
      x = random(100, 210);
      y = random(4, 13);
      strength = 1;
    }
    Eel(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 1;
    }
    void Initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 1;
    }
    int get_x() {
      return x;
    }
    int get_y() {
      return y;
    }
    int get_strength() {
      return strength;
    }
    void set_x(int x_arg) {
      x = x_arg;
    }
    void move() {
      erase();
      x--;
      draw();
      if (x == -4) {
        x = random(100, 250);
        y = random(4, 13);
        strength = 1;
      }
    }
    void reset() {
      x = random(100, 250);
      y = random(5, 12);
      strength = 1;

    }

    bool isUP() {
      if (x % 4 == 3) {
        return true;
      }
      else {
        return false;
      }
    }
    bool isDOWN() {
      if (x % 4 == 1) {
        return true;
      }
      else {
        return false;
      }
    }
    void draw() {
      if (strength != 0) {
        if (isUP()) {
          draw_with_rgb_up(RED);
        }
        else if (isDOWN()) {
          draw_with_rgb_down(RED);
        }
        else {
          draw_with_rgb_mid(RED);
        }
      }
      else {
        draw_with_rgb_up(BLACK);
        draw_with_rgb_down(BLACK);
        draw_with_rgb_mid(BLACK);
      }
    }

    void erase() {
      draw_with_rgb_up(BLACK);
      draw_with_rgb_down(BLACK);
      draw_with_rgb_mid(BLACK);
    }

    void hit() {
      erase();
      reset();
    }
    bool made_contact(int playerY, int ojX, int ojY, int ojStrength) {

      if ((ojX <= 8) && (ojX >= 5) && (ojStrength != 0)) {

        if (ojX == 6) {
          if ((playerY - 1) == (ojY) && ojStrength != 0) {

            return true;
          }
        }
        else if (ojY == playerY && ojStrength != 0) {
          return true;

        }
        else if (isUP() &&  ((ojY - 1) == playerY) && ojStrength != 0) {
          return true;
        }
        else if (isDOWN() && ((ojY + 1) == playerY) && ojStrength != 0) {
          return true;
        }
        else {
          return false;
        }
      }
      return false;
    }

  private:
    int x;
    int y;
    int strength;

    void draw_with_rgb_up(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
      matrix.drawPixel((x + 1), (y - 1), numColor);
      matrix.drawPixel((x + 2), y, numColor);
    }
    void draw_with_rgb_down(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
      matrix.drawPixel((x + 1), (y + 1), numColor);
      matrix.drawPixel((x + 2), y, numColor);
    }
    void draw_with_rgb_mid(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
      matrix.drawPixel((x + 1), y, numColor);
      matrix.drawPixel((x + 2), y, numColor);
    }
};

class Puff {
  public:
    Puff() {
      x = 35;
      y = 5;
      strength = 2;
    }
    Puff(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 2;
    }

    void Initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 2;
    }

    void set_x(int x_arg) {
      x = x_arg;
    }

    void set_y(int y_arg) {
      y = y_arg;
    }
    int set_strength(int strength_arg()) {
      strength = strength_arg;
    }
    int get_x() {
      return x;
    }
    int get_y() {
      return y;
    }
    int get_strength() {
      return strength;
    }
    void move() {
      erase();
      x--;
      draw();
      if (x == -4) {
        x = random(33, 39);
        y = random(3, 12);
      }
    }

    void draw() {
      Color bodyColor;
      switch (strength) {
        case 1:
          bodyColor = RED;
        case 2:
          bodyColor = AQUA;
      }
      draw_with_rgb(bodyColor);
    }

    void erase() {
      draw_with_rgb(BLACK);
    }

    void hit() {
      erase();
      strength--;
      if (strength == 0) {
        erase();
      }
    }

    void reset() {
      x = random(69, 100);
      y = random(3, 13);
      strength = 2;
    }

    bool made_contact(int playerX, int playerY, int puffX, int puffY, int puffStrength) {

      if (((playerX + 3) == puffX) && (playerY == puffY) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 3) == (puffX + 1)) && ((playerY == (puffY - 1))) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 3) == (puffX + 1)) && (playerY == (puffY + 1)) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 1) == (puffX + 1)) && ((playerY - 1) == (puffY + 1)) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 1) == (puffX + 2) && ((playerY - 1) == (puffY + 1))) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 1) == (puffX + 3)) && ((playerY - 1) == puffY) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 1) == puffX) && ((playerY - 1) == puffY) && puffStrength != 0) {
        return true;
      }
      else if ((playerX == (puffX + 3)) && (playerY == puffY) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 3) == (puffX + 1)) && (playerY == (puffY + 1)) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 2) == (puffX + 1)) && (playerY == (puffY - 1)) && puffStrength != 0) {
        return true;
      }
      else if (((playerX + 1) == (puffX + 1)) && (playerY == (puffY - 1)) && puffStrength != 0) {
        return true;
      }
      else if ((playerX == (puffX + 1)) && (playerY == (puffY - 1)) && puffStrength != 0) {
        return true;
      }
      else {
        return false;
      }
    }

    bool one_bullet_contact(int ballX, int ballY) {

      if ((ballY == y) && (ballX == x)) {
        return true;
      }
      else if ((ballY == (y - 1)) && (ballX == (x + 1))) {
        return true;
      }
      else if ((ballY == (y + 1)) && (ballX == (x + 1))) {
        return true;
      }
      else {
        return false;
      }
    }

    bool two_bullet_contact(int ballX, int ballY) {

      if ( (ballY == (y - 2) ) && (ballX ==  (x + 1) ) ) {
        return true;
      }
      else if ( (ballY == (y - 1) ) && (ballX == x) ) {
        return true;
      }
      else if ( (ballY == y) && (ballX == x) ) {
        return true;
      }
      else if ( (ballY == (y + 1) ) && (ballX == (x + 1) ) ) {
        return true;
      }
      else {
        return false;
      }
    }

  private:
    int x;
    int y;
    int strength;

    void draw_with_rgb(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
      matrix.drawPixel((x + 1), (y - 1), numColor);
      matrix.drawPixel((x + 1), y, numColor);
      matrix.drawPixel((x + 1), (y + 1), numColor);
      matrix.drawPixel((x + 2), y, numColor);
      matrix.drawPixel((x + 2), (y + 1), numColor);
      matrix.drawPixel((x + 2), (y - 1), numColor);
      matrix.drawPixel((x + 3), y, numColor);

    }
};


class Cannonball {
  public:
    Cannonball() {
      x = 9;
      y = 0;
      fired = false;
      dub = false;
    }

    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
      dub = false;
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
      if (x <= 32) {
        erase();
        x++;
        draw();
      }
      else {
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

      if (dub) {
        int greenNum = GREEN.to_333();
        matrix.drawPixel(x, y, greenNum);
        matrix.drawPixel(x, y + 1, greenNum);
      }
      else {
        int orangenum = ORANGE.to_333();
        matrix.drawPixel(x, y, orangenum);
      }
    }

    // draws black where the Cannonball used to be
    void erase() {
      if (!dub) {
        matrix.drawPixel(x, y, 0);
      }
      else {
        matrix.drawPixel(x, y, 0);
        matrix.drawPixel(x, (y + 1), 0);
      }
    }
    void set_double() {
      dub = true;
    }

    void set_normal() {
      dub = false;
    }
    bool dub_on() {
      return dub;
    }

  private:
    int x;
    int y;
    bool fired;
    bool dub;
};

class BigBullet {
  public:
    BigBullet() {
      x = random(33, 50);
      y = random(3, 11);
      on = false;
    }
    int get_x() {
      return x;
    }
    int get_y() {
      return y;
    }
    void draw() {
      draw_with_rgb(ORANGE);
    }
    void erase() {
      draw_with_rgb(BLACK);
    }
    void move() {
      erase();
      x--;
      draw();

    }

    void set_x(int x_arg) {
      x = x_arg;
    }

    void set_y(int y_arg) {
      y = y_arg;
    }

    void set_on() {
      on = true;
    }

    void turn_off() {
      on = false;
    }

    bool is_on() {
      return on;
    }

    bool made_contact(int playerX, int playerY, int bulletX, int bulletY) {

      if (((playerX + 3) == bulletX) && (playerY == bulletY)) {
        return true;
      }
      else if (((playerX + 3) == bulletX) && (playerY == (bulletY + 1))) {
        return true;
      }
      else if (((playerX + 1) == bulletX) && ((playerY - 1) == (bulletY) + 1)) {
        return true;
      }
      else if (((playerX + 3) == bulletX) && (playerY == (bulletY + 1))) {
        return true;
      }
      else if (((playerX + 2) == bulletX) && (playerY == (bulletY + 1))) {
        return true;
      }
      else if (((playerX + 1) == bulletX) && ((playerY - 1) == (bulletY + 1))) {
        return true;
      }
      else if (((playerX + 1) == (bulletX + 1)) && ((playerY - 1) == (bulletY + 1))) {
        return true;
      }
      else if ((playerX == (bulletX + 1)) && (playerY == (bulletY + 1))) {
        return true;
      }
      else if (playerX == (bulletX + 1) && (playerY == bulletY)) {
        return true;
      }
      else if (((playerX + 2) == bulletX) && (playerY == bulletY)) {
        return true;
      }
      else if (((playerX + 1) == bulletX) && (playerY == bulletY)) {
        return true;
      }
      else if ((playerX == bulletX) && (playerY == bulletY)) {
        return true;
      }
      else {
        return false;
      }
    }

    void reset() {
      x = random(325, 800);
      y = random(4, 12);
    }

  private:
    int x;
    int y;
    bool on;

    void draw_with_rgb(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
      matrix.drawPixel((x + 1), y, numColor);
      matrix.drawPixel(x, (y + 1), numColor);
      matrix.drawPixel((x + 1), (y + 1), numColor);
    }
};

class Player {
  public:
    Player() {
      x = 5;
      y = 9;
      lives = 1;
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

    void set_y(int y_arg) {
      y = y_arg;
    }
    void set_lives(int lives_arg) {
      lives = lives_arg;
    }
    void set_color(Color color_arg) {
      color = color_arg;
    }
    void set_secColor(Color color_arg) {
      secColor = color_arg;
    }

    // Modifies: lives
    void die() {
      lives--;
    }

    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(YELLOW);
    }

    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

  private:
    int x;
    int y;
    int lives;
    Color color;
    Color secColor;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }

    void draw_with_rgb(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
      matrix.drawPixel((x + 1), y, numColor);
      matrix.drawPixel((x + 2), y, numColor);
      matrix.drawPixel((x + 3), y, numColor);
      matrix.drawPixel((x + 1), (y - 1), numColor);
    }
};

class Bubbles {
  public:
    Bubbles() {
      x = 5;
      y = -1;
    }
    int get_x() {
      return x;
    }
    int get_y() {
      return y;
    }
    int set_x(int x_arg) {
      x = x_arg;
    }
    int set_y(int y_arg)  {
      y = y_arg;
    }
    void draw() {
      draw_with_rgb(WHITE);
    }
    void erase() {
      draw_with_rgb(BLACK);
    }
    void move() {
      erase();
      x--;
      if (x == -2) {
        x = 5;
      }
      draw();
    }

  private:
    int x;
    int y;

    void draw_with_rgb(Color color) {
      int numColor = color.to_333();

      matrix.drawPixel(x, y, numColor);
    }
};

class StaticWall {
  public:
    StaticWall() {
      x = 0;
      y = 0;
    }
    void draw() {
      draw_with_rgb(BLUE);
    }

  private:
    int x;
    int y;

    void draw_with_rgb(Color color) {
      int numColor = color.to_333();

      for (int i = x; i < 32; i++) {
        matrix.drawPixel(i, 0, numColor);
        matrix.drawPixel(i, 15, numColor);
      }
    }

};

class DynaWall {
  private:
    int x;
    int y;
    int len;

    void draw_with_rgb(Color color) {
      int numColor = color.to_333();
      for (int i = 0; i < len; i++) {
        matrix.drawPixel(x + i, y, numColor);
      }
    }

  public:
    DynaWall() {
      x = 0;
      y = 0;
      len = 0;
    }

    DynaWall(int x_arg, int y_arg, int len_arg) {
      x = x_arg;
      y = y_arg;
      len = len_arg;
    }

    int getX() {
      return x;
    }
    int getY() {
      return y;
    }

    int getLen() {
      return len;
    }

    void draw() {
      draw_with_rgb(BLUE);
    }

    void erase() {
      draw_with_rgb(BLACK);
    }

    void move() {
      erase();
      x--;
      if (x == -len) {
        x = 32;
      }
      draw();

    }

    void initialize(int x_arg, int y_arg, int len_arg) {
      x = x_arg;
      y = y_arg;
      len = len_arg;
    }
};

class Game {
  public:
    Game() {
    }


    void setup() {
      //initializes potentiometer array to 0
      for (int thisReading = 0; thisReading < NUMREADINGS; thisReading++) {
        readings[thisReading] = 8;
      }
      // Prints intro to game
      big_blue();
      instructions();
      ready321();
    }

    void loop(int potentiometer_value, bool button_pressed) {
      if (player.get_lives() == 0) {
        game_over();
      }
      // draw/initialize walls
      cave.draw();
      if (no_walls()) {
        reset_walls();
        distance_timer = millis();
        for (int x = 0; x < NUM_WALLS; x++) {
          cavern[x].draw();
        }
      }

      // checks if player hits the walls after 3 seconds pass
      if (millis() > 3000) {
        for (int i = 0; i < NUM_WALLS; i++) {
          if (cavern[i].getY() == 1)  {
            for (int x = cavern[i].getX(); x < cavern[i].getLen(); x++) {
              if (((player.get_y() - 1) == 1) && (player.get_x() == x)) {
                game_over();
              }
            }
          }
          else if (cavern[i].getY() == 14) {
            for (int x = cavern[i].getX(); x < cavern[i].getLen(); x++) {
              if (player.get_y() == 14 && player.get_x() == x) {
                game_over();
              }
            }
          }
        }
      }

      readPotentiometer(potentiometer_value);
      player.erase();
      player.set_y(position_in);
      player.draw();
      bullet.draw();

      if (!bullet.made_contact(player.get_x(), player.get_y(), bullet.get_x(), bullet.get_y())) {
        if ((millis() % 100 <= 13)) {
          bullet.move();
          if (bullet.get_x() < -2) {
            bullet.reset();
          }
          if (bullet.made_contact(player.get_x(), player.get_y(), bullet.get_x(), bullet.get_y())) {
            bullet_timer = millis();
            bullet.set_on();
            bullet.erase();
            bullet.reset();
          }
        }
      }

      // If the bullet power up is hit, then update the cannon ball
      // Only keep the updated cannon ball for 15 seconds
      if (bullet.is_on() && (millis() < bullet_timer + 15000)) {
        ball.set_double();
      }
      // Return the cannonbal to normal after 15 seconds of use
      else {
        ball.set_normal();
        bullet.turn_off();
      }

      //move the wall, the bubbles, and OJ, the eel
      if ((millis() > distance_timer + gameSpeed)) {
        count++;
        distance_timer = millis();
        for (int x = 0; x < NUM_WALLS; x++) {
          cavern[x].move();
        }

        for (int i = 0; i < NUM_BUBBLES; i++) {
          bubbles[i].erase();
          bubbles[i].set_y((player.get_y()));
          bubbles[i].move();
          bubbles[i + 1].move();
          bubbles[i + 1].erase();
          bubbles[i + 1].set_y((player.get_y()));
        }
        for (int i = 0; i < NUM_EELS; i++) {
          oj[i].move();

        }
        if (scoreCount >= 2000) {
          for (int j = 0; j < NUM_PUFFS; j++) {
            puffy[j].move();
          }
        }
        if (count == 30) {
          gameSpeed -= 3;
          count = 0;
          scoreCount += 137;
        }
      }

      // Update the bubbles that the submarine leaves behind
      for (int i = 0; i < NUM_BUBBLES; i++) {
        bubbles[i].draw();
        if (bubbles[i].get_x() == -3) {
          bubbles[i].set_x(player.get_x());
          bubbles[i].draw();
        }
      }

      // check to see if the button was pressed, it so, then shoot
      if (button_pressed) {
        ball.erase();
        ball.fire(9, player.get_y());
        cannon_timer = millis();
      }

      if (ball.has_been_fired() && (millis() > cannon_timer + 12)) {
        cannon_timer = millis();
        ball.move();
      }


      for (int i = 0; i < NUM_EELS; i++) {
        oj[i].draw();
        if (oj[i].get_x() == oj[i - 1].get_x()) {
          oj[i].set_x((oj[i].get_x()) + 20);
        }
      }

      for (int i = 0; i < NUM_EELS; i++) {
        if (oj[i].made_contact(player.get_y(), oj[i].get_x(), oj[i].get_y(), oj[i].get_strength())) {
          game_over();
        }
      }


      for (int i = 0; i < NUM_EELS; i++) {
        if (oj[i].isUP() && !bullet.is_on()) {
          if ((ball.get_y() == oj[i].get_y()) && (ball.get_x() == oj[i].get_x())) {
            ball.hit();
            oj[i].hit();
            if (oj[i].get_x() == oj[i - 1].get_x()) {
              oj[i].set_x((oj[i].get_x()) + 20);
            }
          }
        }
        else if ((ball.get_y() == (oj[i].get_y() - 1)) && (ball.get_x() == (oj[i].get_x() + 1))) {
          ball.hit();
          oj[i].hit();
          if (oj[i].get_x() == oj[i - 1].get_x()) {
            oj[i].set_x((oj[i].get_x()) + 20);
          }
        }
        if (oj[i].isUP() && bullet.is_on()) {
          if (((ball.get_y() || (ball.get_y() + 1)) == oj[i].get_y()) && (ball.get_x() == oj[i].get_x())) {
            ball.hit();
            oj[i].hit();
            if (oj[i].get_x() == oj[i - 1].get_x()) {
              oj[i].set_x((oj[i].get_x()) + 20);
            }
          }
        }
        else if (((ball.get_y() + 1) == (oj[i].get_y() - 1)) && (ball.get_x() == oj[i].get_x())) {
          ball.hit();
          oj[i].hit();
          if (oj[i].get_x() == oj[i - 1].get_x()) {
            oj[i].set_x((oj[i].get_x()) + 20);
          }
        }
        if (oj[i].isDOWN() && bullet.is_on()) {
          if (((ball.get_y() + 1) == oj[i].get_y()) && (ball.get_x() == oj[i].get_x())) {
            ball.hit();
            oj[i].hit();
            if (oj[i].get_x() == oj[i - 1].get_x()) {
              oj[i].set_x((oj[i].get_x()) + 20);
            }
          }
        }
        else if ((ball.get_y() == oj[i].get_y()) && (ball.get_x() == oj[i].get_x())) {
          ball.hit();
          oj[i].hit();
          if (oj[i].get_x() == oj[i - 1].get_x()) {
            oj[i].set_x((oj[i].get_x()) + 20);
          }
        }
        else if ((ball.get_y() == (oj[i].get_y() - 1)) && (ball.get_x() == (oj[i].get_x() + 1))) {
          ball.hit();
          oj[i].hit();
          if (oj[i].get_x() == oj[i - 1].get_x()) {
            oj[i].set_x((oj[i].get_x()) + 20);
          }
        }
        if (oj[i].isDOWN() && !bullet.is_on()) {
          if ((ball.get_y() == oj[i].get_y()) && (ball.get_x() == oj[i].get_x())) {
            ball.hit();
            oj[i].hit();
            if (oj[i].get_x() == oj[i - 1].get_x()) {
              oj[i].set_x((oj[i].get_x()) + 20);
            }
          }
        }
        else if ((ball.get_y() == (oj[i].get_y() - 1)) && (ball.get_x() == (oj[i].get_x() + 1))) {
          ball.hit();
          oj[i].hit();
          if (oj[i].get_x() == oj[i - 1].get_x()) {
            oj[i].set_x((oj[i].get_x()) + 20);
          }
        }
        else if ((!oj[i].isDOWN() && !oj[i].isUP()) && !bullet.is_on()) {
          if ((ball.get_y() == oj[i].get_y()) && (ball.get_x() == oj[i].get_x())) {
            ball.hit();
            oj[i].hit();
            if (oj[i].get_x() == oj[i - 1].get_x()) {
              oj[i].set_x((oj[i].get_x()) + 20);
            }
          }
        }
        else if ((!oj[i].isDOWN() && !oj[i].isUP()) && bullet.is_on()) {
          if ((((ball.get_y() == oj[i].get_y()) || ((ball.get_y() + 1)) == oj[i].get_y())) && (ball.get_x() == oj[i].get_x())) {
            ball.hit();
            oj[i].hit();
            if (oj[i].get_x() == oj[i - 1].get_x()) {
              oj[i].set_x((oj[i].get_x()) + 20);
            }
          }
        }
      }
      for (int i = 0; i < NUM_PUFFS; i++) {
        puffy[i].draw();
        if (puffy[i].get_x() == puffy[i + 1].get_x()) {
          if (puffy[i].get_y() == puffy[i + 1].get_y()) {
            puffy[i + 1].set_x((puffy[i + 1].get_x()) + 10);
            puffy[i + 1].set_y((puffy[i + 1].get_y()) + 3);
          }
          if ( puffy[i + 1].get_y() >= 12) {
            puffy[i].reset();
          }
          if (((puffy[i + 1].get_y() || (puffy[i + 1].get_y() - 1) || (puffy[i + 1].get_y() + 1)) == oj[i].get_y())
              && (((puffy[i + 1].get_x() || (puffy[i + 1].get_x() + 1) || (puffy[i + 1].get_x() + 2) || (puffy[i + 1].get_x() + 3)) == oj[i].get_x()))) {
            puffy[i + 1].reset();
          }
        }
      }
      for (int i = 0; i < NUM_PUFFS; i++) {
        if (puffy[i].made_contact(player.get_x(), player.get_y(), puffy[i].get_x(), puffy[i].get_y(), puffy[i].get_strength())) {
          player.die();
          if (player.get_lives() == 0) {
            game_over();
          }
        }
      }

      for (int i = 0; i < NUM_PUFFS; i++) {
        if (ball.dub_on()) {
          if (puffy[i].two_bullet_contact(ball.get_x(), ball.get_y())) {
            Serial.println("makes double contact");
            ball.hit();
            puffy[i].hit();
            Serial.println(puffy[i].get_strength());
            if (puffy[i].get_strength() != 0) {
              puffy[i].draw();
            }
            else if (puffy[i].get_strength() == 0) {
              puffy[i].reset();
            }
          }
        }
        else if (!ball.dub_on()) {
          if (puffy[i].one_bullet_contact(ball.get_x(), ball.get_y())) {
            Serial.println("contact");
            ball.hit();
            puffy[i].hit();
            Serial.println(puffy[i].get_strength());
            if (puffy[i].get_strength() != 0) {
              puffy[i].draw();
            }
            else if (puffy[i].get_strength() == 0) {
              puffy[i].reset();
            }
          }
        }
      }
    }


  private:
    int position_in;
    int readings[NUMREADINGS];
    int readIndex = 0;
    int pixels = 14;
    int gameSpeed = 120;

    unsigned long count = 0;
    unsigned long bullet_timer = 0;
    unsigned long speed_timer = 0;
    unsigned long time;
    unsigned long cannon_timer = 0;
    unsigned long distance_timer;
    unsigned long goober_timer;
    unsigned long bubble_move = 0;
    unsigned long powerup_timer = 0;
    unsigned long shield_timer;
    Player player;
    Bubbles bubbles[NUM_BUBBLES];
    Cannonball ball;
    Eel oj[NUM_EELS];
    Puff puffy[NUM_PUFFS];
    BigBullet bullet;
    StaticWall cave;
    DynaWall cavern[NUM_WALLS];
    DynaWall wall[NUM_WALLS];
    double total = 0.0;
    double average = 0.0;
    double y_total = 0.0;
    double y_average = 0.0;




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
      position_in = floor(average / 15.0);
      if (position_in > 16) {
        position_in = 15;
      }
      else if (position_in < 1) {
        position_in = 2;
      }
    }


    bool no_walls() {
      for (int i = 0; i < NUM_WALLS; i++) {
        if (cavern[i].getLen() == 0) {
          return true;
        }
      }
      return false;
    }



    // set up a level
    void reset_walls() {

      for (int wall = 0; wall < NUM_WALLS; wall++) {
        int len = random(2, 10);

        int x = random(32, 64);
        if (wall % 2 == 1) {
          cavern[wall].initialize(x, 1, len);
        }
        else {
          cavern[wall].initialize(x + 4, 14, len);
        }
      }
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

// displays "game over"
void game_over() {
  printX(RED);
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(4, 1);
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
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(200);
  matrix.setCursor(7, 1);
  matrix.print('Y');
  matrix.setCursor(13, 1);
  matrix.print('O');
  matrix.setCursor(19, 1);
  matrix.print('U');
  delay(300);
  matrix.setCursor(4, 8);
  matrix.print('S');
  matrix.setCursor(10, 8);
  matrix.print('W');
  matrix.setCursor(16, 8);
  matrix.print('A');
  matrix.setCursor(22, 8);
  matrix.print('M');
  delay(500);
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(2, 0);
  matrix.print(scoreCount);
  matrix.setTextSize(.1);
  matrix.setCursor(2, 9);
  matrix.print("YARDS");
  delay(5000);
  exit(0);
}

void ready321() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(RED.to_333());
  matrix.setTextSize(0.5);
  matrix.setCursor(0, 1);
  matrix.print('L');
  matrix.setCursor(5, 1);
  matrix.print('E');
  matrix.setCursor(9, 1);
  matrix.print('T');
  matrix.setCursor(13, 1);
  matrix.print('S');
  matrix.setCursor(18, 1);
  matrix.print('G');
  matrix.setCursor(23, 1);
  matrix.print('E');
  matrix.setCursor(27, 1);
  matrix.print('T');
  matrix.setCursor(0, 8);
  matrix.setTextColor(AQUA.to_333());
  matrix.print('A');
  matrix.setCursor(5, 8);
  matrix.print('Q');
  matrix.setCursor(10, 8);
  matrix.print('U');
  matrix.setCursor(15, 8);
  matrix.print('A');
  matrix.setCursor(20, 8);
  matrix.print('T');
  matrix.setCursor(24, 8);
  matrix.print('I');
  matrix.setCursor(28, 8);
  matrix.print('C');

  delay(3000);
  matrix.setTextSize(2);
  matrix.fillScreen(BLACK.to_333());
  delay(300);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(11, 1);
  matrix.print('3');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(50);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(11, 1);
  matrix.print('2');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(50);
  matrix.setTextColor(WHITE.to_333());
  matrix.setCursor(11, 1);
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

void big_blue() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(BLUE.to_333());
  matrix.setTextSize(2);
  matrix.setCursor(2, 1);
  matrix.print('B');
  delay(200);
  matrix.setCursor(11, 1);
  matrix.print('I');
  delay(200);
  matrix.setCursor(20, 1);
  matrix.print('G');
  delay(650);
  matrix.setTextSize(1);
  matrix.fillScreen(BLACK.to_333());
  delay(100);
  matrix.setCursor(4, 5);
  matrix.print('B');
  delay(100);
  matrix.setCursor(10, 5);
  matrix.print('L');
  delay(100);
  matrix.setCursor(16, 5);
  matrix.print('U');
  delay(100);
  matrix.setCursor(22, 5);
  matrix.print('E');
  delay(1000);
  matrix.fillScreen(BLUE.to_333());
  delay(100);
  matrix.setTextColor(YELLOW.to_333());
  matrix.setCursor(7, 1);
  matrix.print('B');
  matrix.setCursor(13, 1);
  matrix.print('I');
  matrix.setCursor(19, 1);
  matrix.print('G');
  delay(300);
  matrix.setCursor(4, 8);
  matrix.print('B');
  matrix.setCursor(10, 8);
  matrix.print('L');
  matrix.setCursor(16, 8);
  matrix.print('U');
  matrix.setCursor(22, 8);
  matrix.print('E');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  delay(100);
}

void instructions() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(5, 1);
  matrix.print('S');
  matrix.setCursor(11, 1);
  matrix.print('W');
  matrix.setCursor(16, 1);
  matrix.print('I');
  matrix.setCursor(21, 1);
  matrix.print('M');
  delay(500);
  matrix.setCursor(5, 9);
  matrix.print('F');
  matrix.setCursor(10, 9);
  matrix.print('A');
  matrix.setCursor(16, 9);
  matrix.print('S');
  matrix.setCursor(22, 9);
  matrix.print('T');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());

  matrix.setTextColor(GREEN.to_333());
  matrix.setCursor(7, 1);
  matrix.print('G');
  matrix.setCursor(13, 1);
  matrix.print('E');
  matrix.setCursor(19, 1);
  matrix.print('T');
  delay(500);
  matrix.setCursor(0, 9);
  matrix.print('P');
  matrix.setCursor(5, 9);
  matrix.print('O');
  matrix.setCursor(10, 9);
  matrix.print('W');
  matrix.setCursor(16, 9);
  matrix.print('E');
  matrix.setCursor(22, 9);
  matrix.print('R');
  matrix.setCursor(27, 9);
  matrix.print('S');
  delay(1000);

  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(RED.to_333());
  matrix.setCursor(2, 1);
  matrix.print('A');
  //  delay(100);
  matrix.setCursor(8, 1);
  matrix.print('V');
  //  delay(100);
  matrix.setCursor(14, 1);
  matrix.print('O');
  //  delay(100);
  matrix.setCursor(19, 1);
  matrix.print('I');
  //  delay(100);
  matrix.setCursor(24, 1);
  matrix.print('D');
  delay(500);
  matrix.setCursor(5, 9);
  matrix.print('F');
  matrix.setCursor(10, 9);
  matrix.print('I');
  matrix.setCursor(15, 9);
  matrix.print('S');
  matrix.setCursor(21, 9);
  matrix.print('H');
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
}
