#include "pch.h"
#include <vector>
#include "../ContoursDefiner/ContourDefiner.h"
#include "../ContoursDefiner/ContourDefiner.cpp"
#include "../ContoursDefiner/Point.h"
#include "../ContoursDefiner/Point.cpp"
#include "../ContoursDefiner/Contour.h"
#include "../ContoursDefiner/Contour.cpp"
#include "../ContoursDefiner/ImageDataManager.h"
#include "../ContoursDefiner/ImageDataManager.cpp"
#include "../ContoursDefiner/TestImageData.h"
#include "../ContoursDefiner/TestImageData.cpp"


TEST(ConvertToPathTest, TestConvertToPath1) {
  ContourDefiner cd;
  std::vector<Point> points;
  points.push_back(Point(12, 10));
  points.push_back(Point(10, 10));
  points.push_back(Point(10, 9));
  points.push_back(Point(11, 9));
  points.push_back(Point(12, 9));

  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected;
  expected.push_back(Point(10, 10));
  expected.push_back(Point(10, 9));
  expected.push_back(Point(11, 9));
  expected.push_back(Point(12, 9));
  expected.push_back(Point(12, 10));

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}

TEST(ContourDefinerTest, TestDistanceTo) {
  Point p1(1, 1);
  Point p2(2, 2);

  ASSERT_NEAR(sqrt(2), p1.DistanceTo(p2), 0.001);
}

TEST(ContourDefinerTest, TestGetPointValue) {
  TestImageData manager;
  manager.addLine({   0,   0, 255,   0,   0 });
  manager.addLine({   0, 255,   0, 255, 255 });
  manager.addLine({   0, 255,   0,   0, 255 });
  manager.addLine({   0, 255,   0,   0, 255 });
  manager.addLine({   0,   0, 255, 255,   0 });

  ASSERT_EQ(manager.getPointValue(Point(2, 0)), 255);
  ASSERT_EQ(manager.getPointValue(Point(4, 4)), 0);
  ASSERT_EQ(manager.getPointValue(Point(1, 2)), 255);
}

TEST(ContourDefinerTest, TestDefineContourPointsAround) {
  TestImageData manager;
  manager.addLine({ 0,   0, 255,   0,   0 });
  manager.addLine({ 0, 255,   0, 255, 255 });
  manager.addLine({ 0, 255,   0,   0, 255 });
  manager.addLine({ 0, 255,   0,   0, 255 });
  manager.addLine({ 0,   0, 255, 255,   0 });
  ContourDefiner cd(&manager);

  std::vector<Point> actual = cd.defineContourPointsAround(Point(2, 1));

  std::vector<Point> expected;
  expected.push_back(Point(3, 1));
  expected.push_back(Point(1, 2));
  expected.push_back(Point(1, 1));
  expected.push_back(Point(2, 0));

  ASSERT_EQ(actual, expected);
}

TEST(ConvertToPathTest, TestConvertToPath2) {
  ContourDefiner cd;
  std::vector<Point> points;
  points.push_back(Point(3, 4));
  points.push_back(Point(2, 4));
  points.push_back(Point(1, 3));
  points.push_back(Point(1, 2));

  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected;
  expected.push_back(Point(3, 4));
  expected.push_back(Point(2, 4));
  expected.push_back(Point(1, 3));
  expected.push_back(Point(1, 2));

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}

TEST(ConvertToPathTest, TestConvertToPath3) {
  ContourDefiner cd;
  std::vector<Point> points;
  points.push_back(Point(3, 4));
  points.push_back(Point(2, 5));
  points.push_back(Point(2, 3));
  
  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(2, 3),
    Point(3, 4),
    Point(2, 5),
  };
  
  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}

TEST(ContourDefinerTest, TestDefinePossiblePoints) {
  TestImageData manager;
  manager.addLine({ 0,   0, 255,   0,   0 });
  manager.addLine({ 0, 255,   0, 255, 255 });
  manager.addLine({ 0, 255,   0,   0, 255 });
  manager.addLine({ 0, 255,   0,   0, 255 });
  manager.addLine({ 0,   0, 255, 255,   0 });
  ContourDefiner cd(&manager);

  std::vector<Point> actual = cd.definePossiblePoints(Point(2, 3));

  std::vector<Point> expected = {
    Point(3, 3),
    Point(2, 2),
    Point(3, 2),
  };

  ASSERT_EQ(actual, expected);


  actual = cd.definePossiblePoints(Point(2, 1));

  expected = {
    Point(3, 2),
    Point(2, 2),
  };

  ASSERT_EQ(actual, expected);
}

TEST(ContourDefinerTest, TestDefineContour) {
  TestImageData manager;
  manager.addLine({ 0,   0, 255,   0,   0 });
  manager.addLine({ 0, 255,   0, 255, 255 });
  manager.addLine({ 0, 255,   0,   0, 255 });
  manager.addLine({ 0, 255,   0,   0, 255 });
  manager.addLine({ 0,   0, 255, 255,   0 });
  ContourDefiner cd(&manager);

  Contour actual = cd.defineContour(Point(2, 2));

  Contour expected;
  std::vector<Point> points = {
    Point(3, 1),
    Point(4, 1),
    Point(4, 2),
    Point(4, 3),
    Point(3, 4),
    Point(2, 4),
    Point(1, 3),
    Point(1, 2),
    Point(1, 1),
    Point(2, 0),
  };
  expected.addPoints(points);

  ASSERT_EQ(actual, expected);
}


TEST(ContourDefinerTest, TestDefineContourWithStick) {
  TestImageData manager;
  manager.addLine({ 0,   0, 255, 255, 255,   0 });
  manager.addLine({ 0, 255,   0,   0,   0, 255 });
  manager.addLine({ 0, 255,   0,   0,   0, 255 });
  manager.addLine({ 0, 255,   0, 255,   0, 255 });
  manager.addLine({ 0, 255,   0, 255,   0, 255 });
  manager.addLine({ 0,   0, 255, 255, 255,  0 });
  ContourDefiner cd(&manager);

  Contour actual = cd.defineContour(Point(2, 2));

  Contour expected;
  std::vector<Point> points = {
    Point(5, 1),
    Point(5, 2),
    Point(5, 3),
    Point(5, 4),
    Point(4, 5),
    Point(3, 5),
    Point(3, 4),
    Point(3, 3),
    Point(3, 4),
    Point(3, 5),
    Point(2, 5),
    Point(1, 4),
    Point(1, 3),
    Point(1, 2),
    Point(1, 1),
    Point(2, 0),
    Point(3, 0),
    Point(4, 0),
  };
  expected.addPoints(points);

  ASSERT_EQ(actual, expected);
}


TEST(ConvertToPathTest, TestConvertToPath4) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(8, 5),
    Point(8, 6),
    Point(7, 6),
    Point(7, 4),
    Point(8, 4),
  };
  

  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(7, 4),
    Point(8, 4),
    Point(8, 5),
    Point(8, 6),
    Point(7, 6),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, DeepToUp) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(5, 2),
    Point(5, 3),
    Point(3, 3),
    Point(3, 2),
    Point(3, 1),
    Point(4, 1),
    Point(5, 1),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(3, 3),
    Point(3, 2),
    Point(3, 1),
    Point(4, 1),
    Point(5, 1),
    Point(5, 2),
    Point(5, 3),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, DeepToLeft) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(5, 3),
    Point(4, 3),
    Point(3, 3),
    Point(3, 2),
    Point(3, 1),
    Point(4, 1),
    Point(5, 1),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(5, 3),
    Point(4, 3),
    Point(3, 3),
    Point(3, 2),
    Point(3, 1),
    Point(4, 1),
    Point(5, 1),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, DeepToRight) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(5, 2),
    Point(5, 3),
    Point(4, 3),
    Point(3, 3),
    Point(3, 1),
    Point(4, 1),
    Point(5, 1),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(3, 1),
    Point(4, 1),
    Point(5, 1),
    Point(5, 2),
    Point(5, 3),
    Point(4, 3),
    Point(3, 3),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, Corner) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(2, 4),
    Point(2, 3),
    Point(3, 3),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(2, 4),
    Point(2, 3),
    Point(3, 3),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, HorizontalLine) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(4, 3),
    Point(3, 3),
    Point(2, 3),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(4, 3),
    Point(3, 3),
    Point(2, 3),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, ReserseL) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(2, 1),
    Point(2, 2),
    Point(1, 0),
    Point(2, 0),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(1, 0),
    Point(2, 0),
    Point(2, 1),
    Point(2, 2),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, Left5Points) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(3, 3),
    Point(2, 3),
    Point(2, 4),
    Point(2, 5),
    Point(3, 5),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(3, 3),
    Point(2, 3),
    Point(2, 4),
    Point(2, 5),
    Point(3, 5),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(ConvertToPathTest, DeepToDown) {
  ContourDefiner cd;
  std::vector<Point> points = {
    Point(2, 1),
    Point(2, 2),
    Point(1, 2),
    Point(0, 1),
    Point(0, 0),
    Point(2, 0),
  };


  std::vector<Point> actual = cd.convertToPath(points);

  std::vector<Point> expected = {
    Point(2, 0),
    Point(2, 1),
    Point(2, 2),
    Point(1, 2),
    Point(0, 1),
    Point(0, 0),
  };

  std::vector<Point> reversed_expected(expected);
  std::reverse(reversed_expected.begin(), reversed_expected.end());

  ASSERT_TRUE(actual == expected || actual == reversed_expected);
}


TEST(GetNextPointTest, OnlyOnePointPossible) {
  TestImageData manager;
  manager.addLine({ 255, 255, 255});
  manager.addLine({ 255,   0, 255});
  manager.addLine({ 255,   0, 255});
  ContourDefiner cd(&manager);

  Point basePoint(1, 1);
  Point predPoint(1, 2);

  Point actual = cd.getNextPoint(basePoint, predPoint);

  Point expected(1, 2);

  ASSERT_EQ(actual, expected);
}


TEST(GetNextPointTest, VertToUp) {
  TestImageData manager;
  manager.addLine({ 255,   0, 255 });
  manager.addLine({ 255,   0, 255 });
  manager.addLine({ 255,   0, 255 });
  ContourDefiner cd(&manager);

  Point basePoint(1, 1);
  Point predPoint(1, 2);

  Point actual = cd.getNextPoint(basePoint, predPoint);

  Point expected(1, 0);

  ASSERT_EQ(actual, expected);
}


TEST(GetNextPointTest, VertToDown) {
  TestImageData manager;
  manager.addLine({ 255,   0, 255 });
  manager.addLine({ 255,   0, 255 });
  manager.addLine({ 255,   0, 255 });
  ContourDefiner cd(&manager);

  Point basePoint(1, 1);
  Point predPoint(1, 0);

  Point actual = cd.getNextPoint(basePoint, predPoint);

  Point expected(1, 2);

  ASSERT_EQ(actual, expected);
}

