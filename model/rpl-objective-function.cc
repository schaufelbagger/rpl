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
#include "rpl-objective-function.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplObjectiveFunction");

namespace rpl {


RplObjectiveFunction::RplObjectiveFunction(RplObjectiveCodePoint_e type, uint16_t minHopRankIncrease, uint16_t maxRankIncrease) 
:
m_type (type),
m_minHopRankIncrease (minHopRankIncrease),
m_maxRankIncrease (maxRankIncrease)
{
}

RplNode RplObjectiveFunction::GetPreferredParent (std::set<RplNode> parents, RplNode currentPreferredParent)
{
  NS_ABORT_MSG_IF (parents.empty (), "No parent in set while trying to find the preferred parent");
  RplNode preferredParent = currentPreferredParent;
  uint16_t rankWithPrefParent = CalculateRank (currentPreferredParent.rank);

  for(RplNode node : parents) 
  {
    uint16_t rankWithNodeParent = CalculateRank (node.rank);
    if (std::tie(rankWithNodeParent, node.interface) < std::tie(rankWithPrefParent, preferredParent.interface))
    {
      preferredParent = node;
      rankWithPrefParent = CalculateRank (preferredParent.rank);
    }
  }
  return preferredParent;
}


void RplObjectiveFunction::ProcessingDio (RplObjectiveCodePoint_e type, uint16_t minHopRankIncrease, uint16_t maxRankIncrease)
{
  m_type = type;
  m_minHopRankIncrease = minHopRankIncrease;
  m_maxRankIncrease = maxRankIncrease;
  CalculateRankIncrease ();
}

void RplObjectiveFunction::CalculateRankIncrease (uint16_t minHopRankIncrease, uint16_t maxRankIncrease)
{
  m_rankIncrease = (m_rankFactor*m_stepOfRank + m_stretchOfRank) * m_minHopRankIncrease;
  if(maxRankIncrease > 0 && m_rankIncrease > maxRankIncrease)
  {
    m_rankIncrease = maxRankIncrease;
  }
}
void RplObjectiveFunction::CalculateRankIncrease ()
{
  CalculateRankIncrease (m_minHopRankIncrease, m_maxRankIncrease);
}

uint16_t RplObjectiveFunction::CalculateRank (uint16_t preferredParentRank)
{
  // use ETX
  uint16_t rank;
  rank = preferredParentRank + m_rankIncrease;

  // check for overflow
  if (preferredParentRank > rank)
  {
    return INFINITE_RANK;
  }

  return rank;
}
uint16_t RplObjectiveFunction::DagRank (uint16_t rank)
{
  return floor (rank/m_minHopRankIncrease);
}



}
}