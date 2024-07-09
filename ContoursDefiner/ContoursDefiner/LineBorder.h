#pragma once
#include "Contour.h"
#include "BresenhamLine.h"

#include <vector>

class LineBorder
{

  Contour& owner;
  int fromIndex;
  int toIndex;

public:

  LineBorder(Contour& owner, int fromIndex, int toIndex);

  LineBorder& operator=(const LineBorder& other);
  bool operator==(const LineBorder& other) const;
  bool operator<(const LineBorder& other) const;

  Contour& getOwner();
  int getFromIndex() const;
  int getToIndex() const;

  virtual Point fromPoint() const;
  virtual Point toPoint() const;

  virtual int getNextIdx(int curIndex, int step = 1) const;
  virtual Point getPoint(int index) const;
  virtual std::vector<Point> getPoints() const;

  void replaceBorderWith(const LineBorder& line);

  void reduceEnds(int countPoints);

  virtual int size() const;

  double euclideanLength() const;
  double squareLength() const;

  static void reduceEndsWhileApproxTo(LineBorder& left, LineBorder& right, int maxDeleted);

  LineBorder inverse() const;

  void agreeWith(const LineBorder& line);

  bool isClockwise() const;

  int minStep(int from, int to) const;
  int distance(int from, int to) const;


  bool isInsideBorder(int idx) const;
  bool canUnionWith(const LineBorder& other) const;

  void unionWith(const LineBorder& other);

private:

  void deleteContourLine();

  void insertLine(const LineBorder& line, int startIdx, int step = 1);


};

