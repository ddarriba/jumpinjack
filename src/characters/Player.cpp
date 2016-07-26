/*
 * Player.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "Player.h"
#include "../items/Gunshot.h"

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
    att_jump  = 20;
    att_speed = 15;
    direction = DIRECTION_RIGHT;
  }

  Player::~Player ()
  {
  }

  void Player::onCreate (void)
  {

  }

  void Player::onDestroy (void)
  {
	 unsetStatus (STATUS_LISTENING);
	 setStatus (STATUS_DYING);
  }

  t_collision Player::onCollision (Drawable * item, t_direction dir,
                                   t_itemtype type, t_point & point,
                                   t_point & delta, t_point * otherpoint,
                                   t_point * otherdelta)
  {
	if (!item) return COLLISION_IGNORE;
	t_collision collisionEffect = item->getCollisionEffect(ITEM_PLAYER, dir);
	if (!getStatus (STATUS_UNTOUCHABLE) && collisionEffect == COLLISION_HIT)
	{
		if (!getStatus (STATUS_UNTOUCHABLE))
		{
		  if (dir & DIRECTION_LEFT)
		  {
		    delta.x = 25;
		  }
		  else
		  {
		    delta.x = -25;
		  }
		  current_state = PLAYER_HIT;
		  unsetStatus (STATUS_LISTENING);
		  setStatus (STATUS_UNTOUCHABLE);
		  sprite_index = 0;
		  hit_counter = 0;
		}
		return COLLISION_DIE;
	}
    if (type == ITEM_ENEMY)
      {
        if (dir & DIRECTION_DOWN)
          {
            /* jump */
            if (delta.y < 0)
              {
                delta.y -= att_jump / 2;
                //onJump = 1;
              }
            else
              {
                delta.y = -att_jump;
                //onJump = 1;
              }
          }
        else
          {
            if (!getStatus (STATUS_UNTOUCHABLE))
              {
                /* hit */
                //delta.y = -15;
                if (dir & DIRECTION_LEFT)
                  {
                    delta.x = 25;
                  }
                else
                  {
                    delta.x = -25;
                  }
                current_state = PLAYER_HIT;
                unsetStatus (STATUS_LISTENING);
                setStatus (STATUS_UNTOUCHABLE);
                sprite_index = 0;
                hit_counter = 0;
              }
          }
      }
    return COLLISION_IGNORE;
  }

  void Player::update (SDL_Point & next_point)
  {
    if (current_state == PLAYER_HIT || hit_counter)
      {
        hit_counter++;
        //*y += 10;
        if (hit_counter >= sprite_length)
          {
            if (hit_counter == sprite_length)
              {
                setStatus (STATUS_LISTENING);
                current_state = PLAYER_STAND;
              }
            else if (hit_counter >= 4 * sprite_length)
              {
                unsetStatus (STATUS_UNTOUCHABLE);
                hit_counter = 0;
              }
          }
      }

    sprite_line = sprite_start_line + (int) current_state;
    if (current_state == PLAYER_JUMP)
      {
        sprite_index = min (sprite_index, sprite_length - 2);
      }
    ActiveDrawable::update(next_point);
  }

  void Player::renderFixed (t_point point)
  {
    if (!getStatus(STATUS_UNTOUCHABLE) || !((hit_counter/3)%2))
      ActiveDrawable::renderFixed (point);
  }

  void Player::jump()
  {
    jumpId++;
  }

  Projectile * Player::createProjectile(t_point & delta) const
  {
    return new Gunshot (
        renderer,
        GlobalDefs::getResource (RESOURCE_IMAGE, "bullet.png"),
        getDirection (),
        delta, 0, 60, 0, 750);
  }
} /* namespace sdlfw */
