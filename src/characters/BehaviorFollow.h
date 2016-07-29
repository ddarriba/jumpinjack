/*
 * BehaviorWalker.h
 *
 *  Created on: Jul 26, 2016
 *      Author: diego
 */

#ifndef BEHAVIORFOLLOW_H_
#define BEHAVIORFOLLOW_H_

#include "Behavior.h"

namespace jumpinjack
{

  class BehaviorFollow : public Behavior
  {
  public:
    BehaviorFollow(int speed = DEFAULT_SPEED);
    virtual
    ~BehaviorFollow();
    virtual bool update(t_point & next_point,
                        t_direction & direction);
  protected:
    int speed;
  };

} /* namespace jumpinjack */

#endif /* BEHAVIORFOLLOW_H_ */
