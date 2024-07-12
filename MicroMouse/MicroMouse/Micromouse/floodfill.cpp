#include "globals.h"
#include "floodfill.h"

#include "src/CircularBufferQueue/CircularBufferQueue.h"
CircularBufferQueue floodQueue(512);  // This queue stores the cells that need to be flooded

static bool isNeighbourValid(uint8_t location, uint8_t direction);
static uint8_t getTargetAbsoluteDirection(uint8_t target);
static uint8_t getTargetRelativeDirection(uint8_t target);
static bool isDestination(uint8_t location);
static bool isEnclosed(uint8_t location);
static bool isTunnel(uint8_t location);

uint8_t currentCell, targetCell;
uint8_t leftDir, currentDir, rightDir, nextLeftDir, nextDir, nextRightDir;

uint8_t readingCellLoc, readingCellDistance, readingCellScore, minNeighbourDistance, targetRelativeDirection, targetScore;
uint8_t distanceFromTarget = 1;

short cellDirectionAddition[4] = { -rows, 1, rows, -1 };  // The location of a neighbouring cell can be obtained using the values in this dictionary
uint8_t targetScoreFromDirection[4] = { 0, 1, 2, 1 };

void flood() {
  floodQueue.enqueue(currentCell);
  while (!floodQueue.isEmpty()) {
    readingCellLoc = *floodQueue.dequeue();
    if (isEnclosed(readingCellLoc)) continue;
    readingCellDistance = floodArray[readingCellLoc].flood;
    minNeighbourDistance = 255;
    for (uint8_t i = 0; i < 4; i++) {
      minNeighbourDistance = minimum(minNeighbourDistance, getNeighbourDistance(readingCellLoc, i));
    }
    if (minNeighbourDistance != readingCellDistance - 1) {
      floodArray[readingCellLoc].flood = minNeighbourDistance + 1;
      for (uint8_t i = 0; i < 4; i++) {
        if (isNeighbourValid(readingCellLoc, i)) {
          if (!isDestination(getNeighbourLocation(readingCellLoc, i))) {
            floodQueue.enqueue(getNeighbourLocation(readingCellLoc, i));
          }
        }
      }
    }
  }
}

void updateTargetCell() {
  minNeighbourDistance = getNeighbourDistance(currentCell, 0);
  targetScore = 3;
  for (uint8_t i = 0; i < 4; i++) {
    if (!wallExists(currentCell, i)) {
      readingCellLoc = getNeighbourLocation(currentCell, i);
      readingCellDistance = getNeighbourDistance(currentCell, i);
      readingCellScore = targetScoreFromDirection[getTargetRelativeDirection(readingCellLoc)];
      if ((readingCellDistance < minNeighbourDistance) || ((readingCellDistance == minNeighbourDistance) && (readingCellScore < targetScore))) {
        minNeighbourDistance = readingCellDistance;
        targetScore = readingCellScore;
        targetCell = readingCellLoc;
      }
    }
  }

  targetRelativeDirection = getTargetRelativeDirection(targetCell);

  updateDirection(&nextLeftDir, targetRelativeDirection);
  updateDirection(&nextDir, targetRelativeDirection);
  updateDirection(&nextRightDir, targetRelativeDirection);

  distanceFromTarget = 1;

  while (isNeighbourValid(targetCell, nextDir)) {
    readingCellLoc = getNeighbourLocation(targetCell, nextDir);
    if (isTunnel(readingCellLoc) && floodArray[readingCellLoc].flood == floodArray[targetCell].flood - 1) {
      targetCell = readingCellLoc;
      distanceFromTarget++;
    } else break;
  }
}

void goToTargetCell() {
  if (targetRelativeDirection == north) {
  } else if (targetRelativeDirection == east) {
    turn(90, 70);
  } else if (targetRelativeDirection == south) {
    turn(180, 70);
  } else if (targetRelativeDirection == west) {
    turn(-90, 70);
  }
  moveForward(distanceFromTarget, 100);

  updateDirection(&leftDir, targetRelativeDirection);
  updateDirection(&currentDir, targetRelativeDirection);
  updateDirection(&rightDir, targetRelativeDirection);
  
  currentCell = targetCell;
}

void updateWalls() {
  readWall();
  if (sensorValue[leftSensor] > wallThreshold) {
    markWall(currentCell, leftDir);
    if (isNeighbourValid(currentCell, leftDir)) {
      markWall(getNeighbourLocation(currentCell, leftDir), (leftDir + 2) % 4);
    }
  }
  if (sensorValue[centreSensor] > wallThreshold) {
    markWall(currentCell, currentDir);
    if (isNeighbourValid(currentCell, currentDir)) {
      markWall(getNeighbourLocation(currentCell, currentDir), (currentDir + 2) % 4);
    }
    alignFront();
  }
  if (sensorValue[rightSensor] > wallThreshold) {
    markWall(currentCell, rightDir);
    if (isNeighbourValid(currentCell, rightDir)) {
      markWall(getNeighbourLocation(currentCell, rightDir), (rightDir + 2) % 4);
    }
  }
}

static bool isNeighbourValid(uint8_t location, uint8_t direction) {
  if (direction == north) return delineariseRow(location) > 0;
  else if (direction == east) return delineariseCol(location) < (cols - 1);
  else if (direction == south) return delineariseRow(location) < (rows - 1);
  else if (direction == west) return delineariseCol(location) > 0;
}

static uint8_t getTargetAbsoluteDirection(uint8_t target) {
  short diff = (short)target - (short)currentCell;
  if (diff == -rows) return north;
  if (diff == 1) return east;
  if (diff == rows) return south;
  if (diff == -1) return west;
}

static uint8_t getTargetRelativeDirection(uint8_t target) {
  return (getTargetAbsoluteDirection(target) + 4 - currentDir) % 4;
}

static bool isDestination(uint8_t location) {
  return floodArray[location].flood == 0;
}

static bool isEnclosed(uint8_t location) {
  // 15 is 00001111 in binary, which means that there are walls in 4 all 4 directions of the cell
  return floodArray[location].neighbours == 15;
}

static bool isTunnel(uint8_t location) {
  return (!wallExists(location, nextDir)) && wallExists(location, nextLeftDir) && wallExists(location, nextRightDir) && floodArray[location].visited;
}

void initialise() {
  currentCell = startCell;
  currentDir = startDir;
  leftDir = (currentDir + 3) % 4;
  rightDir = (currentDir + 1) % 4;
  nextLeftDir = leftDir;
  nextDir = currentDir;
  nextRightDir = rightDir;
}