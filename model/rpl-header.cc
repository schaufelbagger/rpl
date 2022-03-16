
#include "rpl-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplHeader");

namespace rpl {

RplHeader::RplHeader () : Icmpv6Header()
{
  this->SetType (uint8_t (155));
  NS_LOG_FUNCTION (this);
}

TypeId RplHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::RplHeader")
    .SetParent<Icmpv6Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplHeader> ()
  ;
  return tid;
}


RplHeaderOption::RplHeaderOption (uint8_t type, uint8_t optionLength, std::vector<uint8_t> data) 
:
m_type (type),
m_optionLength (optionLength),
m_data (data)
{
}
TypeId RplHeaderOption::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::RplHeaderOption")
    .SetParent<Object> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplHeaderOption> ()
  ;
  return tid;
}

  uint8_t RplHeaderOption::GetType ()
  {
    return m_type;
  }
  uint8_t RplHeaderOption::GetOptionLength ()
  {
    return m_optionLength;
  }
  std::vector<uint8_t> RplHeaderOption::GetData()
  {
    return m_data;
  }


void RplHeaderOption::Print ()
{
  std::cout << "RplHeaderOption:" << std::endl;
  std::cout<<"type: "<< unsigned(m_type) << std::endl;
  std::cout<<"length: "<< unsigned(m_optionLength) << std::endl;
  for (auto item: m_data)
  {
    std::bitset<8> x(item);
    std::cout << x << ", " << unsigned(item) << std::endl;;
  }
}

void RplHeaderOption::SetPadN (uint8_t optionLength)
{
  m_type = 0x01;
  m_optionLength = optionLength;
  if (optionLength > 5)
  {
    NS_FATAL_ERROR ("In RPL Header Option PadN the length was set to an invalid value.");
  }
  m_data.resize (optionLength,0);
}

void RplHeaderOption::SetDagMetricContainer (uint8_t optionLength, std::vector<uint8_t> metricData)
{
  m_type = 0x02;
  m_optionLength = optionLength;
  m_data = metricData;
}
void RplHeaderOption::SetDagMetricContainer (std::vector<uint8_t> metricData)
{
  m_type = 0x02;
  m_optionLength = metricData.size ();
  m_data = metricData;
}

void RplHeaderOption::SetRouteInformation (uint8_t optionLength, uint8_t flags, uint32_t routeLifetime, Ipv6Prefix prefix)
{
  uint8_t buf[16];
  uint8_t prefixLength = prefix.GetPrefixLength ();
  m_type = 0x03;
  m_optionLength = optionLength;
  m_data.push_back(prefixLength);
  m_data.push_back(flags);
  std::memcpy (&routeLifetime, &m_data[2], sizeof(uint32_t));
  prefix.GetBytes(buf);
  for (int i = 0; i < prefixLength; i++) {
    m_data.push_back(buf[i]);
  }
}


DisHeader::DisHeader (uint8_t flags, uint8_t reserved, std::vector<RplHeaderOption> options) : RplHeader(), 
  m_flags (flags),
  m_reserved (reserved),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DIS));
}

TypeId DisHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DisHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DisHeader> ()
  ;
  return tid;
}

uint8_t DisHeader::GetFlags ()
{
  return this->m_flags;
}

void DisHeader::SetFlags (uint8_t flags)
{
  this->m_flags = flags;
}


DioHeader::DioHeader (uint8_t rplInstanceId, 
  uint8_t versionNumber,
  uint16_t rank,
  uint8_t grounded,
  uint8_t mop,
  uint8_t prf,
  uint8_t dtsn,
  uint8_t flags,
  uint8_t reserved,
  Ipv6Address dodagid, 
  std::vector<RplHeaderOption> options)
  : 
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_versionNumber (versionNumber),
  m_rank (rank),
  m_grounded (grounded),
  m_mop (mop),
  m_prf (prf),
  m_dtsn (dtsn),
  m_flags (flags),
  m_reserved (reserved),
  m_dodagid (dodagid),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DIO));
}

TypeId DioHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DioHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DioHeader> ()
  ;
  return tid;
}

DaoHeader::DaoHeader (uint8_t rplInstanceId, 
  uint8_t k,
  uint8_t d,
  uint8_t flags,
  uint8_t reserved,
  uint8_t daoSequence,
  Ipv6Address dodagid,
  std::vector<RplHeaderOption> options)
  :
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
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

TypeId DaoHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DaoHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DaoHeader> ()
  ;
  return tid;
}

DaoAckHeader::DaoAckHeader (uint8_t rplInstanceId, 
  uint8_t d,
  uint8_t reserved,
  uint8_t daoSequence,
  uint8_t status,
  Ipv6Address dodagid,
  std::vector<RplHeaderOption> options)
  :
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_d (d),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_status (status),
  m_dodagid (dodagid),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DAO_ACK));
}

TypeId DaoAckHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DaoAckHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DaoAckHeader> ()
  ;
  return tid;
}

CcHeader::CcHeader (uint8_t rplInstanceId, 
  uint8_t r,
  uint8_t flags,
  uint16_t ccNonce,
  Ipv6Address dodagid,
  uint32_t destinationCounter,
  std::vector<RplHeaderOption> options)
  :
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_r (r),
  m_flags (flags),
  m_ccNonce (ccNonce),
  m_dodagid (dodagid),
  m_destinationCounter (destinationCounter),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_CONS_CHECK));
}

TypeId CcHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::CcHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<CcHeader> ()
  ;
  return tid;
}


}
}