
#include "rpl-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplHeader");

namespace rpl {

RplHeader::RplHeader () : Icmpv6Header()
{
  this->SetType (uint8_t (155));
  NS_LOG_FUNCTION (this);
}

RplHeaderOptions::RplHeaderOptions (uint8_t type, uint8_t length, std::vector<uint8_t> data) 
:
m_type (type),
m_length (length),
m_data (data)
{

}

DisHeader::DisHeader (uint8_t flags, uint8_t reserved, RplHeaderOptions options) : RplHeader(), 
  m_flags (flags),
  m_reserved (reserved),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DIS));
}

uint8_t DisHeader::GetFlags ()
{
  return this->m_flags;
}

void DisHeader::SetFlags (uint8_t flags)
{
  this->m_flags = flags;
}


DioHeader::DioHeader (Ipv6Address dodagid, 
  uint8_t instanceId, 
  uint8_t versionNumber,
  uint16_t rank,
  uint8_t grounded,
  uint8_t mop,
  uint8_t prf,
  uint8_t dtsn,
  uint8_t flags,
  uint8_t reserved,
  RplHeaderOptions options)
  : 
  RplHeader(),
  m_instanceId (instanceId),
  m_versionNumber (versionNumber),
  m_rank (rank),
  m_grounded (grounded),
  m_mop (mop),
  m_prf (prf),
  m_dtsn (dtsn),
  m_flags (flags),
  m_reserved (reserved),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DIO));
}

  DaoHeader::DaoHeader (uint8_t instanceId, 
  uint8_t k,
  uint8_t d,
  uint8_t flags,
  uint8_t reserved,
  uint8_t daoSequence,
  Ipv6Address dodagid,
  RplHeaderOptions options)
  :
  RplHeader(),
  m_instanceId (instanceId),
  m_k (k),
  m_d (d),
  m_flags (flags),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_dodagid (dodagid),
  m_options (options)
  {
    this->SetCode (uint8_t (RplHeader::TYPE_DAO));
  }


DaoAckHeader::DaoAckHeader (uint8_t instanceId, 
  uint8_t d,
  uint8_t reserved,
  uint8_t daoSequence,
  uint8_t status,
  Ipv6Address dodagid,
  RplHeaderOptions options)
  :
  RplHeader(),
  m_instanceId (instanceId),
  m_d (d),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_status (status),
  m_dodagid (dodagid),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DAO_ACK));
}

CcHeader::CcHeader (uint8_t instanceId, 
  uint8_t r,
  uint8_t flags,
  uint16_t ccNonce,
  Ipv6Address dodagid,
  uint32_t destinationCounter,
  RplHeaderOptions options)
  :
  RplHeader(),
  m_instanceId (instanceId),
  m_r (r),
  m_flags (flags),
  m_ccNonce (ccNonce),
  m_dodagid (dodagid),
  m_destinationCounter (destinationCounter),
  m_options (options)
  {
    this->SetCode (uint8_t (RplHeader::TYPE_CONS_CHECK));
  }


}
}