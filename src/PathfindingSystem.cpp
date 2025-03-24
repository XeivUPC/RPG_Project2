#include "PathfindingSystem.h"
#include "Engine.h"
#include "Debug.h"
#include "LevelManager.h"
#include "Textures.h"

void PathfindingSystem::ResetPath(Vector2D pos)
{

    while (!frontierAStar.empty()) {
        frontierAStar.pop();
    }

    visited.clear(); //Clear the visited list
    breadcrumbs.clear(); //Clear the breadcrumbs list
    pathTiles.clear(); //Clear the pathTiles list

    frontierAStar.push(std::make_pair(0, pos)); //AStar
    visited.push_back(pos);
    breadcrumbs.push_back(pos);

    costSoFar = std::vector<std::vector<int>>(mapWidth, std::vector<int>(mapHeight, 0));
}

bool PathfindingSystem::IsWalkable(int x, int y)
{
    bool isWalkable = true;

    if (x >= 0 && y >= 0 && x < mapWidth && y < mapHeight) {
        int gid = Get(x, y);
        for (size_t i = 0; i < blockedTiles.size(); i++)
        {
            if (gid == blockedTiles.at(i))
            {
                isWalkable = false;
                break;
            }
        }
    }
    else
        isWalkable = false;


    return isWalkable;
}

int PathfindingSystem::Get(int i, int j) const
{
    return mapTiles[(j * mapWidth) + i];
}

int PathfindingSystem::MovementCost(int x, int y)
{
    int ret = -1;

    if ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight))
    {
        int gid = Get(x, y);
        if (gid == -1) {
            ret = 5;
        }
        else ret = 1;
    }

    return ret;
}

void PathfindingSystem::ComputePath(int x, int y)
{
    pathTiles.clear();
    Vector2D currentTile = Vector2D(x, y);
    pathTiles.push_back(currentTile);
    int index = Find(visited, currentTile);

    while ((index >= 0) && (currentTile != breadcrumbs[index]))
    {
        currentTile = breadcrumbs[index];
        pathTiles.push_back(currentTile);
        index = Find(visited, currentTile);
    }
}

void PathfindingSystem::PropagateAStar(ASTAR_HEURISTICS heuristic)
{

    bool foundDestination = false;
    if (frontierAStar.size() > 0) {
        Vector2D frontierTile = frontierAStar.top().second;

        if (frontierTile == target) {
            foundDestination = true;
            path_status = FOUND;
            ComputePath(frontierTile.getX(), frontierTile.getY());
        }
    }
    if (frontierAStar.empty())path_status = NO_PATH;
    if (frontierAStar.size() > 0 && !foundDestination) {

        Vector2D frontierTile = frontierAStar.top().second;
        frontierAStar.pop();

        std::list<Vector2D> neighbors;
        if (IsWalkable(frontierTile.getX() + 1, frontierTile.getY())) {
            neighbors.push_back(Vector2D((int)frontierTile.getX() + 1, (int)frontierTile.getY()));
        }
        if (IsWalkable(frontierTile.getX(), frontierTile.getY() + 1)) {
            neighbors.push_back(Vector2D((int)frontierTile.getX(), (int)frontierTile.getY() + 1));
        }
        if (IsWalkable(frontierTile.getX() - 1, frontierTile.getY())) {
            neighbors.push_back(Vector2D((int)frontierTile.getX() - 1, (int)frontierTile.getY()));
        }
        if (IsWalkable(frontierTile.getX(), frontierTile.getY() - 1)) {
            neighbors.push_back(Vector2D((int)frontierTile.getX(), (int)frontierTile.getY() - 1));
        }

        for (const auto& neighbor : neighbors) {

            int g = costSoFar[(int)frontierTile.getX()][(int)frontierTile.getY()] + MovementCost((int)neighbor.getX(), (int)neighbor.getY());

            int h = 0;

            switch (heuristic)
            {
            case ASTAR_HEURISTICS::MANHATTAN:
                h = neighbor.distanceMahattan(target);
                break;
            case ASTAR_HEURISTICS::EUCLIDEAN:
                h = neighbor.distanceEuclidean(target);
                break;
            case ASTAR_HEURISTICS::SQUARED:
                h = neighbor.distanceSquared(target);
                break;
            }

            int f = g + h;

            if (std::find(visited.begin(), visited.end(), neighbor) == visited.end() || g < costSoFar[neighbor.getX()][neighbor.getY()]) {
                costSoFar[neighbor.getX()][neighbor.getY()] = g;
                frontierAStar.push(std::make_pair(f, neighbor));
                visited.push_back(neighbor);
                breadcrumbs.push_back(frontierTile);
            }
        }
    }
}

int PathfindingSystem::Find(std::vector<Vector2D> vector, Vector2D elem)
{
    int index = 0;
    bool found = false;
    for (const auto& e : vector) {
        if (e == elem) {
            found = true;
            break;
        }
        index++;
    }

    if (found) return index;
    else return -1;
}

PathfindingSystem::PathfindingSystem()
{
}

PathfindingSystem::~PathfindingSystem()
{
}

bool PathfindingSystem::HasFinished()
{
    return path_status != SEARCHING;
}

bool PathfindingSystem::HasFound()
{
    return path_status == FOUND;
}

PathData PathfindingSystem::GetData()
{
    PathData pathData;
    pathData.pathTiles = pathTiles;
    pathData.visitedTiles = visited;
    pathData.frontierAStar = frontierAStar;
    return pathData;
}

void PathfindingSystem::DrawPath(PathData* data, Vector2D offset)
{
    if (!Engine::GetInstance().debug->HasDebug(1))
        return;
    Vector2D point;

    // Draw visited
    Engine::GetInstance().render->LockLayer(Render::Layer7);
    for (const auto& pathTile : data->visitedTiles) {
        Vector2D pathTileWorld = { (float)METERS_TO_PIXELS(pathTile.getX()+ offset.getX()), (float)METERS_TO_PIXELS(pathTile.getY()+ offset.getY()) };
        SDL_Rect rect = { 0,0,16,16 };
        Engine::GetInstance().render.get()->DrawTexture(pathTex, pathTileWorld.getX(), pathTileWorld.getY(), SDL_FLIP_NONE, &rect);
    }


    // Create a copy of the queue to iterate over
    std::priority_queue<std::pair<int, Vector2D>, std::vector<std::pair<int, Vector2D>>, std::greater<std::pair<int, Vector2D>> > frontierAStarCopy = data->frontierAStar;

    // Iterate over the elements of the frontier copy
    while (!frontierAStarCopy.empty()) {

        //Get the first element of the queue
        Vector2D frontierTile = frontierAStarCopy.top().second;
        //Get the position of the frontier tile in the world
        Vector2D pathTileWorld = { (float)METERS_TO_PIXELS(frontierTile.getX() + offset.getX()), (float)METERS_TO_PIXELS(frontierTile.getY() + offset.getY()) };
        //Draw the frontier tile
        SDL_Rect rect = { 32,0,16,16 };
        Engine::GetInstance().render.get()->DrawTexture(pathTex, pathTileWorld.getX(), pathTileWorld.getY(), SDL_FLIP_NONE, &rect);
        //Remove the front element from the queue
        frontierAStarCopy.pop();
    }

    // Draw path
    for (const auto& pathTile : data->pathTiles) {
        SDL_Rect rect = { 16,0,16,16 };

        Vector2D pathTileWorld = { (float)METERS_TO_PIXELS(pathTile.getX() + offset.getX()), (float)METERS_TO_PIXELS(pathTile.getY() + offset.getY()) };
        Engine::GetInstance().render.get()->DrawTexture(pathTex, pathTileWorld.getX(), pathTileWorld.getY(), SDL_FLIP_NONE, &rect);
      
    }
    Engine::GetInstance().render->UnlockLayer();
}

void PathfindingSystem::FindPath(std::vector<int> tiles, int width, int height, std::vector<int> blockedTiles, Vector2D currentPosition, Vector2D targetPosition, int distance, bool forced)
{
    mapTiles = tiles;
    mapWidth = width;
    mapHeight = height;
    this->blockedTiles = blockedTiles;

    //WorldToTile
    Vector2D currentTile = { (float)((int)currentPosition.getX()), (float)((int)currentPosition.getY()) };

    target = { (float)((int)targetPosition.getX()), (float)((int)targetPosition.getY()) };
    path_status = SEARCHING;
    ResetPath(currentTile);
    if((abs(currentPosition.getX() - targetPosition.getX()) + abs(currentPosition.getY() - targetPosition.getY()) > distance) && !forced)
        path_status = NO_PATH;
}

bool PathfindingSystem::Start()
{
    pathTex = Engine::GetInstance().textures.get()->GetTexture("PathfindingTexture");
    return true;
}

bool PathfindingSystem::Update(float dt)
{
    return true;
}

bool PathfindingSystem::CleanUp()
{
    return true;
}