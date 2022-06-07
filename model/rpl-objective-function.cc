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

RplNode RplObjectiveFunction::GetPreferredParent (std::set<RplNode> parents)
{
  NS_ABORT_MSG_IF (parents.empty (), "No parent in set while trying to find the preferred parent");
  RplNode preferredParent;
  // take the first parent
  for (const RplNode iter_parent : parents)
  {
    preferredParent = iter_parent;
    break;
  }
  for(RplNode node : parents) 
  {
    if (std::tie(node.rank, node.interface) < std::tie(preferredParent.rank, preferredParent.interface))
    {
      preferredParent = node;
    }
  }
  return preferredParent;
}
std::set<RplNode> RplObjectiveFunction::GetDaoParents (std::set<RplNode> parents)
{
  std::set<RplNode> daoParents;
  daoParents.insert (GetPreferredParent (parents));
  return daoParents;
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
  // TODO change, use ETX
  uint16_t rank;
  rank = preferredParentRank + m_rankIncrease;

  return rank;
}
uint16_t RplObjectiveFunction::DagRank (uint16_t rank)
{
  return floor (rank/m_minHopRankIncrease);
}



}
}