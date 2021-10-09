/* The Lassoist
   Game developed by Aditya Byju (20D070004)
   Game developed as part of project for CS101 course */

#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <composite.h>
#include <sprite.h>
#include <ctime>
#include <cstdlib>
#include <canvas.h>

using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

class MovingObject : public Sprite{
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused = true){
    vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
  }
  public:
  MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused = true) : Sprite(){
    initMO(argvx, argvy, argax, argay, argpaused);
  }
  MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite(){
    double angle_rad = angle_deg*PI/180.0;
    double argvx = speed*cos(angle_rad);
    double argvy = -speed*sin(angle_rad);
    initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx){ vx = argvx;}
  void set_vy(double argvy){ vy = argvy;}
  void set_ax(double argax){ ax = argax;}
  void set_ay(double argay){ ay = argay;}
  double getXPos();
  double getYPos();

  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);
  void pause(){ paused = true;}
  void unpause(){ paused = false;}
  bool isPaused(){ return paused;}

  void addPart(Sprite* p){
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);

};

#endif
//MovingObject.cpp

void MovingObject::nextStep(double t){
  if(paused){ return;}
  for(size_t i = 0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos(){
  return (parts.size() > 0) ? parts[0]->getX() : -1;
} // End MovingObject::getXPos()

double MovingObject::getYPos(){
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}// End MovingObject::getYPos()

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta){
  for(size_t i = 0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m){
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i = 0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}// End MovingObject::getAttachedTo()

//coin.h
#ifndef __COIN_H__
#define __COIN_H__

class Coin : public MovingObject{
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

  public:
  Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int k) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta){
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin(k);
  }
  void initCoin(int k);
  void resetCoin(int t);

}; // End class Coin

class Bomb : public MovingObject{
  double bomb_start_x;
  double bomb_start_y;
  double brelease_speed;
  double brelease_angle_deg;
  double bomb_ax;
  double bomb_ay;

  // Moving parts
  Circle bomb_circle;

  public:
  Bomb(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int k) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta){
    brelease_speed = speed;
    brelease_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb(k);
  }
  void initBomb(int k);
  void resetBomb(int t);

}; // End class Bomb

class Magnet : public MovingObject{
  double mag_start_x;
  double mag_start_y;
  double mrelease_speed;
  double mrelease_angle_deg;
  double mag_ax;
  double mag_ay;

  // Moving parts
  Circle mag_circle;

  public:
  Magnet(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int k) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta){
    mrelease_speed = speed;
    mrelease_angle_deg = angle_deg;
    mag_ax = argax;
    mag_ay = argay;
    initMagnet(k);
  }
  void initMagnet(int k);
  void resetMagnet(int t);
  void deleteMagnet();

}; // End class Magnet

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//defining common variables
//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

#define BOMB_SIZE 10
#define MAGNET_SIZE 8

#define GOX (WINDOW_X + PLAY_X_START)/2 //Game Over X-coordinate
#define GOY PLAY_Y_HEIGHT/2 //Game Over Y-coordinate

#define X (WINDOW_X/2)//For construction of Main Menu
#define Y 256.5
#define UX 412.5
#define UY 127.5

class Lasso : public MovingObject{
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;
  Line lasso_line;

  // Non-moving parts
  Line lasso_band;

  // State info
  bool lasso_looped;
  bool magnetActive;
  Coin *the_coin;
  Bomb *the_bomb;
  Magnet *the_magnet;
  int num_coins;
  int num_life;
  int num_mag;

  void initLasso();
  public:
  Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta){
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_coin_when_magActive(Coin *coin);
  void check_for_bomb(Bomb *bomb);

  int getNumCoins(){ return num_coins;}
  int getNumLife(){ return num_life;}

  void resetScore();
  void resetLife();

  //For Magnet
  void mActivate();
  void resetMag();
  void resetMagActive();
  void check_for_magnet(Magnet *magnet);
  bool magIsActive();

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin(int k){
  if(k == 0){ //Different starting positions
    coin_start_x = (PLAY_X_START + WINDOW_X)/2.0;
    coin_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 1){
    coin_start_x = (PLAY_X_START + WINDOW_X)/1.75;
    coin_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 2){
    coin_start_x = (PLAY_X_START + WINDOW_X)/1.5;
    coin_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 3){
    coin_start_x = (PLAY_X_START + WINDOW_X)*5/12.0;
    coin_start_y = PLAY_Y_HEIGHT - 5;
  }
  else{
    coin_start_x = (PLAY_X_START + WINDOW_X)*7/12.0;
    coin_start_y = PLAY_Y_HEIGHT - 5;
  }
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(255, 215, 0)); //gold
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin(int t){
  double coin_speed, coin_angle_deg;
  if(t == 0){ //Change coin speed and angle
    coin_speed = COIN_SPEED;
    coin_angle_deg = COIN_ANGLE_DEG;
  }
  else if(t == 1){
    coin_speed = COIN_SPEED - 20;
    coin_angle_deg = COIN_ANGLE_DEG - 40;
  }
  else if(t == 2){
    coin_speed = COIN_SPEED + 20;
    coin_angle_deg = COIN_ANGLE_DEG - 20;
  }
  else if(t == 3){
    coin_speed = COIN_SPEED - 30;
    coin_angle_deg = COIN_ANGLE_DEG + 10;
  }
  else{
    coin_speed = COIN_SPEED + 30;
    coin_angle_deg = COIN_ANGLE_DEG + 30;
  }
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

//bomb.h

void Bomb::initBomb(int k){
  if(k == 0){ //Different starting positions
    bomb_start_x = (PLAY_X_START + WINDOW_X)/2.0;
    bomb_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 1){
    bomb_start_x = (PLAY_X_START + WINDOW_X)/1.75;
    bomb_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 2){
    bomb_start_x = (PLAY_X_START + WINDOW_X)/1.5;
    bomb_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 3){
    bomb_start_x = (PLAY_X_START + WINDOW_X)*5/12.0;
    bomb_start_y = PLAY_Y_HEIGHT - 5;
  }
  else{
    bomb_start_x = (PLAY_X_START + WINDOW_X)*7/12.0;
    bomb_start_y = PLAY_Y_HEIGHT - 5;
  }
  bomb_circle.reset(bomb_start_x, bomb_start_y, BOMB_SIZE);
  bomb_circle.setColor(COLOR("black")); //black
  bomb_circle.setFill(true);
  addPart(&bomb_circle);
}

void Bomb::resetBomb(int t){
  double bomb_speed, bomb_angle_deg;
  if(t == 0){ //Change bomb speed and angle
    bomb_speed = COIN_SPEED;
    bomb_angle_deg = COIN_ANGLE_DEG;
  }
  else if(t == 1){
    bomb_speed = COIN_SPEED - 20;
    bomb_angle_deg = COIN_ANGLE_DEG - 40;
  }
  else if(t == 2){
    bomb_speed = COIN_SPEED + 20;
    bomb_angle_deg = COIN_ANGLE_DEG - 20;
  }
  else if(t == 3){
    bomb_speed = COIN_SPEED - 30;
    bomb_angle_deg = COIN_ANGLE_DEG + 10;
  }
  else{
    bomb_speed = COIN_SPEED + 30;
    bomb_angle_deg = COIN_ANGLE_DEG + 30;
  }
  bomb_ax = 0;
  bomb_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(bomb_start_x, bomb_start_y, bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
}

//magnet.h

void Magnet::initMagnet(int k){
  if(k == 0){ //Different starting positions
    mag_start_x = (PLAY_X_START + WINDOW_X)/2.0;
    mag_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 1){
    mag_start_x = (PLAY_X_START + WINDOW_X)/1.75;
    mag_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 2){
    mag_start_x = (PLAY_X_START + WINDOW_X)/1.5;
    mag_start_y = PLAY_Y_HEIGHT - 5;
  }
  else if(k == 3){
    mag_start_x = (PLAY_X_START + WINDOW_X)*5/12.0;
    mag_start_y = PLAY_Y_HEIGHT - 5;
  }
  else{
    mag_start_x = (PLAY_X_START + WINDOW_X)*7/12.0;
    mag_start_y = PLAY_Y_HEIGHT - 5;
  }
  mag_circle.reset(mag_start_x, mag_start_y, MAGNET_SIZE);
  mag_circle.setColor(COLOR("red")); //red
  mag_circle.setFill(true);
  addPart(&mag_circle);
}

void Magnet::resetMagnet(int t){
  double mag_speed, mag_angle_deg;
  if(t == 0){ //Change magnet speed and angle
    mag_speed = COIN_SPEED;
    mag_angle_deg = COIN_ANGLE_DEG;
  }
  else if(t == 1){
    mag_speed = COIN_SPEED - 20;
    mag_angle_deg = COIN_ANGLE_DEG - 40;
  }
  else if(t == 2){
    mag_speed = COIN_SPEED + 20;
    mag_angle_deg = COIN_ANGLE_DEG - 20;
  }
  else if(t == 3){
    mag_speed = COIN_SPEED - 30;
    mag_angle_deg = COIN_ANGLE_DEG + 10;
  }
  else{
    mag_speed = COIN_SPEED + 30;
    mag_angle_deg = COIN_ANGLE_DEG + 30;
  }
  mag_ax = 0;
  mag_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(mag_start_x, mag_start_y, mag_speed, mag_angle_deg, mag_ax, mag_ay, paused, rtheta);
}

void Magnet::deleteMagnet(){
  double mag_speed, mag_angle_deg;
  mag_speed = 0;
  mag_angle_deg = COIN_ANGLE_DEG;
  mag_ax = 0;
  mag_ay = 0;
  bool paused = true, rtheta = true;
  reset_all(mag_start_x, mag_start_y-2000, mag_speed, mag_angle_deg, mag_ax, mag_ay, paused, rtheta);
}

//lasso.cpp

void Lasso::draw_lasso_band(){
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x - xlen), (lasso_start_y + ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso(){
  lasso_start_x = (PLAY_X_START + LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT - LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(169, 169, 169)); //dark grey
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(211, 211, 211)); //light grey
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  magnetActive = false;
  the_coin = NULL;
  the_bomb = NULL;
  the_magnet = NULL;
  num_coins = 0;
  num_life = 3;
  num_mag = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(139, 69, 19)); //dark brown

  lasso_band.setColor(COLOR(165, 42, 42)); //light brown
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::resetScore(){ num_coins = 0;}// End Lasso::resetScore()

void Lasso::resetLife(){ num_life = 3;}// End Lasso::resetLife()

void Lasso::resetMag(){ num_mag = 0;}//End Lasso::resetMag()

void Lasso::resetMagActive(){ magnetActive = false;}//End Lasso::resetMagActive()

void Lasso::mActivate(){ magnetActive = true;}//End Lasso::mActivate()

bool Lasso::magIsActive(){ return magnetActive;}//End Lasso::magIsActive()

void Lasso::yank(){
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL){ //checking for coin
    num_coins++;
    int r = (time(0)/5)%5;
    the_coin->resetCoin(r);
    the_coin = NULL;
  }
  if(the_bomb != NULL){ //checking for bomb
    num_life--;
    int a = (time(0) + 2)%5;
    the_bomb->resetBomb(a);
    the_bomb = NULL;
  }
  if(the_magnet != NULL){ //checking for magnet
    num_mag++;
    the_magnet->deleteMagnet();
    the_magnet = NULL;
    mActivate();
  }

} // End Lasso::yank()

void Lasso::loopit(){
  if(lasso_looped){ return;} // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg){
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG){ release_angle_deg = MIN_RELEASE_ANGLE_DEG;}
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG){ release_angle_deg = MAX_RELEASE_ANGLE_DEG;}
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed){
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED){ release_speed = MIN_RELEASE_SPEED;}
  if(release_speed > MAX_RELEASE_SPEED){ release_speed = MAX_RELEASE_SPEED;}
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime){
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() + 50 > PLAY_Y_HEIGHT || getXPos() - 50 < PLAY_X_START){ yank();}
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr){
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff) + (ydiff*ydiff));
  if(distance <= LASSO_RADIUS){
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()

void Lasso::check_for_coin_when_magActive(Coin *coinPtr){
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff) + (ydiff*ydiff));
  if(distance <= (LASSO_RADIUS+100)){
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
}//End Lasso::check_for_coin_when_magActive()

void Lasso::check_for_bomb(Bomb *bombPtr){
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb_x = bombPtr->getXPos();
  double bomb_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb_x);
  double ydiff = (lasso_y - bomb_y);
  double distance = sqrt((xdiff*xdiff) + (ydiff*ydiff));
  if(distance <= LASSO_RADIUS){
    the_bomb = bombPtr;
    the_bomb->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()

void Lasso::check_for_magnet(Magnet *magPtr){
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double mag_x = magPtr->getXPos();
  double mag_y = magPtr->getYPos();
  double xdiff = (lasso_x - mag_x);
  double ydiff = (lasso_y - mag_y);
  double distance = sqrt((xdiff*xdiff) + (ydiff*ydiff));
  if(distance <= LASSO_RADIUS){
    the_magnet = magPtr;
    the_magnet->getAttachedTo(this);
  }
} // End Lasso::check_for_magnet()



main_program{

  initCanvas("The Lassoist :)", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  //For Main Menu
  int MainMenu = 0;

  //For Controls
  int Controls = 0;

  //For the magnet
  float magTime = 0;
  bool magActive = false;

  //For Background
  Rectangle bg1(WINDOW_X/2.0, WINDOW_Y/2.0, WINDOW_X, WINDOW_Y);
  bg1.setColor(COLOR(0, 0, 204)); //dark blue
  bg1.setFill(true);

   //Main Menu
{
    Rectangle mmbg1(WINDOW_X/2, WINDOW_Y/2, WINDOW_X, WINDOW_Y);//mmbg = Main Menu background
    mmbg1.setColor(COLOR(0, 0, 204)); mmbg1.setFill(true);
    Rectangle mmbg2(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-50, WINDOW_Y-50);
    mmbg2.setColor(COLOR(0, 128, 255)); mmbg2.setFill(true);
    Rectangle mmbg3(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-85, WINDOW_Y-85);
    mmbg3.setColor(COLOR(0, 0, 204)); mmbg3.setFill(true);

    //To display THE LASSOIST
    Line t211(UX-325, UY-25, UX-275, UY-25), t212(UX-300, UY-25, UX-300, UY+25);
    t211.setColor(COLOR(102, 178, 255)); t212.setColor(COLOR(102, 178, 255));
    Line h211(UX-270, UY-25, UX-270, UY+25), h212(UX-220, UY-25, UX-220, UY+25), h213(UX-270, UY, UX-220, UY);
    h211.setColor(COLOR(102, 178, 255)); h212.setColor(COLOR(102, 178, 255)); h213.setColor(COLOR(102, 178, 255));
    Line e211(UX-215, UY-25, UX-215, UY+25), e212(UX-215, UY-25, UX-165, UY-25), e213(UX-215, UY+25, UX-165, UY+25), e214(UX-215, UY, UX-190, UY);
    e211.setColor(COLOR(102, 178, 255)); e212.setColor(COLOR(102, 178, 255)); e213.setColor(COLOR(102, 178, 255)); e214.setColor(COLOR(102, 178, 255));
    Line l211(UX-135, UY-25, UX-135, UY+25), l212(UX-135, UY+25, UX-85, UY+25);
    l211.setColor(COLOR(102, 178, 255)); l212.setColor(COLOR(102, 178, 255));
    Line a211(UX-80, UY+25, UX-55, UY-25), a212(UX-55, UY-25, UX-30, UY+25), a213(UX-66, UY, UX-42.5, UY);
    a211.setColor(COLOR(102, 178, 255)); a212.setColor(COLOR(102, 178, 255)); a213.setColor(COLOR(102, 178, 255));
    Line s211(UX-25, UY-25, UX+25, UY-25), s212(UX-25, UY+25, UX+25, UY+25), s213(UX-25, UY, UX+25, UY), s214(UX-25, UY-25, UX-25, UY), s215(UX+25, UY, UX+25, UY+25);
    s211.setColor(COLOR(102, 178, 255)); s212.setColor(COLOR(102, 178, 255)); s213.setColor(COLOR(102, 178, 255)); s214.setColor(COLOR(102, 178, 255)); s215.setColor(COLOR(102, 178, 255));
    Line s221(UX+30, UY-25, UX+80, UY-25), s222(UX+30, UY+25, UX+80, UY+25), s223(UX+30, UY, UX+80, UY), s224(UX+30, UY-25, UX+30, UY), s225(UX+80, UY, UX+80, UY+25);
    s221.setColor(COLOR(102, 178, 255)); s222.setColor(COLOR(102, 178, 255)); s223.setColor(COLOR(102, 178, 255)); s224.setColor(COLOR(102, 178, 255)); s225.setColor(COLOR(102, 178, 255));
    Circle o211(UX+110, UY, 25);
    o211.setColor(COLOR(102, 178, 255));
    Line i211(UX+140, UY-25, UX+190, UY-25), i212(UX+140, UY+25, UX+190, UY+25), i213(UX+165, UY-25, UX+165, UY+25);
    i211.setColor(COLOR(102, 178, 255)); i212.setColor(COLOR(102, 178, 255)); i213.setColor(COLOR(102, 178, 255));
    Line s231(UX+195, UY-25, UX+245, UY-25), s232(UX+195, UY+25, UX+245, UY+25), s233(UX+195, UY, UX+245, UY), s234(UX+195, UY-25, UX+195, UY), s235(UX+245, UY, UX+245, UY+25);
    s231.setColor(COLOR(102, 178, 255)); s232.setColor(COLOR(102, 178, 255)); s233.setColor(COLOR(102, 178, 255)); s234.setColor(COLOR(102, 178, 255)); s235.setColor(COLOR(102, 178, 255));
    Line t221(UX+250, UY-25, UX+300, UY-25), t222(UX+275, UY-25, UX+275, UY+25);
    t221.setColor(COLOR(102, 178, 255)); t222.setColor(COLOR(102, 178, 255));

    //Menu Buttons
    Rectangle p1(X, Y, 120, 30), p2(X, Y, 120, 30);
    p1.setColor(COLOR("white")); p1.setFill(true);
    p2.setColor(COLOR(0, 128, 255));
    Text p(X, Y, "PLAY");
    p.setColor(COLOR(102, 178, 255));
    Rectangle htp1(X, Y+79, 120, 30), htp2(X, Y+79, 120, 30);
    htp1.setColor(COLOR("white")); htp1.setFill(true);
    htp2.setColor(COLOR(0, 128, 255));
    Text htp(X, Y+79, "HOW TO PLAY");
    htp.setColor(COLOR(102, 178, 255));
    Rectangle a1(X, Y+158, 120, 30), a2(X, Y+158, 120, 30);
    a1.setColor(COLOR("white")); a1.setFill(true);
    a2.setColor(COLOR(0, 128, 255));
    Text a(X, Y+158, "ABOUT");
    a.setColor(COLOR(102, 178, 255));
    Rectangle q1(X, Y+237, 120, 30), q2(X, Y+237, 120, 30);
    q1.setColor(COLOR("white")); q1.setFill(true);
    q2.setColor(COLOR(0, 128, 255));
    Text q(X, Y+237, "QUIT GAME");
    q.setColor(COLOR(102, 178, 255));

    //For the buttons in main menu

    while(true){
      XEvent e;
      bool pendingEv = checkEvent(e);
      if(pendingEv){
        if(mouseButtonPressEvent(e)){
          if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y-15<=e.xbutton.y && e.xbutton.y<=Y+15){
            break;
          }
          else if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y+64<=e.xbutton.y && e.xbutton.y<=Y+94){
              Rectangle howToPlay(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-130, WINDOW_Y-130);
              howToPlay.setColor(COLOR(0, 128, 255)); howToPlay.setFill(true);

              //To display : How To Play
              Line h11(UX-237.5, UY, UX-237.5, UY-40), h12(UX-197.5, UY-40, UX-197.5, UY), h13(UX-237.5, UY-20, UX-197.5, UY-20);
              h11.setColor(COLOR(0, 0, 204)); h12.setColor(COLOR(0, 0, 204)); h13.setColor(COLOR(0, 0, 204));
              Circle o11(UX-172.5, UY-20, 20);
              o11.setColor(COLOR(0, 0, 204));
              Line w11(UX-147.5, UY, UX-147.5, UY-40), w12(UX-107.5, UY-40, UX-107.5, UY), w13(UX-147.5, UY, UX-127.5, UY-20), w14(UX-127.5, UY-20, UX-107.5, UY);
              w11.setColor(COLOR(0, 0, 204)); w12.setColor(COLOR(0, 0, 204)); w13.setColor(COLOR(0, 0, 204)), w14.setColor(COLOR(0, 0, 204));
              Line t11(UX-77.5, UY-40, UX-37.5, UY-40), t12(UX-57.5, UY-40, UX-57.5, UY);
              t11.setColor(COLOR(0, 0, 204)); t12.setColor(COLOR(0, 0, 204));
              Circle o21(UX-12.5, UY-20, 20);
              o21.setColor(COLOR(0, 0, 204));
              Line p11(UX+37.5, UY-40, UX+37.5, UY), p12(UX+77.5, UY-40, UX+77.5, UY-20), p13(UX+37.5, UY-40, UX+77.5, UY-40), p14(UX+37.5, UY-20, UX+77.5, UY-20);
              p11.setColor(COLOR(0, 0, 204)); p12.setColor(COLOR(0, 0, 204)); p13.setColor(COLOR(0, 0, 204)), p14.setColor(COLOR(0, 0, 204));
              Line l11(UX+82.5, UY-40, UX+82.5, UY), l12(UX+82.5, UY, UX+122.5, UY);
              l11.setColor(COLOR(0, 0, 204)); l12.setColor(COLOR(0, 0, 204));
              Line a11(UX+127.5, UY, UX+147.5, UY-40), a12(UX+147.5, UY-40, UX+167.5, UY), a13(UX+139, UY-20, UX+157.5, UY-20);
              a11.setColor(COLOR(0, 0, 204)); a12.setColor(COLOR(0, 0, 204)); a13.setColor(COLOR(0, 0, 204));
              Line y11(UX+172.5, UY-40, UX+192.5, UY-20), y12(UX+192.5, UY-20, UX+212.5, UY-40), y13(UX+192.5, UY-20, UX+192.5, UY);
              y11.setColor(COLOR(0, 0, 204)); y12.setColor(COLOR(0, 0, 204)); y13.setColor(COLOR(0, 0, 204));

              //Objective:
              Rectangle ob1(X, UY+35, 120, 30), ob2(X, UY+35, 120, 30);
              ob1.setColor(COLOR("white")); ob1.setFill(true);
              ob2.setColor(COLOR(102, 178, 255));
              Text ob(X, UY+35, "Objective :");
              ob.setColor(COLOR(0, 0, 204));

              Rectangle obj1(X, UY+98.75, 550, 87.5), obj2(X, UY+98.75, 550, 87.5);//tl = throw lasso
              obj1.setColor(COLOR("white")); obj1.setFill(true);
              obj2.setColor(COLOR(102, 178, 255));
              Text objectivel(X, UY+70.5, "Try to collect maximum number of coins (gold), while avoiding");
              objectivel.setColor(COLOR(0, 0, 204));
              Text objective2(X, UY+99.5, "bombs (black). You win if you collect 10 coins without losing");
              objective2.setColor(COLOR(0, 0, 204));
              Text objective3(X, UY+128.5, "all lives. Also, make use of the magnet (red). Happy playing!");
              objective3.setColor(COLOR(0, 0, 204));

              //Controls:
              Rectangle ct1(X, UY+177.5, 120, 30), ct2(X, UY+177.5, 120, 30);
              ct1.setColor(COLOR("white")); ct1.setFill(true);
              ct2.setColor(COLOR(102, 178, 255));
              Text ct(X, UY+177.5, "Controls :");
              ct.setColor(COLOR(0, 0, 204));

              Rectangle tl1(X, UY+207.5, 550, 20), tl2(X, UY+207.5, 550, 20);//tl = throw lasso
              tl1.setColor(COLOR("white")); tl1.setFill(true);
              tl2.setColor(COLOR(102, 178, 255));
              Text tl(X, UY+207.5, "Throw the lasso                                                       T");
              tl.setColor(COLOR(0, 0, 204));
              Rectangle ll1(X, UY+232.5, 550, 20), ll2(X, UY+232.5, 550, 20);//ll = loop lasso
              ll1.setColor(COLOR("white")); ll1.setFill(true);
              ll2.setColor(COLOR(102, 178, 255));
              Text ll(X, UY+232.5, "Loop the lasso                                                         L");
              ll.setColor(COLOR(0, 0, 204));
              Rectangle yl1(X, UY+257.5, 550, 20), yl2(X, UY+257.5, 550, 20);//yl = yank lasso
              yl1.setColor(COLOR("white")); yl1.setFill(true);
              yl2.setColor(COLOR(102, 178, 255));
              Text yl(X, UY+257.5, "Yank the lasso                                                         Y");
              yl.setColor(COLOR(0, 0, 204));
              Rectangle qg1(X, UY+282.5, 550, 20), qg2(X, UY+282.5, 550, 20);//qg = quit game
              qg1.setColor(COLOR("white")); qg1.setFill(true);
              qg2.setColor(COLOR(102, 178, 255));
              Text qg(X, UY+282.5, "Quit game                                                                 Q");
              qg.setColor(COLOR(0, 0, 204));
              Rectangle ids1(X, UY+307.5, 550, 20), ids2(X, UY+307.5, 550, 20);//ids = increase decrease speed
              ids1.setColor(COLOR("white")); ids1.setFill(true);
              ids2.setColor(COLOR(102, 178, 255));
              Text ids(X, UY+307.5, "Increase or decrease throwing speed of lasso        Up & Down Arrow Keys");
              ids.setColor(COLOR(0, 0, 204));
              Rectangle ca1(X, UY+332.5, 550, 20), ca2(X, UY+332.5, 550, 20);//ca = change angle
              ca1.setColor(COLOR("white")); ca1.setFill(true);
              ca2.setColor(COLOR(102, 178, 255));
              Text ca(X, UY+332.5, "Change the launch angle of lasso                             Left & Right Arrow Keys");
              ca.setColor(COLOR(0, 0, 204));

              Rectangle htp1(UX-12.5, UY+377.5, 120, 30), htp2(UX-12.5, UY+377.5, 120, 30);
              htp1.setColor(COLOR("white")); htp1.setFill(true);
              htp2.setColor(COLOR(102, 178, 255));
              Text htp(UX-12.5, UY+377.5, "BACK");
              htp.setColor(COLOR(0, 0, 204));

              while(true){
                XEvent e;
                bool pendingEv = checkEvent(e);
                if(pendingEv){
                  if(mouseButtonPressEvent(e)){
                    if(UX-72.5<=e.xbutton.x && e.xbutton.x<=UX+47.5 && UY+362.5<=e.xbutton.y && e.xbutton.y<=UY+392.5){
                      break;
                    }
                  else continue;
                  }
                }
              }
          }
          else if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y+143<=e.xbutton.y && e.xbutton.y<=Y+173){
              Rectangle about(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-130, WINDOW_Y-130);
              about.setColor(COLOR(0, 128, 255)); about.setFill(true);

              //To display : About
              Line a311(UX-122.5, UY, UX-102.5, UY-40), a312(UX-102.5, UY-40, UX-82.5, UY), a313(UX-111, UY-20, UX-92.5, UY-20);
              a311.setColor(COLOR(0, 0, 204)); a312.setColor(COLOR(0, 0, 204)); a313.setColor(COLOR(0, 0, 204));
              Line b311(UX-77.5, UY-40, UX-37.5, UY-40), b312(UX-77.5, UY-40, UX-77.5, UY), b313(UX-77.5, UY, UX-37.5, UY), b314(UX-77.5, UY-20, UX-42.5, UY-20);
              Line b321(UX-37.5, UY-40, UX-37.5, UY-25), b322(UX-37.5, UY-15, UX-37.5, UY), b323(UX-37.5, UY-25, UX-42.5, UY-20), b324(UX-37.5, UY-15, UX-42.5, UY-20);
              b311.setColor(COLOR(0, 0, 204)); b312.setColor(COLOR(0, 0, 204)); b313.setColor(COLOR(0, 0, 204)); b314.setColor(COLOR(0, 0, 204));
              b321.setColor(COLOR(0, 0, 204)); b322.setColor(COLOR(0, 0, 204)); b323.setColor(COLOR(0, 0, 204)); b324.setColor(COLOR(0, 0, 204));
              Circle o311(UX-12.5, UY-20, 20);
              o311.setColor(COLOR(0, 0, 204));
              Line u311(UX+12.5, UY-40, UX+12.5, UY), u312(UX+52.5, UY-40, UX+52.5, UY), u313(UX+12.5, UY, UX+52.5, UY);
              u311.setColor(COLOR(0, 0, 204)); u312.setColor(COLOR(0, 0, 204)); u313.setColor(COLOR(0, 0, 204));
              Line t321(UX+57.5, UY-40, UX+97.5, UY-40), t322(UX+77.5, UY-40, UX+77.5, UY);
              t321.setColor(COLOR(0, 0, 204)); t322.setColor(COLOR(0, 0, 204));

              Rectangle al11(UX-12.5, UY+51, 400, 30), al12(UX-12.5, UY+51, 400, 30);
              al11.setColor(COLOR("white")); al11.setFill(true);
              al12.setColor(COLOR(102, 178, 255));
              Text al1(UX-12.5, UY+51, "Game developed by      -      ADITYA BYJU");
              al1.setColor(COLOR(0, 0, 204));
              Rectangle al21(UX-12.5, UY+116, 400, 30), al22(UX-12.5, UY+116, 400, 30);
              al21.setColor(COLOR("white")); al21.setFill(true);
              al22.setColor(COLOR(102, 178, 255));
              Text al2(UX-12.5, UY+116, "Roll No.                  -                    20D070004");
              al2.setColor(COLOR(0, 0, 204));
              Rectangle al31(UX-12.5, UY+181, 400, 30), al32(UX-12.5, UY+181, 400, 30);
              al31.setColor(COLOR("white")); al31.setFill(true);
              al32.setColor(COLOR(102, 178, 255));
              Text a31(UX-12.5, UY+181, "Game developed as project for CS101 Course");
              a31.setColor(COLOR(0, 0, 204));
              Rectangle al41(UX-12.5, UY+246, 400, 30), al42(UX-12.5, UY+246, 400, 30);
              al41.setColor(COLOR("white")); al41.setFill(true);
              al42.setColor(COLOR(102, 178, 255));
              Text al4(UX-12.5, UY+246, "General colour layout      -      shades of blue");
              al4.setColor(COLOR(0, 0, 204));
              Rectangle al51(UX-12.5, UY+311, 400, 30), al52(UX-12.5, UY+311, 400, 30);
              al51.setColor(COLOR("white")); al51.setFill(true);
              al52.setColor(COLOR(102, 178, 255));
              Text al5(UX-12.5, UY+311, "coin -> gold, bomb -> black, magnet -> red");
              al5.setColor(COLOR(0, 0, 204));

              Rectangle ab1(UX-12.5, UY+377.5, 120, 30), ab2(UX-12.5, UY+377.5, 120, 30);
              ab1.setColor(COLOR("white")); ab1.setFill(true);
              ab2.setColor(COLOR(102, 178, 255));
              Text ab(UX-12.5, UY+377.5, "BACK");
              ab.setColor(COLOR(0, 0, 204));

              while(true){
                XEvent e;
                bool pendingEv = checkEvent(e);
                if(pendingEv){
                  if(mouseButtonPressEvent(e)){
                    if(UX-72.5<=e.xbutton.x && e.xbutton.x<=UX+47.5 && UY+362.5<=e.xbutton.y && e.xbutton.y<=UY+392.5){
                    break;
                  }
                  else continue;
                  }
                }
              }
          }
          else if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y+222<=e.xbutton.y && e.xbutton.y<=Y+252){
              exit(0);
          }
          else continue;
        }
      }
    }
}

  //For background (cont.)
  Rectangle bg2((WINDOW_X + PLAY_X_START)/2.0, PLAY_Y_HEIGHT/2.0, WINDOW_X - PLAY_X_START, PLAY_Y_HEIGHT+16);
  bg2.setColor(COLOR(102, 178, 255)); //light blue
  bg2.setFill(true);

  //To display : THE LASSOIST
  Line t11(50, 15, 50, 55), t12(30, 15, 70, 15);
  t11.setColor(COLOR("white")); t12.setColor(COLOR("white"));
  Line h11(30, 85, 70, 85), h12(30, 65, 30, 105), h13(70, 65, 70, 105);
  h11.setColor(COLOR("white")); h12.setColor(COLOR("white")); h13.setColor(COLOR("white"));
  Line e11(30, 115, 70, 115), e12(30, 135, 50, 135), e13(30, 155, 70, 155), e14(30, 115, 30, 155);
  e11.setColor(COLOR("white")); e12.setColor(COLOR("white")); e13.setColor(COLOR("white")); e14.setColor(COLOR("white"));
  Line l11(30, 195, 30, 235), l12(30, 235, 70, 235);
  l11.setColor(COLOR("white")); l12.setColor(COLOR("white"));
  Line a11(50, 245, 30, 285), a12(50, 245, 70, 285), a13(41, 265, 60, 265);
  a11.setColor(COLOR("white")); a12.setColor(COLOR("white")); a13.setColor(COLOR("white"));
  Line s11(30, 295, 70, 295), s12(30, 315, 70, 315), s13(30, 335, 70, 335), s14(30, 295, 30, 315), s15(70, 315, 70, 335);
  s11.setColor(COLOR("white")); s12.setColor(COLOR("white")); s13.setColor(COLOR("white")); s14.setColor(COLOR("white")); s15.setColor(COLOR("white"));
  Line s21(30, 345, 70, 345), s22(30, 365, 70, 365), s23(30, 385, 70, 385), s24(30, 345, 30, 365), s25(70, 365, 70, 385);
  s21.setColor(COLOR("white")); s22.setColor(COLOR("white")); s23.setColor(COLOR("white")); s24.setColor(COLOR("white")); s25.setColor(COLOR("white"));
  Circle o11(50, 415, 20);
  o11.setColor(COLOR("white"));
  Line i11(30, 445, 70, 445), i12(50, 445, 50, 485), i13(30, 485, 70, 485);
  i11.setColor(COLOR("white")); i12.setColor(COLOR("white")); i13.setColor(COLOR("white"));
  Line s31(30, 495, 70, 495), s32(30, 515, 70, 515), s33(30, 535, 70, 535), s34(30, 495, 30, 515), s35(70, 515, 70, 535);
  s31.setColor(COLOR("white")); s32.setColor(COLOR("white")); s33.setColor(COLOR("white")); s34.setColor(COLOR("white")); s35.setColor(COLOR("white"));
  Line t21(50, 545, 50, 585), t22(30, 545, 70, 545);
  t21.setColor(COLOR("white")); t22.setColor(COLOR("white"));

  // Draw the lassoist at the start position
  Circle mouth1(PLAY_X_START + 40, PLAY_Y_HEIGHT - 102, 13), mouth2(PLAY_X_START + 40, PLAY_Y_HEIGHT - 113, 20);
  mouth2.setColor(COLOR(102, 178, 255)); mouth2.setFill(true);
  Circle head(PLAY_X_START + 40, PLAY_Y_HEIGHT - 100, 20);
  Circle eye1(PLAY_X_START + 33, PLAY_Y_HEIGHT - 105, 2), eye2(PLAY_X_START + 47, PLAY_Y_HEIGHT - 105, 2);
  eye1.setFill(true); eye2.setFill(true);
  Line body(PLAY_X_START + 40,PLAY_Y_HEIGHT - 80, PLAY_X_START + 40, PLAY_Y_HEIGHT - 25);
  Line leg1(PLAY_X_START + 40, PLAY_Y_HEIGHT - 25, PLAY_X_START + 20, PLAY_Y_HEIGHT + 5), leg2(PLAY_X_START + 40, PLAY_Y_HEIGHT - 25, PLAY_X_START + 60, PLAY_Y_HEIGHT + 5);
  Line hand11(PLAY_X_START + 40,PLAY_Y_HEIGHT - 80, PLAY_X_START + 20, PLAY_Y_HEIGHT - 60), hand21(PLAY_X_START + 40,PLAY_Y_HEIGHT - 80, PLAY_X_START + 55, PLAY_Y_HEIGHT - 55);
  Line hand12(PLAY_X_START + 15,PLAY_Y_HEIGHT - 40, PLAY_X_START + 20, PLAY_Y_HEIGHT - 60), hand22(PLAY_X_START + 75,PLAY_Y_HEIGHT - 75, PLAY_X_START + 55, PLAY_Y_HEIGHT - 55);

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(PLAY_X_START, PLAY_Y_HEIGHT+8, WINDOW_X, PLAY_Y_HEIGHT+8);
  b1.setColor(COLOR(0, 128, 255)); //medium blue
  Line b2(PLAY_X_START, 0, PLAY_X_START, PLAY_Y_HEIGHT+8);
  b2.setColor(COLOR(0, 128, 255)); //medium blue

  char coinScoreStr[256];
  sprintf(coinScoreStr, "Score: %d", lasso.getNumCoins());
  Rectangle score1(PLAY_X_START + 105, PLAY_Y_HEIGHT + 50, 100, 30), score2(PLAY_X_START + 105, PLAY_Y_HEIGHT + 50, 100, 30);
  score1.setColor(COLOR("white")); score1.setFill(true);
  score2.setColor(COLOR(0, 128, 255));
  Text coinScore(PLAY_X_START + 105, PLAY_Y_HEIGHT + 50, coinScoreStr);
  coinScore.setColor(COLOR(102, 178, 255));
  char lifeStr[256];
  sprintf(lifeStr, "Lives Left: %d", lasso.getNumLife());
  Rectangle life1(PLAY_X_START + 265, PLAY_Y_HEIGHT + 50, 100, 30), life2(PLAY_X_START + 265, PLAY_Y_HEIGHT + 50, 100, 30);
  life1.setColor(COLOR("white")); life1.setFill(true);
  life2.setColor(COLOR(0, 128, 255));
  Text life(PLAY_X_START + 265, PLAY_Y_HEIGHT + 50, lifeStr);
  life.setColor(COLOR(102, 178, 255));
  Rectangle controls1(PLAY_X_START + 425, PLAY_Y_HEIGHT + 50, 100, 30), controls2(PLAY_X_START + 425, PLAY_Y_HEIGHT + 50, 100, 30);
  controls1.setColor(COLOR("white")); controls1.setFill(true);
  controls2.setColor(COLOR(0, 128, 255));
  Text controls(PLAY_X_START + 425, PLAY_Y_HEIGHT + 50, "Controls");
  controls.setColor(COLOR(102, 178, 255));
  Rectangle mainMenu1(PLAY_X_START + 585, PLAY_Y_HEIGHT + 50, 100, 30), mainMenu2(PLAY_X_START + 585, PLAY_Y_HEIGHT + 50, 100, 30);
  mainMenu1.setColor(COLOR("white")); mainMenu1.setFill(true);
  mainMenu2.setColor(COLOR(0, 128, 255));
  Text mainMenu(PLAY_X_START + 585, PLAY_Y_HEIGHT + 50, "Main Menu");
  mainMenu.setColor(COLOR(102, 178, 255));

  //coin1
  bool paused1 = true, rtheta1 = true;
  double coin_speed1 = COIN_SPEED;
  double coin_angle_deg1 = COIN_ANGLE_DEG;
  double coin_ax1 = 0;
  double coin_ay1 = COIN_G;
  int m = time(0)%5;
  Coin coin1(coin_speed1, coin_angle_deg1, coin_ax1, coin_ay1, paused1, rtheta1, m);

  //coin2
  bool paused2 = true, rtheta2 = true;
  double coin_speed2 = COIN_SPEED + 30;
  double coin_angle_deg2 = COIN_ANGLE_DEG - 45;
  double coin_ax2 = 0;
  double coin_ay2 = COIN_G;
  int n = (time(0)+2)%5;
  Coin coin2(coin_speed2, coin_angle_deg2, coin_ax2, coin_ay2, paused2, rtheta2, n);

  //coin3
  bool paused3 = true, rtheta3 = true;
  double coin_speed3 = COIN_SPEED + 60;
  double coin_angle_deg3 = COIN_ANGLE_DEG - 20;
  double coin_ax3 = 0;
  double coin_ay3 = COIN_G;
  int q = (time(0)+3)%5;
  Coin coin3(coin_speed3, coin_angle_deg3, coin_ax3, coin_ay3, paused3, rtheta3, q);

  //coin4
  bool paused4 = true, rtheta4 = true;
  double coin_speed4 = COIN_SPEED + 45;
  double coin_angle_deg4 = COIN_ANGLE_DEG - 27;
  double coin_ax4 = 0;
  double coin_ay4 = COIN_G;
  int w = (time(0)+4)%5;
  Coin coin4(coin_speed4, coin_angle_deg4, coin_ax4, coin_ay4, paused4, rtheta4, w);

  //coin5
  bool paused5 = true, rtheta5 = true;
  double coin_speed5 = COIN_SPEED - 15;
  double coin_angle_deg5 = COIN_ANGLE_DEG + 15;
  double coin_ax5 = 0;
  double coin_ay5 = COIN_G;
  int y = (time(0)+1)%5;
  Coin coin5(coin_speed5, coin_angle_deg5, coin_ax5, coin_ay5, paused5, rtheta5, y);

  //bomb1
  bool bpaused1 = true, brtheta1 = true;
  double bomb_speed1 = COIN_SPEED + 25;
  double bomb_angle_deg1 = COIN_ANGLE_DEG - 14;
  double bomb_ax1 = 0;
  double bomb_ay1 = COIN_G;
  int o = time(0)%5;
  Bomb bomb1(bomb_speed1, bomb_angle_deg1, bomb_ax1, bomb_ay1, bpaused1, brtheta1, o);

  //bomb2
  bool bpaused2 = true, brtheta2 = true;
  double bomb_speed2 = COIN_SPEED + 37;
  double bomb_angle_deg2 = COIN_ANGLE_DEG + 17;
  double bomb_ax2 = 0;
  double bomb_ay2 = COIN_G;
  int z = (time(0)+3)%5;
  Bomb bomb2(bomb_speed2, bomb_angle_deg2, bomb_ax2, bomb_ay2, bpaused2, brtheta2, z);

  //magnet1
  bool mpaused1 = true, mrtheta1 = true;
  double mag_speed1 = COIN_SPEED + 15;
  double mag_angle_deg1 = COIN_ANGLE_DEG - 34;
  double mag_ax1 = 0;
  double mag_ay1 = COIN_G;
  int p = (time(0)+4)%5;
  Magnet magnet1(mag_speed1, mag_angle_deg1, mag_ax1,mag_ay1, mpaused1, mrtheta1, p);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end1 = 0, last_coin_jump_end2 = 0, last_coin_jump_end3 = 0, last_coin_jump_end4 = 0, last_coin_jump_end5 = 0,last_bomb_jump_end1 = 0, last_bomb_jump_end2 = 0, last_mag_jump_end1 = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;){
    if((runTime > 0) && (currTime > runTime)){ break;}

    XEvent e;
    bool pendingEv = checkEvent(e);

    if(pendingEv){
      char c = charFromEvent(e);

      switch(c){
      case 't':
	    lasso.unpause();
	    break;
      case 'y':
	    lasso.yank();
	    if(lasso.magIsActive()){
	      magActive = true;
	      lasso.resetMag();
	    }
	    break;
      case 'l':
	    lasso.loopit();
	    lasso.check_for_coin(&coin1);
	    lasso.check_for_coin(&coin2);
	    lasso.check_for_coin(&coin3);
	    lasso.check_for_coin(&coin4);
	    lasso.check_for_coin(&coin5);
	    lasso.check_for_bomb(&bomb1);
	    lasso.check_for_bomb(&bomb2);
	    lasso.check_for_magnet(&magnet1);
	    wait(STEP_TIME*5);
	    break;
      case 'K'://Left Arrow Key
	    if(lasso.isPaused()){ lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);}
	    break;
      case 'M'://Right Arrow Key
	    if(lasso.isPaused()){ lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);}
	    break;
      case 'P'://Down Arrow Key
	    if(lasso.isPaused()){ lasso.addSpeed(-RELEASE_SPEED_STEP);}
	    break;
      case 'H'://Up Arrow Key
	    if(lasso.isPaused()){ lasso.addSpeed(+RELEASE_SPEED_STEP);}
	    break;
      case 'q':
        exit(0);
      default:
        break;
      }

      //Controls
      if(PLAY_X_START+375<=e.xbutton.x && e.xbutton.x<=PLAY_X_START+475 && PLAY_Y_HEIGHT+35<=e.xbutton.y && e.xbutton.y<=PLAY_Y_HEIGHT+65){
        Controls++;
      }
      //Main Menu
      else if(PLAY_X_START+535<=e.xbutton.x && e.xbutton.x<=PLAY_X_START+635 && PLAY_Y_HEIGHT+35<=e.xbutton.y && e.xbutton.y<=PLAY_Y_HEIGHT+65){
        MainMenu++;
      }
      else ;

    }

    //For movement of game objects and conditions on them
    lasso.nextStep(stepTime);

    coin1.nextStep(stepTime);
    coin2.nextStep(stepTime);
    coin3.nextStep(stepTime);
    coin4.nextStep(stepTime);
    coin5.nextStep(stepTime);

    bomb1.nextStep(stepTime);
    bomb2.nextStep(stepTime);

    magnet1.nextStep(stepTime);

    if(coin1.isPaused()){
      if((currTime-last_coin_jump_end1) >= COIN_GAP){
	    coin1.unpause();
      }
    }

    if(coin1.getYPos() + 5 > PLAY_Y_HEIGHT || coin1.getXPos() - 5 < PLAY_X_START || coin1.getXPos() > WINDOW_X){
      int l = (time(0)+1)%5;
      coin1.resetCoin(l);
      last_coin_jump_end1 = currTime;
    }

    if(coin2.isPaused()){
      if((currTime-last_coin_jump_end2) >= COIN_GAP){
	    coin2.unpause();
      }
    }

    if(coin2.getYPos() + 5 > PLAY_Y_HEIGHT || coin2.getXPos() - 5 < PLAY_X_START || coin2.getXPos() > WINDOW_X){
      int l = time(0)%5;
      coin2.resetCoin(l);
      last_coin_jump_end2 = currTime;
    }

    if(coin3.isPaused()){
      if((currTime-last_coin_jump_end3) >= COIN_GAP){
	    coin3.unpause();
      }
    }

    if(coin3.getYPos() + 5 > PLAY_Y_HEIGHT || coin3.getXPos() - 5 < PLAY_X_START || coin3.getXPos() > WINDOW_X){
      int l = (time(0)+2)%5;
      coin3.resetCoin(l);
      last_coin_jump_end3 = currTime;
    }

    if(coin4.isPaused()){
      if((currTime-last_coin_jump_end4) >= COIN_GAP){
	    coin4.unpause();
      }
    }

    if(coin4.getYPos() + 5 > PLAY_Y_HEIGHT || coin4.getXPos() - 5 < PLAY_X_START || coin4.getXPos() > WINDOW_X){
      int l = (time(0)+3)%5;
      coin4.resetCoin(l);
      last_coin_jump_end4 = currTime;
    }

    if(coin5.isPaused()){
      if((currTime-last_coin_jump_end5) >= COIN_GAP){
	    coin5.unpause();
      }
    }

    if(coin5.getYPos() + 5 > PLAY_Y_HEIGHT || coin5.getXPos() - 5 < PLAY_X_START || coin5.getXPos() > WINDOW_X){
      int l = (time(0)+4)%5;
      coin5.resetCoin(l);
      last_coin_jump_end5 = currTime;
    }

    if(bomb1.isPaused()){
      if((currTime-last_bomb_jump_end1) >= COIN_GAP){
	    bomb1.unpause();
      }
    }

    if(bomb1.getYPos() + 5 > PLAY_Y_HEIGHT || bomb1.getXPos() - 5 < PLAY_X_START || bomb1.getXPos() > WINDOW_X){
      int l = (time(0) + 3)%5;
      bomb1.resetBomb(l);
      last_bomb_jump_end1 = currTime;
    }

    if(bomb2.isPaused()){
      if((currTime-last_bomb_jump_end2) >= COIN_GAP){
	    bomb2.unpause();
      }
    }

    if(bomb2.getYPos() + 5 > PLAY_Y_HEIGHT || bomb2.getXPos() - 5 < PLAY_X_START || bomb2.getXPos() > WINDOW_X){
      int l = (time(0) + 4)%5;
      bomb2.resetBomb(l);
      last_bomb_jump_end2 = currTime;
    }

    if(magnet1.isPaused()){
      if((currTime-last_mag_jump_end1) >= COIN_GAP){
	    magnet1.unpause();
      }
    }

    if(magnet1.getYPos() + 5 > PLAY_Y_HEIGHT || magnet1.getXPos() - 5 < PLAY_X_START || magnet1.getXPos() > WINDOW_X){
      int l = (time(0)+2)%5;
      magnet1.resetMagnet(l);
      last_mag_jump_end1 = currTime;
    }

    sprintf(coinScoreStr, "Score: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(lifeStr, "Lives Left: %d", lasso.getNumLife());
    life.setMessage(lifeStr);

    //When the game is over
    if(lasso.getNumLife() == 0){
      Rectangle r((WINDOW_X + PLAY_X_START)/2, PLAY_Y_HEIGHT/2, 500, 300);
      r.setColor(COLOR(0, 0, 204)); r.setFill(true);

      //Game Over Display
      Line g11(GOX-190, GOY-60, GOX-150, GOY-60), g12(GOX-190, GOY-20, GOX-150, GOY-20), g13(GOX-170, GOY-40, GOX-150, GOY-40), g14(GOX-190, GOY-60, GOX-190, GOY-20), g15(GOX-150, GOY-40, GOX-150, GOY-20);
      g11.setColor(COLOR("white")); g12.setColor(COLOR("white")); g13.setColor(COLOR("white")); g14.setColor(COLOR("white")); g15.setColor(COLOR("white"));
      Line a11(GOX-145, GOY-20, GOX-125, GOY-60), a12(GOX-125, GOY-60, GOX-105, GOY-20), a13(GOX-135, GOY-40, GOX-115, GOY-40);
      a11.setColor(COLOR("white")); a12.setColor(COLOR("white")); a13.setColor(COLOR("white"));
      Line m11(GOX-100, GOY-60, GOX-100, GOY-20), m12(GOX-60, GOY-60, GOX-60, GOY-20), m13(GOX-100, GOY-60, GOX-80, GOY-40), m14(GOX-80, GOY-40, GOX-60, GOY-60);
      m11.setColor(COLOR("white")); m12.setColor(COLOR("white")); m13.setColor(COLOR("white")); m14.setColor(COLOR("white"));
      Line e11(GOX-55, GOY-60, GOX-55, GOY-20), e12(GOX-55, GOY-60, GOX-15, GOY-60), e13(GOX-55, GOY-20, GOX-15, GOY-20), e14(GOX-55, GOY-40, GOX-35, GOY-40);
      e11.setColor(COLOR("white")); e12.setColor(COLOR("white")); e13.setColor(COLOR("white")); e14.setColor(COLOR("white"));
      Circle o11(GOX+35, GOY-40, 20);
      o11.setColor(COLOR("white"));
      Line v11(GOX+60, GOY-60, GOX+80, GOY-20), v12(GOX+80, GOY-20, GOX+100, GOY-60);
      v11.setColor(COLOR("white")); v12.setColor(COLOR("white"));
      Line e21(GOX+105, GOY-60, GOX+105, GOY-20), e22(GOX+105, GOY-60, GOX+145, GOY-60), e23(GOX+105, GOY-20, GOX+145, GOY-20), e24(GOX+105, GOY-40, GOX+125, GOY-40);
      e21.setColor(COLOR("white")); e22.setColor(COLOR("white")); e23.setColor(COLOR("white")); e24.setColor(COLOR("white"));
      Line r11(GOX+150, GOY-60, GOX+190, GOY-60), r12(GOX+150, GOY-40, GOX+190, GOY-40), r13(GOX+150, GOY-60, GOX+150, GOY-20), r14(GOX+190, GOY-60, GOX+190, GOY-40), r15(GOX+150, GOY-40, GOX+190, GOY-20);
      r11.setColor(COLOR("white")); r12.setColor(COLOR("white")); r13.setColor(COLOR("white")); r14.setColor(COLOR("white")); r15.setColor(COLOR("white"));

      Rectangle p1(((WINDOW_X + PLAY_X_START)/2) - 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30), p2(((WINDOW_X + PLAY_X_START)/2) - 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30);
      p1.setColor(COLOR("white")); p1.setFill(true);
      p2.setColor(COLOR(0, 128, 255));
      Text p(((WINDOW_X + PLAY_X_START)/2) - 100, (PLAY_Y_HEIGHT/2) + 100, "Play Again");
      p.setColor(COLOR(102, 178, 255));
      Rectangle q1(((WINDOW_X + PLAY_X_START)/2) + 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30), q2(((WINDOW_X + PLAY_X_START)/2) + 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30);
      q1.setColor(COLOR("white")); q1.setFill(true);
      q2.setColor(COLOR(0, 128, 255));
      Text q(((WINDOW_X + PLAY_X_START)/2) + 100, (PLAY_Y_HEIGHT/2) + 100, "Quit Game");
      q.setColor(COLOR(102, 178, 255));

      while(true){
        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv){
          if(mouseButtonPressEvent(e)){
            if(GOX-150<=e.xbutton.x && e.xbutton.x<=GOX-50 && GOY+85<=e.xbutton.y && e.xbutton.y<=GOY+115){
              lasso.resetScore();
              lasso.resetLife();
              break;
            }
            else if(GOX+50<=e.xbutton.x && e.xbutton.x<=GOX+150 && GOY+85<=e.xbutton.y && e.xbutton.y<=GOY+115) exit(0);
            else continue;
          }
        }
      }
    }

    //When you win
    if(lasso.getNumCoins() == 10){
      Rectangle r((WINDOW_X + PLAY_X_START)/2, PLAY_Y_HEIGHT/2, 500, 300);
      r.setColor(COLOR(0, 0, 204)); r.setFill(true);

      //You Win Display
      Line y11(GOX-150, GOY-60, GOX-125, GOY-40), y12(GOX-125, GOY-40, GOX-105, GOY-60), y13(GOX-125, GOY-40, GOX-125, GOY-20);
      y11.setColor(COLOR("white")); y12.setColor(COLOR("white")); y13.setColor(COLOR("white"));
      Circle o11(GOX-85, GOY-40, 20);
      o11.setColor(COLOR("white"));
      Line u11(GOX-60, GOY-60, GOX-60, GOY-20), u12(GOX-60, GOY-20, GOX-20, GOY-20), u13(GOX-20, GOY-20, GOX-20, GOY-60);
      u11.setColor(COLOR("white")); u12.setColor(COLOR("white")); u13.setColor(COLOR("white"));
      Line w11(GOX+20, GOY-60, GOX+20, GOY-20), w12(GOX+20, GOY-20, GOX+40, GOY-40), w13(GOX+40, GOY-40, GOX+60, GOY-20), w14(GOX+60, GOY-20, GOX+60, GOY-60);
      w11.setColor(COLOR("white")); w12.setColor(COLOR("white")); w13.setColor(COLOR("white")); w14.setColor(COLOR("white"));
      Line i11(GOX+65, GOY-60, GOX+105, GOY-60), i12(GOX+65, GOY-20, GOX+105, GOY-20), i13(GOX+85, GOY-20, GOX+85, GOY-60);
      i11.setColor(COLOR("white")); i12.setColor(COLOR("white")); i13.setColor(COLOR("white"));
      Line n11(GOX+110, GOY-60, GOX+110, GOY-20), n12(GOX+150, GOY-60, GOX+150, GOY-20), n13(GOX+110, GOY-60, GOX+150, GOY-20);
      n11.setColor(COLOR("white")); n12.setColor(COLOR("white")); n13.setColor(COLOR("white"));

      Rectangle p1(((WINDOW_X + PLAY_X_START)/2) - 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30), p2(((WINDOW_X + PLAY_X_START)/2) - 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30);
      p1.setColor(COLOR("white")); p1.setFill(true);
      p2.setColor(COLOR(0, 128, 255));
      Text p(((WINDOW_X + PLAY_X_START)/2) - 100, (PLAY_Y_HEIGHT/2) + 100, "Play Again");
      p.setColor(COLOR(102, 178, 255));
      Rectangle q1(((WINDOW_X + PLAY_X_START)/2) + 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30), q2(((WINDOW_X + PLAY_X_START)/2) + 100, (PLAY_Y_HEIGHT/2) + 100, 100, 30);
      q1.setColor(COLOR("white")); q1.setFill(true);
      q2.setColor(COLOR(0, 128, 255));
      Text q(((WINDOW_X + PLAY_X_START)/2) + 100, (PLAY_Y_HEIGHT/2) + 100, "Quit Game");
      q.setColor(COLOR(102, 178, 255));

      while(true){
        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv){
          if(mouseButtonPressEvent(e)){
            if(GOX-150<=e.xbutton.x && e.xbutton.x<=GOX-50 && GOY+85<=e.xbutton.y && e.xbutton.y<=GOY+115){
              lasso.resetScore();
              lasso.resetLife();
              break;
            }
            else if(GOX+50<=e.xbutton.x && e.xbutton.x<=GOX+150 && GOY+85<=e.xbutton.y && e.xbutton.y<=GOY+115) exit(0);
            else continue;
          }
        }
      }
    }

    if(Controls >= 2){
      Rectangle ctrls(((WINDOW_X + PLAY_X_START)/2.0)+3, (PLAY_Y_HEIGHT/2.0)-3, WINDOW_X - PLAY_X_START, PLAY_Y_HEIGHT+18);//ctrls = controls
      ctrls.setColor(COLOR(102, 178, 255)); //light blue
      ctrls.setFill(true);

      //To display Controls
      Line c11(GOX-180, GOY-215, GOX-180, GOY-175), c12(GOX-180, GOY-215, GOX-140, GOY-215), c13(GOX-180, GOY-175, GOX-140, GOY-175);
      c11.setColor(COLOR(0, 0, 204)); c12.setColor(COLOR(0, 0, 204)); c13.setColor(COLOR(0, 0, 204));
      Circle o11(GOX-115, GOY-195, 20);
      o11.setColor(COLOR(0, 0, 204));
      Line n11(GOX-90, GOY-215, GOX-90, GOY-175), n12(GOX-50, GOY-215, GOX-50, GOY-175), n13(GOX-90, GOY-215, GOX-50, GOY-175);
      n11.setColor(COLOR(0, 0, 204)); n12.setColor(COLOR(0, 0, 204)); n13.setColor(COLOR(0, 0, 204));
      Line t11(GOX-45, GOY-215, GOX-5, GOY-215), t12(GOX-25, GOY-215, GOX-25, GOY-175);
      t11.setColor(COLOR(0, 0, 204)); t12.setColor(COLOR(0, 0, 204));
      Line r11(GOX+5, GOY-215, GOX+5, GOY-175), r12(GOX+45, GOY-215, GOX+45, GOY-195), r13(GOX+5, GOY-215, GOX+45, GOY-215), r14(GOX+5, GOY-195, GOX+45, GOY-195), r15(GOX+5, GOY-195, GOX+45, GOY-175);
      r11.setColor(COLOR(0, 0, 204)); r12.setColor(COLOR(0, 0, 204)); r13.setColor(COLOR(0, 0, 204)); r14.setColor(COLOR(0, 0, 204)); r15.setColor(COLOR(0, 0, 204));
      Circle o21(GOX+70, GOY-195, 20);
      o21.setColor(COLOR(0, 0, 204));
      Line l11(GOX+95, GOY-215, GOX+95, GOY-175), l12(GOX+95, GOY-175, GOX+135, GOY-175);
      l11.setColor(COLOR(0, 0, 204)); l12.setColor(COLOR(0, 0, 204));
      Line s11(GOX+140, GOY-215, GOX+180, GOY-215), s12(GOX+140, GOY-195, GOX+180, GOY-195), s13(GOX+140, GOY-175, GOX+180, GOY-175), s14(GOX+140, GOY-215, GOX+140, GOY-195), s15(GOX+180, GOY-195, GOX+180, GOY-175);
      s11.setColor(COLOR(0, 0, 204)); s12.setColor(COLOR(0, 0, 204)); s13.setColor(COLOR(0, 0, 204)); s14.setColor(COLOR(0, 0, 204)); s15.setColor(COLOR(0, 0, 204));

      //The Controls
      Rectangle tl1(GOX, GOY-120, 550, 30), tl2(GOX, GOY-120, 550, 30);//tl = throw lasso
      tl1.setColor(COLOR("white")); tl1.setFill(true);
      tl2.setColor(COLOR(0, 128, 255));
      Text tl(GOX, GOY-120, "Throw the lasso                                                       T");
      tl.setColor(COLOR(0, 0, 204));

      Rectangle ll1(GOX, GOY-66, 550, 30), ll2(GOX, GOY-66, 550, 30);//ll = loop lasso
      ll1.setColor(COLOR("white")); ll1.setFill(true);
      ll2.setColor(COLOR(0, 128, 255));
      Text ll(GOX, GOY-66, "Loop the lasso                                                         L");
      ll.setColor(COLOR(0, 0, 204));

      Rectangle yl1(GOX, GOY-12, 550, 30), yl2(GOX, GOY-12, 550, 30);//yl = yank lasso
      yl1.setColor(COLOR("white")); yl1.setFill(true);
      yl2.setColor(COLOR(0, 128, 255));
      Text yl(GOX, GOY-12, "Yank the lasso                                                         Y");
      yl.setColor(COLOR(0, 0, 204));

      Rectangle qg1(GOX, GOY+42, 550, 30), qg2(GOX, GOY+42, 550, 30);//qg = quit game
      qg1.setColor(COLOR("white")); qg1.setFill(true);
      qg2.setColor(COLOR(0, 128, 255));
      Text qg(GOX, GOY+42, "Quit game                                                                 Q");
      qg.setColor(COLOR(0, 0, 204));

      Rectangle ids1(GOX, GOY+96, 550, 30), ids2(GOX, GOY+96, 550, 30);//ids = increase decrease speed
      ids1.setColor(COLOR("white")); ids1.setFill(true);
      ids2.setColor(COLOR(0, 128, 255));
      Text ids(GOX, GOY+96, "Increase or decrease throwing speed of lasso         Up & Down Arrow Keys");
      ids.setColor(COLOR(0, 0, 204));

      Rectangle ca1(GOX, GOY+150, 550, 30), ca2(GOX, GOY+150, 550, 30);//ca = change angle
      ca1.setColor(COLOR("white")); ca1.setFill(true);
      ca2.setColor(COLOR(0, 128, 255));
      Text ca(GOX, GOY+150, "Change the launch angle of lasso                             Left & Right Arrow Keys");
      ca.setColor(COLOR(0, 0, 204));

      //btg = Back to Game
      Rectangle btg1(GOX, GOY+215, 150, 30), btg2(GOX, GOY+215, 150, 30);
      btg1.setColor(COLOR("white")); btg1.setFill(true);
      btg2.setColor(COLOR(0, 128, 255));
      Text btg(GOX, GOY+215, "Back to Game");
      btg.setColor(COLOR(0, 0, 204));

      while(true){
        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv){
          if(mouseButtonPressEvent(e)){
            if(GOX-75<=e.xbutton.x && e.xbutton.x<=GOX+75 && GOY+200<=e.xbutton.y && e.xbutton.y<=GOY+230){
              break;
            }
            else continue;
          }
        }
      }
      Controls = 0;
    }

  if(MainMenu >= 2){
    {
    Rectangle mmbg1(WINDOW_X/2, WINDOW_Y/2, WINDOW_X, WINDOW_Y);//mmbg = Main Menu background
    mmbg1.setColor(COLOR(0, 0, 204)); mmbg1.setFill(true);
    Rectangle mmbg2(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-50, WINDOW_Y-50);
    mmbg2.setColor(COLOR(0, 128, 255)); mmbg2.setFill(true);
    Rectangle mmbg3(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-85, WINDOW_Y-85);
    mmbg3.setColor(COLOR(0, 0, 204)); mmbg3.setFill(true);

    //To display THE LASSOIST
    Line t211(UX-325, UY-25, UX-275, UY-25), t212(UX-300, UY-25, UX-300, UY+25);
    t211.setColor(COLOR(102, 178, 255)); t212.setColor(COLOR(102, 178, 255));
    Line h211(UX-270, UY-25, UX-270, UY+25), h212(UX-220, UY-25, UX-220, UY+25), h213(UX-270, UY, UX-220, UY);
    h211.setColor(COLOR(102, 178, 255)); h212.setColor(COLOR(102, 178, 255)); h213.setColor(COLOR(102, 178, 255));
    Line e211(UX-215, UY-25, UX-215, UY+25), e212(UX-215, UY-25, UX-165, UY-25), e213(UX-215, UY+25, UX-165, UY+25), e214(UX-215, UY, UX-190, UY);
    e211.setColor(COLOR(102, 178, 255)); e212.setColor(COLOR(102, 178, 255)); e213.setColor(COLOR(102, 178, 255)); e214.setColor(COLOR(102, 178, 255));
    Line l211(UX-135, UY-25, UX-135, UY+25), l212(UX-135, UY+25, UX-85, UY+25);
    l211.setColor(COLOR(102, 178, 255)); l212.setColor(COLOR(102, 178, 255));
    Line a211(UX-80, UY+25, UX-55, UY-25), a212(UX-55, UY-25, UX-30, UY+25), a213(UX-66, UY, UX-42.5, UY);
    a211.setColor(COLOR(102, 178, 255)); a212.setColor(COLOR(102, 178, 255)); a213.setColor(COLOR(102, 178, 255));
    Line s211(UX-25, UY-25, UX+25, UY-25), s212(UX-25, UY+25, UX+25, UY+25), s213(UX-25, UY, UX+25, UY), s214(UX-25, UY-25, UX-25, UY), s215(UX+25, UY, UX+25, UY+25);
    s211.setColor(COLOR(102, 178, 255)); s212.setColor(COLOR(102, 178, 255)); s213.setColor(COLOR(102, 178, 255)); s214.setColor(COLOR(102, 178, 255)); s215.setColor(COLOR(102, 178, 255));
    Line s221(UX+30, UY-25, UX+80, UY-25), s222(UX+30, UY+25, UX+80, UY+25), s223(UX+30, UY, UX+80, UY), s224(UX+30, UY-25, UX+30, UY), s225(UX+80, UY, UX+80, UY+25);
    s221.setColor(COLOR(102, 178, 255)); s222.setColor(COLOR(102, 178, 255)); s223.setColor(COLOR(102, 178, 255)); s224.setColor(COLOR(102, 178, 255)); s225.setColor(COLOR(102, 178, 255));
    Circle o211(UX+110, UY, 25);
    o211.setColor(COLOR(102, 178, 255));
    Line i211(UX+140, UY-25, UX+190, UY-25), i212(UX+140, UY+25, UX+190, UY+25), i213(UX+165, UY-25, UX+165, UY+25);
    i211.setColor(COLOR(102, 178, 255)); i212.setColor(COLOR(102, 178, 255)); i213.setColor(COLOR(102, 178, 255));
    Line s231(UX+195, UY-25, UX+245, UY-25), s232(UX+195, UY+25, UX+245, UY+25), s233(UX+195, UY, UX+245, UY), s234(UX+195, UY-25, UX+195, UY), s235(UX+245, UY, UX+245, UY+25);
    s231.setColor(COLOR(102, 178, 255)); s232.setColor(COLOR(102, 178, 255)); s233.setColor(COLOR(102, 178, 255)); s234.setColor(COLOR(102, 178, 255)); s235.setColor(COLOR(102, 178, 255));
    Line t221(UX+250, UY-25, UX+300, UY-25), t222(UX+275, UY-25, UX+275, UY+25);
    t221.setColor(COLOR(102, 178, 255)); t222.setColor(COLOR(102, 178, 255));

    //Menu Buttons
    Rectangle p1(X, Y, 120, 30), p2(X, Y, 120, 30);
    p1.setColor(COLOR("white")); p1.setFill(true);
    p2.setColor(COLOR(0, 128, 255));
    Text p(X, Y, "PLAY");
    p.setColor(COLOR(102, 178, 255));
    Rectangle htp1(X, Y+79, 120, 30), htp2(X, Y+79, 120, 30);
    htp1.setColor(COLOR("white")); htp1.setFill(true);
    htp2.setColor(COLOR(0, 128, 255));
    Text htp(X, Y+79, "HOW TO PLAY");
    htp.setColor(COLOR(102, 178, 255));
    Rectangle a1(X, Y+158, 120, 30), a2(X, Y+158, 120, 30);
    a1.setColor(COLOR("white")); a1.setFill(true);
    a2.setColor(COLOR(0, 128, 255));
    Text a(X, Y+158, "ABOUT");
    a.setColor(COLOR(102, 178, 255));
    Rectangle q1(X, Y+237, 120, 30), q2(X, Y+237, 120, 30);
    q1.setColor(COLOR("white")); q1.setFill(true);
    q2.setColor(COLOR(0, 128, 255));
    Text q(X, Y+237, "QUIT GAME");
    q.setColor(COLOR(102, 178, 255));

    while(true){
        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv){
          if(mouseButtonPressEvent(e)){
            if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y-15<=e.xbutton.y && e.xbutton.y<=Y+15){
              break;
            }
            else if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y+64<=e.xbutton.y && e.xbutton.y<=Y+94){
              Rectangle howToPlay(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-130, WINDOW_Y-130);
              howToPlay.setColor(COLOR(0, 128, 255)); howToPlay.setFill(true);

              //To display : How To Play
              Line h11(UX-237.5, UY, UX-237.5, UY-40), h12(UX-197.5, UY-40, UX-197.5, UY), h13(UX-237.5, UY-20, UX-197.5, UY-20);
              h11.setColor(COLOR(0, 0, 204)); h12.setColor(COLOR(0, 0, 204)); h13.setColor(COLOR(0, 0, 204));
              Circle o11(UX-172.5, UY-20, 20);
              o11.setColor(COLOR(0, 0, 204));
              Line w11(UX-147.5, UY, UX-147.5, UY-40), w12(UX-107.5, UY-40, UX-107.5, UY), w13(UX-147.5, UY, UX-127.5, UY-20), w14(UX-127.5, UY-20, UX-107.5, UY);
              w11.setColor(COLOR(0, 0, 204)); w12.setColor(COLOR(0, 0, 204)); w13.setColor(COLOR(0, 0, 204)), w14.setColor(COLOR(0, 0, 204));
              Line t11(UX-77.5, UY-40, UX-37.5, UY-40), t12(UX-57.5, UY-40, UX-57.5, UY);
              t11.setColor(COLOR(0, 0, 204)); t12.setColor(COLOR(0, 0, 204));
              Circle o21(UX-12.5, UY-20, 20);
              o21.setColor(COLOR(0, 0, 204));
              Line p11(UX+37.5, UY-40, UX+37.5, UY), p12(UX+77.5, UY-40, UX+77.5, UY-20), p13(UX+37.5, UY-40, UX+77.5, UY-40), p14(UX+37.5, UY-20, UX+77.5, UY-20);
              p11.setColor(COLOR(0, 0, 204)); p12.setColor(COLOR(0, 0, 204)); p13.setColor(COLOR(0, 0, 204)), p14.setColor(COLOR(0, 0, 204));
              Line l11(UX+82.5, UY-40, UX+82.5, UY), l12(UX+82.5, UY, UX+122.5, UY);
              l11.setColor(COLOR(0, 0, 204)); l12.setColor(COLOR(0, 0, 204));
              Line a11(UX+127.5, UY, UX+147.5, UY-40), a12(UX+147.5, UY-40, UX+167.5, UY), a13(UX+139, UY-20, UX+157.5, UY-20);
              a11.setColor(COLOR(0, 0, 204)); a12.setColor(COLOR(0, 0, 204)); a13.setColor(COLOR(0, 0, 204));
              Line y11(UX+172.5, UY-40, UX+192.5, UY-20), y12(UX+192.5, UY-20, UX+212.5, UY-40), y13(UX+192.5, UY-20, UX+192.5, UY);
              y11.setColor(COLOR(0, 0, 204)); y12.setColor(COLOR(0, 0, 204)); y13.setColor(COLOR(0, 0, 204));

              //Objective:
              Rectangle ob1(X, UY+35, 120, 30), ob2(X, UY+35, 120, 30);
              ob1.setColor(COLOR("white")); ob1.setFill(true);//ob = objective
              ob2.setColor(COLOR(102, 178, 255));
              Text ob(X, UY+35, "Objective :");
              ob.setColor(COLOR(0, 0, 204));

              Rectangle obj1(X, UY+98.75, 550, 87.5), obj2(X, UY+98.75, 550, 87.5);//obj = objective
              obj1.setColor(COLOR("white")); obj1.setFill(true);
              obj2.setColor(COLOR(102, 178, 255));
              Text objectivel(X, UY+70.5, "Try to collect maximum number of coins (gold), while avoiding");
              objectivel.setColor(COLOR(0, 0, 204));
              Text objective2(X, UY+99.5, "bombs (black). You win if you collect 10 coins without losing");
              objective2.setColor(COLOR(0, 0, 204));
              Text objective3(X, UY+128.5, "all lives. Also, make use of the magnet (red). Happy playing!");
              objective3.setColor(COLOR(0, 0, 204));

              //Controls:
              Rectangle ct1(X, UY+177.5, 120, 30), ct2(X, UY+177.5, 120, 30);
              ct1.setColor(COLOR("white")); ct1.setFill(true);
              ct2.setColor(COLOR(102, 178, 255));
              Text ct(X, UY+177.5, "Controls :");
              ct.setColor(COLOR(0, 0, 204));

              Rectangle tl1(X, UY+207.5, 550, 20), tl2(X, UY+207.5, 550, 20);//tl = throw lasso
              tl1.setColor(COLOR("white")); tl1.setFill(true);
              tl2.setColor(COLOR(102, 178, 255));
              Text tl(X, UY+207.5, "Throw the lasso                                                       T");
              tl.setColor(COLOR(0, 0, 204));
              Rectangle ll1(X, UY+232.5, 550, 20), ll2(X, UY+232.5, 550, 20);//ll = loop lasso
              ll1.setColor(COLOR("white")); ll1.setFill(true);
              ll2.setColor(COLOR(102, 178, 255));
              Text ll(X, UY+232.5, "Loop the lasso                                                         L");
              ll.setColor(COLOR(0, 0, 204));
              Rectangle yl1(X, UY+257.5, 550, 20), yl2(X, UY+257.5, 550, 20);//yl = yank lasso
              yl1.setColor(COLOR("white")); yl1.setFill(true);
              yl2.setColor(COLOR(102, 178, 255));
              Text yl(X, UY+257.5, "Yank the lasso                                                         Y");
              yl.setColor(COLOR(0, 0, 204));
              Rectangle qg1(X, UY+282.5, 550, 20), qg2(X, UY+282.5, 550, 20);//qg = quit game
              qg1.setColor(COLOR("white")); qg1.setFill(true);
              qg2.setColor(COLOR(102, 178, 255));
              Text qg(X, UY+282.5, "Quit game                                                                 Q");
              qg.setColor(COLOR(0, 0, 204));
              Rectangle ids1(X, UY+307.5, 550, 20), ids2(X, UY+307.5, 550, 20);//ids = increase decrease speed
              ids1.setColor(COLOR("white")); ids1.setFill(true);
              ids2.setColor(COLOR(102, 178, 255));
              Text ids(X, UY+307.5, "Increase or decrease throwing speed of lasso        Up & Down Arrow Keys");
              ids.setColor(COLOR(0, 0, 204));
              Rectangle ca1(X, UY+332.5, 550, 20), ca2(X, UY+332.5, 550, 20);//ca = change angle
              ca1.setColor(COLOR("white")); ca1.setFill(true);
              ca2.setColor(COLOR(102, 178, 255));
              Text ca(X, UY+332.5, "Change the launch angle of lasso                             Left & Right Arrow Keys");
              ca.setColor(COLOR(0, 0, 204));

              Rectangle htp1(UX-12.5, UY+377.5, 120, 30), htp2(UX-12.5, UY+377.5, 120, 30);
              htp1.setColor(COLOR("white")); htp1.setFill(true);
              htp2.setColor(COLOR(102, 178, 255));
              Text htp(UX-12.5, UY+377.5, "BACK");
              htp.setColor(COLOR(0, 0, 204));

              while(true){
                XEvent e;
                bool pendingEv = checkEvent(e);
                if(pendingEv){
                  if(mouseButtonPressEvent(e)){
                  if(UX-72.5<=e.xbutton.x && e.xbutton.x<=UX+47.5 && UY+362.5<=e.xbutton.y && e.xbutton.y<=UY+392.5){
                    break;
                  }
                  else continue;
                  }
                }
              }

            }
            else if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y+143<=e.xbutton.y && e.xbutton.y<=Y+173){
              Rectangle about(WINDOW_X/2, WINDOW_Y/2, WINDOW_X-130, WINDOW_Y-130);
              about.setColor(COLOR(0, 128, 255)); about.setFill(true);

              //To display : About
              Line a311(UX-122.5, UY, UX-102.5, UY-40), a312(UX-102.5, UY-40, UX-82.5, UY), a313(UX-111, UY-20, UX-92.5, UY-20);
              a311.setColor(COLOR(0, 0, 204)); a312.setColor(COLOR(0, 0, 204)); a313.setColor(COLOR(0, 0, 204));
              Line b311(UX-77.5, UY-40, UX-37.5, UY-40), b312(UX-77.5, UY-40, UX-77.5, UY), b313(UX-77.5, UY, UX-37.5, UY), b314(UX-77.5, UY-20, UX-42.5, UY-20);
              Line b321(UX-37.5, UY-40, UX-37.5, UY-25), b322(UX-37.5, UY-15, UX-37.5, UY), b323(UX-37.5, UY-25, UX-42.5, UY-20), b324(UX-37.5, UY-15, UX-42.5, UY-20);
              b311.setColor(COLOR(0, 0, 204)); b312.setColor(COLOR(0, 0, 204)); b313.setColor(COLOR(0, 0, 204)); b314.setColor(COLOR(0, 0, 204));
              b321.setColor(COLOR(0, 0, 204)); b322.setColor(COLOR(0, 0, 204)); b323.setColor(COLOR(0, 0, 204)); b324.setColor(COLOR(0, 0, 204));
              Circle o311(UX-12.5, UY-20, 20);
              o311.setColor(COLOR(0, 0, 204));
              Line u311(UX+12.5, UY-40, UX+12.5, UY), u312(UX+52.5, UY-40, UX+52.5, UY), u313(UX+12.5, UY, UX+52.5, UY);
              u311.setColor(COLOR(0, 0, 204)); u312.setColor(COLOR(0, 0, 204)); u313.setColor(COLOR(0, 0, 204));
              Line t321(UX+57.5, UY-40, UX+97.5, UY-40), t322(UX+77.5, UY-40, UX+77.5, UY);
              t321.setColor(COLOR(0, 0, 204)); t322.setColor(COLOR(0, 0, 204));

              Rectangle al11(UX-12.5, UY+51, 400, 30), al12(UX-12.5, UY+51, 400, 30);
              al11.setColor(COLOR("white")); al11.setFill(true);
              al12.setColor(COLOR(102, 178, 255));
              Text al1(UX-12.5, UY+51, "Game developed by      -      ADITYA BYJU");
              al1.setColor(COLOR(0, 0, 204));
              Rectangle al21(UX-12.5, UY+116, 400, 30), al22(UX-12.5, UY+116, 400, 30);
              al21.setColor(COLOR("white")); al21.setFill(true);
              al22.setColor(COLOR(102, 178, 255));
              Text al2(UX-12.5, UY+116, "Roll No.                  -                    20D070004");
              al2.setColor(COLOR(0, 0, 204));
              Rectangle al31(UX-12.5, UY+181, 400, 30), al32(UX-12.5, UY+181, 400, 30);
              al31.setColor(COLOR("white")); al31.setFill(true);
              al32.setColor(COLOR(102, 178, 255));
              Text a31(UX-12.5, UY+181, "Game developed as project for CS101 Course");
              a31.setColor(COLOR(0, 0, 204));
              Rectangle al41(UX-12.5, UY+246, 400, 30), al42(UX-12.5, UY+246, 400, 30);
              al41.setColor(COLOR("white")); al41.setFill(true);
              al42.setColor(COLOR(102, 178, 255));
              Text al4(UX-12.5, UY+246, "General colour layout      -      shades of blue");
              al4.setColor(COLOR(0, 0, 204));
              Rectangle al51(UX-12.5, UY+311, 400, 30), al52(UX-12.5, UY+311, 400, 30);
              al51.setColor(COLOR("white")); al51.setFill(true);
              al52.setColor(COLOR(102, 178, 255));
              Text al5(UX-12.5, UY+311, "coin -> gold, bomb -> black, magnet -> red");
              al5.setColor(COLOR(0, 0, 204));

              Rectangle ab1(UX-12.5, UY+377.5, 120, 30), ab2(UX-12.5, UY+377.5, 120, 30);
              ab1.setColor(COLOR("white")); ab1.setFill(true);
              ab2.setColor(COLOR(102, 178, 255));
              Text ab(UX-12.5, UY+377.5, "BACK");
              ab.setColor(COLOR(0, 0, 204));

              while(true){
                XEvent e;
                bool pendingEv = checkEvent(e);
                if(pendingEv){
                  if(mouseButtonPressEvent(e)){
                    if(UX-72.5<=e.xbutton.x && e.xbutton.x<=UX+47.5 && UY+362.5<=e.xbutton.y && e.xbutton.y<=UY+392.5){
                    break;
                  }
                  else continue;
                  }
                }
              }
            }
            else if(X-60<=e.xbutton.x && e.xbutton.x<=X+60 && Y+222<=e.xbutton.y && e.xbutton.y<=Y+252){
              exit(0);
            }
            else continue;
          }
        }
      }
    }
    MainMenu = 0;
  }

    if(magActive){
      lasso.check_for_coin_when_magActive(&coin1);
      lasso.check_for_coin_when_magActive(&coin2);
      lasso.check_for_coin_when_magActive(&coin3);
      lasso.check_for_coin_when_magActive(&coin4);
      lasso.check_for_coin_when_magActive(&coin5);
      if(magTime <= 10){
        magTime += stepTime;
      }
      else{
        magTime = 0;
        magActive = false;
        lasso.resetMagActive();
      }
    }

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)
  wait(3);
} // End main_program
// Thankyou :)
