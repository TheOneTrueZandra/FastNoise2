#include <FastNoise/SmartNode.h>

#include <shared_mutex>
#include <unordered_map>
#include <cassert>

namespace FastNoise
{
    static std::shared_mutex gMutex;
    static std::unordered_map<size_t, size_t> gReferenceMap;
    static size_t gReferenceIdCounter = 0;

    size_t SmartNodeManager::NewReference()
    {
        std::unique_lock lock( gMutex );

        gReferenceMap.emplace( gReferenceIdCounter, 1 );

        return gReferenceIdCounter++;
    }

    void SmartNodeManager::IncReference( size_t id )
    {
        if( id != InvalidReferenceId )
        {
            std::unique_lock lock( gMutex );

            ++gReferenceMap.at( id );
        }
    }

    bool SmartNodeManager::DecReference( size_t id )
    {
        if( id != InvalidReferenceId )
        {
            std::unique_lock lock( gMutex );

            if( gReferenceMap.at( id ) == 1 )
            {
                gReferenceMap.erase( id );
                return true;
            }

            --gReferenceMap.at( id );
        }
        return false;
    }

} // namespace FastNoise