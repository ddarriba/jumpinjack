/*
 * LevelManager.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "LevelManager.h"
#include "../characters/Enemy.h"
#include <sstream>
#include <fstream>
#include "../items/Gunshot.h"
#include "../items/StaticAnimation.h"

using namespace std;

namespace jumpinjack
{

  static void parse_level_file(int level_id, int n_players, t_level_data & level_data)
  {
    stringstream ss;
    string line;
    string filename;
    ss << "level" << level_id << ".dat";
    filename = GlobalDefs::getResource (RESOURCE_DATA, ss.str().c_str());

    ifstream myfile (filename);
    assert (myfile.is_open ());

    level_data.player_start_point.reserve (n_players);
    level_data.player_start_delta.reserve (n_players);

    for (int i = 0; i < n_players; ++i)
    {
      t_point position, delta;
      getline (myfile, line);
      sscanf (line.c_str (), "{%d,%d} {%d,%d}", &position.x, &position.y,
              &delta.x, &delta.y);
      level_data.player_start_point.push_back (position);
      level_data.player_start_delta.push_back (delta);
    }

    for (int i = n_players; i < MAX_PLAYERS; i++)
    {
      /* skip line */
      getline (myfile, line);
    }

    level_data.parallax_layers.resize (PARALLAX_LAYERS);
    for (int i = 0; i < PARALLAX_LAYERS; i++)
    {
      getline (myfile, line);
      char bg_file_cstr[300];
      string bg_file;
      int parallax_level;
      int repeat_x;
      int auto_speed;
      sscanf (line.c_str (), "%s %d %d %d", bg_file_cstr, &parallax_level,
              &repeat_x, &auto_speed);
      bg_file = GlobalDefs::getResource (RESOURCE_IMAGE, bg_file_cstr);

      level_data.parallax_layers[i].filename = bg_file;
      level_data.parallax_layers[i].parallax_level = parallax_level;
      level_data.parallax_layers[i].repeat_x = repeat_x;
      level_data.parallax_layers[i].parallax_speed = auto_speed;
    }

    getline (myfile, line);
    level_data.surface_filename = GlobalDefs::getResource (RESOURCE_IMAGE, line.c_str ());

    getline (myfile, line);
    int n_items;
    sscanf (line.c_str (), "%d", &n_items);
    level_data.items.resize (n_items);

    for (int i = 0; i < n_items; ++i)
    {
      getline (myfile, line);
      char resource_img[300], item_typestr[40];
      int sprite_len, sprite_start, sprite_freq;
      t_point item_pos, item_delta;
      t_itemtype item_type = ITEM_PASSIVE;
      sscanf (line.c_str (), "%s %d %d %d %s {%d,%d} {%d,%d}", resource_img,
              &sprite_len, &sprite_start, &sprite_freq, item_typestr,
              &item_pos.x, &item_pos.y, &item_delta.x, &item_delta.y);
      if (!strcmp (item_typestr, "ITEM_ENEMY"))
        item_type = ITEM_ENEMY;

      level_data.items[i].sprite_filename = GlobalDefs::getResource (RESOURCE_IMAGE, resource_img);
      level_data.items[i].sprite_len      = sprite_len;
      level_data.items[i].sprite_start    = sprite_start;
      level_data.items[i].sprite_speed    = sprite_freq;
      level_data.items[i].type            = item_type;
      level_data.items[i].start_point     = item_pos;
      level_data.items[i].start_delta     = item_delta;
    }
    myfile.close ();
  }

  void LevelManager::loadLevelData(vector<Player *> & players)
  {
    /* clean */
    for (itemInfo item : items)
    {
      if (item.type != ITEM_PLAYER)
        delete item.item;
    }
    items.clear();

    for (BackgroundDrawable * bg_layer : bg_layers)
    {
      delete bg_layer;
    }
    bg_layers.clear();

    if (level_surface)
    {
      delete level_surface;
      level_surface = 0;
    }

    items.reserve (MAX_LEVEL_ITEMS);
    int i = 0;
    for (Player * player : players)
    {
      items.push_back (
            { player, ITEM_PLAYER, level_data.player_start_point[i],
                level_data.player_start_delta[i] });
      checkpoint = level_data.player_start_point[i];
      ++i;
    }
    bg_layers.reserve (level_data.parallax_layers.size ());
    for (t_parallax_layer & p_layer : level_data.parallax_layers)
      {
        bg_layers.push_back (
            new BackgroundDrawable (renderer, p_layer.filename,
                                    p_layer.parallax_level, p_layer.repeat_x,
                                    p_layer.parallax_speed));
      }
    level_surface = new Surface (level_data.surface_filename);
    for (t_item_desc & item_desc : level_data.items)
    {
      switch (item_desc.type)
      {
        case ITEM_ENEMY:
          items.push_back (
                { new Enemy (renderer, item_desc.sprite_filename,
                             item_desc.sprite_len, item_desc.sprite_start,
                             item_desc.sprite_speed), ITEM_ENEMY,
                    item_desc.start_point, item_desc.start_delta });
          break;
        default:
          assert (0);
      }
    }
  }

  LevelManager::LevelManager (SDL_Renderer * renderer, int level_id,
                              vector<Player *> & players) :
      renderer (renderer), level_id (level_id), num_players (players.size ())
  {
    level_surface = 0;

    parse_level_file(level_id, players.size(), level_data);

    loadLevelData(players);

    level_width = 4000;

    sound_manager = new SoundManager();
    sound_jump  = sound_manager->loadFromFile(
        GlobalDefs::getResource (RESOURCE_SOUND, "jump001.wav"));
    sound_shoot = sound_manager->loadFromFile(
        GlobalDefs::getResource (RESOURCE_SOUND, "shoot001.wav"));
    sound_bgmusic = sound_manager->loadMusic(
        GlobalDefs::getResource (RESOURCE_SOUND, "music001.ogg"));

    sound_manager->playMusic(sound_bgmusic);

    paused = false;
  }

  LevelManager::~LevelManager ()
  {
    for (itemInfo & it : items)
      if (it.type != ITEM_PLAYER)
        delete it.item;
    for (BackgroundDrawable * bg : bg_layers)
      delete bg;
    delete level_surface;
    delete sound_manager;
  }

  void LevelManager::applyAction (int player_id, t_action action)
  {
    assert (player_id < num_players);
    itemInfo & player_info = items[player_id];
    Player * player = (Player *) player_info.item;

    if (!player->getStatus (STATUS_LISTENING))
      return;

    bool run = false;
    int max_speed =
        (action & ACTION_SPRINT) ?
            1.5 * player->getSpeed () : player->getSpeed ();
    if (action & ACTION_RIGHT)
    {
      player->setState (PLAYER_RUN);
      player->setDirection (DIRECTION_RIGHT);
      player_info.delta.x += player->getAccel ();
      if (player_info.delta.x > max_speed)
        player_info.delta.x = max_speed;
      run = true;
    }
    if (action & ACTION_LEFT)
    {
      player->setState (PLAYER_RUN);
      player->setDirection (DIRECTION_LEFT);
      player_info.delta.x -= player->getAccel ();
      if (player_info.delta.x < -max_speed)
        player_info.delta.x = -max_speed;
      run = true;
    }
    if (action & ACTION_SHOOT)
    {
      t_point point = player_info.point;
      if (player->getDirection () == DIRECTION_RIGHT)
        point.x += 50;
      else
        point.x -= 50;
      t_point delta;
      Projectile * projectile = new Gunshot (
          renderer, GlobalDefs::getResource (RESOURCE_IMAGE, "bullet.png"),
          player->getDirection (), delta, 0, 40, 0, 750);
      itemInfo shoot_info =
        { projectile, ITEM_PROJECTILE, point, delta };
      items.push_back (shoot_info);
      sound_manager->playSound(sound_shoot);
    }
    if (!run && !player_info.delta.x)
      player->setState (PLAYER_STAND);
    if (action & ACTION_UP)
    {
      if (player->onJump < GlobalDefs::jump_sensitivity)
      {

        if ((player->jumpId < player->multipleJump ()) && !player->onJump)
        {
          player_info.delta.y = 0;
          player->jump();
          sound_manager->playSound(sound_jump);
        }

        /* ignore gravity when jumping */
        player_info.delta.y -= GlobalDefs::base_gravity;
        int jump_power = player->getJump () + abs (player_info.delta.x) / 5;
        if (!player->onJump)
        {
          player_info.delta.y -= jump_power;
        }
        player->onJump++;
      }
    }
    if (player_info.delta.y < 0)
    {
      player->setState (PLAYER_JUMP);
    }
    else if (player_info.delta.y > 0)
    {
      player->setState (PLAYER_FALL);
    }

    if (action & ACTION_UP_REL)
    {
      /* prevent jump while on air */
      if (player->onJump > 0 && player->onJump <= JUMPING_TRIGGER)
      {
        player->onJump = JUMPING_TRIGGER;
      }
      if (player->jumpId < player->multipleJump ())
      {
        player->onJump = JUMPING_RESET;
      }
    }
  }

  t_move LevelManager::canMoveTo (t_point p, ActiveDrawable * character,
                                t_direction dir)
  {
    pixelType pixel;
    bool move_ok = false;
    switch (dir & 0xF)
    {
      case DIRECTION_DOWN:
        p.y++;
        pixel = level_surface->testPixel (p);
        move_ok = (pixel != PIXELTYPE_SOLID && pixel != PIXELTYPE_DOWN_ONLY);
        break;
      case DIRECTION_UP:
        p.y -= character->getHeight ();
        pixel = level_surface->testPixel (p);
        move_ok = (pixel != PIXELTYPE_SOLID && pixel != PIXELTYPE_UP_ONLY);
        break;
      case DIRECTION_LEFT:
        p.x -= character->getWidth () / 4;
        pixel = level_surface->testPixel (p);
        move_ok = (pixel != PIXELTYPE_SOLID);
        break;
      case DIRECTION_RIGHT:
        p.x += character->getWidth () / 4;
        pixel = level_surface->testPixel (p);
        move_ok = (pixel != PIXELTYPE_SOLID);
        break;
      default:
        assert (0);
    }
    if (pixel == PIXELTYPE_DEATH)
      return MOVE_DEATH;
    if (move_ok)
      return MOVE_OK;
    return MOVE_NOT;
  }

  template<typename T>
    int sgn (T val)
    {
      return (T (0) < val) - (val < T (0));
    }

  bool detectCollision (itemInfo & it1, itemInfo & it2,
                        t_direction * collision_direction)
  {
    t_point min1 =
      { it1.point.x - it1.item->getWidth () / 3, it1.point.y
          - it1.item->getHeight () };
    t_point max1 =
      { it1.point.x + it1.item->getWidth () / 3, it1.point.y };
    t_point min2 =
      { it2.point.x - it2.item->getWidth () / 3, it2.point.y
          - it2.item->getHeight () };
    t_point max2 =
      { it2.point.x + it2.item->getWidth () / 3, it2.point.y };
    if (min2.x > max1.x || min1.x > max2.x || min2.y > max1.y
        || min1.y > max2.y)
      return false;
    else
    {
      t_direction hdir =
          (it1.point.x < it2.point.x) ? DIRECTION_RIGHT : DIRECTION_LEFT;
      *collision_direction = (t_direction) (DIRECTION_HORIZONTAL | hdir);
      if (it1.point.y < (it2.point.y - it2.item->getHeight () / 2))
      {
        t_direction vdir = DIRECTION_DOWN;
        *collision_direction = (t_direction) (*collision_direction
            | DIRECTION_VERTICAL | vdir);
      }
      else if (it2.point.y < (it1.point.y - it1.item->getHeight () / 2))
      {
        t_direction vdir = DIRECTION_UP;
        *collision_direction = (t_direction) (*collision_direction
            | DIRECTION_VERTICAL | vdir);
      }
      return true;
    }
  }

  bool LevelManager::updatePosition (itemInfo & it)
  {
    int friction = GlobalDefs::base_friction;
    int gravity = GlobalDefs::base_gravity;

    if (it.type == ITEM_PASSIVE)
    {
      it.item->update (it.delta);
    }
    else
    {
      gravity = (int) round(gravity * dynamic_cast<ActiveDrawable *>(it.item)->getGravityEffect());
      ActiveDrawable * character = (ActiveDrawable *) it.item;
      character->update (it.delta);

      /* move horizontal */
      if (it.delta.x)
      {
        if (it.delta.x > 0)
          it.delta.x = max (it.delta.x - friction, 0);
        else
          it.delta.x = min (it.delta.x + friction, 0);
        int inc = sgn (it.delta.x);
        t_direction dir = (inc > 0) ? DIRECTION_RIGHT : DIRECTION_LEFT;
        for (int i = 0; i < abs (it.delta.x); i++)
        {
          t_move move_result = canMoveTo (it.point, character, dir);
          if (move_result == MOVE_OK)
          {
            it.point.x += inc;
          }
          else if (move_result == MOVE_DEATH)
          {
            character->onDestroy ();
          }
          else
          {
            t_collision collision_result = character->onCollision (0, DIRECTION_HORIZONTAL, ITEM_PASSIVE,
                                        it.point, it.delta);
            if (collision_result != COLLISION_IGNORE)
            {
              it.delta.x = 0;
              if (collision_result == COLLISION_EXPLODE)
              {
                StaticAnimation * explosion = new StaticAnimation (
                    renderer,
                    GlobalDefs::getResource (RESOURCE_IMAGE, "explosion.png"), 11,
                    0, 2, LIFESPAN_ONE_LOOP, 0);
                itemInfo explosion_info =
                  { explosion, ITEM_PASSIVE, it.point,
                    { 0, 0 } };
                items.push_back (explosion_info);
                ((ActiveDrawable *) it.item)->onDestroy ();
              }
            }
          }
        }

        if (it.type == ITEM_PLAYER)
        {
          if (it.point.x < 0)
            it.point.x = 0;
          else if (it.point.x > level_width)
            it.point.x = level_width;
        }
      }

      /* gravity */
      t_move move_result = canMoveTo (it.point, character, DIRECTION_DOWN);
      if (move_result == MOVE_OK)
      {
        it.delta.y = min (GlobalDefs::max_falling_speed, it.delta.y + gravity);
        character->jumpId = max (1, character->jumpId);
      }
      else if (move_result == MOVE_DEATH)
      {
        character->onDestroy ();
      }
      else
        character->jumpId = 0;

      /* move vertical */
      if (it.delta.y)
      {
        int inc = sgn (it.delta.y);
        t_direction dir = (inc > 0) ? DIRECTION_DOWN : DIRECTION_UP;
        for (int i = 0; i < abs (it.delta.y); i++)
        {
          t_move move_result = canMoveTo (it.point, character, dir);
          if (move_result == MOVE_OK)
          {
            it.point.y += inc;
            if (!(character->jumpId < character->multipleJump ())
                && dir == DIRECTION_DOWN)
            {
              if (!character->onJump)
              {
                character->onJump = JUMPING_TRIGGER;
              }
              else if (character->onJump < GlobalDefs::jump_sensitivity)
                character->onJump = GlobalDefs::jump_sensitivity;
            }
          }
          else if (move_result == MOVE_DEATH)
          {
            character->onDestroy ();
          }
          else
          {
            if (dir == DIRECTION_DOWN)
            {
              if (character->onJump == JUMPING_TRIGGER)
              {
                character->jumpId = 0;
                character->onJump = JUMPING_RESET;
              }
              else if (character->onJump)
                character->onJump = (JUMPING_TRIGGER + 1);
            }
            if (character->onCollision (
                0, (t_direction) (DIRECTION_VERTICAL | dir), ITEM_PASSIVE,
                it.point, it.delta) == COLLISION_IGNORE)
              it.delta.y = 0;
            break;
          }
        }
      }
      /* move vertical */

      if (it.type == ITEM_PLAYER && character->getStatus (STATUS_DYING))
      {
        character->unsetStatus (STATUS_DYING);
        character->setStatus (STATUS_LISTENING);
        it.point = checkpoint;
        it.delta = { 0,0};
        return false;
      }
    }
    return true;
  }

  t_direction reverseDirection (t_direction dir)
  {
    t_direction newdir = dir;
    if (dir & DIRECTION_LEFT)
    {
      newdir = (t_direction) ((newdir & ~DIRECTION_LEFT) | DIRECTION_RIGHT);
    }
    else if (dir & DIRECTION_RIGHT)
    {
      newdir = (t_direction) ((newdir & ~DIRECTION_RIGHT) | DIRECTION_LEFT);
    }
    if (dir & DIRECTION_UP)
    {
      newdir = (t_direction) ((newdir & ~DIRECTION_UP) | DIRECTION_DOWN);
    }
    else if (dir & DIRECTION_DOWN)
    {
      newdir = (t_direction) ((newdir & ~DIRECTION_DOWN) | DIRECTION_UP);
    }
    return newdir;
  }

  void LevelManager::update ()
  {
    bool alive = true;
    /* update positions */
    for (size_t i = 0; i < items.size (); i++)
    {
      if (items[i].item->getStatus (STATUS_ALIVE))
      {
        alive &= updatePosition (items[i]);
      }
      else
      {
        delete items[i].item;
        items.erase (items.begin () + i);
        i--;
      }
    }

    if (!alive)
    {
      vector<Player *> players;
      for (itemInfo item : items)
      {
        if (item.type == ITEM_PLAYER)
          players.push_back(dynamic_cast<Player *>(item.item));
      }
      loadLevelData(players);
    }

    /* collision detection */
    t_direction collision_direction;
    for (size_t i = 0; i < items.size (); i++)
    {
      itemInfo & item1 = items[i];
      if (!item1.item->getStatus (STATUS_LISTENING))
        continue;
      for (size_t j = i + 1; j < items.size (); j++)
      {
        itemInfo & item2 = items[j];
        if (!item2.item->getStatus (STATUS_LISTENING))
          continue;
        if (detectCollision (item1, item2, &collision_direction))
        {
          t_collision collision_result;
          if (item1.type != ITEM_PASSIVE)
          {
            collision_result = ((ActiveDrawable *) item1.item)->onCollision (
                item2.item, collision_direction, item2.type, item1.point,
                item1.delta, &item2.point, &item2.delta);
            if (collision_result == COLLISION_DIE)
              ((ActiveDrawable *) item1.item)->onDestroy ();
            if (collision_result == COLLISION_EXPLODE)
            {
              StaticAnimation * explosion = new StaticAnimation (
                  renderer,
                  GlobalDefs::getResource (RESOURCE_IMAGE, "explosion.png"), 11,
                  0, 2, LIFESPAN_ONE_LOOP, 0);
              itemInfo explosion_info =
                { explosion, ITEM_PASSIVE, item1.point,
                  { 0, 0 } };
              items.push_back (explosion_info);
              ((ActiveDrawable *) item1.item)->onDestroy ();
            }
          }
          if (item2.type != ITEM_PASSIVE)
          {
            collision_result = ((ActiveDrawable *) item2.item)->onCollision (
                item1.item, reverseDirection (collision_direction), item1.type,
                item2.point, item2.delta, &item1.point, &item1.delta);
            if (collision_result == COLLISION_DIE)
              ((ActiveDrawable *) item2.item)->onDestroy ();
            if (collision_result == COLLISION_EXPLODE)
            {
              StaticAnimation * explosion = new StaticAnimation (
                  renderer,
                  GlobalDefs::getResource (RESOURCE_IMAGE, "explosion.png"), 11,
                  0, 2, LIFESPAN_ONE_LOOP, 0);
              itemInfo explosion_info =
                { explosion, ITEM_PASSIVE, item2.point,
                  { 0, 0 } };
              items.push_back (explosion_info);
              ((ActiveDrawable *) item2.item)->onDestroy ();
            }
          }
        }
      }
    }
  }

  void LevelManager::render ()
  {
    int xOffset =
        (items[0].point.x > GlobalDefs::window_size.x / 2) ?
            (items[0].point.x - GlobalDefs::window_size.x / 2) : 0;
    if (xOffset > (level_width - GlobalDefs::window_size.x))
      xOffset = (level_width - GlobalDefs::window_size.x);

    for (BackgroundDrawable * bg : bg_layers)
    {
      bg->renderFixed (
        { xOffset, 0 });
    }
    for (itemInfo & it : items)
    {
      int effectiveX = it.point.x - xOffset;

      if (effectiveX > -it.item->getWidth ()
          && effectiveX < (GlobalDefs::window_size.x + it.item->getWidth ()))
      {
        t_point render_point =
          { effectiveX, it.point.y };
        it.item->renderFixed (render_point);
      }
    }
  }

  void LevelManager::pause(bool set)
  {
    if (set == paused)
      return;

    paused = set;
    sound_manager->setMusicVolume(paused?48:128);
  }
} /* namespace jumpinjack */
