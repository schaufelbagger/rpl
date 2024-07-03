/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2024 Alexander Baranyai
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
 * Author: Alexander Baranyai <alexander.baranyai@gmx.net>
 */
#ifndef RPL_OBJECTIVE_FUNCTION_H
#define RPL_OBJECTIVE_FUNCTION_H

#include "rpl-header.h"
#include "rpl-node.h"

#include <math.h>

namespace ns3 {
namespace rpl {


// RPL params [RFC6550, 17]
#define DEFAULT_MIN_HOP_RANK_INCREASE 256
#define BASE_RANK 0
#define ROOT_RANK DEFAULT_MIN_HOP_RANK_INCREASE



/// OF0 defines
#define DEFAULT_STEP_OF_RANK 1
#define MINIMUM_STEP_OF_RANK 1
#define MAXIMUM_STEP_OF_RANK 9
#define DEFAULT_RANK_STRETCH 0
#define MAXIMUM_RANK_STRETCH 5  // not used
#define DEFAULT_RANK_FACTOR 1
#define MINIMUM_RANK_FACTOR 1
#define MAXIMUM_RANK_FACTOR 4

enum RplObjectiveCodePoint_e : uint16_t {OF0=0, MRHOF=1};

/**
 * \ingroup rpl
 * \brief   RPL objective function
 */
class RplObjectiveFunction
{
public:

  /**
   * constructor
   *
   */
  RplObjectiveFunction(RplObjectiveCodePoint_e type = OF0, uint16_t minHopRankIncrease = 1, uint16_t maxRankIncrease = 0);


  RplObjectiveCodePoint_e GetOcp () const
  {
    return m_type;
  }

  RplNode GetPreferredParent (std::set<RplNode> parents, RplNode currentPreferredParent);
  void ProcessingDio (RplObjectiveCodePoint_e type, uint16_t minHopRankIncrease, uint16_t maxRankIncrease);
  void ProvideDagInformation ();
  void TriggeredUpdates ();
  void CalculateRankIncrease (uint16_t minHopRankIncrease, uint16_t maxRankIncrease);
  void CalculateRankIncrease ();
  uint16_t CalculateRank (uint16_t preferredParentRank);
  uint16_t DagRank(uint16_t rank);

private:
  RplObjectiveCodePoint_e m_type;
  uint16_t m_minHopRankIncrease;
  uint16_t m_maxRankIncrease;
  /// OF0 operands
  unsigned int m_rankIncrease;
  unsigned int m_stepOfRank = DEFAULT_STEP_OF_RANK;
  unsigned int m_stretchOfRank = DEFAULT_RANK_STRETCH;
  unsigned int m_rankFactor = DEFAULT_RANK_FACTOR;
};




}
}

#endif // RPL_OBJECTIVE_FUNCTION_H