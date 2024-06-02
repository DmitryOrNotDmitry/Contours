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

  Contour& getOwner();
  int getFromIndex() const;
  int getToIndex() const;

  virtual Point fromPoint() const;
  virtual Point toPoint() const;

  virtual int getNextIdx(int curIndex, int step) const;
  virtual Point getPoint(int index) const;

  void replaceBorderWith(const LineBorder& line);

  void reduceEnds(int countPoints);

  virtual int size() const;

  double euclideanLength() const;

  static void reduceEndsWhileApproxTo(LineBorder& left, LineBorder& right, int maxDeleted);

  LineBorder inverse() const;

private:

  void deleteContourLine();

  void insertLine(const LineBorder& line, int startIdx, int step = 1);

};

