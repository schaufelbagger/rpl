#ifndef UINT32TAG_H
#define UINT32TAG_H

#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include <iostream>

using namespace ns3;

// define this class in a public header
class Uint32Tag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  // these are our accessors to our tag structure
  virtual void SetSimpleValue (uint32_t value);
  uint32_t GetSimpleValue (void) const;
private:
  uint32_t m_simpleValue;
};



#endif // UINT32TAG_H