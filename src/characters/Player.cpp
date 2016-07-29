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
                          sprite_start_line, sprite_frequency, {32, 32})
  {
    base_sprite_frequency = sprite_frequency;
    resetState();
  }

  Player::~Player ()
  {
  }

  void Player::resetState(void)
  {
    unsetStatus(STATUS_ALL);
    setStatus(STATUS_ALIVE);
    setStatus(STATUS_LISTENING);

    att_accel = 5;
    att_jump  = 20;
    att_speed = 15;

    render_size = {32,32};

    sprite_line = sprite_start_line;
    player_state = PLAYER_RUN;
    direction = DIRECTION_RIGHT;

    status_count = 0;
    sprite_frequency = base_sprite_frequency;
  }

  void Player::setPlayerState (playerState new_state)
  {
    player_state = new_state;
  }

  playerState Player::getPlayerState () const
  {
    return player_state;
  }

  void Player::onCreate (void)
  {

  }

  void Player::onDestroy (void)
  {
    if (!getStatus(STATUS_DYING))
    {
      resetSpriteIndex();
      unsetStatus (STATUS_LISTENING);
      setStatus (STATUS_DYING);
      status_count = 0;
      player_state = PLAYER_DEAD;
    }
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
		  player_state = PLAYER_HIT;
		  unsetStatus (STATUS_LISTENING);
		  setStatus (STATUS_UNTOUCHABLE);
		  resetSpriteIndex();
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
                player_state = PLAYER_HIT;
                unsetStatus (STATUS_LISTENING);
                setStatus (STATUS_UNTOUCHABLE);
                resetSpriteIndex();
                hit_counter = 0;
              }
          }
      }
    return COLLISION_IGNORE;
  }

  void Player::update (t_point & next_point)
  {
    if (getStatus (STATUS_DYING))
    {
      status_count++;
      render_size.x += 20;
      render_size.y += 20;
      if (status_count >= (4*base_sprite_frequency))
      {
        unsetStatus (STATUS_ALIVE);
      }
    }
    else
    {
      if (player_state == PLAYER_HIT || hit_counter)
      {
        hit_counter++;
        //*y += 10;
        if (hit_counter >= sprite_length)
        {
          if (hit_counter == sprite_length)
          {
            setStatus (STATUS_LISTENING);
            player_state = PLAYER_STAND;
          }
          else if (hit_counter >= 4 * sprite_length)
          {
            unsetStatus (STATUS_UNTOUCHABLE);
            hit_counter = 0;
          }
        }
      }
    }
    sprite_line = sprite_start_line + (int) player_state + (status_count/base_sprite_frequency);
    if (player_state == PLAYER_JUMP)
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
