#include "Module.h"
#include "LevelSection.h"
#include <queue>

enum ASTAR_HEURISTICS {
	MANHATTAN = 0,
	EUCLIDEAN,
	SQUARED
};

enum PATH_STATUS
{
	SEARCHING,
	FOUND,
	NO_PATH,
};

struct PathData
{
	std::vector<Vector2D> pathTiles;
	std::vector<Vector2D> visitedTiles;
	std::priority_queue<std::pair<int, Vector2D>, std::vector<std::pair<int, Vector2D>>, std::greater<std::pair<int, Vector2D>> > frontierAStar;
};

class PathfindingSystem : public Module
{
	friend class Pathfinder;
private:
	SDL_Texture* pathTex;
	PATH_STATUS path_status = SEARCHING;
	std::vector<int> mapTiles;
	int mapWidth;
	int mapHeight;
	std::vector<int> blockedTiles;
	Vector2D target;
	std::vector<Vector2D> breadcrumbs;
	std::vector<std::vector<int>> costSoFar;
	std::vector<Vector2D> pathTiles;
	std::vector<Vector2D> visited;
	std::priority_queue<std::pair<int, Vector2D>, std::vector<std::pair<int, Vector2D>>, std::greater<std::pair<int, Vector2D>> > frontierAStar;


	void ResetPath(Vector2D pos);
	bool IsWalkable(int x, int y);
	int Get(int i, int j)const;
	int MovementCost(int x, int y);
	void ComputePath(int x, int y);

	int Find(std::vector<Vector2D> vector, Vector2D elem);
public:
	PathfindingSystem();
	~PathfindingSystem();

	bool HasFinished();
	bool HasFound();
	void PropagateAStar(ASTAR_HEURISTICS heuristic);

	PathData GetData();
	void DrawPath(PathData* data, Vector2D offset);
	void FindPath(std::vector<int> tiles, int width, int height, std::vector<int> blockedTiles, Vector2D currentPosition, Vector2D targetPosition, int distance, bool forced = false);
	bool Start()override;
	bool Update(float dt)override;
	bool CleanUp()override;
};