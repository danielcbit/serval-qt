/* 
Serval Distributed Numbering Architecture (DNA)
Copyright (C) 2010 Paul Gardner-Stephen 

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "serval.h"

int process_packet(unsigned char *packet, size_t len,
		   int recvttl,struct sockaddr *sender, size_t sender_len)
{
  //int authenticatedP=0;
  char did[128];
  char sid[128];
  unsigned char *transaction_id=&packet[OFS_TRANSIDFIELD];
  
  did[0]=0; sid[0]=0;

  /* Get DID or SID */
  if (packetGetID(packet,len,did,sid) == -1)
    return WHY("Could not parse DID or SID");
  
  /* Check for PIN */
  if (!isFieldZeroP(packet,OFS_PINFIELD,16))
    {
      /* Authentication has been attempted.
	 If it is incorrect, then we need to return with ACTION_DECLINED
      */
      if (debug&DEBUG_SECURITY) DEBUG("A PIN has been supplied");
      
      /* Can only authenticate by SID, not DID (since DIDs are ambiguous) */
      if (packet[OFS_SIDDIDFIELD]!=1) return WHY("You can only authenticate against a SID");
   
      /* XXX check authentication */
      return WHY("Authentication not yet supported");
    }
  else 
    {
      /* No attempt at authentication was made */
      //authenticatedP=0;
      if (debug&DEBUG_SECURITY) DEBUG("No PIN was supplied");
    }

  if (serverMode) return processRequest(packet,len,sender,sender_len,transaction_id,
					recvttl,did,sid);
   
  return 0;
}

int packetOk(struct overlay_interface *interface, unsigned char *packet, size_t len,
	     unsigned char *transaction_id,int ttl,
	     struct sockaddr *recvaddr, size_t recvaddrlen,int parseP)
{
  if (len<HEADERFIELDS_LEN) return WHY("Packet is too short");

  if (packet[0]==0x41&&packet[1]==0x10) 
    {
      return packetOkDNA(packet,len,transaction_id,ttl,recvaddr,recvaddrlen,parseP);
    }

  if (packet[0]==0x4F&&packet[1]==0x10) 
    {
      if (interface!=NULL)
	{
	  return packetOkOverlay(interface,packet,len,transaction_id,ttl,
				 recvaddr,recvaddrlen,parseP);
	}
      else
	/* We ignore overlay mesh packets in simple server mode, which is indicated by interface==-1 */
	return WHY("Ignoring overlay mesh packet");
    }

  return WHY("Packet type not recognised.");
}

int packetOkDNA(unsigned char *packet,int len,unsigned char *transaction_id,
		int recvttl,
		struct sockaddr *recvaddr, size_t recvaddrlen, int parseP)
{
  /* Make sure that the packet is meant for us, and is not mal-formed */
  int version;
  int cipher;
  int length;
  int payloadRotation;

  version=(packet[2]<<8)|packet[3];
  length=(packet[4]<<8)|packet[5];
  cipher=(packet[6]<<8)|packet[7];
  if (version!=1) return WHY("Unknown packet format version");
  if (cipher!=0) return WHY("Unknown packet cipher");
  if (length!=len) return WHY("Packet length incorrect");

  if (cipher) 
	  if (packetDecipher(packet,len,cipher)) 
		  return WHY("Could not decipher packet");

  /* Make sure the transaction ID matches */
  if (transaction_id)
    {
      int i;
	  for(i=0;i<TRANSID_SIZE;i++)
		if (packet[OFS_TRANSIDFIELD+i]!=transaction_id[i])
		  return WHY("transaction ID mismatch");
    }
  
  /* Unrotate the payload */
  payloadRotation=packet[OFS_ROTATIONFIELD];
  {
    unsigned char temp[256];
    bcopy(&packet[len-payloadRotation],&temp[0],payloadRotation);
    bcopy(&packet[HEADERFIELDS_LEN],&packet[HEADERFIELDS_LEN+payloadRotation],
	  len-(HEADERFIELDS_LEN)-payloadRotation);
    bcopy(&temp[0],&packet[HEADERFIELDS_LEN],payloadRotation);
  }

  if (debug&DEBUG_PACKETFORMATS) {
    DEBUG("Packet passes sanity checks and is ready for decoding");
    dump("unrotated packet",packet,len);
  }

  if (parseP) return process_packet(packet,len,recvttl,recvaddr,recvaddrlen); else return 0;
}

int packetMakeHeader(unsigned char *packet,int packet_maxlen,int *packet_len,
		     unsigned char *transaction_id,int cryptoflags)
{
  int i;

  CHECK_PACKET_LEN(OFS_PAYLOAD);

  /* 0x4110 magic value */
  packet[0]=0x41;
  packet[1]=0x10;
  
  /* encoding version */
  packet[2]=0x00;
  packet[3]=0x01;
  
  /* Payload length (to be filled in later) */
  packet[4]=0x00;
  packet[5]=0x00;
  
  /* Payload cipher (0x0000 = plain text) */
  packet[6]=0x00;
  packet[7]=0x00;

  /* Add 64bit transaction id */
  if (transaction_id)
    /* Use supplied transaction ID */
    for(i=0;i<TRANSID_SIZE;i++) packet[OFS_TRANSIDFIELD+i]=transaction_id[i];
  else
    /* No transaction ID supplied, so create random transaction ID */
    for(i=0;i<TRANSID_SIZE;i++) packet[OFS_TRANSIDFIELD+i]=random()&0xff;

  /* payload rotation (not yet applied) */
  packet[OFS_ROTATIONFIELD]=0x00;

  *packet_len=HEADERFIELDS_LEN;

  /* Clear did/subscriber ID, salt and hashed pin fields.
     However, we cannot zero them, because that would provide significant knowable plain-text
     for a known plain text attack.
     Thus, instead we fill it with random date, but make the modulo sum of each field == 0x00
     to indicate that no PIN has been provided. */
  safeZeroField(packet,*packet_len,SIDDIDFIELD_LEN); *packet_len+=SIDDIDFIELD_LEN;
  safeZeroField(packet,*packet_len,16); *packet_len+=16;
  safeZeroField(packet,*packet_len,16); *packet_len+=16;

  return 0;
}

int packetSetDid(unsigned char *packet,int packet_maxlen,int *packet_len,char *did)
{
  /* Set the subject field to the supplied DID.
     DIDs get encoded 4bits per digit (0-9,#,*,+,SPARE1,ESCAPE,END)
  */
  int ofs=OFS_SIDDIDFIELD; /* where the DID/subscriber ID gets written */

  /* Put DID (ie not SID) marker into packet */
  packet[ofs++]=0x00;

  return stowDid(packet,&ofs,did);
}

int packetSetSidFromId(unsigned char *packet,int packet_maxlen,int *packet_len,
		       keyring_identity *id)
{
  if (!id) return WHY("id is null");

  unsigned char *sid=NULL;
  int i;

  for(i=0;i<id->keypair_count;i++)
    if (id->keypairs[i]->type==KEYTYPE_CRYPTOBOX)
      { sid=id->keypairs[i]->public_key; break; }

  if (!sid) return WHY("Could not find SID in identity");

  /* find and copy SID from identity */
  int ofs=OFS_SIDDIDFIELD;
  packet[ofs++]=0x01; /* SID */
  bcopy(sid,&packet[ofs],SID_SIZE);
  return 0;
}


int packetFinalise(unsigned char *packet,int packet_maxlen,int recvttl,
		   int *packet_len,int cryptoflags)
{
  /* Add any padding bytes and EOT to packet */
  int paddingBytes=rand()&0xf;
  int payloadRotation;

  if (paddingBytes)
    {
      CHECK_PACKET_LEN(2+paddingBytes);
      packet[(*packet_len)++]=ACTION_PAD;
      packet[(*packet_len)++]=paddingBytes;
      while(paddingBytes--) packet[(*packet_len)++]=random()&0xff;
    }

  /* tell requester what the ttl was when we received the packet */
  if (recvttl>-1) {
    CHECK_PACKET_LEN(2);
    packet[(*packet_len)++]=ACTION_RECVTTL;
    packet[(*packet_len)++]=recvttl;
  }

  /* mark end of packet */
  CHECK_PACKET_LEN(1);
  packet[(*packet_len)++]=ACTION_EOT;

  /* Set payload length */
  packet[4]=((*packet_len)>>8)&0xff;
  packet[5]=((*packet_len)&0xff);

  /* Work out by how much to rotate the packet payload.
     The purpose of the rotation is to make it more difficult to
     conduct a known-plaintext attack against any ciphers that we 
     may later support.
  */
  payloadRotation=(*packet_len)-HEADERFIELDS_LEN;
  if (payloadRotation>0xff) payloadRotation=0xff;
  payloadRotation=random()%payloadRotation;
  if (debug&DEBUG_SECURITY) {
    DEBUGF("Known Plaintext counter-measure: rotating packet payload by 0x%02x bytes",
	    payloadRotation);
    dump("unrotated packet",packet,*packet_len);
  }

  /* Now rotate the payload */
  {
    unsigned char temp[256];

    /*Copy first part of payload to a temporary buffer */
    bcopy(&packet[HEADERFIELDS_LEN],&temp[0],payloadRotation);
    /* Copy the main part of the payload left by the rotation factor */
    bcopy(&packet[HEADERFIELDS_LEN+payloadRotation],&packet[HEADERFIELDS_LEN],
	  (*packet_len)-(HEADERFIELDS_LEN)-payloadRotation);
    /* Copy the temporary buffer to the end of the packet to complete the rotation */
    bcopy(&temp[0],&packet[(*packet_len)-payloadRotation],payloadRotation);
  }
  packet[OFS_ROTATIONFIELD]=payloadRotation;
  if (debug&DEBUG_SECURITY) dump("rotated packet",packet,*packet_len);

  if (cryptoflags) return packetEncipher(packet,packet_maxlen,packet_len,cryptoflags);

  return 0;
}

int extractRequest(unsigned char *packet,int *packet_ofs,int packet_len,
		   int *itemId,int *instance,unsigned char *value,
		   int *start_offset,int *bytes,int *flags)
{
  if (*packet_ofs<0||(*packet_ofs)+6>=packet_len) 
    return WHY("mal-formed request packet (packet too short/bad offset)");

  *itemId=packet[(*packet_ofs)++];

  if ((*itemId)&0x80) *instance=packet[(*packet_ofs)++]; else *instance=0;
  if (*instance==0xff) *instance=-1;

  *start_offset=packet[(*packet_ofs)++]<<8;
  *start_offset|=packet[(*packet_ofs)++];

  *bytes=packet[(*packet_ofs)++]<<8;
  *bytes|=packet[(*packet_ofs)++];

  *flags=packet[(*packet_ofs)++];
  if (debug&DEBUG_PACKETFORMATS) DEBUGF("Write flags = 0x%02x",*flags);

  if (*packet_ofs<0||(*packet_ofs)+(*bytes)>=packet_len)
    {
      if (debug&DEBUG_PACKETFORMATS) DEBUGF("Packet offset is %d, length is %d, and asked for %d bytes",*packet_ofs,packet_len,*bytes);
      return WHY("mal-formed request packet (too short for claimed data)");
    }

  bcopy(&packet[*packet_ofs],value,*bytes);
  (*packet_ofs)+=*bytes;

  return 0;
}


int extractResponses(struct in_addr sender,unsigned char *buffer,int len,struct response_set *responses)
{
  int ofs=OFS_PAYLOAD;
  
  struct response *first_response=NULL;

  while(ofs<len)
    {
      /* XXX should allocate responses from a temporary and bounded slab of memory */
      struct response *r=calloc(sizeof(struct response),1);
      if (!r) exit(WHY("calloc() failed."));
      
      r->code=buffer[ofs];
      r->sender=sender;
      /* XXX doesn't make sure it is SID instead of DID */
      bcopy(&buffer[HEADERFIELDS_LEN+1],r->sid,SID_SIZE);

      switch(buffer[ofs])
	{
	case ACTION_EOT:
	  if (debug&DEBUG_DNARESPONSES) DEBUGF("Reached response packet EOT");
	case ACTION_DECLINED: case ACTION_OKAY:
	case ACTION_CREATEHLR:
	  r->response_len=0; break;
	case ACTION_GET: 
	  /* Followed by variable # to fetch.
	     XXX If variable number >=0x80 then get instance information */
	  r->response_len=1; break;
	case ACTION_ERROR:
	  r->response_len=buffer[++ofs];
	  break;
	case ACTION_DATA:
	  /* Extract variable value */
	  unpackageVariableSegment(&buffer[ofs+1],len-ofs,WITHDATA,r);
	  break;
	case ACTION_DONE:
	  r->value_offset=buffer[ofs+1];
	  r->response_len=1;
	  break;
	case ACTION_PAD:
	  /* Skip padding bytes */
	  r->response_len=1+buffer[ofs+1];	 
	  break;
	case ACTION_WROTE:
	  /* Extract info about the variable segment that was written.
	     This uses the same format as the request to write it, but without the data */
	  unpackageVariableSegment(&buffer[ofs+1],len-ofs,WITHOUTDATA,r);
	  r->response=NULL;
	  break;
	case ACTION_RECVTTL:
	  r->recvttl=buffer[ofs+1];
	  r->response_len=1;
	  /* Attach TTL to other responses from this packet */
	  {
	    struct response *rr=first_response;
	    while(rr) {
	      rr->recvttl=r->recvttl;
	      rr=rr->next;
	    }
	  }
	  break;
	case ACTION_SET:
	case ACTION_DEL:
	case ACTION_XFER:
	default:
	  free(r);
	  if (debug&(DEBUG_DNARESPONSES|DEBUG_PACKETFORMATS)) DEBUGF("Encountered unimplemented response code 0x%02x @ 0x%x",buffer[ofs],ofs);
	  fixResponses(responses);
	  return WHY("Encountered unimplemented response type");
	}
      ofs++;
      if (r->response_len) {
	/* extract bytes of response */
	unsigned char *rr;
	if (r->response) rr=r->response; else rr=&buffer[ofs];
	r->response=malloc(r->response_len+1);
	if (!r->response) exit(WHY("malloc() failed."));
	bcopy(&rr[0],r->response,r->response_len);
	r->response[r->response_len]=0;
	ofs+=r->response_len;
      }

      /* Work out peer ID */
      r->sender=sender;
      for(r->peer_id=0;r->peer_id<peer_count;r->peer_id++)
	{
	  if (sender.s_addr==peers[r->peer_id].s_addr) break;
	}
      if (r->peer_id>peer_count) r->peer_id=-1;

      /* Link new response into chain */
      if (debug&DEBUG_DNARESPONSES) DEBUGF("Linking response into response set");
      r->prev=responses->last_response;
      if (responses->last_response)
	responses->last_response->next=r;
      else
	responses->responses=r;
      responses->last_response=r;
      responses->response_count++;

      if (!first_response) first_response=r;

      responseFromPeer(responses,r->peer_id);

      if (debug&DEBUG_DNARESPONSES) dumpResponses(responses);
    }
  
  fixResponses(responses);
  return 0;
}

int packageVariableSegment(unsigned char *data,int *dlen,
			   struct response *h,
			   int offset,int buffer_size)
{
  int bytes;
  int dlen_in=*dlen;

  if ((buffer_size-(*dlen))<8) return WHY("Insufficient buffer space for packageVariableSegment()");

  /* Figure out how many bytes we need to package */
  bytes=buffer_size-(*dlen)-8;
  if ((h->value_len-offset)<bytes) bytes=h->value_len-offset;
  if (bytes<0) bytes=0;
  if (debug&DEBUG_PACKETFORMATS) DEBUGF("Packaging %d bytes of variable",bytes);

  /* Describe variable */

  /* Variable id and instance # (if required) */
  data[(*dlen)++]=h->var_id;
  if (h->var_id&0x80) data[(*dlen)++]=h->var_instance;

  /* Variable length */
  data[(*dlen)++]=h->value_len>>8;
  data[(*dlen)++]=h->value_len&0xff;

  /* Start offset in this segment */
  data[(*dlen)++]=(offset>>8)&0xff;
  data[(*dlen)++]=offset&0xff;

  /* Number of bytes in this segment */
  data[(*dlen)++]=(bytes>>8)&0xff;
  data[(*dlen)++]=bytes&0xff;
  if (debug&DEBUG_PACKETFORMATS) DEBUGF("Packaging %d bytes",bytes);

  /* Package the variable value itself (or part thereof) */
  bcopy(&h->response[offset],&data[*dlen],bytes);
  (*dlen)+=bytes;

  if (debug&DEBUG_PACKETFORMATS) dump("Variable segment octets",&data[dlen_in],(*dlen)-dlen_in);

  return 0;
}

int unpackageVariableSegment(unsigned char *data,int dlen,int flags,struct response *r)
{
  r->response_len=0;
  if (dlen<7) return WHY("unpackageVariableSegment() fed insufficient data");
  
  r->var_id=data[r->response_len++];
  if (r->var_id&0x80) r->var_instance=data[r->response_len++]; else r->var_instance=0;
  if (r->var_instance==0xff) r->var_instance=-1;

  r->value_len=data[r->response_len++]<<8;
  r->value_len|=data[r->response_len++];

  r->value_offset=data[r->response_len++]<<8;
  r->value_offset|=data[r->response_len++];

  r->value_bytes=data[r->response_len++]<<8;
  r->value_bytes|=data[r->response_len++];

  r->response=&data[r->response_len];

  r->response_len+=r->value_bytes;

  if (flags!=WITHOUTDATA)
    if (r->response_len>dlen) 
      return WHY("unpackageVariableSegment() fed insufficient or corrupt data");
  
  return 0;
}
