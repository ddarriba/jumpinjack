/*
 * GlobalDefs.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "GlobalDefs.h"

namespace jumpinjack
{

  t_dim GlobalDefs::window_size     = { 1024, 768 };
  int GlobalDefs::base_gravity      = 4;
  int GlobalDefs::max_falling_speed = 25;
  int GlobalDefs::max_jump_speed    = 25;
  int GlobalDefs::base_friction     = 2;
  int GlobalDefs::framerate         = 25; //FRAMERATE_DYNAMIC;

  int GlobalDefs::jump_sensitivity  = 5; //FRAMERATE_DYNAMIC;

} /* namespace sdlfw */
