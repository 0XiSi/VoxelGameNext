#include "WorldSerializer.hpp"
#include "Voxel/ChunkManager.hpp"
#include "WorldSerializer.hpp"
// already implemented inside ChunkManager::saveAll / loadAll
void WorldSerializer::save(const ChunkManager& cm, const std::string& folder) {
	const_cast<ChunkManager&>(cm).saveAll(folder);
}
void WorldSerializer::load(ChunkManager& cm, const std::string& folder) {
	cm.loadAll(folder);
}