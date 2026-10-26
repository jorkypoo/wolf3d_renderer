#include "../inc/game.h"

Player::Player() {
  pos = {0,0};
  dir = {-1,0};
  plane = {0,0.7};

  collision_radius = 0.2;

  param = Parameters(1);
}

Player::Player(double fov) {
  pos = {0,0};
  dir = {-1,0};
  plane = {0,fov};

  collision_radius = 0.2;

  param = Parameters(1);
}

void Player::set_spawn_point(double x, double y) {
  pos.x = x;
  pos.y = y;
  dir = { -1.0, 0.0 };
  // plane = { 0.0, 0.7 };
}

void Player::rotate(double rot) {
  const v2 od = dir;
  const v2 op = plane;

  dir.x = od.x * cos(rot) - od.y * sin(rot);
  dir.y = od.x * sin(rot) + od.y * cos(rot);
  plane.x = op.x * cos(rot) - op.y * sin(rot);
  plane.y = op.x * sin(rot) + op.y * cos(rot);
}

void Player::move(double mov) {
  v2 tomov = { dir.x * mov, dir.y * mov };
  
  int next_x = (int) (pos.x + tomov.x + sign(tomov.x) * collision_radius);
  int cur_y  = (int) pos.y;

  // this function must have access to the global gamemap (which it will)
  // so it can check

  if (GAMEMAP[cur_y * MAPX + next_x].is_air)
    pos.x += tomov.x;

  int cur_x  = (int) pos.x;
  int next_y = (int) (pos.y + tomov.y + sign(tomov.y) * collision_radius);

  if (GAMEMAP[next_y * MAPX + cur_x].is_air)
    pos.y += tomov.y;
}
