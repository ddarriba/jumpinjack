/*
 * BehaviorWalker.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: diego
 */

#include "BehaviorFollow.h"

using namespace std;

namespace jumpinjack
{

  BehaviorFollow::BehaviorFollow(int speed) :
      speed(speed)
  {

  }

  BehaviorFollow::~BehaviorFollow()
  {

  }

  bool BehaviorFollow::update(t_point & next_point,
                              t_direction & direction)
  {
    if (direction == DIRECTION_RIGHT)
      next_point.x = speed;
    else
      next_point.x = -speed;

    return true;
  }
} /* namespace jumpinjack */
