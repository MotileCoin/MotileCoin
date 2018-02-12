// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (0,	Params().HashGenesisBlock() )
		(1, uint256("0xe4d5915fe2d76e0dcc765e948d8158ef3abfdcff81b4ac3b8899e8d9354da390"))
		(2, uint256("0x44497fac78c7f5ad59cc7eb82ba3cf5d860ef25f3154b90febfff54e3e6ad4d2"))
		(3, uint256("0xad3bfd10867bba31b342623912f09dd4d9c8d96feb03607890dff3acb38b65b7"))
		(18, uint256("0xc6ee5107b9693cf4415b437606ec2729563bf71946f351dbd85692c0fdcc5c04"))
		(23, uint256("0xc80f55be5829c0bbe14d8efd74bb16d292947e4aff8410c748a87ead50ff773d"))
		(35, uint256("0xeccdb2d8bd28665b43e6a355d41d1aa0258ebc812a0fa40ec314fd574d80eb31"))
		(43, uint256("0xedab88ce3fe8f32f98dcc0d0d89acde606c4bdce3dde8a338e862f8ac0f7c46e"))
		(78, uint256("0xc8f691b6ea3dd0b2e606ab6719a5bb50b93bd8dd6840f4274692ba290a9a04a9"))
		(87, uint256("0xf7fc05f99667b97acb005856f7313e343c73cc0a1de725996415b1fceafdb7bf"))
		(192, uint256("0xb0e1080fe1190bad679f43dab19c148e45a5f192f1528fe58c801f2cff0dc4c2"))
		
		
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
