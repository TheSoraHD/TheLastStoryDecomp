/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/natneg/NATify.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/natneg/LICENSE.txt. */

#ifndef _NATIFY_H_
#define _NATIFY_H_

#define NATIFY_COOKIE 777
#define NATIFY_TIMEOUT 10000
#define NATIFY_STATUS_STEPS (NATIFY_TIMEOUT / 1000) + 7

typedef enum {
  packet_map1a,
  packet_map2,
  packet_map3,
  packet_map1b,
  NUM_PACKETS
} NatifyPacket;
typedef enum {
  no_nat,
  firewall_only,
  full_cone,
  restricted_cone,
  port_restricted_cone,
  symmetric,
  unknown,
  NUM_NAT_TYPES
} NatType;
typedef enum {
  promiscuous,
  not_promiscuous,
  port_promiscuous,
  ip_promiscuous,
  promiscuity_not_applicable,
  NUM_PROMISCUITY_TYPES
} NatPromiscuity;
typedef enum {
  unrecognized,
  private_as_public,
  consistent_port,
  incremental,
  mixed,
  NUM_MAPPING_SCHEMES
} NatMappingScheme;

typedef struct _AddressMapping {
  unsigned int privateIp;
  unsigned short privatePort;
  unsigned int publicIp;
  unsigned short publicPort;
} AddressMapping;

typedef struct _NAT {
  char brand[32];
  char model[32];
  char firmware[64];
  gsi_bool ipRestricted;
  gsi_bool portRestricted;
  NatPromiscuity promiscuity;
  NatType natType;
  NatMappingScheme mappingScheme;
  AddressMapping mappings[4];
  gsi_bool qr2Compatible;
} NAT;

int DiscoverReachability(SOCKET sock, unsigned int ip, unsigned short port,
                         int portType);
int DiscoverMapping(SOCKET sock, unsigned int ip, unsigned short port,
                    int portType, int id);
int NatifyThink(SOCKET sock, NAT* nat);
gsi_bool DetermineNatType(NAT* nat);
void OutputMapping(const AddressMapping* theMap);

#endif
