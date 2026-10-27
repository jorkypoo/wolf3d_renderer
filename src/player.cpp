#include "../inc/game.h"
#include "../inc/entity.h"

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
  /* v2 tomov = { dir.x * mov, dir.y * mov };
  int next_x = (int) (pos.x + tomov.x + sign(tomov.x) * collision_radius);
  int cur_y  = (int) pos.y;
  int next_y = (int) (pos.y + tomov.y + sign(tomov.y) * collision_radius);
  int cur_x  = (int) pos.x;
  if (GAMEMAP[cur_y * MAPX + next_x].is_air)
    pos.x += tomov.x;
  if (GAMEMAP[next_y * MAPX + cur_x].is_air)
    pos.y += tomov.y; */

  v2 tomov = { dir.x * mov, dir.y * mov };

  pos.x += tomov.x; // Apply X movement first

  for (Entity* e : ENTITIES) {
    double dx = pos.x - e->get_posx();
    double dy = pos.y - e->get_posy();
    double distSq = SQ(dx) + SQ(dy);
    double minDist = collision_radius + e->get_radius();

    if (distSq < minDist * minDist) {
      double dist = sqrt(distSq);
      if (dist == 0.0f) dist = 0.001f; // Prevent division by zero
            
      double overlap = minDist - dist;
      // Push player away along the X axis
      pos.x += (dx / dist) * overlap; 
    }
  }

  int next_x = (int)(pos.x + sign(tomov.x) * collision_radius);
  int cur_y  = (int)pos.y;

  if (!GAMEMAP[cur_y * MAPX + next_x].is_air) {
    if (tomov.x > 0) {
      // Moving Right: Snap to the left edge of the wall tile
      pos.x = (double)next_x - collision_radius - 0.001f;
    } else {
      // Moving Left: Snap to the right edge of the wall tile
      pos.x = (double)next_x + 1.0f + collision_radius + 0.001f;
    }
  }

  pos.y += tomov.y; 
  for (Entity* e : ENTITIES) {
    double dx = pos.x - e->get_posx(); // Uses the freshly updated pos.x
    double dy = pos.y - e->get_posy();
    double distSq = SQ(dx) + SQ(dy);
    double minDist = collision_radius + e->get_radius();

    if (distSq < minDist * minDist) {
      double dist = sqrt(distSq);
      if (dist == 0.0f) dist = 0.001f;
      double overlap = minDist - dist;
      pos.y += (dy / dist) * overlap; 
    }
  }

  int cur_x  = (int)pos.x;
  int next_y = (int)(pos.y + sign(tomov.y) * collision_radius);

  if (!GAMEMAP[next_y * MAPX + cur_x].is_air) {
    if (tomov.y > 0) {
      pos.y = (double)next_y - collision_radius - 0.001f;
    } else if (tomov.y < 0) {
      pos.y = (double)next_y + 1.0f + collision_radius + 0.001f;
    }
  }
}
