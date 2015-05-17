/*
 * Player.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "Player.h"

using namespace std;

namespace jumpinjack
{

  Player::Player (SDL_Renderer * renderer, std::string sprite_file,
                  int sprite_length, int sprite_start_line,
                  int sprite_frequency) :
          ActiveDrawable (renderer, sprite_file, sprite_length,
                          sprite_start_line, sprite_frequency)
  {
    current_state = PLAYER_RUN;
    att_accel = 5;
    att_jump = 20;
    att_speed = 10;
  }

  Player::~Player ()
  {
  }

  void Player::onCreate (void)
  {

  }

  void Player::onDestroy (void)
  {

  }

  t_collision Player::onCollision (Drawable * item, t_direction dir,
                                   t_itemtype type, t_point & point,
                                   t_point & delta)
  {
    if ((dir & DIRECTION_UP) && type == ITEM_ENEMY)
      {
        /* jump */
        if (delta.y < 0)
          {
            delta.y -= att_jump / 2;
            onJump = 1;
          }
        else
          {
            delta.y = -att_jump;
            onJump = 1;
          }
      }
    return COLLISION_IGNORE;
  }

  void Player::update (SDL_Point & next_point)
  {

  }

  void Player::renderFixed (t_point point)
  {

    sprite_line = sprite_start_line + (int) current_state;
    if (current_state == PLAYER_JUMP)
      {
        sprite_index = min (sprite_index, sprite_length - 2);
      }
    ActiveDrawable::renderFixed (point);
  }
} /* namespace sdlfw */
