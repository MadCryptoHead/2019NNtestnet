/******************************************************************************
 * Copyright © 2014-2016 The SuperNET Developers.                             *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#ifndef H_BASILISK_H
#define H_BASILISK_H

#include "../iguana/iguana777.h"

#define BASILISK_TIMEOUT 30000
#define BASILISK_MINFANOUT 8
#define BASILISK_MAXFANOUT 64
#define BASILISK_DEFAULTDIFF 0x1effffff
#define BASILISK_MAXRELAYS 64
#define BASILISK_DEXDURATION 60

#define BASILISK_MAXFUTUREBLOCK 60
//#define BASILISK_MAXBLOCKLAG 600
#define BASILISK_HDROFFSET ((int32_t)(sizeof(bits256)+sizeof(struct iguana_msghdr)+sizeof(uint32_t)))

struct basilisk_value { bits256 txid; int64_t value; int32_t height; int16_t vout; char coinaddr[64]; };

struct basilisk_item
{
    struct queueitem DL; UT_hash_handle hh;
    double expiration; cJSON *retarray;
    uint32_t submit,finished,basilisktag,numresults,numsent,numrequired,nBits;
    char symbol[32],CMD[4],remoteaddr[64],*retstr;
};

struct basilisk_info
{
    //queue_t resultsQ,submitQ;
    void *launched; //portable_mutex_t *mutex;
    struct basilisk_item *issued;
    struct basilisk_value values[8192]; int32_t numvalues;
};

struct basilisk_request
{
    uint32_t crc,timestamp,requestid,quoteid;
    uint64_t srcamount,destamount;
    bits256 hash; 
    char src[8],dest[8],message[48];
    uint32_t relaybits;
} __attribute__((packed));

struct basilisk_relaystatus
{
    uint8_t pingdelay;
};

struct basilisk_relay
{
    bits256 pubkey; int32_t relayid,oldrelayid; uint32_t ipbits,lastping; uint8_t pubkey33[33];
    struct basilisk_request *requests; int32_t maxrequests,numrequests;
    struct basilisk_relaystatus direct,reported[BASILISK_MAXRELAYS];
};

void basilisk_msgprocess(struct supernet_info *myinfo,void *addr,uint32_t senderipbits,char *type,uint32_t basilisktag,uint8_t *data,int32_t datalen);
int32_t basilisk_sendcmd(struct supernet_info *myinfo,char *destipaddr,char *type,uint32_t *basilisktagp,int32_t encryptflag,int32_t delaymillis,uint8_t *data,int32_t datalen,int32_t fanout,uint32_t nBits); // data must be offset by sizeof(iguana_msghdr)+sizeof(basilisktag)

void basilisks_init(struct supernet_info *myinfo);
void basilisk_p2p(void *myinfo,void *_addr,char *ipaddr,uint8_t *data,int32_t datalen,char *type,int32_t encrypted);
uint8_t *basilisk_jsondata(int32_t extraoffset,uint8_t **ptrp,uint8_t *space,int32_t spacesize,int32_t *datalenp,char *symbol,cJSON *sendjson,uint32_t basilisktag);

uint8_t *SuperNET_ciphercalc(void **ptrp,int32_t *cipherlenp,bits256 *privkeyp,bits256 *destpubkeyp,uint8_t *data,int32_t datalen,uint8_t *space2,int32_t space2size);
void *SuperNET_deciphercalc(void **ptrp,int32_t *msglenp,bits256 privkey,bits256 srcpubkey,uint8_t *cipher,int32_t cipherlen,uint8_t *buf,int32_t bufsize);
uint8_t *get_dataptr(int32_t hdroffset,uint8_t **ptrp,int32_t *datalenp,uint8_t *space,int32_t spacesize,char *hexstr);
char *basilisk_addhexstr(char **ptrp,cJSON *valsobj,char *strbuf,int32_t strsize,uint8_t *data,int32_t datalen);
char *basilisk_standardservice(char *CMD,struct supernet_info *myinfo,void *_addr,bits256 hash,cJSON *valsobj,char *hexstr,int32_t blockflag); // client side
char *basilisk_respond_mempool(struct supernet_info *myinfo,char *CMD,void *_addr,char *remoteaddr,uint32_t basilisktag,cJSON *valsobj,uint8_t *data,int32_t datalen,bits256 hash,int32_t from_basilisk);
char *basilisk_addrelay_info(struct supernet_info *myinfo,uint8_t *pubkey33,uint32_t ipbits,bits256 pubkey);

void basilisk_request_goodbye(struct supernet_info *myinfo);
int32_t basilisk_update(char *symbol,uint32_t reftimestamp);
void basilisk_seqresult(struct supernet_info *myinfo,char *retstr);
struct iguana_info *basilisk_geckochain(struct supernet_info *myinfo,char *symbol,char *chainname,cJSON *valsobj);

#endif
