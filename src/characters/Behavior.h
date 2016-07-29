/*
 * Behavior.h
 *
 *  Created on: Jul 26, 2016
 *      Author: diego
 */

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "../GlobalDefs.h"

namespace jumpinjack
{

class Behavior
{
public:
  virtual ~Behavior(){}
  virtual bool update(t_point & next_point,
                      t_direction & direction) = 0;
};

} /* jumpinjack */

#endif /* BEHAVIOR_H_ */
