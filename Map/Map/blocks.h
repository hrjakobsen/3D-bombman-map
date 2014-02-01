#define BLOCK_AIR 0
#define BLOCK_WALL 1
#define BLOCK_CRATE 2
#define BLOCK_XFIRE 3
#define BLOCK_ZFIRE 4
#define BLOCK_CFIRE 5
#define BLOCK_BONUS_SPEED 101
#define BLOCK_BONUS_LIFE 102
#define BLOCK_BONUS_POWER 103
#define BLOCK_BONUS_MOREBOMBS 104


bool SolidBlock(int BlockID) {
	if (BlockID == BLOCK_AIR || BlockID == BLOCK_XFIRE || BlockID == BLOCK_ZFIRE || BlockID == BLOCK_CFIRE || BlockID == BLOCK_BONUS_SPEED || BlockID == BLOCK_BONUS_LIFE || BlockID == BLOCK_BONUS_POWER || BlockID == BLOCK_BONUS_MOREBOMBS) {
		return false;
	} else {
		return true;
	}
} 

bool FireBlock(int BlockID) {
	if (BlockID == BLOCK_XFIRE || BlockID == BLOCK_ZFIRE || BlockID == BLOCK_CFIRE) {
		return true;
	} else {
		return false;
	}
}