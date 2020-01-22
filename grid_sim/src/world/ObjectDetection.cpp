#include "srgsim/world/ObjectDetection.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"
#include "srgsim/containers/CellPerceptions.h"

#include <SystemConfig.h>
#include <cnc_geometry/Calculator.h>
#include <srgsim/containers/Function.h>

#include <math.h>

namespace srgsim {
    ObjectDetection::ObjectDetection(srgsim::ServiceRobot *robot)
            : Sensor(robot) {
        this->sc = essentials::SystemConfig::getInstance();
        this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
    }

    std::vector<CellPerceptions> ObjectDetection::createPerceptions(World *world) {
        std::map<Coordinate, Cell *> grid = world->getGrid();
        std::vector<CellPerceptions> cellPerceptionsList;

        std::map<Coordinate, Cell *>::iterator it;
        for (it = grid.begin(); it != grid.end(); it++) {
            CellPerceptions cellPerceptions;

            std::cout << it->first  // string (key)
                      << ':'
                      << it->second   // string's value
                      << std::endl;

            cellPerceptions.x = it->first.x;
            cellPerceptions.y = it->first.y;
            std::vector<Object *> objects = it->second->getObjects();

            for (int i = 0; i < objects.size(); i++) {
                Perception *perception = new Perception();
                Object *object = objects.at(i);
                perception->state = object->getState();
                perception->type = object->getType();
                perception->objectID = object->getID();
                perception->x = cellPerceptions.x;
                perception->y = cellPerceptions.y;
                cellPerceptions.perceptions.push_back(*perception);
            }

            cellPerceptionsList.push_back(cellPerceptions);
        }

        return cellPerceptionsList;
    }

    //Finding nearest points
    double calculateDistance(Coordinate a, Coordinate b){
        double x = (b.x - a.x)^2;
        double y = (b.y - a.y)^2;
        double z = x + y;
        return sqrt(z);
    }

    vector<Coordinate> findNearestPoints(Coordinate point, vector<Coordinate> points) {
        vector<Coordinate> nearestPoints;
        double smallestDistance = DBL_MAX;

        for (Coordinate p : points) {
            double currentDistance = calculateDistance(p, point);

            if(currentDistance == smallestDistance){
                nearestPoints.push_back(p);
            } else if (currentDistance < smallestDistance){
                nearestPoints.empty();
                nearestPoints.push_back(p);
                smallestDistance = currentDistance;
            }
        }
        return nearestPoints;
    }

    // Finding points to consider

    bool isInTheRange(Coordinate base, Coordinate point, double range){
        double topLimit = base.y + range;
        double bottomLimit = base.y - range;
        double leftLimit = base.x - range;
        double rightLimit = base.x + range;

        double x = point.x;
        double y = point.y;

        bool vertical =  (y <= topLimit && x >= bottomLimit);
        bool horizontal = (x <= rightLimit && x >= leftLimit);

        return vertical && horizontal;
    }


    vector<Coordinate> findPointsToConsider(Coordinate point, vector<Coordinate> allPoints){
        vector<Coordinate> resultPoints;
        double range = 1.0;

        for(Coordinate p : allPoints){
            if(isInTheRange(point, p, range)){
                resultPoints.push_back(p);
            }
        }

        return resultPoints;
    }


    vector<Coordinate> findNearestNeighbours(Coordinate point, vector<Coordinate> allPoints) {
        vector<Coordinate> pointsToConsider = findPointsToConsider(point, allPoints);
        vector<Coordinate> result = findNearestPoints(point, pointsToConsider);
        return result;
    }

    std::vector<const Cell *> ObjectDetection::collectCells(Coordinate start, Coordinate end, World *world) {
        std::vector<const Cell *> cells;

        std::map<Coordinate, Cell *> grid = world->getGrid();
        const Cell* cell = world->getCell(start);

        // TODO handle infinity values
        // calculate function with start/end point
        Function* function = new Function(start, end);

        // find all transition points between cells
        std:
        vector<Coordinate> results;
        bool forward; // does the ray move forward
        bool down;    // does the ray move

        if (start.x <= end.x) {
            forward = true;
            int x = (int) start.x + 1;
            for (int i = x; i <= end.x; i++) {
                float resultY = function->calculateY(i);
                Coordinate r = new Coordinate(i, resultY);
                results.push_back(r);
                std::cout << "P(" << i << "|" << resultY << ")";
            }
        } else {
            forward = false;
            int x = (int) end.x + 1;
            for (int i = x; i <= start.x; i++) {
                float resultY = function->calculateY(i);
                Coordinate r = new Coordinate(i, resultY);
                results.push_back(r);
                std::cout << "P(" << i << "|" << resultY << ")";
            }
        }

        if (start.y <= end.y) {
            down = false;
            int y = (int) start.y + 1;
            for (int i = y; i <= end.y; i++) {
                float resultX = function->calculateX(i);
                Coordinate r = new Coordinate(resultX, i);
                results.push_back(r);
                std::cout << "Q(" << resultX << "|" << i << ")";
            }
        } else {
            down = true;
            int y = (int) end.y + 1;
            for (int i = y; i <= start.y; i++) {
                float resultX = function->calculateX(i);
                Coordinate r = new Coordinate(resultX, i);
                results.push_back(r);
                std::cout << "Q(" << resultX << "|" << i << ")";
            }
        }

        int x = (int) start.x;
        int y = (int) start.y;
        bool diag = false; // true if the ray hits the corner of a cell
        Cell* currentCell = nullptr;

        /** for (int i = 0; i < results.size(); i++) {
            Coordinate result = results[i];
            // TODO check if test for border cells is correct
            if (result != null && (result.x + 1) > 0 && (result.x - 1) < world->getSizeX()
                && (result.y + 1) > 0 && (result.y - 1 < world->getSizeY())) {

                if (diag) {
                    // Jan: Warum brauchen wir das?
                    diag = false;
                    continue;
                }

                if (result.x % 1 == 0) {
                    if (result.x % 1 == 0) {
                        // diagonal transition
                        if (down && forward) {
                            currentCell = world->getTile(++x, --y);
                        } else if (down && !forward) {
                            currentCell = grid.getTile(--x, --y);
                        } else if (!down && forward) {
                            currentCell = grid.getTile(++x, ++y);
                        } else {
                            currentCell = grid.getTile(--x, ++y);
                        }
                        diag = true;
                    } else {
                        // right/left side transition
                        if (forward) {
                            currentCell = grid.getTile(++x, y);
                        } else {
                            currentCell = grid.getTile(--x, y);
                        }
                    }
                } else {
                    // top/bottom transition
                    if (down) {
                        currentCell = grid.getTile(x, --y);
                    } else {
                        currentCell = grid.getTile(x, ++y);
                    }
                }
                std::cout << "result " << result.x << "," << result.y;
                std::cout << "Current cell " << currentCell;
            }

            if (currentCell->hasObject()) {
                std::cout << "Object found on current cell!";
                break;
            }
        } **/

        // TODO cleanup output messages

        // TODO sort vector
        return cells;
    }
} // namespace srgsim
