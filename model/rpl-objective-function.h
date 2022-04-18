/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2022 COPYRIGHTHOLDER
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Baranyai <e1525251@student.tuwien.ac.at>
 */
#ifndef RPL_OBJECTIVE_FUNCTION_H
#define RPL_OBJECTIVE_FUNCTION_H

#include "rpl-header.h"

#include <math.h>

namespace ns3 {
namespace rpl {


// RPL params [RFC6550, 17]
#define BASE_RANK 0
#define ROOT_RANK 1
#define INF_RANK 0xFFFF
#define DEFAULT_MIN_HOP_RANK_INCREASE 256
/**
 * \ingroup rpl
 * \brief   RPL objective function
 */
class RplObjectiveFunction
{
public:
  enum RplObjectiveFunctionType_e : uint8_t {HOP_COUNT=0, METRIC=1} RplObjectiveFunctionType_t;
  /**
   * constructor
   *
   */
  RplObjectiveFunction(RplObjectiveFunctionType_e type = RplObjectiveFunction::HOP_COUNT, double minHopRankIncrease = 1);

  uint16_t calculateRank ();

private:
  double m_minHopRankIncrease;
  RplObjectiveFunctionType_e m_type;
};




}
}

#endif // RPL_OBJECTIVE_FUNCTION_H