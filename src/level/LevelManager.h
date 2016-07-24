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
      void updatePosition (itemInfo & it);
      t_move canMoveTo (t_point p, ActiveDrawable * character, t_direction dir);

      SDL_Renderer * renderer;

      int level_id;
      int level_width;
      int num_players;
      std::vector<itemInfo> items;
      std::vector<BackgroundDrawable *> bg_layers;
      Surface * level_surface;

      t_point checkpoint;

  };

} /* namespace jumpinjack */

#endif /* LEVEL_LEVELMANAGER_H_ */
