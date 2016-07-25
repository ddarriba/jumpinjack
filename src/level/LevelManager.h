/*
 * LevelManager.h
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#ifndef LEVEL_LEVELMANAGER_H_
#define LEVEL_LEVELMANAGER_H_

#include "Surface.h"
#include "../GlobalDefs.h"
#include "../sdl/BackgroundDrawable.h"
#include "../characters/Player.h"

#define PARALLAX_LAYERS 3

#include <vector>

namespace jumpinjack
{

  typedef struct
  {
    std::string filename;
    int parallax_level;
    bool repeat_x;
    int parallax_speed;
  } t_parallax_layer;

  typedef struct
  {
    std::string sprite_filename;
    int sprite_len;
    int sprite_start;
    int sprite_speed;
    t_itemtype type;
    t_point start_point;
    t_point start_delta;
  } t_item_desc;

  typedef struct
  {
    std::vector<t_point> player_start_point;
    std::vector<t_point> player_start_delta;
    std::vector<t_parallax_layer> parallax_layers;
    std::string surface_filename;
    std::vector<t_item_desc> items;
  } t_level_data;

  typedef struct
  {
      DrawableItem * item;
      t_itemtype type;
      t_point point;
      t_point delta;
  } itemInfo;

  class LevelManager
  {
    public:
      LevelManager (SDL_Renderer * renderer, int level_id,
                    std::vector<Player *> & players);
      virtual ~LevelManager ();

      void applyAction (int player_id, t_action action);

      void update ();

      void render ();

    private:
      bool updatePosition (itemInfo & it);
      t_move canMoveTo (t_point p, ActiveDrawable * character, t_direction dir);
      void loadLevelData(std::vector<Player *> & players);

      SDL_Renderer * renderer;

      int level_id;
      int level_width;
      int num_players;
      std::vector<itemInfo> items;
      std::vector<BackgroundDrawable *> bg_layers;
      Surface * level_surface;

      t_point checkpoint;
      t_level_data level_data;

  };

} /* namespace jumpinjack */

#endif /* LEVEL_LEVELMANAGER_H_ */
