/*
 * DeathScreen.h
 *
 *  Created on: May 24, 2015
 *      Author: diego
 */

#ifndef DEATH_SCREEN_H
#define DEATH_SCREEN_H

#include "AbstractMenu.h"

const t_point death_screen_size =
  { 400, 300 };

namespace jumpinjack
{
  class DeathScreen : public AbstractMenu
  {
    public:
      DeathScreen (SDL_Renderer * renderer);
      virtual ~DeathScreen ();

      virtual menu_action poll ();
      virtual void renderFixed (t_point point);
    private:
      int menu_y;
  };

} /* namespace jumpinjack */

#endif /* DEATH_SCREEN_H */
