/*
 * BehaviorWalker.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: diego
 */

#include "BehaviorWalker.h"

using namespace std;

namespace jumpinjack
{

  BehaviorWalker::BehaviorWalker(int speed) :
      speed(speed)
  {

  }

  BehaviorWalker::~BehaviorWalker()
  {

  }

  bool BehaviorWalker::update(SDL_Point & next_point,
                              t_direction & direction)
  {
    if (direction == DIRECTION_RIGHT)
      next_point.x = speed;
    else
      next_point.x = -speed;

    return true;
  }
} /* namespace jumpinjack */
