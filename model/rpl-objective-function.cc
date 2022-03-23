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


RplObjectiveFunction::RplObjectiveFunction(RplObjectiveFunctionType_e type, double minHopRankIncrease) 
:
m_minHopRankIncrease (minHopRankIncrease),
m_type (type)
{

}


uint16_t RplObjectiveFunction::calculateRank ()
{
  // TODO change
  uint16_t rank = 1;
  return floor(rank/m_minHopRankIncrease);
}


}
}