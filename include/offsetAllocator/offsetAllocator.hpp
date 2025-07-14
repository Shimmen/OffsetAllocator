// (C) Sebastian Aaltonen 2023
// (C) Simon Moos 2025
// MIT License (see file: LICENSE)

#pragma once

#include <array>
#include <cstdint>
#include <limits>

//#define USE_16_BIT_NODE_INDICES

namespace OffsetAllocator
{
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;

    // 16 bit offsets mode will halve the metadata storage cost
    // But it only supports up to 65536 maximum allocation count
#ifdef USE_16_BIT_NODE_INDICES
    typedef uint16 NodeIndex;
#else
    typedef uint32 NodeIndex;
#endif

    static constexpr uint32 NUM_TOP_BINS = 32;
    static constexpr uint32 BINS_PER_LEAF = 8;
    static constexpr uint32 TOP_BINS_INDEX_SHIFT = 3;
    static constexpr uint32 LEAF_BINS_INDEX_MASK = 0x7;
    static constexpr uint32 NUM_LEAF_BINS = NUM_TOP_BINS * BINS_PER_LEAF;

    struct Allocation
    {
        static constexpr uint32 NO_SPACE = std::numeric_limits<NodeIndex>::max();

        uint32 offset = NO_SPACE;
        NodeIndex metadata = NO_SPACE; // internal: node index

        bool isValid() const
        {
            return offset != NO_SPACE && metadata != NO_SPACE;
        }
    };

    struct StorageReport
    {
        uint32 totalFreeSpace;
        uint32 largestFreeRegion;
    };

    struct StorageReportFull
    {
        struct Region
        {
            uint32 size;
            uint32 count;
        };

        Region freeRegions[NUM_LEAF_BINS];
    };

    class Allocator
    {
    public:
        Allocator(uint32 size, uint32 maxAllocs = 128 * 1024);
        Allocator(Allocator &&other) noexcept;
        ~Allocator();
        void reset();

        Allocation allocate(uint32 size);
        void free(Allocation allocation);

        uint32 allocationSize(Allocation allocation) const;
        StorageReport storageReport() const;
        StorageReportFull storageReportFull() const;

    private:
        uint32 insertNodeIntoBin(uint32 size, uint32 dataOffset);
        void removeNodeFromBin(uint32 nodeIndex);

        struct Node
        {
            static constexpr NodeIndex UNUSED = std::numeric_limits<NodeIndex>::max();

            uint32 dataOffset = 0;
            uint32 dataSize = 0;
            NodeIndex binListPrev = UNUSED;
            NodeIndex binListNext = UNUSED;
            NodeIndex neighborPrev = UNUSED;
            NodeIndex neighborNext = UNUSED;
            bool used = false; // TODO: Merge as bit flag
        };

        uint32 m_size;
        uint32 m_maxAllocs;
        uint32 m_freeStorage;

        uint32 m_usedBinsTop;
        std::array<uint8, NUM_TOP_BINS> m_usedBins;
        std::array<NodeIndex, NUM_LEAF_BINS> m_binIndices;

        Node* m_nodes;
        NodeIndex* m_freeNodes;
        uint32 m_freeOffset;
    };
}
