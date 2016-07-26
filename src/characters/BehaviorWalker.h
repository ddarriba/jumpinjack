/*
 * BehaviorWalker.h
 *
 *  Created on: Jul 26, 2016
 *      Author: diego
 */

#ifndef BEHAVIORWALKER_H_
#define BEHAVIORWALKER_H_

#include "Behavior.h"

namespace jumpinjack
{

  class BehaviorWalker : public Behavior
  {
  public:
    BehaviorWalker(int speed = DEFAULT_SPEED);
    virtual
    ~BehaviorWalker();
    virtual bool update(SDL_Point & next_point,
                        t_direction & direction);
  protected:
    int speed;
  };

} /* namespace jumpinjack */

#endif /* BEHAVIORWALKER_H_ */
