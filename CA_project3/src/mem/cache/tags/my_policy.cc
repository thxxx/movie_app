/*
 * Copyright (c) 2012-2013 ARM Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2003-2005,2014 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Erik Hallnor
 */

/**
 * @file
 * Definitions of a MY_POLICY tag store.
 */

#include "debug/CacheRepl.hh"
#include "debug/CA_DEBUG.hh"
#include "mem/cache/tags/my_policy.hh"
#include "mem/cache/base.hh"


MY_POLICY::MY_POLICY(const Params *p)
    : BaseSetAssoc(p)
{
}

CacheBlk*
MY_POLICY::accessBlock(Addr addr, bool is_secure, Cycles &lat, int master_id)
{
    CacheBlk *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat, master_id);

    if (blk != NULL) {
        // move this block to head of the MRU list
        sets[blk->set].moveToHead(blk);
        DPRINTF(CacheRepl, "set %x: moving blk %x (%s) to MRU\n",
                blk->set, regenerateBlkAddr(blk->tag, blk->set),
                is_secure ? "s" : "ns");
    
        if(blk->set==0)
            DPRINTF(CA_DEBUG, "lastAccess(accessBlock)/set:%x/tag:%x\n", blk->set, blk->tag);
    }

    
    
    return blk;
}

CacheBlk*
MY_POLICY::findVictim(Addr addr)
{
    CacheBlk *blk;
    //In here BlkType == CacheBlk
    
    
    /*
    
    WRITE_YOUR_CODE_IN_HERE;
    
    variable blk should indicate victim block (for the replacement)
    
    TIP) you need to implement
    1) Try to find invalid block.
    2) If there is an invalid block, select it as victim.
       -> Refer findVictim function of random_repl.cc
          It tries to find invalid block first, and if there is no invalid block, then it selects random block
          
    3) Otherwise, you need to select ....
       -> Check that how lru.cc works (access/insert/invalidate/findVictim)
       
       
    */
    
    
    if(blk->set==0)
    {
        if (blk->isValid())
            DPRINTF(CA_DEBUG, "victim/set:%x/tag:%x\n", blk->set, blk->tag);
        else
            DPRINTF(CA_DEBUG, "victim(Invalid)/set:%x\n", blk->set);
    }

    return blk;
}

void
MY_POLICY::insertBlock(PacketPtr pkt, BlkType *blk)
{
    BaseSetAssoc::insertBlock(pkt, blk);

    int set = extractSet(pkt->getAddr());
    sets[set].moveToHead(blk);
    
    if(blk->set==0)
        DPRINTF(CA_DEBUG, "lastAccess(insertBlock)/set:%x/tag:%x\n", blk->set, blk->tag);
}

void
MY_POLICY::invalidate(CacheBlk *blk)
{
    BaseSetAssoc::invalidate(blk);

    // should be evicted before valid blocks
    int set = blk->set;
    sets[set].moveToTail(blk);
    
    if(blk->set==0)
        DPRINTF(CA_DEBUG, "invalidBlock/set:%x\n", set);
}

MY_POLICY*
MY_POLICYParams::create()
{
    return new MY_POLICY(this);
}
